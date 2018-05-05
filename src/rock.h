#include "main.h"

#ifndef ROCK_H
#define ROCK_H

class Rock
{
  public:
    Rock() {}
    Rock(float x, float y, float z, float rot, float siz);
    glm::vec3 position;
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
