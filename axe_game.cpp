#include <string>
#include "raylib.h"

const int WIDTH = 350;
const int HEIGHT = 200;
const char* TITLE = "My Title of the Game";
const int FPS = 60;

int main()
{
    InitWindow(WIDTH, HEIGHT, TITLE);

    int circle_x{175};
    int circle_y{100};
    int circle_radius{25};
    Color circle_color{RED};
    int circle_step{10};

    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircle(circle_x, circle_y, circle_radius, circle_color);
        if (IsKeyDown(KEY_D) && circle_x < WIDTH)
        {
            circle_x += circle_step;
        }
        if (IsKeyDown(KEY_A) && circle_x > 0)
        {
            circle_x -= circle_step;
        }
        if (IsKeyDown(KEY_W) && circle_y > 0)
        {
            circle_y -= circle_step;
        }
        if (IsKeyDown(KEY_S) && circle_y < HEIGHT)
        {
            circle_y += circle_step;
        }
        EndDrawing();
    }
    return 0;
}