#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
using namespace std;
using namespace sf;

struct Platform{
    public:
        float speed;
        sf::RectangleShape platform;
        float length = 200;
        int direction;
        Platform(const sf::Vector2f & size, const sf::Vector2f & position, const sf::Color & color, float speed){
            platform.setSize(size);
            platform.setFillColor(color);
            platform.setPosition(position);
            this->speed = speed;
        };

        void Update(float dt){
            if(direction == -1){
                platform.move(-speed*dt, 0.f);
            }
            if(direction == 1){
                platform.move(speed*dt, 0.f);
            }
        }
        float x(){
            return platform.getPosition().x;
        }
};



struct Block{
    public:
        sf::Color clr;
        sf::RectangleShape block;
        float length;
        float height;
        Block(const sf::Vector2f & size, const sf::Vector2f & position, const sf::Color & color){
            block.setSize(size);
            block.setFillColor(color);
            block.setPosition(position);
            clr = color;
            length = size.x;
            height = size.y;
        };
        float x(){
            return block.getPosition().x;
        }
        float y(){
            return block.getPosition().y;
        }
};

struct Ball{
    public:
        sf::CircleShape ball;
        sf::Vector2f velocity = Vector2f(4.f, -4.f);
        Ball(float r, const sf::Vector2f start_position, const sf::Color & color){
            ball.setRadius(r);
            ball.setFillColor(color);
            ball.setPosition(start_position);
        };
        void Update(float dt){
            ball.move(velocity * dt);
        }
        float x(){
            return ball.getPosition().x;
        }
        float y(){
            return ball.getPosition().y;
        }
        
        void Colision(Platform & pl){
            if(fabs(pl.x() + pl.length/2 - this->x()) < pl.length/2 and fabs((this->y() - 860)) < 3){
                velocity = Vector2f(velocity.x, -velocity.y);
            }
        }
        void Colision(){
            if(this->x() > 1600){
                velocity = Vector2f(-velocity.x, velocity.y);
            }
            if(this->x() < 0){
                velocity = Vector2f(-velocity.x, velocity.y);
            }
            if(this->y() < 0){
                velocity = Vector2f(velocity.x, -velocity.y);
            }
        }
        bool Colision(Block & bl){
            if((this->x() - bl.x()) < bl.length and (this->x() - bl.x()) > 0 and ((-bl.y() + this->y()) < bl.height) and ((-bl.y() + this->y()) > 0)){
                //bl.block.setFillColor(sf::Color::Yellow);
                if(this->x() - (bl.x() + bl.length/2) > bl.length/bl.height*fabs(this->y() - (bl.y() + bl.height/2))){
                    velocity = Vector2f(-velocity.x, velocity.y);
                }
                else if(-(this->x() - (bl.x() + bl.length/2)) > bl.length/bl.height*fabs(this->y() - (bl.y() + bl.height/2))){
                    velocity = Vector2f(-velocity.x, velocity.y);
                }
                else{
                    velocity = Vector2f(velocity.x, -velocity.y);
                }
                
                return 1;
            }
            return 0;
        }
};

int main()
{
    list<Block> blocks;
    for(int i = 0; i < 5; i++){
        float x = 200*i;
        blocks.push_back(Block(sf::Vector2f(100.f, 40.f), sf::Vector2f(x, 400.f), sf::Color::Green));
    }
    blocks.push_back(Block(sf::Vector2f(100.f, 40.f), sf::Vector2f(1400.f, 400.f), sf::Color::Green));
    float timer;
    float time_k = 0.1;
    float time;
    sf::Clock clock;
    RenderWindow window(sf::VideoMode(1600, 900), "Arkanoid");
    Platform p(sf::Vector2f(200.f, 20.f), sf::Vector2f(0.f, 880.f), sf::Color::Red, 10.f);
    Ball b(5, sf::Vector2f(800.f, 420.f), sf::Color::Black);
    window.setFramerateLimit(60);
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
                p.direction = -1;
            else if((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
                p.direction = 1;
            else
                p.direction = 0;
        }
        b.Colision(p);
        b.Colision();
        for(list<Block>::iterator it = blocks.begin(); it != blocks.end(); it++){
            if(b.Colision(*it)){
                blocks.erase(it);
                break;
            }
        }
        sf::Time elapsed = clock.getElapsedTime();
        timer = clock.restart().asMilliseconds();
        time = timer * time_k;
        p.Update(time);
        b.Update(time);
        window.clear(sf::Color::White);
        window.draw(p.platform);
        window.draw(b.ball);
        for(list<Block>::iterator it = blocks.begin(); it != blocks.end(); it++){
            window.draw((*it).block);
        }
        window.display();
        //cout << p.x() << endl;
    }
    return 0;
}