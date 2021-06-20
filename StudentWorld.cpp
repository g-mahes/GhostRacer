#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;



GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
}



int StudentWorld::init()
{

    // adding Ghostracer
    m_racer = new GhostRacer(this,128, 32);

    // adding n yellow border lines and pushing into vector
    int n = VIEW_HEIGHT / SPRITE_HEIGHT;
    for (int i = 0; i < n; i++)
    {
        object.push_back(new Borderline(this,IID_YELLOW_BORDER_LINE, LEFT_EDGE, i * SPRITE_HEIGHT)); // adds yellow borderline
        object.push_back(new Borderline(this,IID_YELLOW_BORDER_LINE, RIGHT_EDGE, i * SPRITE_HEIGHT)); // adds yellow borderline
        if (i == n - 1)
        {
            yellow_last = i * SPRITE_HEIGHT;
        }
    }

    // adding M white border lines and pushing into vector
    int m = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    for (int i = 0; i < m; i++) {
        object.push_back(new Borderline(this, IID_WHITE_BORDER_LINE, LEFT_EDGE + (ROAD_WIDTH / 3), i * 4 * SPRITE_HEIGHT)); // adds white borderline
        object.push_back(new Borderline(this, IID_WHITE_BORDER_LINE, RIGHT_EDGE - (ROAD_WIDTH / 3), i * 4 * SPRITE_HEIGHT)); // adds white borderline
        if (i == m - 1)
        {
            white_last = i * 4 * SPRITE_HEIGHT; // keep track of last white line 
        }
    }
    return GWSTATUS_CONTINUE_GAME; // continue game
}

GhostRacer* StudentWorld::racer()  {
    GhostRacer* temp = m_racer;
    return temp; // return ghost racer pointer
}

int StudentWorld::getSouls() {
    return soulsSaved; // return souls saved by user
}


void StudentWorld::incSouls(int amt) {
    soulsSaved += amt; // increase souls saved by user to amt
}

void StudentWorld::setSoulsSaved(int amt)
{
    soulsSaved = amt; // set the souls saved by the user to amt
}

void StudentWorld::setLevelSouls(int amt) {
    levelSoul = amt; // set souls to be saved to amt
}


int StudentWorld::getLevelSouls() {
    return levelSoul - getSouls(); // return the number of souls left to save in the level
}


bool StudentWorld::ProjOverlap(Actor* m_actor) {
    for (int i = 0; i < object.size(); i++) {
        if (object[i]->isProj()) { // find object in vector that is holy water projectile
            if (m_actor->overlap(object[i])) { // check if projectile overlaps with object
                return true; // return true if there is overlap
            }
            else
                return false; // return false if no overlap
        }
    }
    return false; // return false if no projectile found
}

bool StudentWorld::ProjActivate() {
    for (int i = 0; i < object.size(); i++) {
        if (object[i]->isProj()) // if object is a projectile
        {
            for (int g = 0; g < object.size(); g++) { // loop until we find object that is affected by projectiles and overlaps with the projectile
                if (object[g]->isAffectedProj() && object[i]->overlap(object[g]))
                {
                    if (object[g]->isZombieCab()) {
                        object[g]->decHealth(1); // decrease health by 1
                        if (object[g]->getHealth() <= 0) { // see if health is 0 or below
                            object[g]->makeDead(); // make dead if health reaches below 1
                            playSound(SOUND_VEHICLE_DIE); // play sound
                            int oilchance = randInt(1, 5); // variable to see if new oil slick will be left behind
                            if (oilchance == 1) { // if 1 in 5 chance occurs
                                externalAddOil(object[g]->getX(), object[g]->getY()); // spawn new oil slick where zombie cab died
                                increaseScore(200); // increase player score
                            }
                        }
                        else { // not dead with projectile
                           playSound(SOUND_VEHICLE_HURT); // plays sound
                        }
                    }
                    if (object[g]->isZombiePed()) {
                        object[g]->decHealth(1); // decrement health by 1
                        if (object[g]->getHealth() <= 0) { // if health is below 0
                            object[g]->makeDead(); // make zombie dead
                            playSound(SOUND_PED_DIE);
                            if (!object[g]->overlap(racer())) {
                                int chance; // variable to track chance that zombie ped will leave something behind after dying
                                chance = randInt(1, 5); // 1 in 5 chance that something will be dropped
                                if (chance == 1) { // if the probability is a success
                                    externalAddHealingGood(object[g]->getX(), object[g]->getY());// new healing good left at zombie ped's location
                                }
                            }
                            increaseScore(150); // increase score 
                        }
                        else {
                            playSound(SOUND_PED_HURT);
                        }
                    }
                    if (object[g]->isGoodie() && !object[g]->isSoulGoodie()) {
                        object[g]->makeDead();// make dead if overlaps with projectile
                    }
                    object[i]->makeDead(); // set the projectile dead
                    return true; // return true as projectile has died early
                }
            }
        }
    } 
    return false; // else return false
}




