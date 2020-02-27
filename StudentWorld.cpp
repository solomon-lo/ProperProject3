#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <cmath>
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
}

int StudentWorld::init()
{

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
			if ((distanceToCenterActor <= 8 || ((*it)->blocksBacteriumMovement())))
			{
				overlappedWithSomething = true;
				break;
			}
		}
		if (overlappedWithSomething)
		{
			i--;
			continue;
		}
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
			if ((distanceToCenterActor <= 8)) //|| ((*it)->blocksBacteriumMovement()))
			{
				overlappedWithSomething = true;
				break;
			}
		}
		if (overlappedWithSomething)
		{
			i--;
			continue;
		}
		Food* newFood = new Food(randomFoodX, randomFoodY, this);
		addToActorsVector(newFood);
	}

	int numOfDirtPiles = max(180 - 20 * getLevel(), 20);
	for (int i = 0; i < numOfDirtPiles; i++)
	{
		int randomX = 0;
		int randomY = 0;
		while (sqrt((randomX - (VIEW_WIDTH / 2)) * (randomX - (VIEW_WIDTH / 2)) + (randomY - (VIEW_HEIGHT / 2)) * (randomY - (VIEW_HEIGHT / 2))) > 120)
		{

			randomX = randInt((VIEW_WIDTH / 2) - 120, (VIEW_WIDTH / 2) + 120);
			randomY = randInt((VIEW_HEIGHT / 2) - 120, (VIEW_HEIGHT / 2) + 120);
		}
		DirtPile* newDirtPile = new DirtPile(randomX, randomY, this);
		ActorsVector.push_back(newDirtPile);
	}

	addToActorsVector(new Salmonella(120, 120, this));
	//init a goodie(MUST BE REMOVED, THIS IS FOR TESTING ONLY)
	const double PI = 4 * atan(1);
	//THE 90 WILL BE REPLACED WITH A RANDOM NUMBER FROM 0 TO 360
	double goodieX = (VIEW_WIDTH / 2) + (128 * cos(175 * 1.0 / 360 * 2 * PI));
	//cerr << "getpositionalnagle is" << getPositionalAngle() << endl;
	//cerr << "newX is" << newX << endl;
	double goodieY = (VIEW_HEIGHT / 2) + (128 * sin(175 * 1.0 / 360 * 2 * PI));
	ActorsVector.push_back(new Food(50, 50, this));
	//ActorsVector.push_back(new Food(95, 118, this));
	ActorsVector.push_back(new Food(80, 80, this));
	ActorsVector.push_back(new Food(132, 132, this));

	ActorsVector.push_back(new AggressiveSalmonella(128, 128, this, IID_FLAME));
	ActorsVector.push_back(new AggressiveSalmonella(75, 75, this, IID_FLAME));

	//ActorsVector.push_back(new Salmonella(100, 100, this));

	double flameX = (VIEW_WIDTH / 2) + (128 * cos(160 * 1.0 / 360 * 2 * PI));
	double flameY = (VIEW_HEIGHT / 2) + (128 * sin(160 * 1.0 / 360 * 2 * PI));
	ActorsVector.push_back(new FlameThrowerGoodie(flameX, flameY, this));

	//double extraLifeX = (VIEW_WIDTH / 2) + (128 * cos(185 * 1.0 / 360 * 2 * PI));
	//double extraLifeY = (VIEW_HEIGHT / 2) + (128 * sin(185 * 1.0 / 360 * 2 * PI));
	//ActorsVector.push_back(new ExtraLifeGoodie(extraLifeX, extraLifeY, this));
	//playerObject = new Socrates(this);

	double fungusX = (VIEW_WIDTH / 2) + (128 * cos(185 * 1.0 / 360 * 2 * PI));
	double fungusY = (VIEW_HEIGHT / 2) + (128 * sin(185 * 1.0 / 360 * 2 * PI));
	ActorsVector.push_back(new Fungus(fungusX, fungusY, this));
	playerObject = new Socrates(this);

	//init food items
	return GWSTATUS_CONTINUE_GAME;

}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	playerObject->doSomething();
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{
		(*it)->doSomething();
	}

	removeDeadActors();
	double fixed_health_num = getPlayerObjectHealth();
	setGameStatText("Score: " + to_string(getScore()) + " Level: " + to_string(getLevel()) + " Lives: " + to_string(getLives()) + " Health: " + to_string(fixed_health_num) + " Sprays: " + to_string(getPlayerObjectSpraysLeft()) + " Flames: " + to_string(getPlayerObjectFlamesLeft()));

	return GWSTATUS_CONTINUE_GAME;
}

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
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); )
	{
		if ((*it)->getAliveStatus() == false)
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
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{

		double distanceToCenterActor = getEuclideanDistance(centerActorX, centerActorY, (*it)->getX(), (*it)->getY());
		if (distanceToCenterActor <= SPRITE_RADIUS * 2)
		{
			if ((*it)->sprayWillHarm() == true)
			{
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::wentOverFlammableObject(int centerActorX, int centerActorY)
{
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{

		double distanceToCenterActor = getEuclideanDistance(centerActorX, centerActorY, (*it)->getX(), (*it)->getY());
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

//double angle = (180/PI) * atan2(player->getY() - a->getY()), player->getX() - a-getX()));


//food or socrates is front value
//bacteria is back value
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

bool StudentWorld::isThisCoordinateFilled(double testX, double textY)
{
	vector<ActorBaseClass*>::iterator it;
	for (it = ActorsVector.begin(); it != ActorsVector.end(); it++)
	{

	}
	return false;
}

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

void StudentWorld::addToActorsVector(ActorBaseClass* actorToAdd)
{
	ActorsVector.push_back(actorToAdd);
}

StudentWorld::~StudentWorld()
{
	this->cleanUp();
}
