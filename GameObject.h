#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct GameObject {
    int x, y, w, h;
};

struct Bullet : public GameObject {
    bool active;
};

struct Enemy : public GameObject {
    bool alive;
};
#endif 