bool StudentWorld::inFrontCab(Actor* m_actor) { // check if there is any object in front of the zombie cab (excluding borderline) 
    double temp_y;
    double ghost_y;
    bool ghost = false;
    bool temp = false;
    if (getLane(racer()) == getLane(m_actor) && racer()->getY() > m_actor->getY()) {
       ghost_y = racer()->getY(); // keep track of ghost racer y value if ghost racer is in same lane as zombie cab and above it
       if (ghost_y - m_actor->getY() < 96)
           ghost = true; // tells us that there is ghost in the lane that is less than 96 pixels in front
    }
    for (int i = 0; i < object.size(); i++) {
        if (m_actor == object[i]) {
            if(i< object.size()-1)
            i++; // increment if actor is itself
        }
        if (object[i]->isCollision()) {
            if (getLane(object[i]) == getLane(m_actor) && object[i]->getY() > m_actor->getY()) {
                temp_y = object[i]->getY();
                if (temp_y - m_actor->getY() < 96)
                    temp = true; // tells us that there is collision avoidance actor in the lane that is less than 96 pixels in front
            }
        }
    }
    if (temp || ghost) {
        return true; // collision avoidance actor ahead in the lane that is closer than 96 pixels
    }
    else
        return false; // nothing to worry about ahead of zombie cab

}

bool StudentWorld::behindCab(Actor* m_actor) { // check if there is any object behind of the zombie cab (excluding borderline) 
    double temp_y;
    bool temp = false;
    for (int i = 0; i < object.size(); i++) { // loop to find actor closest behind cab
        if (m_actor == object[i]) {
            if (i < object.size() - 1)
                i++; // increment if actor is itself
        }
        if (object[i]->isCollision()) {
            if (getLane(object[i]) == getLane(m_actor) && object[i]->getY() < m_actor->getY()) {
                temp_y = object[i]->getY();
                if (temp_y - m_actor->getY() > -96)
                    temp = true; // tells us that there is collision avoidance actor in the lane that is less than 96 pixels in front
            }
        }
    }
    
    if (temp) {
        return true; // collision avoidance actor ahead in the lane that is closer than 96 pixels
    }
    else
        return false; // nothing to worry about ahead of zombie cab

}


void StudentWorld::setendLevel() {
    endLevel = true; // variable to track if level has ended (eg. human ped overlaps with ghostracer)
}

bool StudentWorld::getendLevel() { // getter function
    return endLevel; // return whether or not level has ended
}

void StudentWorld::setcompleteLevel() {
    completeLevel = true; // variable to track if level has been completed
}

bool StudentWorld::getcompleteLevel() { // getter function
    return completeLevel; // return whether or not level has been completed
}

int StudentWorld::getLane(Actor* m_actor) {
    if (m_actor->getX() < lane1right && m_actor->getX() >= lane1left) {
        return 1; // identifies which lane actor is in
    }
    if (m_actor->getX() < lane2right && m_actor->getX() >= lane2left) {
        return  2; // identifies which lane actor is in
    }
    if (m_actor->getX() < lane3right && m_actor->getX() >= lane3left) {
        return 3; // identifies which lane actor is in
    }
}

