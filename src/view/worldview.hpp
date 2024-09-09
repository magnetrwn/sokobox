#ifndef WORLDVIEW_HPP_
#define WORLDVIEW_HPP_

#include <algorithm>
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

struct WorldTransition {
    WorldElement elem;
    WorldElement on_end;
    usize end_x, end_y;
    f32_2 start, position;
    f32_2 increment;
    u16 anim_steps_left;

    // Careful: this doesn't care about end coordinates, it's just stepping in the direction of the increment!
    WorldTransition(f32_2 start_pos, 
                    usize target_x, usize target_y, 
                    f32_2 move_increment, 
                    const WorldElement& transition_elem, 
                    const WorldElement& final_elem = WorldElement(), 
                    u16 animation_steps = 8)
        : start(start_pos), 
          end_x(target_x), 
          end_y(target_y), 
          position(start_pos), 
          increment(move_increment), 
          elem(transition_elem), 
          on_end(final_elem), 
          anim_steps_left(animation_steps) {}
};

class WorldView {
private:
    std::vector<WorldElement> world;
    std::vector<WorldTransition> trans;
    usize w, h;
    IsometricManager& iso;

public:
    WorldView(usize width, usize height, IsometricManager& iso) : w(width), h(height), iso(iso), world(width * height) {}

    inline void set(usize x, usize y, WorldElement elem) { world[y * w + x] = elem; }
    inline void unset(usize x, usize y) { world[y * w + x] = WorldElement(); }
    inline void transition(WorldTransition tran) { trans.push_back(tran); }
    inline bool is_transition() const { return trans.size() != 0; }
    inline WorldElement get(usize x, usize y) const { return world[y * w + x]; }
    inline usize width() const { return w; }
    inline usize height() const { return h; }
    inline usize size() const { return world.size(); }
    inline const WorldElement* data() const { return world.data(); }
    inline WorldElement* data() { return world.data(); }
    inline void clear() { std::fill(world.begin(), world.end(), WorldElement()); }
    inline void resize(usize w, usize h) { world.resize(w * h); }

    inline void draw() const {
        for (i64 y = 0; y < h; ++y) {
            for (i64 x = 0; x < w; ++x) {
                const WorldElement& elem = get(x, y);

                for (i64 i = 0; i < elem.tile_idxs.size(); ++i)
                    if (elem.tile_idxs[i] != WorldElement::NOT_SET)
                        iso.draw(elem.tileset, elem.tile_idxs[i] + elem.anim_step, f32_2{ static_cast<f32>(x - i), static_cast<f32>(y - i) });
                
                // This is slow on lots of transitions, and glitches the graphics through when moving out of cover.
                for (const WorldTransition& tran : trans)
                    if (tran.end_x == x and tran.end_y == y)
                        for (i64 i = 0; i < tran.elem.tile_idxs.size(); ++i)
                            if (tran.elem.tile_idxs[i] != WorldElement::NOT_SET)
                                iso.draw(tran.elem.tileset, tran.elem.tile_idxs[0] + tran.elem.anim_step, tran.position);
            }
        }
    }

    inline void step() {
        for (WorldElement& elem : world) {
            if (elem.animate_by == 0)
                continue;

            elem.anim_step = (elem.anim_step + 1) % elem.animate_by;
        }

        trans.erase(std::remove_if(trans.begin(), trans.end(), [this] (WorldTransition& tran) {
            tran.position.x += tran.increment.x;
            tran.position.y += tran.increment.y;
            tran.elem.anim_step = (tran.elem.anim_step + 1) % tran.elem.animate_by;
            --tran.anim_steps_left;
            bool cond = tran.anim_steps_left == 0;
            if (cond and tran.on_end.tile_idxs.size() != 0)
                set(tran.end_x, tran.end_y, tran.on_end);
            return cond;
        }), trans.end());
    }
};

#endif