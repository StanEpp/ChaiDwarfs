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
#include "DwarfComponentRegistrations.hpp"
#include "Entity.hpp"

#include <chaiscript\chaiscript.hpp>
#include <chaiscript\utility\utility.hpp>

namespace CDwarfs {

  template<class TCompType>
  class ECSFactory{
    
    /*
    * Base class for ComponentHolder. Contains the component type ID of the component it holds and provides  
    * the "createComponent" interface to create the specific component.
    */
    struct ComponentHolder {
      ComponentUUID componentTypeID;
      virtual TCompType* createComponent() = 0;
      virtual ~ComponentHolder(){}
    };

    /*
    * Holds the arguments provided during object definition and provides ability to create the component with the given arguments.
    * When chaiscript calls e.g. "addComp_TouchValue(42)" during object definition then the argument "42" will be saved in this struct.
    * When the component "Comp" will be created the arguments will be passed to the constructor of the component.
    *
    * \tparam Comp The component type
    * \tparam ArgTypes Template parameter pack that contains the types of the arguments
    */
    template <class TComp, class... TArgTypes>
    struct ComponentHolder_ : public ComponentHolder {
      
      ComponentHolder_(TArgTypes&&... values) : initArguments(std::forward<TArgTypes>(values)...) {
        componentTypeID = TComp::componentTypeID;
      }

      TCompType* createComponent() override {
        auto f = [](auto&&... params) { return new TComp(std::forward<decltype(params)>(params)...); };
        return std::apply(f, initArguments);
      }

      std::tuple<TArgTypes...> initArguments;
    };

    /* 
    * \brief ComponentCascading is used to provide the cascading functionality in defining a new object.
    * 
    * For example: "factory.defineObject("Diamond").addComp_TouchValue().addComp_TouchValue()"
    */
    struct ComponentCascading {
      template<class TComp, class... TArgs>
      ComponentCascading& addComponent(TArgs&... params) {
        
        // TODO: Linear search should be avoided.
        auto it = std::find_if(componentVec->begin(), componentVec->end(), [](auto& compHolder) {
          return compHolder->componentTypeID == TComp::componentTypeID;
        });

        if (it != componentVec->end()) {
          std::cerr << "Trying to add duplicate component " << typeid(TComp).name() << " for object definition of \"" << objectName << "\"!\n";
        }

        componentVec->push_back(new ComponentHolder_<TComp, TArgs...>(std::forward<TArgs>(params)...));
        return *this;
      }

      std::vector<ComponentHolder*>* componentVec;
      std::string objectName;
    };

  public:

    ECSFactory() : m_chaiDefModule(std::make_shared<chaiscript::Module>()), m_chaiCreateModule(std::make_shared<chaiscript::Module>()) {
      m_chaiDefModule->add(chaiscript::fun(&ECSFactory<TCompType>::defineObject), "defineObject");
      m_chaiCreateModule->add(chaiscript::fun(&ECSFactory<TCompType>::createObject), "createObject");
    }
    // Rule of five!
    ECSFactory(ECSFactory&) = delete;
    ECSFactory(ECSFactory&&) = delete;
    ECSFactory& operator=(ECSFactory&) = delete;
    ECSFactory& operator=(ECSFactory&&) = delete;

    ~ECSFactory() {
      // Delete all components and component holder!
      for (auto& objects : m_objects) {
        for (auto& comp : objects.second) {
          if (comp.second) delete comp.second;
        }
      }

      for (auto& objects : m_objectDefinitions) {
        for (auto ptr : objects.second) {
          if (ptr) delete ptr;
        }
      }
    }

    /*
    * \brief Loads a chaiscript file to define objects.
    * 
    * \param path The path to the chaiscript file which contains the object definitions.
    */
    void loadObjectDefinitions(const std::string& path) {
      chaiscript::ChaiScript chai;
      chai.add(chaiscript::var(std::ref(*this)), "factory");
      chai.add(m_chaiDefModule);
      chai.add(m_chaiCreateModule);
      chai.eval_file(path);
    }
    
