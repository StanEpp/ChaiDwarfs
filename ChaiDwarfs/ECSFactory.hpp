/*
*  Copyright(c) 2016 - 2017 Stanislaw Eppinger
*  Scripting based game called ChaiDwarfs
*
*  This file is part of ChaiDwarfs.
*
*  ChaiDwarfs is free software : you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _ECSFACTORY_HPP_
#define _ECSFACTORY_HPP_

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <typeinfo>
#include <bitset>

#include "ObjectComponentRegistrations.hpp"
#include "Entity.hpp"

#include <chaiscript\chaiscript.hpp>
#include <chaiscript\utility\utility.hpp>

namespace cdwarfs {

  class ECSFactory{
    /*
    * Base class for ComponentHolder. Contains the component type ID of the component it holds and provides
    * the "createComponent" interface to create the specific component.
    */
    struct ComponentHolder {
      ComponentUUID componentTypeID;
      virtual BaseComponent* createComponent() const = 0;
      virtual ~ComponentHolder(){}
    };

    /*
    * Holds the arguments provided during object definition and provides ability to create the component with the given arguments.
    * When chaiscript calls e.g. "addComp_TouchValue(42)" during object definition then the argument "42" will be saved in this struct.
    * When the component "Comp" will be created the arguments will be passed to the constructor of the component.
    *
    * \tparam TComp The component type
    * \tparam TArgTypes Template parameter pack that contains the types of the arguments
    */
    template <class TComp, class... TArgTypes>
    struct ComponentHolder_ : public ComponentHolder {
      
      ComponentHolder_(const TArgTypes&... values) : initArguments(values...) {
        componentTypeID = TComp::componentTypeID;
      }

      BaseComponent* createComponent() const override {
        auto f = [](const auto&... params) { 
          return new TComp(params...); 
        };
        return std::apply(f, initArguments);
      }

      std::tuple<typename std::remove_reference_t<TArgTypes>...> initArguments;
    };

    /* 
    * \brief ComponentCascading is used to provide the cascading functionality in defining a new object.
    * 
    * For example: "factory.defineObject("Diamond").addComp_TouchValue().addComp_TouchValue()"
    */
    struct ComponentCascading {
      template<class TComp, class... TArgs>
      ComponentCascading& addComponent(const TArgs&... params) {
        
        // TODO: Linear search should be avoided.
        const auto it = std::find_if(componentVec->cbegin(), componentVec->cend(), [](const auto& compHolder) {
          return compHolder->componentTypeID == TComp::componentTypeID;
        });

        if (it != componentVec->cend()) {
          std::cerr << "Trying to add duplicate component " << typeid(TComp).name() << " for object definition of \"" << objectName << "\"!\n";
        }
        else {
          componentVec->push_back(std::make_shared<ComponentHolder_<TComp, TArgs...>>(params...));
        }

        return *this;
      }

      std::vector<std::shared_ptr<ComponentHolder>>* componentVec;
      std::string objectName;
    };

  public:
    using ComponentList = std::unordered_map<ComponentUUID, BaseComponent*>;

    ECSFactory() : m_chaiDefModule(std::make_shared<chaiscript::Module>()), m_chaiCreateModule(std::make_shared<chaiscript::Module>()) {
      m_chaiDefModule->add(chaiscript::fun(&ECSFactory::defineObject), "defineObject");
      objDef::registerConversions(m_chaiCreateModule);
      //m_chaiCreateModule->add(chaiscript::fun(&ECSFactory::createObject), "createObject");
    }
    // Rule of five!
    ECSFactory(ECSFactory&) = delete;
    ECSFactory(ECSFactory&&) = delete;
    ECSFactory& operator=(ECSFactory&) = delete;
    ECSFactory& operator=(ECSFactory&&) = delete;

    ~ECSFactory() {}

    /*
    * \brief Registers a component in the factory which can be used to define objects in chaiscript.
    *
    * Example: "registerComponent<TouchValue>("TouchVal")" creates and exposes the function "addComp_TouchVal" to chaiscript.
    *          "registerComponent<TouchValue, const int>("TouchVal")" creates and exposes two functions to chaiscript, "addComp_TouchVal()" 
    *          and "addComp_TouchVal(int)".
    * \param name The name exposed in chaiscript. Will generate the function "addComp_[name]". 
    * \tparam TComp The component type to be registered
    * \tparam TArgs the types of the component's constructor parameters
    * \return Signals whether registrations was successful
    */
    template<class TComp, class... TArgs>
    bool registerComponent(const std::string &name) {
      static_assert(std::is_base_of_v<BaseComponent, TComp>, "Component trying to register in ECS factory has wrong base class!");

      if (m_componentRegistrations.find(TComp::componentTypeID) != m_componentRegistrations.end()) {
        std::cerr << "Component \"" << name << "\" has already been registered!\n";
        return false;
      }

      m_componentRegistrations.insert(TComp::componentTypeID);

      m_chaiDefModule->add(chaiscript::fun(&ComponentCascading::addComponent<TComp>), "addComp_" + name);
      
      // TODO: Exchange with C++17's 'constexpr if' when visual studio starts supporting it.
      __pragma(warning(push))
      __pragma(warning(disable:4127))
      if (sizeof...(TArgs) != 0) {
      __pragma(warning(pop))
        m_chaiDefModule->add(chaiscript::fun(&ComponentCascading::addComponent<TComp, TArgs...>), "addComp_" + name);
      }

      m_chaiCreateModule->add(chaiscript::user_type<TComp>(), name);
      objDef::registerComponent<TComp>(m_chaiCreateModule);
      return true;
    }

    /*
    * \brief Creates a new object definition with the given name.
    *
    * Create a new definition for an object with the given name and returns a component cascading 
    * object which allows to add components to the newly created object definition.
    * \param name The name of the newly created object definition.
    * \return Component cascading object to do the following "defineObject("Diamond").addComp<TouchValue>()" in C++
    *         or e.g. "defineObject("Diamond").addComp_TouchValue()" in chaiscript
    */
    ComponentCascading& defineObject(const std::string &name) {
      if (m_objectDefinitions.find(name) != m_objectDefinitions.end()) {
        throw std::runtime_error("Object with name \"" + name + "\" is already defined!");
      }
      const auto newDef = m_objectDefinitions.insert({ name, {} });
      if (!newDef.second) throw std::runtime_error("Couldn't create definition for object \"" + name + "\"!\n");
      m_compCascading.componentVec = &(newDef.first->second);
      m_compCascading.objectName = name;
      return m_compCascading;
    }

    /*
    * \brief Instantiates a new object based according the object defintion with the given name if it exists.
    *
    * \param name The name of object definition which should be instantiated.
    * \return The ID of the newly created Entity
    */
    ComponentList createObject(const std::string &name) {
      const auto definition = m_objectDefinitions.find(name);
      if (definition == m_objectDefinitions.end()) {
        throw std::runtime_error("Object with name \"" + name + "\" has not been defined and cannot be created!");
      }

      const auto& compHolder = definition->second;
      ComponentList compList;

      for (size_t i = 0; i < compHolder.size(); ++i) {
        compList[compHolder[i]->componentTypeID] = compHolder[i]->createComponent();
      }

      return compList;
    }

    const chaiscript::ModulePtr& getChaiDefModule() const {
      return m_chaiDefModule;
    }

    const chaiscript::ModulePtr& getChaiCreateModule() const {
      return m_chaiCreateModule;
    }

  private:
    chaiscript::ModulePtr  m_chaiDefModule, m_chaiCreateModule;
    ComponentCascading     m_compCascading;

    std::unordered_set<ComponentUUID>                                                m_componentRegistrations;
    std::unordered_map<std::string, std::vector<std::shared_ptr<ComponentHolder>>>   m_objectDefinitions;
  };

}

#endif // !_ECSFACTORY_HPP_