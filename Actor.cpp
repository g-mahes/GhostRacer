#define _USE_MATH_DEFINES
#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <cmath>
using namespace std;

/////////////////// Actor ////////////////////////////

Actor::Actor(int imageID, double xstart, double ystart, int startDirection, double size, int depth, StudentWorld* w) : GraphObject(imageID, xstart, ystart, startDirection, size, depth) {
	m_world = w; // specify which world actor is in
	m_depth = depth; // store depth in depth data member
}

StudentWorld* Actor::world() const {
	return m_world; // return pointer to world
}



double Actor::hSpeed() {
	return hspeed; // return actor horizontal speed
}

void Actor::hincSpeed(double amt) {
	hspeed += amt; // increase actor horizontal speed by specified amount
}

void Actor::hdecSpeed(double amt) {
	hspeed -= amt; // decrease actor horizontoal speed by specified amount
}

void Actor::sethSpeed(double amt) {
	hspeed = amt; // set actor horizontal speed to specified amount
}
double Actor::vSpeed() {
	return vspeed; // return actor vertical speed
}

void Actor::vincSpeed(double amt) {
	vspeed += amt; // increase actor vertical speed by specified amount
}

void Actor::vdecSpeed(double amt) {
	vspeed -= amt; // decrease actor vertical speed by specified amount
}
void Actor::setvSpeed(double amt) {
	vspeed = amt; // set actor vertical speed to specified amount
}

bool Actor::isAlive() {
	return m_alive; // return actor state of alive or dead
}

void Actor::makeDead() {
	m_alive = false; // turn actor to dead state
}

double Actor::getDepth() {
	return m_depth; // return depth of specified actor
}

bool Actor::isCollision() {
	return m_collision; // return true or false for whether actor is collision avoidance worthy actor
}

void Actor::setCollision() {
	m_collision = true;// set actor as collision avoidance worthy actor
}

bool Actor::overlap(Actor* actor2) { // check if two objects overlap
	double delta_x = abs(getX() - actor2->getX());
	double delta_y = abs(getY() - actor2->getY());
	double sum = getRadius() + actor2->getRadius();
	if (delta_x < sum * .25 && delta_y < sum * .6) { 
		return true; // return true if two actors are overlapping
	}
	else
		return false; // return false if no overlap
}

void Actor::move() { // move function for all actors
	double vert_speed = vSpeed() - world()->racer()->Speed(); // current vertical speed - Ghost Racer’s current vertical speed
	double horiz_speed = hSpeed(); // current  horizontal speed
	double new_y = getY() + vert_speed; // current y added to vert speed
	double new_x = getX() + horiz_speed; // current x added to horizontal speed
	moveTo(new_x, new_y); // actor will move to new position
}

bool Actor::offScreen() {
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) { // check if actor has gone off the screen
		makeDead(); // make that actor dead if it has gone off the screen
		return true; // return true
	}
	else
		return false; // return false if not off the screen
}

bool Actor::isProj() {
	return proj; // return whether actor is projectile
}

void Actor::setProj() {
	proj = true; // set proj to true
}

bool Actor::isAffectedProj() {
	return affectedProj; // return whether actor is affected by projectiles
}

void Actor::setAffectedProj() {
	affectedProj = true; // set acto as affected by projectiles
}

void Actor::setDamageable() {
	damageable = true; // set actor as damageable
}

bool Actor::isDamageable() {
	return damageable; // return whether actor is damageable
}

int Actor::getHealth() const {
	return m_health; // return integer of health
}

void Actor::decHealth(int amt) {
	m_health -= amt; //  decrease health by amt
}

void Actor::incHealth(int amt) {
	m_health += amt; // increase health by amt
}

void Actor::setHealth(int amt) {
	m_health = amt; // set health to amt
}

