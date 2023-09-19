#include <iostream>
#include "./headers/SnakePlayer.h"
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;



Color green = {198, 225, 168, 255};
Color darkGreen = {0, 33, 30, 255};
Color foodColor = {255, 0, 17, 255};
Color headColor= {132, 187, 241, 255};
Color fontColor = {0, 33, 30, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

float speed = 0.1;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for (size_t i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return (true);
        }
    }
    return false;
}

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool addSegment = false;

    void Draw()
    {
        for (size_t i = 0; i < body.size(); i++)
        {

            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset+ x * cellSize, offset +  y * cellSize, (float)cellSize, (float)cellSize};
            if (i == 0){
                DrawRectangleRounded(segment, 0.5, 6, headColor);
            }else{

                DrawRectangleRounded(segment, 0.5, 6, darkGreen);
            }
        }
    }
    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment == true){

            addSegment = false;

        }else{

            body.pop_back();
     

        }
    }

    void Reset(){
        body ={Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1,0};
    }

};

class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody)
    {

        Image image = LoadImage("./graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food()
    {

        UnloadTexture(texture);
    }

    void Draw()
    {

        DrawTexture(texture,  offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return {x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {

     
        Vector2 position =  GenerateRandomCell();
        while (ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game
{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool isRunning = true;
    int score = 0;

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        if (isRunning)
        {
            snake.Update();
            CheckCollisionEdges();
            CheckCollisionFood();
            CheckCollisionTail();
        }
        
    }



    void GameOver(){
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        isRunning = false;
        score = 0;
    }

    void CheckCollisionFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
        }
    }
    void CheckCollisionEdges(){
        if (snake.body[0].x == cellCount || snake.body[0].x == -1){
            if (snake.body[0].x == cellCount ){
 
                snake.body[0].x = 0;

            }else{
                
                snake.body[0].x = (float)cellCount-1;

            }
           
        }
        if (snake.body[0].y == cellCount || snake.body[0].y == -1){
            if (snake.body[0].y == cellCount ){

                snake.body[0].y= 0;
                
            }else{

                snake.body[0].y = (float)cellCount-1;
            }
            
        }
    }
    void CheckCollisionTail(){
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(snake.body[0], headlessBody)){
            GameOver();
        }
    }
};

int main()
{

    InitWindow(2*offset + cellSize * cellCount, 2*offset + cellSize * cellCount, "Snake");
    SetTargetFPS(60);

    Game game = Game();

    while (!WindowShouldClose())
    {
        // Lógica de tu juego aquí

        BeginDrawing();

        if (eventTriggered(speed))
        {
            game.Update();
        }

        if(IsKeyPressed(KEY_SPACE)){
            
            game.isRunning = true;

        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
        }

        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
        }

        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
        }

        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
        }

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize*cellCount +10,(float)cellSize*cellCount +10 }, 5, GRAY);
        DrawText("Retro Snake", offset-5, 20, 40, fontColor);
        DrawText(TextFormat("%i",game.score),offset-5, offset+ cellSize*cellCount+10, 40, fontColor);
        game.Draw();
        // Dibuja cosas en la ventana
        EndDrawing();
    }

    // Cierra Raylib
    CloseWindow();
    return 0;
}
