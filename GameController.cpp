
#include "GameController.h"
#include "VisibleRect.h"
#include "cocos2d.h"
#include "PauseScene.h"
USING_NS_CC;
GameController* GameController::singleton_ = nullptr;
GameController::GameController() {
	auto cache = AnimationCache::getInstance();
	auto animationbomb0 = Animation::create();
	animationbomb0->addSpriteFrameWithFile(s_pathEnemy0Down1);
	animationbomb0->addSpriteFrameWithFile(s_pathEnemy0Down2);
	animationbomb0->addSpriteFrameWithFile(s_pathEnemy0Down3);
	animationbomb0->addSpriteFrameWithFile(s_pathEnemy0Down4);
	animationbomb0->setDelayPerUnit(1.0f / 4.0f);
	animationbomb0->setRestoreOriginalFrame(true);
	cache->addAnimation(animationbomb0,"enemy0");
	

	auto animationbomb1 = Animation::create();
	animationbomb1->addSpriteFrameWithFile(s_pathEnemy1Down1);
	animationbomb1->addSpriteFrameWithFile(s_pathEnemy1Down2);
	animationbomb1->addSpriteFrameWithFile(s_pathEnemy1Down3);
	animationbomb1->addSpriteFrameWithFile(s_pathEnemy1Down4);
	animationbomb1->setDelayPerUnit(1.0f / 4.0f);
	animationbomb1->setRestoreOriginalFrame(true);
	cache->addAnimation(animationbomb1, "enemy1");

	auto animationbomb2= Animation::create();
	animationbomb2->addSpriteFrameWithFile(s_pathEnemy2Down1);
	animationbomb2->addSpriteFrameWithFile(s_pathEnemy2Down2);
	animationbomb2->addSpriteFrameWithFile(s_pathEnemy2Down3);
	animationbomb2->addSpriteFrameWithFile(s_pathEnemy2Down4);
	animationbomb2->addSpriteFrameWithFile(s_pathEnemy2Down5);
	animationbomb2->addSpriteFrameWithFile(s_pathEnemy2Down6);
	animationbomb2->setDelayPerUnit(1.0f / 6.0f);
	animationbomb2->setRestoreOriginalFrame(true);
	cache->addAnimation(animationbomb2, "enemy2");

	auto animationbomb3 = Animation::create();
	animationbomb3->addSpriteFrameWithFile(s_pathHeroDown1);
	animationbomb3->addSpriteFrameWithFile(s_pathHeroDown2);
	animationbomb3->addSpriteFrameWithFile(s_pathHeroDown3);
	animationbomb3->addSpriteFrameWithFile(s_pathHeroDown4);
	animationbomb3->setDelayPerUnit(1.0f / 4.0f);
	animationbomb3->setRestoreOriginalFrame(true);
	cache->addAnimation(animationbomb2, "hero");
	initElepool();
}
GameController*  GameController::instance() {
	if (singleton_ == nullptr) {
		singleton_ = new GameController();
	}
	return singleton_;
}
void GameController::initElepool() {
	initEnemy(0);
	initEnemy(1);
	initEnemy(2);

}
void GameController::initEnemy(int index) {
	char enemyName[100] = { 0 };
	sprintf(enemyName, "image/enemy%d.png", index);
	for (int i = 0; i < _initCount[index]; i++) {
		//待优化,生命值,分值这些用的是if区分,后者现在这样只能设置成一样的
		_enemyElement* ele = _enemyElement::enemyWithTexture(Director::getInstance()->getTextureCache()->addImage(enemyName), 0.16f, 10, Vec2(1, 1), false, 10);
		//待优化
		if (i == 0) {
			if (index == 0) {
				getFrame(ele, __enemy0frame);
			}
			else if (index == 1) {
				getFrame(ele, __enemy1frame);
			}
			else {
				getFrame(ele, __enemy2frame);
			}
		}
		_enemyPool[index].pushBack(ele);
	}

}
void GameController::startGame(Node* node) {
	_gameScene = dynamic_cast<GameScene*>(node);
	hero = dynamic_cast<cocos2d::Sprite*>(node->getChildByName("hero"));
	label = dynamic_cast<cocos2d::Label*>(node->getChildByName("label"));
	GameController::instance()->getFrame(hero, __heroframe);
	hero->schedule(CC_SCHEDULE_SELECTOR(GameController::herodoStep),0.0f);
	_gameScene->schedule([this](float) {this->randomEnemy(0); },2.0f,"randomEnermy0");
	_gameScene->schedule([this](float) {this->randomEnemy(1); }, 5.0f, "randomEnermy1");
	_gameScene->schedule([this](float) {this->randomEnemy(2); }, 10.0f, "randomEnermy2");
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event* event) {
		this->destPos = new Vec2(touch->getLocation());
		return true; 
	};
	listener->onTouchMoved = [this](Touch* touch, Event* event) {
		this->destPos = new Vec2(touch->getLocation());
		return true;
	};
	listener->onTouchEnded = [this](Touch* touch, Event* event) {
		this->destPos = new Vec2(touch->getLocation());

		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, hero);	
}
void GameController::shooting(float deltaTime) {
	Vec2 pos= GameController::instance()->_gameScene->getChildByName("hero")->getPosition();
	pos.add(Vec2(0, 60));
	Sprite* sprite_bullet;
	if (!GameController::instance()->_bulletPool[0].empty()) {
		sprite_bullet = GameController::instance()->_bulletPool[0].back();
		if (sprite_bullet->getParent() != nullptr)
		{
			int t = 0;
		}
		GameController::instance()->_gameScene->addChild(sprite_bullet);
	    GameController::instance()->_bulletPool[0].popBack();
	}
	else {
		sprite_bullet = cocos2d::Sprite::create(s_pathBullet1);
		if (sprite_bullet->getParent() != nullptr)
		{
			int t = 0;
		}
		GameController::instance()->_gameScene->addChild(sprite_bullet);
	}
	sprite_bullet->setPosition(pos);
	sprite_bullet->schedule(CC_SCHEDULE_SELECTOR(GameController::bulletdoStep),0.0f);
	
}
void GameController::randomEnemy(int index) {
	char enemyName[100] = { 0 };
	sprintf(enemyName, "image/enemy%d.png", index);
	
	_enemyElement* sprite_enemy;
	if (!this->_enemyPool[index].empty()) {
 		sprite_enemy = this->_enemyPool[index].back(); 
		//CCLOG("pool prompted %p, hp: %d\n", sprite_enemy, sprite_enemy->_hp);
		if(sprite_enemy->getParent() != nullptr)
		{
			int t = 0;
		}
		this->_gameScene->addChild(sprite_enemy);
		sprite_enemy->setHp(10);//bug修正,防止一出现敌机就被回收的现象(多线程引发的问题)
		sprite_enemy->setPosition(random(60, 400), 800);
		this->_enemyPool[index].popBack();
		sprite_enemy->_tag = index;
		sprite_enemy->scheduleUpdate();
	}
	else {
		auto enemynode = _enemyElement::enemyWithTexture(Director::getInstance()->getTextureCache()->addImage(enemyName), 0.16f, 10, Vec2(1, 1), false,10);
		this->_enemyPool[index].pushBack(enemynode);
		randomEnemy(index);
		return;
	}
}

