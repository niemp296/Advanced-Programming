#include "StudentWorld.h"
#include <string>
using namespace std;

int StudentWorld::init()
{	//PROTESTER
    actors.push_back(new RegularProtester(60, 60, this));
    //DIRT
    for (int y = 0; y < VIEW_HEIGHT - 4; y++)
    {
        for (int x = 0; x < VIEW_WIDTH; x++)
        {
            m_dirt[x][y] = new Dirt(x, y, this);
            if (y < 4)
                m_dirt[x][y]->setVisible(true);
            
            if ((x >= 30 && x <= 33) && y >= 4) {
                if (m_dirt[x][y] != nullptr)
                {
                    delete m_dirt[x][y];
                    m_dirt[x][y] = nullptr;
                    
                }
                
            }
            
        }
    }
    
    //DiggerMan
    m_diggerMan = new DiggerMan(30, 60, this);
    
    
    //Boulder
    int level = getLevel();
    int B = min(level / 2 + 2, 7);
    
    while (B > 0)
    {
        int r = rand() % (VIEW_HEIGHT - 7);
        int c = rand() % (VIEW_WIDTH - 3);
        
        if ((c < 27 && r >= 20 && r <= 56) || (c > 36 && r >= 20 && r <= 56))
        {
            bool isObject = false;
            for (auto p = actors.begin(); p != actors.end(); p++)
            {
                int x = (*p)->getX();
                int y = (*p)->getY();
                // distance between each boulder is 6 radius
                if (objectRadius(c, r, x, y) < 6)
                {
                    isObject = true;
                    break;
                }
                
            }
            
            if (isObject == false)
            {
                actors.push_back(new Boulder(c, r, this));
                B--;
                
                
                //remove dirt around the boulder
                for (int x = c; x <= c + 3; x++)
                {
                    for (int y = r; y <= r + 3; y++)
                    {
                        if (m_dirt[x][y] != nullptr)
                        {
                            delete m_dirt[x][y];
                            m_dirt[x][y] = nullptr;
                        }
                    }
                }
            }
            
        }
        
    }
    
    // Gold Nugget
    //m_gold = new GoldNugget(40, 30, this);
    int G = max(5 - level / 2, 2);
    while (G > 0)
    {
        int r = rand() % (VIEW_HEIGHT - 7);
        int c = rand() % (VIEW_WIDTH - 3);
        
        if ((c < 27 && r >= 20 && r <= 56) || (c > 36 && r >= 20 && r <= 56))
        {
            bool isObject = false;
            for (auto p = actors.begin(); p != actors.end(); p++)
            {
                int x = (*p)->getX();
                int y = (*p)->getY();
                if (objectRadius(c, r, x, y) < 6)
                {
                    isObject = true;
                    break;
                }
                
            }
            
            if (isObject == false)
            {
                bool k = false;
                if (rand() % 2)
                    k = true;
                actors.push_back(new GoldNugget(c, r, false, this));
                
                G--;
                
            }
        }
        
    }
    
    //Barrel of Oil
    int L = min(2 + level, 18);
    while (L > 0)
    {
        int r = rand() % (VIEW_HEIGHT - 7);
        int c = rand() % (VIEW_WIDTH - 3);
        
        if ((c < 27 && r >= 20 && r <= 56) || (c > 36 && r >= 20 && r <= 56))
        {
            bool isObject = false;
            for (auto p = actors.begin(); p != actors.end(); p++)
            {
                int x = (*p)->getX();
                int y = (*p)->getY();
                if (objectRadius(c, r, x, y) < 6)
                {
                    isObject = true;
                    break;
                }
                
            }
            
            if (isObject == false)
            {
                actors.push_back(new Barrel(c, r, this));
                
                L--;
                
            }
        }
        
    }
    
  //  searchShortestPath(shortestPath, 60, 60);
   // (pathMapExit, 60, 6
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    
    //decLives();
    //return GWSTATUS_PLAYER_DIED;
    //shortestPathUpdate();
    int level = getLevel();
    int L = min(2 + level, 18);
    int barrelsLeft = L - (m_diggerMan->getBarrel());
    if (barrelsLeft == 0)
    {	
        return GWSTATUS_FINISHED_LEVEL;
    }
    //PROTESTER
    m_ticksProtester--;
    if (m_ticksProtester == 0 && m_numProtester <= m_maxProtester)
    {
        m_numProtester++;
        m_ticksProtester = max(25, 200 - level);
        if ((rand() % 50) < m_probOfHardcore)
        {
            actors.push_back(new RegularProtester(60, 60, this));
        }
        else
        {
            actors.push_back(new HardcoreProtester(60, 60, this));
        }
    }
    setDisplayText();
    
    makeSonarWater();
    Ask_all_actors_to_do_something();
    removeDeadActors();
    return diggerManStatus();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_diggerMan;
    for (int i = 0; i < 64; i++)
        for (int j = 0; j < 60; j++)
        {
            if (m_dirt[i][j] != nullptr)
                delete m_dirt[i][j];
        }
    /*	vector<Actor*>::iterator B = actor.begin();
     while (B != actor.end())
     {
     Actor* a = (*B);
     
     
     B = actor.erase(B);
     delete a;
     cout << " clean up deleted" << endl;
     B++;
     
     }*/
    vector<Actor*>::iterator B = actors.begin();
    while (B != actors.end())
    {
        Actor* a = (*B);
        if ((*B)->getAlive() == true)
        {
            B = actors.erase(B);
            delete a;
            cout << "deleted" << endl;
        }
        else
        {
            B++;
        }
    }
    
}




GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

int StudentWorld::objectRadius(int x1, int y1, int x2, int y2)
{
	int c = 0;
	int leg1 = x2 - x1;
	int leg2 = y2 - y1;
	c = (int)sqrt((leg1*leg1) + (leg2*leg2));
	return c;
}

bool StudentWorld::isStable(int x, int y)
{
	int dirtRemoved = 0;
	for (int i = 0; i < 4; i++)
	{
		if (m_dirt[x + i][y - 1] == nullptr)
		{
			dirtRemoved++;
		}
	}
	// if the 4 spaces directly below the boulder have any dirt, return true
	// else, there is no dirt directly below the boulder, return false
	if (dirtRemoved != 4)
		return true;
	else
		return false;
}

bool StudentWorld::boulderCheck(int x, int y)
{
	bool value = false;
	for (vector<Actor*> ::iterator B = actors.begin(); B != actors.end(); B++)
	{
		if (dynamic_cast<Boulder*> (*B))
		{
			if ((*B)->getX() == x && (*B)->getY() == y)
			{

			}
			// get the absolute value of diggerman's x minus boulder's x
			// get the absolute value of diggerman's y minus boulder's y
			// if the next step diggerman takes is around radius 3(4?) of a boulder
			// set value to true, meaning don't move
			// this works no matter what direction diggerman is coming at a boulder
			int mx = abs(x - (*B)->getX());
			int my = abs(y - (*B)->getY());
			// just testing code to monitor diggerman and boulder positions
			//cout << "\ndiggerman (" << x << ", " << y << ")\n"
			//	"boulder (" << (*B)->getX() << ", " << (*B)->getY() << ")\n";
			//cout << "\nx: " << mx << '\n' << "y: " << my << '\n';
			if (mx <= 3 && my <= 3)
			{
				value = true;
			}
		}
	}
	return value;
}