int Actor::getMovementPlan() const {
	return movementPlan; // return movement plan distance integer
}
void Actor::decMovementPlan(int amt) {
	movementPlan -= amt; // decrease movement plan distance by amt
}
void Actor::incMovementPlan(int amt) {
	movementPlan += amt; // increase movement plan distance by amt
}

void Actor::setMovementPlan(int amt) {
	movementPlan = amt; // set movement plan distance to amt
}

bool Actor::isZombieCab() {
	return zombieCab; // return whether or not actor is zombiecab
}
void Actor::setZombieCab() {
	zombieCab = true; // set actor as identifiable as zombie cab
}

bool Actor::isZombiePed() {
	return zombie_ped; // return whether or not actor is zombiecab
}
void Actor::setZombiePed() {
	zombie_ped = true; // set actor as identifiable as zombie cab
}

bool Actor::isGhost() {
	return m_Ghost; // return whether or not actor is ghost
}

void Actor::setGhost() {
	m_Ghost = true; // set ghost as true
}

bool Actor::isOilSlick() {
	return m_oilSlick; // return whether or not actor is oil slick
}

void Actor::setOilSlick() {
	m_oilSlick = true; // set oil slick as true
}

bool Actor::isGoodie() {
	return m_Goodie; // return whether or not actor is a goodie
}

void Actor::setGoodie() {
	m_Goodie = true; // set goodie as true
}

bool Actor::isSoulGoodie() {
	return soulgoodie;
}

void Actor::setSoulGoodie() {
	 soulgoodie=true;
}
////////////////////// GHOSTRACER //////////////////////////

GhostRacer::GhostRacer(StudentWorld* m_world,double xstart, double ystart) : Actor(IID_GHOST_RACER, 128, 32, 90, 4.0, 0, m_world) {
	setHealth(100); // ghostwater starts with 100 units of health
	setSpeed(0); // ghostracer starts with forward speed 0
	holy_spray = 10; // GhostRacer starts with 100 units of holy water spray goodie
	setCollision(); // ghostracer is  “collision avoidance-worthy actor”
	setDamageable(); // ghost racer can be damaged
	setGhost();
}

void GhostRacer::doSomething() {
	if (!isAlive()) // immediately return if ghostracer is not alive
		return;
	if (getX() <= LEFT_EDGE) // checks if ghost racer swerving towards left boundary
	{
		if (getDirection() > 90) // if facing toward the left side of the road
			decHealth(10); // damage itself by 10 hitpoints
		setDirection(82); // change direction away from boundary
		world()->playSound(SOUND_VEHICLE_CRASH); // play crash sound
		GhostMovement(); // ghost racer will move
	}
	if (getX() >= RIGHT_EDGE) { // check if racer is swerving towards right boundary
		if (getDirection() < 90) // if facing toward the right side of the road
			decHealth(10); // damage itself by 10 hitpoints
		setDirection(98); // change direction away from boundary
		world()->playSound(SOUND_VEHICLE_CRASH); // play crash sound
		GhostMovement(); // ghost racer will move
	}
	if (getHealth() <= 0)
		makeDead();
	int ch;
	if (world()->getKey(ch)) { // check if player pressed key
		switch (ch) {
		case KEY_PRESS_LEFT: // left key pressed
			if (getDirection() < 114) // if angle is less than 114 degrees
			{
				setDirection(getDirection() + 8); // increase direction by 8 degrees
				GhostMovement(); // ghost racer will move
				break;
			}
			break;
		case KEY_PRESS_RIGHT: // left key pressed
			if (getDirection() > 66) // if angle is greater than 66 degrees
			{
				setDirection(getDirection() - 8); // decrease direction by 8 degrees
				GhostMovement(); // ghost racer will move
				break;
			}
			break;
		case KEY_PRESS_UP: // up key pressed
			if (Speed() < 5) // if speed is less than 5, then we can increase speed
				incSpeed(1); // increase speed by 1
			GhostMovement(); // move ghost racer
			break;
		case KEY_PRESS_DOWN:
			if (Speed() > -1) // if speed is greater than -1, then we can decrease speed
				decSpeed(1); // decrease ghost racer speed by 1
			GhostMovement(); // move ghost racer
			break;
		case KEY_PRESS_SPACE:
			if (getHoly() > 0) {
				 // new holy water spray object sprite_height pixes ahead of ghost racer (same direction)
				world()->externalAddHolyWaterProj(getX() + (SPRITE_HEIGHT * cos(getDirection() * PI / 180)), getY() + (SPRITE_HEIGHT * sin(getDirection() * PI / 180)), world()->racer()->getDirection()); // add holy water projectile in direction that ghost racer is facing
				world()->playSound(SOUND_PLAYER_SPRAY); // play sound
				decHoly(1); // decrease holy srpay count by 1
				GhostMovement(); // move ghost racer
			}
		}
	}
	GhostMovement(); // move ghost racer
	return; // return from function
}

