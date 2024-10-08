#ifndef ISOTF_HPP_
#define ISOTF_HPP_

#include <vector>
#include "typedef.hpp"
#include "atlas.hpp"

struct IsometricAtlas {
    Atlas& atlas;
    f32 tile_scale;
};

class IsometricTf {
private:
    std::vector<IsometricAtlas> atlv;
    f32_2 viewport, position;

    inline f32_2 isometric(f32 x, f32 y, usize atlas_idx) const {
        const Atlas& atlas = atlv[atlas_idx].atlas;

        f32_2 iso;
        iso.x = (x - y) * (atlas.tile_size().x / 2.0f) + position.x - atlv[atlas_idx].tile_scale * atlas.tile_size().x / 2.0f;
        iso.y = (x + y) * (atlas.tile_size().y / 4.0f) + position.y - atlv[atlas_idx].tile_scale * atlas.tile_size().y / 4.0f;
        return iso;
    }

public:
    IsometricTf(f32_2 viewport, f32_2 position = { 0.0f, 0.0f }) : viewport(viewport), position(position) {}

    inline usize with(Atlas& atlas, f32 tile_scale) {
        atlv.push_back({ atlas, tile_scale });
        return atlv.size() - 1;
    }

    inline void draw(usize atlas_idx, usize sprite_idx, f32_2 xy) const {
        const f32 tscale = atlv[atlas_idx].tile_scale;
        const f32 cull = 16.0f * tscale;
        const f32_2 iso = isometric(tscale * xy.x, tscale * xy.y, atlas_idx);

        if (iso.x < -cull or iso.x > viewport.x + cull or iso.y < -cull or iso.y > viewport.y + cull)
            return;

        atlv[atlas_idx].atlas.draw(sprite_idx, iso, tscale, tscale);
    }

    inline void update_pos(f32_2 pos) { position = { position.x + pos.x, position.y + pos.y }; }

    inline void update_scale(f32 scale, f32_2 center) {
        position.x = center.x + (position.x - center.x) * scale;
        position.y = center.y + (position.y - center.y) * scale;

        for (IsometricAtlas& atlas : atlv)
            atlas.tile_scale *= scale;
    }
};

#endif