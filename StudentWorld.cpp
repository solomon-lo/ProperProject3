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
	numOfPits = 0;
	numOfBacteria = 0;
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

	playerObject = new Socrates(this);

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
		addToActorsVector(new Salmonella(120, 120, this));
		const double PI = 4 * atan(1);
		double goodieX = (VIEW_WIDTH / 2) + (128 * cos(175 * 1.0 / 360 * 2 * PI));
		double goodieY = (VIEW_HEIGHT / 2) + (128 * sin(175 * 1.0 / 360 * 2 * PI));
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

	if (playerObject->getHP() <= 0 || playerObject->getAliveStatus() == false)
	{
		return GWSTATUS_PLAYER_DIED;
		decLives();
	}
	if (numOfPits == 0 && numOfBacteria == 0)
	{
		return GWSTATUS_FINISHED_LEVEL;
	}

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
				(*it)->modifyHP(-2);
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
