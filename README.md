# Digger Man Game
Project description link: https://www.luminpdf.com/viewer/5df1b2e98fe72f0019623feb

Credit to James and Syndy.
This is a group project written in C++ in my Advanced Programming in C++ class.
The only files my team modify are Actor.h, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

![Diggerman](https://user-images.githubusercontent.com/28214466/65370212-2da43d00-dc0b-11e9-86be-0ba2b87147a1.jpg)

1. All the classes we made are inherited from an abstract class called Actor. All the classes that are inherited from Actor gets a pointer to StudentWorld, health, and a Boolean statement, alive, which shows whether the actor is alive or dead. Functions like doSomething, Annoy, getAlive, getHealth are inherited to the child classes as well. Those functions are all virtual functions so one child class can do something unique from other child classes although the name of the functions is the same. 
2. Visual Studio 2013 and Xcode are the platforms that we used to develop and test our program

3. Because none of us were familiar with modern C++ features, we thought using those features might lead us to many errors that we wouldn’t be able to handle (or take long time to fix). So decided to use C++ features that we were familiar and comfortable with (materials we learned in cs1 and cs2) instead. 

4. We tried to organize the code by classes and we chose variable names in a way that we could understand what it meant. For positions, we usually used x, y instead of num1, num2. We tried to write our braces aligned so it becomes easier for us to check that we don’t get any misleading errors due to unmatched braces. Since we did the project all together, we understood what most of the code meant. So we didn’t leave any comment until the end. 

5. Since we never had an experience with such a big project like this, we decided to meet up and work together rather than splitting the work so there was no specific responsibility each group member had. However, we all were responsible for coming to lab and meeting up at least once a week to work on the project. Also when we faced a challenging problem while working on the project together but couldn't figure out how to solve the problem, we were responsible for coming up with an idea or even solve/fix the problem at home. 

6. 
a. The code we wrote for boulder doSomething() appropriately checks if the 4 dirts right below the boulder has been removed or not. If so, the boulder is treated as unstable and waits for 30 ticks. By the time the ticks the boulder waited becomes 30, the boulder is now ready to fall and move to one step down. While the boulder falls by moving a step down every tick, the function checks if there’s any protesters/ diggerman in the path the boulder is falling. If there is, it annoys them and keeps falling, eventually disappears from the oil field

b. At first, we tried to use a while loop when boulder was falling. (our code at first: while(!getWorld() -> isStable(getX() , getY()), …). However, when we ran the program with the code, the boulder was disappearing right away at the position where the boulder had been at, not falling to the ground. Fortunately, we figured out that we were supposed to use an if statement rather than while loop in order to see the image falling (revised code:  if (!getWorld() -> isStable(getX(), getY()), …)). However, we had to revise our code one more time, because we didn’t cover the case when the boulder falls on another boulder. In order to check if there would be any other boulder located in the path the boulder was falling, we made a function called boulderCheck inside studentWorld, which takes in the position of the boulder that is falling. By checking both if the boulder is stable or not and if there’s any boulder below the boulder that is falling, we were able to cover all the cases for boulder doSomething (if (!getWorld()->isStable(getX(), getY()) && !getWorld()->boulderCheck(getX(), getY()))).

c.
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
		if (waiting == 30) // when the waiting reaches 30, the boulder      falls
		{

			if (!getWorld()->isStable(getX(), getY()) && !getWorld()->boulderCheck(getX(), getY()))
			{
				//cout << "falling!" << endl;
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

7. 
a. We got the bug when the Diggerman could go through the Boulder instead of stopping when it was in front of the Boulder. After we created a function to prevent the Diggerman going through the Boulder, we got another the problem that I just worked when it was exactly at the same position of diggerman, otherwise the diggerman still went overlap the boulder.

b. We discovered the bugs by cout multiple code while the Diggerman was executing to check how it worked. The size 4x4 of the Diggerman and the Protester was the problem. We figured out that we were only check one position of the boulder, but also check for whole size of the boulder.

c. We fixed the code by create a function call boulderCheck(). What it did was to check for the whole size of boulder and the position of the diggerman when he was moving. If the Diggerman was going to go through the Boulder, it would call the boulderCheck(), if there was a boulder in the next moving, the diggerman would stop and could not move anymore.

8.
DiggerMan functions that have been implemented: It properly moves up, down, left , and right. The functions for pressing esc, space, tab, and f.
 
Dirt functions that have been implemented: It populates most of the map except the shaft and the top part of the map. If DiggerMan mines through the dirt, the dirt will disappear the appropriate amount.
 
Boulder functions that have been implemented: Boulder does nothing if it is stable (There is enough dirt underneath to keep the boulder from falling). If it’s unstable, the boulder will fall after enough ticks have passed, and keep falling until it comes into contact with dirt or another boulder. If it falls through DiggerMan or a Protester, it’ll deal appropriate damage.
 
Gold Nugget functions that have been implemented: It sets itself to temporary or permanent at initialization. If permanent, it’ll set itself visible if DiggerMan is close enough. If DiggerMan picks up a gold nugget, it’ll increase the score, play a sound, and set itself. If temporary, if a Protester picks up a gold nugget, it’ll increase the score.
 
Barrel functions that have been implemented: It’ll set itself to visible if DiggerMan is close enough. If DiggerMan picks up a barrel, it’ll increase the score and play a sound. When DiggerMan picks a Barrel, it’ll increase the score by the appropriate amount.
 
Sonar Kit functions that have been implemented: It properly sets the visible states of objects to visible in the appropriate range. It spawns in the top left corner of the map. After a set amount of ticks, a new Sonar Kit will spawn. When DiggerMan picks up a Sonar Kit, it increases the score by the appropriate amount.
 
Squirt functions that have been implemented: It travels the appropriate amount of distance from the DiggerMan in the correct direction. If Squirt comes into contact with a Protester, it’ll damage them. After traveling the appropriate distance, it’ll set itself to dead.
 
Water Pool functions that have been implemented: If DiggerMan picks up a water pool, it’ll increase the score and play a sound. It spawns on the map where there is no dirt.
 
Protester functions that have been implemented: Protester moves  up, down, left, right. It can follow the DiggerMan around the map. It shouts at DiggerMan if it’s in range to shout, and waits a certain amount of ticks until it can yell again. It does not walk through dirt or boulders.
 
Hardcode functions that have been implemented: Hardcore Protester moves  up, down, left, right. It can follow the DiggerMan around the map. It shouts at DiggerMan if it’s in range to shout, and waits a certain amount of ticks until it can yell again. It does not walk through dirt or boulders.
 
The score of the game has been implemented and updates properly. Actors are removed from the game after their states have been set to dead. The game moves onto the next level after DiggerMan collects enough of the barrels on the map. After DiggerMan loses all of his lives, the game is over.
 
Things that have not been implemented or not working properly: The shortest path for Protester and Hardcore Protester haven’t been implemented properly. Cellphone has not been implemented. The protester keep moving around DiggerMan after shouting instead of staying in place.
 
We tested most of our code by using cout to see the states of certain variable. By monitoring the variables, we were able to determine what values were set properly and what conditions were being met.  