void GhostRacer::GhostMovement() { // function to simplify ghost racer movement (step 5 of do something)
	double max_shift_per_tick = 4.0; // variable for max shift per tick
	double direction = getDirection(); // direction in degrees of ghost racer
	double delta_x = cos(direction * PI / 180) * max_shift_per_tick; // delta x is cos of direction * max shift per tick
	double cur_x = getX(); // variable for ghost racer current x
	double cur_y = getY(); // variable for ghost racer current y
	moveTo(cur_x + delta_x, cur_y); // move ghost racer to new coords
}


int GhostRacer::getHoly() {
	return holy_spray; // return number of holy sprays ghost racer has 
}
void GhostRacer::decHoly(int amt) {
	holy_spray -= amt; // decrease number of holy sprays by amt
}

void GhostRacer::incHoly(int amt) {
	holy_spray += amt; // increase number of holy sprays by amt
}

void GhostRacer::spin() {
	int newDir = randInt(5,20); // variable to track new direction after spin
	int rotate = randInt(1, 2); // variable to see if car will rotate counter-clockwise or clockwise
	if (rotate == 1) { // spins counter-clockwise (add newDir)
		if (getDirection() + newDir > 120) {
			setDirection(120); // set direction to 120 if current direction + new dir is greater than 120
		}
		else
			setDirection(getDirection() + newDir);
	}
	if (rotate == 2) { // spin clockwise
		if (getDirection() - newDir < 60) {
			setDirection(60); // set direction to 60 if current direction - new dir is less than 60
		}
		else
			setDirection(getDirection() - newDir); // set new direction for ghostracer
	}
}

void GhostRacer::damageRacer(int amt) {
	decHealth(amt); // decrease health of ghost racer by amt
	if (getHealth() <= 0) { // if ghost racer health goes below 1
		makeDead(); // make ghost racer dead
		world()->playSound(SOUND_PLAYER_DIE); // play sound 
	}
}

void GhostRacer::setHoly(int amt) {
	holy_spray = amt; // set holy spray number to amt
}

double GhostRacer::Speed() {
	return speed; // return actor speed
}

void GhostRacer::incSpeed(double amt) {
	speed += amt;  // increase actor speed by specified amount
}

void GhostRacer::decSpeed(double amt) {
	speed -= amt; // decrease actor speed by specified amount
}

void GhostRacer::setSpeed(double amt) {
	speed = amt; // set actor speed to specified amount
}

/////////////////// BORDERLINE /////////////////


Borderline::Borderline(StudentWorld* m_world,int imageID, double xstart, double ystart ) : Actor(imageID, xstart, ystart, 0, 2.0, 2, m_world) {
	setvSpeed(-4); // sets vertical speed to -4
	sethSpeed(0); //set horizontal speed to 0
}


void Borderline::doSomething() {
	move(); // move the borderline
	offScreen(); // check if it has gone off the screen
}

///////////////// PEDESTRIANS ///////////////

