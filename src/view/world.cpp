#include "world.hpp"

constexpr static auto END = WorldElement::END;
constexpr static auto SKIP = WorldElement::SKIP;

// NOTE: walkable tiles are not elevated, so that stacking tiles with walkable tiles allow walkable behavior (like a box with a box shadow)
void World::draw_tile_stack(const WorldElement& elem, f32_2 position) const {
    f32 up_count = 0.0f;
    for (i64 i = 0; i < elem.stacked_tiles.size() and elem.stacked_tiles[i] != END; ++i) {
        if (elem.stacked_tiles[i] != SKIP)
            iso.draw_tile(
                elem.tileset[i], 
                elem.stacked_tiles[i] + elem.anim_step[i], 
                (elem.is_walkable(i) or elem.is_player(i) or elem.is_walkable_movable_stack())
                  ? f32_2{ position.x, position.y }
                  : f32_2{ position.x - up_count, position.y - up_count }
            );
        up_count += 1.0f * static_cast<f32>(!(elem.is_walkable(i) or elem.is_player(i) or elem.is_walkable_movable_stack()));
    }
}

void World::draw() const {
    for (i64 sum = 0; sum < w + h - 1; ++sum)
        for (i64 x = 0; x < w; ++x) {
            i64 y = sum - x;
            if (y < 0 or y >= h)
                continue;
            const WorldElement& elem = get(x, y);

            // if (elem.stacked_tiles[0] == END)// and !iso.is_cullable(0, { static_cast<f32>(x), static_cast<f32>(y) }))
            //     iso.draw_tile(0, 53, { static_cast<f32>(x), static_cast<f32>(y) });

            draw_tile_stack(elem, f32_2{ static_cast<f32>(x), static_cast<f32>(y) });
            
            if (static_cast<i64>(player_transition.position.x + 0.5f) == x and static_cast<i64>(player_transition.position.y + 0.5f) == y)
                draw_tile_stack(player_transition.elem, player_transition.position);

            for (const WorldTransition& tran : tile_transitions)
                if (static_cast<i64>(tran.position.x + 0.5f) == x and static_cast<i64>(tran.position.y + 0.5f) == y)
                    draw_tile_stack(tran.elem, tran.position);
        }
}

void World::step_animations() {
    for (WorldElement& elem : world)
        for (i64 i = 0; i < elem.stacked_tiles.size() and elem.stacked_tiles[i] != END; ++i)
            if (elem.stacked_tiles[i] != SKIP) {
                if (elem.animation_steps[i] == 0)
                    continue;
                elem.anim_step[i] = (elem.anim_step[i] + 1) % elem.animation_steps[i];
            }
}

void World::step_transitions() {
    for (WorldTransition& tran : tile_transitions) {
        for (i64 i = 0; i < tran.elem.stacked_tiles.size() and tran.elem.stacked_tiles[i] != END; ++i)
            if (tran.elem.stacked_tiles[i] != SKIP) {
                tran.position.x += tran.increment.x;
                tran.position.y += tran.increment.y;
                if (tran.elem.animation_steps[i])
                    tran.elem.anim_step[i] = (tran.elem.anim_step[i] + 1) % tran.elem.animation_steps[i];
                --tran.anim_steps_left;

                if (tran.anim_steps_left == 0 and !tran.on_end.stacked_tiles.empty())
                    set(tran.end_x, tran.end_y, tran.on_end);
        }
    }

    tile_transitions.erase(std::remove_if(tile_transitions.begin(), tile_transitions.end(), [this] (const WorldTransition& tran) {
        return tran.anim_steps_left == 0;
    }), tile_transitions.end());

    if (player_transition.empty())
        return;

    player_transition.position.x += player_transition.increment.x;
    player_transition.position.y += player_transition.increment.y;
    player_transition.elem.anim_step[0] = (player_transition.elem.anim_step[0] + 1) % player_transition.elem.animation_steps[0];
    --player_transition.anim_steps_left;
    
    if (player_transition.anim_steps_left == 0) {
        set(player_transition.end_x, player_transition.end_y, player_transition.on_end);
        player_transition.clear();
    }
}
