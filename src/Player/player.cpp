#include "Player.hpp"

Player::Player(const std::string &idleTexturePath,
               const std::string &walkTexturePath,
               const std::string &jumpTexturePath,
               float positionX = 100.f,
               float positionY = 100.f)
    : sprite(idleTexture),
      speed(120.f),
      jumpForce(-350.f),
      gravity(900.f),
      position(positionX, positionY),
      velocity(0.f, 0.f),
      isOnGround(false),
      isJumping(false),
      m_isFacingRight(true),
      currentState(AnimationState::Idle),
      previousState(AnimationState::Idle),
      currentFrameIndex(0),
      animationTimer(0.f),
      idleAnimSpeed(0.1f),
      walkAnimSpeed(0.1f)
{
    // Load textures
    if (!idleTexture.loadFromFile(idleTexturePath))
    {
        std::cerr << "Error loading idle texture!" << std::endl;
    }

    if (!walkTexture.loadFromFile(walkTexturePath))
    {
        std::cerr << "Error loading walk texture!" << std::endl;
    }

    if (!jumpTexture.loadFromFile(jumpTexturePath))
    {
        std::cerr << "Error loading jump texture!" << std::endl;
    }

    // Konfigurasi animasi
    idleAnim.columns = 2;
    idleAnim.rows = 4;
    idleAnim.frameCount = 8;

    walkAnim.columns = 2;
    walkAnim.rows = 4;
    walkAnim.frameCount = 8;

    jumpAnim.columns = 2;
    jumpAnim.rows = 4;
    jumpAnim.frameCount = 8;

    // Setup frame awal
    currentFrameWidth = static_cast<int>(idleTexture.getSize().x) / idleAnim.columns;
    currentFrameHeight = static_cast<int>(idleTexture.getSize().y) / idleAnim.rows;

    currentFrame = sf::IntRect({0, 0}, {currentFrameWidth, currentFrameHeight});
    sprite.setTextureRect(currentFrame);
    sprite.setOrigin({currentFrameWidth / 2.f, currentFrameHeight / 2.f});
    sprite.setPosition(position);

    // Definisikan hitbox kustom kita.
    float collisionWidth = 32.f;
    float collisionHeight = 58.f;

    // Ini adalah 'local' bounds, relatif terhadap origin (tengah sprite)
    m_collisionBox = {
        {-(collisionWidth / 2.f), -(collisionHeight / 2.f)}, // Posisi {left, top}
        {collisionWidth, collisionHeight}                    // Ukuran {width, height}
    };

    // Jika ingin hitbox-nya di BAWAH (untuk platformer):
    // m_collisionBox = {
    //    {-(collisionWidth / 2.f), (currentFrameHeight / 2.f) - collisionHeight},
    //    {collisionWidth, collisionHeight}
    // };
}

void Player::handleInput()
{
    // Horizontal movement
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        velocity.x = -speed;
        m_isFacingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        velocity.x = speed;
        m_isFacingRight = true;
    }

    // Update scale untuk flip sprite
    if (m_isFacingRight)
    {
        sprite.setScale({1.f, 1.f}); // Normal
    }
    else
    {
        sprite.setScale({-1.f, 1.f}); // Flip horizontal
    }

    // Jump
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) &&
        isOnGround && !isJumping)
    {
        velocity.y = jumpForce;
        isJumping = true;
        isOnGround = false;
    }

    // Update state
    previousState = currentState;
    if (!isOnGround)
    {
        currentState = AnimationState::Jumping;
    }
    else if (velocity.x != 0)
    {
        currentState = AnimationState::Walking;
    }
    else
    {
        currentState = AnimationState::Idle;
    }
}