Ped::Ped(int imageID, StudentWorld* m_world, double xstart, double ystart, double size) :Actor(imageID, xstart,ystart,0,size,0, m_world){
	sethSpeed(0); // horizontal speed is 0
	setvSpeed(-4); // vertical speed is -4
	setCollision(); // all pedestrians are collision avoidance actors
	setHealth(2); // pedestrians start with 2 health points
	setMovementPlan(0); // pedestrians start with 0 movement plan distance
	setAffectedProj();
}

void Ped::newMovementPlan() {
	int temp = 0; // variable to track random int
	while (temp == 0) { // keeps looping if random int is not from -3 to 3 excluding 0
		temp = randInt(-3, 3); // assigns random int to temp
	}
	sethSpeed(temp); // horizontal speed gets the random int from above
	int temp2 = randInt(4, 32); // variable that gets random int from 4 to 32
	setMovementPlan(temp2); // movement plan distance gets random int from 4 to 32
	if (hSpeed() < 0) { // if ped horizontal speed is less than 0, change direction
		setDirection(180); // ped direction changed to 180
	}
	else if (hSpeed() > 0) { // if horizontal speed is positive
		setDirection(0); // set direction to 0
	}
}

HumanPed::HumanPed(StudentWorld* m_world, double xstart, double ystart) : Ped(IID_HUMAN_PED, m_world, xstart,ystart,2.0){	
	setvSpeed(-4); // set vertical speed to -4
	sethSpeed(0); // set horizontal speed to 0
	setCollision(); // set as collision avoidance actor
	setHealth(2);// set health to 2
	setMovementPlan(0); // set movement plan distance to 0
}



ZombiePed::ZombiePed(StudentWorld* m_world, double xstart, double ystart) : Ped(IID_ZOMBIE_PED, m_world, xstart, ystart, 3.0) {
	setGrunt(0); // starts with 0 ticks till it grunts
	setvSpeed(-4); // vertical speed of -4
	sethSpeed(0); // horizontal speed of 0
	setHealth(2); // set health to 2
	setMovementPlan(0); // set movement plan distance to 0
	setCollision();	// is a collision avoidance worthy actor
	setDamageable(); // can take damage
	setZombiePed();
}

void HumanPed::doSomething() {
	if (!isAlive())
		return;
	if (overlap(world()->racer())) { // check if human ped overlaps with ghost racer
		world()->decLives();// decrease player lives if overlap
		world()->setendLevel(); // end level if overlap
		return; // return from function
	}
	move(); // move to new location
	if (offScreen()) {
		return; // remove human ped if it goes off the screen and return 
	}
	decMovementPlan(1);
	if (getMovementPlan() > 0) {
		return; // return if movementplan distance is greater than 0
	}
	else {
		newMovementPlan(); // new movement plan for human ped
	}
	if (world()->ProjOverlap(this)) { // if it is hit by a projectile
		int temp = hSpeed();
		temp = -1 * temp;
		sethSpeed(temp); // horizontal speed is changed to opposite direction
		if (getDirection() == 180) {
			setDirection(0); // change directions
		}
		else
			setDirection(180); // change directions
		world()->playSound(SOUND_PED_HURT); //play this sound in the game
	}
}

