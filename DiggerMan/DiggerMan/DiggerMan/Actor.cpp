#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor
Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world) : GraphObject(imageID, startX, startY, dir, size, depth)
{
	m_world = world;
}

Actor::~Actor()
{
}

//Dirt
Dirt::Dirt(int startX, int startY, StudentWorld* world) : Actor(IMID_DIRT, startX, startY, right, 0.25, 3, world)
{
	setVisible(true);
}

Dirt::~Dirt()
{
}

//DiggerMan
DiggerMan::DiggerMan(int startX, int startY, StudentWorld* world) : Actor(IMID_PLAYER, startX, startY, right, 1, 0, world)
{
	m_health = 10;
	m_water = 5;
	m_sonar = 1;
	setVisible(true);
}

void DiggerMan::doSomething()
{
	Direction d;
	int ch;
	d = getDirection();

	if (getWorld()->getKey(ch) == true)
	{
		switch (ch)
		{
		case KEY_PRESS_ESCAPE: //
		{
			alive = false;
			break;
		}
		case 'q': case 'Q': //
		{
			exit(0);
			break;
		}
		case 'z': case 'Z':{ // sonar gets activated
			if (m_sonar > 0){
				getWorld()->useSonar(getX(), getY());
				m_sonar--;
			}
			break;
		}
		case KEY_PRESS_SPACE: // shoots a squirt
		{
			if (m_water > 0) {
				getWorld()->makeSquirt();
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
				m_water--;
			}
			break;
		}
		case KEY_PRESS_TAB: // diggerMan drops a gold nugget
		{
			if (m_nuggets != 0)
			{
				getWorld()->dropGold();
				m_nuggets--;
			}
			break;
		}

		case KEY_PRESS_LEFT:
			// move one step to the left after checking if there's any boulder at the postion the diggerman is about to move to. After the diggerman moves, remove the dirt at the postion the diggerman has moved to
		{
			if (d != left)
			{
				setDirection(left);
				break;
			}
			if (getX() - 1 >= 0)
			{

				if (!getWorld()->boulderCheck(getX() - 1, getY()))
					moveTo(getX() - 1, getY());
			}
			getWorld()->removeDirt(getX(), getY());
			break;
		}
		case KEY_PRESS_RIGHT:
			// move one step to the right after checking if there's any boulder at the postion the diggerman is about to move to. After the diggerman moves, remove the dirt at the postion the diggerman has moved to
		{
			if (d != right)
			{
				setDirection(right);
				break;
			}
			if (getX() + 1 <= 60)
			{

				if (!getWorld()->boulderCheck(getX() + 1, getY()))
					moveTo(getX() + 1, getY());
			}
			getWorld()->removeDirt(getX(), getY());
			break;

		}
		case KEY_PRESS_DOWN:
			// move one step down after checking if there's any boulder at the postion the diggerman is about to move to.After the diggerman moves, remove the dirt at the postion the diggerman has moved to
		{
			if (d != down)
			{
				setDirection(down);
				break;
			}
			if (getY() - 1 >= 0)
			{
				if (!getWorld()->boulderCheck(getX(), getY() - 1))
					moveTo(getX(), getY() - 1);
			}
			getWorld()->removeDirt(getX(), getY());
			break;
		}
		case KEY_PRESS_UP:
			// move one step up after checking if there's any boulder at the postion the diggerman is about to move to.After the diggerman moves, remove the dirt at the postion the diggerman has moved to
		{
			if (d != up)
			{
				setDirection(up);
				break;
			}
			if (getY() + 1 <= 60)
			{
				//if (!getWorld()->isBoulder(getX(), getY() + 4))
				//	moveTo(getX(), getY() + 1);
				if (!getWorld()->boulderCheck(getX(), getY() + 1))
					moveTo(getX(), getY() + 1);
			}
			getWorld()->removeDirt(getX(), getY());
			break;
		}

		}
	}
}

void DiggerMan::Annoy(int damage)
{
	if (alive)
	{
		//std::cout << "annoyed for " << damage << " points!\n";
		m_health -= damage;
		if (m_health <= 0)
			alive = false;
	}
}

DiggerMan::~DiggerMan()
{
}

int DiggerMan::getHealth()
{
	return m_health;
}

int DiggerMan::getBarrel()
{
	return m_barrel;
}

int DiggerMan::getNugget()
{
	return m_nuggets;
}

int DiggerMan::getSonar()
{
	return m_sonar;
}

int DiggerMan::getWater()
{
	return m_water;
}

void DiggerMan::setBarrel()
{
	m_barrel++;
}

void DiggerMan::setNuggets()
{
	m_nuggets++;
}

void DiggerMan::setSonar()
{
	m_sonar++;
}

void DiggerMan::setWater()
{
	m_water += 5;
}

//Boulder
Boulder::Boulder(int startX, int startY, StudentWorld* world) : Actor(IMID_BOULDER, startX, startY, down, 1, 1, world)
{
	setVisible(true);
}


