#include <raylib.h>
#include <raymath.h>

#include "classes.h"
#include "constants.h"

extern Texture2D TEX_bullet;
extern Rectangle RENDER_bullet_source;
extern Vector2 RENDER_bullet_origin;

Bullet::Bullet(Vector2 sPos, Vector2 sVelo) {
    pos = sPos;
    active = true;
    velo = Vector2Scale(sVelo, BULLET_VELOCITY);
    angle = Vector2Angle((Vector2) {1.0f, 0.0f}, velo);
}

void Bullet::render() {
    DrawTexturePro(TEX_bullet, RENDER_bullet_source, {pos.x, pos.y, (float) TEX_bullet.width, (float) TEX_bullet.height}, RENDER_bullet_origin, angle * RAD2DEG, WHITE);
}

void Bullet::update() {
    pos = Vector2Add(pos, velo);
    if (pos.x > SCREEN_WIDTH || pos.x < 0 || pos.y > SCREEN_HEIGHT || pos.y < 0)
        // flag to kill object on OOB
        active = false;
}