void StudentWorld::addZombieCabbelow() {
    
    int ChanceVehicle = max(100 - getLevel() * 10, 20);
    int random = randInt(0, ChanceVehicle - 1);
    Actor* temp = racer();
    if (random == 0) {
        int cur_lane = randInt(1, 3);
        double temp_y = racer()->getY(); // variable to track
        double temp_above_y = racer()->getY();
        for (int i = 1; i < 4; i++) {
            for (int g = 0; g < object.size(); g++) {
                if (object[g]->isCollision() && getLane(object[g]) == cur_lane) // if in same lane and is collision worthy actor
                    if (object[g]->getY() <= temp_y) { // check if object is lower than temp
                        temp_y = object[g]->getY(); // closest actor to the bottom of candidate lane
                        temp = object[g];
                    }
            }
            if (temp->isZombieCab())
                return; // don't add if there is already a zombie cab
            if (cur_lane != getLane(racer())) { // check if adding cabs under ghost racer
                if (temp_y == racer()->getY() || temp_y > VIEW_HEIGHT / 3) // check if there is enough room for zombie cab
                {
                    double ran_v = randInt(2, 4);
                    double v = racer()->Speed() + ran_v;
                    if (cur_lane == 1) {
                        object.push_back(new ZombieCab(lane1middle, SPRITE_HEIGHT / 2, this, v)); // add zombie cab to lane 1 if cur_lane = 1
                        return;
                    }
                    if (cur_lane == 2) {
                        object.push_back(new ZombieCab(lane2middle, SPRITE_HEIGHT / 2, this, v)); // add zombie cab to lane 2 if cur_lane = 2
                        return;
                    }
                    if (cur_lane == 3) {
                        object.push_back(new ZombieCab(lane3middle, SPRITE_HEIGHT / 2, this, v)); // add zombie cab to lane 3 if cur_lane = 3
                        return;
                    }
                }
            }
            else { // increment cur_lane if no zombie cab was added
                if (cur_lane == 1) {
                    cur_lane = 2;
                }
                if (cur_lane == 2) {
                    cur_lane = 3;
                }
                if (cur_lane == 3) {
                    cur_lane = 1;
                }
            }
        }
    }
    return; // no zombies cab added
}

void StudentWorld::addZombieCababove() {

    int ChanceVehicle = max(100 - getLevel() * 10, 20); // variable to see if zombie cab will be spawned
    int random = randInt(0, ChanceVehicle - 1); // variable to see if zombie cab will be spawned 
    Actor* temp = racer(); // temp variable to track object closest to the top 
    if (random == 0) {
        int cur_lane = randInt(1, 3);
        double temp_y = racer()->getY(); // variable to track
        double temp_above_y = racer()->getY();
        for (int g = 0; g < object.size(); g++) {
            if (object[g]->isCollision() && getLane(object[g]) == cur_lane) // if in same lane and is collision worthy actor
                if (object[g]->getY() >= temp_above_y) { // check if object is greater than temp
                    temp_above_y = object[g]->getY(); // closest actor to the top of candidate lane
                    temp = object[g];
                }
        }
        if (temp->isZombieCab())
            return; // do not add zombie cab if there is one already there
        if (temp_above_y < VIEW_HEIGHT * 2 / 3)
        {
            double ran_v = randInt(2, 4);
            double v = racer()->Speed() - ran_v;
            if (cur_lane == 1) {
                object.push_back(new ZombieCab(lane1middle, VIEW_HEIGHT - SPRITE_HEIGHT / 2, this, v)); // add zombie cab to lane 1 if cur_lane = 1
                return;
            }
            if (cur_lane == 2) {
                object.push_back(new ZombieCab(lane2middle, VIEW_HEIGHT - SPRITE_HEIGHT / 2, this, v)); // add zombie cab to lane 2 if cur_lane = 2
                return;
            }
            if (cur_lane == 3) {
                object.push_back(new ZombieCab(lane3middle, VIEW_HEIGHT - SPRITE_HEIGHT / 2, this, v)); // add zombie cab to lane 3 if cur_lane = 3
                return;
            }
        }
        else { // increment cur_lane if no zombie cab was added
            if (cur_lane == 1) {
                cur_lane = 2;
            }
            if (cur_lane == 2) {
                cur_lane = 3;
            }
            if (cur_lane == 3) {
                cur_lane = 1;
            }
        }
    }
}

void StudentWorld::addOilSlick() {
    int ChanceOilSlick = max(150 - (getLevel()*10),40);
    int random = randInt(0, ChanceOilSlick - 1); // random int
    int ran_x = randInt(LEFT_EDGE, RIGHT_EDGE); // random x position on road
    if (random == 0) {
        object.push_back(new OilSlick(ran_x, VIEW_HEIGHT, randInt(2, 5), this));// add new oil slick at random x on road and y = view_height and add to vector
    }
    return;
}

