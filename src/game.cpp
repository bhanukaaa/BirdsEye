#include "header.h" // contains includes, macros and class definitions

Sound AUDIO_single;
Sound AUDIO_burst;
Sound AUDIO_select;
Sound AUDIO_noAmmo;
Sound AUDIO_forbidden;
Sound AUDIO_negative;
Sound AUDIO_powerup;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bird's Eye");
    InitAudioDevice();
    SetMasterVolume(0.25);
    SetTargetFPS(60);

    AUDIO_single = LoadSound("ogg/single.ogg");
    AUDIO_burst = LoadSound("ogg/burst.ogg");
    AUDIO_select = LoadSound("ogg/select.ogg");
    AUDIO_noAmmo = LoadSound("ogg/noAmmo.ogg");
    AUDIO_forbidden = LoadSound("ogg/forbidden.ogg");
    AUDIO_negative = LoadSound("ogg/negative.ogg");
    AUDIO_powerup = LoadSound("ogg/powerup.ogg");

    introScreen();

RESTART_LABEL:
    infoScreen();

    Player player;
    std::vector<Enemy*> enemies;
    std::vector<PowerUp> powerups;

    float score = 0;
    short moveX, moveY;
    short transAlpha = 0;
    short wave = 0, lives = 5;
    short mgTime = -1, infTime = -1, speedTime = -1;

    // game
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // GUI ig?
        DrawRectangle(0, 0, DANGER_ZONE, SCREEN_HEIGHT, {253, 158, 52, 40});
        DrawText(TextFormat("Wave: %d", wave), (SCREEN_WIDTH - 154) / 2, 10, 40, {255, 255, 255, 120});
        DrawText(TextFormat("Score: %.0f", score), 10, 10, 30, {255, 255, 255, 200});
        DrawText(TextFormat("HP: %d", lives), 10, SCREEN_HEIGHT - 50, 40, {255, 255, 255, 255});
        DrawText(TextFormat("Ammo: %d", player.ammo), SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50, 40, {255, 255, 255, 150});

        // --------------------------------------------
        if (lives <= 0) break; // game over
        if (player.cooldown) player.cooldown -= 1;
        score += 0.05;

        if (transAlpha > 0) {
            DrawText(TextFormat("Wave %d", wave), (SCREEN_WIDTH - MeasureText(TextFormat("Wave %d", wave), 100)) / 2,
                    (SCREEN_HEIGHT - 100) / 2, 100, {255, 255, 255, (unsigned char) transAlpha});
            transAlpha -= 3;
        }
        
        if (enemies.size() == 0) {
            score += wave * 100;
            wave++;
            transAlpha = 255;
            // spawn enemies upon wave end
            for (int i = 0; i < 3 * powf(1.3f, wave) * powf(0.85f, wave); i++)
                spawnEnemy(&enemies);

            if (wave % 3 == 2) { // randomly spawn powerups
                if (GetRandomValue(0, 1)) powerups.push_back(PowerUp(player.pos));
            }
        }

        if (speedTime != -1) {
            // speed boost powerup
            DrawRectangle(DANGER_ZONE, 0, SCREEN_WIDTH - DANGER_ZONE, SCREEN_HEIGHT, {20, 0, 90, 50});
            if (speedTime < GetTime()) {
                speedTime = -1;
                player.maxVelocity = MAX_VELOCITY;
            }
        }

        for (size_t b = 0; b < player.bullets.size(); b++) {
            // update and render/despawn
            player.bullets[b].update();
            if (player.bullets[b].active) player.bullets[b].render();
            else player.bullets.erase(player.bullets.begin() + b);

            for (size_t e = 0; e < enemies.size(); e++) {
                if (!player.bullets[b].active) continue;
                if (!enemies[e]->alive) continue;

                short dist = Vector2Distance(player.bullets[b].pos, enemies[e]->pos);

                if (enemies[e]->type != 3 && dist <= BULLET_RADIUS + RADIUS) {
                    enemies[e]->alive = false;

                    switch (enemies[e]->type) {
                        case 1: // wanderer
                            score += 30;
                            break;
                        case 2: // swiper
                            score += 70;
                            break;
                        case 4: // sprinter
                            score += 150;
                            player.ammo += 10;
                    }
                }
                else if (enemies[e]->type == 3 && dist <= BULLET_RADIUS + BRUTE_RADIUS) {
                    // brute
                    enemies[e]->hp--;
                    player.bullets[b].active = false;

                    if (enemies[e]->hp == 0) {
                        enemies[e]->alive = false;
                        score += 150;
                        player.ammo += 5;
                    }
                }
                else if (dist <= EVADE_THRESHOLD)
                    enemies[e]->evade(player.bullets[b].pos);
            }
        }

        for (size_t e = 0; e < enemies.size();) {
            if (player.ammo > 0 && enemies[e]->type == 2 && infTime == -1 && enemies[e]->checkPlayer(player.pos)) {
                // swiper pursuit
                playNegative();
                player.ammo = 0;
            }

            if (enemies[e]->pos.x < DANGER_ZONE) {
                // reached target
                playNegative();
                lives -= 1;
                enemies[e]->alive = false;
            }

            if (enemies[e]->alive) {
                enemies[e]->update();
                enemies[e]->render();
                e++;
            }
            else { // remove dead enemies
                delete enemies[e];
                enemies.erase(enemies.begin() + e);
            }
        }

        for (size_t p = 0; p < powerups.size(); p++) {
            short dist = Vector2Distance(powerups[p].pos, player.pos);

            powerups[p].render();
            if (dist < POWERUP_RADIUS + RADIUS) {
                // powerup collected
                playPowerup();
                switch (powerups[p].type) {
                    case 1: // machine gun
                        mgTime = GetTime() + 6;
                        if (player.ammo < 20) player.ammo = 20;
                        break;
                    case 2: // inf ammo
                        infTime = GetTime() + 10;
                        break;
                    case 3: // super speed
                        speedTime = GetTime() + 15;
                        player.maxVelocity = 8;
                        break;
                }
                powerups.erase(powerups.begin() + p);
            } 
        }


        moveX = moveY = 0;
        // allows for keycombo movement
        if (IsKeyDown(KEY_W)) moveY -= 1;
        if (IsKeyDown(KEY_S)) moveY += 1;
        if (IsKeyDown(KEY_A)) moveX -= 1;
        if (IsKeyDown(KEY_D)) moveX += 1;
        player.move(moveX, moveY);

        if (infTime != -1) {
            // inf ammo powerup
            DrawRectangle(SCREEN_WIDTH - 210, SCREEN_HEIGHT - 50, 210, 50, BLACK);
            DrawRectangle(DANGER_ZONE, 0, SCREEN_WIDTH - DANGER_ZONE, SCREEN_HEIGHT, {150, 0, 0, 25});
            DrawText("Ammo: INF", SCREEN_WIDTH - 210, SCREEN_HEIGHT - 50, 40, {255,0,0,255});

            if (infTime > GetTime()) {
                if (player.ammo < 20) player.ammo = 20;
            } else infTime = -1;
        }

        if (mgTime != -1) {
            // machine gun powerup
            if (mgTime > GetTime()) player.machinegun();
            else mgTime = -1;
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) player.shoot();
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) player.shotgun();

        player.update();
        player.render();

        // -------------------------------------------
        EndDrawing();
    }
    playSelect();

    // cleanup
    powerups.clear();
    for (size_t e = 0; e < enemies.size();) {
        delete enemies[e];
        enemies.erase(enemies.begin() + e);
    }

    if (endScreen(score)) goto RESTART_LABEL;

    UnloadSound(AUDIO_burst);
    UnloadSound(AUDIO_forbidden);
    UnloadSound(AUDIO_negative);
    UnloadSound(AUDIO_noAmmo);
    UnloadSound(AUDIO_select);
    UnloadSound(AUDIO_single);
    UnloadSound(AUDIO_powerup);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

