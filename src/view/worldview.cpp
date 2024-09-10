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
            
            for (const WorldTransition& tran : trans)
                if (tran.end_x == x and tran.end_y == y)
                    draw_tile_stack(tran.elem, tran.position);
        }
}

void WorldView::step() {
    for (WorldElement& elem : world) {
        if (elem.animate_by == 0)
            continue;
        elem.anim_step = (elem.anim_step + 1) % elem.animate_by;
    }

    trans.erase(std::remove_if(trans.begin(), trans.end(), [this] (WorldTransition& tran) {
        tran.position.x += tran.increment.x;
        tran.position.y += tran.increment.y;
        if (tran.elem.animate_by)
            tran.elem.anim_step = (tran.elem.anim_step + 1) % tran.elem.animate_by;
        --tran.anim_steps_left;
        bool cond = tran.anim_steps_left == 0;
        if (cond and !tran.on_end.stacked_tiles.empty())
            set(tran.end_x, tran.end_y, tran.on_end);
        return cond;
    }), trans.end());
}