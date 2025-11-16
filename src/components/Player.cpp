#include "components/Player.hpp"

Player::Player(const std::string &idleTexturePath,
               const std::string &walkTexturePath,
               const std::string &jumpTexturePath,
               const std::string &attackTexturePath,
               float positionX = 100.f,
               float positionY = 100.f)
    : m_sprite(m_idleTexture),
      m_speed(120.f),
      m_jumpForce(-350.f),
      m_gravity(900.f),
      m_position(positionX, positionY),
      m_velocity(0.f, 0.f),
      m_isOnGround(false),
      m_isJumping(false),
      m_isFacingRight(true),
      m_currentState(AnimationState::Idle),
      m_previousState(AnimationState::Idle),
      m_currentFrameIndex(0),
      m_animationTimer(0.f),
      m_idleAnimSpeed(0.1f),
      m_walkAnimSpeed(0.1f),
      m_attackAnimSpeed(0.08f),
      m_isAttacking(false),
      m_attackCooldown(0.5f),
      m_attackCooldownTimer(0.f),
      m_attackHitboxActive(false)
{
    // Load textures
    if (!m_idleTexture.loadFromFile(idleTexturePath))
    {
        std::cerr << "Error loading idle texture!" << std::endl;
    }

    if (!m_walkTexture.loadFromFile(walkTexturePath))
    {
        std::cerr << "Error loading walk texture!" << std::endl;
    }

    if (!m_jumpTexture.loadFromFile(jumpTexturePath))
    {
        std::cerr << "Error loading jump texture!" << std::endl;
    }

    if (!m_attackTexture.loadFromFile(attackTexturePath))
    {
        std::cerr << "Error loading attack texture!" << std::endl;
    }

    // configure default animation
    m_idleAnim.m_columns = 2;
    m_idleAnim.m_rows = 4;
    m_idleAnim.m_frameCount = 8;

    m_walkAnim.m_columns = 2;
    m_walkAnim.m_rows = 4;
    m_walkAnim.m_frameCount = 8;

    m_jumpAnim.m_columns = 2;
    m_jumpAnim.m_rows = 4;
    m_jumpAnim.m_frameCount = 8;

    m_attackAnim.m_columns = 8;
    m_attackAnim.m_rows = 5;
    m_attackAnim.m_frameCount = 9;

    // Setup first frame
    m_currentFrameWidth = static_cast<int>(m_idleTexture.getSize().x) / m_idleAnim.m_columns;
    m_currentFrameHeight = static_cast<int>(m_idleTexture.getSize().y) / m_idleAnim.m_rows;

    m_currentFrame = sf::IntRect({0, 0}, {m_currentFrameWidth, m_currentFrameHeight});
    m_sprite.setTextureRect(m_currentFrame);
    m_sprite.setOrigin({m_currentFrameWidth / 2.f, m_currentFrameHeight / 2.f});
    m_sprite.setPosition(m_position);

    // init custom hitbox.
    float collisionWidth = 20.f;
    float collisionHeight = 45.f;

    // These are 'local' bounds, relative to the origin (middle sprite)
    // m_collisionBox = {
    //     {-(collisionWidth / 2.f), -(collisionHeight / 2.f)}, // Posisi {left, top}
    //     {collisionWidth, collisionHeight}                    // Ukuran {width, height}
    // };

    // if you want the hitbox to be below (for platformer):
    m_collisionBox = {
        {-(collisionWidth / 2.f), (m_currentFrameHeight / 2.f) - collisionHeight},
        {collisionWidth, collisionHeight}};
}

void Player::handleInput()
{
    // Horizontal movement
    m_velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        m_velocity.x = -m_speed;
        m_isFacingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        m_velocity.x = m_speed;
        m_isFacingRight = true;
    }

    // Update scale for flip sprite
    if (m_isFacingRight)
    {
        m_sprite.setScale({1.f, 1.f}); // Normal
    }
    else
    {
        m_sprite.setScale({-1.f, 1.f}); // Flip horizontal
    }

    // Jump
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) &&
        m_isOnGround && !m_isJumping)
    {
        m_velocity.y = m_jumpForce;
        m_isJumping = true;
        m_isOnGround = false;
    }

    // Attack
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
    {
        attack();
    }

    // Update state
    m_previousState = m_currentState;
    if (m_isAttacking)
    {
        m_currentState = AnimationState::Attacking;
    }
    else if (!m_isOnGround)
    {
        m_currentState = AnimationState::Jumping;
    }
    else if (m_velocity.x != 0)
    {
        m_currentState = AnimationState::Walking;
    }
    else
    {
        m_currentState = AnimationState::Idle;
    }
}