void Boulder::doSomething()
{
	// when alive:
	if (alive == true)
	{
		if (stable == true) // there is a dirt below the boulder
		{
			stable = getWorld()->isStable(getX(), getY());
		}
		if (stable == false && waiting < 30) // if all the dirts below the boulder are removed, waiting starts to increase by one
		{
			waiting++;
			//cout << waiting << endl;
		}
		if (waiting == 30) // when the waiting reaches 30, the boulder falls
		{

			if (!getWorld()->isStable(getX(), getY()) && !getWorld()->boulderCheck(getX(), getY()))
			{
//				cout << "falling!" << endl;
				moveTo(getX(), getY() - 1);
				getWorld()->playSound(SOUND_FALLING_ROCK);

				// when the diggerMan gets bonked by the boulder, damage diggerMan by 100
				if (getWorld()->diggerCheck(getX(), getY(), 3))
				{
					getWorld()->annoyDiggerMan(100);
				}

				int n = getWorld()->regularCheckDamage(getX(), getY(), 3, 100, false); //amount of protesters killed


				if (n > 0) // when a protester/protesters are bonked by the boulder, increase the score by 500 * num of protesters that are bonked
				{
					getWorld()->increaseScore(500 * n);
				}
			}

			
			else if (getWorld()->isStable(getX(), getY()) || getWorld()->boulderCheck(getX(), getY()))
			{
//				cout << "boulder being set to dead" << endl;
				alive = false;
			}
		}
	}

}

Boulder::~Boulder()
{

}

//GOLD NUGGET
GoldNugget::GoldNugget(int startX, int startY, bool temp, StudentWorld* world) : Actor(IMID_GOLD, startX, startY, right, 1, 2, world)
{
	// temporary = dropped gold nugget. should disappear after 100 ticks
	temporary = temp;
	if (temp == true)
		setVisible(true);
	else
		setVisible(false);
}

void GoldNugget::doSomething()
{
	if (alive)
	{
		// when the tick hasn't reached 100 after the gold nugget has dropped by diggerman
		if (waiting != 100 && temporary)
			waiting++;

		// when the tick has reached 100 after the gold nugget has dropped by diggerman
		if (waiting == 100 && temporary)
			alive = false;

		// if the gold nugget is dropped by diggerman
		if (temporary){

			//if the gold nugget is picked up by a regular protester, increase the score by 25 and make the gold nugget disappear
			if (getWorld()->protesterCheck(getX(), getY(), 3) == "regular")
			{
				alive = false;
				getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
				getWorld()->increaseScore(25);

			}

			//if the gold nugget is picked up by a hardcore protester, increase the score by 50 and make the gold nugget disappear
			else if (getWorld()->protesterCheck(getX(), getY(), 3) == "hardcore")
			{
				alive = false;
				getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
				getWorld()->increaseScore(50);
			}
		}

		//when diggerman is within radius of 4 from the gold nugget, its visibility changes to true
		if (getWorld()->diggerCheck(getX(), getY(), 4) && !temporary)
		{
			setVisible(true);

			// if the diggerman picks up the gold nugget,increase the score by 10, increase the number of gold nuggets diggerman has by one, and set the gold nugget to dead
			if (getWorld()->diggerCheck(getX(), getY(), 3))
			{
				alive = false;
				getWorld()->playSound(SOUND_GOT_GOODIE);
				getWorld()->increaseScore(10);
				getWorld()->setDiggerNuggets();
			}
		}
	}
}

GoldNugget::~GoldNugget()
{

}

//BARREL
Barrel::Barrel(int startX, int startY, StudentWorld* world) : Actor(IMID_BARREL, startX, startY, right, 1, 2, world)
{
	setVisible(false);
}

void Barrel::doSomething()
{
	if (alive)
	{
		//when diggerman is w/i 4 units from the barrel of oil, make its visibility to true;
		if (getWorld()->diggerCheck(getX(), getY(), 4))
		{
			setVisible(true);

			//if diggerman picks up the oil, increase score by 1000, increase the number of oil diggerman has by one, and set the barrel of oil to dead
			if (getWorld()->diggerCheck(getX(), getY(), 3))
			{
				alive = false;
				getWorld()->playSound(SOUND_FOUND_OIL);
				getWorld()->increaseScore(1000);
				getWorld()->setDiggerBarrels();
			}
		}

	}
}

Barrel::~Barrel()
{
}

//SONAR KIT
SonarKit::SonarKit(int startX, int startY, StudentWorld* world) : Actor(IMID_SONAR, startX, startY, right, 1, 2, world)
{
	setVisible(true);
}

void SonarKit::doSomething()
{

	int level = getWorld()->getLevel();

	// T = number of ticks that the sonar kit can stay in the oil field
	unsigned int T = max(100, 300 - 10 * level);

	if (alive)
	{
		waiting++;

		// since waiting has passed the number of ticks that the sonar kit could stay in the oil field, the sonar kit disappears and set it to dead
		if (waiting > T)
		{
			alive = false;
		}

		// when diggerman picks up the sonar kit, increase the score by 75, increase the number of sonar kits diggerman has by one, and set the sonar kit to dead
		if (getWorld()->diggerCheck(getX(), getY(), 3))
		{
			alive = false;
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->increaseScore(75);
			getWorld()->setDiggerSonar();
		}
	}
}

SonarKit:: ~SonarKit()
{

}

//WATER POOL

WaterPool::WaterPool(int startX, int startY, StudentWorld* world) : Actor(IMID_WATER_POOL, startX, startY, right, 1, 2, world)
{
	setVisible(true);
}

void WaterPool::doSomething()
{
	int level = getWorld()->getLevel();

	// T = number of ticks that the wataer pool can stay in the oil field
	unsigned int T = max(100, 300 - 10 * level);

	if (alive)
	{

		waiting++;

		// since waiting has passed the number of ticks that the water pool could stay in the oil field, the sonar kit disappears and set it to dead
		if (waiting > T)
		{
			alive = false;
		}

		//if diggerman picks up the water pool, increase the score by 100,increase the number of squirt diggerman has, and set the water pool to dead
		if (getWorld()->diggerCheck(getX(), getY(), 3))
		{
			alive = false;
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->increaseScore(100);
			getWorld()->setDiggerSquirt();
		}
	}
}

