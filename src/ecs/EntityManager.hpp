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
#pragma once

#include "ECSFactory.hpp"
#include "ComponentsAI.hpp"
#include "Components.hpp"

namespace cdwarfs
{

class EntityManager
{
public:
    EntityManager() : m_chaiModule(std::make_shared<chaiscript::Module>()) {}

    ~EntityManager()
    {
        // Delete all components and component holder!
        for (auto& objects : m_objects) {
            for (auto& comp : objects.second) {
                if (comp.second) delete comp.second;
            }
        }
    }

    /*
    * \brief Initializes the EntityManager. All components need to be registered here!
    */
    void init()
    {
        registerComponent<comp::TouchValue, int>("TouchValue");
        registerComponent<comp::TouchHeal, int>("TouchHeal");
        registerComponent<comp::TouchDamage, int>("TouchDamage");
        registerComponent<comp::Name, const std::string&>("Name");
        registerComponent<comp::Position, int, int>("Position");
        registerComponent<comp::HP, int, int>("HP");
        registerComponent<comp::Speed, int>("Speed");
        registerComponent<comp::Points, int>("Points");
        registerComponent<comp::View, int>("View");
        registerComponent<comp::DwarfAI, const std::string&>("DwarfAI");
        registerComponent<comp::TouchDestroy>("DestroyOnTouch");
        registerComponent<comp::Sprites, const std::vector<std::pair<std::string, std::string>>&>("Sprites");
        registerComponent<comp::AnimatedSprites, const std::vector<std::pair<std::string, std::string>>&>("AnimatedSprites");
    }

    EntityID::UUID createObject(const std::string& name)
    {
        auto entityID = EntityID::getID();
        auto newEntity = m_objects.insert({ entityID, {} });
        if(!newEntity.second) throw std::runtime_error("Couldn't create new entity for object \"" + name + "\"!\n");
        auto f = m_factory.createObject(name);
        newEntity.first->second = std::move(f);
        return entityID;
    }

    /*
    * \brief Loads a chaiscript file to define objects.
    *
    * \param path The path to the chaiscript file which contains the object definitions.
    */
    void loadObjectDefinitions(const std::string& path)
    {
        chaiscript::ChaiScript chai;
        chai.add(chaiscript::var(std::ref(m_factory)), "factory");
        chai.add(m_factory.getChaiDefModule());
        chai.add(m_factory.getChaiCreateModule());
        chai.eval_file(path);
    }

    /*
    * \brief Loads a chaiscript file to create objects.
    *
    * \param path The path to the chaiscript file which contains the object creations.
    */
    void loadObjectCreations(const std::string& path)
    {
        chaiscript::ChaiScript chai;
        chai.add(chaiscript::var(std::ref(*this)), "factory");
        chai.add(chaiscript::fun(&EntityManager::createObject), "createObject");
        chai.add(m_factory.getChaiCreateModule());
        chai.add(m_chaiModule);
        chai.eval_file(path);
    }


    template<class TComp>
    std::vector<EntityID::UUID> getAllEntitiesWithComponent() const
    {
        std::vector<EntityID::UUID> retVec;
        for (auto& ent : m_objects) {
            if (ent.second.find(TComp::componentTypeID) != ent.second.end())
                retVec.push_back(ent.first);
        }
        return retVec;
    }

    template<class TComp>
    bool hasComponent(EntityID::UUID ID) const
    {
        auto it = m_objects.find(ID);
        if (it == m_objects.end()) return false;

        auto itC = it->second.find(TComp::componentTypeID);
        if (itC == it->second.end()) return false;
        return true;
    }

    template<class TComp>
    TComp* getComponent(EntityID::UUID ID) const
    {
        auto it = m_objects.find(ID);
        if (it == m_objects.end()) return nullptr;

        auto itC = it->second.find(TComp::componentTypeID);
        if (itC == it->second.end()) return nullptr;
        return static_cast<TComp*>(itC->second);
    }

    template<class TComp>
    std::vector<TComp*> getAllComponents() const
    {
      std::vector<TComp*> retVec;
      for (auto& comps : m_objects) {
        auto it = comps.second.find(TComp::componentTypeID);
        if (it != comps.second.end()) retVec.push_back(static_cast<TComp*>(it->second));
      }
      return retVec;
    }

