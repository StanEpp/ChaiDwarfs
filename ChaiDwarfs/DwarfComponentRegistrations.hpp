#ifndef _DWARFCOMPONENTREGISTRATIONS_HPP_
#define _DWARFCOMPONENTREGISTRATIONS_HPP_

#include "DwarfComponents.hpp"

#include <chaiscript\chaiscript.hpp>

namespace CDwarfs {
  namespace objDef {

    template<class TDwarfsComp>
    void registerComponent(chaiscript::ModulePtr& module);

    template<>
    void registerComponent<DwarfHP>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<DwarfHP>(), "DwarfHP");
      module->add(chaiscript::fun(&DwarfHP::HP), "HP");
    }

    template<>
    void registerComponent<DwarfPosition>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<DwarfPosition>(), "DwarfPosition");
      module->add(chaiscript::fun(&DwarfPosition::row), "row");
      module->add(chaiscript::fun(&DwarfPosition::col), "col");
    }

    template<>
    void registerComponent<DwarfView>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<DwarfView>(), "DwarfView");
      module->add(chaiscript::fun(&DwarfView::dist), "dist");
    }

    template<>
    void registerComponent<DwarfSpeed>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<DwarfSpeed>(), "DwarfSpeed");
      module->add(chaiscript::fun(&DwarfSpeed::speed), "speed");
    }

    template<>
    void registerComponent<DwarfPoints>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<DwarfPoints>(), "DwarfPoints");
      module->add(chaiscript::fun(&DwarfPoints::points), "points");
    }

  }
}

#endif // !_OBJECTCOMPONENTREGISTRATIONS_HPP_