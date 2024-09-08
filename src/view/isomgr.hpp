#ifndef ISOMGR_HPP_
#define ISOMGR_HPP_

#include <vector>
#include <raylib.h>
#include "typedef.hpp"
#include "atlasmgr.hpp"

struct IsometricAtlas {
    AtlasManager& atlas;
    f32 tile_scale;
};

class IsometricManager {
private:
    std::vector<IsometricAtlas> atlv;
    f32_2 position;

    inline f32_2 isometric(f32 x, f32 y, usize atlas_idx) const {
        const AtlasManager& atlas = atlv[atlas_idx].atlas;

        f32_2 iso;
        iso.x = (x - y) * (atlas.tile_size().x / 2.0f) + position.x - atlv[atlas_idx].tile_scale * atlas.tile_size().x / 2.0f;
        iso.y = (x + y) * (atlas.tile_size().y / 4.0f) + position.y - atlv[atlas_idx].tile_scale * atlas.tile_size().y / 4.0f;
        return iso;
    }

public:
    IsometricManager(f32_2 position = { 0.0f, 0.0f }) : position(position) {}

    inline usize with(AtlasManager& atlas, f32 tile_scale) {
        atlv.push_back({ atlas, tile_scale });
        return atlv.size() - 1;
    }

    inline void draw(usize atlas_idx, usize sprite_idx, f32_2 xy) const {
        const AtlasManager& atlas = atlv[atlas_idx].atlas;
        const f32 tscale = atlv[atlas_idx].tile_scale;
        const f32_2 iso = isometric(tscale * xy.x, tscale * xy.y, atlas_idx);
        
        atlas.draw(sprite_idx, iso, tscale, tscale);
    }

    inline void set_pos(f32_2 pos) { position = pos; }
};

#endif