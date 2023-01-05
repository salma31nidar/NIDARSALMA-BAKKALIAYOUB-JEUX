#include "Level3.h"
#include "Definition.h"
#include "CCEventKeyboard.h"
#include <map>
#include "win.h"
#include "GameOver.h"

USING_NS_CC;

int isJumping3 = 0;
int isWin = 0;


Scene* Level3::createScene()
{
    return Level3::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Level3::init()
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
    //ajouter un vecteur Graviter au physicWorld
    getPhysicsWorld()->setGravity(Vec2(0, -98));
    //DEBUGDRAW_ALL pour afficher les corps physique , liaison , collision
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);


    //ajouter Tiled map 
    map_level3 = TMXTiledMap::create("tile3.tmx");
    _background3 = map_level3->getLayer("ground3");
    cle = map_level3->getLayer("cle");
    this->addChild(map_level3);
    //ajouter background avec un z-index = -1
    auto backgroundForLevels = Sprite::create("BackGrounfForLevels.jpg");

    float rX = map_level3->getContentSize().width;
    float rY = map_level3->getContentSize().height;

    backgroundForLevels->setScaleX(rX);
    backgroundForLevels->setScaleY(rY);

    this->addChild(backgroundForLevels, -1);

    // recuperer x et y de l'objet player 
    TMXObjectGroup* objects = map_level3->getObjectGroup("PlayerObject");
    auto playerShowUpPoint = objects->getObject("PlayerSpawnPoint");
    KeyToWin = objects->getObject("KeyToWin");
    auto Win = objects->getObject("Win3");
    auto monsterSpawnPoint = objects->getObject("SpawnMonster");
    auto PointToMove = objects->getObject("PointTomove");

    x_ToMove = PointToMove["x"].asFloat();
    y_ToMove = PointToMove["y"].asFloat();

    x_monster = monsterSpawnPoint["x"].asFloat();
    y_monster = monsterSpawnPoint["y"].asFloat();

    float x1 = playerShowUpPoint["x"].asFloat();
    float y1 = playerShowUpPoint["y"].asFloat();

    float win_x = Win["x"].asFloat();
    float win_y = Win["y"].asFloat();
    float win_width = Win["width"].asFloat();
    float win_height = Win["height"].asFloat();
    //recupere la x,y,width, height de win Rect
    winRect3 = Rect(win_x, win_y, win_width, win_height);
    //ajouter Player et le positionner au x,y de l'objet PlayerSpawnPoint
    player3 = Sprite::create("PicoPark player.png");
    player3->setPosition(Vec2(x1, y1));
    //ajouter un physic Body au sprite player
    PlayerBody2 = PhysicsBody::createBox(player3->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.7f));
    PlayerBody2->setDynamic(true);
    PlayerBody2->setRotationEnable(false);                //player ne peut pas faire un mouvement de rotation
    PlayerBody2->addMass(10);
    player3->setPosition(x1, y1);
    player3->setPhysicsBody(PlayerBody2);

    this->addChild(player3);
    //ajouter monster sprite
    monster = Sprite::create("Monster.png");
    monster->setScale(0.5);

    MonsterBody = PhysicsBody::createBox(monster->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.7f));
    MonsterBody->setDynamic(true);
    MonsterBody->setRotationEnable(false);                //Monster ne peut pas faire un mouvement de rotation
    MonsterBody->addMass(10);
    monster->setPosition(Vec2(x_monster, y_monster));
    monster->setPhysicsBody(MonsterBody);

    this->addChild(monster);

    auto actionMove1 = MoveTo::create(5.0f , Vec2(x_ToMove , y_ToMove));
    auto actionMove2 = MoveTo::create(5.0f , Vec2(x_monster ,y_monster));

    auto sequenceActions = Sequence::create(actionMove1 , actionMove2 , NULL);
    auto SequenceForEver = RepeatForever::create(sequenceActions);
    monster->runAction(SequenceForEver);


    //ajouter un physic body a chaque tile de layer "ground"
    for (int x = 0; x < 44; x++)       //width of map
    {
        for (int y = 0; y < 20; y++)   //height of map
        {
            auto spriteTile = _background3->getTileAt(Vec2(x, y));
            if (spriteTile != NULL)
            {
                tilePhysics3 = PhysicsBody::createBox(Size(16, 16), PhysicsMaterial(0.0f, 0.0f, 2.0f));
                tilePhysics3->setDynamic(false); //static is good enough for wall
                tilePhysics3->setGravityEnable(false);
                tilePhysics3->addMass(100);
                spriteTile->setPhysicsBody(tilePhysics3);
            }
        }
    }
    //ajouter un physic body a chaque tile de layer "cle"
    for (int x = 0; x < 44; x++)       //width of map
    {
        for (int y = 0; y < 20; y++)   //height of map
        {
            auto spriteTile = cle->getTileAt(Vec2(x, y));
            if (spriteTile != NULL)
            {
                tilePhysics3 = PhysicsBody::createBox(Size(16, 16), PhysicsMaterial(0.0f, 0.0f, 2.0f));
                tilePhysics3->setDynamic(false); //static is good enough for wall
                tilePhysics3->setGravityEnable(false);
                tilePhysics3->addMass(100);
                spriteTile->setPhysicsBody(tilePhysics3);
            }
        }
    }


    //implementation de Keyboard 
    auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = CC_CALLBACK_2(Level3::onKeyPressed, this);
    eventListener->onKeyReleased = CC_CALLBACK_2(Level3::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);



    this->scheduleUpdate();

    return true;
}

