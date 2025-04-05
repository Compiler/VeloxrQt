#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <VulkanRenderer_global.h>

namespace Veloxr {

    struct Point {
        uint32_t x, y;
    };

    class VULKANRENDERER_EXPORT OIIOTexture {
        public:
            OIIOTexture() = default;
            OIIOTexture(std::string filename);
            void init(std::string filename);

            inline const Point& getResolution() const { return _resolution; }
            inline const std::string& getFilename() const { return _filename; }
            inline const int& getNumChannels() const { return _numChannels; }
            std::vector<unsigned char> load(std::string filename="");
            inline const bool isInitialized() const { return _loaded; }

        private:
            Point _resolution;
            std::string _filename;
            int _numChannels;
            bool _loaded{false};

    };

}