int StudentWorld::regularCheckDamage(int x, int y, int r, int damage, bool Squirt)
{
	int count = 0;
	for (vector<Actor*> ::iterator B = actors.begin(); B != actors.end(); B++)
	{
		if (dynamic_cast<RegularProtester*> (*B))
		{
			int mx = abs(x - (*B)->getX());
			int my = abs(y - (*B)->getY());
			if (mx <= r && my <= r)
			{
				(*B)->Annoy(damage);

				count++;
				if (Squirt == true)
					if ((*B)->getHealth() <= 0) {
                        int level = getLevel();
                        int P = max(0, 3 - level / 4);
                        (*B) -> ticksWait(P);
						increaseScore(100);
						break;
					}
			}

		}
		if (dynamic_cast<HardcoreProtester*> (*B))
		{
			int mx = abs(x - (*B)->getX());
			int my = abs(y - (*B)->getY());
			if (mx <= r && my <= r)
			{
				(*B)->Annoy(damage);

				count++;
				if (Squirt == true)
					if ((*B)->getHealth() <= 0){
                        int level = getLevel();
                        int P = max(0, 3 - level / 4);
                        (*B) -> ticksWait(P);
						increaseScore(250);
						break;
					}
			}

		}


	}
	return count;
}
std::string StudentWorld::protesterCheck(int x, int y, int r)
{
	for (vector<Actor*> ::iterator B = actors.begin(); B != actors.end(); B++)
	{
		if (dynamic_cast<RegularProtester*> (*B))
		{
			int mx = abs(x - (*B)->getX());
			int my = abs(y - (*B)->getY());
			if (mx <= r && my <= r)
			{
                (*B) -> setleaveOilField(true);
				return "regular";
			}

		}
		if (dynamic_cast<HardcoreProtester*> (*B))
		{
			int mx = abs(x - (*B)->getX());
			int my = abs(y - (*B)->getY());
			if (mx <= r && my <= r)
			{
                int level = getLevel();
                int ticks_to_stare = max(50, 100 - level * 10);
                (*B) -> ticksWait(ticks_to_stare);
				return "hardcore";
			}

		}

	}
	return "neither";
}
bool StudentWorld::diggerCheck(int x, int y, int r)
{
	int mx = abs(x - m_diggerMan->getX());
	int my = abs(y - m_diggerMan->getY());
	if (mx <= r && my <= r)
	{
		return true;
	}
	return false;
}

bool StudentWorld::checkObject(int x, int y)
{
	bool isObject = false;

	//checks if there is dirt at position x & y
	for (int a = x; a <= x + 3; a++)
	{
		for (int b = y; b <= y + 3; b++)
		{
			if (a < 64 && b < 60)
			{
				if (m_dirt[a][b] != nullptr)
				{
					isObject = true;
				}
			}
		}
	}

	//checks if there is boulder at position x to x+3 & y to y+3
	for (vector<Actor*> ::iterator B = actors.begin(); B != actors.end(); B++)
	{
		if (dynamic_cast<Boulder*> (*B))
		{
			// get the absolute value of diggerman's x minus boulder's x
			// get the absolute value of diggerman's y minus boulder's y
			// if the next step diggerman takes is around radius 3(4?) of a boulder
			// set value to true, meaning don't move
			// this works no matter what direction diggerman is coming at a boulder
			int mx = abs(x - (*B)->getX());
			int my = abs(y - (*B)->getY());
			// just testing code to monitor diggerman and boulder positions
			//cout << "\nsquirt (" << x << ", " << y << ")\n"
			//	"boulder (" << (*B)->getX() << ", " << (*B)->getY() << ")\n";
			//cout << "\nx: " << mx << '\n' << "y: " << my << '\n';
			if (mx <= 3 && my <= 3)
			{
				isObject = true;
			}
		}
	}
	return isObject;
}

