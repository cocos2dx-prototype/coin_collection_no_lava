//
//  Player.cpp
//  CoinCollection-desktop
//
//  Created by hoangbui-fe on 1/8/26.
//

#include "Player.hpp"
#include "Constants.h"

USING_NS_CC;

Player* Player::create()
{
    auto* player = new (std::nothrow) Player();
    if (player != nullptr && player->init())
    {
        player->autorelease();
        return player;
    }

    CC_SAFE_DELETE(player);
    return nullptr;
}

bool Player::init()
{
    if (!Node::init())
    {
        return false;
    }

    setContentSize(Size(GameConstants::PLAYER_SIZE, GameConstants::PLAYER_SIZE));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    _visual = DrawNode::create();
    const float halfSize = GameConstants::PLAYER_SIZE * 0.5f;
    _visual->drawSolidRect(
        Vec2(-halfSize, -halfSize),
        Vec2(halfSize, halfSize),
        GameConstants::PLAYER_COLOR);
    _visual->drawRect(
        Vec2(-halfSize, -halfSize),
        Vec2(halfSize, halfSize),
        GameConstants::PLAYER_BORDER_COLOR);
    addChild(_visual);

    registerKeyboardInput();
    resetPosition();
    return true;
}

void Player::registerKeyboardInput()
{
    auto* keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void Player::resetPosition()
{
    _moveUp = false;
    _moveDown = false;
    _moveLeft = false;
    _moveRight = false;
    setPosition(GameConstants::PLAYER_START_X, GameConstants::PLAYER_START_Y);
}

void Player::updateMove(float deltaTime)
{
    Vec2 direction = Vec2::ZERO;

    if (_moveLeft)
    {
        direction.x -= 1.0f;
    }
    if (_moveRight)
    {
        direction.x += 1.0f;
    }
    if (_moveUp)
    {
        direction.y += 1.0f;
    }
    if (_moveDown)
    {
        direction.y -= 1.0f;
    }

    if (direction.isZero())
    {
        return;
    }

    direction.normalize();

    Vec2 nextPosition = getPosition() +
        direction * GameConstants::PLAYER_SPEED * deltaTime;

    const float halfSize = GameConstants::PLAYER_SIZE * 0.5f;
    nextPosition.x = clampf(
        nextPosition.x,
        halfSize,
        GameConstants::DESIGN_WIDTH - halfSize);
    nextPosition.y = clampf(
        nextPosition.y,
        halfSize,
        GameConstants::DESIGN_HEIGHT - halfSize);

    setPosition(nextPosition);
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_W:
            _moveUp = true;
            break;
        case EventKeyboard::KeyCode::KEY_S:
            _moveDown = true;
            break;
        case EventKeyboard::KeyCode::KEY_A:
            _moveLeft = true;
            break;
        case EventKeyboard::KeyCode::KEY_D:
            _moveRight = true;
            break;
        default:
            break;
    }
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_W:
            _moveUp = false;
            break;
        case EventKeyboard::KeyCode::KEY_S:
            _moveDown = false;
            break;
        case EventKeyboard::KeyCode::KEY_A:
            _moveLeft = false;
            break;
        case EventKeyboard::KeyCode::KEY_D:
            _moveRight = false;
            break;
        default:
            break;
    }
}

Rect Player::getCollisionBounds() const
{
    const float halfSize = GameConstants::PLAYER_SIZE * 0.5f;
    const Vec2 position = getPosition();
    return Rect(
        position.x - halfSize,
        position.y - halfSize,
        GameConstants::PLAYER_SIZE,
        GameConstants::PLAYER_SIZE);
}
