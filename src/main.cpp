#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <list>
#include <fstream>
//#include "clearEventsQueue.hpp"
using namespace std;
//using namespace sf;
#define ScreenX 1600
#define ScreenY 900

int opt = 0;
auto PlatformColor = sf::Color::Black;

struct Platform
{
public:
    float speed;
    sf::RectangleShape platform;
    float length = 200;
    int direction;
    Platform(const sf::Vector2f &size, const sf::Vector2f &position, const sf::Color &color, float speed)
    {
        platform.setSize(size);
        platform.setFillColor(color);
        platform.setPosition(position);
        this->speed = speed;
    };

    void Update(float dt)
    {
        if ((direction == -1) and (this->x() > 0))
        {
            platform.move(-speed * dt, 0.f);
        }
        if ((direction == 1) and (this->x() + length < ScreenX - 200))
        {
            platform.move(speed * dt, 0.f);
        }
    }
    float x()
    {
        return platform.getPosition().x;
    }
};

struct Block
{
public:
    sf::Color clr;
    sf::RectangleShape block;
    float length;
    float height;
    Block(const sf::Vector2f &size, const sf::Vector2f &position, const sf::Color &color)
    {

        block.setSize(size);
        block.setFillColor(color);
        block.setPosition(position);
        clr = color;
        length = size.x;
        height = size.y;
    };
    float x()
    {
        return block.getPosition().x;
    }
    float y()
    {
        return block.getPosition().y;
    }
};

struct Ball
{
public:
    sf::CircleShape ball;
    sf::Vector2f velocity = sf::Vector2f(-4.f, -4.f);
    sf::Sound sound;
    sf::SoundBuffer buffer;
    Ball(float r, const sf::Vector2f start_position, const sf::Color &color)
    {
        ball.setRadius(r);
        ball.setFillColor(color);
        ball.setPosition(start_position);

        if (!buffer.loadFromFile("beep.wav"))
        {
            cout << "Error";
        }
        sound.setBuffer(buffer);
        std::ifstream file("volume.txt");
        if (file.is_open())
        {
            std::string line;
            std::getline(file, line);
            sound.setVolume(stoi(line));
            cout << stoi(line);
            file.close();
        }
    };
    void Update(float dt)
    {
        ball.move(velocity * dt);
    }
    float x()
    {
        return ball.getPosition().x;
    }
    float y()
    {
        return ball.getPosition().y;
    }

    void Colision(Platform &pl)
    {
        if (fabs(pl.x() + pl.length / 2 - this->x()) < pl.length / 2 and fabs((this->y() - 860)) < 5 and velocity.y > 0)
        {
            velocity = sf::Vector2f(velocity.x, -velocity.y);

            sound.play();
        }
    }
    void Colision()
    {
        if (this->x() > ScreenX - 200)
        {
            velocity = sf::Vector2f(-velocity.x, velocity.y);
        }
        if (this->x() < 0)
        {
            velocity = sf::Vector2f(-velocity.x, velocity.y);
        }
        if (this->y() < 0)
        {
            velocity = sf::Vector2f(velocity.x, -velocity.y);
        }
    }
    bool Colision(Block &bl)
    {
        if ((this->x() - bl.x()) < bl.length and (this->x() - bl.x()) > 0 and ((-bl.y() + this->y()) < bl.height) and ((-bl.y() + this->y()) > 0))
        {
            // bl.block.setFillColor(sf::Color::Yellow);
            if (this->x() - (bl.x() + bl.length / 2) > bl.length / bl.height * fabs(this->y() - (bl.y() + bl.height / 2)))
            {
                velocity = sf::Vector2f(-velocity.x, velocity.y);
            }
            else if (-(this->x() - (bl.x() + bl.length / 2)) > bl.length / bl.height * fabs(this->y() - (bl.y() + bl.height / 2)))
            {
                velocity = sf::Vector2f(-velocity.x, velocity.y);
            }
            else
            {
                velocity = sf::Vector2f(velocity.x, -velocity.y);
            }
            return 1;
        }
        return 0;
    }
};

void clearEventsQueue(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event));
}