void Player::attack()
{
    if (!m_isAttacking && m_attackCooldownTimer <= 0.f)
    {
        m_isAttacking = true;
        m_attackCooldownTimer = m_attackCooldown;
        m_currentFrameIndex = 0;
        m_animationTimer = 0.f;

        // activate on certain frame (example frame 3-5)
        m_attackHitboxActive = false;
    }
}

void Player::updateAnimation(float deltaTime)
{
    // change texture if state changed
    if (m_currentState != m_previousState)
    {
        m_currentFrameIndex = 0;
        m_animationTimer = 0.f;

        if (m_currentState == AnimationState::Idle)
        {
            m_sprite.setTexture(m_idleTexture);
            m_currentFrameWidth = static_cast<int>(m_idleTexture.getSize().x) / m_idleAnim.m_columns;
            m_currentFrameHeight = static_cast<int>(m_idleTexture.getSize().y) / m_idleAnim.m_rows;
        }
        else if (m_currentState == AnimationState::Walking)
        {
            m_sprite.setTexture(m_walkTexture);
            m_currentFrameWidth = static_cast<int>(m_walkTexture.getSize().x) / m_walkAnim.m_columns;
            m_currentFrameHeight = static_cast<int>(m_walkTexture.getSize().y) / m_walkAnim.m_rows;
        }
        else if (m_currentState == AnimationState::Jumping)
        {
            m_sprite.setTexture(m_jumpTexture);
            m_currentFrameWidth = static_cast<int>(m_jumpTexture.getSize().x) / m_jumpAnim.m_columns;
            m_currentFrameHeight = static_cast<int>(m_jumpTexture.getSize().y) / m_jumpAnim.m_rows;
        }
        else if (m_currentState == AnimationState::Attacking)
        {
            m_sprite.setTexture(m_attackTexture);
            m_currentFrameWidth = static_cast<int>(m_attackTexture.getSize().x) / m_attackAnim.m_columns;
            m_currentFrameHeight = static_cast<int>(m_attackTexture.getSize().y) / m_attackAnim.m_rows;
        }

        m_sprite.setOrigin({m_currentFrameWidth / 2.f, m_currentFrameHeight / 2.f});
    }

    m_animationTimer += deltaTime;
    AnimationConfig *currentAnim = nullptr;
    float currentAnimSpeed = 0.f;

    if (m_currentState == AnimationState::Idle)
    {
        currentAnim = &m_idleAnim;
        currentAnimSpeed = m_idleAnimSpeed;
    }
    else if (m_currentState == AnimationState::Walking)
    {
        currentAnim = &m_walkAnim;
        currentAnimSpeed = m_walkAnimSpeed;
    }
    else if (m_currentState == AnimationState::Jumping)
    {
        currentAnim = &m_jumpAnim;
        currentAnimSpeed = m_walkAnimSpeed;
    }
    else if (m_currentState == AnimationState::Attacking)
    {
        currentAnim = &m_attackAnim;
        currentAnimSpeed = m_attackAnimSpeed;
    }

    if (currentAnim && m_animationTimer >= currentAnimSpeed)
    {
        m_animationTimer = 0.f;

        // Handle attack animation
        if (m_currentState == AnimationState::Attacking)
        {
            m_currentFrameIndex++;

            // activate hitbox in frame 3-5
            if (m_currentFrameIndex >= 3 && m_currentFrameIndex <= 5)
            {
                m_attackHitboxActive = true;
                updateAttackHitbox();
            }
            else
            {
                m_attackHitboxActive = false;
            }

            // finish attack nimation
            if (m_currentFrameIndex >= currentAnim->m_frameCount)
            {
                m_currentFrameIndex = 0;
                m_isAttacking = false;
                m_attackHitboxActive = false;
            }
        }
        else
        {
            m_currentFrameIndex = (m_currentFrameIndex + 1) % currentAnim->m_frameCount;
        }

        int col = m_currentFrameIndex % currentAnim->m_columns;
        int row = m_currentFrameIndex / currentAnim->m_columns;

        m_currentFrame.position = {col * m_currentFrameWidth, row * m_currentFrameHeight};
        m_currentFrame.size = {m_currentFrameWidth, m_currentFrameHeight};

        m_sprite.setTextureRect(m_currentFrame);
    }
}

