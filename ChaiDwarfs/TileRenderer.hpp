#ifndef _TILERENDERER_HPP_
#define _TILERENDERER_HPP_

#include <memory>

namespace CDwarfs {

  class TerrainMap;

  namespace render {

    class TileRenderer {
    public:
      TileRenderer() = delete;
      TileRenderer(const std::shared_ptr<TerrainMap>& terrainMap);
      ~TileRenderer();

      void init();
      void render();

    private:
      std::shared_ptr<TerrainMap> m_terrainMap;
    };

  }
}

#endif // !_TILERENDERER_HPP_
