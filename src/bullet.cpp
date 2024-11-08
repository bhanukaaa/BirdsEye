#include "header.h"

Bullet::Bullet(Vector2 sPos, Vector2 sVelo) {
    // set starting values
    pos = sPos;
    velo = Vector2Scale(sVelo, BULLET_VELOCITY);
    active = true;
}

void Bullet::render() {
    DrawCircle(pos.x, pos.y, BULLET_RADIUS, YELLOW);
}

void Bullet::update() {
    pos = Vector2Add(pos, velo);
    if (pos.x > SCREEN_WIDTH || pos.x < 0 || pos.y > SCREEN_HEIGHT || pos.y < 0)
        // flag to kill object on OOB
        active = false;
}