int minScore()
{
    int min = 9999999;
    std::ifstream file("score.txt");
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            string score = line.substr(line.find(" "), line.length());
            if (min > stoi(score))
                min = stoi(score);
        }
        file.close();
    }
    return min;
}

int searchPosition(int score)
{
    int position = 0;
    std::ifstream file("score.txt");
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            string scoreRead = line.substr(line.find(" "), line.length());
            if (score > stoi(scoreRead))
                return position;
            position++;
        }
        file.close();
    }
    return -1;
}

bool ask(sf::RenderWindow &window, int score)
{
    if (searchPosition(score) != -1)
    {
        window.clear(sf::Color::White);
        sf::Text text;
        sf::Font font;
        text.setFillColor(sf::Color::Black);
        font.loadFromFile("freesansbold.ttf");
        text.setFont(font);
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(50);
        text.setString("Do you want to save your score = " + to_string(score) + "?" + "(y/n)");

        text.setPosition(ScreenX / 2 - text.getLocalBounds().width / 2, 200);
        window.draw(text);
        window.display();
        while (1)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
                {
                    return 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
                {
                    return 0;
                }
            }
        }
    }
    return 0;
}

string enterNickname(sf::RenderWindow &window)
{
    string str;
    window.clear(sf::Color::White);
    sf::Text text;
    sf::Font font;
    text.setFillColor(sf::Color::Black);
    font.loadFromFile("freesansbold.ttf");
    text.setFont(font);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(100);
    text.setString("Enter nickname: ");
    text.setPosition(ScreenX / 2 - text.getLocalBounds().width / 2, 200);
    window.draw(text);
    window.display();
    clearEventsQueue(window);
    while (1)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::TextEntered)
            {
                str += static_cast<char>(event.text.unicode);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                cout << str << endl;
                return str;
            }
            sf::Text text2;
            text2.setFillColor(sf::Color::Black);
            text2.setFont(font);
            text2.setStyle(sf::Text::Bold);
            text2.setCharacterSize(100);
            text2.setString(str);
            text2.setPosition(ScreenX / 2 - text2.getLocalBounds().width / 2, 300);
            window.draw(text);
            window.draw(text2);
            window.display();
            window.clear(sf::Color::White);
        }
    }
}
void saveScore(sf::RenderWindow &window, int score, string nickName)
{
    if (searchPosition(score) != -1)
    {
        int pos = searchPosition(score);
        window.clear(sf::Color::White);
        sf::Text text;
        sf::Font font;
        text.setFillColor(sf::Color::Black);
        font.loadFromFile("freesansbold.ttf");
        text.setFont(font);
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(100);
        text.setString(to_string(score));
        text.setPosition(ScreenX / 2 - text.getLocalBounds().width / 2, 200);
        window.draw(text);
        window.display();
        cout << "nbyghubhhujn";
        vector<string> newFile;
        std::fstream file("score.txt");
        if (file.is_open())
        {
            cout << "starting";
            string line;
            for (int i = 0; i < 9; i++)
            {
                std::getline(file, line);
                newFile.push_back(line);
            }
            for (int i = 0; i < 9; i++)
            {
                cout << newFile[i] << endl;
            }
            file.close();
            std::fstream file("score.txt", ios::out);
            for (int i = 0; i < pos; i++)
            {
                file << newFile[i] << endl;
            }
            file << nickName << " " << score << endl;
            for (int i = pos; i < 9; i++)
            {
                file << newFile[i] << endl;
            }
        }
        file.close();
    }
}

void WriteScore(sf::RenderWindow &window, int score)
{
    sf::Text text;
    sf::Font font;
    text.setFillColor(sf::Color::Black);
    font.loadFromFile("freesansbold.ttf");
    text.setFont(font);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(100);
    text.setString(to_string(score));
    text.setPosition(ScreenX - 100 - text.getLocalBounds().width / 2, 200);
    window.draw(text);
}

