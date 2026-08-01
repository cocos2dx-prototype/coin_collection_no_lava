//
//  Coin.hpp
//  CoinCollection-desktop
//
//  Created by hoangbui-fe on 1/8/26.
//

#ifndef Coin_hpp
#define Coin_hpp

#include "cocos2d.h"

class Coin final : public cocos2d::Node
{
public:
    static Coin* create();

    bool init() override;
    void collect();
    void resetCoin(const cocos2d::Vec2& position);
    bool isCollected() const;
    float getCollisionRadius() const;

private:
    cocos2d::DrawNode* _visual = nullptr;
    bool _collected = false;
};

#endif /* Coin_hpp */
