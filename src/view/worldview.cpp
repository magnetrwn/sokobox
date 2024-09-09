#include "worldview.hpp"

void WorldView::draw() const {
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
        if (cond and tran.on_end.tile_idxs.size() != 0)
            set(tran.end_x, tran.end_y, tran.on_end);
        return cond;
    }), trans.end());
}