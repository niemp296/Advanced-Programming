#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
using namespace std;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
private:
    struct Node
    {
        int x, y;
        Node() {}
        Node(int num1, int num2)
        {
            x = num1;
            y = num2;
        }
        int getX()
        {
            return x;
        }
        int getY()
        {
            return y;
        }
    };
    Dirt* m_dirt[VIEW_WIDTH][VIEW_HEIGHT];
	int shortestPath[VIEW_WIDTH][VIEW_HEIGHT];
    DiggerMan* m_diggerMan;
    vector <Actor*> actors;
	int level = getLevel();
	//protester
	int m_ticksProtester = max(25, 200 - level);
	int m_maxProtester = min(15, 2 + level*2);
	int m_probOfHardcore = min(90, level * 10 + 30);
	int m_numProtester = 0;
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {
    }
    
	int objectRadius(int x1, int y1, int x2, int y2);
	bool isStable(int x, int y);
	bool boulderCheck(int x, int y);
	bool diggerCheck(int x, int y, int r);
	int  regularCheckDamage(int x, int y, int r, int damge, bool Squirt); // if damaged by squirt - true, else false
	std::string protesterCheck(int x, int y, int r);
	bool checkObject(int x, int y);
	bool hvObjectCheck(int x, int y);

	void makeSonarWater();
	void makeSquirt();
	void dropGold();
	void removeDirt(int X, int Y);
	void removeDeadActors();
	void useSonar(int x, int y);
	

	int getDiggerManX();
	int getDiggerManY();

	void annoyRegularPros(int damage);
	


	void annoyDiggerMan(int damage);
	void setDiggerBarrels();
	void setDiggerNuggets();
	void setDiggerSonar();
	void setDiggerSquirt();
	int diggerManStatus();

	void setDisplayText();
	string text(int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft, int score);


	void searchShortestPath( int proX, int proY);
    string moveToLeave(int x, int y);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
    virtual void Ask_all_actors_to_do_something();
    
    
    
    
};

#endif // STUDENTWORLD_H_
