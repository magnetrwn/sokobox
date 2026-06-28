#ifndef SERIALIZER_HPP_
#define SERIALIZER_HPP_

#include <cstdint>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

#include <bxzstr/bxzstr.hpp>

#include "world.hpp"

class WorldSerializer {
public:
    static void save(const std::string& filename, const World& level) {
        std::ostringstream rawStream(std::ios::binary);

        {
            cereal::BinaryOutputArchive rawArchive(rawStream);
            rawArchive(level);
        }

        const std::string rawData = rawStream.str();
        const u32 checksum = fnv1a32(rawData);

        bxz::ofstream file(filename, bxz::zstd, 6);

        if (!file)
            throw std::runtime_error("Failed to open file for saving: " + filename);

        {
            cereal::BinaryOutputArchive archive(file);
            archive(checksum, rawData);
        }
    }

    static void load(const std::string& filename, World& level) {
        bxz::ifstream file(filename);

        if (!file)
            throw std::runtime_error("Failed to open file for loading: " + filename);

        u32 storedChecksum = 0;
        std::string rawData;

        {
            cereal::BinaryInputArchive archive(file);
            archive(storedChecksum, rawData);
        }

        const u32 actualChecksum = fnv1a32(rawData);

        if (actualChecksum != storedChecksum)
            throw std::runtime_error("Invalid or corrupted level file: checksum mismatch");


        std::istringstream rawStream(rawData, std::ios::binary);

        {
            cereal::BinaryInputArchive rawArchive(rawStream);
            rawArchive(level);
        }
    }

private:
    static u32 fnv1a32(const std::string& data) {
        u32 hash = 2166136261u;

        for (unsigned char c : data) {
            hash ^= c;
            hash *= 16777619u;
        }

        return hash;
    }
};

#endif