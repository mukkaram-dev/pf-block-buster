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
int main()
{
    sf::RenderWindow window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "Block Buster");
    window.setFramerateLimit(60);

}