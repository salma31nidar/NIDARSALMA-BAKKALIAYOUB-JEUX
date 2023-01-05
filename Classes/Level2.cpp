#include "Level2.h"
#include "Definition.h"
#include <iostream>
#include "GameOver.h"
#include "win.h"

USING_NS_CC;

int isJumping1 = 0;


Scene* Level2::createScene()
{
    return Level2::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Level2::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //initiallisation de PhysicWorld
    initWithPhysics();
    sceneWorld2 = getPhysicsWorld();
    //ajouter un vecteur Graviter au physicWorld
    getPhysicsWorld()->setGravity(Vec2(0, -98));
    //DEBUGDRAW_ALL pour afficher les corps physique , liaison , collision
    sceneWorld2->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);




    map_level2 = TMXTiledMap::create("tilemap2.tmx");
    _background2 = map_level2->getLayer("ground");

    water2 = map_level2->getLayer("water2");

    addChild(map_level2);


    auto backgroundForLevels = Sprite::create("BackGrounfForLevels.jpg");

    float rX = map_level2->getContentSize().width;
    float rY = map_level2->getContentSize().height;

    backgroundForLevels->setScaleX(rX);
    backgroundForLevels->setScaleY(rY);

    this->addChild(backgroundForLevels, -1);

    TMXObjectGroup* objects = map_level2->getObjectGroup("Object-player");
    auto playerShowUpPoint = objects->getObject("PlayerShowUpPoint");
    auto winarea = objects->getObject("win");

    float win_x = winarea["x"].asFloat();
    float win_y = winarea["y"].asFloat();
    float win_width = winarea["width"].asFloat();
    float win_height = winarea["height"].asFloat();

    winRect = Rect(win_x , win_y ,win_width, win_height);



    float x = playerShowUpPoint["x"].asFloat();
    float y = playerShowUpPoint["y"].asFloat();

    _player2 = Sprite::create("PicoPark player.png");
    _player2->setPosition(Vec2(x,y));

    player_body2 = PhysicsBody::createBox(_player2->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.7f));
    player_body2->setDynamic(true);
    player_body2->setRotationEnable(false);                //player ne peut pas faire un mouvement de rotation
    player_body2->addMass(10);
    _player2->setPosition(x, y);
    _player2->setPhysicsBody(player_body2);

    this->addChild(_player2);


    //ajouter un corp physique (Box) pur chaque Tile  
    for (int x = 0; x < 60; x++)       //width of map
    {
        for (int y = 0; y < 40; y++)   //height of map
        {
            auto spriteTile = _background2->getTileAt(Vec2(x, y));
            if (spriteTile != NULL)
            {
                tilePhysics2 = PhysicsBody::createBox(Size(16, 16), PhysicsMaterial(0.0f, 0.0f, 2.0f));
                tilePhysics2->setDynamic(false); //static is good enough for wall
                tilePhysics2->setGravityEnable(false);
                tilePhysics2->addMass(100);
                spriteTile->setPhysicsBody(tilePhysics2);
            }
        }
    }
    // ajouter un corp physique (box) pour claque water
    for (int x = 0; x < 60; x++)       //width of map
    {
        for (int y = 0; y < 40; y++)   //height of map
        {
            auto spriteTile = water2->getTileAt(Vec2(x, y));
            if (spriteTile != NULL)
            {
                tilePhysics2 = PhysicsBody::createBox(Size(16, 16), PhysicsMaterial(0.0f, 0.0f, 2.0f));
                tilePhysics2->setDynamic(false); //static is good enough for wall
                tilePhysics2->setGravityEnable(false);
                tilePhysics2->addMass(100);
                spriteTile->setPhysicsBody(tilePhysics2);
            }
        }
    }

    /////////////////////////////////////////KEYBOARD IMPLEMENTATION///////////////////////////////////

    auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = CC_CALLBACK_2(Level2::onKeyPressed, this);
    eventListener->onKeyReleased = CC_CALLBACK_2(Level2::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    auto eventlistener1 = EventListenerKeyboard::create();


    this->scheduleUpdate();
    //////////////////////////////////////////////////////////////////////////////////////////////////

    return true;
}

void Level2::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

    keys[keyCode] = true;

}

void Level2::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{

    keys[keyCode] = false;

}

