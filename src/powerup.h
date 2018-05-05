#include "main.h"

#ifndef POWERUP_H
#define POWERUP_H

class Powerup
{
  public:
    Powerup() {}
    Powerup(float x, float y, float z, float rot);
    glm::vec3 position;
    glm::vec3 speed;
    // glm::vec3 accel;
    float rotation;
    bounding_box_t bounding_box();
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();

  private:
    VAO *object;
};

#endif // POWERUP_H
