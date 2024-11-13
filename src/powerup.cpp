// #include <raylib.h>
// #include <raymath.h>

#include "includes.h"
#include "classes.h"
#include "constants.h"

PowerUp::PowerUp(Vector2 playerPos) {
    angle = 0;
    active = true;
    while (true) {
        pos = {(float) GetRandomValue(DANGER_ZONE + 300, SCREEN_WIDTH - POWERUP_RADIUS - 300),
                (float) GetRandomValue(POWERUP_RADIUS, SCREEN_HEIGHT - POWERUP_RADIUS)};
        short distToPlayer = Vector2Distance(playerPos, pos);

        if (distToPlayer > 400) break;
    }

    type = GetRandomValue(1, 3);
}

void PowerUp::render() {
    angle++;
    switch (type) {
        case 1:
            DrawPoly(pos, 5, POWERUP_RADIUS, angle, {255, 128, 0, 255});
            break;
        case 2:
            DrawPoly(pos, 5, POWERUP_RADIUS, angle, {243, 8, 223, 255});
            break;
        case 3:
            DrawPoly(pos, 5,POWERUP_RADIUS, angle, {0, 128, 255, 255});
    }
    DrawPoly(pos, 5, POWERUP_RADIUS - 7, angle, {101, 243, 8, 255});
}