bool StudentWorld::hvObjectCheck(int x, int y)
{
	// if posx is true, diggerman is on the right side of protestor
	// if posx is false, diggerman is on the left side of protestor
	// if posy is true, diggerman is on top of the protestor
	// if posy is false, diggerman is on bottom of the protestor
	bool posx = x < m_diggerMan->getX();
	bool posy = y < m_diggerMan->getY();
	int pathx = abs(x - m_diggerMan->getX());
	int pathy = abs(y - m_diggerMan->getY());
	bool object = false;

	if (y == m_diggerMan->getY())
	{
		for (int i = 0; i < pathx; i++)
		{
			if (posx == true && object != true)
				object = checkObject(x + i, y);
			else if (posx == false && object != true)
				object = checkObject(x - i, y);
		}
	}
	else if (x == m_diggerMan->getX())
	{
		for (int i = 0; i < pathy; i++)
		{
			if (posy == true && object != true)
				object = checkObject(x, y + i);
			else if (posy == false && object != true)
				object = checkObject(x, y - i);
		}
	}
	return object;
}

void StudentWorld::makeSonarWater()
{
	int level = getLevel();
	int W = level * 25 + 300;
	//int S = 1;
	int r = rand() % W;
	//cout << " r = " << r << endl;
	if (r == 1) {
		int goodie = rand() % 5;
		//cout << " goodie = " << goodie << endl;
		if (goodie == 0) {
			//cout << "SonarKit added! " << endl;
			actors.push_back(new SonarKit(0, 60, this));
		}
		else {
			bool create = false;
			while (create == false) {
				int r = rand() % (VIEW_HEIGHT - 4);
				int c = rand() % (VIEW_WIDTH);
				bool check = true;
				for (int i = r; i < r + 4; i++)
					for (int j = c; j < c + 4; j++) {
						if (m_dirt[j][i] != nullptr)
							check = false;
					}
				if (check == true) {
					for (auto p = actors.begin(); p != actors.end(); p++)
					{
						int x = (*p)->getX();
						int y = (*p)->getY();
						if (objectRadius(c, r, x, y) < 6)
						{
							check = false;
							break;
						}

					}
				}

				if (check == true)
				{
					actors.push_back(new WaterPool(c, r, this));
					create = true;
				}
			}
		}
	}
}

void StudentWorld::makeSquirt()
{
	if (m_diggerMan->getDirection() == Actor::up)
	{
		actors.push_back(new Squirt(m_diggerMan->getX(), m_diggerMan->getY() + 4, m_diggerMan->getDirection(), this));
	}
	else if (m_diggerMan->getDirection() == Actor::down)
	{
		actors.push_back(new Squirt(m_diggerMan->getX(), m_diggerMan->getY() - 4, m_diggerMan->getDirection(), this));
	}
	else if (m_diggerMan->getDirection() == Actor::left)
	{
		actors.push_back(new Squirt(m_diggerMan->getX() - 4, m_diggerMan->getY(), m_diggerMan->getDirection(), this));
	}
	else if (m_diggerMan->getDirection() == Actor::right)
	{
		actors.push_back(new Squirt(m_diggerMan->getX() + 4, m_diggerMan->getY(), m_diggerMan->getDirection(), this));
	}
}

void StudentWorld::removeDirt(int X, int Y)
{
	if ((X >= 0 && X < 64) && (Y >= 0 && Y < 60))
	{
		for (int a = X; a <= X + 3; a++)
		{
			for (int b = Y; b <= Y + 3; b++)
			{
				if (a < 64 && b < 60)
				{
					if (m_dirt[a][b] != nullptr)
					{
						delete m_dirt[a][b];
						m_dirt[a][b] = nullptr;
						playSound(SOUND_DIG);
					}
				}
			}
		}
	}
}

void StudentWorld::removeDeadActors()
{
	vector<Actor*>::iterator B = actors.begin();
	while (B != actors.end())
	{
		Actor* a = (*B);
		if ((*B)->getAlive() == false)
		{
			B = actors.erase(B);
			delete a;
            searchShortestPath(60, 60);
			//cout << "deleted" << endl;
		}
		else
		{
			B++;
		}
	}

	//for (vector<Actor*>::iterator B = actors.begin(); B != actors.end(); B++)
	//{
	//	Actor* a = (*B);
	//	if (a->getAlive() == false)
	//	{
	//		//delete (*B);
	//		B = actors.erase(B);
	//		cout << "Deleted" << endl;
	//		//index--;
	//		delete a;
	//		B--;
	//	}
	//	//index++;
	//}
}

