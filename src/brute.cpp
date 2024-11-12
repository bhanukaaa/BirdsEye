#include "header.h"

Brute::Brute() {
    hp = 4;
    type = 3;
    velo = SetMagnitude(velo, 0.4);
}

void Brute::render() {
    switch (hp) {
    case 2:
        DrawCircle(pos.x, pos.y, BRUTE_RADIUS, BLUE);
        break;
    case 1:
        DrawCircle(pos.x, pos.y, BRUTE_RADIUS, SKYBLUE);
        break;
    default: 
        DrawCircle(pos.x, pos.y, BRUTE_RADIUS, DARKBLUE);
        break;
    }
    DrawCircleLines(pos.x, pos.y, BRUTE_RADIUS, BLACK);
}

void Brute::update() {
    attack();
    movementUpdate();

    if (Vector2Length(velo) > MAX_VELOCITY_BRUTE)
        velo = SetMagnitude(velo, MAX_VELOCITY_BRUTE);
}