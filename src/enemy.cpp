# include "enemy.h"
# include "main.h"

Enemy::Enemy(float x, float y, float z, float rad)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    static GLfloat base_vertex_buffer_data[30000];
    int i = 0; 
    bool alt = 0;
    float change = 50;
    this->radius = rad;
    this->speed = glm::vec3(getRandDouble(-0.3,0.3), getRandDouble(-0.3,0.3),0);
    for (float alpha = 0; alpha < 181.0; alpha++)
    {
        for (float theta = 0; theta < 361.0; theta += change)
        {
            base_vertex_buffer_data[i] = this->radius * sin(alpha * M_PI / 180.0) * cos(theta * M_PI / 180.0);
            base_vertex_buffer_data[i+1] = 2*this->radius * cos(alpha * M_PI / 180.0);
            base_vertex_buffer_data[i+2] = this->radius * sin(alpha * M_PI / 180.0) * sin(theta * M_PI / 180.0);

            base_vertex_buffer_data[i+3] = this->radius * sin((alpha + 1) * M_PI / 180.0) * cos(theta * M_PI / 180.0);
            base_vertex_buffer_data[i+4] = this->radius * cos((alpha + 1) * M_PI / 180.0);
            base_vertex_buffer_data[i+5] = this->radius * sin((alpha + 1) * M_PI / 180.0) * sin(theta * M_PI / 180.0);

            if (!alt)
            {
                base_vertex_buffer_data[i+6] = this->radius * sin((alpha + 1) * M_PI / 180.0) * cos((theta + change) * M_PI / 180.0);
                base_vertex_buffer_data[i+7] = this->radius * sin((alpha + 1) * M_PI / 180.0);
                base_vertex_buffer_data[i+8] = this->radius * sin((alpha + 1) * M_PI / 180.0) * sin((theta + change) * M_PI / 180.0);
            }
            else
            {
                base_vertex_buffer_data[i+6] = this->radius * sin(alpha * M_PI / 180.0) * cos((theta - change) * M_PI / 180.0);
                base_vertex_buffer_data[i+7] = 2*this->radius * sin(alpha * M_PI / 180.0);
                base_vertex_buffer_data[i+8] = this->radius * sin(alpha * M_PI / 180.0) * sin((theta - change) * M_PI / 180.0);
                theta -= change;
            }
            alt = !alt;
            i+=9;
        }
    }
    this->object = create3DObject(GL_TRIANGLES, i / 3, base_vertex_buffer_data, COLOR_PURPLE);
}

void Enemy::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 1, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Enemy::set_position(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
}

void Enemy::tick()
{
    this->rotation = this->rotation + 2 ;
    this->position += this->speed;
}

bounding_box_t Enemy::bounding_box()
{
    bounding_box_t bbox = {this->position.x, this->position.y, this->position.z, 4 * this->radius, 4 * this->radius, 4 * this->radius};
    return bbox;
}