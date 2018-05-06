#include "HelloWorldScene.h"
//#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"
#include "EffectSprite\LightEffect.h"
#include "EffectSprite\EffectSprite.h"

using namespace std;
USING_NS_CC;
using namespace ui;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();


	if (initBox2DWorld(true))
	{
		this->schedule(schedule_selector(HelloWorld::tick));
		_world->SetContactListener((b2ContactListener*)this);

		auto body = addNewSprite(Vec2(visibleSize * 0.5f) + Vec2(0, -200), Size(100, 350), b2_staticBody, nullptr, 1, 0.0f);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* touch, Event* event) {

		return true;
	};
	listener->onTouchMoved = [=](Touch* touch, Event* event) {

		Point pos = touch->getLocation();
		_lightSprite->setPosition(pos);
		_lightPos.set(pos.x, pos.y, _lightPos.z);
		_effect->setLightPos(_lightPos);
	};
	listener->onTouchEnded = [=](Touch* touch, Event* event) {

		if (false)
		{
			if (random(0, 1) == 0)
			{
				auto spr = Sprite::create("bat_penguind_0.png");
				spr->getTexture()->setAliasTexParameters();
				spr->setScale(2.0f);
				spr->setOpacity(128);

				auto body = addNewSprite(touch->getLocation(), Size(200, 50), b2_dynamicBody, spr, 2, 0.0f, "myplane.xml.plist");
				body->SetAwake(true);
			}
			else
			{
				auto spr = Sprite::create("pipo-enemy009.png");
				//spr->getTexture()->setAliasTexParameters();
				//spr->setScale(2.0f);
				spr->setOpacity(128);

				auto body = addNewSprite(touch->getLocation(), Size(200, 50), b2_dynamicBody, spr, 2, 0.0f, "slime.plist");
				body->SetAwake(true);
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_effect = LightEffect::create();
	_effect->retain();

	_lightPos = Vec3(visibleSize.width - 100, visibleSize.height - 100, 100);
	_effect->setLightPos(_lightPos);
	_effect->setLightCutoffRadius(1000);
	_effect->setBrightness(2.0);

	float _scale = 2.0f;
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("8.png", Rect(0, 0, 98, 87)), "FRAME0");
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("8.png", Rect(10, 0, 78, 87)), "FRAME1");
	//auto sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("FRAME0"));
	//sprite->setPosition(visibleSize.width / 2.0, visibleSize.height / 2.0 - 75.0 * _scale);
	//sprite->setScale(_scale);
	//addChild(sprite);

	Vector<SpriteFrame*> frames;
	frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("FRAME0"));
	frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("FRAME1"));
	auto ani = Animation::createWithSpriteFrames(frames, 0.5f);
	EffectSprite* sprite = EffectSprite::createWithSpriteFrame(frames.front());
	sprite->setPosition(visibleSize.width / 2.0, visibleSize.height / 2.0 - 75.0 * _scale);
	sprite->runAction(RepeatForever::create(
		Animate::create(ani)
	));
	sprite->setEffect(_effect, "8_n.png");
	sprite->setScale(_scale);
	addChild(sprite);

	_lightSprite = Sprite::create("lightbulb.png");
	_lightSprite->setPosition(_lightPos.x, _lightPos.y);
	this->addChild(_lightSprite);

	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
b2Body* HelloWorld::addNewSprite(Vec2 pos, Size size, b2BodyType bodyType, Sprite* sprite, int type, float restitution)
{
	return addNewSprite(pos, size, bodyType, sprite, type, restitution, "");
}

