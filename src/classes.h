#ifndef CLASSES_H
#define CLASSES_H

#include <raylib.h>
#include <vector>

class Bullet;
class Enemy;

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

class Player : public Entity {
    private:
        Vector2 turretTip;
    public:
        Player();

        short ammo;
        float direct;
        short cooldown;
        short maxVelocity;
        std::vector<Bullet> bullets;

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

class PowerUp {
    private:
        short angle;
    public:
        PowerUp(Vector2);

        Vector2 pos;
        short type;
        bool active;

        void render();
};

#endif