    /*
    * \brief Registers a component in the factory which can be used to define objects in chaiscript.
    *
    * Example: "registerComponent<TouchValue>("TouchVal")" creates and exposes the function "addComp_TouchVal" to chaiscript.
    *          "registerComponent<TouchValue, const int>("TouchVal")" creates and exposes two functions to chaiscript, "addComp_TouchVal()" 
    *          and "addComp_TouchVal(int)".
    * \param name The name exposed in chaiscript. Will generate the function "addComp_[name]". 
    * \tparam T The component type to be registered
    * \tparam Args the types of the component's constructor parameters
    */
    template<class TComp, class... TArgs>
    void registerComponent(const std::string &name) {
      static_assert(std::is_base_of_v<TCompType, TComp>, "Component trying to register in a factory has wrong base class!");

      if (m_componentRegistrations.find(TComp::componentTypeID) != m_componentRegistrations.end()) {
        std::cerr << "Component \"" << name << "\" has already been registered!\n";
        return;
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

      m_chaiCreateModule->add(chaiscript::fun(&ECSFactory<TCompType>::getAllEntitiesWithComponent<TComp>), "getAllEntitiesWith_" + name);
      m_chaiCreateModule->add(chaiscript::fun(&ECSFactory<TCompType>::hasComponent<TComp>), "hasComponent_" + name);
      m_chaiCreateModule->add(chaiscript::fun(&ECSFactory<TCompType>::getComponent<TComp>), "getComponent_" + name);
      m_chaiCreateModule->add(chaiscript::fun(&ECSFactory<TCompType>::getAllComponents<TComp>), "getAllComponents_" + name);
      m_chaiCreateModule->add(chaiscript::fun(&ECSFactory<TCompType>::addComponent<TComp>), "addComponent_" + name);
      m_chaiCreateModule->add(chaiscript::fun(&ECSFactory<TCompType>::deleteComponent<TComp>), "deleteComponent_" + name);
      m_chaiCreateModule->add(chaiscript::user_type<TComp>(), name);
      objDef::registerComponent<TComp>(m_chaiCreateModule);
    }

    /*
    * \brief Creates a new object definition with the given name.
    *
    * Create a new definition for an object with the given name and returns a component cascading 
    * object which allows to add components to the newly creted object definition.
    * \param name The name of the newly created object definition.
    * \return Component cascading object to do the following "defineObject("Diamond").addComp<TouchValue>()" in C++
    *         or e.g. "defineObject("Diamond").addComp_TouchValue()" in chaiscript
    */
    ComponentCascading& defineObject(const std::string &name) {
      if (m_objectDefinitions.find(name) != m_objectDefinitions.end()) {
        throw std::runtime_error("Object with name \"" + name + "\" is already defined!");
      }
      auto newDef = m_objectDefinitions.insert({ name, {} });
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
    EntityID::UUID createObject(const std::string &name) {
      auto definition = m_objectDefinitions.find(name);
      if ( definition == m_objectDefinitions.end()) {
        throw std::runtime_error("Object with name \"" + name + "\" has not been defined and cannot be created!");
      }

      auto entityID = EntityID::getID();
      auto newEntity = m_objects.insert({ entityID, {} });
      if(!newEntity.second) throw std::runtime_error("Couldn't create new entity for object \"" + name + "\"!\n");

      auto& compHolder = definition->second;
      auto& newComponents = newEntity.first->second;
      newComponents.reserve(compHolder.size());

      for (size_t i = 0; i < compHolder.size(); ++i) {
        newComponents[compHolder[i]->componentTypeID] = compHolder[i]->createComponent();
      }

      return entityID;
    }

    template<class TComp>
    std::vector<EntityID::UUID> getAllEntitiesWithComponent() const {
      std::vector<EntityID::UUID> retVec;
      for (auto& ent : m_objects) {
        if (ent.second.find(TComp::componentTypeID) != ent.second.end()) retVec.push_back(ent.first);
      }
      return retVec;
    }

    template<class TComp>
    bool hasComponent(EntityID::UUID ID) const {
      auto it = m_objects.find(ID);
      if (it == m_objects.end()) return false;

      auto itC = it->second.find(TComp::componentTypeID);
      if (itC == it->second.end()) return false;
      return true;
    }

    template<class TComp>
    TComp* getComponent(EntityID::UUID ID) const {
      auto it = m_objects.find(ID);
      if (it == m_objects.end()) return nullptr;
      
      auto itC = it->second.find(TComp::componentTypeID);
      if (itC == it->second.end()) return nullptr;
      return static_cast<TComp*>(itC->second);
    }

    template<class TComp>
    std::vector<TComp*> getAllComponents() const {
      std::vector<TComp*> retVec;
      for (auto& comps : m_objects) {
        auto it = comps.second.find(TComp::componentTypeID);
        if (it != comps.second.end()) retVec.push_back(static_cast<TComp*>(it->second));
      }
      return retVec;
    }

    template<class TComp>
    TComp* addComponent(EntityID::UUID ID) {
      auto it = m_objects.find(ID);
      if (it == m_objects.end()) return nullptr;
      auto itC = it->second.find(TComp::componentTypeID);
      TComp* ret = nullptr;
      if (itC == it->second.end()) {
        ret = new TComp();
        it->second[TComp::componentTypeID] = ret;
      }
      else {
        ret = static_cast<TComp*>(itC->second);
      }
      return ret;
    }

    template<class TComp>
    void deleteComponent(EntityID::UUID ID) {
      auto it = m_objects.find(ID);
      if (it == m_objects.end()) return;

      auto itC = it->second.find(TComp::componentTypeID);
      if (itC == it->second.end()) return;
      if (itC->second) delete itC->second;
      it->second.erase(itC);
    }

  private:
    chaiscript::ModulePtr  m_chaiDefModule, m_chaiCreateModule;
    ComponentCascading     m_compCascading;

    std::unordered_set<ComponentUUID>                                m_componentRegistrations;
    std::unordered_map<std::string, std::vector<ComponentHolder*>>   m_objectDefinitions;
    // TODO: Think whether weak_ptr for BaseComponent makes sense.
    std::unordered_map<EntityID::UUID, std::unordered_map<ComponentUUID, TCompType*> >  m_objects;
  };

}

#endif // !_ECSFACTORY_HPP_