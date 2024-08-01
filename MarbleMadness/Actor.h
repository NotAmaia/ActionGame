#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Actor : public GraphObject{
public:
    Actor(int ID, int col, int row, int d, StudentWorld *student);
    virtual void doSomething() = 0;
    virtual ~Actor();
    virtual StudentWorld* getSW();
    virtual bool isSwallowable();
    virtual void setAlive(bool alive);
    virtual bool getAlive();
    virtual bool isPushable();
    virtual bool isSwallower();
    virtual bool isObstruction();
    virtual bool isKillShoot();
    virtual bool isNext();
    virtual void move(int r, int c, int d);
    virtual void setActivation(bool active);
    virtual bool getActivation();
    virtual bool isPower();
    virtual bool isRobber();
    virtual void changeHealth(int num);
    virtual int getHealth();
    virtual void setDied();
private:
    StudentWorld *s;
    bool isAlive;
    bool activated;
};
class Health : public Actor{
public:
    Health(int ID, int col, int row, int d, int health, StudentWorld *student);
    void changeHealth(int num);
    int getHealth();
    ~Health();
private:
    int m_health;
};
class passShot : public Actor{
public:
    passShot(int ID, int col, int row, int d, StudentWorld *student);
    bool isKillShoot();
    bool isObstruction();
    virtual ~passShot();
};
class Collect : public passShot{
public:
    Collect(int ID, int col, int row, int d, StudentWorld *student);
    bool isObstruction();
    virtual ~Collect();
};
class Goodie : public Collect{
public:
    Goodie(int ID, int col, int row, int d, StudentWorld *student);
    bool isPower();
    virtual ~Goodie();
};
class Crystal : public Collect{
public:
    Crystal(int col, int row, StudentWorld *student);
    void doSomething();
    ~Crystal();
};
class Ammo : public Goodie{
public:
    Ammo(int col, int row, StudentWorld *student);
    void doSomething();
    ~Ammo();
};
class ExtraLife : public Goodie{
public:
    ExtraLife(int col, int row, StudentWorld *student);
    void doSomething();
    ~ExtraLife();
};
class RestoreHealth : public Goodie{
public:
    RestoreHealth(int col, int row, StudentWorld *student);
    void doSomething();
    ~RestoreHealth();
};
class Wall : public Actor{
public:
    Wall(int col, int row, StudentWorld *student);
    void doSomething();
    ~Wall();
};
class Player : public Health{
public:
    Player(int col, int row, StudentWorld *student);
    void doSomething();
    void changeHealth(int num);
    ~Player();
    int getShoot();
    void increaseShoot(int incr);
private:
    int shoot;
};
class Marble : public Health{
public:
    Marble(int col, int row, StudentWorld *student);
    void doSomething();
    ~Marble();
    void setSwallowable();
    bool isSwallowable();
    bool isPushable();
};
class Pit : public passShot{
public:
    Pit(int col, int row, StudentWorld *student);
    void doSomething();
    bool isSwallower();
    bool isObstruction();
    ~Pit();
};
class Pea : public passShot{
public:
    Pea(int col, int row, int d, StudentWorld *student);
    void doSomething();
    ~Pea();
};
class Exit : public passShot{
public:
    Exit(int col, int row, StudentWorld *student);
    void doSomething();
    bool isNext();
    ~Exit();
private:
};
class Robot : public Health{
public:
   Robot(int ID, int col, int row, int d, int h, StudentWorld *student);
    bool rest();
    void shoot();
    void changeHealth(int num);
    void setDied();
    virtual ~Robot();
private:
    int count;
};
class Ragebot: public Robot{
public:
    Ragebot(int col, int row, int d, StudentWorld *student);
    void onTick();
    void setDied();
    ~Ragebot();
};
class Vert_Ragebot: public Ragebot{
public:
    Vert_Ragebot(int col, int row, StudentWorld *student);
    void doSomething();
    ~Vert_Ragebot();
};
class Horiz_Ragebot: public Ragebot{
public:
    Horiz_Ragebot(int col, int row, StudentWorld *student);
    void doSomething();
    ~Horiz_Ragebot();
};
class Thiefbot: public Robot{
public:
    Thiefbot(int ID, bool reg, int col, int row, int h, StudentWorld *student);
    void onTick();
    void setItem();
    bool isRobber();
    ~Thiefbot();
private:
    Actor* item;
    int length;
    bool hasItem;
    bool isReg;
};
class RegThiefbot: public Thiefbot{
public:
    RegThiefbot(int col, int row, StudentWorld *student);
    void doSomething();
    void setDied();
    ~RegThiefbot();
};
class MeanThiefbot: public Thiefbot{
public:
    MeanThiefbot(int col, int row, StudentWorld *student);
    void doSomething();
    void setDied();
    ~MeanThiefbot();

};
class ThiefbotFactory: public Actor{
public:
    ThiefbotFactory(int col, int row, bool shoot, StudentWorld *student);
    void doSomething();
    //bool isObstruction();
    ~ThiefbotFactory();
private:
    bool shooters;
    Actor* isBorn;
};
#endif // ACTOR_H_
