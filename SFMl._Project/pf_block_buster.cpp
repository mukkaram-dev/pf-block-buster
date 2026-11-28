#include <SFML/Graphics.hpp>   
#include <fstream>             
#include <string>
#include <cstdlib> 
#include <ctime>
#include <optional>

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

//feature 6 and 7 declaration
void loseLife();
void endGame(bool won);

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

const int LAUNCH_SPEED_X = 3;     // sideways speed when the ball is launched
void launchBall()
{
    ballLaunched = true;
    dy = -BALL_SPEED_Y;
    if (rand() % 2 == 0)
        dx = LAUNCH_SPEED_X;       // it will move random left or right
    else
        dx = -LAUNCH_SPEED_X;
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
int score = 0;

//feature 5 
void damageBrick(int r, int c)
{
    bricks[r][c]--;
    if (bricks[r][c] == 0)     // the last hit vanish the brick
    {
        score += BREAK_POINTS;
    }
    else
    {
        score += HIT_POINTS;
    }
}
bool hitBricks()
{
    int hitRow[4], hitCol[4]; 
    int hitCount = 0;

    for (int i = 0; i < 4; i++)
    {
        
        int cx = ballX + (i % 2) * (BALL_SIZE - 1);
        int cy = ballY + (i / 2) * (BALL_SIZE - 1);
        if (cx < 0 || cx >= WINDOW_W)
            continue;
        if (cy < BRICKS_Y || cy >= BRICKS_Y + ROWS * BRICK_H)
            continue;
        int r = (cy - BRICKS_Y) / BRICK_H;   // the formula from the guidelines
        int c = cx / BRICK_W;
        if (bricks[r][c] == 0) 
            continue;           //No brick there
        bool alreadyHit = false;
        for (int j = 0; j < hitCount; j++)
            if (hitRow[j] == r && hitCol[j] == c)
                alreadyHit = true;
        if (alreadyHit) continue;

        hitRow[hitCount] = r;
        hitCol[hitCount] = c;
        hitCount++;
        damageBrick(r, c);
    }
    return hitCount > 0;
}

//feature 6
void loseLife()
{
    lives--;
    if (lives <= 0)
    {
        endGame(false);  // game end if 0 lives
    }
    else
    {
        ballLaunched = false; //ball will again go on paddle
        placeBallOnPaddle();
    }
}
int countBricks()
{
    int total = 0;
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (bricks[r][c] > 0)
                total++;
    return total;
}
sf::Font font;     //the text of sfml needs font to be drawn on the screen and drawing text on the screen
void drawText(sf::RenderWindow& window, const string& s, float x, float y,
    unsigned int size, sf::Color color)
{
    sf::Text text(font, s, size); 
    text.setFillColor(color);
    text.setPosition(sf::Vector2f(x, y));
    window.draw(text);
}

void drawHud(sf::RenderWindow& window)
{
    sf::RectangleShape hud(sf::Vector2f(WINDOW_W, HUD_H));
    hud.setFillColor(sf::Color(30, 30, 70));
    window.draw(hud);

    drawText(window, "SCORE  " + to_string(score), 15, 12, 24, sf::Color::White);
    drawText(window, "LIVES  " + to_string(lives), 440, 12, 24, sf::Color::White);
}

//feature 7
void resetRound()
{
    paddleX = PADDLE_START_X;
    paddleWidth = PADDLE_W;
    ballLaunched = false;
    placeBallOnPaddle();
}


void startNewGame()   // Starts new game score 0, 3 lives,and bricks from level.txt
{
    if (!loadLevel())
    {
        menuMessage = "Error: level.txt is missing!";
        return;         //will stay on the menu
    }
    score = 0;
    lives = START_LIVES;
    resetRound();
    gameState = PLAYING;
}

void endGame(bool won) // Ends the game and shows YOU WIN , GAME OVER and emptiees the save file
{
    playerWon = won;
    gameState = GAME_OVER;
    clearSave();     //will be in Feature 8                              
}
void updateMenu()
{
    if (keyUp)
    {
        menuChoice--;
        if (menuChoice < 0) menuChoice = 2;
        menuMessage = "";
    }
    if (keyDown)
    {
        menuChoice++;
        if (menuChoice > 2) menuChoice = 0;
        menuMessage = "";
    }
    if (keyEnter)
    {
        menuMessage = "";
        switch (menuChoice)
        {
        case 0:
            startNewGame();
            break;
        case 1:
            continueGame();  //will be in Feature 8
            break;            
        case 2:
            exitGame = true;
            break;
        }
    }
}
void updatePlaying()
{
    if (keyEsc) // save and go back to the menu (Feature 8)
    {
        saveGame();
        gameState = MENU;
        menuMessage = "";
        return;
    }

    movePaddle();                  // Feature 3

    if (keySpace && !ballLaunched) // Feature 3
        launchBall();

    if (ballLaunched)              // Features 4 and 5
        moveBall();

    if (gameState == PLAYING && countBricks() == 0) // YOU WIN 
        endGame(true);
}

void updateGameOver()
{
    if (keyEnter)
        gameState = MENU;
}
void drawCenteredText(sf::RenderWindow& window, const string& s, float y,unsigned int size, sf::Color color)
{                                                                       // Draws text on the window 
    sf::Text text(font, s, size);
    text.setFillColor(color);
    float textWidth = text.getLocalBounds().size.x;
    text.setPosition(sf::Vector2f((WINDOW_W - textWidth) / 2.f, y));
    window.draw(text);
}

void drawMenu(sf::RenderWindow& window)
{
    drawCenteredText(window, "BLOCK BUSTER", 100, 52, sf::Color(255, 210, 60));

    string items[3] = { "New Game", "Continue", "Exit" };
    for (int i = 0; i < 3; i++)
    {
        if (i == menuChoice)        
            drawCenteredText(window, "> " + items[i] + " <", 260.f + i * 60, 32, sf::Color(60, 220, 255));
        else
            drawCenteredText(window, items[i], 260.f + i * 60, 32, sf::Color::White);
    }

    drawCenteredText(window, menuMessage, 470, 22, sf::Color(255, 80, 110));
    drawCenteredText(window, "Up/Down + Enter to choose", 600, 18, sf::Color(150, 150, 190));
    drawCenteredText(window, "In game: Left/Right or A/D move, Space launch, Esc save", 630, 16, sf::Color(150, 150, 190));
}

void drawPlaying(sf::RenderWindow& window)
{
    drawBricks(window);  // Feature 2

    sf::RectangleShape paddle(sf::Vector2f(paddleWidth, PADDLE_H)); //draw paddle 
    paddle.setFillColor(sf::Color(60, 220, 255));
    paddle.setPosition(sf::Vector2f(paddleX, PADDLE_Y));
    window.draw(paddle);

    sf::CircleShape ball(BALL_SIZE / 2);  // ball a circle of radius 6
    ball.setFillColor(sf::Color::White);
    ball.setPosition(sf::Vector2f(ballX, ballY));
    window.draw(ball);

    if (!ballLaunched)
        drawCenteredText(window, "Press SPACE to launch", 500, 22, sf::Color(150, 150, 190));

    drawHud(window);                           
}

void drawGameOver(sf::RenderWindow& window)
{
    if (playerWon)
        drawCenteredText(window, "YOU WIN", 240, 60, sf::Color(60, 220, 150));
    else
        drawCenteredText(window, "GAME OVER", 240, 60, sf::Color(255, 80, 110));

    drawCenteredText(window, "Final score: " + to_string(score), 340, 30, sf::Color::White);
    drawCenteredText(window, "Press Enter", 410, 24, sf::Color(150, 150, 190));
}
//feature 8
void saveGame()
{
    ofstream fout("savegame.txt");               // overwrites the file
    fout << score << " " << lives << "\n";
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            fout << bricks[r][c];
            if (c < COLS - 1) fout << " ";       // space between numbers
        }
        fout << "\n";                            // the file closes by itself at the end of this function
    }
}