WaterPool::~WaterPool()
{

}

//Squirt
Squirt::Squirt(int startX, int startY, Direction dir, StudentWorld* world) : Actor(IMID_WATER_SPURT, startX, startY, dir, 1, 1, world)
{
	m_distance = 4;
	setVisible(true);
}

void Squirt::doSomething()
{
	if (alive)
	{
		if (m_distance == 0)
		{
			alive = false;
		}
		else
		{

			int n = getWorld()->regularCheckDamage(getX(), getY(), 3, 2, true);

			if (n > 0)
			{
				alive = false;
				return;
			}

			// when shoot squirt up
			if (getDirection() == up)
			{
//				cout << "DISTANCE UP: " << m_distance << endl;

				// if the squirt is blocked by a boulder/ dirt, set the squirt to dead
				if (getWorld()->checkObject(getX(), getY()) == true)
					alive = false;

				else // move one step up
				{
					moveTo(getX(), getY() + 1);
					m_distance--;
				}
			}
			else if (getDirection() == down)
			{
//				cout << "DISTANCE DOWN: " << m_distance << endl;

				// if the squirt is blocked by a boulder/ dirt, set the squirt to dead
				if (getWorld()->checkObject(getX(), getY()) == true)
					alive = false;

				else // move one step down
				{
					moveTo(getX(), getY() - 1);
					m_distance--;
				}
			}
			else if (getDirection() == left)
			{
//				cout << "DISTANCE LEFT: " << m_distance << endl;

				// if the squirt is blocked by a boulder/ dirt, set the squirt to dead
				if (getWorld()->checkObject(getX(), getY()) == true)
					alive = false;
				else // move left
				{
					moveTo(getX() - 1, getY());
					m_distance--;
				}
			}
			else if (getDirection() == right)
			{
//				cout << "DISTANCE RIGHT: " << m_distance << endl;

				// if the squirt is blocked by a boulder/ dirt, set the squirt to dead
				if (getWorld()->checkObject(getX(), getY()) == true)
					alive = false;

				else// move right
				{
					moveTo(getX() + 1, getY());
					m_distance--;
				}
			}
		}
	}
}

Squirt:: ~Squirt()
{

}

//Regular Protestor
RegularProtester::RegularProtester(int startX, int startY, StudentWorld *world) : Actor(IMID_PROTESTER, startX, startY, left, 1, 0, world)
{
	setVisible(true);
	m_health = 5;
	leaveOilField = false;
	int level = getWorld()->getLevel();
	ticksToWait = max(0, 3 - level / 4);
	ticksToAnnoy = 15;
	ticksPerpendicular = 200;
}

void RegularProtester::doSomething()
{
	//cout << "health" << m_health << endl;
	if (alive)
	{
		//rest state
		if (ticksToWait > 0)
		{
			ticksToWait--;
		}

		//non-rest state
		else if (ticksToWait == 0)
		{
			//regular protester doesn't shout unless ticksToAnnoy = 0;
			if (ticksToAnnoy != 0)
				ticksToAnnoy--;


			if (ticksPerpendicular != 0)
				ticksPerpendicular--;

			// if regular protester's hit point reaches 0, change the state of leaveOilField to true
			if (m_health <= 0 && leaveOilField == false)
			{
				ticksToWait = 0;
				leaveOilField = true;
			}

			else if (leaveOilField) // when leaveOilField is true
			{
				// when the regular proteter is already at top right corner, make it dead
				if (getX() == 60 && getY() == 60) {
					alive = false;
					return;
				}
				else
				{
					//call exit function here
                    /******DOESN'T WORK*********/
//                string moveDirection;
//                moveDirection = getWorld() -> moveToLeave(getX(), getY());
//                if (moveDirection == "right")
//                {
//                    if (getX() + 1 <= 60)
//                    {
//                        moveTo(getX()+1, getY());
//                        return;
//                    }
//                }
//                else if (moveDirection == "left")
//                {
//                    if(getX() - 1 >= 0)
//                    {
//                        moveTo(getX()-1, getY());
//                        return;
//                    }
//                }
//                else if (moveDirection == "up")
//                {
//                    if (getY() + 1 <= 0)
//                    {
//                        moveTo(getX(), getY()+1);
//                        return;
//                    }
//                }
//                else if (moveDirection == "down")
//                {
//                    if(getY() -1 >= 0)
//                    {
//                        moveTo(getX(), getY()-1);
//                        return;
//                    }
//                }
                }
			}

			// when diggerman is w/i 3 units from the protester and the regular protester is looking at the diggerman
			else if (getWorld()->diggerCheck(getX(), getY(), 3) && correctDirection() == true)
			{
				//cout << "Attempting to annoy DiggerMan" << endl;
				//cout << ticksToAnnoy << endl;

				// shouts to diggerMan since TicksToAnnoy = 0 and reset ticksToAnnoy and ticksToWait.
				if (ticksToAnnoy == 0)
				{
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					getWorld()->annoyDiggerMan(2);
					ticksToAnnoy = 15;
					int level = getWorld()->getLevel();
					ticksToWait = max(0, 3 - level / 4);
					return;
				}

			}

			// when diggerman and the protester are located on the same x/y axis
			else if (getX() == getWorld()->getDiggerManX() || getY() == getWorld()->getDiggerManY())
			{
				// if diggerman is w/i 3 units from the regular protester
				if (getWorld()->diggerCheck(getX(), getY(), 3) == false)
				{
					// if there's no object in the path between the diggerman and the protester, change the protester's direction
					if (getWorld()->hvObjectCheck(getX(), getY()) == false)
					{
						changeDirection();

						// if the diggerman is not in line of sight, set numSquares to 0 so the regular protetser changes its direction.
						if (!lineOfSight()) {
							numSquares = 0;
						}

					}
				}
			}
			else
			{
				// if the protester's steps to move have left, meaning not 0
				if (numSquares != 0)
				{
					numSquares--;
				}

			}

			//if the protester can no longer move to the direction it's facing right now, pick a new direction
			if (numSquares <= 0)
			{
				PickNewDirection();
			}

			
			if (ticksPerpendicular <= 0) {
				PickPerpendicularDir();
                ticksPerpendicular = 200;
			}

			//after all, make the regular protester to move one step to the currently facing direction
			if (getDirection() == up)
			{
				if (getY() + 1 <= 60 && getWorld()->checkObject(getX(), getY() + 1) == false)
				{
					moveTo(getX(), getY() + 1);
				}
				else numSquares = 0;
			}

			else if (getDirection() == down)
			{
				if (getY() - 1 >= 0 && getWorld()->checkObject(getX(), getY() - 1) == false)
				{
					moveTo(getX(), getY() - 1);
				}
				else numSquares = 0;
			}
			else if (getDirection() == left)
			{
				if (getX() - 1 >= 0 && getWorld()->checkObject(getX() - 1, getY()) == false)
				{
					moveTo(getX() - 1, getY());
				}
				else numSquares = 0;
			}
			else if (getDirection() == right)
			{
				if (getX() + 1 <= 60 && getWorld()->checkObject(getX() + 1, getY()) == false)
				{
					moveTo(getX() + 1, getY());
				}
				else numSquares = 0;

			}

			//when the regular protester is done with its job during non-rest state, reset tickToWait
			int level = getWorld()->getLevel();
			ticksToWait = max(0, 3 - level / 4);

		}
	}
}

