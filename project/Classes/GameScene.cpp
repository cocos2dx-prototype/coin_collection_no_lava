//
//  GameScene.cpp
//  CoinCollection-desktop
//
//  Created by hoangbui-fe on 1/8/26.
//

#include "GameScene.hpp"
#include "Constants.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

USING_NS_CC;

namespace
{
    std::mt19937& randomEngine()
    {
        static std::random_device randomDevice;
        static std::mt19937 engine(randomDevice());
        return engine;
    }
}

GameScene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    createBackground();
    createPlayer();
    createCoins();
    createInterface();

    return true;
}

void GameScene::onEnter() {
    Scene::onEnter();
    registerInput();
    resetRound();
    scheduleUpdate();
}

void GameScene::onExit() {
    unscheduleUpdate();
    Scene::onExit();
}

void GameScene::createBackground()
{
    auto* background = DrawNode::create();
    background->drawSolidRect(
        Vec2::ZERO,
        Vec2(GameConstants::DESIGN_WIDTH, GameConstants::DESIGN_HEIGHT),
        GameConstants::BACKGROUND_COLOR);

    const float wall = GameConstants::WALL_THICKNESS;
    background->drawSolidRect(
        Vec2::ZERO,
        Vec2(GameConstants::DESIGN_WIDTH, wall),
        GameConstants::CONTROL_BORDER_COLOR);
    background->drawSolidRect(
        Vec2(0.0f, GameConstants::DESIGN_HEIGHT - wall),
        Vec2(GameConstants::DESIGN_WIDTH, GameConstants::DESIGN_HEIGHT),
        GameConstants::CONTROL_BORDER_COLOR);
    background->drawSolidRect(
        Vec2::ZERO,
        Vec2(wall, GameConstants::DESIGN_HEIGHT),
        GameConstants::CONTROL_BORDER_COLOR);
    background->drawSolidRect(
        Vec2(GameConstants::DESIGN_WIDTH - wall, 0.0f),
        Vec2(GameConstants::DESIGN_WIDTH, GameConstants::DESIGN_HEIGHT),
        GameConstants::CONTROL_BORDER_COLOR);

    addChild(background, kChildTagBackground);
}

void GameScene::createPlayer()
{
    _player = Player::create();
    CCASSERT(_player != nullptr, "Failed to create player.");
    addChild(_player, kChildTagPlayer);
}

void GameScene::createCoins()
{
    for (Coin*& coin : _coins)
    {
        coin = Coin::create();
        CCASSERT(coin != nullptr, "Failed to create coin.");
        addChild(coin, kChildTagCoin);
    }
}

void GameScene::createInterface()
{
    _scoreLabel = Label::createWithSystemFont(
        "Score: 0 / 20",
        "Arial",
        GameConstants::SCORE_FONT_SIZE);
    _scoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _scoreLabel->setPosition(GameConstants::SCORE_X, GameConstants::SCORE_Y);
    _scoreLabel->setTextColor(Color4B::WHITE);
    addChild(_scoreLabel, kChildTagHUD);

    _winLabel = Label::createWithSystemFont(
        "YOU WIN",
        "Arial",
        GameConstants::WIN_FONT_SIZE);
    _winLabel->setPosition(
        GameConstants::DESIGN_WIDTH * 0.5f,
        GameConstants::DESIGN_HEIGHT * 0.5f);
    _winLabel->setTextColor(Color4B::WHITE);
    _winLabel->setVisible(false);
    addChild(_winLabel, kChildTagHUD);

    const float resetLeft = GameConstants::DESIGN_WIDTH -
        GameConstants::RESET_MARGIN_RIGHT - GameConstants::RESET_WIDTH;
    const float resetBottom = GameConstants::RESET_MARGIN_BOTTOM;
    _resetBounds = Rect(
        resetLeft,
        resetBottom,
        GameConstants::RESET_WIDTH,
        GameConstants::RESET_HEIGHT);

    _resetVisual = DrawNode::create();
    _resetVisual->drawSolidRect(
        Vec2(resetLeft, resetBottom),
        Vec2(resetLeft + GameConstants::RESET_WIDTH,
             resetBottom + GameConstants::RESET_HEIGHT),
        GameConstants::CONTROL_COLOR);
    _resetVisual->drawRect(
        Vec2(resetLeft, resetBottom),
        Vec2(resetLeft + GameConstants::RESET_WIDTH,
             resetBottom + GameConstants::RESET_HEIGHT),
        GameConstants::CONTROL_BORDER_COLOR);
    addChild(_resetVisual, kChildTagHUD);

    _resetLabel = Label::createWithSystemFont(
        "Reset",
        "Arial",
        GameConstants::RESET_FONT_SIZE);
    _resetLabel->setPosition(
        resetLeft + GameConstants::RESET_WIDTH * 0.5f,
        resetBottom + GameConstants::RESET_HEIGHT * 0.5f);
    _resetLabel->setTextColor(Color4B::WHITE);
    addChild(_resetLabel, kChildTagHUD);
}

