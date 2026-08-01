//
//  GameScene.hpp
//  CoinCollection-desktop
//
//  Created by hoangbui-fe on 1/8/26.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include <array>
#include <vector>
#include "Coin.hpp"
#include "Player.hpp"

class GameScene final : public cocos2d::Scene
{
public:
    static GameScene* createScene();

    bool init() override;
    void update(float deltaTime) override;

    CREATE_FUNC(GameScene);

private:
    void createBackground();
    void createPlayer();
    void createCoins();
    void createInterface();
    void registerInput();

    void resetRound();
    bool generateValidCoinLayout();
    bool isValidCoinPosition(
        const cocos2d::Vec2& candidate,
        const std::vector<cocos2d::Vec2>& acceptedPositions) const;

    void checkCoinCollisions();
    bool rectangleIntersectsCircle(
        const cocos2d::Rect& rectangle,
        const cocos2d::Vec2& circleCenter,
        float circleRadius) const;
    void updateScoreLabel();
    void completeRound();

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    Player* _player = nullptr;
    std::array<Coin*, 20> _coins{};
    cocos2d::Label* _scoreLabel = nullptr;
    cocos2d::Label* _winLabel = nullptr;
    cocos2d::DrawNode* _resetVisual = nullptr;
    cocos2d::Label* _resetLabel = nullptr;
    cocos2d::Rect _resetBounds;

    int _score = 0;
    bool _roundComplete = false;
};


#endif /* GameScene_hpp */
