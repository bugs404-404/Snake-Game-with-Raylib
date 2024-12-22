#include <iostream>
#include <raylib.h>

using namespace std;
Color green = (Color){173, 204, 96, 255};
Color darkgreen = (Color){43, 51, 24, 255};

int cellsize = 30;
int cellcount = 24;

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
    }
    ~Food() {
        UnloadTexture(texture);
    }

    void draw() {
        DrawTexture(texture, position.x * cellsize, position.y * cellsize, WHITE);
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
