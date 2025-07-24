#include "pong.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Ping pong");
    window.setFramerateLimit(60);
    
    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    GameSettings settings;

    GameAudio audio;
    audio.loadSounds();
    audio.backgroundMusic.play();  // Запускаем музыку один раз при старте
    
    Button playButton("Play", font, sf::Vector2f(540, 250), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button settingsButton("Settings", font, sf::Vector2f(540, 350), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button exitButton("Exit", font, sf::Vector2f(540, 450), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button gameExitButton("Back to Menu", font, sf::Vector2f(20, 20), sf::Color::Transparent, sf::Color::Transparent, sf::Color::White);
    Button pvpButton("PvP (1 vs 1)", font, sf::Vector2f(540, 250), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button pveButton("PvE (vs Computer)", font, sf::Vector2f(540, 350), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button tvtButton("TvT (2 vs 2)", font, sf::Vector2f(540, 450), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    Button backButton("Back", font, sf::Vector2f(540, 550), sf::Color(70, 70, 70), sf::Color::White, sf::Color::White);
    
    sf::Text title("PONG", font, 80);
    title.setPosition(1280/2 - title.getLocalBounds().width/2, 100);
    title.setFillColor(sf::Color::White);
    
    Button easyButton("Easy", font, sf::Vector2f(330, 450), 
                     settings.difficulty == Difficulty::Easy ? sf::Color::Green : sf::Color(70, 70, 70));
    Button mediumButton("Medium", font, sf::Vector2f(540, 450), 
                       settings.difficulty == Difficulty::Medium ? sf::Color::Green : sf::Color(70, 70, 70));
    Button hardButton("Hard", font, sf::Vector2f(750, 450), 
                     settings.difficulty == Difficulty::Hard ? sf::Color::Green : sf::Color(70, 70, 70));
    
    // Ползунки
    Slider ballSpeedSlider("Ball Speed", font, 540, 250, 1, 10, settings.ballSpeed);
    Slider winScoreSlider("Win Score", font, 540, 350, 1, 15, settings.winScore);
    
    GameState currentState = GameState::MainMenu;
    
    sf::Text score_text;
    score_text.setFont(font);
    score_text.setCharacterSize(50);
    score_text.setFillColor(sf::Color::White);
    score_text.setString("0 : 0");
    score_text.setPosition(1280/2 - score_text.getLocalBounds().width/2, 20);
    
    int left_score = 0, right_score = 0;
    bool gameOver = false;
    
    // мячик и его скорость
    sf::CircleShape ball(15.f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(640, 360);
    sf::Vector2f ball_speed(settings.ballSpeed, settings.ballSpeed);
    
    // для пвп
    sf::RectangleShape left_racket(sf::Vector2f(15, 100));
    left_racket.setFillColor(sf::Color::White);
    left_racket.setPosition(50, 160);

    sf::RectangleShape right_racket(sf::Vector2f(15, 100));
    right_racket.setFillColor(sf::Color::White);
    right_racket.setPosition(1205, 160);

    // для пве
    sf::RectangleShape playerPaddle(sf::Vector2f(15, 100));
    playerPaddle.setFillColor(sf::Color::White);
    playerPaddle.setPosition(50, 160);

    sf::RectangleShape aiPaddle(sf::Vector2f(15, 100));
    aiPaddle.setFillColor(sf::Color::White);
    aiPaddle.setPosition(1205, 160);

    Difficulty currentDifficulty = Difficulty::Easy;

    // для твт
    sf::RectangleShape left_racket1(sf::Vector2f(15, 100));
    left_racket1.setFillColor(sf::Color::Red);
    left_racket1.setPosition(50, 160);

    sf::RectangleShape left_racket2(sf::Vector2f(15, 100));
    left_racket2.setFillColor(sf::Color::Red);
    left_racket2.setPosition(70, 160);

    sf::RectangleShape right_racket1(sf::Vector2f(15, 100));
    right_racket1.setFillColor(sf::Color::Blue);
    right_racket1.setPosition(1205, 160);

    sf::RectangleShape right_racket2(sf::Vector2f(15, 100));
    right_racket2.setFillColor(sf::Color::Blue);
    right_racket2.setPosition(1185, 160);


    
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
                    gameOver = false;
                    left_score = 0;
                    right_score = 0;
                }
                if (settingsButton.is_mouse_over(window)) {
                    currentState = GameState::Settings;
                }
                if (exitButton.is_mouse_over(window)) {
                    audio.backgroundMusic.stop();
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
                        gameOver = false;
                        ball_speed = sf::Vector2f(settings.ballSpeed, settings.ballSpeed);
                    }
                    else if (pveButton.is_mouse_over(window)) {
                        currentState = GameState::PvE;
                        left_score = 0;
                        right_score = 0;
                        gameOver = false;
                        ball_speed = sf::Vector2f(settings.ballSpeed, settings.ballSpeed);
                    }
                    else if (tvtButton.is_mouse_over(window)) {
                        currentState = GameState::TvT;
                        left_score = 0;
                        right_score = 0;
                        gameOver = false;
                        ball_speed = sf::Vector2f(settings.ballSpeed, settings.ballSpeed);
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
            
            else if ((currentState == GameState::PvP || currentState == GameState::PvE || currentState == GameState::TvT) &&
                   event.type == sf::Event::MouseButtonReleased &&
                   event.mouseButton.button == sf::Mouse::Left &&
                   gameExitButton.is_mouse_over(window)) {
                
                currentState = GameState::GameModeSelection;
            }

            if (currentState == GameState::Settings) {
                // Обработка ползунков
                ballSpeedSlider.handleEvent(event, window);
                winScoreSlider.handleEvent(event, window);
                
                // Обработка кнопок сложности
                if (event.type == sf::Event::MouseButtonReleased &&
                    event.mouseButton.button == sf::Mouse::Left) {
                    
                    if (easyButton.is_mouse_over(window)) {
                        currentDifficulty = Difficulty::Easy;
                        easyButton.shape.setFillColor(sf::Color::Green);
                        mediumButton.shape.setFillColor(sf::Color(70, 70, 70));
                        hardButton.shape.setFillColor(sf::Color(70, 70, 70));
                    }
                    else if (mediumButton.is_mouse_over(window)) {
                        currentDifficulty = Difficulty::Medium;
                        easyButton.shape.setFillColor(sf::Color(70, 70, 70));
                        mediumButton.shape.setFillColor(sf::Color::Green);
                        hardButton.shape.setFillColor(sf::Color(70, 70, 70));
                    }
                    else if (hardButton.is_mouse_over(window)) {
                        currentDifficulty = Difficulty::Hard;
                        easyButton.shape.setFillColor(sf::Color(70, 70, 70));
                        mediumButton.shape.setFillColor(sf::Color(70, 70, 70));
                        hardButton.shape.setFillColor(sf::Color::Green);
                    }
                    else if (backButton.is_mouse_over(window)) {
                        settings.ballSpeed = ballSpeedSlider.getValue();
                        settings.winScore = static_cast<int>(winScoreSlider.getValue());
                        settings.difficulty = currentDifficulty;
                        currentState = GameState::MainMenu;
                    }
                }
                
                // Обработка выхода по ESC
                if (event.type == sf::Event::KeyPressed && 
                    event.key.code == sf::Keyboard::Escape) {
                    settings.ballSpeed = ballSpeedSlider.getValue();
                    settings.winScore = static_cast<int>(winScoreSlider.getValue());
                    settings.difficulty = currentDifficulty;
                    currentState = GameState::MainMenu;
                }
            }

            if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                currentState = GameState::GameModeSelection;
                gameOver = false;
            }

        }
        
        window.clear();
        
        if (currentState == GameState::MainMenu) {
            handle_main_menu(window, currentState, playButton, settingsButton, exitButton, title);
        } 
        else if (currentState == GameState::GameModeSelection) {
            handle_game_mode_selection(window, currentState, pvpButton, pveButton, tvtButton, backButton, title);
        }
        else if (currentState == GameState::PvP) {
            handle_game_pvp(window, currentState, gameExitButton, score_text,
                        ball, ball_speed, left_racket, right_racket,
                        left_score, right_score, settings, gameOver, audio);
        }
        else if (currentState == GameState::PvE) {
            handle_game_pve(window, currentState, gameExitButton, score_text,
                        ball, ball_speed, playerPaddle, aiPaddle,
                        left_score, right_score, settings, gameOver, audio);
        }

        else if (currentState == GameState::TvT) {
            handle_game_tvt(window, currentState, gameExitButton, score_text,
                        ball, ball_speed, left_racket1, left_racket2, right_racket1, right_racket2, 
                        left_score, right_score, settings, gameOver, audio);
        }

        else if (currentState == GameState::Settings) {
            window.clear();
            
            // Рисуем заголовок
            sf::Text settingsTitle("Settings", font, 50);
            settingsTitle.setPosition(1280/2 - settingsTitle.getLocalBounds().width/2, 50);
            settingsTitle.setFillColor(sf::Color::White);
            window.draw(settingsTitle);
            
            ballSpeedSlider.draw(window);
            winScoreSlider.draw(window);
    
            easyButton.draw(window);
            mediumButton.draw(window);
            hardButton.draw(window);
            backButton.draw(window);
        }
        
        window.display();
    }
    
    return 0;
}