// helper functions
Vector2 SetMagnitude(Vector2 vec, float mag) {
    return Vector2Scale(Vector2Normalize(vec), mag);
}

void playSingle() { PlaySound(AUDIO_single); }
void playBurst() { PlaySound(AUDIO_burst); }
void playSelect() { PlaySound(AUDIO_select); }
void playNoAmmo() { PlaySound(AUDIO_noAmmo); }
void playForbidden() { PlaySound(AUDIO_forbidden); }
void playNegative() { PlaySound(AUDIO_negative); }
void playPowerup() { PlaySound(AUDIO_powerup); }

void introScreen() {
    short alpha = 255;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Bird's Eye", (SCREEN_WIDTH - MeasureText("Bird's Eye", 160)) / 2, -50 + (SCREEN_HEIGHT - 160) / 2, 160, WHITE);
        DrawText("Space to Start", (SCREEN_WIDTH - MeasureText("Space to Start", 50)) / 2, 70 + (SCREEN_HEIGHT - 50) / 2, 50, GRAY);
        DrawText("v1.4", 10, SCREEN_HEIGHT - 35, 25, GRAY);

        if (alpha > 0) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0, 0, (unsigned char) alpha});
            alpha -= 5;
        }

        if (IsKeyPressed(KEY_SPACE)) break;
        EndDrawing();
    }
    playSelect();
}

