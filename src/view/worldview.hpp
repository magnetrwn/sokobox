#ifndef WORLDVIEW_HPP_
#define WORLDVIEW_HPP_

#include <algorithm>
#include <vector>
#include <array>
#include "isotf.hpp"
#include "typedef.hpp"
#include "util.hpp"

using u8_13 = std::array<u8, 13>;

struct WorldElement {
    constexpr static u8 END = U8MAX;
    constexpr static u8 SKIP = U8MAX - 1;

    struct WorldElementInit {
        u8_13 stacked_tiles;
        u8 tileset;
        u8 animation_steps;

        WorldElementInit(u8 tileset, u8 tile_idx, u8 animation_steps = 0)
            : tileset(tileset), stacked_tiles({ tile_idx, END }), animation_steps(animation_steps) {}

        WorldElementInit(u8 tileset, u8_13 stacked_tiles, u8 animation_steps = 0)
            : tileset(tileset), stacked_tiles(stacked_tiles), animation_steps(animation_steps) {}

        static WorldElementInit MOVABLE_CRATE() {
            return WorldElementInit(
                util::cfg_usize("Resources.Tiledata", "MOVABLE_CRATE_TILESET"),
                util::cfg_usize("Resources.Tiledata", "MOVABLE_CRATE_INDEX")
            );
        }

        static WorldElementInit PLAYER_IDLE() {
            return WorldElementInit(
                util::cfg_usize("Resources.Tiledata", "PLAYER_IDLE_TILESET"),
                util::cfg_usize("Resources.Tiledata", "PLAYER_IDLE_INDEX"),
                util::cfg_usize("Resources.Tiledata", "PLAYER_IDLE_STEPS")
            );
        }

        static WorldElementInit PLAYER_MOVE() {
            return WorldElementInit(
                util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_TILESET"),
                util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_INDEX"),
                util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_STEPS")
            );
        }
    };

    u8_13 stacked_tiles;
    u8 tileset;
    u8 animation_steps;
    u8 anim_step;
    
    WorldElement() 
        : tileset(0), stacked_tiles({ END }), animation_steps(0), anim_step(0) {}
        
    WorldElement(const WorldElementInit& init)
        : tileset(init.tileset), stacked_tiles(init.stacked_tiles), animation_steps(init.animation_steps), anim_step(0) {}

    inline bool operator==(const WorldElement& other) const {
        return tileset == other.tileset and stacked_tiles == other.stacked_tiles;
    }

    inline bool operator!=(const WorldElement& other) const { return !(*this == other); }
    inline bool empty() const { return stacked_tiles[0] == END; }
};

struct WorldTransition {
    WorldElement elem;
    WorldElement on_end;
    usize end_x, end_y;
    f32_2 start, position;
    f32_2 increment;
    u8 anim_steps_left;

    // Careful: this doesn't care about end coordinates, it's just stepping in the direction of the increment!
    WorldTransition(f32_2 start_pos, 
                    usize target_x, usize target_y, 
                    f32_2 move_increment, 
                    const WorldElement& transition_elem, 
                    const WorldElement& final_elem,      // WorldElement() for none.
                    u8 animation_steps)
        : start(start_pos), 
          end_x(target_x), 
          end_y(target_y), 
          position(start_pos), 
          increment(move_increment), 
          elem(transition_elem), 
          on_end(final_elem), 
          anim_steps_left(animation_steps) {}

    WorldTransition()
        : start(f32_2{ 0.0f, 0.0f }), 
          end_x(0), 
          end_y(0), 
          position(f32_2{ 0.0f, 0.0f }), 
          increment(f32_2{ 0.0f, 0.0f }), 
          elem(), 
          on_end(), 
          anim_steps_left(0) {}

    inline bool empty() const { return elem.empty(); }
    inline void clear() { elem = WorldElement(); }
};

class WorldView {
private:
    std::vector<WorldElement> world;
    std::vector<WorldTransition> trans;
    WorldTransition player_transition;
    usize w, h;
    IsometricTf& iso;

    void draw_tile_stack(const WorldElement& elem, f32_2 position) const;

public:
    WorldView(usize width, usize height, IsometricTf& iso) : w(width), h(height), iso(iso), world(width * height) {}

    inline void set(usize x, usize y, WorldElement elem) { world[y * w + x] = elem; }
    inline void unset(usize x, usize y) { world[y * w + x] = WorldElement(); }

    inline void move_player(WorldTransition tran) { player_transition = tran; }
    inline bool is_player_moving() const { return !player_transition.empty(); }
    inline void move_tile(WorldTransition tran) { trans.push_back(tran); }
    inline bool is_tile_moving() const { return trans.size() != 0; }

    inline WorldElement get(usize x, usize y) const { return world[y * w + x]; }
    
    inline usize width() const { return w; }
    inline usize height() const { return h; }
    inline usize size() const { return world.size(); }
    inline const WorldElement* data() const { return world.data(); }
    inline WorldElement* data() { return world.data(); }
    inline void clear() { std::fill(world.begin(), world.end(), WorldElement()); }
    inline void resize(usize w, usize h) { world.resize(w * h); }

    void draw() const; 
    void step_animations();
    void step_transitions();
    void step(); 
};

#endif