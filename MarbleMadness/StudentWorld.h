#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>

class Actor;
class Player;
class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    bool obstruction(int col, int row, int d, Actor *o);
    bool swallow(int col, int row, Actor *o);
    bool push(int col, int row, Actor *o);
    bool canPush(int col, int row, Actor *o);
    Player* canCollect(int col, int row, Actor *o);
    void setDisplayText();
    void decCollectForLevel();
    Actor* destroyShot(int col, int row, int d, Actor *o);
    void createShot(int col, int row, int d);
    bool enemyShot(int col, int row, int d, Actor *o);
    void moveOn(int col, int row, Actor *o);
    int countRobbers(Actor *o);
    Actor* enemyCollect(Actor *o);
    Actor* createRobber(bool createShooter, Actor *o);
    ~StudentWorld();

private:
    std::list<Actor*> s;
    Player* sp;
    int collectForLevel;
    int bonus;
    bool nextLev;
};

#endif // STUDENTWORLD_H_
