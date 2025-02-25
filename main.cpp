#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <cstring> // Added for TextFormat if needed

using namespace std;
Color green = (Color){173, 204, 96, 255};
Color darkgreen = (Color){43, 51, 24, 255};

int cellsize = 30;
int cellcount = 24;
int offset = 65;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
    for (unsigned int i = 0; i < deque.size(); i++) {
        if (Vector2Equals(deque[i], element)) {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake {
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool addSegment = false;

    void Draw() {
        for (unsigned int i = 0; i < body.size(); i++) {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segments = {offset + x * cellsize, offset + y * cellsize, (float)cellsize, (float)cellsize};
            DrawRectangleRounded(segments, 0.5, 6, darkgreen);
        }
    }

    void Update() {
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment == true) {
            addSegment = false;
        } else {
            body.pop_back();
        }
    }

    void Reset() {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class Food {
public:
    Vector2 position = {5, 6};
    Texture2D texture;

    Food(deque<Vector2> snakeBody) {
        Image image = LoadImage("C:\\Users\\Anonymous\\Documents\\Snake Game Raylib\\Graphics\\food.png");
        if (image.data == nullptr) {
            cout << "Failed to load food.png" << endl;
        } else {
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = GenerateRandomPos(snakeBody);
        }
    }
    ~Food() {
        UnloadTexture(texture);
    }

    void draw() {
        DrawTexture(texture, offset + position.x * cellsize, offset + position.y * cellsize, WHITE);
    }

    Vector2 GenerateRandomCell() {
        float x = GetRandomValue(0, cellcount - 1);
        float y = GetRandomValue(0, cellcount - 1);
        return (Vector2){x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakebody) {
        Vector2 position = GenerateRandomCell();
        while (ElementInDeque(position, snakebody)) {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    Sound eatSound;
    Sound wallSound;
    int score = 0;

    Game() {
        InitAudioDevice();
        eatSound = LoadSound("C:\\Users\\Anonymous\\Documents\\Snake Game Raylib\\Sounds\\eat.mp3");
        wallSound = LoadSound("C:\\Users\\Anonymous\\Documents\\Snake Game Raylib\\Sounds\\wall.mp3");
    }

    void Draw() {
        food.draw();
        snake.Draw();
    }

    void Update() {
        if (running) {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood() {
        if (Vector2Equals(snake.body[0], food.position)) {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithEdges() {
        if (snake.body[0].x == cellcount || snake.body[0].x == -1) {
            GameOver();
        }
        if (snake.body[0].y == cellcount || snake.body[0].y == -1) {
            GameOver();
        }
    }

    void CheckCollisionWithTail() {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(snake.body[0], headlessBody)) {
            GameOver();
        }
    }

    void GameOver() {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }
};

int main() {
    cout << "starting the game.." << endl;
    InitWindow(2 * offset + cellsize * cellcount, 2 * offset + cellsize * cellcount, "Retro Snake");
    SetTargetFPS(60);

    cout << "Current working directory: " << GetWorkingDirectory() << endl;

    Game game = Game();

    while (WindowShouldClose() == false) {
        if (eventTriggered(0.2)) {
            game.Update();
        }
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = {1, 0};
            game.running = true;
        }

        // drawing
        BeginDrawing();
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellsize * cellcount + 10, (float)cellsize * cellcount + 10}, 5, darkgreen);
        DrawText("Retro Snake", offset - 5, 20, 40, darkgreen);
        DrawText(TextFormat("%i", game.score), offset - 5, offset + cellsize * cellcount + 10, 40, darkgreen);
        game.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