void ZombiePed::doSomething(){
	if (!isAlive())
			return; // return if zombie ped is not alive
	move(); // move to new location
	if (offScreen())
		return; // do nothing if went off the screen (kills object if so)
	if (overlap(world()->racer())) {
		world()->racer()->damageRacer(5);// 5 points of damage if overlap
		decHealth(2); // zombie ped loses two hitpoints
		if (getHealth() <= 0) { // if zombie ped does not have anymore health
			makeDead(); // zombie ped is set to not alive
			world()->playSound(SOUND_PED_DIE); // sound is played for zombie ped dying
			world()->increaseScore(150); // player receives 150 points
		}
		else {
			world()->playSound(SOUND_PED_HURT); // if not dead, then zombie ped is hurt, so hurt sound will play
		}
		return; // does nothing else
	}
	double xDiff = world()->racer()->getX() - getX();
	xDiff = abs(xDiff);
	if ((0 >= xDiff >= 30) && getY() > world()->racer()->getY()) { //If the zombie pedestrian’s X coordinate is within 30 pixels of Ghost Racer’s X coordinate, either left or right and zombie ped is in front of ghost racer
		setDirection(270);
		if (getX() < world()->racer()->getX()) // if zombie ped is to the left of ghost racer
		{
			sethSpeed(1); // zombie ped horizontal speed is set to 1
		}
		else if (getX() > world()->racer()->getX()) { // if zombie ped is to the right
			sethSpeed(-1); // zombie ped horizontal speed is set to -1
		}
		else if (getX() == world()->racer()->getX()) { // if zombie ped and ghost racer have same x
			sethSpeed(0); // zombie ped horizontal speed is set to 0
		}
		decGrunt(1); // number of ticks till grunt is decreased by 1
		if (getGrunt() <= 0) { // if it is time for zombie ped to grunt
			world()->playSound(SOUND_ZOMBIE_ATTACK); // play zombie attack sound
			setGrunt(20); // zombie ped grunt tick set to 20
		}
	}
	
	if (getMovementPlan() > 0) { // if movement plan distance is greater than 0
		decMovementPlan(1); // decrease movement plan distance
		return; // return after decreasing
	}
	else {
		newMovementPlan(); // new movement plan for zombie ped
	}
	if (world()->ProjOverlap(this)) {
		decHealth(1); // decrement health by 1
		if (getHealth() <= 0) { // if health is below 0
			makeDead(); // make zombie dead
			world()->playSound(SOUND_PED_DIE);
			if (!overlap(world()->racer())) {
				int chance; // variable to track chance that zombie ped will leave something behind after dying
				chance = randInt(1, 5); // 1 in 5 chance that something will be dropped
				if (chance == 1) { // if the probability is a success
					world()->externalAddHealingGood(getX(), getY());// new healing good left at zombie ped's location
				}
			}
			world()->increaseScore(150); // increase score 
		}
		else {
			world()->playSound(SOUND_PED_HURT);
		}
	}
}

int ZombiePed::getGrunt() {
	return gruntTicks; // return ticks until zombie grunts next
}
void ZombiePed::setGrunt(int amt)
{
	gruntTicks = amt; // set gruntticks to amt
}
void ZombiePed::decGrunt(int amt) {
	gruntTicks -= amt; // decrement gruntTicks by amt
}
void ZombiePed::incGrunt(int amt) {
	gruntTicks += amt; // increase the gruntTicks by amt
}
//////////////// GOODIES ///////////////////

Goodie::Goodie(int imageID, double xstart, double ystart, StudentWorld* m_world, double size, int dir) : Actor(imageID, xstart, ystart, dir, size, 2, m_world) {
	sethSpeed(0); // horizontal speed is 0 for all goodies
	setvSpeed(-4); // vertical speed is -4 for all goodies
	setGoodie(); // all goodies are identified as goodies
}

HealingGood::HealingGood(double xstart, double ystart, StudentWorld* m_world) : Goodie(IID_HEAL_GOODIE, xstart, ystart, m_world, 1.0, 0) {
	setAffectedProj(); // healing goodie is affected by projectiles
} // constructor for healing goodie that makes a new goodie with size 1.0, direction 0, and depth 2

