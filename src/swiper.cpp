#include "header.h"

Swiper::Swiper() {
    type = 2;
}

void Swiper::render() {
    DrawCircle(pos.x, pos.y, RADIUS, MAROON);
}

bool Swiper::checkPlayer(Vector2 playerPos) {
    short dist = Vector2Distance(pos, playerPos);
    if (dist < 2 * RADIUS) // caught player
        return true;
    if (Vector2Distance(pos, playerPos) <= PURSUIT_THRESHOLD)
        pursue(playerPos); // chase player
    return false;
}


void Swiper::pursue(Vector2 playerPos) {
    // desired velocity is directly towards player, max velocity
    Vector2 desired = Vector2Subtract(playerPos, pos);
    acce = SetMagnitude(desired, MAX_VELOCITY_ENEMY);
    if (Vector2Length(acce) > MAX_ACCELERATION_ENEMY)
        acce = SetMagnitude(acce, MAX_ACCELERATION_ENEMY);
}