#include "pong.h"

#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    int const racket_speed = 10; //скорость ракетки

    sf::RenderWindow window(sf::VideoMode(1280, 720), L"Пинг Понг"); // окно и ограничение кадров
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("fonts/Monocraft.ttf");
    sf::Text score_text;
    score_text.setFont(font);
    score_text.setCharacterSize(50);
    score_text.setFillColor(sf::Color::White);
    score_text.setString("0 : 0");
    score_text.setPosition(1280/2 - score_text.getLocalBounds().width/2, 20);
    
    int left_score = 0, right_score = 0;

    // мячик и его скорость
    sf::CircleShape ball(15.f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(640, 360);

    sf::Vector2f ball_speed(3.f, 3.f);
    
    // левая и правая ракетки
    sf::RectangleShape left_racket(sf::Vector2f(15, 100));
    left_racket.setFillColor(sf::Color::White);
    left_racket.setPosition(50,160);

    sf::RectangleShape right_racket(sf::Vector2f(15, 100));
    right_racket.setFillColor(sf::Color::White);
    right_racket.setPosition(1205,160);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // мячик
        ball.move(ball_speed);

        if (ball.getPosition().y <= 0 || ball.getPosition().y >= 720 - ball.getRadius()*2 ) {
            ball_speed.y = -ball_speed.y;
        }

        if (ball.getPosition().x <= 0 || ball.getPosition().x >= 1280 - ball.getRadius()*2 ) {
            ball.setPosition(640, 360);
        }

        // отскоки
        if (ball.getGlobalBounds().intersects(left_racket.getGlobalBounds())) {
            float hit_position = (ball.getPosition().y - left_racket.getPosition().y) / left_racket.getSize().y;
            ball_speed.x = fabs(ball_speed.x) * 1.05f; // Гарантируем движение вправо
            ball_speed.y = hit_position * 8.0f;
        }
        else if (ball.getGlobalBounds().intersects(right_racket.getGlobalBounds())) {
            float hit_position = (ball.getPosition().y - right_racket.getPosition().y) / right_racket.getSize().y;
            ball_speed.x = -fabs(ball_speed.x) * 1.05f; // Гарантируем движение влево
            ball_speed.y = hit_position * 8.0f;
        }

        // Ограничение максимальной скорости
        float max_speed = 15.0f;
        if (std::abs(ball_speed.x) > max_speed) {
            ball_speed.x = (ball_speed.x > 0) ? max_speed : -max_speed;
        }
        if (std::abs(ball_speed.y) > max_speed) {
        ball_speed.y = (ball_speed.y > 0) ? max_speed : -max_speed;
        }
    
        // движение ракеток
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && left_racket.getPosition().y > 0) {
            left_racket.move(0, -racket_speed); // Вверх
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && left_racket.getPosition().y + left_racket.getSize().y  < 720) {
            left_racket.move(0, racket_speed);  // Вниз
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) && right_racket.getPosition().y > 0) {
            right_racket.move(0, -racket_speed); // Вверх
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && right_racket.getPosition().y + left_racket.getSize().y  < 720) {
            right_racket.move(0, racket_speed); // Вниз
        }

        if (ball.getPosition().x <= 0) {
            right_score++;
            ball.setPosition(640, 360);
            ball_speed.x = fabs(ball_speed.x); 
        }
        if (ball.getPosition().x >= 1280 - ball.getRadius()*2) {
            left_score++;
            ball.setPosition(640, 360);
            ball_speed.x = -fabs(ball_speed.x); 
        }

        // Обновление текста счета
        score_text.setString(std::to_string(left_score) + " : " + std::to_string(right_score));
        score_text.setPosition(1280/2 - score_text.getLocalBounds().width/2, 20);

        window.clear();
        window.draw(ball);
        window.draw(left_racket);
        window.draw(right_racket);
        window.draw(score_text);
        window.display();
    }
    return 0;
}