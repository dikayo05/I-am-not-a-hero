#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <optional>

enum class AnimationState
{
    Idle,
    Walking,
    Jumping
};

class Player
{
private:
    sf::Texture idleTexture;
    sf::Texture walkTexture;
    sf::Texture jumpTexture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::FloatRect m_collisionBox; // Hitbox kustom
    float speed;
    float jumpForce;
    float gravity;
    bool isOnGround;
    bool isJumping;
    bool m_isFacingRight;

    // Sistem animasi
    AnimationState currentState;
    AnimationState previousState;
    sf::IntRect currentFrame;
    int currentFrameIndex;
    float animationTimer;
    float idleAnimSpeed;
    float walkAnimSpeed;

    struct AnimationConfig
    {
        int frameCount;
        int columns;
        int rows;
    };

    AnimationConfig idleAnim;
    AnimationConfig walkAnim;
    AnimationConfig jumpAnim;

    int currentFrameWidth;
    int currentFrameHeight;

public:
    Player(const std::string &idleTexturePath,
           const std::string &walkTexturePath,
           const std::string &jumpTexturePath,
           float positionX,
           float positionY);
    void handleInput();
    void updateAnimation(float deltaTime);
    void applyPhysics(float deltaTime, const std::vector<sf::FloatRect> &groundBoxes);
    void update(float deltaTime, const std::vector<sf::FloatRect> &groundBoxes);
    void draw(sf::RenderWindow &window);
    bool isFacingRight();
    sf::Vector2f getPosition() const;
    sf::FloatRect getCollisionHitbox() const;
    void setIdleAnimation(int columns, int rows, int frameCount);
    void setWalkAnimation(int columns, int rows, int frameCount);
    void setJumpAnimation(int columns, int rows, int frameCount);
    void setJumpForce(float force);
    void setGravity(float grav);
};