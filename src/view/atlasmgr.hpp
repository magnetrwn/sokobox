#ifndef ATLASMGR_HPP_
#define ATLASMGR_HPP_

#include <raylib.h>
#include "typedef.hpp"

class AtlasManager {
private:
    const Texture2D atlas;
    const Vector2 bsize;
    const usize sprites;
    const usize cols;

    inline Rectangle id_to_rect(usize id) const {
        return { (id % cols) * bsize.x, static_cast<f32>(id / cols) * bsize.y, bsize.x, bsize.y };
    }

    inline usize max_sprites() const {
        return atlas.width / bsize.x * atlas.height / bsize.y;
    }

public:
    AtlasManager(const char* atlas_file, Vector2 base_size, usize sprites = 0) 
        : atlas(LoadTexture(atlas_file)), 
          bsize(base_size),
          sprites((sprites == 0) ? max_sprites() : sprites), 
          cols(atlas.width / bsize.x) {}

    ~AtlasManager() { UnloadTexture(atlas); }

    inline void draw(usize sprite_id, f32 x, f32 y, f32 scale_x = 1.0f, f32 scale_y = 1.0f) const {
        DrawTexturePro(
            atlas, 
            id_to_rect(sprite_id), 
            { x, y, bsize.x * scale_x, bsize.y * scale_y }, 
            { 0.0f, 0.0f }, 
            0.0f, 
            WHITE
        );
    }

    inline void draw(usize sprite_id, Rectangle dest) const {
        DrawTexturePro(
            atlas, 
            id_to_rect(sprite_id), 
            { dest.x, dest.y, bsize.x * dest.width, bsize.y * dest.height }, 
            { 0.0f, 0.0f }, 
            0.0f, 
            WHITE
        );
    }

    inline usize count() const { return sprites; }
};

#endif