void GameScene::registerInput()
{
    auto* touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameScene::resetRound()
{
    _score = 0;
    _roundComplete = false;
    _player->resetPosition();
    _winLabel->setVisible(false);
    updateScoreLabel();

    const bool layoutGenerated = generateValidCoinLayout();
    CCASSERT(layoutGenerated, "Could not generate a valid 20-coin layout.");
}

bool GameScene::generateValidCoinLayout()
{
    std::uniform_real_distribution<float> xDistribution(
        GameConstants::COIN_BOUNDARY_MARGIN,
        GameConstants::DESIGN_WIDTH - GameConstants::COIN_BOUNDARY_MARGIN);
    std::uniform_real_distribution<float> yDistribution(
        GameConstants::COIN_BOUNDARY_MARGIN,
        GameConstants::DESIGN_HEIGHT - GameConstants::COIN_BOUNDARY_MARGIN);

    for (int layoutRestart = 0;
         layoutRestart < GameConstants::MAX_LAYOUT_RESTARTS;
         ++layoutRestart)
    {
        std::vector<Vec2> acceptedPositions;
        acceptedPositions.reserve(GameConstants::COIN_COUNT);

        bool layoutFailed = false;
        for (int coinIndex = 0; coinIndex < GameConstants::COIN_COUNT; ++coinIndex)
        {
            bool positionFound = false;
            for (int attempt = 0;
                 attempt < GameConstants::MAX_LAYOUT_ATTEMPTS_PER_COIN;
                 ++attempt)
            {
                const Vec2 candidate(
                    xDistribution(randomEngine()),
                    yDistribution(randomEngine()));

                if (isValidCoinPosition(candidate, acceptedPositions))
                {
                    acceptedPositions.push_back(candidate);
                    positionFound = true;
                    break;
                }
            }

            if (!positionFound)
            {
                layoutFailed = true;
                break;
            }
        }

        if (!layoutFailed &&
            acceptedPositions.size() == GameConstants::COIN_COUNT)
        {
            for (int index = 0; index < GameConstants::COIN_COUNT; ++index)
            {
                _coins[index]->resetCoin(acceptedPositions[index]);
            }
            return true;
        }
    }

    return false;
}

bool GameScene::isValidCoinPosition(
    const Vec2& candidate,
    const std::vector<Vec2>& acceptedPositions) const
{
    const float playerHalfSize = GameConstants::PLAYER_SIZE * 0.5f;
    const Rect playerStartingArea(
        GameConstants::PLAYER_START_X - playerHalfSize - GameConstants::COIN_RADIUS,
        GameConstants::PLAYER_START_Y - playerHalfSize - GameConstants::COIN_RADIUS,
        GameConstants::PLAYER_SIZE + GameConstants::COIN_DIAMETER,
        GameConstants::PLAYER_SIZE + GameConstants::COIN_DIAMETER);
    if (playerStartingArea.containsPoint(candidate))
    {
        return false;
    }

    const Rect scoreReservedArea(
        0.0f,
        GameConstants::DESIGN_HEIGHT - GameConstants::SCORE_RESERVED_HEIGHT,
        GameConstants::SCORE_RESERVED_WIDTH,
        GameConstants::SCORE_RESERVED_HEIGHT);
    const Rect expandedScoreArea(
        scoreReservedArea.origin.x - GameConstants::COIN_RADIUS,
        scoreReservedArea.origin.y - GameConstants::COIN_RADIUS,
        scoreReservedArea.size.width + GameConstants::COIN_DIAMETER,
        scoreReservedArea.size.height + GameConstants::COIN_DIAMETER);
    if (expandedScoreArea.containsPoint(candidate))
    {
        return false;
    }

    const Rect expandedResetArea(
        _resetBounds.origin.x - GameConstants::COIN_RADIUS,
        _resetBounds.origin.y - GameConstants::COIN_RADIUS,
        _resetBounds.size.width + GameConstants::COIN_DIAMETER,
        _resetBounds.size.height + GameConstants::COIN_DIAMETER);
    if (expandedResetArea.containsPoint(candidate))
    {
        return false;
    }

    const float minimumDistanceSquared =
        GameConstants::COIN_MIN_CENTER_DISTANCE *
        GameConstants::COIN_MIN_CENTER_DISTANCE;
    for (const Vec2& acceptedPosition : acceptedPositions)
    {
        if (candidate.distanceSquared(acceptedPosition) < minimumDistanceSquared)
        {
            return false;
        }
    }

    return true;
}

void GameScene::update(float deltaTime)
{
    _player->updateMove(deltaTime);

    if (!_roundComplete)
    {
        checkCoinCollisions();
    }
}

void GameScene::checkCoinCollisions()
{
    const Rect playerBounds = _player->getCollisionBounds();

    for (Coin* coin : _coins)
    {
        if (coin->isCollected())
        {
            continue;
        }

        if (rectangleIntersectsCircle(
                playerBounds,
                coin->getPosition(),
                coin->getCollisionRadius()))
        {
            coin->collect();
            ++_score;
            updateScoreLabel();

            if (_score == GameConstants::COIN_COUNT)
            {
                completeRound();
                return;
            }
        }
    }
}

bool GameScene::rectangleIntersectsCircle(
    const Rect& rectangle,
    const Vec2& circleCenter,
    float circleRadius) const
{
    const float closestX = clampf(
        circleCenter.x,
        rectangle.getMinX(),
        rectangle.getMaxX());
    const float closestY = clampf(
        circleCenter.y,
        rectangle.getMinY(),
        rectangle.getMaxY());

    const float deltaX = circleCenter.x - closestX;
    const float deltaY = circleCenter.y - closestY;
    return deltaX * deltaX + deltaY * deltaY <= circleRadius * circleRadius;
}

void GameScene::updateScoreLabel()
{
    _scoreLabel->setString(
        StringUtils::format("Score: %d / %d", _score, GameConstants::COIN_COUNT));
}

void GameScene::completeRound()
{
    _roundComplete = true;
    _winLabel->setVisible(true);
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    const Vec2 location = touch->getLocation();
    const Rect hitBounds(
        _resetBounds.origin.x - GameConstants::RESET_HIT_MARGIN,
        _resetBounds.origin.y - GameConstants::RESET_HIT_MARGIN,
        _resetBounds.size.width + GameConstants::RESET_HIT_MARGIN * 2.0f,
        _resetBounds.size.height + GameConstants::RESET_HIT_MARGIN * 2.0f);

    if (hitBounds.containsPoint(location))
    {
        resetRound();
        return true;
    }

    return false;
}
