//
//  Player.hpp
//  CoinCollection-desktop
//
//  Created by hoangbui-fe on 1/8/26.
//

#ifndef Player_hpp
#define Player_hpp

#include "cocos2d.h"

class Player final : public cocos2d::Node
{
public:
    static Player* create();

    bool init() override;
    void resetPosition();
    void updateMove(float deltaTime);
    cocos2d::Rect getCollisionBounds() const;

private:
    void registerKeyboardInput();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    cocos2d::DrawNode* _visual = nullptr;
    bool _moveUp = false;
    bool _moveDown = false;
    bool _moveLeft = false;
    bool _moveRight = false;
};


#endif /* Player_hpp */
