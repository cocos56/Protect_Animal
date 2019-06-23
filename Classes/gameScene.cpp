#pragma execution_character_set("utf-8")

#include "gameScene.h"
#include "NewLoseScene.h"
#include "NewWinScene.h"
#include "QE.h"
USING_NS_CC;

//设置初始时，即默认时按钮为英雄行走状态，当isbullet为true时为发子弹状态
bool isbullet=false;
Label * label_score_num_; //所得总分
Label *countTimeLabel;
int score_ ;//所得分数，没赋初值，默认为0
//初始化ButtonMenu所创建的layer
bool ButtonMenu::init()
{
    if (!Layer::init())
    {

        return false;
		
    }
	score_=0;
    auto item1=MenuItemImage::create("projectile-button-off.png", "projectile-button-off.png");
    auto item2=MenuItemImage::create("projectile-button-on.png","projectile-button-on.png");
    auto toggle=MenuItemToggle::createWithCallback(CC_CALLBACK_1(ButtonMenu::change, this),item1,item2,nullptr);

    auto menu=Menu::create(toggle,NULL);
    menu->setPosition(Point(20,20));
    this->addChild(menu);
	auto visibleSize=Director::getInstance()->getVisibleSize();
	//分数
	//4.添加当前所得具体分数score_num
    label_score_num_=Label::create();
    label_score_num_->setSystemFontSize(20);
    label_score_num_->setAnchorPoint(Vec2(0,0.5));
	label_score_num_->setPosition(Vec2(visibleSize.width-40 ,visibleSize.height-30));
    label_score_num_->setColor(cocos2d::Color3B(255,0,0));
     //设置分数值，初始值为0
    label_score_num_->setString(String::createWithFormat("%d",score_)->getCString());
   this->addChild(label_score_num_);
   //倒计时初始化
   time = 60;
   countTimeLabel = Label::createWithTTF("60", QE_Font, 50);
   countTimeLabel->setString("Time:20");
   countTimeLabel->setPosition(Vec2(visibleSize.width - 240, visibleSize.height - 30));
   countTimeLabel->setColor(Color3B(255, 255, 0));
   this->addChild(countTimeLabel, 5);
   this->schedule(schedule_selector(ButtonMenu::countDown), 1.0f);

	
    return true;
}

void ButtonMenu::change(Ref *obj)
{
   isbullet=isbullet?false:true;
     //isbullet=!isbullet;
    
}
	
