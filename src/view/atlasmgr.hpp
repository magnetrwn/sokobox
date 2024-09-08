#ifndef ATLASMGR_HPP_
#define ATLASMGR_HPP_

#include <raylib.h>
#include "typedef.hpp"

class AtlasManager {
private:
    const Texture2D atlas;
    const f32_2 tsize;
    const usize sprites;
    const usize cols;

    inline f32_4 id_to_rect(usize id) const {
        return { (id % cols) * tsize.x, static_cast<f32>(id / cols) * tsize.y, tsize.x, tsize.y };
    }

    inline usize max_sprites() const {
        return atlas.width / tsize.x * atlas.height / tsize.y;
    }

public:
    AtlasManager(const char* atlas_file, f32_2 tile_size, usize sprites = 0) 
        : atlas(LoadTexture(atlas_file)), 
          tsize(tile_size),
          sprites((sprites == 0) ? max_sprites() : sprites), 
          cols(atlas.width / tsize.x) {}

    ~AtlasManager() { UnloadTexture(atlas); }

    inline void draw(usize sprite_idx, f32_2 xy, f32 scale_x = 1.0f, f32 scale_y = 1.0f) const {
        DrawTexturePro(
            atlas, 
            id_to_rect(sprite_idx), 
            { xy.x, xy.y, tsize.x * scale_x, tsize.y * scale_y }, 
            { 0.0f, 0.0f }, 
            0.0f, 
            WHITE
        );
    }

    inline void draw(usize sprite_idx, f32_4 dest) const {
        DrawTexturePro(
            atlas, 
            id_to_rect(sprite_idx), 
            { dest.x, dest.y, tsize.x * dest.width, tsize.y * dest.height }, 
            { 0.0f, 0.0f }, 
            0.0f, 
            WHITE
        );
    }

    inline usize count() const { return sprites; }
    inline f32_2 tile_size() const { return tsize; }
};

#endif