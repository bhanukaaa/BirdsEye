#ifndef GAME_H
#define GAME_H

#include <vector>
#include "classes.h"

void loadAudio();
void unloadAudio();

void playSelect();
void playNegative();
void playPowerup();
void playDamage();

void loadTextures();
void unloadTextures();

void infoScreen();
void introScreen();
bool endScreen(float);
void spawnEnemy(std::vector<Enemy*> *);

void refreshHue(Color *);

#endif