//初始化HelloWorld所创建的layer
Scene* gameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = gameScene::create();
    scene->addChild(layer);
    auto menuLayer= ButtonMenu::create( );//创建一个layer层，名称叫menu
    scene->addChild(menuLayer);
   // log("scene的位置坐标:%f,%f",scene->getPosition().x,scene->getPosition().y);
    /*
     scene的位置坐标：0.000000,0.000000
     */
    log("layer的位置坐标:%f,%f",layer->getPosition().x,layer->getPosition().y);
    /*
     layer的位置坐标：0.000000,0.000000
     */
    return scene;
}
bool gameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
		int a  = UserDefault::getInstance()->getIntegerForKey("cha");
		log("%d",a);
		char uiLayerName[40];
	    sprintf(uiLayerName,"Mymap%d.tmx",a);
    //加载地图
	   _tileMap =TMXTiledMap::create(uiLayerName);

    this->addChild(_tileMap);

   // log("_tileMap的位置坐标：%f,%f",_tileMap->getPosition().x,_tileMap->getPosition().y);
    /*
     _tileMap的位置坐标：0.000000,0.000000
     */
    
    //获取地图中的不能行走的层（墙等障碍物）,meta和Fruit为普通图层
    _meta=_tileMap->getLayer("meta");
	_meta->setVisible(false);
    _fruit=_tileMap->getLayer("animal");
    
    //添加英雄精灵;
    _player=Sprite::create("www.png");
   
    //获取对象图层hero中的对象数组 ，英雄pa和敌人enemy为对象图层
    TMXObjectGroup *objectsValue=_tileMap->getObjectGroup("hero");
   
    //获取地图上设置好的英雄对象
    ValueMap playerPoint=objectsValue->getObject("pa");
    
    //根据地图上设置好的英雄位置来设置当前精灵的位置
    _player->setAnchorPoint(Point(0,0));
    //每一个瓦片的锚点都是（0，0），整个地图的锚点也是（0，0），
    _player->setPosition(Point(playerPoint.at("x").asFloat(),playerPoint.at("y").asFloat()));
    _tileMap->addChild(_player);//把英雄节点添加到地图节点上
    
    /*     __Array是cocos2d-x鼎力支持的数据结构类，它对游戏存储数组型数据做了优化，模拟了苹果里的NSMutableArray的功能，但执行效率更高。__Array是一个面向对象包装类，继承自Ref，并提供了一些列接口。
     初始化存放敌人的数组，因为create（）函数会将创建出来的数组放到自动释放池中，要想在其他函数中用就要retain一下,以免未用就被释放。

     */
    _enemyArray=__Array::create();//create()函数做4件事
    _enemyArray->retain();//array->autorelease();免得还没用完就释放
    //初始化存放子弹的数组
    _bulletArray=__Array::create();
    _bulletArray->retain();
    //快速遍历vector,里面存放的是对象层hero中的所有对象
    for (auto element:objectsValue->getObjects())//element为对象
    {
        //打印元素
       // log("快速遍历中element的Description：%s",element.getDescription().c_str());
        if (!element.isNull())	
        {
            //获取vector中各个对象的名称,asString()获取OC中的字符串，c_str()获取C中的字符串，非指针对象调用函数用点“.”，指针对象调用函数用“->”
            auto nameStr=(element.asValueMap().at("name")).asString();
            auto keyStr="enemy";
			log("name=%s",nameStr.c_str());
            //如果找到敌人对象，把精灵回到地图上显示出来
            if (strcmp(nameStr.c_str(),keyStr)==0)
            {
                //初始化敌人精灵，把地图中已经初始化好的敌人对象初始化为要用的敌人精灵。
                auto enemySprite=Sprite::create("enemy.png");
                float x=element.asValueMap().at("x").asFloat();
                float y=element.asValueMap().at("y").asFloat();
                enemySprite->setPosition(Point(x,y));
                enemySprite->setAnchorPoint(Point(0,0));                
                _tileMap->addChild(enemySprite);
                _enemyArray->addObject(enemySprite);
                //让敌人向英雄运动
                auto func=CallFuncN::create(CC_CALLBACK_1(gameScene::goon, this));
                //enemySprite->runAction(Sequence::create(func, NULL));
                enemySprite->runAction(func);
            }
        }
    }
    
    auto dispatcher=Director::getInstance()->getEventDispatcher();
    auto touchListener=EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan=CC_CALLBACK_2(gameScene::onTouchBegan, this);
    touchListener->onTouchMoved=CC_CALLBACK_2(gameScene::onTouchMoved,this);
    
    touchListener->onTouchEnded=CC_CALLBACK_2(gameScene::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    //间隔0.1秒监听一次是否发生碰撞
    this->schedule(schedule_selector(gameScene::updateGame),0.1);

	addChild(MessageSprite::createSprite(200, 150));
	return true;
}

void gameScene:: updateGame(float f)
{
     //数组的快速遍历
    Ref *obj=NULL;
    
    CCARRAY_FOREACH(_enemyArray, obj)
    {
       /* 强制类型转换：把Ref类型的obj 强制转换为sprite类型
      auto s=static_cast<Sprite *>(obj); 也可写成下面的转化方式 auto s=(Sprite *)obj;
       */
        auto s=(Sprite *)obj;
        if (_player->getBoundingBox().intersectsRect(s->boundingBox()))
        {
            //如果英雄与敌人发生碰撞就结束游戏回到主场景。
         //Director::getInstance()->replaceScene(HelloWorld::createScene());
			UserDefault::getInstance()->setIntegerForKey("score_", score_);
			Director::getInstance()->replaceScene(NewLoseScene::createScene());
        
        }
    }
    CCARRAY_FOREACH(_bulletArray, obj)
    {
        auto bullet=(Sprite*)obj;
        CCARRAY_FOREACH(_enemyArray, obj)
        {
            auto enemy=(Sprite *)obj;
            //如果子弹与敌人发生碰撞
            if (bullet->boundingBox().intersectsRect(enemy->boundingBox()))
            {
                //把子弹和敌人从地图和数组中移除
                enemy->removeFromParent();
                bullet->removeFromParent();
                _enemyArray->removeObject(enemy);
                _bulletArray->removeObject(bullet);
                 //子弹移除之后就跳出当前的循环
                break;
            }
        }
    }
}

