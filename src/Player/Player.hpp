#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <optional>

enum class AnimationState
{
    Idle,
    Walking,
    Jumping,
    Attacking
};

class Player
{
private:
    sf::Texture m_idleTexture;
    sf::Texture m_walkTexture;
    sf::Texture m_jumpTexture;
    sf::Texture m_attackTexture;
    sf::Sprite m_sprite;
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::FloatRect m_collisionBox;

    float m_speed;
    float m_jumpForce;
    float m_gravity;
    bool m_isOnGround;
    bool m_isJumping;
    bool m_isFacingRight;

    // animation
    AnimationState m_currentState;
    AnimationState m_previousState;
    sf::IntRect m_currentFrame;
    int m_currentFrameIndex;
    float m_animationTimer;
    float m_idleAnimSpeed;
    float m_walkAnimSpeed;
    float m_attackAnimSpeed;
    bool m_isAttacking;
    float m_attackCooldown;
    float m_attackCooldownTimer;

    sf::FloatRect m_attackHitbox;
    bool m_attackHitboxActive;

    struct AnimationConfig
    {
        int m_frameCount;
        int m_columns;
        int m_rows;
    };

    AnimationConfig m_idleAnim;
    AnimationConfig m_walkAnim;
    AnimationConfig m_jumpAnim;
    AnimationConfig m_attackAnim;

    int m_currentFrameWidth;
    int m_currentFrameHeight;

public:
    Player(const std::string &idleTexturePath,
           const std::string &walkTexturePath,
           const std::string &jumpTexturePath,
           const std::string &attackTexturePath,
           float positionX,
           float positionY);
    void handleInput();
    void attack();
    void updateAnimation(float deltaTime);
    void applyPhysics(float deltaTime, const std::vector<sf::FloatRect> &groundBoxes);
    void update(float deltaTime, const std::vector<sf::FloatRect> &groundBoxes);
    void draw(sf::RenderWindow &window);
    bool isFacingRight();
    sf::Vector2f getPosition() const;
    sf::FloatRect getCollisionHitbox() const;
    sf::FloatRect getAttackHitbox() const;
    void drawAttackHitbox(sf::RenderWindow &window);
    bool isAttackHitboxActive() const;
    void setAttackAnimation(int columns, int rows, int frameCount);
    void setAttackSpeed(float speed);
    void setAttackCooldown(float cooldown);
    void updateAttackHitbox();
    void setIdleAnimation(int columns, int rows, int frameCount);
    void setWalkAnimation(int columns, int rows, int frameCount);
    void setJumpAnimation(int columns, int rows, int frameCount);
    void setJumpForce(float force);
    void setGravity(float grav);
};