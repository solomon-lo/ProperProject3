#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;
//class ActorBaseClass;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath)
{
	numOfPits = 0;
	numOfBacteria = 0;
}
int StudentWorld::getNumOfPits()
{
	return numOfPits;
}
int StudentWorld::getNumOfBacteria()
{
	return numOfBacteria;
}

void StudentWorld::modifyNumOfPits(int modifyAmount)
{
	numOfPits += modifyAmount;
}

void StudentWorld::modifyNumOfBacteria(int modifyAmount)
{
	numOfBacteria+= modifyAmount;
}

int StudentWorld::init()
{
	//instantiates the playerObject Socrates
	playerObject = new Socrates(this);
	//this creates all of the new pits
	for (int i = 0; i < getLevel(); i++)
	{
		int randomPitX = 0;
		int randomPitY = 0;
		while (getEuclideanDistance(randomPitX, randomPitY, (VIEW_WIDTH / 2), (VIEW_HEIGHT / 2)) > 120.00)
		{
			randomPitX = randInt((VIEW_WIDTH / 2) - 120, (VIEW_WIDTH / 2) + 120);
			randomPitY = randInt((VIEW_HEIGHT / 2) - 120, (VIEW_HEIGHT / 2) + 120);
		}
		bool overlappedWithSomething = false;
		vector<ActorBaseClass*>::iterator it;
		for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
		{
			double distanceToCenterActor = getEuclideanDistance(randomPitX, randomPitY, (*it)->getX(), (*it)->getY());
			if ((distanceToCenterActor < SPRITE_RADIUS * 2))
			{
				overlappedWithSomething = true;
				break;
			}
		}
		//reruns the loop if it actually overlaps with something
		if (overlappedWithSomething)
		{
			i--;
			continue;
		}
		modifyNumOfPits(1);
		Pit* newPit = new Pit(randomPitX, randomPitY, this);
		addToActorsVector(newPit);
	}


	//adding food to the StudentWorld
	for (int i = 0; i < min(5 * getLevel(), 25); i++)
	{
		int randomFoodX = 0;
		int randomFoodY = 0;
		while (getEuclideanDistance(randomFoodX, randomFoodY, (VIEW_WIDTH / 2), (VIEW_HEIGHT / 2)) > 120.00)
		{
			randomFoodX = randInt((VIEW_WIDTH / 2) - 120, (VIEW_WIDTH / 2) + 120);
			randomFoodY = randInt((VIEW_HEIGHT / 2) - 120, (VIEW_HEIGHT / 2) + 120);
		}
		bool overlappedWithSomething = false;
		vector<ActorBaseClass*>::iterator it;
		for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
		{
			double distanceToCenterActor = getEuclideanDistance(randomFoodX, randomFoodY, (*it)->getX(), (*it)->getY());
			if ((distanceToCenterActor < 2 * SPRITE_RADIUS))
			{
				overlappedWithSomething = true;
				break;
			}
		}
		//reruns loop if overlap detected
		if (overlappedWithSomething)
		{
			i--;
			continue;
		}
		Food* newFood = new Food(randomFoodX, randomFoodY, this);
		addToActorsVector(newFood);
	}

	//ADDING DIRTPILES
	int numOfDirtPiles = max(180 - 20 * getLevel(), 20);

	//uses the previously generated amount of dirt piles
	for (int i = 0; i < numOfDirtPiles; i++)
	{
		int randomDirtX = 0;
		int randomDirtY = 0;
		while (getEuclideanDistance(randomDirtX, randomDirtY, (VIEW_WIDTH / 2), (VIEW_HEIGHT / 2)) > 120.00)
		{
			randomDirtX = randInt((VIEW_WIDTH / 2) - 120, (VIEW_WIDTH / 2) + 120);
			randomDirtY = randInt((VIEW_HEIGHT / 2) - 120, (VIEW_HEIGHT / 2) + 120);
		}
		bool overlappedWithSomething = false;
		vector<ActorBaseClass*>::iterator it;

		for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
		{
			double distanceToCenterActor = getEuclideanDistance(randomDirtX, randomDirtY, (*it)->getX(), (*it)->getY());
			if ((distanceToCenterActor < 2 * SPRITE_RADIUS))
			{
				if ((*it)->blocksBacteriumMovement() == false)
				{
					overlappedWithSomething = true;
					break;
				}
			}
		}
		if (overlappedWithSomething)
		{
			i--;
			continue;
		}
		DirtPile* newDirt = new DirtPile(randomDirtX, randomDirtY, this);
		addToActorsVector(newDirt);
	}

	return GWSTATUS_CONTINUE_GAME;

}