void Level3::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

    keys[keyCode] = true;

}

void Level3::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{

    keys[keyCode] = false;

}

void Level3::update(float dt)
{
    int scale = -1;
    float translate = 100.0 * dt;

    if (keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW] == true ||
        keys[EventKeyboard::KeyCode::KEY_A] == true)
    {
        player3->setScaleX(scale);                        //inverser playerSprite 
        player3->getPhysicsBody()->setVelocityLimit(70);
        player3->getPhysicsBody()->applyImpulse(Vec2(-145, 0));

    }
    else
        if (keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] == true ||
            keys[EventKeyboard::KeyCode::KEY_D] == true)
        {
            player3->setScaleX(scale * -1);                      //inverser playerSprite 
            player3->getPhysicsBody()->setVelocityLimit(70);
            player3->getPhysicsBody()->applyImpulse(Vec2(145, 0));
        }

        else
            if (keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW] == true ||
                keys[EventKeyboard::KeyCode::KEY_S] == true)
            {
                player3->getPhysicsBody()->applyImpulse(Vec2(0, -10));
            }
            else
                if (keys[EventKeyboard::KeyCode::KEY_UP_ARROW] == true ||
                    keys[EventKeyboard::KeyCode::KEY_Z] == true ||
                    keys[EventKeyboard::KeyCode::KEY_SPACE] == true)
                {
                    if (isJumping3 == 0)
                    {
                        /*Augmenter la graviter apres que le "Player" saute pour annuler l'effet de flotemment apres qu'il bouge a droite ou a gauche.
                          Ce effet ce produit car le vecteur gravit� plus le vecteur mouvement egale presque un vecteur horyzontale ce qui donne ce effet*/
                        player3->getPhysicsBody()->setVelocityLimit(70);
                        player3->getPhysicsBody()->applyImpulse(Vec2(0, 3500));

                        auto dalayJump = DelayTime::create(0.5);
                        auto funcCallback1 = CallFunc::create([&]() {Level3::changeGravity(); });
                        this->runAction(Sequence::create(dalayJump, funcCallback1, NULL));  //la classe sequence permet de faire des appel en sequence

                        isJumping3 = 1;


                        auto delayAction = DelayTime::create(1.0);     //attendre 1sec et apr�s faire l'appel achangeIsJumping
                        auto funcCallback = CallFunc::create([&]() {Level3::changeIsJumping(); });
                        this->runAction(Sequence::create(delayAction, funcCallback, NULL));

                        //avec ces parametre le joueur peut sauter 2 blocs.
                    }
                }

    Rect mapsize = map_level3->getBoundingBox();
    auto followplayer = Follow::create(player3, mapsize);
    this->runAction(followplayer);


    for (int x = 0; x < 44; x++)       //width of map
    {
        for (int y = 0; y < 20; y++)   //height of map
        {
            auto CleTile = cle->getTileAt(Vec2(x, y));
            if (CleTile != NULL)
            {
                if (player3->getBoundingBox().intersectsRect(CleTile->getBoundingBox())) {
                    isWin = 1;
                    CleTile->setOpacity(0);
                }
            }
        }
    }

    for (int x = 0; x < 44; x++)       //width of map
    {
        for (int y = 0; y < 20; y++)   //height of map
        {
            auto CleTile = cle->getTileAt(Vec2(x, y));
            if (CleTile != NULL)
            {
                if (player3->getBoundingBox().intersectsRect(winRect3)) {
                    if (isWin == 1){
                        auto WinScene3 = Win::create();
                        Director::getInstance()->replaceScene(WinScene3);
                    }
                }
            }
        }


    }

    if (player3->getBoundingBox().intersectsRect(monster->getBoundingBox())) {
        auto GameOver3 = GameOver ::create();
        Director::getInstance()->replaceScene(GameOver3);
    }
}


void Level3::changeIsJumping()
{
    isJumping3 = 0;
    getPhysicsWorld()->setGravity(Vec2(0, -98));
}

void Level3::changeGravity()
{
    getPhysicsWorld()->setGravity(Vec2(0, -20000));
}

