#include "platform.hpp"

Platform::Platform(const sf::Vector2f & size, const sf::Vector2f & position, const sf::Color & color, float speed){
    platform.setSize(size);
    platform.setFillColor(color);
    platform.setPosition(position);
    this->speed = speed;
};

void Platform::Update(float dt){
    if((direction == -1) and (this->x() > 0)){
        platform.move(-speed*dt, 0.f);
    }
    if((direction == 1) and (this->x() + length  < ScreenX)){
        platform.move(speed*dt, 0.f);
    }
}
float Platform::x(){
    return platform.getPosition().x;
}