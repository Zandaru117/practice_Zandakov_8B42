#include "pong.h"
#include <cmath>

Button::Button(const std::string& btnText, sf::Font& font, sf::Vector2f position, sf::Color fillColor, sf::Color outlineColor, sf::Color textColor) {
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
                           Button& pvpButton, Button& pveButton, Button& backButton,
                           sf::Text& title) {
    window.draw(title);
    pvpButton.draw(window);
    pveButton.draw(window);
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

    if (backButton.is_mouse_over(window)) {
        backButton.shape.setFillColor(sf::Color(100, 100, 100));
    } else {
        backButton.shape.setFillColor(sf::Color(70, 70, 70));
    }
}

void handle_game_pvp(sf::RenderWindow& window, GameState& currentState, Button& gameExitButton, 
    sf::Text& score_text, sf::CircleShape& ball, sf::Vector2f& ball_speed, 
    sf::RectangleShape& left_racket, sf::RectangleShape& right_racket, int& left_score, int& right_score) {
    window.draw(ball);
    gameExitButton.draw(window);
    
    ball.move(ball_speed);

    if (ball.getPosition().y <= 0 || ball.getPosition().y >= 720 - ball.getRadius()*2) {
        ball_speed.y = -ball_speed.y;
    }

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

    if (gameExitButton.is_mouse_over(window)) {
        gameExitButton.text.setFillColor(sf::Color::Yellow);
    } else {
        gameExitButton.text.setFillColor(sf::Color::White);
    }
    
    window.draw(left_racket);
    window.draw(right_racket);
    window.draw(score_text);
}

// Обработка игры против компьютера
void handle_game_pve(sf::RenderWindow& window, GameState& currentState,
                 Button& gameExitButton, sf::Text& score_text,
                 sf::CircleShape& ball, sf::Vector2f& ball_speed,
                 sf::RectangleShape& playerPaddle, sf::RectangleShape& aiPaddle,
                 int& player_score, int& ai_score, Difficulty difficulty) {
    window.draw(ball);
    gameExitButton.draw(window);
    
    // Движение мяча
    ball.move(ball_speed);

    // Коллизии со стенами
    if (ball.getPosition().y <= 0 || ball.getPosition().y >= 720 - ball.getRadius()*2) {
        ball_speed.y = -ball_speed.y;
    }

    // Коллизии с ракетками
    if (ball.getGlobalBounds().intersects(playerPaddle.getGlobalBounds())) {
        float hit_position = (ball.getPosition().y - playerPaddle.getPosition().y) / playerPaddle.getSize().y;
        ball_speed.x = fabs(ball_speed.x) * 1.05f;
        ball_speed.y = hit_position * 8.0f;
    }
    else if (ball.getGlobalBounds().intersects(aiPaddle.getGlobalBounds())) {
        float hit_position = (ball.getPosition().y - aiPaddle.getPosition().y) / aiPaddle.getSize().y;
        ball_speed.x = -fabs(ball_speed.x) * 1.05f;
        ball_speed.y = hit_position * 8.0f;
    }

    // Управление игрока
    const int RACKET_SPEED = 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && playerPaddle.getPosition().y > 0) {
        playerPaddle.move(0, -RACKET_SPEED);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && playerPaddle.getPosition().y + playerPaddle.getSize().y < 720) {
        playerPaddle.move(0, RACKET_SPEED);
    }

    // ИИ для компьютера
    float paddleCenter = aiPaddle.getPosition().y + aiPaddle.getSize().y / 2;
    float ballCenter = ball.getPosition().y + ball.getRadius();
    
    float reaction_error = 0.f;
    float ai_speed = 0.f;
    
    switch (difficulty) {
        case Difficulty::Easy: reaction_error = 30.f; ai_speed = 5.f; break;
        case Difficulty::Medium: reaction_error = 15.f; ai_speed = 7.f; break;
        case Difficulty::Hard: reaction_error = 5.f; ai_speed = 9.f; break;
    }
    
    float targetY = ballCenter + (rand() % (int)reaction_error - reaction_error/2);
    
    if (paddleCenter < targetY - 10) {
        aiPaddle.move(0, ai_speed);
    } 
    else if (paddleCenter > targetY + 10) {
        aiPaddle.move(0, -ai_speed);
    }

    // Подсчёт очков
    if (ball.getPosition().x + ball.getRadius()*2 <= 0) {
        ai_score++;
        ball.setPosition(640, 360);
        ball_speed = sf::Vector2f(3.f, 3.f);
    }
    else if (ball.getPosition().x >= 1280) {  
        player_score++;
        ball.setPosition(640, 360);
        ball_speed = sf::Vector2f(-3.f, 3.f);
    }

    score_text.setString(std::to_string(player_score) + " : " + std::to_string(ai_score));
    score_text.setPosition(1280/2 - score_text.getLocalBounds().width/2, 20);

    if (gameExitButton.is_mouse_over(window)) {
        gameExitButton.text.setFillColor(sf::Color::Yellow);
    } else {
        gameExitButton.text.setFillColor(sf::Color::White);
    }
    
    window.draw(playerPaddle);
    window.draw(aiPaddle);
    window.draw(score_text);
}