//
//  Coin.cpp
//  CoinCollection-desktop
//
//  Created by hoangbui-fe on 1/8/26.
//

#include "Coin.hpp"
#include "Constants.h"

USING_NS_CC;

Coin* Coin::create()
{
    auto* coin = new (std::nothrow) Coin();
    if (coin != nullptr && coin->init())
    {
        coin->autorelease();
        return coin;
    }

    CC_SAFE_DELETE(coin);
    return nullptr;
}

bool Coin::init()
{
    if (!Node::init())
    {
        return false;
    }

    setContentSize(Size(GameConstants::COIN_DIAMETER, GameConstants::COIN_DIAMETER));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    _visual = DrawNode::create();
    _visual->drawSolidCircle(
        Vec2::ZERO,
        GameConstants::COIN_RADIUS,
        0.0f,
        40,
        GameConstants::COIN_COLOR);
    _visual->drawCircle(
        Vec2::ZERO,
        GameConstants::COIN_RADIUS,
        0.0f,
        40,
        false,
        GameConstants::COIN_BORDER_COLOR);
    addChild(_visual);

    return true;
}

void Coin::collect()
{
    if (_collected)
    {
        return;
    }

    _collected = true;
    setVisible(false);
}

void Coin::resetCoin(const Vec2& position)
{
    _collected = false;
    setPosition(position);
    setVisible(true);
}

bool Coin::isCollected() const
{
    return _collected;
}

float Coin::getCollisionRadius() const
{
    return GameConstants::COIN_RADIUS;
}
