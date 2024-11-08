#include "header.h"

Sprinter::Sprinter() {
    type = 4;
    velo = SetMagnitude(velo, MAX_VELOCITY_SPRINTER);
}

void Sprinter::render() {
    DrawCircle(pos.x - 3*velo.x, pos.y - 3*velo.y, RADIUS, {100, 100, 100, 60});
    DrawCircle(pos.x - 2*velo.x, pos.y - 2*velo.y, RADIUS,{150, 150, 150, 100});
    DrawCircle(pos.x, pos.y, RADIUS, {255, 255, 255, 255});
}

void Sprinter::update() {
    attack();
    movementUpdate();

    if (Vector2Length(velo) > MAX_VELOCITY_SPRINTER)
        velo = SetMagnitude(velo, MAX_VELOCITY_SPRINTER);
}