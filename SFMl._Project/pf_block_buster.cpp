#include <SFML/Graphics.hpp>   
#include <fstream>             
#include <string>
#include <cstdlib> 
using namespace std;
//windows initialization
const int WINDOW_W = 600;
const int WINDOW_H = 700;
const int HUD_H = 50;
//bricks initialization
const float BRICK_W = 60;
const float BRICK_H = 25;
const float BRICKS_Y = 100;
//paddle initialization
const int PADDLE_Y = 650;
const int PADDLE_START_X = 250;
const int PADDLE_H = 14;
const int PADDLE_W = 100;
const int PADDLE_SPEED = 8;
//ball initialization
const int BALL_SIZE = 12;     //12*12 pixels and radius 6 because ball is circle
const int BALL_START_Y = 638;      
const int BALL_SPEED_Y = 5;
//lives and points initialization
const int START_LIVES = 3;
const int HIT_POINTS = 10;
const int BREAK_POINTS = 50;
//bricks array initialization
const int ROWS = 8;
const int COLS = 10;
int bricks[ROWS][COLS];


bool keyUp = false, keyDown = false, keyEnter = false, keyEsc = false, keySpace = false, moveLeft = false, moveRight = false;
void handleInput(sf::RenderWindow& window)
{

    keyUp = keyDown = keyEnter = keyEsc = keySpace = false;


    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
        else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Up || key->code == sf::Keyboard::Key::W)
                keyUp = true;
            if (key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S)
                keyDown = true;
            if (key->code == sf::Keyboard::Key::Enter)
                keyEnter = true;
            if (key->code == sf::Keyboard::Key::Escape)
                keyEsc = true;
            if (key->code == sf::Keyboard::Key::Space)
                keySpace = true;
        }
    }

    moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
}
//feature 2
bool readBricks(ifstream& fin)
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (!(fin >> bricks[r][c]))  
                return false;    // if reading is failed
            if (bricks[r][c] < 0 || bricks[r][c] > 3)
                return false;
        }
    }
    return true;
}
bool loadLevel()
{
    ifstream fin("level.txt");
    if (!fin)      
        return false; //It will return false instead of crashing if file not found
    int rows, cols;
    fin >> rows >> cols;   
    if (!fin || rows != ROWS || cols != COLS)
        return false;

    return readBricks(fin);
}
sf::Color brickColors[4] =
{
    sf::Color(25, 25, 45),
    sf::Color::Green,
    sf::Color::Yellow,
    sf::Color::Red
};
void drawBricks(sf::RenderWindow& window)
{
    sf::RectangleShape brick(sf::Vector2f(BRICK_W - 2, BRICK_H - 2));  
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (bricks[r][c] == 0) continue;   
            brick.setFillColor(brickColors[bricks[r][c]]);
            brick.setPosition(sf::Vector2f(c * BRICK_W + 1, BRICKS_Y + r * BRICK_H + 1));
            window.draw(brick);
        }
    }
}

int paddleX = PADDLE_START_X; //250
int paddleWidth = PADDLE_W;   //100
int ballX = 0, ballY = 0;
int dx = 0, dy = 0;
bool ballLaunched = false;
//feature 3
void placeBallOnPaddle()
{
    ballX = paddleX + paddleWidth / 2 - BALL_SIZE / 2;   // P_W=100 ,B_S=12
    ballY = BALL_START_Y; //638
}
void movePaddle()
{
    if (moveLeft)
        paddleX -= PADDLE_SPEED;
    if (moveRight)
        paddleX += PADDLE_SPEED;

    if (paddleX < 0) //it will not let paddle go out of window
        paddleX = 0;
    if (paddleX + paddleWidth > WINDOW_W)
        paddleX = WINDOW_W - paddleWidth;

    if (!ballLaunched)// the ball is on the paddle before launch
        placeBallOnPaddle();
}
void launchBall()
{
    ballLaunched = true;
    dy = -BALL_SPEED_Y;
    if (rand() % 2 == 0)
        dx = 3;       // it will move random left or right
    else
        dx = -3;
}
int main()
{
    sf::RenderWindow window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "Block Buster");
    window.setFramerateLimit(60);

}