void StudentWorld::addZombiePed() {
    int ChanceZombiePed = max(100 - (getLevel() * 10), 20);
    int random = randInt(0, ChanceZombiePed - 1); // random int
    int ran_x = randInt(LEFT_EDGE, RIGHT_EDGE); // random x position on road
    if (random == 0) {
        object.push_back(new ZombiePed(this, ran_x, VIEW_HEIGHT));// add new zombie ped at random x on road and y = view_height and add to vector
    }
    return;
}

void StudentWorld::addHumanPed() {
    int ChanceHumanPed = max(200 - (getLevel() * 10), 30);
    int random = randInt(0, ChanceHumanPed - 1); // random int
    int ran_x = randInt(LEFT_EDGE, RIGHT_EDGE); // random x position on road
    if (random == 0) {
        object.push_back(new HumanPed(this, ran_x, VIEW_HEIGHT)); // add new human ped at random x on road and y = view_height and add to vector
    }
    return;
}

void StudentWorld::addHolyWaterRefill() {
    int ChanceHolyWater = 100 + 10 * getLevel();
    int random = randInt(0, ChanceHolyWater - 1);// random int
    int ran_x = randInt(LEFT_EDGE, RIGHT_EDGE); // random x position on road
    if (random == 0) {
        object.push_back(new HolyWater(ran_x, VIEW_HEIGHT, this));// add new holy water at random x on road and y = view_height and add to vector
    }
    return;
}

void StudentWorld::addLostSoul() {
    int ChanceLostSoul = 100;
    int random = randInt(0, ChanceLostSoul - 1); // random variable from 0 to 99
    int ran_x = randInt(LEFT_EDGE, RIGHT_EDGE); // random x position on road
    if (random == 0) { // if random is 0
        object.push_back(new SoulGood(ran_x, VIEW_HEIGHT, this)); // add new lost soul at random x on road and y = view_height and add to vector
    }
    return;
}

void StudentWorld::externalAddOil(double x, double y) {
    object.push_back(new OilSlick(x, y, randInt(2, 5), this)); // tells world to add new oil slick at specified location (actor.cpp uses this)
    return;
}

void StudentWorld::externalAddHealingGood(double x, double y) {
    object.push_back(new HealingGood(x, y, this)); // tells world to add new healing goodie at specified location (actor.cpp uses this)
    return;
}

void StudentWorld::externalAddHolyWaterProj(double x, double y, int dir) {
    object.push_back(new HolyWaterProj(x, y, this, dir)); // tells world to add new healing goodie at specified location (actor.cpp uses this)
    return;
}

void StudentWorld::reset() {
    setSoulsSaved(0); // set souls saved by ghost racer back to 0
    setLevelSouls(2 * (getLevel() + 1) + 5); // reset souls2save to the next level requirement
    racer()->setHoly(10); // reset holy spray
    increaseScore(bonus); // increase score by bonus
    racer()->setHealth(100); // racer gets 100 health again
    bonus = 5000; // reset bonus to 5000
}