void RegularProtester::PickPerpendicularDir(){

	Direction dir = getDirection();
	int ranDir = rand() % 2;
	bool turnUp = false;
	bool turnDown = false;
	bool turnRight = false;
	bool turnLeft = false;
	if (getWorld()->checkObject(getX(), getY() + 1) == false && dir != up && dir != down)
		if (getY() + 1 <= 60)
			turnUp = true;
	if (getWorld()->checkObject(getX(), getY() - 1) == false && dir != up && dir != down)
		if (getY() - 1 >= 0)
			turnDown = true;
	if (getWorld()->checkObject(getX() - 1, getY()) == false && dir != left && dir != right)
		if (getX() - 1 >= 0)
			turnLeft = true;
	if (getWorld()->checkObject(getX() + 1, getY()) == false && dir != up && dir != down)
		if (getX() + 1 <= 60)
			turnRight = true;


	// check which direction perpendicular and should turn
	if (dir == up || dir == down)
	{
		if (turnRight && turnLeft)
		{
			if (ranDir == 0)
			{
				setDirection(right);
				numSquares = rand() % 60 + 8;
			}
			else
			{
				setDirection(left);
				numSquares = rand() % 60 + 8;
			}
		}
		else if (turnRight)
		{
			setDirection(right);
			numSquares = rand() % 60 + 8;
		}
		else if (turnLeft)
		{
			setDirection(left);
			numSquares = rand() % 60 + 8;
		}
	}
	else if (dir == right || dir == left)
	{
		if (turnUp && turnDown)
		{
			if (ranDir == 0)
			{
				setDirection(up);
				numSquares = rand() % 60 + 8;
			}
			else
			{
				setDirection(right);
				numSquares = rand() % 60 + 8;
			}
		}
		else if (turnUp)
		{
			setDirection(up);
			numSquares = rand() % 60 + 8;
		}
		else if (turnDown)
		{
			setDirection(down);
			numSquares = rand() % 60 + 8;
		}
	}
	if (dir != getDirection())
		ticksPerpendicular = 0;
}

void RegularProtester::PickNewDirection(){
	bool changeDir = false;
	while (changeDir == false)
	{
		int RandDir = rand() % 4; // 0 = up, 1 = down, 2 = left, 4 = right
		if (RandDir == 0)
		{
			if (getWorld()->checkObject(getX(), getY() + 1) == false) {
				if (getY() + 1 <= 60) {
					setDirection(up);
					numSquares = rand() % 60 + 8;
					changeDir = true;

				}
			}
			while (getWorld()->checkObject(getX(), getY() + 1) == true) //blocked by an object
			{
				RandDir = rand() % 4; //0 = up, 1 = down, 2= left , 3 = right
				if (RandDir != 0)
					break;
			}
		}
		if (RandDir == 1)
		{
			if (getWorld()->checkObject(getX(), getY() - 1) == false){
				if (getY() - 1 >= 0) {
					setDirection(down);
					numSquares = rand() % 60 + 8;
					changeDir = true;

				}
			}
			while (getWorld()->checkObject(getX(), getY() - 1) == true)
			{
				RandDir = rand() % 4; //0 = up, 1 = down, 2= left , 3 = right
				if (RandDir != 1)
					break;
			}
		}
		if (RandDir == 2)
		{
			if (getWorld()->checkObject(getX() - 1, getY()) == false){
				if (getX() - 1 >= 0) {
					setDirection(left);
					numSquares = rand() % 60 + 8;
					changeDir = true;

				}
			}
			while (getWorld()->checkObject(getX() - 1, getY()) == true)
			{
				RandDir = rand() % 4; //0 = up, 1 = down, 2= left , 3 = right
				if (RandDir != 2)
					break;
			}
		}
		if (RandDir == 3)
		{
			if (getWorld()->checkObject(getX() + 1, getY()) == false){
				if (getX() + 1 <= 60){
					setDirection(right);
					numSquares = rand() % 60 + 8;
					changeDir = true;

				}
			}
			while (getWorld()->checkObject(getX() + 1, getY()) == true)
			{
				RandDir = rand() % 4; //0 = up, 1 = down, 2= left , 3 = right
				if (RandDir != 3)
					break;
			}
		}
	}
}

