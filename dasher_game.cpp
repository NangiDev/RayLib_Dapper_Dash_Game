#include <string.h>
#include "raylib.h"

const int WIDTH = 640;
const int HEIGHT = 320;
const char *TITLE = "Dapper-Dasher";
const int FPS = 60;

int main()
{
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    const int gravity{1'000};
    const int jumpVel{-600};

    bool isInAir{false};
    int velocity{0};

    const char *scarfyFilepath = "textures/scarfy.png";
    Texture2D scarfy = LoadTexture(scarfyFilepath);
    Rectangle scarfyRect;
    scarfyRect.width = scarfy.width / 6;
    scarfyRect.height = scarfy.height;
    scarfyRect.x = 0;
    scarfyRect.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = WIDTH / 2 - scarfyRect.width / 2;
    scarfyPos.y = HEIGHT - scarfyRect.height;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        const float dT{GetFrameTime()};

        if (scarfyPos.y >= HEIGHT - scarfyRect.height)
        {
            isInAir = false;
            velocity = 0;
        }
        else
        {
            isInAir = true;
            velocity += gravity * dT;
        }

        if (IsKeyPressed(KEY_SPACE) && !isInAir)
            velocity += jumpVel;

        scarfyPos.y += velocity * dT;

        DrawTextureRec(scarfy, scarfyRect, scarfyPos, WHITE);

        EndDrawing();
    }
    UnloadTexture(scarfy);
    CloseWindow();
    return 0;
}