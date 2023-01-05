#include "Level1.h"
#include "Definition.h"
#include "MainMenu.h"
#include <map>
#include "GameOver.h"
#include "win.h"

USING_NS_CC;
using namespace std;

float scale = -1;
int isJumping = 0;


Scene* Level1::createScene()
{
    return Level1::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Level1::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

//initiallisation de Physics
    initWithPhysics();
//recupere physicsWorld
    sceneWorld = getPhysicsWorld();
//ajouter un vecteur Graviter au physicWorld
    getPhysicsWorld()->setGravity(Vec2(0,-98));
//DEBUGDRAW_ALL pour afficher les corps physique , liaison , collision
    sceneWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

//ajouter Tile Map avec TMXTileMap class
    map_level1 = TMXTiledMap::create("Level1test.tmx");

    float XX = map_level1->getPosition().x;
    float YY = map_level1->getPosition().y;

    _background1 = map_level1->getLayer("Level1");
    addChild(map_level1);

    water = map_level1->getLayer("waterCollision");

    auto backgroundForLevels = Sprite::create("BackGrounfForLevels.jpg");

    float rX = map_level1->getContentSize().width;
    float rY = map_level1->getContentSize().height;

    backgroundForLevels->setScaleX(rX);
    backgroundForLevels->setScaleY(rY);

    this->addChild(backgroundForLevels, -1);


//importer le point de Spawn de joueur de tile map au cocos2d avec la fonction getObjectGroup 
    TMXObjectGroup* objects = map_level1->getObjectGroup("Object-player");
    auto playerShowUpPoint = objects->getObject("PlayerShowUpPoint");
    auto EndOfLevel1 = objects->getObject("Level1End");

//importer les coordonnés de l'objet "PlayerShowUpPoint" du fichier "Level1test.tmx" par la methode getObject() de la class TMXObjectGroup
    float x = playerShowUpPoint["x"].asFloat();
    float y = playerShowUpPoint["y"].asFloat();
    ///////////////////////////////////////////////////////////
    float width_end = EndOfLevel1["width"].asFloat();
    float height_end = EndOfLevel1["height"].asFloat();
    float x_end = EndOfLevel1["x"].asFloat();
    float y_end = EndOfLevel1["y"].asFloat();

    sizeEnd = Rect(x_end , y_end , width_end , height_end);
    /////////////////////////////////////////////////////

//ajouter le joueur au Tile Map et le positionné au coordonné de l'objet "PlayerShowUpPoint"
     _player = Sprite::create("Picopark player.png");

//ajouter un physic corps dynamique et affecter par la graviter pour le "player" 
    player_body = PhysicsBody::createBox(_player->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.7f));
    player_body->setDynamic(true);
    player_body->setRotationEnable(false);                //player ne peut pas faire un mouvement de rotation
    player_body->addMass(10);
    _player->setPosition(x , y );
    _player->setPhysicsBody(player_body); 
    this->addChild(_player);

    float limit = player_body->getVelocityLimit();

//ajouter un corp physique (Box) pur chaque Tile  
    for (int x = 0; x < 70; x++)       //width of map
    {
        for (int y = 0; y < 32; y++)   //height of map
        {
            auto spriteTile = _background1->getTileAt(Vec2(x, y));
            if (spriteTile != NULL)
            {
                tilePhysics = PhysicsBody::createBox(Size(16, 16), PhysicsMaterial(0.0f , 0.0f , 2.0f));
                tilePhysics->setDynamic(false); //static is good enough for wall
                tilePhysics->setGravityEnable(false);
                tilePhysics->addMass(100);
                spriteTile->setPhysicsBody(tilePhysics);
            }
        }
    }
// ajouter un corp physique (box) pour claque water
    for (int x = 0; x < 70; x++)       //width of map
    {
        for (int y = 0; y < 32; y++)   //height of map
        {
            auto spriteTile = water->getTileAt(Vec2(x, y));
            if (spriteTile != NULL)
            {
                tilePhysics = PhysicsBody::createBox(Size(16, 16), PhysicsMaterial(0.0f, 0.0f, 2.0f));
                tilePhysics->setDynamic(false); //static is good enough for wall
                tilePhysics->setGravityEnable(false);
                tilePhysics->addMass(100);
                spriteTile->setPhysicsBody(tilePhysics);
            }
        }
    }

    /////////////////////////////////////////KEYBOARD IMPLEMENTATION///////////////////////////////////

    auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = CC_CALLBACK_2(Level1::onKeyPressed, this);
    eventListener->onKeyReleased = CC_CALLBACK_2(Level1::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    this->scheduleUpdate(); 
    //////////////////////////////////////////////////////////////////////////////////////////////////
    return true;
}

