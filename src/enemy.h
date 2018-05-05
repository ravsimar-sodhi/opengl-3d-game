#include "main.h"

#ifndef ENEMY_H
#define ENEMY_H

class Enemy
{
  public:
    Enemy() {}
    Enemy(float x, float y, float z, float siz);
    glm::vec3 position;
    glm::vec3 speed;
    // glm::vec3 accel;
    float rotation;
    float radius;
    bounding_box_t bounding_box();
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();

  private:
    VAO *object;
};

#endif // BOAT_H
