#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include <GLES-Render.h>

#define PTM_RATIO 108

class LightEffect;
class EffectSprite;

using namespace std;
using namespace cocos2d;

class HelloWorld : public cocos2d::Scene, public b2ContactListener
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	Size visibleSize;
	Vec2 origin;


	b2World* _world;
	b2ContactListener* b2listener;
	bool initBox2DWorld(bool);
	void box2dUpdate(float);
	void tick(float);

	//for impact
	int num;
	virtual void BeginContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	//virtual void PreSolve(b2Contact* contact, b2Manifold* mainFold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	//virtual void PostSolve(b2Contact* contact, b2ContactImpulse* impulse);
	virtual void EndContact(b2Contact* contact);

	b2Body* addNewSprite(Vec2 pos, Size size, b2BodyType bodyType, Sprite* sprite, int type, float restitution);
	b2Body* addNewSprite(Vec2 pos, Size size, b2BodyType bodyType, Sprite* sprite, int type, float restitution, const string fname);

	//for debug
	GLESDebugDraw* m_debugDraw;
	CustomCommand _customCmd;
	virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;
	void onDraw(const Mat4& transform, uint32_t flags);

	//light sprite
	cocos2d::Vec3 _lightPos;
	LightEffect *_effect;
	cocos2d::Sprite *_lightSprite;
};

#endif // __HELLOWORLD_SCENE_H__
