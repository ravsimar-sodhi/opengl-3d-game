#include "barrel.h"
#include "main.h"

Barrel::Barrel(float x, float y, float z, float rot, float rad)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = rot;
    static GLfloat barrel_vertex_buffer_data[7000];
    int i = 0;
    bool alt = 0;
    this->radius = rad;
    float angle = 1;
    for (float theta = 0; theta < 361.0;)
    {
        barrel_vertex_buffer_data[i] = 0.0 - radius * cos(theta * M_PI / 180.0);
        barrel_vertex_buffer_data[i+1] = -3.0;
        barrel_vertex_buffer_data[i+2] = 0.0 - radius * sin(theta * M_PI / 180.0);

        barrel_vertex_buffer_data[i+3] = 0.0 - radius * cos(theta * M_PI / 180.0);
        barrel_vertex_buffer_data[i+4] = 3.0;
        barrel_vertex_buffer_data[i+5] = 0.0 - radius * sin(theta * M_PI / 180.0);

        if (!alt)
        {
            barrel_vertex_buffer_data[i+6] = 0.0 - radius * cos((theta + angle) * M_PI / 180.0);
            barrel_vertex_buffer_data[i+7] = 3.0;
            barrel_vertex_buffer_data[i+8] = 0.0 - radius * sin((theta + angle) * M_PI / 180.0);
            theta += angle;
        }
        else
        {
            barrel_vertex_buffer_data[i+6] = 0.0 - radius * cos((theta - angle) * M_PI / 180.0);
            barrel_vertex_buffer_data[i+7] = -3.0;
            barrel_vertex_buffer_data[i+8] = 0.0 - radius * sin((theta - angle) * M_PI / 180.0);
        }
        i = i+9;
        alt = !alt;
    }
    this->object = create3DObject(GL_TRIANGLES, i / 3, barrel_vertex_buffer_data, COLOR_BROWN2);
}

void Barrel::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    // glm::mat4 translate = glmq::translate (glm::vec3(0,0,0));    // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Barrel::set_position(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
}

void Barrel::tick()
{
    if(this->position.z < -1.5)
    {
        this->speed.z = 0.05;
    }
    if(this->position.z > 0.5)
    {
        this->speed.z = -0.05;
    }
    this->position += this->speed;
}
bounding_box_t Barrel::bounding_box()
{
    bounding_box_t bbox = {this->position.x, this->position.y, this->position.z, 2 * this->radius, 2 * this->radius, 2 * this->radius};
    return bbox;
}