void Settings(sf::RenderWindow &window)
{
    clearEventsQueue(window);
    vector<sf::Color> clrs = {sf::Color::Black, sf::Color::Blue, sf::Color::Cyan, sf::Color::Green, sf::Color::Magenta, sf::Color::Red};
    sf::RectangleShape line(sf::Vector2f(ScreenX / 5, 5.f));
    line.setFillColor(sf::Color::Black);
    line.setPosition(ScreenX / 2 - ScreenX / 10, ScreenY / 2 + 100);
    int volume = 0;
    std::ifstream file("volume.txt");
    if (file.is_open())
    {
        std::string line;
        std::getline(file, line);
        volume = stoi(line);
        file.close();
    }
    sf::RectangleShape rect(sf::Vector2f(20.f, 20.f));
    rect.setFillColor(sf::Color::Black);
    rect.setPosition(ScreenX / 2 - ScreenX / 10 + (ScreenX / 2 - ScreenX / 10) * volume / 200, ScreenY / 2 + 100 - 7);
    sf::Text text;
    sf::Text text2;
    sf::Font font;
    text.setFillColor(sf::Color::Black);
    font.loadFromFile("freesansbold.ttf");
    text.setFont(font);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(50);
    text.setString("Platform color");
    text.setPosition(ScreenX / 2 - text.getLocalBounds().width / 2, 50);

    text2.setFillColor(sf::Color::Black);
    text2.setFont(font);
    text2.setStyle(sf::Text::Bold);
    text2.setCharacterSize(50);
    text2.setString("Volume");
    text2.setPosition(ScreenX / 2 - text2.getLocalBounds().width / 2, ScreenY / 2);
    while (window.isOpen())
    {
        window.clear(sf::Color::White);

        window.draw(line);

        window.draw(text);
        window.draw(text2);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i globalPosition = sf::Mouse::getPosition(window);
                if (globalPosition.y > (ScreenY / 4) and globalPosition.y < (ScreenY / 4 + 100))
                {
                    for (int i = 0; i < 6; i++)
                    {
                        if (globalPosition.x > (ScreenX / 7 * (i + 1) - 50) and globalPosition.x < (ScreenX / 7 * (i + 1) + 50))
                        {
                            PlatformColor = clrs[i];
                        }
                    }
                }
                if (globalPosition.y > (ScreenY / 2 + 80) and globalPosition.y < (ScreenY / 2 + 120))
                {
                    if (globalPosition.x > (ScreenX / 2 - ScreenX / 10 - 10) and globalPosition.x < (ScreenX / 2 - ScreenX / 10 + ScreenX / 5))
                    {
                        rect.setPosition(globalPosition.x, ScreenY / 2 + 100 - 7);
                    }
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                std::ofstream file("volume.txt");
                if (file.is_open())
                {
                    string line = to_string((rect.getPosition().x - (ScreenX / 2 - ScreenX / 10)) / 3);
                    if ((rect.getPosition().x - (ScreenX / 2 - ScreenX / 10)) / 3 < 1)
                    {
                        line = to_string(0);
                    }
                    if ((rect.getPosition().x - (ScreenX / 2 - ScreenX / 10)) / 3 > 100)
                    {
                        line = to_string(100);
                    }
                    file << line;
                }
                file.close();
                return;
            }
        }
        window.draw(rect);
        vector<sf::CircleShape> circles;
        for (int i = 0; i < 6; i++)
        {
            circles.push_back(sf::CircleShape(50.f));
            circles[i].setPosition(ScreenX / 7 * (i + 1) - 50, ScreenY / 4);
            circles[i].setFillColor(clrs[i]);
            window.draw(circles[i]);
        }
        window.display();
    }
}

void lose(int score, sf::RenderWindow &window)
{
    window.clear(sf::Color::White);
    sf::Text text;
    sf::Font font;
    text.setFillColor(sf::Color::Black);
    font.loadFromFile("freesansbold.ttf");
    text.setFont(font);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(100);
    text.setString(to_string(score));
    text.setPosition(ScreenX / 2 - text.getLocalBounds().width / 2, 200);
    window.draw(text);
    window.display();
    clearEventsQueue(window);
    sf::Event event;
    while (1)
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                return;
            }
        }
    }
}

