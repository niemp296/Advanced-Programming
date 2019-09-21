#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <algorithm>
#include <cmath>


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
//class GameWorld;

class Actor: public GraphObject
{
private:
    StudentWorld* m_world;
protected:
    int m_health;
    bool alive = true;bool leaveOilField = false;
    int ticksToWait;
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world);
    virtual void doSomething() = 0;
	virtual void Annoy(int damage) = 0;
	bool getAlive() { return alive; }
	int getHealth() { return m_health; }
    virtual ~Actor();
    StudentWorld* getWorld() { return m_world; }
    void ticksWait(int ticks) { ticksToWait = ticks;}
    void setleaveOilField(bool leave) {leaveOilField = leave;}
};

class Dirt: public Actor
{
public:
    Dirt(int startX, int startY, StudentWorld* world);
    virtual void doSomething() {}
	void Annoy(int damage) {}
    virtual ~Dirt();
    
};

class Boulder : public Actor
{
private:
    bool stable = true;
    int waiting = 0;
public:
    Boulder(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
	void Annoy(int damage) {}
    virtual ~Boulder();
};

class DiggerMan: public Actor
{
private:
    int m_nuggets = 0;
	int m_sonar = 1;
	int m_water = 0;
	int m_barrel = 0;
public:
    DiggerMan(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
	void Annoy(int damage);
    virtual ~DiggerMan();

	int getHealth();
	int getBarrel();
	int getNugget();
	int getSonar();
	int getWater();

	void setBarrel();
    void setNuggets();
	void setSonar();
	void setWater();
};

class GoldNugget : public Actor
{
private:
    bool temporary;
    int waiting = 0;
public:
    GoldNugget(int startX, int startY, bool temp, StudentWorld* world);
    virtual void doSomething();
	void Annoy(int damage) {}
    virtual ~GoldNugget();
};

class Barrel: public Actor
{
private:
public:
    Barrel(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
	void Annoy(int damage) {}
    virtual ~Barrel();
};

class SonarKit: public Actor
{
private:
    int waiting = 0;
public:
    SonarKit(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
	void Annoy(int damage) {}
    virtual ~SonarKit();
};

class WaterPool : public Actor
{
private:
	int waiting = 0;
public:
	WaterPool(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
	void Annoy(int damage) {}
	virtual ~WaterPool();
};

class Squirt : public Actor
{
private:
	int m_distance;
public:
	Squirt(int startX, int startY, Direction dir, StudentWorld* world);
	virtual void doSomething();
	void Annoy(int damage) {}
	virtual ~Squirt();
};

class RegularProtester : public Actor
{
private:
	//bool leaveOilField = false;
	int numSquares;
	//int ticksToWait;
	int ticksToAnnoy;
	int ticksPerpendicular;
public:
	RegularProtester(int startX, int startY, StudentWorld *world);
	virtual void doSomething();
	virtual void Annoy(int damage);
	void PickPerpendicularDir();
	void PickNewDirection();
	bool correctDirection();
	void changeDirection();
	bool lineOfSight();
	//void ticksWait(int ticks);
	virtual ~RegularProtester();
};
class HardcoreProtester : public Actor
{
private:
	//bool leaveOilField = false;
	int numSquares;
	//int ticksToWait;
	int ticksToAnnoy;
	int ticksPerpendicular;
public:
	HardcoreProtester(int startX, int startY, StudentWorld *world);
	virtual void doSomething();
	virtual void Annoy(int damage);
	void PickPerpendicularDir();
	void PickNewDirection();
	bool correctDirection();
	void changeDirection();
	bool lineOfSight();
	//void ticksWait(int ticks);
	virtual ~HardcoreProtester();

};



 
#endif // ACTOR_H_