bool loadSave()                                  // this functionloads savegame.txt and returns false if there is no saved game
{
    ifstream fin("savegame.txt");
    if (!fin)
        return false;

    int savedScore, savedLives;
    if (!(fin >> savedScore >> savedLives))        // empty file -> no saved game
        return false;
    if (savedLives <= 0)
        return false;
    if (!readBricks(fin))
        return false;

    score = savedScore;
    lives = savedLives;
    return true;
}

void clearSave()                                 // it empties savegame.txt that opens with ofstream and close
{
    ofstream fout("savegame.txt");
}


void continueGame()                              // this funcction will load the saved game
{
    if (!loadSave())
    {
        menuMessage = "No saved game";
        return;                                  //will remain on the menu when no game is saved
    }
	resetRound();                                //will reset the paddle and ball position
    gameState = PLAYING;
}
int main()
{
    srand((unsigned)time(0));
    sf::RenderWindow window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "Block Buster");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false); 

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))      // it loadss a font 
        if (!font.openFromFile("arial.ttf"))  
            return 1;                              // if no font found
    while (window.isOpen())  //  input , update and draw and 60 times per second
    {
        handleInput(window);
        updateGame();
        if (exitGame) //if exit is chosen
        {
            window.close();
            break;
        }
        drawGame(window);
    }

    return 0;

}