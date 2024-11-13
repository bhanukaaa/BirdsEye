#include <raylib.h>

#include "constants.h"

Texture2D TEX_ship;
Texture2D TEX_bullet;
Texture2D TEX_base_full;
Texture2D TEX_base_dmg;
Texture2D TEX_base_crit;
Texture2D TEX_stars_bg;

Vector2 RENDER_ship_origin;
Vector2 RENDER_bullet_origin;
Rectangle RENDER_ship_source;
Rectangle RENDER_bullet_source;

void loadTextures() {
    Image img;
    switch (GetRandomValue(1, 3)) {
        // random ship texture
        case 1:
            img = LoadImage("png/ship1.png");
            break;
        case 2:
            img = LoadImage("png/ship2.png");
            break;
        case 3:
            img = LoadImage("png/ship3.png");
            break;
    }
    ImageResize(&img, img.width * SHIP_SCALE, img.height * SHIP_SCALE);
    TEX_ship = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("png/bullet.png");
    ImageResize(&img, img.width * BULLET_SCALE, img.height * BULLET_SCALE);
    TEX_bullet = LoadTextureFromImage(img);
    UnloadImage(img);

    SetTextureFilter(TEX_ship, TEXTURE_FILTER_TRILINEAR);
    SetTextureFilter(TEX_bullet, TEXTURE_FILTER_TRILINEAR);

    // constant offset values; bullet rendering
    RENDER_bullet_source = {0, 0, (float) TEX_bullet.width, (float) TEX_bullet.height};
    RENDER_bullet_origin = {TEX_bullet.width * 0.5f, TEX_bullet.height * 0.5f};
    RENDER_ship_origin = {TEX_ship.width * 0.5f, TEX_ship.height * 0.5f};
    RENDER_ship_source = {0, 0, (float) TEX_ship.width, (float) TEX_ship.height};

    TEX_base_full = LoadTexture("png/base_100.png");
    TEX_base_dmg = LoadTexture("png/base_50.png");
    TEX_base_crit = LoadTexture("png/base_15.png");

    TEX_stars_bg = LoadTexture("png/stars.png");
}

void unloadTextures() {
    UnloadTexture(TEX_ship);
    UnloadTexture(TEX_bullet);
    UnloadTexture(TEX_base_crit);
    UnloadTexture(TEX_base_dmg);
    UnloadTexture(TEX_base_full);
    UnloadTexture(TEX_stars_bg);
}