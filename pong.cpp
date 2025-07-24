#include "pong.h"

#include <cmath>
#include <iostream>

void GameAudio::loadSounds() {
    
    backgroundMusic.openFromFile("C:/C++/practice/practice_course_first/practice_Zandakov_8B42/assets/sounds/background.wav");
    backgroundMusic.setLoop(true);
    
    paddleHitBuffer.loadFromFile("C:/C++/practice/practice_course_first/practice_Zandakov_8B42/assets/sounds/paddle_hit.wav");
    paddleHitSound.setBuffer(paddleHitBuffer);

    wallHitBuffer.loadFromFile("C:/C++/practice/practice_course_first/practice_Zandakov_8B42/assets/sounds/wall_hit.wav");
    wallHitSound.setBuffer(wallHitBuffer);
    
    scoreBuffer.loadFromFile("C:/C++/practice/practice_course_first/practice_Zandakov_8B42/assets/sounds/score.wav");
    scoreSound.setBuffer(scoreBuffer);
    
}

Button::Button(const std::string& btnText, sf::Font& font, sf::Vector2f position, 
    sf::Color fillColor, sf::Color outlineColor, sf::Color textColor) {
        
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

bool Button::is_mouse_over(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return shape.getGlobalBounds().contains(mousePos.x, mousePos.y);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

Slider::Slider(const std::string& name, sf::Font& font, float x, float y, 
              float minVal, float maxVal, float initialValue)
    : minValue(minVal), maxValue(maxVal), currentValue(initialValue) {
    
    track.setSize(sf::Vector2f(200, 5));
    track.setFillColor(sf::Color(100, 100, 100));
    track.setPosition(x, y);
    
    thumb.setRadius(10);
    thumb.setFillColor(sf::Color::White);
    
    label.setFont(font);
    label.setString(name);
    label.setCharacterSize(20);
    label.setFillColor(sf::Color::White);
    label.setPosition(x - 150, y - 25);
    
    valueText.setFont(font);
    valueText.setCharacterSize(20);
    valueText.setFillColor(sf::Color::White);
    
    setValue(initialValue);
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(thumb);
    window.draw(label);
    window.draw(valueText);
}

bool Slider::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && 
        event.mouseButton.button == sf::Mouse::Left) {
        
        if (thumb.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            isDragging = true;
            return true;
        }
        
        if (track.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            float newValue = minValue + (maxValue - minValue) * 
                           ((event.mouseButton.x - track.getPosition().x) / track.getSize().x);
            setValue(newValue);
            return true;
        }
    }
    
    if (event.type == sf::Event::MouseButtonReleased && 
        event.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    }
    
    if (isDragging && event.type == sf::Event::MouseMoved) {
        float newX = event.mouseMove.x;
        // Ограничиваем позицию в пределах трека
        if (newX < track.getPosition().x) newX = track.getPosition().x;
        if (newX > track.getPosition().x + track.getSize().x) newX = track.getPosition().x + track.getSize().x;
        
        float newValue = minValue + (maxValue - minValue) * 
                       ((newX - track.getPosition().x) / track.getSize().x);
        setValue(newValue);
        return true;
    }
    
    return false;
}

void Slider::setValue(float value) {
    currentValue = value;
    if (currentValue < minValue) currentValue = minValue;
    if (currentValue > maxValue) currentValue = maxValue;
    
    valueText.setString(std::to_string((int)currentValue));
    valueText.setPosition(track.getPosition().x + track.getSize().x + 10, track.getPosition().y - 15);
    
    // Обновляем позицию бегунка
    float ratio = (currentValue - minValue) / (maxValue - minValue);
    thumb.setPosition(
        track.getPosition().x + ratio * track.getSize().x - thumb.getRadius(),
        track.getPosition().y - thumb.getRadius() + track.getSize().y/2
    );
}

void handle_main_menu(sf::RenderWindow& window, GameState& currentState, 
    Button& playButton, Button& settingsButton, Button& exitButton, 
    sf::Text& title) {
    
    window.draw(title);
    playButton.draw(window);
    settingsButton.draw(window);
    exitButton.draw(window);
    
    if (playButton.is_mouse_over(window)) {
        playButton.shape.setFillColor(sf::Color(100, 100, 100));
    } else {
        playButton.shape.setFillColor(sf::Color(70, 70, 70));
    }

    if (settingsButton.is_mouse_over(window)) {
        settingsButton.shape.setFillColor(sf::Color(100, 100, 100));
    } else {
        settingsButton.shape.setFillColor(sf::Color(70, 70, 70));
    }

    if (exitButton.is_mouse_over(window)) {
        exitButton.shape.setFillColor(sf::Color(100, 100, 100));
    } else {
        exitButton.shape.setFillColor(sf::Color(70, 70, 70));
    }
}