void Player::applyPhysics(float deltaTime, const std::vector<sf::FloatRect> &groundBoxes)
{
    // Apply gravity
    m_velocity.y += m_gravity * deltaTime;

    // Simpan posisi lama
    sf::Vector2f oldPosition = m_position;

    // === GERAK HORIZONTAL ===
    m_position.x += m_velocity.x * deltaTime;
    m_sprite.setPosition(m_position);

    // Check horizontal collision
    sf::FloatRect playerBounds = getCollisionHitbox();
    for (const auto &groundBox : groundBoxes)
    {
        if (playerBounds.findIntersection(groundBox))
        {
            m_position.x = oldPosition.x;
            m_sprite.setPosition(m_position);
            break;
        }
    }

    // === GERAK VERTIKAL ===
    m_position.y += m_velocity.y * deltaTime;
    m_sprite.setPosition(m_position);

    playerBounds = getCollisionHitbox();
    m_isOnGround = false;

    for (const auto &groundBox : groundBoxes)
    {
        if (playerBounds.findIntersection(groundBox))
        {
            // Landing on ground (dari atas)
            if (m_velocity.y > 0)
            {
                // Gunakan .position.y dan .size.y
                float playerBottom = playerBounds.position.y + playerBounds.size.y;
                float groundTop = groundBox.position.y;

                // Cek posisi lama (penting!)
                float oldPlayerBottom = oldPosition.y + m_collisionBox.position.y + m_collisionBox.size.y;

                // Cek jika frame sebelumnya DI ATAS ground
                if (playerBottom > groundTop && oldPlayerBottom <= groundTop + 1.f)
                {
                    // Setel posisi player TEPAT di atas ground
                    m_position.y = groundTop - m_collisionBox.position.y - m_collisionBox.size.y;

                    m_velocity.y = 0.f;
                    m_isOnGround = true;
                    m_isJumping = false;
                }
            }
            // Hitting ceiling (dari bawah)
            else if (m_velocity.y < 0)
            {
                float playerTop = playerBounds.position.y;
                float groundBottom = groundBox.position.y + groundBox.size.y;
                float oldPlayerTop = oldPosition.y + m_collisionBox.position.y;

                if (playerTop < groundBottom && oldPlayerTop >= groundBottom - 1.f)
                {
                    // Setel posisi player TEPAT di bawah ceiling
                    m_position.y = groundBottom - m_collisionBox.position.y;
                    m_velocity.y = 0.f;
                }
            }
            m_sprite.setPosition(m_position);
        }
    }
}

void Player::update(float deltaTime, const std::vector<sf::FloatRect> &groundBoxes)
{
    // Update cooldown timer
    if (m_attackCooldownTimer > 0.f)
    {
        m_attackCooldownTimer -= deltaTime;
    }

    applyPhysics(deltaTime, groundBoxes);
    updateAnimation(deltaTime);
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(m_sprite);
}

bool Player::isFacingRight()
{
    return m_isFacingRight;
}

sf::Vector2f Player::getPosition() const
{
    return m_position;
}

sf::FloatRect Player::getCollisionHitbox() const
{
    // Ambil kotak lokal
    sf::FloatRect globalBox = m_collisionBox;

    // Geser posisinya ke posisi global player (yang merupakan origin)
    globalBox.position.x += m_position.x;
    globalBox.position.y += m_position.y;

    return globalBox;
}

void Player::drawAttackHitbox(sf::RenderWindow &window)
{
    if (m_attackHitboxActive)
    {
        sf::RectangleShape debugBox;
        debugBox.setSize({m_attackHitbox.size.x, m_attackHitbox.size.y});
        debugBox.setPosition({m_attackHitbox.position.x, m_attackHitbox.position.y});
        debugBox.setFillColor(sf::Color(255, 0, 0, 100)); // Semi-transparent red
        debugBox.setOutlineColor(sf::Color::Red);
        debugBox.setOutlineThickness(2.f);
        window.draw(debugBox);
    }
}

sf::FloatRect Player::getAttackHitbox() const
{
    return m_attackHitbox;
}

bool Player::isAttackHitboxActive() const
{
    return m_attackHitboxActive;
}

void Player::setAttackAnimation(int columns, int rows, int frameCount)
{
    m_attackAnim = {frameCount, columns, rows};
}

void Player::setAttackSpeed(float speed)
{
    m_attackAnimSpeed = speed;
}

void Player::setAttackCooldown(float cooldown)
{
    m_attackCooldown = cooldown;
}

void Player::updateAttackHitbox()
{
    float hitboxWidth = 60.f;
    float hitboxHeight = 40.f;
    float offsetX = m_isFacingRight ? 40.f : -40.f;

    m_attackHitbox = sf::FloatRect(
        {m_position.x + offsetX - hitboxWidth / 2.f, m_position.y - hitboxHeight / 2.f},
        {hitboxWidth, hitboxHeight});
}

void Player::setIdleAnimation(int columns, int rows, int frameCount)
{
    m_idleAnim = {frameCount, columns, rows};
}

void Player::setWalkAnimation(int columns, int rows, int frameCount)
{
    m_walkAnim = {frameCount, columns, rows};
}

void Player::setJumpAnimation(int columns, int rows, int frameCount)
{
    m_jumpAnim = {frameCount, columns, rows};
}

void Player::setJumpForce(float force)
{
    m_jumpForce = -abs(force);
}

void Player::setGravity(float grav)
{
    m_gravity = grav;
}