void StudentWorld::reset_dead() {
    setSoulsSaved(0); // set souls saved by ghost racer back to 0
    setLevelSouls(2 * (getLevel()) + 5); // reset souls2save to the next level requirement
    racer()->setHoly(10); // reset holy spray
    decLives(); // decrease lives by 1
    racer()->setHealth(100); // racer gets 100 health again
    bonus = 5000; // reset bonus to 5000
}
int StudentWorld::move()
{
    if (getSouls() == 2 * getLevel() + 5) {
        reset(); // reset lives, bonus, holy sprays, and souls saved
        return GWSTATUS_FINISHED_LEVEL; // end the level
    }
    m_racer->doSomething();
    vector<Actor*>::iterator it; // iterator to loop
    it = object.begin();
    int count = 0;
    if (!racer()->isAlive()) { // check if ghost racer is alive
        reset_dead(); // reset lives, bonus, holy sprays, and souls saved
        return GWSTATUS_PLAYER_DIED;
    }
    while (it != object.end()) {
        if ((*it)->isAlive() && (*it) != nullptr)
        {
            (*it)->doSomething();
            it++;
            if (!racer()->isAlive()) { // end level if racer has died
                reset(); // reset lives, bonus, holy sprays, and souls saved
                return GWSTATUS_PLAYER_DIED; // return player died
            }
            if (getendLevel()) {
                endLevel = false; // reset variable to track if level has ended
                return GWSTATUS_PLAYER_DIED; // return level has ended
            }
            if (getcompleteLevel()) {
                return GWSTATUS_FINISHED_LEVEL; // return level completed
            }
        }
        else
            it++;
    }
    if (!racer()->isAlive()) { // end level if racer has died
        bonus = 5000; // reset bonus to 5000
        return GWSTATUS_PLAYER_DIED; // return player died
    }
    if (getendLevel()) {
        reset_dead(); // reset lives, bonus, holy sprays, and souls saved
        return GWSTATUS_PLAYER_DIED; // return level has ended
    }
    if (getcompleteLevel()) {
        reset();
        return GWSTATUS_FINISHED_LEVEL; // return level completed
    }

    // remove all dead objects
    it = object.begin();
    while (it != object.end()) {
        if (!(*it)->isAlive())
        {
            delete* it;
            it = object.erase(it);
            count++;
        }
        else
            it++;
    }

    // adding new borderlines to spots where borderlines have died
    int white = 0;
    for (int i = 0; i < object.size(); i++) {
        if (object[i] != nullptr) {
            if (object[i]->getDepth() == 2.0 && (object[i]->getX() == LEFT_EDGE + (ROAD_WIDTH / 3) || object[i]->getX() == RIGHT_EDGE - (ROAD_WIDTH / 3)) && !object[i]->isOilSlick() && !object[i]->isGoodie()) {
                white = i;
            }
        }
    }
    white_last = object[white]->getY();
    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    double delta_y = new_border_y - (white_last);
    if (delta_y >= SPRITE_HEIGHT)
    {
        object.push_back(new Borderline(this, IID_YELLOW_BORDER_LINE, LEFT_EDGE, new_border_y));
        object.back()->doSomething();
        object.push_back(new Borderline(this, IID_YELLOW_BORDER_LINE, RIGHT_EDGE, new_border_y));
        object.back()->doSomething();
    }
    if (delta_y >= 4 * SPRITE_HEIGHT) {
        object.push_back(new Borderline(this, IID_WHITE_BORDER_LINE, LEFT_EDGE + (ROAD_WIDTH / 3), new_border_y));
        object.back()->doSomething();
        object.push_back(new Borderline(this, IID_WHITE_BORDER_LINE, RIGHT_EDGE - (ROAD_WIDTH / 3), new_border_y));
        object.back()->doSomething();
    }

    // adding new actors as required

    // adding zombie cabs
    addZombieCabbelow();
    addZombieCababove();

    // add oil slicks
   addOilSlick();

    // add Zombie Ped
    addZombiePed();

    // add human ped
    addHumanPed();

    // add holy water refill
    addHolyWaterRefill();

    // add lost soul goodies
    addLostSoul();


    if (!racer()->isAlive()) { // end level if racer has died
        reset_dead(); // reset lives, bonus, holy sprays, and souls saved
        return GWSTATUS_PLAYER_DIED;
    }
    if(bonus >= 0)
        bonus--; // decrement bonus by 1 every tick

    // setting game text at top of the screen
    ostringstream oss;
    oss.setf(ios::fixed);
    oss << "Score:" << setw(2) << getScore() << setw(6) << "Lvl:" << setw(2) << getLevel() << setw(13) << "Souls2Save:" << setw(2) << getLevelSouls() << setw(7) << "Lives:" << setw(2) << getLives() << setw(9) << "Health:" << setw(2) << racer()->getHealth() << setw(9) << "Sprays:" << setw(3) << racer()->getHoly() << setw(8) << "Bonus:" << setw(5) << bonus; // stringseam with data
    setGameStatText(oss.str()); // display the stringseam


    return GWSTATUS_CONTINUE_GAME; // continue game
}

StudentWorld::~StudentWorld() {
    cleanUp(); // cleanup function will delete all objects that were dynamically created
}

void StudentWorld::cleanUp()
{
    delete m_racer; // delete Ghost Racer
    for (int i = object.size() - 1; i >= 0; i--)
    {
        if (object[i] != nullptr)
        {
            delete object[i]; // delete each object in the vector (border lines)
            object.pop_back(); // pop from the vector
        }
    }
    return;
}
