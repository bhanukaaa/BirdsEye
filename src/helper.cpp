#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "classes.h"

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

void refreshHue(Color *color) {
    *color = {(unsigned char) GetRandomValue(100, 255), (unsigned char) GetRandomValue(100, 255), (unsigned char) GetRandomValue(100, 255), 255};
}

Vector2 SetMagnitude(Vector2 vec, float mag) {
    return Vector2Scale(Vector2Normalize(vec), mag);
}