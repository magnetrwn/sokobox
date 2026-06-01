#ifndef ISOVIEW_HPP_
#define ISOVIEW_HPP_

#include <vector>
#include "typedef.hpp"
#include "atlas.hpp"
#include "smoothcam.hpp"

struct IsometricAtlas {
    Atlas& atlas;
    f32 tile_scale;
};

class IsometricView {
private:
    std::vector<IsometricAtlas> atlv;
    f32_2 viewport, position;
    SmoothCamera camera;

public:
    IsometricView(f32_2 viewport, f32_2 position = { 0.0f, 0.0f }) 
        : viewport(viewport), 
          position(position), 
          camera(position) {}

    usize load(Atlas& atlas, f32 tile_scale) {
        atlv.push_back({ atlas, tile_scale });
        return atlv.size() - 1;
    }

    f32_2 to_isometric(f32 x, f32 y, usize atlas_idx) const {
        const f32_2 tsize = atlv[atlas_idx].atlas.tile_size();
        const f32 tscale = atlv[atlas_idx].tile_scale;
        return {
            (x - y) * (tsize.x / 2.0f) - tscale * tsize.x / 2.0f,
            (x + y) * (tsize.y / 4.0f) - tscale * tsize.y / 4.0f
        };
    }

    void draw_tile(usize atlas_idx, usize sprite_idx, f32_2 xy) const {
        const f32 tscale = atlv[atlas_idx].tile_scale;
        const f32_2 tsize = atlv[atlas_idx].atlas.tile_size();

        f32_2 iso = to_isometric(tscale * xy.x, tscale * xy.y, atlas_idx);
        iso.x += position.x;
        iso.y += position.y;

        if (iso.x < -(tscale * tsize.x) or iso.x > viewport.x + (tscale * tsize.x) or iso.y < -(tscale * tsize.y) or iso.y > viewport.y + (tscale * tsize.y))
            return;

        atlv[atlas_idx].atlas.draw(sprite_idx, iso, tscale, tscale);
    }

    void update_pos(f32_2 pos) { position = { position.x + pos.x, position.y + pos.y }; }

    void center_view_on(f32_2 pos, usize atlas_idx) {
        const f32_2 tsize = atlv[atlas_idx].atlas.tile_size();
        const f32 tscale = atlv[atlas_idx].tile_scale;
        const f32_2 iso = to_isometric(tscale * pos.x, tscale * pos.y, atlas_idx);

        position = { 
            viewport.x / 2.0f - iso.x - tscale * tsize.x / 2.0f, 
            viewport.y / 2.0f - iso.y - tscale * tsize.y / 2.0f
        };
    }

    void update_scale(f32 scale, f32_2 center) {
        position.x = center.x + (position.x - center.x) * scale;
        position.y = center.y + (position.y - center.y) * scale;

        for (IsometricAtlas& atlas : atlv)
            atlas.tile_scale *= scale;
    }

    void set_camera(f32_2 pos) { camera.set(pos); }
    void target_camera(f32_2 pos) { camera.target(pos); }
    void step_camera(f32 scale, usize atlas_idx) { camera.step(scale); center_view_on(camera.get(), atlas_idx); }
};

#endif