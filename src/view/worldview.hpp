#ifndef WORLDVIEW_HPP_
#define WORLDVIEW_HPP_

#include <vector>
#include <raylib.h>
#include "isomgr.hpp"
#include "typedef.hpp"

struct WorldElement {
    constexpr static u16 NOT_SET = U16MAX;

    u16 tileset;
    u16 animate_by;
    u16 anim_step;
    std::vector<u16> tile_idxs; // They stack vertically in the order they are defined, 0 being the bottom.

    WorldElement() 
        : tileset(0), tile_idxs({}), animate_by(0), anim_step(0) {}

    WorldElement(u16 tileset, u16 tile_idx, u16 animate_by = 0)
        : tileset(tileset), tile_idxs({ tile_idx }), animate_by(animate_by), anim_step(0) {}

    WorldElement(u16 tileset, std::vector<u16> tile_idx, u16 animate_by = 0)
        : tileset(tileset), tile_idxs(tile_idx), animate_by(animate_by), anim_step(0) {}
};

class WorldView {
private:
    std::vector<WorldElement> world;
    usize w, h;
    IsometricManager& iso;

public:
    WorldView(usize width, usize height, IsometricManager& iso) : w(width), h(height), iso(iso), world(width * height) {}

    inline void set(usize x, usize y, WorldElement elem) { world[y * w + x] = elem; }
    inline WorldElement get(usize x, usize y) const { return world[y * w + x]; }
    inline usize width() const { return w; }
    inline usize height() const { return h; }
    inline usize size() const { return world.size(); }
    inline const WorldElement* data() const { return world.data(); }
    inline WorldElement* data() { return world.data(); }
    inline void clear() { std::fill(world.begin(), world.end(), WorldElement()); }
    inline void resize(usize w, usize h) { world.resize(w * h); }

    inline void draw() const {
        for (usize y = 0; y < h; ++y)
            for (usize x = 0; x < w; ++x) {
                const WorldElement& elem = get(x, y);

                for (usize i = 0; i < elem.tile_idxs.size(); ++i)
                    if (elem.tile_idxs[i] != WorldElement::NOT_SET)
                        iso.draw(elem.tileset, elem.tile_idxs[i] + elem.anim_step, f32_2{ static_cast<f32>(x - i), static_cast<f32>(y - i) });
            }
    }

    inline void step() {
        for (WorldElement& elem : world) {
            if (elem.animate_by == 0)
                continue;

            elem.anim_step = (elem.anim_step + 1) % elem.animate_by;
        }
    }
};

#endif