bool RegularProtester::lineOfSight()
{
	bool lineOfSight = false;
	/*for (int i = -3; i <= 3 ; i++)
	{
	if (getX() + i == getWorld()->getDiggerManX() || getY() + i == getWorld()->getDiggerManY())

	{
	cout << "is in line of sight" << endl;
	lineOfSight = true;
	}
	}*/

	if (getWorld()->hvObjectCheck(getX(), getY()))
		lineOfSight = false;
	else
		lineOfSight = true;
	return lineOfSight;
}

bool RegularProtester::correctDirection()
{
	// Checks if a regular protester is facing towards DiggerMan

	// If DiggerMan is on the top left of Protester, and Protester is also facing up or left
	if (getWorld()->getDiggerManX() <= getX() && getWorld()->getDiggerManY() >= getY())
	{
		if (getDirection() == up || getDirection() == left)
			return true;
	}
	// If DiggerMan is on the top right of Protester, and Protester is also facing up or right
	else if (getWorld()->getDiggerManX() >= getX() && getWorld()->getDiggerManY() >= getY())
	{
		if (getDirection() == up || getDirection() == right)
			return true;
	}
	// If DiggerMan is on the bottom left of Protester, and Protester is also facing down or left
	else if (getWorld()->getDiggerManX() <= getX() && getWorld()->getDiggerManY() <= getY())
	{
		if (getDirection() == down || getDirection() == left)
			return true;
	}
	// If DiggerMan is on the bottom right of Protester, and Protester is also facing down or right
	else if (getWorld()->getDiggerManX() >= getX() && getWorld()->getDiggerManY() <= getY())
	{
		if (getDirection() == down || getDirection() == right)
			return true;
	}
	// If Protester is not facing DiggerMan, return false
	return false;


}

void RegularProtester::changeDirection()
{
	// If DiggerMan is on the top left of Protester
	if (getWorld()->getDiggerManX() <= getX() && getWorld()->getDiggerManY() >= getY())
	{
//		cout << "from top left" << endl;
//		cout << "diggerManX:" << getWorld()->getDiggerManX() << " diggerManY: " << getWorld()->getDiggerManY() << endl;
//		cout << getX() << " " << getY() << endl;

		if (getWorld()->getDiggerManX() == getX() && getWorld()->getDiggerManY() > getY()) {
			if (getY() + 1 <= 60 && !getWorld()->checkObject(getX(), getY() + 1))
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
		}
		else if (getWorld()->getDiggerManX() < getX() && getWorld()->getDiggerManY() == getY())
		{
			if (getX() - 1 >= 0 && !getWorld()->checkObject(getX() - 1, getY()))
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
		}
		/*	else if (getWorld()->getDiggerManX() < getX() && getWorld()->getDiggerManY() > getY())
		{
		if (getY() + 1 <= 60 && !getWorld()->checkObject(getX(), getY() +1 ))
		{
		setDirection(up);
		moveTo(getX(), getY() + 1);
		}
		}*/

	}
	// If DiggerMan is on the top right of Protester
	else if (getWorld()->getDiggerManX() >= getX() && getWorld()->getDiggerManY() >= getY())
	{
//		cout << "from top right" << endl;
//		cout << "diggerManX:" << getWorld()->getDiggerManX() << " diggerManY: " << getWorld()->getDiggerManY() << endl;
//		cout << getX() << " " << getY() << endl;
		if (getWorld()->getDiggerManX() == getX() && getWorld()->getDiggerManY() > getY())
		{
			if (getWorld()->getDiggerManY() >= getY() && getY() + 1 <= 60 && !getWorld()->checkObject(getX(), getY() + 1))
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
		}
		else if (getWorld()->getDiggerManX() > getX() && getWorld()->getDiggerManY() == getY())
		{
			if (getWorld()->getDiggerManX() >= getX() && getX() + 1 <= 60 && !getWorld()->checkObject(getX() + 1, getY()))
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
		}
		/*else if (getWorld()->getDiggerManX() > getX() && getWorld()->getDiggerManY() > getY())
		{
		if (getWorld()->getDiggerManX() >= getX() && getX() + 1 <= 60 && !getWorld()->checkObject(getX() + 1, getY()))
		{
		setDirection(right);
		moveTo(getX() + 1, getY());
		}
		}*/
	}
	// If DiggerMan is on the bottom left of Protester
	else if (getWorld()->getDiggerManX() <= getX() && getWorld()->getDiggerManY() <= getY())
	{
//		cout << "from bottom left" << endl;
//		cout << "diggerManX:" << getWorld()->getDiggerManX() << " diggerManY: " << getWorld()->getDiggerManY() << endl;
//		cout << getX() << " " << getY() << endl;
		if (getWorld()->getDiggerManX() == getX() && getWorld()->getDiggerManY() < getY())
		{
			if (getWorld()->getDiggerManY() <= getY() && getY() - 1 >= 0 && !getWorld()->checkObject(getX(), getY() - 1))
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
		}
		else if (getWorld()->getDiggerManX() < getX() && getWorld()->getDiggerManY() == getY())
		{
			if (getWorld()->getDiggerManX() <= getX() && getX() - 1 >= 0 && !getWorld()->checkObject(getX() - 1, getY()))
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
		}
		/*else if (getWorld()->getDiggerManX() < getX() && getWorld()->getDiggerManY() < getY())
		{
		if (getWorld()->getDiggerManX() <= getX() && getX() - 1 >= 0 && !getWorld()->checkObject(getX() - 1, getY()))
		{
		setDirection(left);
		moveTo(getX() - 1, getY());
		}
		}*/
	}
	// If DiggerMan is on the bottom right of Protester
	else if (getWorld()->getDiggerManX() >= getX() && getWorld()->getDiggerManY() <= getY())
	{
//		cout << "from bottom right" << endl;
//		cout << "diggerManX:" << getWorld()->getDiggerManX() << " diggerManY: " << getWorld()->getDiggerManY() << endl;
//
//		cout << getX() << " " << getY() << endl;
		if (getWorld()->getDiggerManX() == getX() && getWorld()->getDiggerManY() < getY())
		{
			if (getWorld()->getDiggerManY() <= getY() && getY() - 1 >= 0 && !getWorld()->checkObject(getX(), getY() - 1))
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
		}
		else if (getWorld()->getDiggerManX() > getX() && getWorld()->getDiggerManY() == getY())
		{
			if (getWorld()->getDiggerManX() >= getX() && getX() + 1 <= 60 && !getWorld()->checkObject(getX() + 1, getY()))
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
		}
		/*else if (getWorld()->getDiggerManX() > getX() && getWorld()->getDiggerManY() < getY())
		{
		if (getWorld()->getDiggerManY() <= getY() && getY() - 1 >= 0 && !getWorld()->checkObject(getX(), getY() - 1))
		{
		setDirection(down);
		moveTo(getX(), getY() - 1);
		}
		}*/
	}

}
//void RegularProtester::ticksWait(int ticks)
//{
//	ticksToWait = ticks;
//}

