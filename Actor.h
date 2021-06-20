#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// constant doubles for lane values

const double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
const double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;

const double lane1left = ROAD_CENTER - ROAD_WIDTH / 2;
const double lane1right = LEFT_EDGE + (ROAD_WIDTH / 3);

const double lane2left = LEFT_EDGE + (ROAD_WIDTH / 3);
const double lane2right = RIGHT_EDGE - (ROAD_WIDTH / 3);

const double lane3left = RIGHT_EDGE - (ROAD_WIDTH / 3);
const double lane3right = ROAD_CENTER + ROAD_WIDTH / 2;

const double lane1middle = ROAD_CENTER - ROAD_WIDTH / 3;
const double lane2middle = ROAD_CENTER;
const double lane3middle = ROAD_CENTER + ROAD_WIDTH / 3;

const double PI = 3.14159265359;

class StudentWorld;


class Actor : public GraphObject {
public:
	Actor(int imageID, double xstart, double ystart, int startDirection, double size, int depth, StudentWorld* w);

	virtual void doSomething() = 0; // virtual function

	bool isAlive(); // accessor function
	void makeDead(); // mutator function

	
	double hSpeed(); // accessor function
	void hincSpeed(double amt);// mutator function
	void hdecSpeed(double amt);// mutator function
	void sethSpeed(double amt);// mutator function
	double vSpeed(); // accessor function
	void vincSpeed(double amt);// mutator function
	void vdecSpeed(double amt);// mutator function
	void setvSpeed(double amt);// mutator function
	
	double getDepth(); // accessor function
	bool overlap(Actor* actor2);// accessor function
	void move();// mutator function
	bool offScreen();// accessor function

	bool isCollision();// accessor function
	void setCollision();// mutator function
	bool isProj();// accessor function
	void setProj();// mutator function
	bool isAffectedProj();// accessor function
	void setAffectedProj();// mutator function
	bool isDamageable();// accessor function
	void setDamageable();// mutator function
	bool isZombieCab(); // accessor function
	void setZombieCab();// mutator function
	bool isZombiePed(); // accessor function
	void setZombiePed();// mutator function
	bool isGhost(); // accessor function
	void setGhost();// mutator function
	bool isOilSlick(); // accessor function
	void setOilSlick();// mutator function
	bool isGoodie(); // accessor function
	void setGoodie();// mutator function
	bool isSoulGoodie();
	void setSoulGoodie();

	int getHealth() const; // accessor function
	void decHealth(int amt);// mutator function
	void incHealth(int amt);// mutator function
	void setHealth(int amt);// mutator function

	int getMovementPlan() const; // accessor function
	void decMovementPlan(int amt);// mutator function
	void incMovementPlan(int amt);// mutator function
	void setMovementPlan(int amt);// mutator function
	StudentWorld* world() const; // accessor function to access studentworld
private:
	StudentWorld* m_world;
	int m_health;
	bool m_alive = true;
	bool m_collision = false;
	double hspeed;
	double vspeed;
	double m_depth;
	bool proj = false;
	bool affectedProj = false;
	bool damageable = false;
	int movementPlan;
	bool zombieCab = false;
	bool m_Ghost = false;
	bool m_oilSlick = false;
	bool m_Goodie = false;
	bool zombie_ped = false;
	bool soulgoodie = false;
};

class GhostRacer : public Actor {
public:
	GhostRacer(StudentWorld* m_world,double xstart, double ystart );
	virtual void doSomething();
	void GhostMovement();
	int getHoly();
	void decHoly(int amt);
	void incHoly(int amt);
	void setHoly(int amt);
	void spin();
	void damageRacer(int amt);
	double Speed(); // accessor function
	void incSpeed(double amt); // mutator function
	void decSpeed(double amt); // mutator function
	void setSpeed(double amt); // mutator function
private:
	int holy_spray;
	double speed;
};

class Borderline : public Actor {
public:
	Borderline(StudentWorld* m_world,int imageID, double xstart, double ystart );
	virtual void doSomething();
private:
	
};

class Ped : public Actor {
public:
	Ped(int imageID, StudentWorld* m_world, double xstart, double ystart,double size);
	virtual void doSomething() =0;
	 void newMovementPlan();
};

class HumanPed : public Ped {
public:
	HumanPed(StudentWorld* m_world, double xstart, double ystart);
	virtual void doSomething();
};

class ZombiePed : public Ped {
public:
	ZombiePed( StudentWorld* m_world, double xstart, double ystart);
	virtual void doSomething();
	int getGrunt();
	void setGrunt(int amt);
	void decGrunt(int amt);
	void incGrunt(int amt);
private:
	int gruntTicks;
};

class ZombieCab : public Actor {
public:
	ZombieCab(double xstart, double ystart, StudentWorld* m_world, double vspeed);
	virtual void doSomething();
	void damageGhost();
	bool getDamageGhost();
private:
	bool damage = false;
};

class OilSlick : public Actor {
public:
	OilSlick(double xstart, double ystart, double size, StudentWorld* m_world);
	virtual void doSomething();
};

class Goodie : public Actor {
public:
	Goodie(int imageID, double xstart, double ystart, StudentWorld* m_world, double size, int dir);
	virtual void doSomething() = 0;
};

class HealingGood : public Goodie {
public:
	HealingGood(double xstart, double ystart, StudentWorld* m_world);
	virtual void doSomething();
};

class SoulGood : public Goodie {
public:
	SoulGood(double xstart, double ystart, StudentWorld* m_world);
	virtual void doSomething();
};

class HolyWater : public Goodie {
public:
	HolyWater(double xstart, double ystart, StudentWorld* m_world);
	virtual void doSomething();
};

class HolyWaterProj : public Actor {
public:
	HolyWaterProj(double xstart, double ystart, StudentWorld* m_world, int dir);
	virtual void doSomething();
	void increaseDistanceTraveled();
	double getDistance();
private:
	double distanceTraveled = 0;
};

#endif // ACTOR_H_
