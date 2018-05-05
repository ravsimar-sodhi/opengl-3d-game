#include "main.h"
#include "timer.h"
#include "boat.h"
#include "water.h"
#include "rock.h"
#include "enemy.h"
#include "barrel.h"
#include "powerup.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

Boat Player;
Water sea;
Powerup po;
vector<Rock> rocks(150);
vector<Enemy> enemies(50);
vector<Barrel> barrels(150);

int score = 0,health = 5;
extern double drag_oldx,drag_oldy;
int colliding = 0;
int currView = 0;
int enemyKills = 0;


float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);
Timer t240(1.0 / 240);
Timer wind15(30);

double getRandDouble(double l, double r)
{
    return r-(((double)rand()) / RAND_MAX) * (r - l);
}

bool detectCollision(bounding_box_t a, bounding_box_t b)
{
    return ((abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.depth + b.depth)) &&
           (abs(a.z - b.z) * 2 < (a.height + b.height)));
}

void windHandler(bool on)
{
    if(on)
    {
        Player.windOn = 1;
        // printf("Hawa chal pi oye!\n");
    }
    else
    {
        Player.windOn = 0;
    }
}
void changeCurrView()
{
    defView = (defView + 1)%5;
}
void changeView()
{
    currView = (currView + 1) % 2;
    reset_screen();
}
void zoomCamera(double yoffset)
{
    if(defView == 1)
        eye = glm::vec3(Player.position.x + 8*yoffset, Player.position.y + 8*yoffset, Player.position.z + 8*yoffset);
}
void switchView(int x)
{
    switch (x)
    {
    case 0:
        // This is the follow-cam view
        eye = Player.position + glm::vec3(0, - 16, 9);
        target = Player.position + glm::vec3(0, 5,0);
        break;
    case 1:
        // This is the helicopter view
        target = Player.position;
        break;
    case 2:
        // This is the boat view
        eye = Player.position + glm::vec3(2* sin(Player.rotation * M_PI / 180.0f),2 * cos(Player.rotation * M_PI / 180.0f), 4);
        target = Player.position + glm::vec3(5 * sin(Player.rotation * M_PI / 180.0f), 5 * cos(Player.rotation * M_PI / 180.0f), 4);
        break;
    case 3:
        //This is the top view
        eye = Player.position + glm::vec3(0, - 20, 100);
        target = Player.position;
        break;
    case 4:
        // This is the tower view
        eye = glm::vec3(-20, -50, 50);
        target = Player.position;
        break;
    }
}
/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() 
{
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);
    switchView(defView);
    
    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    

    glm::mat4 VP = Matrices.projection * Matrices.view;

    glm::mat4 MVP;  // MVP = Projection * View * Model
    sea.draw(VP);
    Player.draw(VP);
    po.draw(VP);
    for(int i=0;i<rocks.size();i++)
    {
        rocks[i].draw(VP);
    }
    for(int i=0;i<enemies.size();i++)
    {
        enemies[i].draw(VP);
    }
    for (int i = 0; i < barrels.size(); i++)
    {
        barrels[i].draw(VP);
    }
    // bullet1.draw(VP);
}

