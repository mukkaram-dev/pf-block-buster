#include <SFML/Graphics.hpp>   
#include <fstream>             
#include <string>
using namespace std;
const int ROWS = 8;
const int COLS = 10;
const int WINDOW_W = 600;
const int WINDOW_H = 700;
const int HUD_H = 50;
const float BRICK_W = 60;
const float BRICK_H = 25;
const float BRICKS_Y = 100;
sf::Color brickColors[4] = 
{
    sf::Color(25, 25, 45),    
    sf::Color::Green,   
    sf::Color::Yellow,   
    sf::Color::Red    
};
int main()
{
     sf::RenderWindow window(sf::VideoMode({WINDOW_W, WINDOW_H}), "Block Buster");
     window.setFramerateLimit(60); 

}