void handle_game_mode_selection(sf::RenderWindow& window, GameState& currentState,
                           Button& pvpButton, Button& pveButton, Button& tvtButton, Button& backButton,
                           sf::Text& title) {
    window.draw(title);
    pvpButton.draw(window);
    pveButton.draw(window);
    tvtButton.draw(window);
    backButton.draw(window);
    
    // Подсветка кнопок при наведении
    if (pvpButton.is_mouse_over(window)) {
        pvpButton.shape.setFillColor(sf::Color(100, 100, 100));
    } else {
        pvpButton.shape.setFillColor(sf::Color(70, 70, 70));
    }

    if (pveButton.is_mouse_over(window)) {
        pveButton.shape.setFillColor(sf::Color(100, 100, 100));
    } else {
        pveButton.shape.setFillColor(sf::Color(70, 70, 70));
    }

    if (tvtButton.is_mouse_over(window)) {
        tvtButton.shape.setFillColor(sf::Color(100, 100, 100));
    } else {
        tvtButton.shape.setFillColor(sf::Color(70, 70, 70));
    }

    if (backButton.is_mouse_over(window)) {
        backButton.shape.setFillColor(sf::Color(100, 100, 100));
    } else {
        backButton.shape.setFillColor(sf::Color(70, 70, 70));
    }
}

void handle_settings(sf::RenderWindow& window, GameState& currentState,
                   Slider& ballSpeedSlider, Slider& winScoreSlider,
                   Button& easyButton, Button& mediumButton, Button& hardButton,
                   Button& backButton, GameSettings& settings) {
    
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        ballSpeedSlider.handleEvent(event, window);
        winScoreSlider.handleEvent(event, window);
        
        if (event.type == sf::Event::MouseButtonReleased && 
            event.mouseButton.button == sf::Mouse::Left) {
            
            if (easyButton.is_mouse_over(window)) {
                settings.difficulty = Difficulty::Easy;
                easyButton.shape.setFillColor(sf::Color::Green);
                mediumButton.shape.setFillColor(sf::Color(70, 70, 70));
                hardButton.shape.setFillColor(sf::Color(70, 70, 70));
            }
            else if (mediumButton.is_mouse_over(window)) {
                settings.difficulty = Difficulty::Medium;
                easyButton.shape.setFillColor(sf::Color(70, 70, 70));
                mediumButton.shape.setFillColor(sf::Color::Green);
                hardButton.shape.setFillColor(sf::Color(70, 70, 70));
            }
            else if (hardButton.is_mouse_over(window)) {
                settings.difficulty = Difficulty::Hard;
                easyButton.shape.setFillColor(sf::Color(70, 70, 70));
                mediumButton.shape.setFillColor(sf::Color(70, 70, 70));
                hardButton.shape.setFillColor(sf::Color::Green);
            }
            else if (backButton.is_mouse_over(window)) {
                // Сохраняем настройки перед выходом
                settings.ballSpeed = ballSpeedSlider.getValue();
                settings.winScore = (int)winScoreSlider.getValue();
                currentState = GameState::MainMenu;
            }
        }
    }
    
    window.clear();
    
    // Рисуем заголовок
    sf::Text settingsTitle("Settings", *backButton.text.getFont(), 50);
    settingsTitle.setPosition(1280/2 - settingsTitle.getLocalBounds().width/2, 50);
    settingsTitle.setFillColor(sf::Color::White);
    window.draw(settingsTitle);
    
    ballSpeedSlider.draw(window);
    winScoreSlider.draw(window);
    
    easyButton.draw(window);
    mediumButton.draw(window);
    hardButton.draw(window);
    
    backButton.draw(window);
    
    window.display();
}

