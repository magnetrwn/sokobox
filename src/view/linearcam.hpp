#ifndef LINEARCAM_HPP_
#define LINEARCAM_HPP_

#include <cmath>
#include "typedef.hpp"

class LinearCamera {
private:
    f32_2 current_pos;
    f32_2 target_pos;

public:
    LinearCamera(f32_2 pos) : current_pos(pos), target_pos(pos) {}

    inline f32_2 get() const { return current_pos; }
    inline void set(f32_2 pos) { current_pos = pos; target_pos = pos; }
    inline void target(f32_2 pos) { target_pos = pos; }

    // Linear interpolation (LERP) with a constant interpolation factor 'scale' (0 to 1)
    inline void step(f32 scale) { 
        current_pos.x = current_pos.x + scale * (target_pos.x - current_pos.x);
        current_pos.y = current_pos.y + scale * (target_pos.y - current_pos.y);
        
        // Optionally, snap to target when very close
        if (std::abs(target_pos.x - current_pos.x) < 0.1f && std::abs(target_pos.y - current_pos.y) < 0.1f) {
            current_pos = target_pos;
        }
    }
};

#endif
