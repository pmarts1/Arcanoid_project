#include "clearEventsQueue.hpp"

void clearEventsQueue(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event));
}