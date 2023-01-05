#ifndef __LEVEL3_H__
#define __LEVEL3_H__

#include "cocos2d.h"
#include<chrono>

using namespace cocos2d;

class Level3 : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
  
    // implement the "static create()" method manually
    CREATE_FUNC(Level3);

    void update(float delta);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    // conteneur map qui contient enum KeyCode et bool (True or false)
    std::map<EventKeyboard::KeyCode, bool> keys;

    void changeIsJumping();
    void changeGravity();

    float x_ToMove;
    float y_ToMove;

    float x_monster;
    float y_monster;

private:

    TMXTiledMap* map_level3;

    TMXLayer* _background3;
    TMXLayer* cle;
    Sprite* player3;
    PhysicsBody* PlayerBody2;
    PhysicsBody* tilePhysics3;
    ValueMap KeyToWin;
    Sprite* Key;
    Rect winRect3;
    Sprite* monster;
    PhysicsBody* MonsterBody;
};

#endif // __MAIN_MENU_H__
