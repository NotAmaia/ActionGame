#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <cstdlib>

Actor::Actor(int ID, int col, int row, int d, StudentWorld *student) : GraphObject(ID, col, row,  d),s(student),isAlive(true),activated(false){
    setVisible(true);
}

Actor::~Actor() {
    
}
bool Actor::isKillShoot(){
    return true;
}
bool Actor::isNext(){
    return false;
}
passShot::passShot(int ID, int col, int row, int d, StudentWorld *student):Actor(ID, col, row, d, student){}
passShot::~passShot(){}
bool passShot::isKillShoot(){return false;}
bool passShot::isObstruction(){return false;}
StudentWorld* Actor::getSW() { return s; }
bool Actor::isSwallowable(){
    return false;
}

bool Actor::getAlive(){
    return isAlive;
}
bool Actor::isRobber(){
    return false;
}
void Actor::setAlive(bool alive){
    isAlive = alive;
}
bool Actor::isPushable(){
    return false;
}
bool Actor::isSwallower(){
    return false;
}
bool Actor::isObstruction(){
    return true;
}
bool Actor::isPower(){
    return false;
}
void Actor::move(int c, int r, int d){
        if(d == up){
            moveTo(getX(),getY()+1);
        }
        else if(d == down){
            moveTo(getX(),getY()-1);
        }
        else if(d == right){
            moveTo(getX()+1,getY());
        }
        else{
            moveTo(getX()-1,getY());
        }
}
void Actor::setActivation(bool active){activated = active;}
bool Actor::getActivation(){
    return activated;
}
void Actor::changeHealth(int num){
    
}
void Actor::setDied(){
    
}
int Actor::getHealth(){
    return -1;
}
Health::Health(int ID, int col, int row, int d, int health, StudentWorld *student):Actor(ID, col, row, d, student), m_health(health){
    
}
void Health::changeHealth(int num){
    m_health += num;
    if(m_health <= 0){
        setAlive(false);
        setVisible(false);
    }
}
int Health::getHealth(){
    return m_health;
}
Health::~Health(){
    
}
Collect::Collect(int ID, int col, int row, int d, StudentWorld *student):passShot(ID, col, row, d, student){
}
bool Collect::isObstruction(){
    return false;
}
Collect::~Collect(){}
Goodie::Goodie(int ID, int col, int row, int d, StudentWorld *student):Collect(ID, col, row, none, student){
    setActivation(true);
}
bool Goodie::isPower(){return true;}
Goodie::~Goodie(){}
Crystal::Crystal(int col, int row, StudentWorld *student):Collect(IID_CRYSTAL, col, row, none, student){
    
}
void Crystal::doSomething(){
    if(!getAlive()){
        return;
    }
    if(getSW()->canCollect(getX(),getY(),this)){
        getSW()->decCollectForLevel();
        getSW()->increaseScore(50);
        setAlive(false);
        setVisible(false);
    }
}
Crystal::~Crystal(){}
Ammo::Ammo(int col, int row, StudentWorld *student):Goodie(IID_AMMO, col, row, none, student){
}
void Ammo::doSomething(){
    if(!getAlive()){
        return;
    }
    if(!getActivation()){
        return;
    }
    Player *s = getSW()->canCollect(getX(),getY(),this);
    if(s != nullptr){
        getSW()->increaseScore(100);
        s->increaseShoot(20);
            setAlive(false);
            setVisible(false);
        }
}
Ammo::~Ammo(){}
ExtraLife::ExtraLife(int col, int row, StudentWorld *student):Goodie(IID_EXTRA_LIFE, col, row, none, student){
}
void ExtraLife::doSomething(){
    if(!getAlive()){
        return;
    }
    if(!getActivation()){
        return;
    }
    Player *s = getSW()->canCollect(getX(),getY(),this);
    if(s != nullptr){
        getSW()->increaseScore(1000);
        getSW()->incLives();
            setAlive(false);
            setVisible(false);
        }
}
ExtraLife::~ExtraLife(){}
RestoreHealth::RestoreHealth(int col, int row, StudentWorld *student):Goodie(IID_RESTORE_HEALTH, col, row, none, student){
}
void RestoreHealth::doSomething(){
    if(!getAlive()){
        return;
    }
    if(!getActivation()){
        return;
    }
    Player *s = getSW()->canCollect(getX(),getY(),this);
    if(s != nullptr){
        getSW()->increaseScore(500);
        int a = 20-(s->getHealth());
        s->changeHealth(a);
            setAlive(false);
            setVisible(false);
        }
}
RestoreHealth::~RestoreHealth(){}
Wall::Wall(int col, int row, StudentWorld *student):Actor(IID_WALL, col, row, none, student){
    
}
void Wall::doSomething(){
    
}
Wall::~Wall(){

}
Player::Player(int col, int row, StudentWorld *student):Health(IID_PLAYER, col, row, right, 20, student),shoot(20)
{
    
}
Player::~Player(){
    
}
void Player::doSomething(){
    if(!getAlive()){
        return;
    }
    int ch;
    StudentWorld* sw = getSW();
    if(sw->getKey(ch)){
        switch(ch){
           case KEY_PRESS_UP: {
               setDirection(up);
               if(sw->push(getX(), getY()+1, this)){
               }
                else if(sw->obstruction(getX(),getY(),up,this)){
                }
                else{
                    move(getX(),getY(),up);
                }
            }
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(sw->push(getX(), getY()-1, this)){
                }
                else if(sw->obstruction(getX(),getY(),down,this)){
                }
                else{
                    move(getX(),getY(),down);
                }
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(sw->push(getX()-1, getY(), this)){
                }
                else if(sw->obstruction(getX(),getY(),left,this)){
                }
                else{
                    move(getX(),getY(),left);
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(sw->push(getX()+1,getY(),this)){
                }
                else if(sw->obstruction(getX(),getY(),right,this)){
                }
                else{
                    move(getX(),getY(),right);
                }
                break;
            case KEY_PRESS_SPACE:
                if(shoot > 0){
                    getSW()->playSound(SOUND_PLAYER_FIRE);
                    sw->createShot(getX(),getY(),getDirection());
                    shoot--;
                }
                break;
            case KEY_PRESS_ESCAPE:
                setAlive(false);
                break;
        }
    }
}
void Player::increaseShoot(int inc){
    shoot += inc;
}
void Player::changeHealth(int num){

    Health::changeHealth(num);
    if(getHealth()>0){
        getSW()->playSound(SOUND_PLAYER_IMPACT);
    }
}
int Player::getShoot(){
    return shoot;
}
Marble::Marble(int col, int row, StudentWorld *student):Health(IID_MARBLE, col, row, none, 10, student){
    
}
void Marble::doSomething(){
    if(!getAlive()){
        
        return;
    }
}
Marble::~Marble(){

}
bool Marble::isSwallowable(){
    return true;
}
bool Marble::isPushable(){
    return true;
}
Pit::Pit(int col, int row, StudentWorld *student):passShot(IID_PIT, col, row, none, student){
    
}
bool Pit::isObstruction(){
    return true;
}
void Pit::doSomething(){
    if(!getAlive()){
        return;
    }
    StudentWorld* sw = getSW();
    if(sw->swallow(getX(), getY(), this)){
        setAlive(false);
        setVisible(false);
    }
}
bool Pit::isSwallower(){
    return true;
}
Pit::~Pit(){

}
Pea::Pea(int col, int row, int d, StudentWorld *student):passShot(IID_PEA, col, row, d, student){
}
void Pea::doSomething(){
    if(!(getAlive())){
        return;
    }
    Actor* a = (getSW()->destroyShot(getX(),getY(),getDirection(),this));
    if(a != nullptr){
        a->changeHealth(-2);
        if(a->getHealth() == 0){
            a->setDied();
        }
        setAlive(false);
        setVisible(false);
        return;
    }
    move(getX(),getY(),getDirection());
}
Pea::~Pea(){}
Exit::Exit(int col, int row, StudentWorld *student):passShot(IID_EXIT, col, row, none, student){
    setVisible(false);
}
void Exit::doSomething(){
    if(getActivation()){
        setVisible(true);
        getSW()->moveOn(getX(),getY(),this);
    }
}
bool Exit::isNext(){
    return true;
}
Exit::~Exit(){}

Ragebot::Ragebot(int col, int row, int d, StudentWorld *student):Robot(IID_RAGEBOT, col, row, d, 10, student){}

void Ragebot::onTick(){
    if(!getAlive()){
        return;
    }
    if(rest()){
        return;
    }
    StudentWorld* sw = getSW();
    if(sw->enemyShot(getX(),getY(),getDirection(),this)){
        getSW()->playSound(SOUND_ENEMY_FIRE);
        return;
    }
    else if(getDirection() == up){
        if(sw->obstruction(getX(),getY(),getDirection(),this)){
            setDirection(down);
        }
        else{
            move(getX(),getY(),up);
        }
    }
    else if(getDirection() == left){
        if(sw->obstruction(getX(),getY(),getDirection(),this)){
            setDirection(right);
        }
        else{
            move(getX(),getY(),left);
        }
    }
    else if (getDirection() == right){
        if(sw->obstruction(getX(),getY(),getDirection(),this)){
            setDirection(left);
        }
        else{
            move(getX(),getY(),right);
        }
    }
    else if (getDirection() == down){
        if(sw->obstruction(getX(),getY(),getDirection(),this)){
            setDirection(up);
        }
        else{
            move(getX(),getY(),down);
        }
    }
}
void Ragebot::setDied(){
    Robot::setDied();
    getSW()->increaseScore(100);
}
Ragebot::~Ragebot(){}

Vert_Ragebot::Vert_Ragebot(int col, int row, StudentWorld *student):Ragebot(col, row, down, student){}
void Vert_Ragebot::doSomething(){
    
    
    
    if(!getAlive()){
        return;
    }
    onTick();
}
Vert_Ragebot::~Vert_Ragebot(){
    
}
Horiz_Ragebot::Horiz_Ragebot(int col, int row, StudentWorld *student):Ragebot(col, row, right, student){}
void Horiz_Ragebot::doSomething(){
    if(!getAlive()){
        return;
    }
    onTick();
}
Horiz_Ragebot::~Horiz_Ragebot(){}
Thiefbot::Thiefbot(int ID, bool reg, int col, int row, int h, StudentWorld *student):Robot(ID, col, row, right, h, student),item(nullptr),hasItem(false),isReg(reg){
    length = randInt(1,6);
}
void Thiefbot::onTick(){
    
    if(!(getAlive())){
        if(item != nullptr){
            item->setActivation(true);
            item->moveTo(getX(),getY());
            item->setVisible(true);
            item = nullptr;
        }
        return;
    }
    if(rest()){
        return;
    }
    if(!isReg){
        StudentWorld* sw = getSW();
        if(sw->enemyShot(getX(),getY(),getDirection(),this)){
            getSW()->playSound(SOUND_ENEMY_FIRE);
            return;
        }
    }
    if(!hasItem){
        item = getSW()->enemyCollect(this);
        if(item != nullptr){
            setItem();
        }
    }
    if(length != 0 && !(getSW()->obstruction(getX(),getY(),getDirection(),this))){
        move(getX(),getY(),getDirection());
        length--;
            return;
    }
    else{
        length = randInt(1,6);
        int d = randInt(1,4);
        d *= 90;
        setDirection(d);
        int count = 0;
        while(getSW()->obstruction(getX(),getY(),d,this) && count != 4){
            d += 90;
            d %= 360;
            setDirection(d);
            count++;
        }
        if(count == 4){
            return;
        }
        else{
        move(getX(),getY(),getDirection());
        length--;
        }
    }
}
void Thiefbot::setItem(){
    hasItem = true;
}
bool Thiefbot::isRobber(){
    return true;
}
Thiefbot::~Thiefbot(){}
MeanThiefbot::MeanThiefbot(int col, int row, StudentWorld *student):Thiefbot(IID_MEAN_THIEFBOT,false,col, row, 8, student){
    
}
void MeanThiefbot::doSomething(){
    onTick();
}
void MeanThiefbot::setDied(){
    Robot::setDied();
    getSW()->increaseScore(20);
}
MeanThiefbot::~MeanThiefbot(){
    
}
RegThiefbot::RegThiefbot(int col, int row, StudentWorld *student):Thiefbot(IID_THIEFBOT,true, col, row, 5, student){

}
void RegThiefbot::doSomething(){
    onTick();
}
void RegThiefbot::setDied(){
    Robot::setDied();
    getSW()->increaseScore(10);
}
RegThiefbot::~RegThiefbot(){
    
}
ThiefbotFactory::ThiefbotFactory(int col, int row, bool shoot, StudentWorld *student):Actor(IID_ROBOT_FACTORY, col, row, none, student),isBorn(nullptr),shooters(shoot){
}

/*
bool ThiefbotFactory::isObstruction(){
    return obstruct;
}*/
void ThiefbotFactory::doSomething(){
    if(isBorn != nullptr && isBorn->getX() != getX() && isBorn->getY() != getY()){
       isBorn = nullptr;
    }
    
    if(getSW()->countRobbers(this) < 3 && isBorn == nullptr){
        int d = randInt(1,50);
        if(d == 1){
            isBorn = getSW()->createRobber(shooters, this);
        }
    }
}
ThiefbotFactory::~ThiefbotFactory(){}
Robot::Robot(int ID, int col, int row, int d, int h, StudentWorld *student):Health(ID, col, row, d, h, student),count(1){}
bool Robot::rest(){
    int ticks = (28-getSW()->getLevel())/4;
    if(ticks < 3){
        ticks = 3;
    }
    if(count == ticks){
        count = 1;
        return false;
    }
    else{
        count++;
        return true;
    }
}
void Robot::changeHealth(int num){

    Health::changeHealth(num);
    if(getHealth()>0){
        getSW()->playSound(SOUND_ROBOT_IMPACT);
    }
}
void Robot::setDied(){
    getSW()->playSound(SOUND_ROBOT_DIE);
}
Robot::~Robot(){}



