#ifndef PONG_H
#define PONG_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

struct GameSettings {
    float ballSpeed = 3.0f;
    int winScore = 5;
    Difficulty difficulty = Difficulty::Medium;
};

struct GameAudio {
    sf::Music backgroundMusic;
    sf::SoundBuffer paddleHitBuffer;
    sf::SoundBuffer wallHitBuffer;
    sf::SoundBuffer scoreBuffer;
    sf::Sound paddleHitSound;
    sf::Sound wallHitSound;
    sf::Sound scoreSound;
    
    void loadSounds();
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

class Slider {
private:
    sf::RectangleShape track;
    sf::CircleShape thumb;
    sf::Text label;
    sf::Text valueText;
    float minValue, maxValue, currentValue;
    bool isDragging = false;
    
public:
    Slider(const std::string& name, sf::Font& font, float x, float y, float minVal, float maxVal, float initialValue);
    
    void draw(sf::RenderWindow& window);
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);
    float getValue() const { return currentValue; }
    void setValue(float value);
    
};

void handle_main_menu(sf::RenderWindow& window, GameState& currentState, 
                   Button& playButton, Button& settingsButton, Button& exitButton, 
                   sf::Text& title);

void handle_game_mode_selection(sf::RenderWindow& window, GameState& currentState,
                Button& pvpButton, Button& pveButton, Button& tvtButton, 
                Button& backButton, sf::Text& text);                   

void handle_game_pvp(sf::RenderWindow& window, GameState& currentState, 
               Button& gameExitButton, sf::Text& score_text,
               sf::CircleShape& ball, sf::Vector2f& ball_speed,
               sf::RectangleShape& left_racket, sf::RectangleShape& right_racket,
               int& left_score, int& right_score, const GameSettings& settings, bool& gameOver, GameAudio& audio);

void handle_game_pve(sf::RenderWindow& window, GameState& currentState,
                Button& gameExitButton, sf::Text& score_text,
                sf::CircleShape& ball, sf::Vector2f& ball_speed,
                sf::RectangleShape& playerPaddle, sf::RectangleShape& aiPaddle,
                int& player_score, int& ai_score, const GameSettings& settings, bool& gameOver, GameAudio& audio);

void handle_game_tvt(sf::RenderWindow& window, GameState& currentState,
                Button& gameExitButton, sf::Text& score_text,
                sf::CircleShape& ball, sf::Vector2f& ball_speed,
                sf::RectangleShape& left_racket1, sf::RectangleShape& left_racket2,
                sf::RectangleShape& right_racket1, sf::RectangleShape& right_racket2,
                int& left_score, int& right_score, const GameSettings& settings, bool& gameOver, GameAudio& audio);

#endif