void StudentWorld::useSonar(int x, int y) {
	playSound(SOUND_SONAR);
	for (vector<Actor*> ::iterator B = actors.begin(); B != actors.end(); B++) {
		if (dynamic_cast<GoldNugget*> (*B)) {
			if (objectRadius(x, y, (*B)->getX(), (*B)->getY()) <= 10)
				(*B)->setVisible(true);
		}
		if (dynamic_cast<Barrel*> (*B)) {
			if (objectRadius(x, y, (*B)->getX(), (*B)->getY()) <= 12)
				(*B)->setVisible(true);
		}

	}
}

int StudentWorld::getDiggerManX()
{
	return m_diggerMan->getX();
}

int StudentWorld::getDiggerManY()
{
	return m_diggerMan->getY();
}

void StudentWorld::annoyDiggerMan(int damage)
{
	m_diggerMan->Annoy(damage);
}

void StudentWorld::setDiggerBarrels()
{
	m_diggerMan->setBarrel();
}

void StudentWorld::setDiggerNuggets()
{
	m_diggerMan->setNuggets();
}

void StudentWorld::setDiggerSonar()
{
	m_diggerMan->setSonar();
}
void StudentWorld::setDiggerSquirt()
{
	m_diggerMan->setWater();
}
 
int StudentWorld::diggerManStatus()
{
	if (m_diggerMan->getAlive() == false)
	{
		decLives();
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}
	return GWSTATUS_CONTINUE_GAME;

}

void StudentWorld::setDisplayText()
{
	int level = getLevel();
	int lives = getLives();
	int health = m_diggerMan->getHealth();
	int squirts = m_diggerMan->getWater();
	int gold = m_diggerMan->getNugget();
	int sonar = m_diggerMan->getSonar();
	int currentLevel = getLevel();
	int L = min(2 + level, 18);
	int barrelsLeft = L - (m_diggerMan->getBarrel());
	int score = getScore();

	//cout <<  "Lvl: " << to_string(level) << " Lives: " << lives << " Hlth: " << health << "%" << " Wtr: " << squirts << " Gld: " << gold << " Sonar: " << sonar << " Oil Left: " << barrel << " Score: " << Score << endl;

	string s = text(level, lives, health, squirts, gold, sonar, barrelsLeft, score);
	setGameStatText(s);
}

string StudentWorld::text(int level, int lives, int health, int squirts, int gold, int sonar, int barrelsLeft, int score)
{
	int healthptg = health * 100;
	string s = "Lvl: " + to_string(level) + " Lives: " + to_string(lives) + " Hlth: " + to_string((healthptg / 10)) + "% Wtr: " + to_string(squirts) + " Gld: " + to_string(gold) + " Sonar: " + to_string(sonar) + " Oil left: " + to_string(barrelsLeft) + " Score: " + to_string(score);
	return s;
}



void StudentWorld::Ask_all_actors_to_do_something()
{
	m_diggerMan->doSomething();
	for (vector <Actor*>::iterator i = actors.begin(); i != actors.end(); i++)
		if (dynamic_cast<Actor*> (*i))
			(*i)->doSomething();
}

void StudentWorld::dropGold()
{
	actors.push_back(new GoldNugget(getDiggerManX(), getDiggerManY(), true, this));


}

