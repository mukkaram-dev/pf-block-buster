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

const int MENU = 0;
const int PLAYING = 1;
const int GAME_OVER = 2;
int gameState = MENU;        
int menuChoice = 0;          
string menuMessage = "";      
bool exitGame = false;        
bool playerWon = false;
int lives = START_LIVES;


void updateMenu();
void updatePlaying();
void updateGameOver();
void drawMenu(sf::RenderWindow& window);
void drawPlaying(sf::RenderWindow& window);
void drawGameOver(sf::RenderWindow& window);
//feature 5 declarattion
void damageBrick(int r, int c);
bool hitBricks();

//feature 8 declaration 
void saveGame();
bool loadSave();
void clearSave();
void continueGame();


//feature 1
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
void updateGame()
{
    switch (gameState)
    {
    case MENU:      updateMenu();     break;
    case PLAYING:   updatePlaying();  break;
    case GAME_OVER: updateGameOver(); break;
    }
}
void drawGame(sf::RenderWindow& window)
{
    window.clear(sf::Color(15, 15, 40));           // dark blue background

    switch (gameState)
    {
    case MENU:      drawMenu(window);     break;
    case PLAYING:   drawPlaying(window);  break;
    case GAME_OVER: drawGameOver(window); break;
    }

    window.display();                              // put everything on the screen
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
//feature 4
bool ballTouchesPaddle()
{
    return ballX < paddleX + paddleWidth && ballX + BALL_SIZE > paddleX && ballY < PADDLE_Y + PADDLE_H && 
        ballY + BALL_SIZE > PADDLE_Y;
}
void bounceOnPaddle()
{
    ballY = BALL_START_Y;  
    dy = -BALL_SPEED_Y;
    int part = (ballX + BALL_SIZE / 2 - paddleX) * 4 / paddleWidth;
    if (part < 0) 
        part = 0;  
    if (part > 3)
        part = 3;
    int newDx[4] = { -6, -3, 3, 6 };
    dx = newDx[part];
}

void endGame(bool won)
{
    playerWon = won;
    gameState = GAME_OVER;
    clearSave();
}
void loseLife()
{
    lives--;
    if (lives <= 0)
    {
        endGame(false);           // GAME OVER
    }
    else
    {
        ballLaunched = false;    // ball goes back on the paddle
        placeBallOnPaddle();
    }
}
void moveBall()
{ 
    ballX += dx;
    if (ballX < 0 || ballX + BALL_SIZE > WINDOW_W || hitBricks())
    {
        ballX -= dx;
        dx = -dx;                 // reverse the speed
    }

    ballY += dy;
    if (ballY < HUD_H || hitBricks())  
    {
        ballY -= dy;
        dy = -dy;
    }
    if (dy > 0 && ballTouchesPaddle())
        bounceOnPaddle();

    if (ballY > WINDOW_H)      // fell below the bottom of the window
        loseLife();
}
int main()
{
    srand((unsigned)time(0));
    sf::RenderWindow window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "Block Buster");
    window.setFramerateLimit(60);

    return 0;

}