#ifndef _TERRAINOBJECT_HPP_
#define _TERRAINOBJECT_HPP_

#include "ChaiScriptTypes.hpp"
#include "Datastructs.hpp"


namespace CDwarfs {

  enum class ObjectType : unsigned int {
    INVALID,
    DIAMOND,
    RUBY,
    SAPPHIRE
  };

  class TerrainObject {
  public:
    virtual ~TerrainObject() {}

    Point pos() {
      return m_position;
    }

    ObjectID ID() const {
      return m_ID;
    }

    virtual ObjectType type() const {
      return m_type;
    }

  protected:

    TerrainObject() = delete;
    TerrainObject(ObjectID ID, ObjectType type) : m_ID(ID), m_type(type) {}

    Point      m_position;
    ObjectID   m_ID;
    const ObjectType m_type = ObjectType::INVALID;
  };


  struct ObjValue {
    ObjValue(int value) : m_value(value) {}
    inline int value() const { return m_value; }
    int m_value = 0; 
  };

  class Diamond : public TerrainObject {
  public:
    Diamond(ObjectID ID) : TerrainObject(ID, ObjectType::DIAMOND), m_value(100) {  }
    int value() const { return m_value.value(); }
  private:
    ObjValue m_value;
  };

  class Ruby : public TerrainObject {
  public:
    Ruby(ObjectID ID) : TerrainObject(ID, ObjectType::RUBY), m_value(75) {  }
    int value() const { return m_value.value(); }
  private:
    ObjValue m_value;
  };

  class Sapphire : public TerrainObject {
  public:
    Sapphire(ObjectID ID) : TerrainObject(ID, ObjectType::SAPPHIRE), m_value(25) {  }
    int value() const { return m_value.value(); }
  private:
    ObjValue m_value;
  };
}

#endif // !_TERRAINOBJECT_HPP_
