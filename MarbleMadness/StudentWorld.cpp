#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <iomanip>
#include "Actor.h"
#include <string>
#include <list>
#include <sstream>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    bonus = 1000;
    nextLev = false;
    collectForLevel = 0;
}
void StudentWorld::setDisplayText(){
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int health = ((sp->getHealth())/20.0)*100;
    int ammo = sp->getShoot();
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.fill('0');
    oss << "Score: " << setw(7) << score << "  Level: " << setw(2) << level;
    oss.fill(' ');
    oss << "  Lives: " << setw(2) << lives << "  Health: " << setw(3) << health << "%  Ammo: " << setw(3) << ammo << "  Bonus: " << setw(4) << bonus;
    string s = oss.str();
    setGameStatText(s);
}
void StudentWorld::createShot(int col, int row, int d){
    if(d == GraphObject::up){
        s.push_front(new Pea(col,row+1,d,this));
    }
    else if(d == GraphObject::down){
        s.push_front(new Pea(col,row-1,d,this));
    }
    else if(d == GraphObject::left){
        s.push_front(new Pea(col-1,row,d,this));
    }
    else{
        s.push_front(new Pea(col+1,row,d,this));
    }
}
void StudentWorld::moveOn(int col, int row, Actor *o){
    if(sp->getX() == o->getX() && sp->getY() == o->getY()){
        nextLev = true;
    }
}
Actor* StudentWorld::enemyCollect(Actor *o){
    int x = randInt(1,10);
    if(x == 1){
        for(list<Actor*>::iterator p = s.begin(); p!=s.end(); p++){
            if((*p) == o){
                continue;
            }
            if((*p)->isPower() && (*p)->getAlive() && (*p)->getX() == o->getX() && (*p)->getY() == o->getY()){
                playSound(SOUND_ROBOT_MUNCH);
                (*p)->setActivation(false);
                (*p)->setVisible(false);
                return *p;
            }
        }
    }
    return nullptr;
}
int StudentWorld::countRobbers(Actor *o){
    int count = 0;
    int minCol = o->getX() - 3;
    if(minCol < 0){
        minCol = 0;
    }
    int minRow = o->getY() - 3;
    if(minRow < 0){
        minRow = 0;
    }
    int maxCol = o->getX() + 3;
    if(maxCol > 14){
        maxCol = 14;
    }
    int maxRow = o->getY() + 3;
    if(maxRow > 14){
        maxRow = 14;
    }
    for(int c = minCol; c < maxCol; c++){
        for(int r = minRow; r < maxRow; r++){
            for(list<Actor*>::iterator p = s.begin(); p!=s.end(); p++){
                if((*p) == o){
                    continue;
                }
                if((*p)->isRobber() && (*p)->getAlive() && (*p)->getX() == c && (*p)->getY() == r){
                    count++;
                }
            }
        }
    }
    return count;
}
bool StudentWorld::enemyShot(int col, int row, int d, Actor *o){
    if(d == GraphObject::up){
        int u = o->getY();
        if(sp->getX() == o->getX() && sp->getY() > o->getY()){
            while(u != sp->getY()){
                Actor* a = destroyShot(col,u,d,o);
                if(a != nullptr){
                    return false;
                }
                u++;
            }
            createShot(col,row,d);
            return true;
        }
        return false;
    }
    else if(d == GraphObject::down){
        int dow = o->getY();
        if(sp->getX() == o->getX() && sp->getY() < o->getY()){
            while(dow != sp->getY()){
                Actor* a = destroyShot(col,dow,d,o);
                if(a != nullptr){
                    return false;
                }
                dow--;
            }
            createShot(col,row,d);
            return true;
        }
        return false;
    }
    else if(d == GraphObject::left){
        int l = o->getX()-1;
        if(sp->getX() < o->getX() && sp->getY() == o->getY()){
            while(l != sp->getX()){
                Actor* a = destroyShot(l,row,d,o);
                if(a != nullptr){
                    return false;
                }
                l--;
            }
            createShot(col,row,d);
            return true;
            
        }
        return false;
    }
    else{
        int r = o->getX();
        if(sp->getX() > o->getX() && sp->getY() == o->getY()){
            while(r != sp->getX()){
                Actor* a = destroyShot(r,row,d,o);
                if(a != nullptr){
                    return false;
                }
                r++;
            }
            createShot(col,row,d);
            return true;
        }
        return false;
    }
}
Actor* StudentWorld::destroyShot(int col, int row, int d, Actor *o){
    for(list<Actor*>::iterator p = s.begin(); p!=s.end(); p++){
        if((*p) == o){
            continue;
        }
        if((*p)->getX() == col && (*p)->getY() == row && (*p)->getAlive() && (*p)->isKillShoot()){
                return (*p);
            }
        if((sp)->getX() == col && (sp)->getY() == row && sp->getAlive() && (sp)->isKillShoot()){
                return sp;
        }
        
        
        /*
        else if(d == GraphObject::down){
            if((*p)->getX() == col && (*p)->getY() == row-1 && (*p)->isKillShoot()){
                return true;
            }
        }
        else if(d == GraphObject::right){
            if((*p)->getX() == col+1 && (*p)->getY() == row && (*p)->isKillShoot()){
                return true;
            }
        }
        else{
            if((*p)->getX() == col-1 && (*p)->getY() == row && (*p)->isKillShoot()){
                return true;
            }
        }*/
    }
    if((sp) == o){
        return nullptr;
    }
return nullptr;
}
Player* StudentWorld::canCollect(int col, int row, Actor *o){
    if((sp)->getX() == col && (sp)->getY() == row){
        playSound(SOUND_GOT_GOODIE);
        return sp;
    }
    return nullptr;
}
void StudentWorld::decCollectForLevel(){
    collectForLevel--;
}
bool StudentWorld::obstruction(int col, int row, int d, Actor *o){
        for(list<Actor*>::iterator p = s.begin(); p!=s.end(); p++){
            if((*p) == o){
                continue;
            }
            if(d == GraphObject::up){
                if((*p)->getX() == col && (*p)->getY() == row+1 && (*p)->getAlive() && (*p)->isObstruction()){
                    return true;
                }
            }
            else if (d == GraphObject::down){
                if((*p)->getX() == col && (*p)->getY() == row-1 && (*p)->getAlive() && (*p)->isObstruction()){
                    return true;
                }
            }
            else if (d == GraphObject::left){
                if((*p)->getX() == col-1 && (*p)->getY() == row && (*p)->getAlive() && (*p)->isObstruction()){
                    return true;
                }
            }
            else{
                if((*p)->getX() == col+1 && (*p)->getY() == row && (*p)->getAlive() && (*p)->isObstruction()){
                    return true;
                }
            }
        }
    if(sp == o){
        return false;
    }
    if(d == GraphObject::up){
        if((sp)->getX() == col && (sp)->getY() == row+1 && (sp)->getAlive() && (sp)->isObstruction()){
            return true;
        }
    }
    else if (d == GraphObject::down){
        if((sp)->getX() == col && (sp)->getY() == row-1 && (sp)->getAlive() && (sp)->isObstruction()){
            return true;
        }
    }
    else if (d == GraphObject::left){
        if((sp)->getX() == col-1 && (sp)->getY() == row && (sp)->getAlive() && (sp)->isObstruction()){
            return true;
        }
    }
    else{
        if((sp)->getX() == col+1 && (sp)->getY() == row && (sp)->getAlive() && (sp)->isObstruction()){
            return true;
        }
    }
    return false;
}
Actor* StudentWorld::createRobber(bool createShooter, Actor *o){
    playSound(SOUND_ROBOT_BORN);
    Actor* p;
    if(createShooter){
        p = new MeanThiefbot(o->getX(),o->getY(),this);
    }
    else{
        p = new RegThiefbot(o->getX(),o->getY(),this);
    }
    s.push_front(p);
    return p;
}
bool StudentWorld::swallow(int col, int row, Actor *o){
    for(list<Actor*>::iterator p = s.begin(); p!=s.end(); p++){
        if((*p) == o){
            continue;
        }
        if((*p)->getX() == col && (*p)->getY() == row && (*p)->getAlive() && (*p)->isSwallowable()){
            (*p)->setAlive(false);
            (*p)->setVisible(false);
            return true;
        }
    }
    return false;
}
bool StudentWorld::canPush(int col, int row, Actor *o){
    for(list<Actor*>::iterator p = s.begin(); p!=s.end(); p++){
        if((*p) == o){
            continue;
        }
        if((*p)->getX() == col && (*p)->getY() == row && !((*p)->isSwallower())){
            return true;
        }
    }
    return false;
}
bool StudentWorld::push(int col, int row, Actor *o){
    for(list<Actor*>::iterator p = s.begin(); p!=s.end(); p++){
        if((*p) == o){
            continue;
        }
        if((*p)->getX() == col && (*p)->getY() == row && (*p)->getAlive() && (*p)->isPushable()){
            int d = o->getDirection();
            if(d == GraphObject::up){
                if(canPush(col,row+1,*p)){
                    return false;
                }
                (*p)->move((*p)->getX(),(*p)->getY(),(o)->getDirection());
                o->move(o->getX(),o->getY(),o->getDirection());
            }
            else if(d == GraphObject::down){
                if(canPush(col,row-1,*p)){
                    return false;
                }
                (*p)->move((*p)->getX(),(*p)->getY(),(o)->getDirection());
                o->move(o->getX(),o->getY(),o->getDirection());
            }
            else if(d == GraphObject::left){
                if(canPush(col-1,row,*p)){
                    return false;
                }
                (*p)->move((*p)->getX(),(*p)->getY(),(o)->getDirection());
                o->move(o->getX(),o->getY(),o->getDirection());
            }
            else{
                if(canPush(col+1,row,*p)){
                    return false;
                }
                (*p)->move((*p)->getX(),(*p)->getY(),(o)->getDirection());
                o->move(o->getX(),o->getY(),o->getDirection());
            }
            return true;
        }
    }
    return false;
}