void HealingGood::doSomething() {
	move();
	if (offScreen()) {
		return;
	}
	if (overlap(world()->racer())) { // if overlap with ghost racer
		if (world()->racer()->getHealth() + 10 > 100) {
			int remain = 100 - world()->racer()->getHealth();
			world()->racer()->incHealth(remain); // increase holy spray by enough to get to 100
		}
		else
			world()->racer()->incHealth(10); // increase holy spray by 10
		makeDead();  // make dead so it will get removed
		world()->playSound(SOUND_GOT_GOODIE); // make dead so it will get removed
		world()->increaseScore(250); // increase score
	}
	if (world()->ProjOverlap(this)) {
		makeDead(); // make dead if overlaps with projectile
		return;
	}
}

HolyWater::HolyWater(double xstart, double ystart, StudentWorld* m_world) : Goodie(IID_HOLY_WATER_GOODIE, xstart, ystart, m_world, 2.0, 90) {
	setAffectedProj(); // holy water goodie is affected by projectiles
}

void HolyWater::doSomething() {
	move(); // moves holy water to new position
	if (offScreen()) {
		return;
	} // removes holy waters that go off screen
	if (overlap(world()->racer())) { // if overlap with ghost racer
		world()->racer()->incHoly(10);
		makeDead(); // make dead so it will get removed
		world()->playSound(SOUND_GOT_GOODIE);
		world()->increaseScore(50); // increase score
	}
	if (world()->ProjOverlap(this)) {
		makeDead();// make dead if overlaps with projectile
		return;
	}
}

SoulGood::SoulGood(double xstart, double ystart, StudentWorld* m_world) : Goodie(IID_SOUL_GOODIE, xstart, ystart, m_world, 4.0, 0) {
	setSoulGoodie();
}

void SoulGood::doSomething() {
	move(); // move soul goodie to new location
	if (offScreen()) {
		return;
	} // check if soul goodie goes off the screen and 
	if (overlap(world()->racer())) {
		world()->incSouls(1); // tell world soul saved
		makeDead(); // make soul object dead
		world()->playSound(SOUND_GOT_SOUL); // play sound 
		world()->increaseScore(100); // increase players score by 100
	}
	else {
		setDirection(getDirection() - 10); // if no overlap, then rotate direction 10 degrees clockwise
	}
}


///////////////// HOLY WATER PROJECTILE ///////////////

HolyWaterProj::HolyWaterProj(double xstart, double ystart, StudentWorld* m_world, int dir) : Actor(IID_HOLY_WATER_PROJECTILE, xstart, ystart, dir, 1.0, 1, m_world) {
	setProj(); // Holy water is a projectile
}

void HolyWaterProj::doSomething() {
	if (!isAlive())
		return; // return immediately if dead
	if (world()->ProjActivate()) {// see if holy water is activited and acts accordingly		
	} 
	else {
		moveForward(SPRITE_HEIGHT); // move forward if no overlap with object
	}
	if (offScreen()) { // check if it has gone off screen
		return;
	}
	increaseDistanceTraveled(); // increase pixels traveled by 8 every tick
	if (getDistance() >= 160) { // make dead if it has traveled more than 160 pixels
		makeDead();
	}
}

void HolyWaterProj::increaseDistanceTraveled() {
	distanceTraveled += 8;
}

double HolyWaterProj::getDistance() {
	return distanceTraveled;
}


//////////////////// OIL SLICK //////////////////////////

OilSlick::OilSlick(double xstart, double ystart, double size, StudentWorld* m_world) :Actor(IID_OIL_SLICK, xstart, ystart, 0,size,2,m_world){
	sethSpeed(0); // starts with 0 horizontal speed
	setvSpeed(-4); // starts with -4 vertical speed
	setOilSlick(); // identifies as oil slick
}

void OilSlick::doSomething() { // function that does something for oil slick
	move(); // move to new location
	if(offScreen()) {
		return; // check if oilslick goes offscreen
	}
	if (overlap(world()->racer())) { // check to see if ghost racer overlaps with oil slick
		world()->playSound(SOUND_OIL_SLICK);// play sound if overlap
		world()->racer()->spin(); // spin ghost racer
	}
}



///////////////////// ZOMBIE CAB //////////////////////////

