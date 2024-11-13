#include <raylib.h>
#include <raymath.h>

#include "classes.h"
#include "constants.h"

Entity::Entity() {
    // default values
    velo = {0, 0};
    acce = {0, 0};
}

void Entity::movementUpdate() {
    // velocity application
    pos = Vector2Add(pos, velo);

    // out of bounds handling
    // method feels stupid, will fix later, maybe
    if (pos.x > SCREEN_WIDTH - RADIUS / 2) {
        pos.x = SCREEN_WIDTH - RADIUS / 2;
        velo.x *= WALL_BOUNCE;
        acce = {0, 0};
    } else if (pos.x < 0) {
        pos.x = RADIUS / 2;
        velo.x *= WALL_BOUNCE;
        acce = {0, 0};
    }

    if (pos.y > SCREEN_HEIGHT - RADIUS / 2) {
        pos.y = SCREEN_HEIGHT - RADIUS / 2;
        velo.y *= WALL_BOUNCE;
        acce = {0, 0};
    } else if (pos.y < 0) {
        pos.y = RADIUS / 2;
        velo.y *= WALL_BOUNCE;
        acce = {0, 0};
    }

    // acceleration application
    velo = Vector2Add(velo, acce);
}