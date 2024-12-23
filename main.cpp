#include <iostream>
#include <raylib.h>
#include <deque>

using namespace std;
Color green = (Color){173, 204, 96, 255};
Color darkgreen = (Color){43, 51, 24, 255};

int cellsize = 30;
int cellcount = 24;

class Snake {
    public:
    deque<Vector2> body = {Vector2{6,9},};
};

class Food {
public:
    Vector2 position = {5, 6};
    Texture2D texture;

    Food() {
        Image image = LoadImage("C:\\Users\\Anonymous\\Documents\\Snake Game Raylib\\Graphics\\food.png");
        if (image.data == nullptr) {
            cout << "Failed to load food.png" << endl;
        }
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandonPas();

    }
    ~Food() {
        UnloadTexture(texture);
    }

    void draw() {
        DrawTexture(texture, position.x * cellsize, position.y * cellsize, WHITE);
    }

    Vector2 GenerateRandonPas() {
        float x =GetRandomValue(0, cellcount - 1);
        float y =GetRandomValue(0, cellcount - 1);
    return (Vector2){x,y};
    }
};

int main() {
    cout << "starting the game.." << endl;
    InitWindow(cellsize * cellcount, cellsize * cellcount, "Retro Snake");
    SetTargetFPS(60);

    cout << "Current working directory: " << GetWorkingDirectory() << endl;

    Food food = Food();

    while (WindowShouldClose() == false) {
        BeginDrawing();

        // drawing
        ClearBackground(green);

        food.draw();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