void tick_input(GLFWwindow *window) 
{
    int left  = glfwGetKey(window, GLFW_KEY_A);
    int right = glfwGetKey(window, GLFW_KEY_D);
    int forward = glfwGetKey(window, GLFW_KEY_W);
    int jump = glfwGetKey(window, GLFW_KEY_SPACE);
    int fire = glfwGetKey(window, GLFW_KEY_F);

    if (left)
    {
        Player.left();
    }
    if(right)
    {
        Player.right();
    }
    if(forward && colliding == 0) 
    {
        Player.forward();
    }
    else
    {
        colliding = 0;
    }
    if(jump)
    {
        Player.jump();
    }
    if(fire && Player.firing == 0)
    {
        Player.fire();
    }
    int drag_pan = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (drag_pan)
    {
        if (drag_oldx == -1 || drag_oldy == -1)
        {
            glfwGetCursorPos(window, &drag_oldx, &drag_oldy);
        }
        else
        {
            double newx, newy;
            glfwGetCursorPos(window, &newx, &newy);
            int width, height;
            glfwGetWindowSize(window, &width, &height);
           
            eye = eye - glm::vec3(newx - drag_oldx,0,newy - drag_oldy);
            drag_oldx = newx;
            drag_oldy = newy;
            // printf("Mouse chalya si hune!");
            reset_screen();
        }
    }
}
void enemyHandler()
{
    for (int i = 0; i < enemies.size(); i++)
    {
        enemies[i].tick();
    }
    for (int i = 0; i < enemies.size(); i++)
    {
            // printf("boss? %d", enemyKills);
        if (Player.firing == 1 && detectCollision(Player.bullet.bounding_box(), enemies[i].bounding_box()) == 1)
        {
            enemyKills++;
            if (enemyKills >= 3)
            {
                // puts("hellooo");
                enemyKills = 0;
                Enemy Boss = Enemy(Player.position.x + 40, Player.position.y+40, sea.position.y + 5, 10);
                Boss.speed = glm::vec3(0, 0, 0);
                enemies.erase(enemies.begin() + i);
                enemies.push_back(Boss);
                // enemies.erase(enemies.begin() + i);
            }
            else
            {
                if(enemies[i].radius == 10)
                {
                    score +=50;
                    po = Powerup(enemies[i].position.x, enemies[i].position.y, 0, 0);
                }

                enemies.erase(enemies.begin() + i);
            }
            score += 100;
        }
        if (detectCollision(Player.bounding_box(), enemies[i].bounding_box()))
        {
            // enemyKills++;
            // printf("boss? %d", enemyKills);
            // if (enemyKills >= 3)
            // {
            //     enemyKills = 0;
            //     Enemy Boss = Enemy(enemies[i].position.x ,enemies[i].position.y, sea.position.y + 2, 6);
            //     Boss.speed = glm::vec3(0,0,0);
            //     enemies.erase(enemies.begin() + i);
            //     enemies.push_back(Boss);
            //     printf("boss? %d",enemyKills);
            // }
            // else
            // {
            enemies.erase(enemies.begin() + i);
            // }
            health--;
        }
    }
    
}
void rockHandler()
{
    for (int i = 0; i < rocks.size(); i++)
    {
        if (detectCollision(Player.bounding_box(), rocks[i].bounding_box()) == 1)
        {
            Player.speed.x = 0;
            Player.speed.y = 0;
            // printf("Takkar hogi patthar naal!\n");
            colliding = 1;
            health--;
            rocks.erase(rocks.begin() + i);
        }
    }
}
void barrelHandler()
{
    for (int i = 0; i < barrels.size(); i++)
    {
        barrels[i].tick();
    }
    for(int i=0;i<barrels.size();i++)
    {
        if(detectCollision(Player.bounding_box(), barrels[i].bounding_box()) == 1)
        {
            barrels.erase(barrels.begin() + i);
            score += 50;
        }
    }
}
void powerupHandler()
{
    if(detectCollision(Player.bounding_box(), po.bounding_box()))
    {
        po.position = glm::vec3(0,0, -5);
        Player.topSpeed = 1;
    }
}
void tick_elements() 
{
    Player.tick();
    rockHandler();
    enemyHandler();
    barrelHandler();
    powerupHandler();
    po.tick();
}

void initGL(GLFWwindow *window, int width, int height) 
{
    sea = Water(0, 0, -2,1500);
    Player = Boat(0, 0, 0);
    po = Powerup(0,0,-5,0);
    for(int i=0;i<rocks.size();i++)
    {
        rocks[i] = Rock(getRandDouble(-1500, 1500), 
        getRandDouble(-1500, 1500), 
        sea.position.y -2, 
        getRandDouble(0,360),
        getRandDouble(2, 5.5));
    }
    for(int i=0;i<enemies.size();i++)
    {
        enemies[i] = Enemy(getRandDouble(-750, 750),
        getRandDouble(-750, 750),
        sea.position.y + 2,
        getRandDouble(3,4));
    }
    for (int i = 0; i < barrels.size(); i++)
    {
        barrels[i] = Barrel(getRandDouble(-1500, 1500),
        getRandDouble(-1500, 1500),
        sea.position.y-2,
        getRandDouble(90,180),
        3);
    }
    eye = glm::vec3(0, - 15,10);
    target = glm::vec3(0, 5, 0);
    up = glm::vec3(0, 0, 1);
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
    reshapeWindow (window, width, height);
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);
}


int main(int argc, char **argv) 
{
    srand(time(0));
    int width  = 600;
    int height = 600;
    char title[100];

    int windTimeOut = 0;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) 
    {
        // Process timers

        if (t60.processTick()) 
        {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
            if(Player.windOn)
            {
                windTimeOut++;
                if(windTimeOut == 1000)
                {
                    // printf("Hawa rokti!");
                    windHandler(0);
                    windTimeOut = 0;
                }
            }
            if(health <= 0)
            {
                printf("Game Over: Your score is %d\n",score);
                quit(window);
            }
            sprintf(title, "Status: Wind: %d Health %d: Score: %d",Player.windOn,health,score);
            glfwSetWindowTitle(window, title);
        }
        if (t240.processTick()) 
        {

        }
        if(wind15.processTick())
        {
            windHandler(1);
        }
        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}



void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    if(currView == 1)
    {
        // printf("is it me?");
        Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    }
    else if (currView == 0)
    {
        Matrices.projection = glm::perspective(1.0f, 1.0f, 0.1f, 500.0f);
    }
}
