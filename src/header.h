#ifndef HEADER_H
#define HEADER_H

// includes
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>

// screen
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define DANGER_ZONE 200

// player
#define RADIUS 20
#define MAX_ACCELERATION 0.4f
#define MAX_VELOCITY 5
#define TURRET_LENGTH 20
#define SHIP_SCALE 0.55f

// entity
#define WALL_BOUNCE -0.1

// bullet
#define BULLET_VELOCITY 0.8
#define BULLET_RADIUS 3
#define BULLET_SCALE 0.6f

// enemy
#define MAX_VELOCITY_ENEMY 3
#define MAX_ACCELERATION_ENEMY 0.4
#define EVADE_THRESHOLD 150
#define EVASION_MULTIPLIER 0.6
#define PURSUIT_THRESHOLD 1200
#define BRUTE_RADIUS 25
#define MAX_VELOCITY_BRUTE 1.5
#define MAX_VELOCITY_SPRINTER 5

// powerup
#define POWERUP_RADIUS 25


// ---Prototypes----------------------------------------

class Bullet;
class Enemy;

void playSingle();
void playBurst();
void playSelect();
void playNoAmmo();
void playForbidden();
void playNegative();
void playPowerup();
void playDamage();

void infoScreen();
void introScreen();
bool endScreen(float);
void spawnEnemy(std::vector<Enemy*> *);

Vector2 SetMagnitude(Vector2, float);

// -----------------------------------------------------
class Entity {
    private:
    public:
        Entity();

        Vector2 velo;
        Vector2 acce;
        Vector2 pos;

        // called by child classes to update position and velocity
        // will also prevent out of bounds
        void movementUpdate();
};

// -----------------------------------------------------
class Player : public Entity {
    private:
        Vector2 turretTip;
        Vector2 origin;
        Rectangle source;
    public:
        Player(Texture *);

        short ammo;
        float direct;
        short cooldown;
        short maxVelocity;
        std::vector<Bullet> bullets;
        Texture* TEX_ship;

        void update();
        void render();
        void shoot();
        void shotgun();
        void machinegun();
        void move(short, short);
};

class Bullet {
    private:
        float angle;
    public:
        Bullet(Vector2, Vector2);

        Vector2 pos;
        Vector2 velo;
        bool active;

        void update();
        void render();
};

// -----------------------------------------------------
class Enemy : public Entity {
    private:
    public:
        Enemy();

        bool alive;
        short type;
        short hp;

        void attack();
        void evade(Vector2);
        virtual void update();
        virtual void render();

        virtual ~Enemy() {};
        virtual bool checkPlayer(Vector2);
};

class Swiper : public Enemy {
    private:
    public:
        Swiper();

        bool checkPlayer(Vector2 playerPos);
        void pursue(Vector2);
        void render() override;
        
        virtual ~Swiper() {};
};

class Brute : public Enemy {
    private:
    public:
        Brute();

        void update() override;
        void render() override;

        virtual ~Brute() {};
};

class Sprinter : public Enemy {
    private:
    public:
        Sprinter();

        void render() override;
        void update() override;

        virtual ~Sprinter() {};
};

// -----------------------------------------------------
class PowerUp {
    private:
    public:
        PowerUp(Vector2);

        Vector2 pos;
        short type;
        bool active;
        short angle;

        void render();
};

#endif