//keeps looping
int StudentWorld::move()
{

	//checks to see if the player is still live
	if (playerObject->getHP() <= 0 || playerObject->getAliveStatus() == false)
	{
		//subtracts from the lives if its actually dead
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	//check for the win condition
	if (numOfPits == 0 && numOfBacteria == 0)
	{
		return GWSTATUS_FINISHED_LEVEL;
	}
	removeDeadActors();
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	playerObject->doSomething();
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{
		(*it)->doSomething();
	}

	
	double fixed_health_num = getPlayerObjectHealth();

	////spawning new goodies
	int chanceFungus = max(510 - getLevel() * 10, 200);
	int fungusRandomNumber = randInt(0, chanceFungus);
	if (fungusRandomNumber == 0)
	{
		const double PI = 4 * atan(1);
		int randomDegree = randInt(0, 359);
		double fungusX = (VIEW_WIDTH / 2) + (128 * cos(randomDegree * 1.0 / 360 * 2 * PI));
		double fungusY = (VIEW_HEIGHT / 2) + (128 * sin(randomDegree * 1.0 / 360 * 2 * PI));
		ActorsVector.push_back(new Fungus(fungusX, fungusY, this));
	}


	//generates the different ratios as well
	int chanceGoodie = max(510 - getLevel() * 10, 250);
	int goodieRandomNumber = randInt(0, chanceGoodie);
	if (goodieRandomNumber == 0)
	{
		const double PI = 4 * atan(1);
		int randomDegree = randInt(0, 359);
		double goodieX = (VIEW_WIDTH / 2) + (128 * cos(randomDegree * 1.0 / 360 * 2 * PI));
		double goodieY = (VIEW_HEIGHT / 2) + (128 * sin(randomDegree * 1.0 / 360 * 2 * PI));

		//implementaiton of the random goodies drop
		int goodieSelector = randInt(1, 10);
		if (goodieSelector == 1)
		{
			ActorsVector.push_back(new ExtraLifeGoodie(goodieX, goodieY, this));
		}
		else if (goodieSelector >= 2 && goodieSelector <= 4)
		{
			ActorsVector.push_back(new FlameThrowerGoodie(goodieX, goodieY, this));
		}
		else
		{
			ActorsVector.push_back(new RestoreHealthGoodie(goodieX, goodieY, this));
		}
	}

	//calls the function to update the game text
	updateDisplayText();

	return GWSTATUS_CONTINUE_GAME;
}


//get the Socrates health
double StudentWorld::getPlayerObjectHealth()
{
	return playerObject->getHP();
}

int StudentWorld::getPlayerObjectSpraysLeft()
{
	return playerObject->getNumOfSprayProjectiles();
}

int StudentWorld::getPlayerObjectFlamesLeft()
{
	return playerObject->getNumOfFlameThrowerCharges();
}
void StudentWorld::removeDeadActors()
{
	//iterates through all the actors sees if any of them have 0 health
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); )
	{
		if ((*it)->getAliveStatus() == false || (*it)->getHP() <=0)
		{
			delete* it;
			it = ActorsVector.erase(it);

		}
		else
		{
			it++;
		}
	}
}


double StudentWorld::getEuclideanDistance(double baseX, double baseY, double newX, double newY)
{

	//trigonometry to calculate the Euclidean distance
	double difX = abs((newX - baseX));
	double difY = abs((newY - baseY));
	double toSqrt = ((difX * difX) + (difY * difY));
	return sqrt(toSqrt);
}

double StudentWorld::getDistanceFromSocrates(ActorBaseClass* targetActor)
{
	double actorX = targetActor->getX();
	double actorY = targetActor->getY();
	return getEuclideanDistance(playerObject->getX(), playerObject->getY(), actorX, actorY);
}

void StudentWorld::makeSocratesFullHP()
{
	int currentHP = playerObject->getHP();
	playerObject->modifyHP(100 - currentHP);
}

void StudentWorld::modifySocratesHP(int modifyAmount)
{
	playerObject->modifyHP(modifyAmount);
}

void StudentWorld::flameThrowerGoodieEffect()
{
	playerObject->modifyNumOfFlameThrowerCharges(5);
}