/*让敌人向英雄的位置移动。goon 暴徒，愚笨者。此处参数为node类，因为CallFuncN * CallFuncN::create(const std::function<void(Node*)> &func)
*/
void gameScene::goon(Node *pSender)
{
    auto s=(Sprite*)pSender;//enemySprite
    //x轴方向移动的距离
    float x=_player->getPosition().x-s->getPosition().x>0?5:-5;
    //y轴方向上移动的距离
     float y=_player->getPosition().y-s->getPosition().y>0?5:-5;
    auto move=MoveBy::create(0.6,Point(x,y));
    auto func=CallFuncN::create(CC_CALLBACK_1(gameScene::goon, this));//递归调用
    s->runAction(Sequence::create(move,func, NULL));
}

 bool gameScene:: onTouchBegan(Touch *touch, Event *unused_event)
{
 //获取当前接触到的点的位置，属于世界坐标系
    auto touchLocation=touch->getLocation();
    //将世界坐标系转化为节点坐标系
    _beginPoint=this->convertToNodeSpace(touchLocation);
    log("_beginPoint:%f",_beginPoint.x);
     log("_beginPoint:%f",_beginPoint.y);
    return true;
}
void gameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}
void gameScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    //获取当前用户触摸到的位置
    Point touchLocation=touch->getLocation();
    //将当前最后的位置转化为节点坐标系，
    Point endPoint=this->convertToNodeSpace(touchLocation);
    
    log("endPoint:%f",endPoint.x);
    log("endPointt:%f",endPoint.y);
    if (!isbullet)
    {
     //如果开始和结束点是同一个点，当用户抬起手指的时候触发。
        //获取精灵当前的位置
        auto playerPos=_player->getPosition();
        //得到用户触摸点与当前精灵点的位置差距
        auto disPos=endPoint - playerPos;
          //判断偏移的距离是偏上下，还是偏左右
          //偏左右，让英雄精灵左右移动，每次偏移一个图块的宽度
        if (abs(disPos.x)>abs(disPos.y))
        {
            if (disPos.x > 0)
            {
                //每次偏移一个图块的宽度
                playerPos.x += _tileMap->getTileSize().width;
            }
            else
            {
                playerPos.x -= _tileMap->getTileSize().width;
            }
        }
         else//偏上下，让英雄精灵上下移动
         {
             if (disPos.y > 0)
             {
                 //每次偏移一个图块的高度
                 playerPos.y += _tileMap->getTileSize().height;
             }
             else
             {
                 playerPos.y -= _tileMap->getTileSize().height;
             }
          }
          //移动精灵到特定的位置
          movePlayer(playerPos);
          //让地图层随着精灵一起运动
          setViewpointCenter(_player->getPosition());
      }
    else
    {
        //发射飞镖，在该状态下，没触摸一次屏幕，就发射一个飞镖
        auto bullet= Sprite::create("Projectile.png");
        bullet->setPosition(_player->getPosition());
        _tileMap->addChild(bullet,4);
        _bulletArray->addObject(bullet);
        
        //当前手触摸点与英雄精灵x轴上的差值
        float dx=endPoint.x -_player->getPosition().x;
        //当前手触摸点与英雄精灵y轴上的差值
        float dy=endPoint.y - _player->getPosition().y;
        //向前发子弹
        if (dx >0)
        {
            auto winSize=Director::getInstance()->getWinSize();
            
            //winSize是指右边屏幕的边界
            /* _tileMap->getTileSize().width 获得每一个瓦块的宽度，单位是像素
               _tileMap->getMapSize().width  获得瓦片地图的宽度，单位是瓦块数，
             二者的相乘就得到瓦片地图宽度的像素值，利用相似三角形解决lx和ly。
             前面的坐标转换做好了铺垫，此时_player->getPosition()是相对于父节点地图的节点坐标系
             */
            float lx= _tileMap->getTileSize().width * _tileMap->getMapSize().width - _player->getPosition().x;
            // ly / lx =  dy / dx;
            float ly=dy/dx * lx;
            auto move=MoveBy::create(3, Point(lx + bullet->getContentSize().width,ly));
            //子弹发射完毕后执行回收子弹的函数
            auto ff=CallFuncN::create(CC_CALLBACK_1(gameScene::targetFinish, this));
            bullet->runAction(Sequence::create(move,ff,NULL));
        }
        else
        {
            float lx=0 - _player->getPosition().x;
            float ly=dy/dx * lx;
            auto move=MoveBy::create(3, Point(lx-bullet->getContentSize().width,ly));
            auto ff=CallFuncN::create(CC_CALLBACK_1(gameScene::targetFinish, this));
            bullet->runAction(Sequence::create(move,ff, NULL));
        }
    }
}
//当飞镖到达边界时移除飞镖
 void gameScene:: targetFinish( Node *pSender)
{
    pSender->removeFromParent();
    _bulletArray->removeObject(pSender);
}


