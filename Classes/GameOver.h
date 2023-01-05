#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    
    // a selector callback
    void menuCloseCallback(Ref* pSender); 
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameOver);

    cocos2d::Sprite* LogoPiccopark;

    void GotoLevels(Ref * psender);



};

#endif // __MAIN_MENU_H__
