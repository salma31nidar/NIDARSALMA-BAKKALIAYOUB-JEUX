 #include "LevelsMenu.h"
#include "MainMenu.h"
#include "Definition.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "AudioEngine.h"

USING_NS_CC;
using namespace ui;

Scene* LevelsMenu::createScene()
{
    return LevelsMenu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool LevelsMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
  
    
    auto backgroundtest = Sprite::create("Backgroung.png");
    backgroundtest->setPosition(Point((visibleSize.width / 2) + origin.x, (visibleSize.height / 2) + origin.y));
    this->addChild(backgroundtest);

//calcule de factor pour mettre le background selon la taille de l'ecran
    float rX = visibleSize.width / backgroundtest->getContentSize().width;
    float rY = visibleSize.height / backgroundtest->getContentSize().height;

    backgroundtest->setScaleX(rX);
    backgroundtest->setScaleY(rY);

    LogoPiccopark2 = Sprite::create("Piccopark Logo.png");
    LogoPiccopark2->setPosition(Vec2(235, 220));
    this->addChild(LogoPiccopark2);
 
//Ajouter et positionnement de Button Level 1 et 2 et 3 et le button de retour

    auto menu_item_Level1 = MenuItemImage::create("Level1 button .png", "Level1 button .png", CC_CALLBACK_1(LevelsMenu::GoToLevel1, this));
    auto menu_item_Level2 = MenuItemImage::create("Level2 button.png", "Level2 button.png", CC_CALLBACK_1(LevelsMenu::GoToLevel2, this));
    auto menu_item_Level3 = MenuItemImage::create("Level3 button.png", "Level3 button.png", CC_CALLBACK_1(LevelsMenu::GoToLevel3, this));
    auto Exit_button = MenuItemImage::create("Back button.png", "Back button.png", CC_CALLBACK_1(LevelsMenu::GoBackToMainMenu, this));

    menu_item_Level1->setPosition(Vec2(85, 165));
    menu_item_Level2->setPosition(Vec2(245, 165));
    menu_item_Level3->setPosition(Vec2(400, 165));
    Exit_button->setPosition(Vec2(240, 100));

    auto* menu1 = Menu::create(menu_item_Level1, menu_item_Level2, menu_item_Level3, Exit_button, NULL);
    menu1->setPosition(Point(0, 0));
    this->addChild(menu1);
    
    return true;
}

//La methode pour remplacer la scenne par la scenne de Level1 et Level2 et Level3
void LevelsMenu::GoBackToMainMenu(Ref *pSender)
{
    auto scene1 = MainMenu::create();

    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME,scene1));
}
void LevelsMenu::GoToLevel1(Ref *pSender)
{
    auto sceneLevel1 = Level1::create();

    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, sceneLevel1));
}
void LevelsMenu::GoToLevel2(Ref* pSender)
{
    auto sceneLevel2 = Level2::create();

    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, sceneLevel2));
}
void LevelsMenu::GoToLevel3(Ref* pSender)
{
    auto sceneLevel3 = Level3::create();

    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, sceneLevel3));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////