    template<class TComp>
    TComp* addComponent(EntityID::UUID ID)
    {
        auto it = m_objects.find(ID);
        if (it == m_objects.end())
            return nullptr;
        auto itC = it->second.find(TComp::componentTypeID);
        TComp* ret = nullptr;
        if (itC == it->second.end()) {
            ret = new TComp();
            it->second[TComp::componentTypeID] = ret;
        } else {
            ret = static_cast<TComp*>(itC->second);
        }
        return ret;
    }

    template<class TComp>
    void deleteComponent(EntityID::UUID ID)
    {
        auto it = m_objects.find(ID);
        if (it == m_objects.end()) return;

        auto itC = it->second.find(TComp::componentTypeID);
        if (itC == it->second.end()) return;
        if (itC->second) delete itC->second;
        it->second.erase(itC);

        auto cbIt = m_destroyComponentCB.find(TComp::componentTypeID);
        if (cbIt != m_destroyComponentCB.end()) {
            for (auto& cb : cbIt->second) {
                cb(ID);
            }
        }
    }

    void killEntity(EntityID::UUID ID)
    {
        auto it = m_objects.find(ID);
        if (it == m_objects.end())
            return;

        for (const auto& [CompID, ptr] : it->second) {
            delete ptr;
            auto cbIt = m_destroyComponentCB.find(CompID);
            if (cbIt != m_destroyComponentCB.end()) {
                for (auto& cb : cbIt->second) {
                    cb(ID);
                }
            }
        }

        m_objects.erase(it);

        auto cbIt = m_destroyEntityCB.find(ID);
        if (cbIt != m_destroyEntityCB.end()) {
            for (auto& cb : cbIt->second) {
                cb(ID);
            }
        }
    }

    void listenToEntityDestruction(EntityID::UUID eID, std::function<void(EntityID::UUID)>&& cb)
    {
        auto it = m_destroyEntityCB.find(eID);
        if (it != m_destroyEntityCB.end()) {
            it->second.push_back(cb);
        } else {
            m_destroyEntityCB[eID].push_back(std::move(cb));
        }
    }

    template<class TComp>
    void listenToComponentDestruction(std::function<void(EntityID::UUID)>&& cb)
    {
        auto it = m_destroyComponentCB.find(TComp::componentTypeID);
        if(it != m_destroyEntityCB.end()) {
            it->second.push_back(cb);
        } else {
            m_destroyComponentCB[TComp::componentTypeID].push_back(std::move(cb));
        }
    }

    const chaiscript::ModulePtr& getChaiModuleForEntityManager()
    {
        return m_chaiModule;
    }

    const chaiscript::ModulePtr& getChaiModuleForComponents()
    {
        return m_factory.getChaiCreateModule();
    }

private:

    template<class TComp, class... TArgs>
    void registerComponent(const std::string& name)
    {
        if (m_factory.registerComponent<TComp, TArgs...>(name)) {
            m_chaiModule->add(chaiscript::fun(&EntityManager::getAllEntitiesWithComponent<TComp>), "getAllEntitiesWith_" + name);
            m_chaiModule->add(chaiscript::fun(&EntityManager::hasComponent<TComp>), "hasComponent_" + name);
            m_chaiModule->add(chaiscript::fun(&EntityManager::getComponent<TComp>), "getComponent_" + name);
            m_chaiModule->add(chaiscript::fun(&EntityManager::getAllComponents<TComp>), "getAllComponents_" + name);
            m_chaiModule->add(chaiscript::fun(&EntityManager::addComponent<TComp>), "addComponent_" + name);
            m_chaiModule->add(chaiscript::fun(&EntityManager::deleteComponent<TComp>), "deleteComponent_" + name);
        }
    }

    ECSFactory  m_factory;
    chaiscript::ModulePtr  m_chaiModule;
    // TODO: Think whether weak_ptr for BaseComponent makes sense.
    std::unordered_map<EntityID::UUID, ECSFactory::ComponentList>  m_objects;

    std::unordered_map <EntityID::UUID, std::vector<std::function<void(EntityID::UUID)>>> m_destroyEntityCB;
    std::unordered_map <ComponentUUID, std::vector<std::function<void(EntityID::UUID)>>> m_destroyComponentCB;
};

}
