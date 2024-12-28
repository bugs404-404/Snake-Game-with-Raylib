#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;
Color green = (Color){173, 204, 96, 255};
Color darkgreen = (Color){43, 51, 24, 255};

int cellsize = 30;
int cellcount = 24;

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
    deque<Vector2> body = {Vector2{6,9},{Vector2{5,9}},{Vector2{4,9}}};
    Vector2 direction = {1,0};
    bool addSegment = false;

    void Draw() {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segments = Rectangle{x * cellsize, y * cellsize,(float)cellsize, (float)cellsize};
            DrawRectangleRounded(segments, 0.5, 6, darkgreen);
        }
    }

    void Update() {
        body.push_front(Vector2Add(body[0],direction));
      if (addSegment == true) {
          addSegment = false;
      }else
        body.pop_back();
    }
    void Reset()
    {
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
        }
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandonPos(snakeBody);

    }
    ~Food() {
        UnloadTexture(texture);
    }

    void draw() {
        DrawTexture(texture, position.x * cellsize, position.y * cellsize, WHITE);
    }
    Vector2 GenerateRandomCell() {
        float x =GetRandomValue(0, cellcount - 1);
        float y =GetRandomValue(0, cellcount - 1);
        return Vector2{x,y};
    }

    Vector2 GenerateRandonPos(deque<Vector2> snakebody) {
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
    bool running =true;

    void Draw()
    {
        food.draw();
        snake.Draw();
    }
    void Update() {
        if (running)
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }
    void CheckCollisionWithFood() {
     if (Vector2Equals(snake.body [0], food.position)) {
         food.position = food.GenerateRandonPos(snake.body);
         snake.addSegment = true;
     }
    }
    void CheckCollisionWithEdges()
    {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1)
        {
            GameOver();
        }
        if (snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            GameOver();
        }
    }
    void GameOver()
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }
};

int main() {
    cout << "starting the game.." << endl;
    InitWindow(cellsize * cellcount, cellsize * cellcount, "Retro Snake");
    SetTargetFPS(60);

    cout << "Current working directory: " << GetWorkingDirectory() << endl;

    Game game = Game();

    while (WindowShouldClose() == false) {
        BeginDrawing();

        if (eventTriggered(0.2)) {
            game.Update();
        }
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0,-1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = {0,1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = {-1,0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = {1,0};
            game.running = true;
        }

        // drawing
        ClearBackground(green);
        game.Draw();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