void infoScreen() {
    bool release = false;
    short alpha = 255, angle = 0;
    while (!WindowShouldClose()) {
        angle++; if (angle > 360) angle = 0;
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Controls", 40, 40, 60, WHITE);
        DrawText("WASD to Move", 40, 110, 40, LIGHTGRAY);
        DrawText("Mouse to Aim", 40, 160, 40, LIGHTGRAY);
        DrawText("LMB: Single Fire", 120, 210, 30, LIGHTGRAY);
        DrawText("RMB: Burst Fire", 120, 250, 30, LIGHTGRAY);

        DrawText("Refill Ammo at Base", 40, SCREEN_HEIGHT - 125, 40, LIGHTGRAY);
        DrawText("Burst Fire unavailable at Base", 40, SCREEN_HEIGHT - 80, 40, LIGHTGRAY);

        DrawText("Enemies", 600, 40, 60, WHITE);

        DrawCircle(600 + RADIUS, 130 + RADIUS, RADIUS, ORANGE);
        DrawText("Wanderer", 640 + RADIUS, 130, 40, WHITE);
        DrawText("Basic Enemy, Proceeds towards target", 660 + RADIUS + MeasureText("Wanderer", 40), 140, 25, LIGHTGRAY);

        DrawCircle(600 + RADIUS, 230 + RADIUS, RADIUS, MAROON);
        DrawText("Swiper", 640 + RADIUS, 230, 40, WHITE);
        DrawText("Chases Player to swipe all ammo if in range", 660 + RADIUS + MeasureText("Swiper", 40), 240, 25, LIGHTGRAY);

        DrawCircle(600 + RADIUS, 330 + RADIUS, RADIUS, WHITE);
        DrawText("Sprinter", 640 + RADIUS, 330, 40, WHITE);
        DrawText("Capable of much higher speeds and acceleration", 660 + RADIUS + MeasureText("Sprinter", 40), 340, 25, LIGHTGRAY);

        DrawCircle(600 + RADIUS, 430 + RADIUS, BRUTE_RADIUS, DARKBLUE);
        DrawText("Brute", 640 + RADIUS, 430, 40, WHITE);
        DrawText("Can take multiple hits and block bullets", 660 + RADIUS + MeasureText("Brute", 40), 440, 25, LIGHTGRAY);

        DrawText("Power Ups", (SCREEN_WIDTH / 2) + 300, 100 + (SCREEN_HEIGHT) / 2, 60, WHITE);

        DrawPoly({350 + (SCREEN_WIDTH) / 2, 210 + (SCREEN_HEIGHT) / 2}, 5, POWERUP_RADIUS, angle, {0, 128, 255, 255});
        DrawPoly({350 + (SCREEN_WIDTH) / 2, 210 + (SCREEN_HEIGHT) / 2}, 5, POWERUP_RADIUS - 7, angle, {101, 243, 8, 255});

        DrawPoly({450 + (SCREEN_WIDTH) / 2, 210 + (SCREEN_HEIGHT) / 2}, 5, POWERUP_RADIUS, angle, {243, 8, 223, 255});
        DrawPoly({450 + (SCREEN_WIDTH) / 2, 210 + (SCREEN_HEIGHT) / 2}, 5, POWERUP_RADIUS - 7, angle, {101, 243, 8, 255});

        DrawPoly({550 + (SCREEN_WIDTH) / 2, 210 + (SCREEN_HEIGHT) / 2}, 5, POWERUP_RADIUS, angle, {255, 128, 0, 255});
        DrawPoly({550 + (SCREEN_WIDTH) / 2, 210 + (SCREEN_HEIGHT) / 2}, 5, POWERUP_RADIUS - 7, angle, {101, 243, 8, 255});
        
        DrawText("Space to Start", (SCREEN_WIDTH - MeasureText("Space to Start", 50)) / 2, 200 + (SCREEN_HEIGHT - 50) / 2, 50, GRAY);
        
        if (alpha > 0) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0, 0, (unsigned char) alpha});
            alpha -= 5;
        }

        if (release && IsKeyPressed(KEY_SPACE)) break;
        else if (IsKeyReleased(KEY_SPACE)) release = true;
        EndDrawing();
    }
    playSelect();

}

bool endScreen(float score) {
    short alpha = 255;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("Score: %.2f", score), (SCREEN_WIDTH - MeasureText(TextFormat("Score: %.2f", score), 70)) / 2, -130 + (SCREEN_HEIGHT - 70) / 2, 70, WHITE);
        DrawText("Game Over", (SCREEN_WIDTH - MeasureText("Game Over", 130)) / 2, (SCREEN_HEIGHT - 130) / 2, 130, WHITE);
        DrawText("Press Space to Retry", (SCREEN_WIDTH - MeasureText("Press Space to Retry", 40)) / 2, 120 + (SCREEN_HEIGHT - 40) / 2, 40, LIGHTGRAY);
        DrawText("Press Esc to Exit", (SCREEN_WIDTH - MeasureText("Press Esc to Exit", 30)) / 2, 190 + (SCREEN_HEIGHT - 30) / 2, 30, GRAY);

        if (alpha > 0) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0, 0, (unsigned char) alpha});
            alpha -= 5;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            playSelect();
            return true;
        }
        EndDrawing();
    }
    return false;
}

void spawnEnemy(std::vector<Enemy*> *enemies) {
    short enemyRandomizer = GetRandomValue(0, 7);

    if (enemyRandomizer == 0)
        enemies->push_back(new Swiper());
    else if (enemyRandomizer == 1)
        enemies->push_back(new Brute());
    else if (enemyRandomizer == 2)
        enemies->push_back(new Sprinter());
    else
        enemies->push_back(new Enemy());
}