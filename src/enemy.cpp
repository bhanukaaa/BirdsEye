// #include <raylib.h>
// #include <raymath.h>

#include "includes.h"
#include "classes.h"
#include "constants.h"

Vector2 SetMagnitude(Vector2, float);

Enemy::Enemy() {
    // random start position and velocity
    pos = {SCREEN_WIDTH - 25, (float) GetRandomValue(RADIUS, SCREEN_HEIGHT - RADIUS)};
    velo = {(float) -GetRandomValue(1, 2), 0};
    type = 1;

    // accelerate towards danger zone
    attack();
    alive = true;
}

void Enemy::render() {
    DrawCircle(pos.x, pos.y, RADIUS, ORANGE);
    DrawCircleLines(pos.x, pos.y, RADIUS, RED);
}

void Enemy::update() {
    attack();
    movementUpdate();

    if (type == 1) {
        if (Vector2Length(velo) > MAX_VELOCITY_ENEMY)
            velo = SetMagnitude(velo, MAX_VELOCITY_ENEMY);
    } else if (type == 2) {
        if (Vector2Length(velo) > MAX_VELOCITY_ENEMY * 1.3)
            velo = SetMagnitude(velo, MAX_VELOCITY_ENEMY * 1.3);
    }
}

void Enemy::evade(Vector2 bulletPos) {
    // desired velocity is directly away from bullet, max velocity
    Vector2 desired = Vector2Subtract(pos, bulletPos);
    desired = SetMagnitude(desired, MAX_VELOCITY_ENEMY * EVASION_MULTIPLIER);

    acce = Vector2Add(acce, desired);
    if (Vector2Length(acce) > MAX_ACCELERATION_ENEMY)
        acce = SetMagnitude(acce, MAX_ACCELERATION_ENEMY);
}

void Enemy::attack() {
    // kinda stupid
    // accelerate leftwards
    acce = Vector2Add(acce, {-0.3, 0});
    if (Vector2Length(acce) > MAX_ACCELERATION_ENEMY)
        acce = SetMagnitude(acce, MAX_ACCELERATION_ENEMY);
}

bool Enemy::checkPlayer(Vector2 p) {return 0;}