void Level1::onKeyPressed(EventKeyboard::KeyCode keyCode , Event *event)
{

    keys[keyCode] = true;
        
}

void Level1::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{

    keys[keyCode] = false;

}

/*le joueur peut sauter sauf si il en collision avec "ground" pour sa le joueur saute si la variable
    isJumping =0, donc le joueur saute car isJumping=0 puis isJumping = 1 pour ne pas laisser le joueur de sauter tanq'il est en ciel
    et apres un Delay de 1sec is Jumping =0 puis le joueur peut sauter une 2eme fois (1sec est le temps pour que le joueur saute et atteris)*/

// la methode update de la class levvel1 se fait appeler a chaque frame du jeu
void Level1::update(float dt)
{
    float translate = 100.0 * dt;

    if (keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW] == true ||
        keys[EventKeyboard::KeyCode::KEY_A] == true)
    {
        _player->setScaleX(scale);                        //inverser playerSprite 
        _player->getPhysicsBody()->setVelocityLimit(70);
        _player->getPhysicsBody()->applyImpulse(Vec2(-145, 0));

    }
    else 
    if(keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] == true ||
       keys[EventKeyboard::KeyCode::KEY_D] == true)
    {
        _player->setScaleX(scale *-1);                      //inverser playerSprite 
        _player->getPhysicsBody()->setVelocityLimit(70);
        _player->getPhysicsBody()->applyImpulse(Vec2(145, 0));
    }

    else 
    if(keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW] == true ||
       keys[EventKeyboard::KeyCode::KEY_S] == true)
    {
        _player->getPhysicsBody()->applyImpulse( Vec2( 0 , -10) );
    }
    else
    if (keys[EventKeyboard::KeyCode::KEY_UP_ARROW] == true ||
        keys[EventKeyboard::KeyCode::KEY_Z] == true        ||
        keys[EventKeyboard::KeyCode::KEY_SPACE] == true)
    {
        //edit : augmenter la graviter apres que le "Player" saute  
        if (isJumping == 0)
        {
            /*Augmenter la graviter apres que le "Player" saute pour annuler l'effet de flotemment apres qu'il bouge a droite ou a gauche.
              Ce effet ce produit car le vecteur gravité plus le vecteur mouvement egale presque un vecteur horyzontale ce qui donne ce effet*/
            _player->getPhysicsBody()->setVelocityLimit(70);
             _player->getPhysicsBody()->applyImpulse(Vec2(0, 3500));

            auto dalayJump = DelayTime::create(0.5);
            auto funcCallback1 = CallFunc::create([&]() {Level1::changeGravity(); });
            this->runAction(Sequence::create(dalayJump, funcCallback1, NULL));  //la classe sequence permet de faire des appel en sequence

            isJumping = 1;
            

            auto delayAction = DelayTime::create(1.0);     //attendre 1sec et après faire l'appel achangeIsJumping
            auto funcCallback = CallFunc::create([&]() {Level1::changeIsJumping(); });
            this->runAction(Sequence::create(delayAction, funcCallback, NULL));

            //avec ces parametre le joueur peut sauter 2 blocs.
        }
    }
    //Pour suicre le joueur selon un 'boundary' = mapsize
    Rect mapsize = map_level1->getBoundingBox();
    auto followplayer = Follow::create(_player , mapsize);
    this->runAction(followplayer);

// collision de water et player et si intersectsRect() est true cad le player intersect avec water en remplace la scene avec GameOver Scene

    for (int x = 0; x < 70; x++)       //width of map
    {
        for (int y = 0; y < 32; y++)   //height of map
        {
            auto spriteTileWater = water->getTileAt(Vec2(x, y));
            if (spriteTileWater != NULL)
            {
                //si BoundingBox de player a intersecte BoundingBox de Layer Water en remplace la scene Level1 par GameOver Scene
                if (spriteTileWater->getBoundingBox().intersectsRect(_player->getBoundingBox())) {
                    auto scene = GameOver::create();
                    Director::getInstance()->replaceScene(scene);
                }
            }
            if (_player->getBoundingBox().intersectsRect(sizeEnd)) {
                auto WinScene1 = Win::create();
                Director::getInstance()->replaceScene(WinScene1);
            }
        }
    }
}

void Level1::changeIsJumping()
{
    isJumping = 0;
    getPhysicsWorld()->setGravity( Vec2(0 , -98) );
}
 
void Level1::changeGravity()
{
    getPhysicsWorld()->setGravity(Vec2(0 , -20000));
}