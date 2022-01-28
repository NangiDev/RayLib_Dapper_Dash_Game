#include <string.h>
#include "raylib.h"

const int WIDTH = 640;
const int HEIGHT = 320;
const char *TITLE = "Axe Game";
const int FPS = 60;

int main()
{
    InitWindow(WIDTH, HEIGHT, TITLE);

    int circle_x{175};
    int circle_y = (int)(HEIGHT / 2);
    int circle_radius{20};
    int circle_step{10};
    Color circle_color{BLUE};

    int r_circle_x{circle_x + circle_radius};
    int l_circle_x{circle_x - circle_radius};
    int u_circle_y{circle_y - circle_radius};
    int b_circle_y{circle_y + circle_radius};

    int axe_x = (int)(WIDTH / 2);
    int axe_y = (int)(HEIGHT / 2);
    int axe_w{25};
    int axe_h{70};
    int axe_step{10};
    Color axe_color{RED};

    int r_axe_x{axe_x + axe_w};
    int l_axe_x{axe_x};
    int u_axe_y{axe_y};
    int b_axe_y{axe_y + axe_h};

    const char *text_game_over = "Game Over!";
    int text_size{60};
    int text_length = (int)strlen(text_game_over) * text_size / 2;
    int text_x = (int)(WIDTH / 2 - text_length / 2);
    int text_y = (int)(HEIGHT / 2 - text_size / 2);
    Color text_color{RED};

    bool collision_with_axe{false};

    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        r_circle_x = circle_x + circle_radius;
        l_circle_x = circle_x - circle_radius;
        u_circle_y = circle_y - circle_radius;
        b_circle_y = circle_y + circle_radius;
        r_axe_x = axe_x + axe_w;
        l_axe_x = axe_x;
        u_axe_y = axe_y;
        b_axe_y = axe_y + axe_h;

        collision_with_axe = b_axe_y >= u_circle_y &&
                             u_axe_y <= b_circle_y &&
                             l_axe_x <= r_circle_x &&
                             r_axe_x >= l_circle_x;

        if (collision_with_axe)
        {
            DrawText(text_game_over, text_x, text_y, text_size, text_color);
        }
        else
        {
            DrawCircle(circle_x, circle_y, circle_radius, circle_color);
            DrawRectangle(axe_x, axe_y, axe_w, axe_h, axe_color);

            axe_y += axe_step;
            if (axe_y > HEIGHT - axe_h || axe_y < 0)
            {
                axe_step = -axe_step;
            }
            if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && circle_x < WIDTH - circle_radius)
            {
                circle_x += circle_step;
            }
            if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && circle_x > circle_radius)
            {
                circle_x -= circle_step;
            }
            if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && circle_y > circle_radius)
            {
                circle_y -= circle_step;
            }
            if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && circle_y < HEIGHT - circle_radius)
            {
                circle_y += circle_step;
            }
        }
        EndDrawing();
    }
    return 0;
}