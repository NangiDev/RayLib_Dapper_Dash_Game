#include <string.h>
#include "raylib.h"

const int WIDTH = 640 * 2;
const int HEIGHT = 320 * 2;
const char *TITLE = "Dapper-Dasher";
const int FPS = 60;
const int NUM_OF_NEBULAS = 5;

const char *scarfyFilepath = "textures/scarfy.png";
const char *nebulaFilePath = "textures/12_nebula_spritesheet.png";
const char *farFilePath = "textures/far-buildings.png";
const char *backFilePath = "textures/back-buildings.png";
const char *foregroundFilePath = "textures/foreground.png";

struct AnimData
{
    const Texture *texture;
    Rectangle rec;
    Vector2 pos;
    int velocity = 0;
    int frame = 0;
    float updateTime = 1.0 / 12.0;
    float runningTime = 0.0;
};

void initNebulas(AnimData *nebulas, Texture2D *texture)
{
    for (int i = 0; i < NUM_OF_NEBULAS; i++)
    {
        AnimData nebulaData{
            texture,                                                                   // texture
            {0.0, 0.0, (float)((*texture).width / 8), (float)((*texture).height / 8)}, // Rect
            {WIDTH + (float)i * 300, HEIGHT - nebulaData.rec.height},                  // Pos
            -200                                                                       // Velocity
        };
        nebulas[i] = nebulaData;
    }
}

void updateFrame(AnimData *data, float dT)
{
    int frameMax = (*data).texture->width / (*data).rec.width - 1;
    (*data).runningTime += dT;
    if ((*data).runningTime >= (*data).updateTime)
    {
        (*data).runningTime = 0.0;
        (*data).rec.x = (*data).frame * (*data).rec.width;
        (*data).frame++;
        if ((*data).frame > frameMax)
            (*data).frame = 0;
    }
}