void showScoreTable(sf::RenderWindow &window)
{
    std::ifstream in("score.txt");
    std::string line;
    sf::Text text;
    sf::Font font;
    text.setFillColor(sf::Color::Black);
    font.loadFromFile("freesansbold.ttf");
    text.setFont(font);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(50);
    window.clear(sf::Color::White);
    if (in.is_open())
    {
        int i = 0;
        while (std::getline(in, line))
        {
            i++;
            text.setString(line);
            text.setPosition(ScreenX / 2 - text.getLocalBounds().width / 2, i * ScreenY / 12);
            window.draw(text);
        }
    }
    window.display();
    clearEventsQueue(window);
    while (1)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                return;
            }
        }
    }
}

int Game(sf::RenderWindow &window, sf::Clock clock)
{
    int score = 0;
    list<Block> blocks;
    for (int j = 0; j < 5; j++)
    {
        float h = 50 * j;
        for (int i = 0; i < 12; i++)
        {
            float x = 110 * i;
            blocks.push_back(Block(sf::Vector2f(100.f, 40.f), sf::Vector2f(x, h), sf::Color::Magenta));
        }
    }
    float timer;

    timer = clock.restart().asMilliseconds();
    float time_k = 0.1;
    float time = 0;
    sf::RectangleShape line(sf::Vector2f(5.f, ScreenY));
    line.move(ScreenX - 200, 0.f);
    line.setFillColor(sf::Color::Black);
    Platform p(sf::Vector2f(200.f, 20.f), sf::Vector2f(0.f, 880.f), PlatformColor, 10.f);
    Ball b(5, sf::Vector2f((float)ScreenX / 2, (float)ScreenY / 2), sf::Color::Black);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
                p.direction = -1;
            else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
                p.direction = 1;
            else
                p.direction = 0;
        }
        b.Colision(p);
        b.Colision();
        for (list<Block>::iterator it = blocks.begin(); it != blocks.end(); it++)
        {
            if (b.Colision(*it))
            {
                blocks.erase(it);
                score++;
                break;
            }
        }
        sf::Time elapsed = clock.getElapsedTime();
        timer = clock.restart().asMilliseconds();
        time = timer * time_k;
        p.Update(time);
        b.Update(time);
        window.clear(sf::Color::White);
        window.draw(line);
        window.draw(p.platform);
        window.draw(b.ball);

        if (b.y() > ScreenY)
        {
            return score;
        }
        for (list<Block>::iterator it = blocks.begin(); it != blocks.end(); it++)
        {
            window.draw((*it).block);
        }
        WriteScore(window, score);
        window.display();
    }
    return 5;
}

void Menu(sf::RenderWindow &window)
{
    sf::Text text;
    vector<string> options = {"Play", "Settings", "Records", "Quit"};
    sf::Font font;
    text.setFillColor(sf::Color::Black);
    font.loadFromFile("freesansbold.ttf");
    text.setFont(font);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(80);
    clearEventsQueue(window);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and (opt < 3))
            {
                opt++;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) and (opt > 0))
            {
                opt--;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                return;
            }
            {
                window.clear(sf::Color::White);
                for (int i = 0; i < 4; i++)
                {
                    if (i == opt)
                        text.setStyle(sf::Text::Bold | sf::Text::Underlined);
                    else
                        text.setStyle(sf::Text::Bold);
                    text.setString(options[i]);
                    text.setPosition(ScreenX / 2 - text.getLocalBounds().width / 2, 200 * i);
                    window.draw(text);
                }
                window.display();
            }
        }
    }
}

int main()
{
    cout << minScore();
    int i = 1;
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(ScreenX, ScreenY), "Arkanoid");
    window.setFramerateLimit(60);
    while (opt != 3)
    {
        Menu(window);
        switch (opt)
        {
        case 0:
        {
            int finalScore = Game(window, clock);
            if (ask(window, finalScore) == 1)
            {
                saveScore(window, finalScore, enterNickname(window));
            }
            break;
        }
        case 1:
        {
            Settings(window);
            break;
        }
        case 2:
        {
            showScoreTable(window);
            break;
        }
        case 3:
        {
            window.close();
            break;
        }
        }
    }
    return 0;
}
