#ifndef PONG_H
#define PONG_H

#include <SFML/Graphics.hpp>
#include <string>

// Состояния игры
enum class GameState {
    MainMenu,
    GameModeSelection,
    PvE,
    PvP,
    TvT,
    Settings,
    Pause
};

enum class Difficulty {
    Easy,
    Medium,
    Hard
};

class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;
    
    Button(const std::string& btnText, sf::Font& font, sf::Vector2f position, 
          sf::Color fillColor = sf::Color::White, 
          sf::Color outlineColor = sf::Color::White,
          sf::Color textColor = sf::Color::Black);
    
    bool is_mouse_over(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
};

void handle_main_menu(sf::RenderWindow& window, GameState& currentState, 
                   Button& playButton, Button& settingsButton, Button& exitButton, 
                   sf::Text& title);

void handle_game_mode_selection(sf::RenderWindow& window, GameState& currentState,
                Button& pvpButton, Button& pveButton, 
                Button& backButton, sf::Text& text);                   

void handle_game_pvp(sf::RenderWindow& window, GameState& currentState, 
               Button& gameExitButton, sf::Text& score_text,
               sf::CircleShape& ball, sf::Vector2f& ball_speed,
               sf::RectangleShape& left_racket, sf::RectangleShape& right_racket,
               int& left_score, int& right_score);

void handle_game_pve(sf::RenderWindow& window, GameState& currentState,
                Button& gameExitButton, sf::Text& score_text,
                sf::CircleShape& ball, sf::Vector2f& ball_speed,
                sf::RectangleShape& playerPaddle, sf::RectangleShape& aiPaddle,
                int& player_score, int& ai_score, Difficulty difficulty);

#endif