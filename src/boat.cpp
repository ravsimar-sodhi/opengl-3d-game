#include "boat.h"
#include "main.h"

Boat::Boat(float x, float y, float z) 
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->speed = glm::vec3(0, 0, 0);
    this->accel = glm::vec3(0, 0, 0);
    this->firing = 0;
    this->windOn = 0;
    this->bullet = Fireball(this->position.x + 7*sin(this->rotation * M_PI / 180.0f), this->position.y+7*cos(this->rotation * M_PI / 180.0f), this->position.z+3, 0, 0.4);
    this->topSpeed = 0.5;

    GLfloat base_vertex_buffer_data[] = 
    {
        -1.0f, 2.0f, -1.0f,
        1.0f, -2.0f, -1.0f,
        -1.0f, -2.0f, -1.0f,

        1.0f, -2.0f, -1.0f,
        1.0f, 2.0f, -1.0f,
        -1.0f, 2.0f, -1.0f,
    };
    GLfloat side_vertex_buffer_data[] = 
    {
        -2.0f, 4.0f, 1.0f,
        -1.0f, 2.0f, -1.0f,
        -1.0f, -2.0f, -1.0f,

        -2.0f, -4.0f, 1.0f,
        -2.0f, 4.0f, 1.0f,
        -1.0f, -2.0f, -1.0f,

        2.0f, 4.0f, 1.0f,
        1.0f, 2.0f, -1.0f,
        1.0f, -2.0f, -1.0f,

        2.0f, -4.0f, 1.0f,
        2.0f, 4.0f, 1.0f,
        1.0f, -2.0f, -1.0f,

    };
    GLfloat face_vertex_buffer_data[] = 
    {
        0.0f, -4.0f, 1.0f,
        -1.0f, -2.0f, -1.0f,
        -2.0f, -4.0f, 1.0f,

        0.0f, -4.0f, 1.0f,
        1.0f, -2.0f, -1.0f,
        2.0f, -4.0f, 1.0f,

        1.0f, -2.0f, -1.0f,
        -1.0f, -2.0f, -1.0f,
        0.0f, -4.0f, 1.0f,

        0.0f, 5.0f, 2.0f,
        -1.0f, 2.0f, -1.0f,
         -2.0f, 4.0f, 1.0f,

        0.0f, 5.0f, 2.0f,
        1.0f, 2.0f, -1.0f,
        2.0f, 4.0f, 1.0f,

        1.0f, 2.0f, -1.0f,
        -1.0f, 2.0f, -1.0f,
        0.0f, 5.0f, 2.0f,

    };
    GLfloat pole_vertex_buffer_data[] = 
    {
        0.1f, 1.7f, -1.0f,
        -0.1f, 1.9f, -1.0f,
        0.1f, 1.9f, -1.0f,

        0.1f, 1.7f, -1.0f,
        -0.1f, 1.7f, -1.0f,
        -0.1f, 1.9f, -1.0f,

        0.1f, 1.7f, 6.0f,
        -0.1f, 1.9f, 6.0f,
        0.1f, 1.9f, 6.0f,

        0.1f, 1.7f, 6.0f,
        -0.1f, 1.7f, 6.0f,
        -0.1f, 1.9f, 6.0f,

        0.1f, 1.9f, 6.0f,
        0.1f, 1.7f, -1.0f,
        0.1f, 1.9f, -1.0f,

        0.1f, 1.9f, 6.0f,
        0.1f, 1.7f, 6.0f,
        0.1f, 1.7f, -1.0f,

        -0.1f, 1.9f, 6.0f,
        -0.1f, 1.7f, -1.0f,
        -0.1f, 1.9f, -1.0f,

        -0.1f, 1.9f, 6.0f,
        -0.1f, 1.7f, 6.0f,
        -0.1f, 1.7f, -1.0f,

        -0.1f, 1.9f, 6.0f,
        -0.1f, 1.9f, -1.0f,
        0.1f, 1.9f, -1.0f,

        0.1f, 1.9f, -1.0f,
        0.1f, 1.9f, 6.0f,
        -0.1f, 1.9f, 6.0f,

        -0.1f, 1.7f, 6.0f,
        -0.1f, 1.7f, -1.0f,
        0.1f, 1.7f, -1.0f,

        0.1f, 1.7f, -1.0f,
        0.1f, 1.7f, 6.0f,
        -0.1f, 1.7f, 6.0f,
    };
    GLfloat cannon_vertex_buffer_data[7000];
    int i = 0;
    bool flag = 0;
    float angle = 1;
    float radius = 0.5;
    for (float theta = 0; theta < 361.0; i+=9)
    {
        cannon_vertex_buffer_data[i] = 0.0 - radius * cos(theta * M_PI / 180.0);
        cannon_vertex_buffer_data[i+1] = 4.5;
        cannon_vertex_buffer_data[i+2] = 1.5 - radius * sin(theta * M_PI / 180.0);

        cannon_vertex_buffer_data[i+3] =  0.0- radius * cos(theta * M_PI / 180.0);
        cannon_vertex_buffer_data[i+4] = 7.0;
        cannon_vertex_buffer_data[i+5] = 3.0 - radius * sin(theta * M_PI / 180.0);

        if (!flag)
        {
            cannon_vertex_buffer_data[i+6] = 0.0 - radius * cos((theta + angle) * M_PI / 180.0);
            cannon_vertex_buffer_data[i+7] = 7.0;
            cannon_vertex_buffer_data[i+8] = 3.0 - radius * sin((theta + angle) * M_PI / 180.0);
            theta += angle;
        }
        else
        {
            cannon_vertex_buffer_data[i+6] = 0.0 - radius * cos((theta - angle) * M_PI / 180.0);
            cannon_vertex_buffer_data[i+7] = 4.5;
            cannon_vertex_buffer_data[i+8] = 1.5 - radius * sin((theta - angle) * M_PI / 180.0);
        }
        flag = !flag;
    }
    GLfloat sail_vertex_buffer_data[] = {
        0.0f, 1.9f, 6.0f,
        (float)cos(this->rotation * M_PI / 180.0f)*0.0f, (float)sin(this->rotation * M_PI / 180.0f) * 5.0f, 2.5f,
        0.0f, 1.9f, 2.0f,

    };
    this->base = create3DObject(GL_TRIANGLES, 2 * 3, base_vertex_buffer_data, COLOR_BROWN3);
    this->side = create3DObject(GL_TRIANGLES, 4 * 3, side_vertex_buffer_data,COLOR_BROWN1);
    this->face = create3DObject(GL_TRIANGLES, 6 * 3, face_vertex_buffer_data, COLOR_BROWN2);
    this->pole = create3DObject(GL_TRIANGLES, 10 * 3, pole_vertex_buffer_data, COLOR_GREEN);
    this->cannon = create3DObject(GL_TRIANGLES, i/3, cannon_vertex_buffer_data, COLOR_BLACK); 
    this->sail = create3DObject(GL_TRIANGLES, 3, sail_vertex_buffer_data, COLOR_RED);
}

