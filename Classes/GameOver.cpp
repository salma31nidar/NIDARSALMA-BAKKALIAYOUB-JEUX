#include "GameOver.h"
#include "LevelsMenu.h"
#include "AudioEngine.h"
#include "Definition.h"
#include "MainMenu.h"


USING_NS_CC;

Scene* GameOver::createScene()
{
    return GameOver::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


  
    //Ajouter un background
    auto background = Sprite::create("background4.jpg");

    //Positionnement du Background au centre
    background->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height / 2) + origin.y));
    this->addChild(background);

    //calcule de factor pour mettre le background selon la taille de l'ecran
    float rX = visibleSize.width / background->getContentSize().width;
    float rY = visibleSize.height / background->getContentSize().height;

    background->setScaleX(rX);
    background->setScaleY(rY);
    
    auto gameOver = Sprite::create("gameover3.png");
    gameOver->setPosition(Vec2(235, 220));
    gameOver->setScale(2.3);
    this->addChild(gameOver);

    auto restart = MenuItemImage::create("restart1.png" , "restart1.png" , CC_CALLBACK_1(GameOver::GotoLevels, this));
    auto Exit = MenuItemImage::create("exit.png", "exit.png", CC_CALLBACK_1(GameOver::menuCloseCallback, this));

    restart->setPosition(Vec2(150 , 140));
    restart->setScale(1,3);
    Exit->setPosition(Vec2(320 , 140));
    Exit->setScale(1, 3);


    auto menu = Menu::create(restart, Exit, NULL);
    menu->setPosition(Point(0, 0));
    this->addChild(menu);

    return true;
}

void GameOver::GotoLevels(Ref* pSender)
{
    auto LevelMenuScene = LevelsMenu::create();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME , LevelMenuScene));
}

void GameOver::menuCloseCallback(cocos2d::Ref* pSender)
{
    auto scene = MainMenu::create();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