void handle_game_pvp(sf::RenderWindow& window, GameState& currentState, Button& gameExitButton, 
    sf::Text& score_text, sf::CircleShape& ball, sf::Vector2f& ball_speed, 
    sf::RectangleShape& left_racket, sf::RectangleShape& right_racket, 
    int& left_score, int& right_score, const GameSettings& settings, bool& gameOver, GameAudio& audio) {
    
    window.draw(ball);
    gameExitButton.draw(window);
    
    if (!gameOver) {
        ball.move(ball_speed);

        if (ball.getPosition().y <= 0 || ball.getPosition().y >= 720 - ball.getRadius()*2) {
            audio.wallHitSound.play();
            ball_speed.y = -ball_speed.y;
        }

        if (ball.getGlobalBounds().intersects(left_racket.getGlobalBounds())) {
            audio.paddleHitSound.play();
            float hit_position = (ball.getPosition().y - left_racket.getPosition().y) / left_racket.getSize().y;
            ball_speed.x = fabs(ball_speed.x) * 1.05f;
            ball_speed.y = hit_position * 8.0f;
        }
        else if (ball.getGlobalBounds().intersects(right_racket.getGlobalBounds())) {
            audio.paddleHitSound.play();
            float hit_position = (ball.getPosition().y - right_racket.getPosition().y) / right_racket.getSize().y;
            ball_speed.x = -fabs(ball_speed.x) * 1.05f;
            ball_speed.y = hit_position * 8.0f;
        }

        float max_speed = 15.0f;
        if (std::abs(ball_speed.x) > max_speed) {
            ball_speed.x = (ball_speed.x > 0) ? max_speed : -max_speed;
        }
        if (std::abs(ball_speed.y) > max_speed) {
            ball_speed.y = (ball_speed.y > 0) ? max_speed : -max_speed;
        }

        const int RACKET_SPEED = 10;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && left_racket.getPosition().y > 0) {
            left_racket.move(0, -RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && left_racket.getPosition().y + left_racket.getSize().y < 720) {
            left_racket.move(0, RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) && right_racket.getPosition().y > 0) {
            right_racket.move(0, -RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && right_racket.getPosition().y + right_racket.getSize().y < 720) {
            right_racket.move(0, RACKET_SPEED);
        }

        if (ball.getPosition().x + ball.getRadius()*2 <= 0) {
            audio.scoreSound.play();
            right_score++;
            ball.setPosition(640, 360);
            ball_speed = sf::Vector2f(settings.ballSpeed, settings.ballSpeed);
        }
        else if (ball.getPosition().x >= 1280) {  
            audio.scoreSound.play();
            left_score++;
            ball.setPosition(640, 360);
            ball_speed = sf::Vector2f(-settings.ballSpeed, settings.ballSpeed);
        }
    }

    score_text.setString(std::to_string(left_score) + " : " + std::to_string(right_score));
    score_text.setPosition(1280/2 - score_text.getLocalBounds().width/2, 20);

    if (left_score >= settings.winScore || right_score >= settings.winScore) {
        gameOver = true;
        sf::Text winText;
        winText.setFont(*score_text.getFont());
        winText.setCharacterSize(60);
        winText.setFillColor(sf::Color::White);
        
        if (left_score >= settings.winScore) {
            winText.setString("Left Player Wins!");
        } else {
            winText.setString("Right Player Wins!");
        }
        
        winText.setPosition(1280/2 - winText.getLocalBounds().width/2, 260 - winText.getLocalBounds().height/2);
        window.draw(winText);
        
        sf::Text restartText("Press SPACE to return to menu", *score_text.getFont(), 30);
        restartText.setPosition(1280/2 - restartText.getLocalBounds().width/2, 450);
        restartText.setFillColor(sf::Color::White);
        window.draw(restartText);
    }

    if (gameExitButton.is_mouse_over(window)) {
        gameExitButton.text.setFillColor(sf::Color::Yellow);
    } else {
        gameExitButton.text.setFillColor(sf::Color::White);
    }
    
    window.draw(left_racket);
    window.draw(right_racket);
    window.draw(score_text);
}

void handle_game_pve(sf::RenderWindow& window, GameState& currentState,
                 Button& gameExitButton, sf::Text& score_text,
                 sf::CircleShape& ball, sf::Vector2f& ball_speed,
                 sf::RectangleShape& playerPaddle, sf::RectangleShape& aiPaddle,
                 int& player_score, int& ai_score, const GameSettings& settings, bool& gameOver, GameAudio& audio) {
    window.draw(ball);
    gameExitButton.draw(window);
    
    if (!gameOver) {
        ball.move(ball_speed);

        if (ball.getPosition().y <= 0 || ball.getPosition().y >= 720 - ball.getRadius()*2) {
            audio.wallHitSound.play();
            ball_speed.y = -ball_speed.y;
        }

        if (ball.getGlobalBounds().intersects(playerPaddle.getGlobalBounds())) {
            audio.paddleHitSound.play();
            float hit_position = (ball.getPosition().y - playerPaddle.getPosition().y) / playerPaddle.getSize().y;
            ball_speed.x = fabs(ball_speed.x) * 1.05f;
            ball_speed.y = hit_position * 8.0f;
        }
        else if (ball.getGlobalBounds().intersects(aiPaddle.getGlobalBounds())) {
            audio.paddleHitSound.play();
            float hit_position = (ball.getPosition().y - aiPaddle.getPosition().y) / aiPaddle.getSize().y;
            ball_speed.x = -fabs(ball_speed.x) * 1.05f;
            ball_speed.y = hit_position * 8.0f;
        }

        const int RACKET_SPEED = 10;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && playerPaddle.getPosition().y > 0) {
            playerPaddle.move(0, -RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && playerPaddle.getPosition().y + playerPaddle.getSize().y < 720) {
            playerPaddle.move(0, RACKET_SPEED);
        }

        // ИИ для компьютера с учетом сложности
        float paddleCenter = aiPaddle.getPosition().y + aiPaddle.getSize().y / 2;
        float ballCenter = ball.getPosition().y + ball.getRadius();
        
        float reaction_error = 0.f;
        float ai_speed = 0.f;
        
        switch (settings.difficulty) {
            case Difficulty::Easy: reaction_error = 30.f; ai_speed = 3.f; break;
            case Difficulty::Medium: reaction_error = 15.f; ai_speed = 5.f; break;
            case Difficulty::Hard: reaction_error = 5.f; ai_speed = 7.f; break;
        }
        
        float targetY = ballCenter + (rand() % (int)reaction_error - reaction_error/2);
        
        if (paddleCenter < targetY - 10 && aiPaddle.getPosition().y + aiPaddle.getSize().y < window.getSize().y) {
            aiPaddle.move(0, ai_speed);
        } 
        else if (paddleCenter > targetY + 10 && aiPaddle.getPosition().y > 0) {
            aiPaddle.move(0, -ai_speed);
        }

        if (ball.getPosition().x + ball.getRadius()*2 <= 0) {
            audio.scoreSound.play();
            ai_score++;
            ball.setPosition(640, 360);
            ball_speed = sf::Vector2f(settings.ballSpeed, settings.ballSpeed);
        }
        else if (ball.getPosition().x >= 1280) {  
            audio.scoreSound.play();
            player_score++;
            ball.setPosition(640, 360);
            ball_speed = sf::Vector2f(-settings.ballSpeed, settings.ballSpeed);
        }
    }

    score_text.setString(std::to_string(player_score) + " : " + std::to_string(ai_score));
    score_text.setPosition(1280/2 - score_text.getLocalBounds().width/2, 20);

    if (player_score >= settings.winScore || ai_score >= settings.winScore) {
        gameOver = true;
        sf::Text winText;
        winText.setFont(*score_text.getFont());
        winText.setCharacterSize(60);
        winText.setFillColor(sf::Color::White);
        
        if (player_score >= settings.winScore) {
            winText.setString("Player Wins!");
        } else {
            winText.setString("Computer Wins!");
        }
        
        winText.setPosition(1280/2 - winText.getLocalBounds().width/2, 260 - winText.getLocalBounds().height/2);
        window.draw(winText);
        
        sf::Text restartText("Press SPACE to return to menu", *score_text.getFont(), 30);
        restartText.setPosition(1280/2 - restartText.getLocalBounds().width/2, 450);
        restartText.setFillColor(sf::Color::White);
        window.draw(restartText);
    }

    if (gameExitButton.is_mouse_over(window)) {
        gameExitButton.text.setFillColor(sf::Color::Yellow);
    } else {
        gameExitButton.text.setFillColor(sf::Color::White);
    }
    
    window.draw(playerPaddle);
    window.draw(aiPaddle);
    window.draw(score_text);
}

void handle_game_tvt(sf::RenderWindow& window, GameState& currentState,
                Button& gameExitButton, sf::Text& score_text,
                sf::CircleShape& ball, sf::Vector2f& ball_speed,
                sf::RectangleShape& left_racket1, sf::RectangleShape& left_racket2,
                sf::RectangleShape& right_racket1, sf::RectangleShape& right_racket2,
                int& left_score, int& right_score, const GameSettings& settings, bool& gameOver, GameAudio& audio) {
                    
    window.draw(ball);
    gameExitButton.draw(window);
    
    if (!gameOver) {
        ball.move(ball_speed);

        if (ball.getPosition().y <= 0 || ball.getPosition().y >= 720 - ball.getRadius()*2) {
            audio.wallHitSound.play();
            ball_speed.y = -ball_speed.y;
        }

        if (ball.getGlobalBounds().intersects(left_racket1.getGlobalBounds())) {
            audio.paddleHitSound.play();
            float hit_position = (ball.getPosition().y - left_racket1.getPosition().y) / left_racket1.getSize().y;
            ball_speed.x = fabs(ball_speed.x) * 1.05f;
            ball_speed.y = hit_position * 8.0f;
        }
        else if (ball.getGlobalBounds().intersects(right_racket1.getGlobalBounds())) {
            audio.paddleHitSound.play();
            float hit_position = (ball.getPosition().y - right_racket1.getPosition().y) / right_racket1.getSize().y;
            ball_speed.x = -fabs(ball_speed.x) * 1.05f;
            ball_speed.y = hit_position * 8.0f;
        }
        if (ball.getGlobalBounds().intersects(left_racket2.getGlobalBounds())) {
            audio.paddleHitSound.play();
            float hit_position = (ball.getPosition().y - left_racket2.getPosition().y) / left_racket2.getSize().y;
            ball_speed.x = fabs(ball_speed.x) * 1.05f;
            ball_speed.y = hit_position * 8.0f;
        }
        else if (ball.getGlobalBounds().intersects(right_racket2.getGlobalBounds())) {
            audio.paddleHitSound.play();
            float hit_position = (ball.getPosition().y - right_racket2.getPosition().y) / right_racket2.getSize().y;
            ball_speed.x = -fabs(ball_speed.x) * 1.05f;
            ball_speed.y = hit_position * 8.0f;
        }

        const int RACKET_SPEED = 10;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && left_racket1.getPosition().y > 0) {
            left_racket1.move(0, -RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && left_racket1.getPosition().y + left_racket1.getSize().y < 720) {
            left_racket1.move(0, RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && left_racket2.getPosition().y > 0) {
            left_racket2.move(0, -RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::V) && left_racket2.getPosition().y + left_racket2.getSize().y < 720) {
            left_racket2.move(0, RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) && right_racket1.getPosition().y > 0) {
            right_racket1.move(0, -RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && right_racket1.getPosition().y + right_racket1.getSize().y < 720) {
            right_racket1.move(0, RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) && right_racket2.getPosition().y > 0) {
            right_racket2.move(0, -RACKET_SPEED);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M) && right_racket2.getPosition().y + right_racket2.getSize().y < 720) {
            right_racket2.move(0, RACKET_SPEED);
        }

        if (ball.getPosition().x + ball.getRadius()*2 <= 0) {
            audio.scoreSound.play();
            right_score++;
            ball.setPosition(640, 360);
            ball_speed = sf::Vector2f(settings.ballSpeed, settings.ballSpeed);
        }
        else if (ball.getPosition().x >= 1280) {  
            audio.scoreSound.play();
            left_score++;
            ball.setPosition(640, 360);
            ball_speed = sf::Vector2f(-settings.ballSpeed, settings.ballSpeed);
        }
    }

    score_text.setString(std::to_string(left_score) + " : " + std::to_string(right_score));
    score_text.setPosition(1280/2 - score_text.getLocalBounds().width/2, 20);

    if (left_score >= settings.winScore || right_score >= settings.winScore) {
        gameOver = true;
        sf::Text winText;
        winText.setFont(*score_text.getFont());
        winText.setCharacterSize(60);
        winText.setFillColor(sf::Color::White);
        
        if (left_score >= settings.winScore) {
            winText.setString("Red Team Wins!");
        } else {
            winText.setString("Blue Team Wins!");
        }
        
        winText.setPosition(1280/2 - winText.getLocalBounds().width/2, 260 - winText.getLocalBounds().height/2);
        window.draw(winText);
        
        sf::Text restartText("Press SPACE to return to menu", *score_text.getFont(), 30);
        restartText.setPosition(1280/2 - restartText.getLocalBounds().width/2, 450);
        restartText.setFillColor(sf::Color::White);
        window.draw(restartText);
    }

    if (gameExitButton.is_mouse_over(window)) {
        gameExitButton.text.setFillColor(sf::Color::Yellow);
    } else {
        gameExitButton.text.setFillColor(sf::Color::White);
    }
    
    window.draw(left_racket1);
    window.draw(left_racket2);
    window.draw(right_racket1);
    window.draw(right_racket2);
    window.draw(score_text);
}