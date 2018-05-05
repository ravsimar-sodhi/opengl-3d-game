#include "main.h"

#ifndef BARREL_H
#define BARREL_H

class Barrel
{
  public:
    Barrel() {}
    Barrel(float x, float y, float z, float rot, float siz);
    glm::vec3 position;
    glm::vec3 speed;
    float rotation;
    float radius;
    bounding_box_t bounding_box();
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
  private:
    VAO *object;
};

#endif // BARREL_H
