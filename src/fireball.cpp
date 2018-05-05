#include "fireball.h"
#include "main.h"

Fireball::Fireball(float x, float y, float z, float rot, float rad)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = rot;
    static GLfloat base_vertex_buffer_data[50000];
    int i = 0; 
    bool alt = 0;
    float change = 30;
    this->radius = rad;
    for (float alpha = 0; alpha < 181.0; alpha += 1)
    {
        for (float theta = 0; theta < 361.0; theta += change)
        {
            base_vertex_buffer_data[i++] = this->radius * sin(alpha * M_PI / 180.0) * cos(theta * M_PI / 180.0);
            base_vertex_buffer_data[i++] = this->radius * cos(alpha * M_PI / 180.0);
            base_vertex_buffer_data[i++] = this->radius * sin(alpha * M_PI / 180.0) * sin(theta * M_PI / 180.0);

            base_vertex_buffer_data[i++] = this->radius * sin((alpha + 1) * M_PI / 180.0) * cos(theta * M_PI / 180.0);
            base_vertex_buffer_data[i++] = this->radius * cos((alpha + 1) * M_PI / 180.0);
            base_vertex_buffer_data[i++] = this->radius * sin((alpha + 1) * M_PI / 180.0) * sin(theta * M_PI / 180.0);

            if (!alt)
            {
                base_vertex_buffer_data[i++] = this->radius * sin((alpha + 1) * M_PI / 180.0) * cos((theta + change) * M_PI / 180.0);
                base_vertex_buffer_data[i++] = this->radius * cos((alpha + 1) * M_PI / 180.0);
                base_vertex_buffer_data[i++] = this->radius * sin((alpha + 1) * M_PI / 180.0) * sin((theta + change) * M_PI / 180.0);
            }
            else
            {
                base_vertex_buffer_data[i++] = this->radius * sin(alpha * M_PI / 180.0) * cos((theta - change) * M_PI / 180.0);
                base_vertex_buffer_data[i++] = this->radius * cos(alpha * M_PI / 180.0);
                base_vertex_buffer_data[i++] = this->radius * sin(alpha * M_PI / 180.0) * sin((theta - change) * M_PI / 180.0);
                theta -= change;
            }
            alt = !alt;
        }
    }
    printf("%d",i);
    this->object = create3DObject(GL_TRIANGLES, i / 3, base_vertex_buffer_data, COLOR_RED);
}

void Fireball::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Fireball::set_position(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
}

void Fireball::tick()
{
    this->position += this->speed;
    this->speed += this->accel;
}
bounding_box_t Fireball::bounding_box()
{
    bounding_box_t bbox = {this->position.x, this->position.y, this->position.z, 2 * this->radius, 2 * this->radius, 2 * this->radius};
    return bbox;
}