void StudentWorld::searchShortestPath(int x, int y)
{
	for (int i = 0; i < VIEW_HEIGHT; i++)
	{
		for (int j = 0; j < VIEW_WIDTH - 3; j++)
		{
			if (m_dirt[j][i] != nullptr || boulderCheck(j, i))
			{
                //cout << "went through 1000" << endl;
                shortestPath[j][i] = 10000; // blocked by a boulder/dirt
			}
			else if (m_dirt[j][i] == nullptr && !boulderCheck(j,i))
			{
				shortestPath[j][i] = 0; // can go through
                //cout << "shortestPath[j][i]" << " j" << j << " i" << i << "\t" << shortestPath[j][i] <<endl;
			}
		}
	}

    
    //breadth first search
    // we create struct Node, which holds x and y position.
    queue<Node> pos;
    Node init(x, y); //initial position
    Node current; // current node
    int distance = 0;
    pos.push(init);
    
    // assigning appropriate distance/count the distance of paths in the entire oil field
    while(!pos.empty())
    {
        distance++;
        
        current = pos.front();
        pos.pop();
        
        // chekcing adjacent positions and push the positions into the queue if they are possible path.
        
        // check left: checking if getX()-1 is outside the boundary and if getX()-1 is a possible path
        // if left is a possible path, then assign current distance to that position (marking it's visited) and push the position into the queue
        if(current.getX()-1 >= 0 && shortestPath[current.getX()-1][current.getY()] == 0)
        {
            shortestPath[current.getX()-1][current.getY()] = distance;
            pos.push(Node(current.getX()-1, current.getY()));
          
        }
        
        // check right: checking if getX()+1 is outside the boundary and if getX()+1 is a possible path
        // if left is a possible path, then assign current distance to that position and push the position into the queue
        if(current.getX()+1 <= 60 && shortestPath[current.getX()+1][current.getY()] == 0)
        {
            shortestPath[current.getX()-1][current.getY()] = distance;
            pos.push(Node(current.getX()+1, current.getY()));
        
        }
        
        // check down: checking if getY()-1 is outside the boundary and if getY()-1 is a possible path
       // if left is a possible path, then assign current distance to that position and push the position into the queue
        if(current.getY()-1 >= 0 && shortestPath[current.getX()][current.getY()-1] == 0)
        {
            shortestPath[current.getX()][current.getY()-1] = distance;
            pos.push(Node(current.getX(), current.getY()-1));
        }
        
        // check up: checking if getY()+1 is outside the boundary and if getY()+1 is a possible path
        // if left is a possible path, then assign current distance to that position and push the position into the queue
        if(current.getY()+1 <= 60 && shortestPath[current.getX()][current.getY()+1] == 0)
        {
            shortestPath[current.getX()][current.getY()-1] = distance;
            pos.push(Node(current.getX(), current.getY()+1));
        }
    
    }
	
}

string StudentWorld:: moveToLeave(int x, int y)
{
    string direction;
    int distance = 10000;
   
    if(shortestPath[x-1][y] < distance && shortestPath[x-1][y] != 10000)
    {
        cout << "choosing left" << endl;
        direction = shortestPath[x-1][y];
        direction = "left";
    }
    if(shortestPath[x+1][y] < distance && shortestPath[x+1][y] != 10000)
    {
        cout << "choosing right" << endl;
        direction = shortestPath[x+1][y];
        direction = "right";
    }
    if(shortestPath[x][y-1] < distance && shortestPath[x][y-1] != 10000)
    {
        cout << "choosing down" << endl;
        direction = shortestPath[x][y-1];
        direction = "down";
    }
    if(shortestPath[x][y+1] < distance && shortestPath[x][y+1] != 10000)
    {
        cout << "choosing up" << endl;
        direction = shortestPath[x][y+1];
        direction = "up";
    }
    cout << "left: " <<shortestPath[x-1][y] << endl;
    cout << "right: " <<shortestPath[x+1][y] << endl;
    cout << "up: " << shortestPath[x][y+1] << endl;
    cout << "down: " << shortestPath[x][y-1] << endl;
   //cout << "curr x y: " << x  << " " << y << endl;
   //cout << "curr: " << shortestPath[x][y] << endl;
   //cout << shortestPath[59][59] << endl;
    return direction;

}

