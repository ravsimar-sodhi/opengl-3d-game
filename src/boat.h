#include "main.h"
#include "fireball.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float y, float z);
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 accel;
    float rotation;
    bool windOn;
    bool firing;
    float topSpeed;
    bounding_box_t bounding_box();
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    void left();
    void right();
    void fire();
    void forward();
    void gravityEffect();
    void waterEffect();
    void windEffect();
    void fireEffect();
    void jump();
    Fireball bullet;
private:
    VAO *base;
    VAO *side;
    VAO *face;
    VAO *pole;
    VAO *cannon;
    VAO *sail;
};

#endif // BOAT_H
