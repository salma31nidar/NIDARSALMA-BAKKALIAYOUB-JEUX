#include "win.h"
#include "LevelsMenu.h"
#include"LevelsMenu.h"
#include "Definition.h"



USING_NS_CC;

Scene*Win::createScene()
{
    return Win::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Win::init()
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
    auto background = Sprite::create("back2.jpg");

    //Positionnement du Background au centre
    background->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height / 2) + origin.y));
    this->addChild(background);

    //calcule de factor pour mettre le background selon la taille de l'ecran
    float rX = visibleSize.width / background->getContentSize().width;
    float rY = visibleSize.height / background->getContentSize().height;

    background->setScaleX(rX);
    background->setScaleY(rY);
    
    auto Win = Sprite::create("win2.png");
    Win->setPosition(Vec2(235,220));
    Win->setScale(2.3);
    this->addChild(Win);


    auto Next = MenuItemImage::create("next.png", "next.png", CC_CALLBACK_1(Win::menuCloseCallback, this));


    Next->setPosition(Vec2(250,140));
    Next->setScale(1,2);

    auto menu = Menu::create(Next, NULL);
    menu->setPosition(Point(0, 0));
    this->addChild(menu);

    return true;
}

void Win::menuCloseCallback(Ref* pSender)
{
    auto levelmenu = LevelsMenu::create();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, levelmenu));
}