int StudentWorld::init()
{
    Level lev(assetPath());
    ostringstream oss;
    int k = getLevel();
    oss.fill('0');
    oss << "level" << setw(2) << k << ".txt";
    string sLev = oss.str();
    //cout << sLev;
    Level::LoadResult result = lev.loadLevel(sLev);
    if (result == Level::load_fail_file_not_found || getLevel() > 99)
        return GWSTATUS_PLAYER_WON;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level\n";
        for(int c = 0 ; c < 15; c++){
            for(int r = 0; r < 15; r++){
                Level::MazeEntry ge = lev.getContentsOf(c,r);
                switch (ge)
                {
                    case Level::empty: {
                        break;
                    }
                    case Level::player: {
                        sp = new Player(c,r,this);
                        break;
                    }
                    case Level::wall: {
                        s.push_back(new Wall(c,r, this));
                        break;
                    }
                    case Level::marble:{
                        s.push_back(new Marble(c,r,this));
                        break;
                    }
                    case Level::pit:{
                        s.push_back(new Pit(c,r,this));
                        break;
                    }
                    case Level::crystal:{
                        s.push_back(new Crystal(c,r,this));
                        collectForLevel++;
                        break;
                    }
                    case Level::ammo:{
                        s.push_back(new Ammo(c,r,this));
                        break;
                    }
                    case Level::extra_life:{
                        s.push_back(new ExtraLife(c,r,this));
                        break;
                    }
                    case Level::restore_health:{
                        s.push_back(new RestoreHealth(c,r,this));
                        break;
                    }
                    case Level::exit:{
                        s.push_back(new Exit(c,r,this));
                        break;
                    }
                    case Level::vert_ragebot:{
                        s.push_back(new Vert_Ragebot(c,r,this));
                        break;
                    }
                    case Level::horiz_ragebot:{
                        s.push_back(new Horiz_Ragebot(c,r,this));
                        break;
                    }
                    case Level::thiefbot_factory:{
                        s.push_back(new ThiefbotFactory(c,r,false, this));
                        break;
                    }
                    case Level::mean_thiefbot_factory:{
                        s.push_back(new ThiefbotFactory(c,r,true,this));
                        break;
                    }
                         
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q
    if(collectForLevel == 0){
        playSound(SOUND_REVEAL_EXIT);
        for(list<Actor*>::iterator p = s.begin(); p != s.end(); p++){
            if((*p)->isNext()){
                (*p)->setActivation(true);
                collectForLevel--;
            }
        }
    }
    for(list<Actor*>::iterator p = s.begin(); p != s.end(); p++){
        (*p)->doSomething();
    }
    if(sp->getHealth()<=0){
        
        playSound(SOUND_PLAYER_DIE);
        bonus = 1000;
        nextLev = false;
        collectForLevel = 0;
        decLives();
       return GWSTATUS_PLAYER_DIED;
    }
    if(nextLev){
        playSound(SOUND_FINISHED_LEVEL);
        increaseScore(bonus+2000);
        bonus = 1000;
        nextLev = false;
        collectForLevel = 0;
        return GWSTATUS_FINISHED_LEVEL;
    }
    
   sp->doSomething();
    if(!(sp->getAlive())){
        playSound(SOUND_PLAYER_DIE);
        bonus = 1000;
        nextLev = false;
        collectForLevel = 0;
        decLives();
       return GWSTATUS_PLAYER_DIED;
    }
    for(list<Actor*>::iterator p = s.begin(); p != s.end(); ){
        if(!((*p)->getAlive())){
            delete *p;
            p = s.erase(p);
        }
        else{
            p++;
        }
    }
    if(bonus > 0){
        bonus--;
    }
    setDisplayText();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if(sp != nullptr){
        delete sp;
        sp = nullptr;
    }
        for(list<Actor*>::iterator p = s.begin(); p != s.end(); ){
                delete *p; //MIGHT NEED TO CHANGE
                p = s.erase(p);
        }
}
StudentWorld::~StudentWorld(){
    cleanUp();
}