void RegularProtester::Annoy(int damage)
{
	if (alive)
	{
		//std::cout << "annoyed for " << damage << " points!\n";
		m_health -= damage;
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		int level = getWorld()->getLevel();
		int N = max(50, 100 - level * 10);

		ticksWait(N);

		if (m_health <= 0) {
			getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
			ticksToWait = 0;
			leaveOilField = true;
			alive = false;
		}
	}
}

RegularProtester::~RegularProtester()
{

}
HardcoreProtester::HardcoreProtester(int startX, int startY, StudentWorld *world) : Actor(IMID_HARD_CORE_PROTESTER, startX, startY, left, 1, 0, world)
{
	setVisible(true);
	m_health = 20;
	numSquares = rand() % 60 + 8;
	int level = getWorld()->getLevel();
	ticksToWait = max(0, 3 - level / 4);
	ticksToAnnoy = 15;
	ticksPerpendicular = 200;

}
void HardcoreProtester::doSomething()
{
	if (alive)
	{
		if (ticksToWait > 0)
		{
			ticksToWait--;
		}
		else if (ticksToWait == 0)
		{
			if (ticksToAnnoy != 0)
				ticksToAnnoy--;
			if (ticksPerpendicular != 0)
				ticksPerpendicular--;
			// if protester is full of annoyed, leave oil field
			if (m_health <= 0 && leaveOilField == false)
			{
				ticksToWait = 1;
				leaveOilField = true;
			}
			else if (leaveOilField)
			{
				if (getX() == 60 && getY() == 60)
				{
					alive = false;
					return;
				}
				else
				{
					//call exit function here
                    string moveDirection;
                    moveDirection = getWorld() -> moveToLeave(getX(), getY());
                    if (moveDirection == "right")
                    {
                        if (getX() + 1 <= 60)
                        {
                            moveTo(getX()+1, getY());
                            return;
                        }
                    }
                    else if (moveDirection == "left")
                    {
                        if(getX() - 1 >= 0)
                        {
                            moveTo(getX()-1, getY());
                            return;
                        }
                    }
                    else if (moveDirection == "up")
                    {
                        if (getY() + 1 <= 0)
                        {
                            moveTo(getX(), getY()+1);
                            return;
                        }
                    }
                    else if (moveDirection == "down")
                    {
                        if(getY() -1 >= 0)
                        {
                            moveTo(getX(), getY()+1);
                            return;
                        }
                    }
				}
			}
            
            
			//if protester within 4 units away and currently racing to the diggerman
			else if (getWorld()->diggerCheck(getX(), getY(), 4) && correctDirection())
			{
				if (ticksToAnnoy == 0)
				{
					getWorld()->playSound(SOUND_PROTESTER_YELL);
					getWorld()->annoyDiggerMan(2);
					ticksToAnnoy = 15;
					int level = getWorld()->getLevel();
					ticksToWait = max(0, 3 - level / 4);
					return;
				}
			}
			//else if (!getWorld()->diggerCheck(getX(), getY(), 4))
			//{
			//	//#5 do code here
			//}
			else if (getX() == getWorld()->getDiggerManX() || getY() == getWorld()->getDiggerManY())
			{
				if (getWorld()->diggerCheck(getX(), getY(), 3) == false)
				{
					if (getWorld()->hvObjectCheck(getX(), getY()) == false)
					{
						changeDirection();
						if (!lineOfSight()) {
							numSquares = 0;
						}

					}
				}
			}
			else
			{
				if (numSquares != 0)
				{
					numSquares--;
				}

			}
			if (numSquares <= 0)
			{
				PickNewDirection();
			}
			if (ticksPerpendicular <= 0) {
				PickPerpendicularDir();
			}
			//after all, make it move one step to the currently facing direction
			if (getDirection() == up)
			{
				if (getY() + 1 <= 60 && getWorld()->checkObject(getX(), getY() + 1) == false)
				{
					moveTo(getX(), getY() + 1);
				}
				else numSquares = 0;
			}

			else if (getDirection() == down)
			{
				if (getY() - 1 >= 0 && getWorld()->checkObject(getX(), getY() - 1) == false)
				{
					moveTo(getX(), getY() - 1);
				}
				else numSquares = 0;
			}
			else if (getDirection() == left)
			{
				if (getX() - 1 >= 0 && getWorld()->checkObject(getX() - 1, getY()) == false)
				{
					moveTo(getX() - 1, getY());
				}
				else numSquares = 0;
			}
			else if (getDirection() == right)
			{
				if (getX() + 1 <= 60 && getWorld()->checkObject(getX() + 1, getY()) == false)
				{
					moveTo(getX() + 1, getY());
				}
				else numSquares = 0;

			}
			int level = getWorld()->getLevel();
			ticksToWait = max(0, 3 - level / 4);

		}
	}
}

