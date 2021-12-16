
#ifndef _GAME_CONTROLLER_H_
#define _GAME_CONTROLLER_H_
#include "GameScene.h"
#include"Resouce.h"
USING_NS_CC;

class GameController : public  cocos2d::Node {
#define BOMBACTION 0x01
    typedef int& (* SEL_SCHEDULE)();
   typedef struct frame {
        float width;
        float height;
        float minX;
        float minY;
        float maxX;
        float maxY;
    }_frame;
   //friend class _enemyElement;
public:
    struct _enemyElement : cocos2d::Sprite
    {
        friend class GameController;
    public:
        _enemyElement::_enemyElement(void)
        {
        }

        _enemyElement::~_enemyElement(void)
        {
        }
        static _enemyElement* enemyWithTexture(cocos2d::Texture2D* aTexture,  float interval, int hp, cocos2d::Vec2 speed, bool hit, int score) {
        	_enemyElement* pEnemy = new (std::nothrow) _enemyElement();
            pEnemy->initWithTexture(aTexture);
            pEnemy->initArguments(interval, hp, speed, hit, score);
            pEnemy->setPosition(random(60, 400), 800);
            pEnemy->setScale(2.0f);
            return pEnemy;
        }
        void  initArguments(float interval, int hp, cocos2d::Vec2 speed, bool hit, int score) {
            _interval = interval;
            _hp = hp;
            _speed = speed;
            _hit = hit;
            _score = score;
        }
        void setInterval(float time) {
            this->_interval = time;
        }
        void setHit(bool hit) {
            _hit = hit;
        }
        void setHp(int hp) {
            _hp = hp;
        }
        float getInterval() {
            return _interval;
        }
        int& getHp() {
            return _hp;
        }
        cocos2d::Vec2 getSpeed() {
            return _speed;
        }
        bool getHit() {
            return _hit;
        }
        void update(float d) {
            cocos2d::Vec2 pos = this->getPosition();
            if (pos.y > -5 && pos.y < 820 && pos.x > 0) {
                pos = pos +Vec2(0, -d * 100);
                this->setPosition(pos);
            }
            //showFirst作用:不管update多少次,当前界面的该敌机只放一次inviewEnemy
            if (GameController::instance()->isInview(pos) && this->getHp() > 0) {
                if (!showFirst) {
                    //CCLOG("enemy put %p into check set\n", this);
                    GameController::instance()->_inviewEnemys.pushBack(this);
                }
                showFirst = 1;
            }
            if (this->getHp() <= 0) {
                //CCLOG("enemy recycle %p\n", this);
                this->unscheduleUpdate();
                char bombName[100] = { 0 };
                sprintf(bombName, "enemy%d", this->_tag);
                
                CCLOG("bombing _enemy : %p \n", this);
               // GameController::instance()->recycleEnemy(this->_tag, this);
                //播放动画的序列把removchild添加进去(和播放动画同步起来),否则在没有播放动画(异步)之前就被remove了
                auto animation = AnimationCache::getInstance()->getAnimation(bombName);
                auto action = Animate::create(animation);
                 CallFunc * func = CallFunc::create(CC_CALLBACK_0(GameController::recycleEnemy, GameController::instance(), this->_tag, this));
                auto bomb = Sequence::create(action, func, nullptr);
                bomb->setTag(BOMBACTION);
                this->runAction(bomb);
                GameController::instance()->gameScore += this->_score;
                GameController::instance()->label->setString(std::to_string(GameController::instance()->gameScore));
                if(GameController::instance()->gameScore >= 100)
                {
                    GameController::instance()->_gameScene->unscheduleAllCallbacks();

                }
                return;
            }
            //UnlockResource(this);
            if (GameController::instance()->isOutofview(pos)) {
                GameController::instance()->recycleEnemy(this->_tag, this);
                GameController::instance()->setInValidPosition(this);
            }

            if (this->getHit() && this->_tag != 0) {
                this->setHit(false);

            }
        }
    private:
        int showFirst = 0;
        int _tag;
        float _interval;
        int _hp;
        int _score;
        cocos2d::Vec2 _speed;
        bool _hit;
    };

	static GameController* instance();
    void initBombanimation();
    void initEnemy(int index);
    void initElepool();
    void shooting(float deltaTime);
    void startGame(Node* node);
    void randomEnemy(int index);
    void enemydoStep(float deltaTime);
    void bulletdoStep(float);
    void herodoStep(float);
    void recycleBullet(cocos2d::Sprite* bulluetnode);
    void recycleEnemy(int index, _enemyElement* enemynode);
    bool isInview(cocos2d::Vec2 position);
    bool isOutofview(cocos2d::Vec2 position);
    void bulletcollisonWithEnemy(cocos2d::Node* node, cocos2d::Vec2 v);
    bool enemycollisonWithHero(_enemyElement* enemynode);
    void setInValidPosition(cocos2d::Node* node);
   // virtual int& getHp();
    static void getFrame(Node* node, _frame*);
    void setheroHp(int hp);
    int& getheroHp();
    void pauseGame();
public:
    std::mutex mtx;
    cocos2d::Vec2* destPos = nullptr;
    static GameController* singleton_;
    GameScene* _gameScene;
    cocos2d::Sprite* hero;
    cocos2d::Label* label;
    GameController();
  
    cocos2d::Vector<struct _enemyElement*> _enemyPool[3];
    cocos2d::Vector<cocos2d::Sprite*> _bulletPool[3];
    int  _initCount[3] = { 10, 5, 2 };
    cocos2d::Vector<cocos2d::Node*> _inviewEnemys;
    int _heroHp = 0;
	_frame* __heroframe = (_frame*)malloc(sizeof(_frame));
    _frame* __enemyframe = (_frame*)malloc(sizeof(_frame));
    _frame* __enemy0frame = (_frame*)malloc(sizeof(_frame));
    _frame* __enemy1frame = (_frame*)malloc(sizeof(_frame));
    _frame* __enemy2frame = (_frame*)malloc(sizeof(_frame));
    int gameScore = 0;
};
#endif