#include "MainMenu.h"
#include "LevelsMenu.h"
#include "AudioEngine.h"
#include "Definition.h"



USING_NS_CC;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainMenu::init()
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
    auto background = Sprite::create("backgroung.png");

    //Positionnement du Background au centre
    background->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height / 2) + origin.y));
    this->addChild(background);

    //calcule de factor pour mettre le background selon la taille de l'ecran
    float rX = visibleSize.width / background->getContentSize().width;
    float rY = visibleSize.height / background->getContentSize().height;

    background->setScaleX(rX);
    background->setScaleY(rY);

    //Ajouter a sprite (Logo of the Game)
    LogoPiccopark = Sprite::create("Piccopark Logo.png");
    LogoPiccopark->setPosition(Vec2(235, 220));
    this->addChild(LogoPiccopark);

    //adding a menu
    auto menu_item_1 = MenuItemImage::create("Start button.png", "Start button.png", CC_CALLBACK_1(MainMenu::GotToLevelsMenu, this));
    auto menu_item_2 = MenuItemImage::create("Exit Botton.png", "Exit Botton.png", CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

    //Positionement du menu Start et Exit
    menu_item_1->setPosition(Vec2(235, 165));
    menu_item_2->setPosition(Vec2(235, 135));

    auto* menu = Menu::create(menu_item_1, menu_item_2, NULL);
    menu->setPosition(Point(0, 0));
    this->addChild(menu);


    return true;
}

//la methode pour replacer la scenne de debut par la deuxieme scenne

void MainMenu::GotToLevelsMenu(cocos2d::Ref* pSender)
{
    auto scene = LevelsMenu::create();
    //replaccer la 1er scenne par un effet 
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME , scene));
}
//la methode pour Sortir du jeu  

void MainMenu::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

