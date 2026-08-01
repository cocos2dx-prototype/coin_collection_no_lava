//
//  Constants.h
//  CoinCollection-desktop
//
//  Created by hoangbui-fe on 1/8/26.
//

#ifndef Constants_h
#define Constants_h

#include "cocos2d.h"

namespace GameConstants
{
    constexpr float DESIGN_WIDTH = 1280.0f;
    constexpr float DESIGN_HEIGHT = 720.0f;

    constexpr float PLAYER_SIZE = 48.0f;
    constexpr float PLAYER_SPEED = 200.0f;
    constexpr float PLAYER_START_X = 80.0f;
    constexpr float PLAYER_START_Y = 80.0f;
    constexpr float WALL_THICKNESS = 4.0f;

    constexpr int COIN_COUNT = 20;
    constexpr float COIN_DIAMETER = 32.0f;
    constexpr float COIN_RADIUS = COIN_DIAMETER * 0.5f;
    constexpr float COIN_MIN_CENTER_DISTANCE = 80.0f;
    constexpr float COIN_BOUNDARY_MARGIN = 60.0f;
    constexpr int MAX_LAYOUT_ATTEMPTS_PER_COIN = 5000;
    constexpr int MAX_LAYOUT_RESTARTS = 100;

    constexpr float SCORE_X = 28.0f;
    constexpr float SCORE_Y = DESIGN_HEIGHT - 28.0f;
    constexpr float SCORE_FONT_SIZE = 30.0f;
    constexpr float SCORE_RESERVED_WIDTH = 240.0f;
    constexpr float SCORE_RESERVED_HEIGHT = 70.0f;

    constexpr float RESET_WIDTH = 120.0f;
    constexpr float RESET_HEIGHT = 40.0f;
    constexpr float RESET_MARGIN_RIGHT = 28.0f;
    constexpr float RESET_MARGIN_BOTTOM = 24.0f;
    constexpr float RESET_FONT_SIZE = 24.0f;
    constexpr float RESET_HIT_MARGIN = 8.0f;

    constexpr float WIN_FONT_SIZE = 64.0f;

    const cocos2d::Color4F BACKGROUND_COLOR(0.07f, 0.09f, 0.14f, 1.0f);
    const cocos2d::Color4F PLAYER_COLOR(0.10f, 0.42f, 0.95f, 1.0f);
    const cocos2d::Color4F PLAYER_BORDER_COLOR(0.65f, 0.82f, 1.0f, 1.0f);
    const cocos2d::Color4F COIN_COLOR(1.0f, 0.80f, 0.12f, 1.0f);
    const cocos2d::Color4F COIN_BORDER_COLOR(1.0f, 0.95f, 0.50f, 1.0f);
    const cocos2d::Color4F CONTROL_COLOR(0.20f, 0.24f, 0.32f, 1.0f);
    const cocos2d::Color4F CONTROL_BORDER_COLOR(0.86f, 0.89f, 0.95f, 1.0f);
    const cocos2d::Color4F TEXT_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
}
#endif /* Constants_h */
