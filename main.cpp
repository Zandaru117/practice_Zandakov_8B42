#include "pong.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Ping pong");
    window.setFramerateLimit(60);
    
    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    
    Button playButton("Play", font, sf::Vector2f(540, 250), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button settingsButton("Settings", font, sf::Vector2f(540, 350), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button exitButton("Exit", font, sf::Vector2f(540, 450), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button gameExitButton("Back to Menu", font, sf::Vector2f(20, 20), sf::Color::Transparent, sf::Color::Transparent, sf::Color::White);
    Button pvpButton("PvP (1 vs 1)", font, sf::Vector2f(540, 250), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button pveButton("PvE (vs Computer)", font, sf::Vector2f(540, 350), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button backButton("Back", font, sf::Vector2f(540, 450), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    
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
    
    // мячик и его скорость
    sf::CircleShape ball(15.f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(640, 360);
    sf::Vector2f ball_speed(3.f, 3.f);
    
    // ракетки
    sf::RectangleShape left_racket(sf::Vector2f(15, 100));
    left_racket.setFillColor(sf::Color::White);
    left_racket.setPosition(50, 160);

    sf::RectangleShape right_racket(sf::Vector2f(15, 100));
    right_racket.setFillColor(sf::Color::White);
    right_racket.setPosition(1205, 160);

    // сложность по умолчанию
    Difficulty currentDifficulty = Difficulty::Medium;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }    
            
            if (currentState == GameState::MainMenu && 
                event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left) {
                
                if (playButton.is_mouse_over(window)) {
                    currentState = GameState::GameModeSelection;
                }
                if (settingsButton.is_mouse_over(window)) {
                    currentState = GameState::Settings;
                }
                if (exitButton.is_mouse_over(window)) {
                    window.close();
                }
            }
            
            else if (currentState == GameState::GameModeSelection) {
                if (event.type == sf::Event::MouseButtonReleased &&
                    event.mouseButton.button == sf::Mouse::Left) {
                    
                    if (pvpButton.is_mouse_over(window)) {
                        currentState = GameState::PvP;
                        left_score = 0;
                        right_score = 0;
                    }
                    else if (pveButton.is_mouse_over(window)) {
                        currentState = GameState::PvE;
                        left_score = 0;
                        right_score = 0;
                    }
                    else if (backButton.is_mouse_over(window)) {
                        currentState = GameState::MainMenu;
                    }
                }
                
                if (event.type == sf::Event::KeyPressed && 
                    event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::MainMenu;
                }
            }
            
            else if ((currentState == GameState::PvP || currentState == GameState::PvE) &&
                   event.type == sf::Event::MouseButtonReleased &&
                   event.mouseButton.button == sf::Mouse::Left &&
                   gameExitButton.is_mouse_over(window)) {
                
                currentState = GameState::GameModeSelection;
            }

            if (currentState == GameState::Settings) {
                if (event.type == sf::Event::MouseButtonReleased &&
                    event.mouseButton.button == sf::Mouse::Left &&
                    gameExitButton.is_mouse_over(window)) {
                    
                    currentState = GameState::MainMenu;
                }
                if (event.type == sf::Event::KeyPressed && 
                    event.key.code == sf::Keyboard::Escape) {
                    
                    currentState = GameState::MainMenu;
                }
            }

        }
        
        window.clear();
        
        if (currentState == GameState::MainMenu) {
            handle_main_menu(window, currentState, playButton, settingsButton, exitButton, title);
        } 
        else if (currentState == GameState::GameModeSelection) {
            handle_game_mode_selection(window, currentState, pvpButton, pveButton, backButton, title);
        }
        else if (currentState == GameState::PvP) {
            handle_game_pvp(window, currentState, gameExitButton, score_text,
                         ball, ball_speed, left_racket, right_racket,
                         left_score, right_score);
        }
        else if (currentState == GameState::PvE) {
            handle_game_pve(window, currentState, gameExitButton, score_text,
                         ball, ball_speed, left_racket, right_racket,
                         left_score, right_score, currentDifficulty);
        }
        
        window.display();
    }
    
    return 0;
}