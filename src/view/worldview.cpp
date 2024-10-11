#include "worldview.hpp"

constexpr static auto END = WorldElement::END;
constexpr static auto SKIP = WorldElement::SKIP;

void WorldView::draw_tile_stack(const WorldElement& elem, f32_2 position) const {
    for (i64 i = 0; i < elem.stacked_tiles.size() and elem.stacked_tiles[i] != END; ++i)
        if (elem.stacked_tiles[i] != SKIP)
            iso.draw(
                elem.tileset, 
                elem.stacked_tiles[i] + elem.anim_step, 
                { position.x - static_cast<f32>(i), position.y - static_cast<f32>(i) }
            );
}

void WorldView::draw() const {
    for (i64 y = 0; y < h; ++y)
        for (i64 x = 0; x < w; ++x) {
            const WorldElement& elem = get(x, y);

            draw_tile_stack(elem, f32_2{ static_cast<f32>(x), static_cast<f32>(y) });
            
            if (player_transition.end_x == x and player_transition.end_y == y)
                draw_tile_stack(player_transition.elem, player_transition.position);

            for (const WorldTransition& tran : trans)
                if (tran.end_x == x and tran.end_y == y)
                    draw_tile_stack(tran.elem, tran.position);
        }
}

void WorldView::step_animations() {
    for (WorldElement& elem : world) {
        if (elem.animation_steps == 0)
            continue;
        elem.anim_step = (elem.anim_step + 1) % elem.animation_steps;
    }
}

void WorldView::step_transitions() {
    trans.erase(std::remove_if(trans.begin(), trans.end(), [this] (WorldTransition& tran) {
        tran.position.x += tran.increment.x;
        tran.position.y += tran.increment.y;
        if (tran.elem.animation_steps)
            tran.elem.anim_step = (tran.elem.anim_step + 1) % tran.elem.animation_steps;
        --tran.anim_steps_left;
        bool cond = tran.anim_steps_left == 0;
        if (cond and !tran.on_end.stacked_tiles.empty())
            set(tran.end_x, tran.end_y, tran.on_end);
        return cond;
    }), trans.end());

    if (player_transition.empty())
        return;

    player_transition.position.x += player_transition.increment.x;
    player_transition.position.y += player_transition.increment.y;
    player_transition.elem.anim_step = (player_transition.elem.anim_step + 1) % player_transition.elem.animation_steps;
    if (--player_transition.anim_steps_left == 0) {
        set(player_transition.end_x, player_transition.end_y, player_transition.on_end);
        player_transition.clear();
    }
}

void WorldView::step() {
    step_animations();
    step_transitions();
}