void GameController::herodoStep(float deltaTime) {
	cocos2d::Vec2 pos = GameController::instance()->hero->getPosition();
	Vec2 delta;
	if (GameController::instance()->destPos) {
		float dis = pos.distance(*(GameController::instance()->destPos));
		float time = dis / 800;
		delta = pos.lerp(*(GameController::instance()->destPos), MIN(deltaTime / time, 1));
		//this->_position = delta;
		this->setPosition(delta);
	}
	GameController::instance()->__heroframe->minX = delta.x - GameController::instance()->__heroframe->width / 2;
	GameController::instance()->__heroframe->maxX = delta.x + GameController::instance()->__heroframe->width / 2;
	GameController::instance()->__heroframe->minY = delta.y - GameController::instance()->__heroframe->height / 2;
	GameController::instance()->__heroframe->maxY = delta.y + GameController::instance()->__heroframe->height / 2;
	for (auto enemy : GameController::instance()->_inviewEnemys) {
		_enemyElement* _enemy = dynamic_cast<_enemyElement*>(enemy);
		if (GameController::instance()->enemycollisonWithHero(_enemy)) {
			if (GameController::instance()->getheroHp()--) {
				//CCLOG("colliding : %p\n", _enemy);
				Node* heronode = GameController::instance()->_gameScene->getChildByName("hero");
				auto tint = Sequence::create(TintTo::create(0.1, 255, 0, 0),
					TintTo::create(0.1, 255, 255, 255),
					nullptr);
				auto to = Sequence::createWithTwoActions(ProgressTo::create(6, 100), ProgressTo::create(0, 0));
				heronode->runAction(tint);
			}
			else {
				GameController::instance()->_gameScene->unscheduleAllCallbacks();
				auto animation = AnimationCache::getInstance()->getAnimation("hero");
				auto action = Animate::create(animation);
				this->runAction(action);
			}
		}
	}
}
void GameController::bulletdoStep(float deltaTime) {
	//CCLog("%p", *GameController::instance()->_enermyPrompted);
	cocos2d::Vec2 pos = this->getPosition() + Vec2(0, 20);
	if (pos.y > -5 && pos.y < 850 &&pos.x > 0) {
		this->setPosition(pos);
	}
	auto  trans = dynamic_cast<cocos2d::Node*>(this);
	Sprite* bulletnode = dynamic_cast<Sprite*>(trans);
	if (GameController::instance()->isOutofview(pos)) {
		setInValidPosition(this);
		GameController::instance()->recycleBullet(bulletnode);
		
	}
	else if (GameController::instance()->isInview(pos)) {
		
		GameController::instance()->bulletcollisonWithEnemy(this, pos);
	}
}
void GameController::recycleBullet(cocos2d::Sprite* bulletnode) {
	//不能更换执行顺序,否则会丢失在场景中的位置等信息
	_bulletPool[0].pushBack(bulletnode);
	this->_gameScene->removeChild(bulletnode);
}
void GameController::recycleEnemy(int index, _enemyElement* enemynode) {
		//CCLOG("before erase inview enemy cnt : %d\n", _inviewEnemys.size());
	    //CCLOG("enemy erase fome check set %p\n", enemynode);
		GameController::instance()->_inviewEnemys.eraseObject(enemynode);
		//CCLOG("after erase inview enemy cnt : %d\n", _inviewEnemys.size());
		enemynode->setHp(10);
		enemynode->showFirst = 0;
		enemynode->stopActionByTag(BOMBACTION);
		_enemyPool[index].pushBack(enemynode);
		this->_gameScene->removeChild(enemynode);
}
bool GameController::isInview(cocos2d::Vec2 position) {
	//x上下限和y上下限判断
	return position > VisibleRect::leftBottom() && position < VisibleRect::rightBottom() \
		&& position.y < VisibleRect::top().y&& position.y > VisibleRect::bottom().y;

}
bool  GameController::isOutofview(cocos2d::Vec2 position) {
	//y上下限判断
	return position.y < VisibleRect::bottom().y || position.y > VisibleRect::top().y + 50 ;
}
void GameController::bulletcollisonWithEnemy(Node* node, cocos2d::Vec2 v) {
	int ind = 0;
	for(auto enemynode : GameController::instance()->_inviewEnemys) {
		Sprite* bulletnode = dynamic_cast<Sprite*>(node);
		auto _enemy = dynamic_cast<_enemyElement*>(enemynode);
		//CCLOG("enemy%d pointer : %p\n", ind++, _enemy);
		//待优化
		if (_enemy->_tag == 0) {
			__enemyframe = __enemy0frame;
		}
		else if (_enemy->_tag == 1) {
			__enemyframe = __enemy1frame;
		}
		else {
			__enemyframe = __enemy2frame;
		}
		__enemyframe->minX = _enemy->getPosition().x - GameController::instance()->__enemyframe->width / 2;
		__enemyframe->maxX = _enemy->getPosition().x + GameController::instance()->__enemyframe->width / 2;
		__enemyframe->minY = _enemy->getPosition().y - GameController::instance()->__enemyframe->height / 2;
		__enemyframe->maxY = _enemy->getPosition().y + GameController::instance()->__enemyframe->height / 2;
		if (v.x >= __enemyframe->minX && v.x <= __enemyframe->maxX
			&& v.y >= __enemyframe->minY && v.y <= __enemyframe->maxY){
			_enemy->setHit(true);
			if (_enemy->getHp() > 0) {
				if (_enemy->getHp() == 1) {
					CCLOG("_enemy : %p \n", _enemy);
				}
				_enemy->getHp()--;
			}
			//CCLOG("inview enemy cnt : %d", _inviewEnemys.size());
			//CCLOG("hit enemy : %p", _enemy);
			GameController::instance()->setInValidPosition(bulletnode);
			GameController::instance()->recycleBullet(bulletnode);
			break;
		}
	}
}
bool GameController::enemycollisonWithHero(_enemyElement* enemynode) {
	//待优化
	if (enemynode->_tag == 0) {
		__enemyframe = __enemy0frame;
	}
	else if (enemynode->_tag == 1) {
		__enemyframe = __enemy1frame;
	}
	else {
		__enemyframe = __enemy2frame;
	}
	Vec2 pos = enemynode->getPosition();
	__enemyframe->minX = pos.x - GameController::instance()->__enemyframe->width / 2;
	__enemyframe->maxX = pos.x + GameController::instance()->__enemyframe->width / 2;
	__enemyframe->minY = pos.y - GameController::instance()->__enemyframe->height / 2;
	__enemyframe->maxY = pos.y + GameController::instance()->__enemyframe->height / 2;
	//CCLOG("enemy  x : pos %d, hero  x : pos %d", __enemyframe->minX)
	bool res = !(__heroframe->maxX < __enemyframe->minX || __heroframe->minX > __enemyframe->maxX
		|| __heroframe->maxY < __enemyframe->minY || __heroframe->minY > __enemyframe->maxY);
	return res;
}
 void GameController::getFrame(Node* node, frame* frame) {
	Size size = node->getContentSize();
	frame->width = size.width;
	frame->height = size.height;
}
void GameController::setheroHp(int hp) {
	_heroHp = hp;
}
int& GameController::getheroHp() {
	return _heroHp;
}
void GameController::setInValidPosition(cocos2d::Node* node) {
	//整个界面左侧
	Sprite* bulletnode = dynamic_cast<Sprite*>(node);
	bulletnode->setPosition(-100, 100);
}
void pauseGame()
{
	
}