void gameScene:: movePlayer(Point p)
{
    /*p.x 是指精灵要移动到的特定位置的横坐标x，单位是像素
    _tileMap->getTileSize().width每一个瓦块的像素高度，二者相除正好得到精灵要移动到的特定点在x的第几块上。假如 为130/30=4，余10，正好在第四块上，块的序号从0开始。
     _tileMap->getMapSize().height 瓦片地图的高度上的瓦块数
    */
    int x =p.x/_tileMap->getTileSize().width;
    int y= _tileMap->getMapSize().height-(p.y/_tileMap->getTileSize().height);
    
    //获取英雄当前所在地图的位置，即在地图的第几个瓦块上，为判断该瓦块的内容是西瓜还是墙等障碍物做准备
    Point currentPoint=Point(x,y);    //判断是否在地图范围内
	log("%d",x);
	log("%d",y);
    if (x< _tileMap->getMapSize().width && y<_tileMap->getMapSize().height&&x>=0&&y>=0)
    {
        //是否获取到目标，根据指定的点获取到瓦片的id
        int tilegid=_meta->getTileGIDAt(currentPoint);
        //3.0中返回的类型是value根据图块id获得当前的各个属性
        cocos2d::Value value=_tileMap->getPropertiesForGID(tilegid);
        //以字符串的形式获取value中的内容
        auto valueStr=value.getDescription();
        //如果瓦片存在
        if (tilegid)
        //tilegid=nullptr时代表瓦片不存在，第（0，0）块也存在，不等于nullptr
        {
            //判断value是否为空
            if (!value.isNull())
            {
                //如果是遇到了障碍物，且不是西瓜，就不让精灵移动
                if (valueStr.find("Collidable")!=-1)
                {
                    return;
                }
                else  if(valueStr.find("Collection")!=-1)
                {
                    //如果是,遇到了水果
                    _meta->removeTileAt(currentPoint);
                    //获取当前的水果，并将其从视图中移除
                    auto fruitSprite=_fruit->getTileAt(currentPoint);
                    fruitSprite->removeFromParent();
					score_+=100;
					label_score_num_->setString(String::createWithFormat("%d", score_)->getCString());
					if (score_ >= 1000)
					{
						auto scene = NewWinScene::createScene();
						Director::getInstance()->replaceScene(scene);
					}
	             }
            }
        }
        //让英雄移到点击的瓦片上
        _player->setPosition(p);
    }
}
void  gameScene:: setViewpointCenter(Point position)
{
    //获取当前屏幕的尺寸
    auto winSize=Director::getInstance()->getWinSize();
    //如果精灵在屏幕的1/2~3/2，x的值就是精灵当前的x值，否则x的值就是屏幕宽度的一半
    int x=MAX(position.x, winSize.width/2);
    int y=MAX(position.y, winSize.height/2);
    //获取整个地图的尺寸,单位是瓦块数
    auto mapSize=_tileMap->getMapSize();
     log("%lf,%lf",mapSize.width,mapSize.height);
    //获取每一个图块的宽高
    auto tileSize=_tileMap->getTileSize();
     log("%lf,%lf",tileSize.width,tileSize.height);
    /*重新规划x, y的值。mapSize.width * tileSize.width的宽度是2屏幕，减去winSize.width/2 的结果其实就是3/2屏幕宽。取3/2屏幕宽与x的较小值，取3/2屏幕高与y的较小值，分别作为x、y的的新值*/
    x=MIN(x, mapSize.width * tileSize.width-winSize.width/2);
    y=MIN(y, mapSize.height * tileSize.height - winSize.height/2);
    auto actualPoint=Point(x,y);
     //获取屏幕中心点的位置
    auto centerPoint=Point(winSize.width/2, winSize.height/2);
    auto viewPoint=centerPoint - actualPoint;
    /*重新规划当前视图（当前层）的位置。这个差始终为负数，把背景层的位置由默认的（0，0）设置为这个差值的位置，即把地图向左移动了|viewPoint|*/
    this->setPosition(viewPoint);
    log("重新规划后地图的位置：%f,%f",this->getPosition().x,this->getPosition().y);
     /*重新规划后地图的位置在-480～0之间，即最多可能向左移动一个屏幕宽*/
}
void ButtonMenu::countDown(float dt)
{
	time--;
	log("%d", time);
	str = __String::createWithFormat("Time:%d", time);

	countTimeLabel->setString(str->getCString());
	if (time <= 0)
	{
		UserDefault::getInstance()->setIntegerForKey("score_", score_);
		////时间到，游戏结束，取消预定函数
		this->unschedule(schedule_selector(ButtonMenu::countDown));
		Scene *scene = NewLoseScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
	/*else
	{
		this->unschedule(schedule_selector(HelloWorld::countDown));
		Scene *scene = NewLoseScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}*/
}

void gameScene::onExit()
{
    Layer::onExit();
    
    _enemyArray->release();
    _bulletArray->release();
}