void Player::updateAnimation(float deltaTime)
{
    // Ganti texture jika state berubah
    if (currentState != previousState)
    {
        currentFrameIndex = 0;
        animationTimer = 0.f;

        if (currentState == AnimationState::Idle)
        {
            sprite.setTexture(idleTexture);
            currentFrameWidth = static_cast<int>(idleTexture.getSize().x) / idleAnim.columns;
            currentFrameHeight = static_cast<int>(idleTexture.getSize().y) / idleAnim.rows;
        }
        else if (currentState == AnimationState::Walking)
        {
            sprite.setTexture(walkTexture);
            currentFrameWidth = static_cast<int>(walkTexture.getSize().x) / walkAnim.columns;
            currentFrameHeight = static_cast<int>(walkTexture.getSize().y) / walkAnim.rows;
        }
        else if (currentState == AnimationState::Jumping)
        {
            sprite.setTexture(jumpTexture);
            currentFrameWidth = static_cast<int>(jumpTexture.getSize().x) / jumpAnim.columns;
            currentFrameHeight = static_cast<int>(jumpTexture.getSize().y) / jumpAnim.rows;
        }

        sprite.setOrigin({currentFrameWidth / 2.f, currentFrameHeight / 2.f});
    }

    // REPLACE bagian "Skip animasi jika sedang jumping" dan seterusnya dengan:
    animationTimer += deltaTime;

    AnimationConfig *currentAnim = nullptr;
    float currentAnimSpeed = 0.f;

    if (currentState == AnimationState::Idle)
    {
        currentAnim = &idleAnim;
        currentAnimSpeed = idleAnimSpeed;
    }
    else if (currentState == AnimationState::Walking)
    {
        currentAnim = &walkAnim;
        currentAnimSpeed = walkAnimSpeed;
    }
    else if (currentState == AnimationState::Jumping)
    {
        currentAnim = &jumpAnim;
        currentAnimSpeed = walkAnimSpeed; // Atau buat jumpAnimSpeed sendiri
    }

    if (currentAnim && animationTimer >= currentAnimSpeed)
    {
        animationTimer = 0.f;
        currentFrameIndex = (currentFrameIndex + 1) % currentAnim->frameCount;

        int col = currentFrameIndex % currentAnim->columns;
        int row = currentFrameIndex / currentAnim->columns;

        currentFrame.position = {col * currentFrameWidth, row * currentFrameHeight};
        currentFrame.size = {currentFrameWidth, currentFrameHeight};

        sprite.setTextureRect(currentFrame);
    }
}

void Player::applyPhysics(float deltaTime, const std::vector<sf::FloatRect> &groundBoxes)
{
    // Apply gravity
    velocity.y += gravity * deltaTime;

    // Simpan posisi lama
    sf::Vector2f oldPosition = position;

    // === GERAK HORIZONTAL ===
    position.x += velocity.x * deltaTime;
    sprite.setPosition(position);

    // Check horizontal collision
    sf::FloatRect playerBounds = getCollisionHitbox();
    for (const auto &groundBox : groundBoxes)
    {
        // Kode 'if' Anda sudah benar!
        if (playerBounds.findIntersection(groundBox))
        {
            position.x = oldPosition.x;
            sprite.setPosition(position);
            break;
        }
    }

    // === GERAK VERTIKAL ===
    position.y += velocity.y * deltaTime;
    sprite.setPosition(position);

    playerBounds = getCollisionHitbox();
    isOnGround = false;

    for (const auto &groundBox : groundBoxes)
    {
        if (playerBounds.findIntersection(groundBox))
        {
            // Landing on ground (dari atas)
            if (velocity.y > 0)
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
                    position.y = groundTop - m_collisionBox.position.y - m_collisionBox.size.y;

                    velocity.y = 0.f;
                    isOnGround = true;
                    isJumping = false;
                }
            }
            // Hitting ceiling (dari bawah)
            else if (velocity.y < 0)
            {
                float playerTop = playerBounds.position.y;
                float groundBottom = groundBox.position.y + groundBox.size.y;
                float oldPlayerTop = oldPosition.y + m_collisionBox.position.y;

                if (playerTop < groundBottom && oldPlayerTop >= groundBottom - 1.f)
                {
                    // Setel posisi player TEPAT di bawah ceiling
                    position.y = groundBottom - m_collisionBox.position.y;
                    velocity.y = 0.f;
                }
            }
            sprite.setPosition(position);
        }
    }
}

void Player::update(float deltaTime, const std::vector<sf::FloatRect> &groundBoxes)
{
    applyPhysics(deltaTime, groundBoxes);
    updateAnimation(deltaTime);
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

bool Player::isFacingRight()
{
    return m_isFacingRight;
}

sf::Vector2f Player::getPosition() const
{
    return position;
}

sf::FloatRect Player::getCollisionHitbox() const
{
    // Ambil kotak lokal
    sf::FloatRect globalBox = m_collisionBox;

    // Geser posisinya ke posisi global player (yang merupakan origin)
    globalBox.position.x += position.x;
    globalBox.position.y += position.y;

    return globalBox;
}

void Player::setIdleAnimation(int columns, int rows, int frameCount)
{
    idleAnim = {frameCount, columns, rows};
}

void Player::setWalkAnimation(int columns, int rows, int frameCount)
{
    walkAnim = {frameCount, columns, rows};
}

void Player::setJumpAnimation(int columns, int rows, int frameCount)
{
    jumpAnim = {frameCount, columns, rows};
}

void Player::setJumpForce(float force)
{
    jumpForce = -abs(force);
}

void Player::setGravity(float grav)
{
    gravity = grav;
}