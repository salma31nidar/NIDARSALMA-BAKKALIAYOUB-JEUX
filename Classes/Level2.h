#ifndef __LEVEL2_H__
#define __LEVEL2_H__

#include "cocos2d.h"
#include<chrono>

using namespace cocos2d;
class Level2 : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(Level2);

    ////////////////////////////KeyBoard/////////////////////////////
    void update(float delta);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    // conteneur map qui contient enum KeyCode et bool (True or false)
    std::map<EventKeyboard::KeyCode, bool> keys;
    //////////////////////////////////////////////////////////////////


    void changeIsJumping();
    void changeGravity();

private:
    TMXTiledMap* map_level2;
    TMXLayer* _background2;
    Sprite* _player2;
    PhysicsBody* tilePhysics2;
    TMXLayer* water2;
    PhysicsWorld* sceneWorld2;
    PhysicsBody* player_body2;

    Rect winRect;

};

#endif // __LEVEL2_H__
