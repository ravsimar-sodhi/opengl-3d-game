#include "main.h"

#ifndef WATER_H
#define WATER_H

class Water
{
    public:
    Water() {}
    Water(float x, float y, float z,float size);
    glm::vec3 position;
    float rotation;
    float size;
    void draw(glm::mat4 VP);
    double speed;
    void left();
    void right();

    private:
    VAO* surface;

};

#endif // BOAT_H