void StudentWorld::fungusEffect()
{
	playerObject->modifyHP(-20);
}
bool StudentWorld::wentOverSprayableObject(int centerActorX, int centerActorY)
{
	//iterates through all of the Actors
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{

		double distanceToCenterActor = getEuclideanDistance(centerActorX, centerActorY, (*it)->getX(), (*it)->getY());
		//if it's close and spray harms it
		if (distanceToCenterActor <= SPRITE_RADIUS * 2)
		{
			if ((*it)->sprayWillHarm() == true)
			{
				(*it)->modifyHP(-2);
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::wentOverFlammableObject(int centerActorX, int centerActorY)
{
	//iterates through all of the different actors
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{

		double distanceToCenterActor = getEuclideanDistance(centerActorX, centerActorY, (*it)->getX(), (*it)->getY());
		//if it overlaps and is harmed by flames
		if (distanceToCenterActor <= SPRITE_RADIUS * 2)
		{
			if ((*it)->flameWillHarm() == true)
			{
				(*it)->modifyHP(-5);
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::wentOverDirtPile(int centerActorX, int centerActorY)
{
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{

		double distanceToCenterActor = getEuclideanDistance(centerActorX, centerActorY, (*it)->getX(), (*it)->getY());
		if (distanceToCenterActor <= SPRITE_RADIUS)	//DIFFERENT FROM REGULAR OVERLAP
		{
			if ((*it)->blocksBacteriumMovement() == true)
			{
				return true;
			}
		}
	}
	return false;
}

//also causes the effects when it goves over food
bool StudentWorld::wentOverFood(int centerActorX, int centerActorY)
{
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{

		double distanceToCenterActor = getEuclideanDistance(centerActorX, centerActorY, (*it)->getX(), (*it)->getY());
		//cerr << distanceToCenterActor << endl;
		if (distanceToCenterActor <= SPRITE_RADIUS * 2)
		{
			if ((*it)->isEdible() == true)
			{

				(*it)->modifyHP(-5);
				(*it)->setAsDead();
				return true;
			}
		}
	}
	return false;
}


//returns true if there's something in the coordinate

void StudentWorld::updateDisplayText()
{	
	ostringstream oss;

	//creates the game text in the proper format
	oss << "  Score: ";
	oss.fill('0');
	if (getScore() < 0)
	{
		oss << "-";
		oss << setw(5) << -1 * getScore();
	}
	else
	{
		oss << setw(6) << getScore();
	}
	oss << "  Level:  ";
	oss << setw(1) << getLevel();
	oss << "  Lives:  ";
	oss << setw(1) << getLives();
	oss << "  Health:  ";
	if (getPlayerObjectHealth() >= 100)
	{
		oss << setw(3) << getPlayerObjectHealth();
	}
	else
	{
		oss << setw(2) << getPlayerObjectHealth();
	}
	
	oss << "  Sprays:  ";
	oss << setw(2) << getPlayerObjectSpraysLeft();
	oss << "  Flames:  ";
	if (getPlayerObjectFlamesLeft() >= 10)
	{
		oss << setw(2) << getPlayerObjectFlamesLeft();
	}
	else
	{
		oss << setw(1) << getPlayerObjectFlamesLeft();
	}
	//oss << "Score: " << score << " Level: " << level << " Lives: " << lives << " Health " << health << " Sprays: " << sprays << " Flames: " << flames;

	string text = oss.str();
	setGameStatText(text);

}

//sees if there's food within 128 units, returns true if there is and sets foodX and foodY to the coordinates of the food
bool StudentWorld::findFoodWithin128(double bacteriaX, double bacteriaY, double& foodX, double& foodY)
{
	vector<ActorBaseClass*>::iterator it;
	double currentSmallestX = 9999999;
	double currentSmallestY = 9999999;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{
		double distanceToActor = getEuclideanDistance(bacteriaX, bacteriaY, (*it)->getX(), (*it)->getY());
		if (distanceToActor <= 128)
		{
			if ((*it)->isEdible() == true)
			{
				if (distanceToActor < (getEuclideanDistance(bacteriaX, bacteriaY, currentSmallestX, currentSmallestY)))
				{
					currentSmallestX = (*it)->getX();
					currentSmallestY = (*it)->getY();
				}
			}
		}
	}
	//only reaches here after findin ghte smallest distance
	if ((currentSmallestX == 9999999) && (currentSmallestY == 9999999))
	{
		return false;
	}
	else
	{

		foodX = currentSmallestX;
		foodY = currentSmallestY;
		return true;
	}
}

//looks to see if the Socrates is actually within the distance
bool StudentWorld::findSocratesWithinDistance(double bacteriaX, double bacteriaY, double& SocratesX, double& SocratesY, int inputDistance)
{
	double currentSocratesX = playerObject->getX();
	double currentSocratesY = playerObject->getY();
	if (getEuclideanDistance(bacteriaX, bacteriaY, currentSocratesX, currentSocratesY) <= inputDistance)
	{
		SocratesX = currentSocratesX;
		SocratesY = currentSocratesY;
		return true;
	}
	SocratesX = 1;
	SocratesY = 1;
	return false;

}

//iterates through everything and deletes everything, even if it's still alive
void StudentWorld::cleanUp()
{
	delete playerObject;
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{
		delete* it;
	}
	ActorsVector.clear();
}

//pushes things to the back of the vector, where it'll be iterated through every tick
void StudentWorld::addToActorsVector(ActorBaseClass* actorToAdd)
{
	ActorsVector.push_back(actorToAdd);
}

StudentWorld::~StudentWorld()
{
	this->cleanUp();
}
