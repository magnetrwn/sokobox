#ifndef WORLDVIEW_HPP_
#define WORLDVIEW_HPP_

#include <algorithm>
#include <vector>
#include <array>
#include "isoview.hpp"
#include "typedef.hpp"
#include "util.hpp"

using u8_8 = std::array<u8, 8>;

struct WorldElement {
    constexpr static u8 END = U8MAX;
    constexpr static u8 SKIP = U8MAX - 1;

    // NOTE: only use stacked_tiles with SKIP and END for tile stack control, the other arrays depend on this one (second one in init list)
    // TODO: refactor
    struct WorldElementInit {
        u8_8 stacked_tiles;
        u8_8 tileset;
        u8_8 animation_steps;

        WorldElementInit(u8 tileset, u8 tile_idx, u8 animation_steps = 0)
            : tileset({ tileset }), stacked_tiles({ tile_idx, END }), animation_steps({ animation_steps }) {}

        WorldElementInit(u8_8 tileset, u8_8 stacked_tiles, u8_8 animation_steps = {0})
            : tileset(tileset), stacked_tiles(stacked_tiles), animation_steps(animation_steps) {}
            
        static WorldElementInit MOVABLE_CRATE_RED()     { return WorldElementInit(0, 35); }
        static WorldElementInit MOVABLE_CRATE_GREEN()   { return WorldElementInit(0, 36); }
        static WorldElementInit MOVABLE_CRATE_BLUE()    { return WorldElementInit(0, 37); }
        static WorldElementInit MOVABLE_CRATE_YELLOW()  { return WorldElementInit(0, 38); }
        static WorldElementInit MOVABLE_CRATE_BLANK()   { return WorldElementInit(0, 39); }

        static WorldElementInit MOVABLE_CRATE_SHADOW_RED()    { return WorldElementInit(0, 40); }
        static WorldElementInit MOVABLE_CRATE_SHADOW_GREEN()  { return WorldElementInit(0, 41); }
        static WorldElementInit MOVABLE_CRATE_SHADOW_BLUE()   { return WorldElementInit(0, 42); }
        static WorldElementInit MOVABLE_CRATE_SHADOW_YELLOW() { return WorldElementInit(0, 43); }
        static WorldElementInit MOVABLE_CRATE_SHADOW_BLANK()  { return WorldElementInit(0, 44); }

        static WorldElementInit METAL_GATE_CLOSED_RED()    { return WorldElementInit(0, 45); }
        static WorldElementInit METAL_GATE_CLOSED_GREEN()  { return WorldElementInit(0, 46); }
        static WorldElementInit METAL_GATE_CLOSED_BLUE()   { return WorldElementInit(0, 47); }
        static WorldElementInit METAL_GATE_CLOSED_YELLOW() { return WorldElementInit(0, 48); }
        static WorldElementInit METAL_GATE_CLOSED_BLANK()  { return WorldElementInit(0, 49); }

        static WorldElementInit METAL_GATE_OPEN_RED()    { return WorldElementInit(0, 50); }
        static WorldElementInit METAL_GATE_OPEN_GREEN()  { return WorldElementInit(0, 51); }
        static WorldElementInit METAL_GATE_OPEN_BLUE()   { return WorldElementInit(0, 52); }
        static WorldElementInit METAL_GATE_OPEN_YELLOW() { return WorldElementInit(0, 53); }
        static WorldElementInit METAL_GATE_OPEN_BLANK()  { return WorldElementInit(0, 54); }

        static WorldElementInit PLAYER_IDLE()           { return WorldElementInit(3, 0, 4); }
        static WorldElementInit PLAYER_MOVE_UP_RIGHT()  { return WorldElementInit(3, 4, 4); }
        static WorldElementInit PLAYER_MOVE_DOWN_LEFT() { return WorldElementInit(3, 8, 4); }
        static WorldElementInit PLAYER_MOVE_FAST()      { return WorldElementInit(3, 12, 4); }
    };

    u8_8 stacked_tiles; // NOTE: only use stacked_tiles with SKIP and END for tile stack control, the other arrays depend on this one
    u8_8 tileset;
    u8_8 animation_steps;
    u8_8 anim_step;
    
    WorldElement() 
        : stacked_tiles({ END }),
          tileset({0}), animation_steps({0}), anim_step({0}) {}
        
    WorldElement(const WorldElementInit& init)
        : stacked_tiles(init.stacked_tiles), 
          tileset(init.tileset), animation_steps(init.animation_steps), anim_step({0}) {}

    // TODO: turn to "in set" when refactoring to avoid magic
    bool is_player(usize idx = 0) const {
        return tileset[idx] == 3;
    }

