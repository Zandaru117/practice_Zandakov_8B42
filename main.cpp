#include "pong.h"

#include <SFML/Graphics.hpp>

int main() {

    sf::RenderWindow window(sf::VideoMode(1280, 720), L"Пинг Понг");
    // Эксперименты
    sf::CircleShape shape(15.f);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(640, 360);
    /*
    sf::RectangleShape rect2(sf::Vector2f(120, 50));  // Размеры
    rect2.setFillColor(sf::Color::Red);
    rect2.setPosition(1200,300);
    //rect.setPosition(3, 4);
    */
    sf::RectangleShape rect1(sf::Vector2f(15, 100));
    rect1.setFillColor(sf::Color::White);
    rect1.setPosition(50,160);


    sf::RectangleShape rect2(sf::Vector2f(15, 100));
    rect2.setFillColor(sf::Color::White);
    rect2.setPosition(1205,160);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
            shape.move(0, -1); // Вверх
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
            shape.move(0, 1);  // Вниз
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
            shape.move(-1, 0);  // Влево
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
            shape.move(1, 0);  // Вправо
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            rect1.move(0, -1); // Вниз
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            rect1.move(0, 1);  // Вверх
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
            rect2.move(0, -1); // Вниз
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
            rect2.move(0, 1); // Вверх
        }


        window.clear();
        window.draw(shape);
        window.draw(rect1);
        window.draw(rect2);
        window.display();
    }
    return 0;
}
