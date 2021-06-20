#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class GhostRacer;
class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    int init();
    int move();
    void cleanUp();
    ~StudentWorld();
    GhostRacer* racer();
    int getSouls();
    void incSouls(int amt);
    int getLevelSouls();
    void setSoulsSaved(int amt);
    void setLevelSouls(int amt);
    bool ProjActivate();
    void setendLevel();
    bool getendLevel();
    void setcompleteLevel();
    bool getcompleteLevel();
    bool ProjOverlap(Actor* m_actor);
    bool inFrontCab(Actor* m_actor);
    bool behindCab(Actor* m_actor);
    int getLane(Actor* m_actor);
    void addZombieCabbelow();
    void addZombieCababove();
    void addOilSlick();
    void addZombiePed();
    void addHumanPed();
    void addHolyWaterRefill();
    void reset();
    void reset_dead();
    void addLostSoul();
    void externalAddOil(double x, double y);
    void externalAddHealingGood(double x, double y);
    void externalAddHolyWaterProj(double x, double y, int dir);
private:
    GhostRacer* m_racer;
    std::vector<Actor*> object;
    double yellow_last;
    double white_last;
    int soulsSaved = 0;
    bool endLevel = false;
    int bonus = 5000;
    bool completeLevel = false;
    int levelSoul = 2 * getLevel() + 5;
    int startlevelsoul = 2 * getLevel() + 5;
};

#endif // STUDENTWORLD_H_