void Boat::draw(glm::mat4 VP) 
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, -1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->base);
    draw3DObject(this->side);
    draw3DObject(this->face);
    draw3DObject(this->pole);
    draw3DObject(this->cannon);
    draw3DObject(this->sail);
    this->bullet.draw(VP);
}

void Boat::set_position(float x, float y, float z) 
{
    this->position = glm::vec3(x, y, z);
}

void Boat::gravityEffect()
{
    if (this->position.z < 0)
    {
        this->accel.z = 0;
        this->speed.z = 0;
    }
    if (this->position.z > 0)
    {
        this->accel.z = -0.1;
    }
}

void Boat::waterEffect()
{
    if (this->speed.y > 0)
    {
        this->accel.y = -0.01;
    }
    else if(this->speed.y < 0)
    {
        this->accel.y = 0.01;
    }
    if (this->speed.x > 0)
    {
        this->accel.x = -0.01;
    }
    else if(this->speed.x < 0)
    {
        this->accel.x = 0.01;
    }
}

void Boat::windEffect()
{
    if(this->windOn)
    {
        this->accel.x = getRandDouble(-0.02, 0.02);
        this->accel.y = getRandDouble(-0.02, 0.02);
        if(abs(this->speed.x) > 0.4)
        {
            if(this->speed.x > 0)
                this->speed.x = 0.4;
            else
                this->speed.x = -0.4;
        }
        if(abs(this->speed.y) > 0.4)
        {
            if(this->speed.y > 0)
                this->speed.y = 0.4;
            else
                this->speed.y = -0.4;
        }
    }
}
void Boat::fireEffect()
{
    if (this->firing == 0)
        this->bullet.position = glm::vec3(this->position.x + 7 * sin(this->rotation * M_PI / 180.0f), this->position.y + 7 * cos(this->rotation * M_PI / 180.0f), this->position.z + 3);
    this->bullet.tick();

    if (this->bullet.position.z < -0.5)
    {
        this->firing = 0;
        this->bullet.accel = glm::vec3(0, 0, 0);
        this->bullet.speed = glm::vec3(0, 0, 0);
        this->bullet.position = glm::vec3(this->position.x + 6 * sin(this->rotation * M_PI / 180.0f), this->position.y + 6 * cos(this->rotation * M_PI / 180.0f), this->position.z + 2);
    }
}
void Boat::tick() 
{
    this->position += this->speed;
    this->speed += this->accel;
    
    gravityEffect();
    waterEffect();
    windEffect();
    fireEffect();
    
    GLfloat sail_vertex_buffer_data[] = {
        0.0f, 1.9f, 6.0f,
        (float)sin((this->rotation - 90) * M_PI / 180.0f) * 5.0f, (float)cos((this->rotation - 90) * M_PI / 180.0f) * 5.0f, 2.5f,
        0.0f, 1.9f, 2.0f,

    };
    this->sail = create3DObject(GL_TRIANGLES, 3, sail_vertex_buffer_data, COLOR_RED);

    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
// boat1.speed.z = 0;
void Boat::jump()
{
    if(this->position.z <= 0)
        this->speed.z = 2; 
}
void Boat::right() 
{
    this->rotation += 1.5;
    // this->position.x += 0.5;
}
void Boat::left()
{
    this->rotation -= 1.5;
}
void Boat::forward()
{
    this->speed.y = this->topSpeed * cos(this->rotation * M_PI / 180.0f);
    this->speed.x = this->topSpeed * sin(this->rotation * M_PI / 180.0f);
}

void Boat::fire()
{
    this->firing = 1;
    this->bullet.speed = glm::vec3(1.5 * sin(this->rotation * M_PI / 180.0f),1.5 * cos(this->rotation * M_PI / 180.0f),1);
    this->bullet.accel = glm::vec3(0,0,-0.05);
}
bounding_box_t Boat::bounding_box()
{
    bounding_box_t bbox = {(float)this->position.x, (float)this->position.y, (float)this->position.z, (float)4, (float)8, (float)2};
    return bbox;
}