    // TODO: turn to "in set" when refactoring to avoid magic
    bool is_movable(usize idx = 0) const {
        return (tileset[idx] == 0 and (stacked_tiles[idx+1] == END) and (
            stacked_tiles[idx] == 35 or stacked_tiles[idx] == 36 or stacked_tiles[idx] == 37 or stacked_tiles[idx] == 38 or stacked_tiles[idx] == 39
        ));
    }

    // TODO: turn to "in set" when refactoring to avoid magic
    bool is_walkable(usize idx = 0) const {
        return (tileset[idx] == 0 and (stacked_tiles[idx+1] == END) and (
            stacked_tiles[idx] == 40 or stacked_tiles[idx] == 41 or stacked_tiles[idx] == 42 or stacked_tiles[idx] == 43 or stacked_tiles[idx] == 44 or
            stacked_tiles[idx] == 50 or stacked_tiles[idx] == 51 or stacked_tiles[idx] == 52 or stacked_tiles[idx] == 53 or stacked_tiles[idx] == 54
        ));
    }

    void push_front(const WorldElement& other) {
        if (stacked_tiles[stacked_tiles.size() - 1] == END)
            stacked_tiles[stacked_tiles.size() - 2] = END;

        for (i64 i = stacked_tiles.size() - 1; i > 0; --i) {
            stacked_tiles[i] = stacked_tiles[i-1];
            tileset[i] = tileset[i-1];
            animation_steps[i] = animation_steps[i-1];
            anim_step[i] = anim_step[i-1];
        }

        stacked_tiles[0] = other.stacked_tiles[0];
        tileset[0] = other.tileset[0];
        animation_steps[0] = other.animation_steps[0];
        anim_step[0] = 0;
    }

    void pop_front() {
        bool found_end = false;

        for (i64 i = 0; i < stacked_tiles.size() - 1; ++i) {
            stacked_tiles[i] = stacked_tiles[i+1];
            tileset[i] = tileset[i+1];
            animation_steps[i] = animation_steps[i+1];
            anim_step[i] = anim_step[i+1];
            found_end = found_end or stacked_tiles[i] == END;
        }

        if (!found_end)
            stacked_tiles[0] = END;
    }

    void push_end(const WorldElement& other) {
        for (i64 i = 0; i < stacked_tiles.size() - 1; ++i)
            if (stacked_tiles[i] == END) {
                stacked_tiles[i] = other.stacked_tiles[0];
                tileset[i] = other.tileset[0];
                animation_steps[i] = other.animation_steps[0];
                anim_step[i] = 0;
                if (i <= stacked_tiles.size() - 2)
                    stacked_tiles[i+1] = END;
                else
                    stacked_tiles[i] = END;
                return;
            }
    }

    void pop_end() {
        for (i64 i = 1; i < stacked_tiles.size(); ++i)
            if (stacked_tiles[i] == END) {
                stacked_tiles[i] = 0;
                stacked_tiles[i-1] = END;
                return;
            }
    }

    bool operator==(const WorldElement& other) const { 
        return tileset == other.tileset and stacked_tiles == other.stacked_tiles and animation_steps == other.animation_steps;
    }
    
    bool operator!=(const WorldElement& other) const { return !(*this == other); }
    bool empty() const { return stacked_tiles[0] == END; }
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

    bool empty() const { return elem.empty(); }
    void clear() { elem = WorldElement(); }
};

class WorldState {
private:
    std::vector<WorldElement> world;
    std::vector<WorldTransition> tile_transitions;
    WorldTransition player_transition;
    usize w, h;
    IsometricView& iso;

    void draw_tile_stack(const WorldElement& elem, f32_2 position) const;

public:
    WorldState(usize width, usize height, IsometricView& iso) : w(width), h(height), iso(iso), world(width * height) {}

    void set(usize x, usize y, WorldElement elem) { world[y * w + x] = elem; }
    void unset(usize x, usize y) { world[y * w + x] = WorldElement(); }

    void move_player(WorldTransition tran) { player_transition = tran; }
    bool is_player_moving() const { return !player_transition.empty(); }
    
    void move_tile(WorldTransition tran) { tile_transitions.push_back(tran); }
    bool is_tile_moving() const { return tile_transitions.size() != 0; }

    bool is_oob(usize x, usize y) const { return x >= width() and y >= height(); }
    WorldElement get(usize x, usize y) const { return world[y * w + x]; }
    
    usize width() const { return w; }
    usize height() const { return h; }
    usize size() const { return world.size(); }
    const WorldElement* data() const { return world.data(); }
    WorldElement* data() { return world.data(); }
    void clear() { std::fill(world.begin(), world.end(), WorldElement()); }
    void resize(usize w, usize h) { world.resize(w * h); }

    void draw() const; 
    void step_animations();
    void step_transitions();
};

using El = WorldElement::WorldElementInit;

#endif