b2Body* HelloWorld::addNewSprite(Vec2 pos, Size size, b2BodyType bodyType, Sprite* sprite, int type, float restitution, const string fname)
{
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	if (sprite != nullptr)
		addChild(sprite);
	bodyDef.userData = sprite;

	b2Body* body = _world->CreateBody(&bodyDef);
	b2FixtureDef fixtureDef;
	b2CircleShape circle;
	b2PolygonShape dynamicBox;

	if (type == 0)
	{
		//circle shape
		circle.m_radius = size.width / 2 / PTM_RATIO;
		fixtureDef.shape = &circle;
		fixtureDef.density = 10.0f;
		fixtureDef.friction = 1.0f;
		fixtureDef.restitution = restitution;// 1.0f;

		body->CreateFixture(&fixtureDef);
	}
	if (type == 1)
	{
		//poly shape
		dynamicBox.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 10.0f;
		fixtureDef.friction = 1.0f;
		fixtureDef.restitution = restitution;// 1.0f;

		body->CreateFixture(&fixtureDef);
	}
	if (type == 2)
	{
		//poly shape from file
		float scale = sprite->getScale();
		vector<b2Vec2> poly;

		ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fname);
		ValueMap &metadata = dict["metadata"].asValueMap();
		int format = metadata["format"].asInt();
		if (format != 1)
		{
			CCASSERT(format == 1, "format not supported!");
			return nullptr;
		}

		ValueMap &bodydict = dict.at("bodies").asValueMap();
		std::vector<b2FixtureDef> bodies(bodydict.size());
		int num = 0;

		for (auto iter = bodydict.cbegin(); iter != bodydict.cend(); ++iter)
		{
			ValueMap bodyData = iter->second.asValueMap();
			std::string bodyName = iter->first;
			ValueVector fixtures = bodyData["fixtures"].asValueVector();
			for (auto viter = fixtures.begin(); viter != fixtures.end(); ++viter)
			{
				auto fData = viter->asValueMap();
				b2FixtureDef def;
				def.density = fData["density"].asFloat();
				def.friction = fData["friction"].asFloat();
				def.restitution = fData["restitution"].asFloat();
				def.filter.categoryBits = fData["filter_categoryBits"].asInt();
				def.filter.groupIndex = fData["filter_maskBits"].asInt();
				def.isSensor = fData["isSensor"].asBool();

				auto polygons = fData["polygons"].asValueVector();
				for (auto piter = polygons.begin(); piter != polygons.end(); ++piter)
				{
					poly.clear();
					auto poslist = piter->asValueVector();
					//auto poslist = polygons.begin()->asValueVector();
					for (auto positer = poslist.begin(); positer != poslist.end(); ++positer)
					{
						auto bpos = PointFromString(positer->asString());
						poly.push_back(b2Vec2(bpos.x / PTM_RATIO * scale, bpos.y / PTM_RATIO * scale));
					}
					dynamicBox.Set(&poly[0], poly.size());

					fixtureDef.shape = &dynamicBox;

					fixtureDef.density = 10.0f;
					fixtureDef.friction = 1.0f;
					fixtureDef.restitution = restitution;// 1.0f;

					body->CreateFixture(&fixtureDef);
				}
			}
		}
	}

	return body;
}

//void HelloWorld::update(float dt)
//{
//
//}
void HelloWorld::tick(float dt)
{
	box2dUpdate(dt);

}
void HelloWorld::box2dUpdate(float dt)
{

	int velocityIterations = 8;
	int positionIterations = 1;

	_world->Step(dt, velocityIterations, positionIterations);

	for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != nullptr)
		{
			Sprite* spriteData = (Sprite*)b->GetUserData();
			spriteData->setPosition(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO);
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));

		}
		if (b->GetPosition().y * PTM_RATIO < -100)
		{
			if (b->GetUserData() != nullptr)
				((Sprite*)b->GetUserData())->removeFromParent();
			b2Body* nextBody = b->GetNext();
			_world->DestroyBody(b);
			b = nextBody;
		}
	}
}

void HelloWorld::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
	Scene::draw(renderer, transform, flags);
	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}
void HelloWorld::onDraw(const Mat4& transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	_world->DrawDebugData();
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
bool HelloWorld::initBox2DWorld(bool debugmode)
{
	Size vsize = visibleSize;

	auto gravity = b2Vec2(0.0f, -10.0f);
	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);

	if (debugmode)
	{
		m_debugDraw = new GLESDebugDraw(PTM_RATIO);
		_world->SetDebugDraw(m_debugDraw);

		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		m_debugDraw->SetFlags(flags);
	}

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);

	b2Body* groundBody = _world->CreateBody(&groundBodyDef);

	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	groundEdge.Set(b2Vec2(-(vsize.width / PTM_RATIO)*0.25f, 0), b2Vec2(vsize.width / PTM_RATIO * 1.25f, 0));
	groundBody->CreateFixture(&boxShapeDef);

	//groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, vsize.height / PTM_RATIO));
	//groundBody->CreateFixture(&boxShapeDef);

	groundEdge.Set(b2Vec2(0, vsize.height / PTM_RATIO), b2Vec2(vsize.width / PTM_RATIO, vsize.height / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	//groundEdge.Set(b2Vec2(vsize.width / PTM_RATIO, vsize.height / PTM_RATIO), b2Vec2(vsize.width / PTM_RATIO, 0));
	//groundBody->CreateFixture(&boxShapeDef);

	//scheduleUpdate();
	return true;
}
void HelloWorld::BeginContact(b2Contact* contact)
{
	log("BeginContact");
	num = 0;
}
void HelloWorld::PreSolve(b2Contact* contact, const b2Manifold* mainFold)
{
	log("PreSolve");
}
void HelloWorld::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	log("PostSolve : %d", num++);

	auto fixA = contact->GetFixtureA();
	auto fixB = contact->GetFixtureB();
	auto bodyA = fixA->GetBody();
	auto bodyB = fixB->GetBody();
	if (bodyA->GetType() == b2_dynamicBody || bodyB->GetType() == b2_dynamicBody)
		log("impulse %f", impulse->normalImpulses[0]);
}
void HelloWorld::EndContact(b2Contact* contact)
{
	log("EndContact");
}