ZombieCab::ZombieCab(double xstart, double ystart, StudentWorld* m_world, double vspeed) :Actor(IID_ZOMBIE_CAB, xstart, ystart, 90,4.0,0,m_world){
	setCollision(); // zombie cab is collision avoidance worthy actor
	setHealth(3); // starts with 3 health
	setMovementPlan(0); // starts with movement plan distance of 0
	sethSpeed(0); // horizontal speed is 0
	setvSpeed(vspeed); // vertical speed is Ghost Racer’s current speed PLUS a random integer between 2 and 4 inclusive
	setZombieCab(); // zombie cabs identifier
	setAffectedProj(); // zombie cabs are affected by projectiles
}

void ZombieCab::doSomething() {
	if (!isAlive())
		return; // do nothing if it is not alive

	if (overlap(world()->racer())) { // check if zombie cab overlaps with ghost racer (skips to move() if it does not overlap)
		if (getDamageGhost()) { // check if zombie cab has damaged ghost racer
		}
		else { // has not damaged ghost racer yet
			world()->playSound(SOUND_VEHICLE_CRASH);
			world()->racer()->damageRacer(20);
			if (getX() <= world()->racer()->getX()) { // zombie is to the left of ghost racer or same x
				sethSpeed(-5); // zombie cab gets horizontal speed of -5
				setDirection(120 + randInt(0, 19)); // zombie cab has new direction of 120 + random int between [0,20)
			}
			if (getX() > world()->racer()->getX()) { // zombie cab is to the right of ghost racer
				sethSpeed(5); // horizontal speed is 5
				setDirection(60 - randInt(0, 19)); // new direction of 60 - random integer from 0 to 19
			}
			damageGhost(); // mark that ghost racer has been damaged
		}
	}

	move(); // move to new location
	if (offScreen()) {
		return;
	} // check if it has gone off screen

	if (vSpeed() > world()->racer()->Speed()) {// check if cab is moving up the screen
		if (world()->inFrontCab(this)) { // check if there is object in front of zombie cab in same lane and less than 96 vertical pixels in front
			vdecSpeed(.5); // decrease verical speed by .5
			return; // return after decreasing speed
		}
	}
	if (vSpeed() <= world()->racer()->Speed()) { // if speed is same or slower than zombie cab
		if (world()->behindCab(this)) {
			vincSpeed(.5); // increase cab vertical speed by .5
			return; // return after increasing speed
		}
	}
	decMovementPlan(1); // decrement zombie cabs movement plan distance by one
	if (getMovementPlan() > 0) {
		return; // return if movementplan distance is greater than 0
	}
	else { // if movementplan distance is less than or equal to 0
		setMovementPlan(randInt(4, 32)); // change movement plan distance to random integer from 4 to 32
		setvSpeed(vSpeed() + randInt(-2, 2)); // change vertical speed to current vertical speed + random integer from -2 to 2
	}
	if (world()->ProjOverlap(this)) { // check if projectile overlaps with zombie cab
		decHealth(1); // decrease health by 1
		if (getHealth() <= 0) { // see if health is 0 or below
			makeDead(); // make dead if health reaches below 1
			world()->playSound(SOUND_VEHICLE_DIE); // play sound
			int oilchance = randInt(1, 5); // variable to see if new oil slick will be left behind
			if (oilchance == 1) { // if 1 in 5 chance occurs
				world()->externalAddOil(getX(), getY()); // spawn new oil slick where zombie cab died
				world()->increaseScore(200); // increase player score
				return;
			}
		}
		else { // not dead with projectile
			world()->playSound(SOUND_VEHICLE_HURT); // plays sound
		}
	}

}

void ZombieCab::damageGhost() {
	damage = true; // set this as true when ghost racer is damaged
}

bool ZombieCab::getDamageGhost() {
	return damage; // return whether or not ghost has been damaged
}