void HardcoreProtester::changeDirection()
{
	// If DiggerMan is on the top left of Protester
	if (getWorld()->getDiggerManX() <= getX() && getWorld()->getDiggerManY() >= getY())
	{
//		cout << "from top left" << endl;
//		cout << "diggerManX:" << getWorld()->getDiggerManX() << " diggerManY: " << getWorld()->getDiggerManY() << endl;
//		cout << getX() << " " << getY() << endl;
		if (getWorld()->getDiggerManX() == getX() && getWorld()->getDiggerManY() > getY()) {
			if (getY() + 1 <= 60 && !getWorld()->checkObject(getX(), getY() + 1))
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
		}
		else if (getWorld()->getDiggerManX() < getX() && getWorld()->getDiggerManY() == getY())
		{
			if (getX() - 1 >= 0 && !getWorld()->checkObject(getX() - 1, getY()))
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
		}

	}
	// If DiggerMan is on the top right of Protester
	else if (getWorld()->getDiggerManX() >= getX() && getWorld()->getDiggerManY() >= getY())
	{
//		cout << "from top right" << endl;
//		cout << "diggerManX:" << getWorld()->getDiggerManX() << " diggerManY: " << getWorld()->getDiggerManY() << endl;
//		cout << getX() << " " << getY() << endl;
		if (getWorld()->getDiggerManX() == getX() && getWorld()->getDiggerManY() > getY())
		{
			if (getWorld()->getDiggerManY() >= getY() && getY() + 1 <= 60 && !getWorld()->checkObject(getX(), getY() + 1))
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
		}
		else if (getWorld()->getDiggerManX() > getX() && getWorld()->getDiggerManY() == getY())
		{
			if (getWorld()->getDiggerManX() >= getX() && getX() + 1 <= 60 && !getWorld()->checkObject(getX() + 1, getY()))
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
		}

	}
	// If DiggerMan is on the bottom left of Protester
	else if (getWorld()->getDiggerManX() <= getX() && getWorld()->getDiggerManY() <= getY())
	{
//		cout << "from bottom left" << endl;
//		cout << "diggerManX:" << getWorld()->getDiggerManX() << " diggerManY: " << getWorld()->getDiggerManY() << endl;
//		cout << getX() << " " << getY() << endl;
		if (getWorld()->getDiggerManX() == getX() && getWorld()->getDiggerManY() < getY())
		{
			if (getWorld()->getDiggerManY() <= getY() && getY() - 1 >= 0 && !getWorld()->checkObject(getX(), getY() - 1))
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
		}
		else if (getWorld()->getDiggerManX() < getX() && getWorld()->getDiggerManY() == getY())
		{
			if (getWorld()->getDiggerManX() <= getX() && getX() - 1 >= 0 && !getWorld()->checkObject(getX() - 1, getY()))
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
		}

	}
	// If DiggerMan is on the bottom right of Protester
	else if (getWorld()->getDiggerManX() >= getX() && getWorld()->getDiggerManY() <= getY())
	{
//		cout << "from bottom right" << endl;
//		cout << "diggerManX:" << getWorld()->getDiggerManX() << " diggerManY: " << getWorld()->getDiggerManY() << endl;
//
//		cout << getX() << " " << getY() << endl;
		if (getWorld()->getDiggerManX() == getX() && getWorld()->getDiggerManY() < getY())
		{
			if (getWorld()->getDiggerManY() <= getY() && getY() - 1 >= 0 && !getWorld()->checkObject(getX(), getY() - 1))
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
		}
		else if (getWorld()->getDiggerManX() > getX() && getWorld()->getDiggerManY() == getY())
		{
			if (getWorld()->getDiggerManX() >= getX() && getX() + 1 <= 60 && !getWorld()->checkObject(getX() + 1, getY()))
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
		}

	}

}
//void HardcoreProtester::ticksWait(int ticks)
//{
//	ticksToWait = ticks;
//}
bool HardcoreProtester::lineOfSight()
{
	bool lineOfSight = false;
	if (getWorld()->hvObjectCheck(getX(), getY()))
		lineOfSight = false;
	else
		lineOfSight = true;
	return lineOfSight;
}
void HardcoreProtester::PickPerpendicularDir(){
	Direction dir = getDirection();
	int ranDir = rand() % 2;
	bool turnUp = false;
	bool turnDown = false;
	bool turnRight = false;
	bool turnLeft = false;
	if (getWorld()->checkObject(getX(), getY() + 1) == false && dir != up && dir != down)
		if (getY() + 1 <= 60)
			turnUp = true;
	if (getWorld()->checkObject(getX(), getY() - 1) == false && dir != up && dir != down)
		if (getY() - 1 >= 0)
			turnDown = true;
	if (getWorld()->checkObject(getX() - 1, getY()) == false && dir != left && dir != right)
		if (getX() - 1 >= 0)
			turnLeft = true;
	if (getWorld()->checkObject(getX() + 1, getY()) == false && dir != up && dir != down)
		if (getX() + 1 <= 60)
			turnRight = true;
	// check which direction perpendicular and should turn
	if (dir == up || dir == down)
	{
		if (turnRight && turnLeft)
		{
			if (ranDir == 0)
			{
				setDirection(right);
				numSquares = rand() % 60 + 8;
			}
			else
			{
				setDirection(left);
				numSquares = rand() % 60 + 8;
			}
		}
		else if (turnRight)
		{
			setDirection(right);
			numSquares = rand() % 60 + 8;
		}
		else if (turnLeft)
		{
			setDirection(left);
			numSquares = rand() % 60 + 8;
		}
	}
	else if (dir == right || dir == left)
	{
		if (turnUp && turnDown)
		{
			if (ranDir == 0)
			{
				setDirection(up);
				numSquares = rand() % 60 + 8;
			}
			else
			{
				setDirection(right);
				numSquares = rand() % 60 + 8;
			}
		}
		else if (turnUp)
		{
			setDirection(up);
			numSquares = rand() % 60 + 8;
		}
		else if (turnDown)
		{
			setDirection(down);
			numSquares = rand() % 60 + 8;
		}
	}
	if (dir != getDirection())
		ticksPerpendicular = 0;
}
void HardcoreProtester::PickNewDirection(){
	bool changeDir = false;
	while (changeDir == false)
	{
		int RandDir = rand() % 4; // 0 = up, 1 = down, 2 = left, 4 = right
		if (RandDir == 0)
		{
			if (getWorld()->checkObject(getX(), getY() + 1) == false) {
				if (getY() + 1 <= 60) {
					setDirection(up);
					numSquares = rand() % 60 + 8;
					changeDir = true;

				}
			}
			while (getWorld()->checkObject(getX(), getY() + 1) == true) //blocked by an object
			{
				RandDir = rand() % 4; //0 = up, 1 = down, 2= left , 3 = right
				if (RandDir != 0)
					break;
			}
		}
		if (RandDir == 1)
		{
			if (getWorld()->checkObject(getX(), getY() - 1) == false){
				if (getY() - 1 >= 0) {
					setDirection(down);
					numSquares = rand() % 60 + 8;
					changeDir = true;

				}
			}
			while (getWorld()->checkObject(getX(), getY() - 1) == true)
			{
				RandDir = rand() % 4; //0 = up, 1 = down, 2= left , 3 = right
				if (RandDir != 1)
					break;
			}
		}
		if (RandDir == 2)
		{
			if (getWorld()->checkObject(getX() - 1, getY()) == false){
				if (getX() - 1 >= 0) {
					setDirection(left);
					numSquares = rand() % 60 + 8;
					changeDir = true;

				}
			}
			while (getWorld()->checkObject(getX() - 1, getY()) == true)
			{
				RandDir = rand() % 4; //0 = up, 1 = down, 2= left , 3 = right
				if (RandDir != 2)
					break;
			}
		}
		if (RandDir == 3)
		{
			if (getWorld()->checkObject(getX() + 1, getY()) == false){
				if (getX() + 1 <= 60){
					setDirection(right);
					numSquares = rand() % 60 + 8;
					changeDir = true;

				}
			}
			while (getWorld()->checkObject(getX() + 1, getY()) == true)
			{
				RandDir = rand() % 4; //0 = up, 1 = down, 2= left , 3 = right
				if (RandDir != 3)
					break;
			}
		}
	}
}

