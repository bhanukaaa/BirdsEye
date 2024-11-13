#include <raylib.h>
#include <raymath.h>

#include "classes.h"
#include "constants.h"

void playSingle();
void playBurst();
void playNoAmmo();
void playForbidden();

Vector2 SetMagnitude(Vector2, float);

extern Texture2D TEX_ship;
extern Vector2 RENDER_ship_origin;
extern Rectangle RENDER_ship_source;

Player::Player() {
    // default values
    pos = {100, 400};
    direct = 90.0f * DEG2RAD;
    turretTip = {pos.x + cos(direct) * 40, pos.y - sin(direct) * 40};
    maxVelocity = MAX_VELOCITY;
    ammo = 20;
    cooldown = 0;
}

void Player::move(short x, short y) {
    // acceleration update
    acce = {(float) x * MAX_ACCELERATION, (float) y * MAX_ACCELERATION};
}

void Player::update() {
    movementUpdate(); // parent function call
    if (Vector2Length(velo) > maxVelocity)
        velo = SetMagnitude(velo, maxVelocity);

    if (ammo < 20 && pos.x + RADIUS < DANGER_ZONE)
        ammo = 20; // reload at base
}

void Player::render() {
    // update tip position w/ direction
    direct = atan2(GetMouseX() - pos.x, GetMouseY() - pos.y) - PI / 2;
    turretTip = {pos.x + cos(direct) * TURRET_LENGTH, pos.y - sin(direct) * TURRET_LENGTH};
    DrawTexturePro(TEX_ship, RENDER_ship_source, {pos.x, pos.y, (float) TEX_ship.width, (float) TEX_ship.height}, RENDER_ship_origin, -direct * RAD2DEG, WHITE);
}

void Player::shoot() {
    if (cooldown) return;

    if (ammo <= 0) {
        DrawRectangle(DANGER_ZONE - 2, 0, 5, SCREEN_HEIGHT, RED);
        DrawText("Ammo: 0", SCREEN_WIDTH - 203, SCREEN_HEIGHT - 53, 40, RED);
        playNoAmmo();
        return;
    }

    // spawn new bullet, velocity scales with player velocity
    Bullet newBullet(turretTip, Vector2Add(Vector2Subtract(turretTip, pos), velo));
    bullets.push_back(newBullet);
    ammo -= 1;
    cooldown = 10;

    // audio
    playSingle();
}

void Player::shotgun() {
    if (cooldown) return;

    if (pos.x + RADIUS < DANGER_ZONE) {
        // no burst fire in base
        DrawRectangle(0, 0, DANGER_ZONE, SCREEN_HEIGHT, {149, 0, 255, 50});
        DrawRectangle(DANGER_ZONE - 2, 0, 5, SCREEN_HEIGHT, PURPLE);
        playForbidden();
        return;
    }
    if (ammo < 3) {
        // no burst fire wo enough ammo
        DrawRectangle(DANGER_ZONE - 2, 0, 5, SCREEN_HEIGHT, RED);
        DrawText(TextFormat("Ammo: %d", ammo), SCREEN_WIDTH - 203, SCREEN_HEIGHT - 53, 40, RED);
        playNoAmmo();
        return;
    }

    // spawn 3 bullets, with angle offset for spread
    Bullet new1(turretTip, Vector2Subtract(turretTip, pos));
    Bullet new2(turretTip, Vector2Rotate(Vector2Subtract(turretTip, pos), 0.03));
    Bullet new3(turretTip, Vector2Rotate(Vector2Subtract(turretTip, pos), -0.03));
    bullets.push_back(new1);
    bullets.push_back(new2);
    bullets.push_back(new3);
    ammo -= 3;
    cooldown = 20;

    // audio
    playBurst();
}

void Player::machinegun() {
    if (cooldown) return;

    // spawn new bullet, velocity scales with player velocity
    Bullet newBullet(turretTip, Vector2Add(Vector2Subtract(turretTip, pos), velo));
    bullets.push_back(newBullet);
    playSingle();
    cooldown = 6;
}