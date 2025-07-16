#include "pong.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

// Состояния игры
enum class GameState {
    MainMenu,
    Playing,
    Settings
};

// Класс кнопки
class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;
    
    // Основной конструктор с параметрами по умолчанию
    Button(const std::string& btnText, sf::Font& font, sf::Vector2f position, 
          sf::Color fillColor = sf::Color::White, 
          sf::Color outlineColor = sf::Color::White,
          sf::Color textColor = sf::Color::Black) 
    {
        shape.setSize(sf::Vector2f(200, 50));
        shape.setFillColor(fillColor);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(outlineColor);
        shape.setPosition(position);
        
        text.setFont(font);
        text.setString(btnText);
        text.setCharacterSize(24);
        text.setFillColor(textColor);
        text.setPosition(
            position.x + shape.getSize().x/2 - text.getLocalBounds().width/2,
            position.y + shape.getSize().y/2 - text.getLocalBounds().height/2 - 5
        );
    }
    
    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        return shape.getGlobalBounds().contains(mousePos.x, mousePos.y);
    }
    
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Ping pong");
    window.setFramerateLimit(60);
    
    // Загрузка шрифта
    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    
    Button playButton("Play", font, sf::Vector2f(540, 250), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button exitButton("Exit", font, sf::Vector2f(540, 350), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    
    Button gameExitButton("Back to Menu", font, sf::Vector2f(20, 20), sf::Color::Transparent, sf::Color::Transparent, sf::Color::White);

    // Текст заголовка
    sf::Text title("PONG", font, 80);
    title.setPosition(1280/2 - title.getLocalBounds().width/2, 100);
    title.setFillColor(sf::Color::White);    
    GameState currentState = GameState::MainMenu;
    
    sf::Text score_text;
    score_text.setFont(font);
    score_text.setCharacterSize(50);
    score_text.setFillColor(sf::Color::White);
    score_text.setString("0 : 0");
    score_text.setPosition(1280/2 - score_text.getLocalBounds().width/2, 20);
    
    int left_score = 0, right_score = 0;
    
    // Инициализация мяча
    sf::CircleShape ball(15.f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(640, 360);
    sf::Vector2f ball_speed(3.f, 3.f);
    
    // Инициализация ракеток
    sf::RectangleShape left_racket(sf::Vector2f(15, 100));
    left_racket.setFillColor(sf::Color::White);
    left_racket.setPosition(50, 160);

    sf::RectangleShape right_racket(sf::Vector2f(15, 100));
    right_racket.setFillColor(sf::Color::White);
    right_racket.setPosition(1205, 160);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }    
            // Обработка клика по кнопке в меню
            if (currentState == GameState::MainMenu && 
                event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left) {
                
                if (playButton.isMouseOver(window)) {
                    currentState = GameState::Playing;
                }
                else if (exitButton.isMouseOver(window)) {
                    window.close();
                }
            }
            
            if (currentState == GameState::Playing) {
                if (event.type == sf::Event::MouseButtonReleased &&
                    event.mouseButton.button == sf::Mouse::Left &&
                    gameExitButton.isMouseOver(window)) {
                    
                    currentState = GameState::MainMenu;
                }
                
                // Возврат в меню по ESC (опционально)
                if (event.type == sf::Event::KeyPressed && 
                    event.key.code == sf::Keyboard::Escape) {
                    
                    currentState = GameState::MainMenu;
                }
            }
        }
        
        window.clear();
        
        if (currentState == GameState::MainMenu) {
            // Отрисовка меню
            window.draw(title);
            playButton.draw(window);
            exitButton.draw(window);
            
            // Подсветка кнопки при наведении
            if (playButton.isMouseOver(window)) {
                playButton.shape.setFillColor(sf::Color(100, 100, 100));
            } else {
                playButton.shape.setFillColor(sf::Color(70, 70, 70));
            }

            if (exitButton.isMouseOver(window)) {
                exitButton.shape.setFillColor(sf::Color(100, 100, 100));
            } else {
                exitButton.shape.setFillColor(sf::Color(70, 70, 70));
            }
        } 
        else if (currentState == GameState::Playing) {
            // Отрисовка игры
            window.draw(ball);
            gameExitButton.draw(window);
            
            ball.move(ball_speed);

            if (ball.getPosition().y <= 0 || ball.getPosition().y >= 720 - ball.getRadius()*2) {
                ball_speed.y = -ball_speed.y;
            }

            // Отскок от ракеток
            if (ball.getGlobalBounds().intersects(left_racket.getGlobalBounds())) {
                float hit_position = (ball.getPosition().y - left_racket.getPosition().y) / left_racket.getSize().y;
                ball_speed.x = fabs(ball_speed.x) * 1.05f;
                ball_speed.y = hit_position * 8.0f;
            }
            else if (ball.getGlobalBounds().intersects(right_racket.getGlobalBounds())) {
                float hit_position = (ball.getPosition().y - right_racket.getPosition().y) / right_racket.getSize().y;
                ball_speed.x = -fabs(ball_speed.x) * 1.05f;
                ball_speed.y = hit_position * 8.0f;
            }

            // Ограничение скорости
            float max_speed = 15.0f;
            if (std::abs(ball_speed.x) > max_speed) {
                ball_speed.x = (ball_speed.x > 0) ? max_speed : -max_speed;
            }
            if (std::abs(ball_speed.y) > max_speed) {
                ball_speed.y = (ball_speed.y > 0) ? max_speed : -max_speed;
            }

            // Управление ракетками
            const int racket_speed = 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && left_racket.getPosition().y > 0) {
                left_racket.move(0, -racket_speed);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && left_racket.getPosition().y + left_racket.getSize().y < 720) {
                left_racket.move(0, racket_speed);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) && right_racket.getPosition().y > 0) {
                right_racket.move(0, -racket_speed);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && right_racket.getPosition().y + right_racket.getSize().y < 720) {
                right_racket.move(0, racket_speed);
            }

            // Подсчёт очков
            if (ball.getPosition().x + ball.getRadius()*2 <= 0) {
                right_score++;
                ball.setPosition(640, 360);
                ball_speed = sf::Vector2f(3.f, 3.f);
            }
            else if (ball.getPosition().x >= 1280) {  
                left_score++;
                ball.setPosition(640, 360);
                ball_speed = sf::Vector2f(-3.f, 3.f);
            }

            score_text.setString(std::to_string(left_score) + " : " + std::to_string(right_score));
            score_text.setPosition(1280/2 - score_text.getLocalBounds().width/2, 20);

            // Подсветка кнопки
            if (gameExitButton.isMouseOver(window)) {
                gameExitButton.text.setFillColor(sf::Color::Yellow);
            } else {
                gameExitButton.text.setFillColor(sf::Color::White);
            }
            
            window.draw(left_racket);
            window.draw(right_racket);
            window.draw(score_text);
        }
        
        window.display();
    }
    
    return 0;
}