bool HardcoreProtester::correctDirection()
{
	// Checks if a regular protester is facing towards DiggerMan

	// If DiggerMan is on the top left of Protester, and Protester is also facing up or left
	if (getWorld()->getDiggerManX() <= getX() && getWorld()->getDiggerManY() >= getY())
	{
		if (getDirection() == up || getDirection() == left)
			return true;
	}
	// If DiggerMan is on the top right of Protester, and Protester is also facing up or right
	else if (getWorld()->getDiggerManX() >= getX() && getWorld()->getDiggerManY() >= getY())
	{
		if (getDirection() == up || getDirection() == right)
			return true;
	}
	// If DiggerMan is on the bottom left of Protester, and Protester is also facing down or left
	else if (getWorld()->getDiggerManX() <= getX() && getWorld()->getDiggerManY() <= getY())
	{
		if (getDirection() == down || getDirection() == left)
			return true;
	}
	// If DiggerMan is on the bottom right of Protester, and Protester is also facing down or right
	else if (getWorld()->getDiggerManX() >= getX() && getWorld()->getDiggerManY() <= getY())
	{
		if (getDirection() == down || getDirection() == right)
			return true;
	}
	// If Protester is not facing DiggerMan, return false
	return false;


}
void HardcoreProtester::Annoy(int damage)
{
	if (alive)
	{
//		std::cout << "annoyed for " << damage << " points!\n";
		m_health -= damage;
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		int level = getWorld()->getLevel();
		int N = max(50, 100 - level * 10);

		ticksWait(N);

		if (m_health <= 0) {
			getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
			ticksToWait = 0;
			leaveOilField = true;
			alive = false;
		}
	}
}
HardcoreProtester::~HardcoreProtester()
{

}
