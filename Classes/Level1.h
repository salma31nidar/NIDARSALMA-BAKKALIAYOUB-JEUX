#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <chrono>

#define PTM_RATIO 32.0
using namespace cocos2d;

class Level1 : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();


    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Level1);

    void setViewPointCenter(cocos2d::Point position);
    void update(float delta );
    ////////////////////////////KeyBoard/////////////////////////////
     void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
     void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    //////////////////////////////////////////////////////////////////

    // conteneur map qui contient enum KeyCode et bool (True or false)
    std::map<EventKeyboard::KeyCode, bool> keys;

    float X, Y;

    Point tileCoordForPosition(Vec2 position);
    void setPlayerPosition(Vec2 position);
    void changeIsJumping();
    void changeGravity();
    //////////////////////////////////////////////////////////////////////////////


void updatePhysics(float delta);

private:

    TMXTiledMap* map_level1;
    TMXLayer *_background1;
    TMXLayer* _blockage;
    Sprite* _player;
    TMXLayer* water;
    PhysicsWorld* sceneWorld;
    int isJumping = 0;
    PhysicsBody* tilePhysics;
    PhysicsBody* player_body;
    Rect sizeEnd;
};



#endif // __MAIN_MENU_H__