/*le joueur peut sauter sauf si il en collision avec "ground" pour sa le joueur saute si la variable
    isJumping =0, donc le joueur saute car isJumping=0 puis isJumping = 1 pour ne pas laisser le joueur de sauter tanq'il est en ciel
    et apres un Delay de 1sec is Jumping =0 puis le joueur peut sauter une 2eme fois (1sec est le temps pour que le joueur saute et atteris)*/

    // la methode update de la class levvel1 se fait appeler a chaque frame du jeu
void Level2::update(float dt)
{
    int scale = -1;
    float translate = 100.0 * dt;

    if (keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW] == true ||
        keys[EventKeyboard::KeyCode::KEY_A] == true)
    {
        _player2->setScaleX(scale);                        //inverser playerSprite 
        _player2->getPhysicsBody()->setVelocityLimit(70);
        _player2->getPhysicsBody()->applyImpulse(Vec2(-145, 0));

    }
    else
        if (keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] == true ||
            keys[EventKeyboard::KeyCode::KEY_D] == true)
        {
            _player2->setScaleX(scale * -1);                      //inverser playerSprite 
            _player2->getPhysicsBody()->setVelocityLimit(70);
            _player2->getPhysicsBody()->applyImpulse(Vec2(145, 0));
        }

        else
            if (keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW] == true ||
                keys[EventKeyboard::KeyCode::KEY_S] == true)
            {
                _player2->getPhysicsBody()->applyImpulse(Vec2(0, -10));
            }
            else
                if (keys[EventKeyboard::KeyCode::KEY_UP_ARROW] == true ||
                    keys[EventKeyboard::KeyCode::KEY_Z] == true ||
                    keys[EventKeyboard::KeyCode::KEY_SPACE] == true)
                {
                    if (isJumping1 == 0)
                    {
                        /*Augmenter la graviter apres que le "Player" saute pour annuler l'effet de flotemment apres qu'il bouge a droite ou a gauche.
                          Ce effet ce produit car le vecteur gravité plus le vecteur mouvement egale presque un vecteur horyzontale ce qui donne ce effet*/
                        _player2->getPhysicsBody()->setVelocityLimit(70);
                        _player2->getPhysicsBody()->applyImpulse(Vec2(0, 3500));

                        auto dalayJump = DelayTime::create(0.5);
                        auto funcCallback1 = CallFunc::create([&]() {Level2::changeGravity(); });
                        this->runAction(Sequence::create(dalayJump, funcCallback1, NULL));  //la classe sequence permet de faire des appel en sequence

                        isJumping1 = 1;


                        auto delayAction = DelayTime::create(1.0);     //attendre 1sec et après faire l'appel achangeIsJumping
                        auto funcCallback = CallFunc::create([&]() {Level2::changeIsJumping(); });
                        this->runAction(Sequence::create(delayAction, funcCallback, NULL));

                        //avec ces parametre le joueur peut sauter 2 blocs.
                    }
                }
    //Pour suicre le joueur selon un 'boundary' = mapsize
    Rect mapsize = map_level2->getBoundingBox();
    auto followplayer = Follow::create(_player2, mapsize);
    this->runAction(followplayer);


    for (int x = 0; x < 60; x++)       //width of map
    {
        for (int y = 0; y < 40; y++)   //height of map
        {
            auto spriteTileWater = water2->getTileAt(Vec2(x, y));
            if (spriteTileWater != NULL)
            {
                //si BoundingBox de player a intersecte BoundingBox de Layer Water en remplace la scene Level1 par GameOver Scene
                if (spriteTileWater->getBoundingBox().intersectsRect(_player2->getBoundingBox())) {
                    auto scene = GameOver::create();
                    Director::getInstance()->replaceScene(scene);
                }
            }
            if (_player2->getBoundingBox().intersectsRect(winRect)) {
                auto WinScene2 = Win::create();
                Director::getInstance()->replaceScene(WinScene2);
            }
        }
    }

}


void Level2::changeIsJumping()
{
    isJumping1 = 0;
    getPhysicsWorld()->setGravity(Vec2(0, -98));
}

void Level2::changeGravity()
{
    getPhysicsWorld()->setGravity(Vec2(0, -20000));
}