int main()
{

    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    const int gravity{1'000};
    const int jumpVel{-600};
    bool isInAir{false};

    Texture2D background = LoadTexture(farFilePath);
    Texture2D midground = LoadTexture(backFilePath);
    Texture2D foreground = LoadTexture(foregroundFilePath);
    float bgX{0};
    float mgX{0};
    float fgX{0};

    const char *text_game_over = "Game Over!";
    int text_game_over_size{100};
    int text_game_over_length = (int)strlen(text_game_over) * text_game_over_size / 2;
    int text_game_over_x = (int)(WIDTH / 2 - text_game_over_length / 2);
    int text_game_over_y = (int)(HEIGHT / 2 - text_game_over_size / 2);
    Color text_game_over_color{WHITE};

    const char *text_you_won = "You Won!";
    int text_you_won_size{100};
    int text_you_won_length = (int)strlen(text_you_won) * text_you_won_size / 2;
    int text_you_won_x = (int)(WIDTH / 2 - text_you_won_length / 2);
    int text_you_won_y = (int)(HEIGHT / 2 - text_you_won_size / 2);
    Color text_you_won_color{WHITE};

    bool collision = false;

    Texture2D scarfyTexture = LoadTexture(scarfyFilepath);
    AnimData scarfyData{
        &scarfyTexture,                                                              // texture
        {0.0, 0.0, (float)(scarfyTexture.width / 6), (float)(scarfyTexture.height)}, // Rect
        {WIDTH / 2 - scarfyData.rec.width / 2, HEIGHT - scarfyData.rec.height}       // Pos
    };

    Texture2D nebulaTexture = LoadTexture(nebulaFilePath);
    AnimData nebulas[NUM_OF_NEBULAS]{};
    initNebulas(nebulas, &nebulaTexture);

    float finishLine{nebulas[NUM_OF_NEBULAS - 1].pos.x + 150};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        const float dT{GetFrameTime()};

        for (auto &&nebula : nebulas)
            updateFrame(&nebula, dT);

        if (!isInAir)
            updateFrame(&scarfyData, dT);

        bool isOnGround = scarfyData.pos.y >= HEIGHT - scarfyData.rec.height;
        if (isOnGround)
        {
            isInAir = false;
            scarfyData.velocity = 0;
        }
        else
        {
            isInAir = true;
            scarfyData.velocity += gravity * dT;
        }

        if (IsKeyPressed(KEY_SPACE) && !isInAir)
            scarfyData.velocity += jumpVel;

        for (auto &&data : nebulas)
        {
            float pad{50};
            Rectangle nebRec{
                data.pos.x + pad,
                data.pos.y + pad,
                data.rec.width - 2 * pad,
                data.rec.height - 2 * pad};
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height,
            };
            if (CheckCollisionRecs(scarfyRec, nebRec))
                collision = true;
        }

        bgX += -20 * dT;
        float backScale{(float)(WIDTH + 5.0) / background.width};
        if (bgX <= -background.width * backScale)
            bgX = 0.0;

        mgX += -40 * dT;
        float midScale{(float)(WIDTH + 5.0) / midground.width};
        if (mgX <= -midground.width * midScale)
            mgX = 0.0;

        fgX += -80 * dT;
        float foreScale{(float)(WIDTH + 5.0) / foreground.width};
        if (fgX <= -foreground.width * foreScale)
            fgX = 0.0;

        Vector2 backPos{bgX, 0.0};
        DrawTextureEx(background, backPos, 0.0, backScale, WHITE);
        Vector2 backPos2{bgX + background.width * backScale, 0.0};
        DrawTextureEx(background, backPos2, 0.0, backScale, WHITE);

        Vector2 midPos{mgX, 0.0};
        DrawTextureEx(midground, midPos, 0.0, midScale, WHITE);
        Vector2 midPos2{mgX + midground.width * midScale, 0.0};
        DrawTextureEx(midground, midPos2, 0.0, midScale, WHITE);

        Vector2 forePos{fgX, 0.0};
        DrawTextureEx(foreground, forePos, 0.0, foreScale, WHITE);
        Vector2 forePos2{fgX + foreground.width * foreScale, 0.0};
        DrawTextureEx(foreground, forePos2, 0.0, foreScale, WHITE);

        if (collision)
        {
            DrawText(text_game_over, text_game_over_x + text_game_over_size / 10, text_game_over_y, text_game_over_size, RED);
            DrawText(text_game_over, text_game_over_x - text_game_over_size / 10, text_game_over_y, text_game_over_size, RED);
            DrawText(text_game_over, text_game_over_x, text_game_over_y + text_game_over_size / 10, text_game_over_size, RED);
            DrawText(text_game_over, text_game_over_x, text_game_over_y - text_game_over_size / 10, text_game_over_size, RED);
            DrawText(text_game_over, text_game_over_x - text_game_over_size / 10, text_game_over_y - text_game_over_size / 10, text_game_over_size, RED);
            DrawText(text_game_over, text_game_over_x + text_game_over_size / 10, text_game_over_y + text_game_over_size / 10, text_game_over_size, RED);
            DrawText(text_game_over, text_game_over_x - text_game_over_size / 10, text_game_over_y + text_game_over_size / 10, text_game_over_size, RED);
            DrawText(text_game_over, text_game_over_x + text_game_over_size / 10, text_game_over_y - text_game_over_size / 10, text_game_over_size, RED);
            DrawText(text_game_over, text_game_over_x, text_game_over_y, text_game_over_size, text_game_over_color);
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            DrawText(text_you_won, text_you_won_x + text_you_won_size / 10, text_you_won_y, text_you_won_size, RED);
            DrawText(text_you_won, text_you_won_x - text_you_won_size / 10, text_you_won_y, text_you_won_size, RED);
            DrawText(text_you_won, text_you_won_x, text_you_won_y + text_you_won_size / 10, text_you_won_size, RED);
            DrawText(text_you_won, text_you_won_x, text_you_won_y - text_you_won_size / 10, text_you_won_size, RED);
            DrawText(text_you_won, text_you_won_x - text_you_won_size / 10, text_you_won_y - text_you_won_size / 10, text_you_won_size, RED);
            DrawText(text_you_won, text_you_won_x + text_you_won_size / 10, text_you_won_y + text_you_won_size / 10, text_you_won_size, RED);
            DrawText(text_you_won, text_you_won_x - text_you_won_size / 10, text_you_won_y + text_you_won_size / 10, text_you_won_size, RED);
            DrawText(text_you_won, text_you_won_x + text_you_won_size / 10, text_you_won_y - text_you_won_size / 10, text_you_won_size, RED);
            DrawText(text_you_won, text_you_won_x, text_you_won_y, text_you_won_size, text_you_won_color);
        }
        else
        {

            for (auto &&data : nebulas)
            {
                data.pos.x += data.velocity * dT;
                DrawTextureRec(*data.texture, data.rec, data.pos, WHITE);
            }

            finishLine += nebulas[0].velocity * dT;

            scarfyData.pos.y += scarfyData.velocity * dT;
            DrawTextureRec(*scarfyData.texture, scarfyData.rec, scarfyData.pos, WHITE);
        }
        EndDrawing();
    }

    UnloadTexture(scarfyTexture);
    UnloadTexture(nebulaTexture);
    UnloadTexture(foreground);
    UnloadTexture(midground);
    UnloadTexture(background);

    CloseWindow();
    return 0;
}