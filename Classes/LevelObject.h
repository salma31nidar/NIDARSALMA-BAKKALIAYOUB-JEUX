#ifndef __LEVEL_OBJECTS_H__
#define __LEVEL_OBJECTS_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

const float kPixelsPerMeter = 32.0f;
const float kGravity = -kPixelsPerMeter / 0.7f;

enum
{
    kFilterCategoryLevel = 0x01,
    kFilterCategorySolidObject = 0x02,
    kFilterCategoryNonSolidObject = 0x04
};

using namespace cocos2d;

class LevelObject : public Node
{
private:
    // It helps to typedef super & self so if you change the name
    // of the class or super class, you don't have to replace all
    // the references
    typedef Node super;
    typedef LevelObject self;

protected:
    b2Body* body;
    Sprite* sprite;

public:
    LevelObject();
    virtual ~LevelObject();

    void addBodyToWorld(b2World *world);
    void addCircularFixtureToBody(float radius);
    void addRectangularFixtureToBody(float width, float height);
    void createFixture(b2Shape *shape);

};

void LevelObject::addBodyToWorld(b2World* world)
{
    // add a dynamic body to world
    // (subclasses can use other body types by overriding
    // this method and calling body->SetType())
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(
        this->getPositionX() / kPixelsPerMeter,
        this->getPositionY() / kPixelsPerMeter
    );
    bodyDef.userData = this;
    bodyDef.fixedRotation = true;
    this->body = world->CreateBody(&bodyDef);
}

void LevelObject::addCircularFixtureToBody(float radius)
{
    b2CircleShape shape;
    shape.m_radius = radius * this->getScale();
    this->createFixture(&shape);
}

void LevelObject::addRectangularFixtureToBody(float width, float height)
{
    b2PolygonShape shape;
    shape.SetAsBox(
        width * this->getScale(),
        height * this->getScale()
    );
    this->createFixture(&shape);
}

void LevelObject::createFixture(b2Shape* shape)
{
    // note that friction, etc. can be modified later by looping
    // over the body's fixtures and calling fixture->SetFriction()
    b2FixtureDef fixtureDef;
    fixtureDef.shape = shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.7f;
    fixtureDef.restitution = 0.1f;
    fixtureDef.filter.categoryBits = kFilterCategorySolidObject;
    fixtureDef.filter.maskBits = 0xffff;
    this->body->CreateFixture(&fixtureDef);
}

#endif // __MAIN_MENU_H__