#ifndef ISOMGR_HPP_
#define ISOMGR_HPP_

#include <vector>
#include <raylib.h>
#include "typedef.hpp"
#include "atlasmgr.hpp"

struct IsometricAtlas {
    AtlasManager& atlas;
    f32_2 start_pos;
    f32_2 tsize;
    f32 tile_scale;
};

class IsometricManager {
private:
    std::vector<IsometricAtlas> atlv;

    inline f32_2 isometric(f32 x, f32 y, usize idx) const {
        f32_2 iso;
        iso.x = (x - y) * (atlv[idx].tsize.x / 2.0f) + atlv[idx].start_pos.x - atlv[idx].tsize.x / 2.0f;
        iso.y = (x + y) * (atlv[idx].tsize.y / 4.0f) + atlv[idx].start_pos.y;
        return iso;
    }

public:
    inline void with(AtlasManager& atlas, f32_2 start_pos, f32 tile_scale) {
        atlv.push_back({ atlas, start_pos, atlas.tile_size(), tile_scale });
    }

    inline void with(AtlasManager&& atlas, f32_2 start_pos, f32 tile_scale) {
        atlv.push_back({ atlas, start_pos, atlas.tile_size(), tile_scale });
    }

    inline void draw(usize atlas_idx, usize sprite_idx, f32_2 xy) const {
        const AtlasManager& atlas = atlv[atlas_idx].atlas;
        const f32_2 iso = isometric(xy.x, xy.y, atlas.tile_size().x * atlv[atlas_idx].tile_scale);
        const f32 tscale = atlv[atlas_idx].tile_scale;

        atlas.draw(sprite_idx, iso, tscale, tscale);
    }
};

#endif