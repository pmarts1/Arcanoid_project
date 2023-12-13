#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#define ScreenX  1600
#define ScreenY 900

struct Platform{
    public:
        float speed;
        sf::RectangleShape platform;
        float length = 200;
        int direction;
        Platform(const sf::Vector2f & size, const sf::Vector2f & position, const sf::Color & color, float speed);
        void Update(float dt);
        float x();
};