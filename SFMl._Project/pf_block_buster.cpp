#include <SFML/Graphics.hpp>   
#include <fstream>             
#include <string>
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
const int NORMAL_PADDLE_W = 100;
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


sf::Color brickColors[4] =
{
    sf::Color(25, 25, 45),
    sf::Color::Green,
    sf::Color::Yellow,
    sf::Color::Red
};
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
        return false; //will return false instead of crashing if file not found
    int rows, cols;
    fin >> rows >> cols;   
    if (!fin || rows != ROWS || cols != COLS)
        return false;

    return readBricks(fin);
}
int main()
{
    sf::RenderWindow window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "Block Buster");
    window.setFramerateLimit(60);

}