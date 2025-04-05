#pragma once
#include <texture.h>
#include <vector>
#include <Vertex.h>
#include <VulkanRenderer_global.h>
namespace Veloxr {

    struct TextureData {
        uint32_t width, height, channels;
        std::vector<unsigned char> pixelData;
    };

    struct TiledResult {
        std::vector<TextureData> tiles;
        std::vector<Vertex>      vertices;
    };


    class VULKANRENDERER_EXPORT TextureTiling {
        
        public:
            TextureTiling() = default;
            void init();
            std::vector<TextureData> tile(OIIOTexture& texture, uint32_t maxResolution = 4096*2);

            TiledResult tile2(OIIOTexture &texture, uint32_t maxResolution=4096*2);
            TiledResult tile3(OIIOTexture &texture, uint32_t maxResolution=4096*2);
            TiledResult tile4(OIIOTexture &texture, uint32_t maxResolution=4096*2);

    };

}
