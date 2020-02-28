#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <cmath>
#include<math.h>
using namespace std;

class StudentWorld;

ActorBaseClass::ActorBaseClass(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* inputStudentWorld, int inputHP)
	:GraphObject(imageID, startX, startY, dir, depth)
{
	aliveStatus = true;
	m_StudentWorld = inputStudentWorld;
	HP = inputHP;
}
bool ActorBaseClass::getAliveStatus()
{
	return aliveStatus;
}

void ActorBaseClass::setAsDead()
{
	aliveStatus = false;
}

StudentWorld* ActorBaseClass::getStudentWorld()
{
	return m_StudentWorld;
}

ActorBaseClass::~ActorBaseClass()
{}

double ActorBaseClass::getHP()
{
	return HP;
}

void ActorBaseClass::modifyHP(int modifyAmount)
{
	HP += modifyAmount;
}

bool ActorBaseClass::blocksBacteriumMovement() const
{
	return false;
}

bool ActorBaseClass::sprayWillHarm()
{
	return false;
}
bool ActorBaseClass::flameWillHarm()
{
	return false;
}

//checks if there's less than or equal to 0 hp, and sets the object as dead if it is.
bool ActorBaseClass::SetAsDeadIfLessThan0HP()
{
	if (getHP() <= 0)
	{
		setAsDead();
		return true;
	}

	return false;
}

bool ActorBaseClass::isEdible()
{
	return false;
}

Pit::Pit(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth)
	:ActorBaseClass(imageID, startX, startY, dir, depth, inputStudentWorld)
{
	RegularSalmonellaInventory = 5;
	AggressiveSalmonellaInventory = 3;
	EColiInventory = 2;
}

void Pit::doSomething()
{

	//condition to see if it should set itself as dead
	//this is to see if there's nothing left to spawn
	if ((RegularSalmonellaInventory + AggressiveSalmonellaInventory + EColiInventory) == 0)
	{
		getStudentWorld()->modifyNumOfPits(-1);
		modifyHP(-5);
		setAsDead();
	}

	//the one in fifty chase per tick
	int randomNumberFromOneToFifty = randInt(1, 50);
	if (randomNumberFromOneToFifty == 10)
	{

		int chooseBacteriaToSpawn;
		bool bacteriaSpawned = false;

		while (bacteriaSpawned == false)
		{
			//keeps regenerating the random number to choose a bacteria that still has stock left to spawn
			chooseBacteriaToSpawn = randInt(1, 3);
			if ((chooseBacteriaToSpawn == 1) && (RegularSalmonellaInventory > 0))
			{
				getStudentWorld()->addToActorsVector(new Salmonella(getX(), getY(), getStudentWorld()));
				//getStudentWorld()->modifyNumOfBacteria(1);
				RegularSalmonellaInventory--;
				bacteriaSpawned = true;
			}
			else if ((chooseBacteriaToSpawn == 2) && (AggressiveSalmonellaInventory > 0))
			{
				getStudentWorld()->addToActorsVector(new AggressiveSalmonella(getX(), getY(), getStudentWorld()));
				//getStudentWorld()->modifyNumOfBacteria(1);
				AggressiveSalmonellaInventory--;
				bacteriaSpawned = true;
			}
			else if ((chooseBacteriaToSpawn == 3) && (EColiInventory > 0))
			{
				getStudentWorld()->addToActorsVector(new EColi(getX(), getY(), getStudentWorld()));
				//getStudentWorld()->modifyNumOfBacteria(1);
				EColiInventory--;
				bacteriaSpawned = true;
			}
		}
		//plays the sound when it spawns the new bacteria
		getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
	}

}
////////////////////////////
//SOCRATES IMPLEMENTATIONS
////////////////////////////


Socrates::Socrates(StudentWorld* inputStudentWorld, int imageID, Direction dir, double startX, double startY, int depth)
	: ActorBaseClass(IID_PLAYER, 0, 128, 0, 0, inputStudentWorld, 100)
{
	numOfSprayProjectiles = 20;
	numOfFlameThrowerCharges = 5;
	positionalAngle = 180;
}

int Socrates::getNumOfSprayProjectiles()
{
	return numOfSprayProjectiles;
}

int Socrates::getNumOfFlameThrowerCharges()
{
	return numOfFlameThrowerCharges;
}

void Socrates::modifyNumOfFlameThrowerCharges(int changeAmount)
{
	numOfFlameThrowerCharges += changeAmount;
}

void Socrates::modifyHP(int modifyAmount)
{
	if (modifyAmount < 0)
	{
		if (getHP() + modifyAmount <= 0)
		{
			getStudentWorld()->playSound(SOUND_PLAYER_DIE);
		}
		else
		{
			getStudentWorld()->playSound(SOUND_PLAYER_HURT);
		}
	}
	ActorBaseClass::modifyHP(modifyAmount);
}


void Socrates::doSomething()
{

	if (!getAliveStatus())
	{
		return;
	}
	int ch;
	if (getStudentWorld()->getKey(ch))
	{
		//steering with the arrow keys
		if (ch == KEY_PRESS_LEFT)
		{

			const double PI = 4 * atan(1);
			double newX = VIEW_RADIUS + (VIEW_RADIUS * cos((getDirection() + 180.00000 + 5.000000000) * 1.0 / 360 * 2 * PI));

			double newY = VIEW_RADIUS + (VIEW_RADIUS * sin((getDirection() + 180.00000 + 5.000000000) * 1.0 / 360 * 2 * PI));

			moveTo(newX, newY);
			setDirection(getDirection() + 5);
		}
		if (ch == KEY_PRESS_RIGHT)
		{
			const double PI = 4 * atan(1);
			double newX = VIEW_RADIUS + (VIEW_RADIUS * cos((getDirection() + 180.00000 - 5.00000) * 1.0 / 360 * 2 * PI));
			double newY = VIEW_RADIUS + (VIEW_RADIUS * sin((getDirection() + 180.00000 - 5.00000) * 1.0 / 360 * 2 * PI));

			moveTo(newX, newY);
			setDirection(getDirection() - 5);
		}

		//shoots the spray projectiles
		if (ch == KEY_PRESS_SPACE)
		{
			if (numOfSprayProjectiles > 0)
			{
				double shotXDirection = 0;
				double shotYDirection = 0;
				getPositionInThisDirection(getDirection(), SPRITE_RADIUS * 2, shotXDirection, shotYDirection);
				SprayProjectile* shotSpray = new SprayProjectile(shotXDirection, shotYDirection, getStudentWorld(), 4, getDirection(), 1, 1);
				numOfSprayProjectiles--;
				getStudentWorld()->addToActorsVector(shotSpray);
				getStudentWorld()->playSound(SOUND_PLAYER_SPRAY);
			}

		}

		//shoots the flames if the enter key is pressed
		if (ch == KEY_PRESS_ENTER)
		{
			if (numOfFlameThrowerCharges > 0)	//SHOULD HAVE numOfFlameThrowerCharges > 0
			{
				for (int i = 0; i < 16; i++)
				{
					double shotXDirection = 0;
					double shotYDirection = 0;
					getPositionInThisDirection(22 * i, SPRITE_RADIUS * 2, shotXDirection, shotYDirection);
					FlameProjectile* shotFlame = new FlameProjectile(shotXDirection, shotYDirection, getStudentWorld(), IID_FLAME, 22 * i, 1, 1);
					getStudentWorld()->addToActorsVector(shotFlame);
				}
				numOfFlameThrowerCharges--;
				getStudentWorld()->playSound(SOUND_PLAYER_FIRE);
			}
		}
	}
	else
	{
		if (numOfSprayProjectiles < 20)
		{
			numOfSprayProjectiles++;
		}
	}
}

////////////////////////////
//DIRTPILE IMPLEMENTATIONS
////////////////////////////


DirtPile::DirtPile(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth)
	:ActorBaseClass(imageID, startX, startY, dir, depth, inputStudentWorld, 1)
{}

void DirtPile::doSomething()
{
	SetAsDeadIfLessThan0HP();
}


bool DirtPile::sprayWillHarm()
{
	modifyHP(-1);
	return true;
}

bool DirtPile::flameWillHarm()
{
	return true;
}

bool DirtPile::blocksBacteriumMovement() const
{
	return true;
}

////////////////////////////
//SPRAY IMPLEMENTATIONS
////////////////////////////

SprayProjectile::SprayProjectile(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth, int inputHP)
	:ActorBaseClass(imageID, startX, startY, dir, depth, inputStudentWorld, 1)
{
	distanceTraveled = 0;
}


void SprayProjectile::doSomething()
{
	//TODO:CHECK FOR OVERLAP
	SetAsDeadIfLessThan0HP();

	//if it detects that it goes over something that takes damage from spray, the spray will set itself as dead
	bool temp = getStudentWorld()->wentOverSprayableObject(getX(), getY());
	if (temp == true)
	{
		this->setAsDead();
	}
	moveAngle(getDirection(), SPRITE_RADIUS * 2);
	distanceTraveled += SPRITE_RADIUS * 2;
	//otherwise part(bullet point 3)

	if (distanceTraveled >= 112)
	{
		setAsDead();
	}
}

FlameProjectile::FlameProjectile(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth, int inputHP)
	:ActorBaseClass(imageID, startX, startY, dir, depth, inputStudentWorld, 1)
{
	distanceTraveled = 0;
}


void FlameProjectile::doSomething()
{

	//if it goes over something that's flammable, it will set itself as dead
	SetAsDeadIfLessThan0HP();
	bool temp = getStudentWorld()->wentOverFlammableObject(getX(), getY());
	if (temp == true)
	{
		this->setAsDead();
	}
	moveAngle(getDirection(), SPRITE_RADIUS * 2);
	distanceTraveled += SPRITE_RADIUS * 2;

	//sets itself as dead if it travels 32 units
	if (distanceTraveled >= 32)
	{
		setAsDead();
	}
}
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Food::Food(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth, int inputHP)
	:ActorBaseClass(imageID, startX, startY, dir, depth, inputStudentWorld, inputHP)
{}

void Food::doSomething()
{
	SetAsDeadIfLessThan0HP();
}

bool Food::sprayWillHarm()
{
	return false;
}

bool Food::flameWillHarm()
{
	return false;
}

bool Food::isEdible()
{
	return true;
}
GoodieBaseClass::GoodieBaseClass(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth)
	:ActorBaseClass(imageID, startX, startY, dir, depth, inputStudentWorld)
{
	//sets a random lifetime as specified by the spec
	int randomTime = randInt(50, (300 - 10 * getStudentWorld()->getLevel()));
	lifetimeTicksTracker = 0;
	for (int i = 0; i < 20; i++)
	{
		randomTime = randInt(50, (300 - 10 * getStudentWorld()->getLevel()));
	}

	ticksBeforeSetAsDead = max(randomTime, 50);
}

bool ActorBaseClass::checkAliveAndIfOverlapWithSocrates()

{
	//returns true if it overlaps socratesd
	int distanceFromSocrates = getStudentWorld()->getDistanceFromSocrates(this);
	if (distanceFromSocrates < 2 * SPRITE_RADIUS)
	{
		return true;
	}

	return false;
}

void GoodieBaseClass::baseActionsIfOverlapWithSocrates(int pointsChange)
{
	//sets the goodie as dead
	StudentWorld* currentStudentWorldPointer = getStudentWorld();
	currentStudentWorldPointer->increaseScore(pointsChange);
	setAsDead();

	//plays GOT_GOODIE when it's an actual goodie
	if (pointsChange > 0)
	{
		currentStudentWorldPointer->playSound(SOUND_GOT_GOODIE);
	}
	//play sound_player_hurt when it hits a fungus
	else
	{
		currentStudentWorldPointer->playSound(SOUND_PLAYER_HURT);
	}
}

void GoodieBaseClass::trackAndDieIfExceedLifeTimeThenIncTick()
{
	//checks to see if it has reached lifetime limit
	if (lifetimeTicksTracker >= ticksBeforeSetAsDead)
	{
		//make itself disappear due to removealldeadactors function
		setAsDead();
	}
	lifetimeTicksTracker++;
}


bool GoodieBaseClass::sprayWillHarm()
{
	return true;
}

bool GoodieBaseClass::flameWillHarm()
{
	return true;
}

RestoreHealthGoodie::RestoreHealthGoodie(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth)
	:GoodieBaseClass(startX, startY, inputStudentWorld, imageID, dir, depth)
{}

void RestoreHealthGoodie::doSomething()
{
	if (checkAliveAndIfOverlapWithSocrates())
	{
		//cerr << "overlppaed with soc and goodie" << endl;
		baseActionsIfOverlapWithSocrates(250);

		getStudentWorld()->makeSocratesFullHP();
		return;
	}

	trackAndDieIfExceedLifeTimeThenIncTick();
}

FlameThrowerGoodie::FlameThrowerGoodie(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth)
	:GoodieBaseClass(startX, startY, inputStudentWorld, imageID, dir, depth)
{}

void FlameThrowerGoodie::doSomething()
{
	if (checkAliveAndIfOverlapWithSocrates())
	{
		//cerr << "overlppaed with soc and goodie" << endl;
		baseActionsIfOverlapWithSocrates(300);

		getStudentWorld()->flameThrowerGoodieEffect();
		return;
	}

	trackAndDieIfExceedLifeTimeThenIncTick();
}

ExtraLifeGoodie::ExtraLifeGoodie(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth)
	:GoodieBaseClass(startX, startY, inputStudentWorld, imageID, dir, depth)
{}

//if it overlaps, do the base actions plus giving one extra life
void ExtraLifeGoodie::doSomething()
{
	if (checkAliveAndIfOverlapWithSocrates())
	{
		baseActionsIfOverlapWithSocrates(500);

		getStudentWorld()->incLives();
		return;
	}

	trackAndDieIfExceedLifeTimeThenIncTick();
}

Fungus::Fungus(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth)
	:GoodieBaseClass(startX, startY, inputStudentWorld, imageID, dir, depth)
{}


void Fungus::doSomething()
{
	if (checkAliveAndIfOverlapWithSocrates())
	{
		//cerr << "overlppaed with soc and goodie" << endl;
		baseActionsIfOverlapWithSocrates(-50);

		getStudentWorld()->fungusEffect();
		return;
	}

	trackAndDieIfExceedLifeTimeThenIncTick();
}

bool Fungus::sprayWillHarm()
{
	return true;
}

bool Fungus::flameWillHarm()
{
	return true;
}

//Bacteria Implementation

Bacteria::Bacteria(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth, int inputHP)
	:ActorBaseClass(imageID, startX, startY, dir, depth, inputStudentWorld, inputHP)
{

	//when a new bacter is created, the number of bacteria tracker goes up by one
	foodEaten = 0;

	getStudentWorld()->modifyNumOfBacteria(1);
	movementPlanDistance = 0;
}

Bacteria::~Bacteria()
{
	//number of bacteria tracker goes down when the bacteria is destroyed
	getStudentWorld()->modifyNumOfBacteria(-1);
	int spawnChance = randInt(1, 2);
	if (spawnChance == 1)
	{
		getStudentWorld()->addToActorsVector(new Food(getX(), getY(), getStudentWorld()));
	}
}


void Bacteria::modifyFoodEaten(int modifyAmount)
{
	foodEaten += modifyAmount;
}

double Bacteria::newXAfter3Food(double inputX)
{
	if (inputX < VIEW_WIDTH / 2)
	{
		return inputX + SPRITE_RADIUS;
	}

	return inputX - SPRITE_RADIUS;
}

double Bacteria::newYAfter3Food(double inputY)
{
	if (inputY < VIEW_HEIGHT / 2)
	{
		return inputY + SPRITE_HEIGHT;
	}

	return inputY - SPRITE_HEIGHT;
}

bool Bacteria::sprayWillHarm()
{
	return true;
}

bool Bacteria::flameWillHarm()
{
	return true;
}

void Bacteria::modifyMovementPlanDistance(int modifyAmount)
{
	movementPlanDistance += modifyAmount;
}

int Bacteria::getMovementPlanDistance()
{
	return movementPlanDistance;
}

int Bacteria::getFoodEaten()
{
	return foodEaten;
}


void Bacteria::checkIfWentOverFoodAndIncrementIfSo()
{
	if (getStudentWorld()->wentOverFood(getX(), getY()))
	{
		modifyFoodEaten(+1);
		return;
	}
}

bool Bacteria::checkIfOverlappedWithSocratesAndModifySocratesHP(int socratesHPModifyAmount)
{
	if (checkAliveAndIfOverlapWithSocrates())
	{

		getStudentWorld()->modifySocratesHP(socratesHPModifyAmount);
		return true;	//tells socrates to take 1 damage	
	}
	return false;
}

void Bacteria::movementPlanMoveForward3AvoidDirt()
{
	modifyMovementPlanDistance(-1);
	double tempX;
	double tempY;
	getPositionInThisDirection(getDirection(), 3, tempX, tempY);

	//following if statement checks to see if it DOESN'T go outside of circle or go over dirtpile
	if ((getStudentWorld()->getEuclideanDistance(tempX, tempY, (VIEW_WIDTH / 2), (VIEW_HEIGHT / 2)) > VIEW_RADIUS) || (getStudentWorld()->wentOverDirtPile(tempX, tempY)))
	{
		int newDirection = randInt(0, 359);
		setDirection(newDirection);
		modifyMovementPlanDistance(10 - getMovementPlanDistance());

	}
	else
	{
		moveAngle(getDirection(), 3);
		//modifyMovementPlanDistance(-1);
	}
}

void Bacteria::lookAndGoAfterFoodWithin128()
{
	double newFoodX;
	double newFoodY;

	//if there is actually food within 128 units
	if (getStudentWorld()->findFoodWithin128(getX(), getY(), newFoodX, newFoodY))
	{
		const double PI = 4 * atan(1);
		double angle = (180.00000 / PI) * atan2(newFoodY - getY(), newFoodX - getX());
		//setDirection(angle);
		double newXAfterFoodFound;
		double newYAfterFoodFound;
		getPositionInThisDirection(angle, 3, newXAfterFoodFound, newYAfterFoodFound);
		if ((getStudentWorld()->getEuclideanDistance(newXAfterFoodFound, newYAfterFoodFound, (VIEW_WIDTH / 2), (VIEW_HEIGHT / 2)) > VIEW_RADIUS) || getStudentWorld()->wentOverDirtPile(newXAfterFoodFound, newYAfterFoodFound))
		{
			int randomDirection = randInt(0, 359);
			setDirection(randomDirection);
			modifyMovementPlanDistance(10 - getMovementPlanDistance());
		}
		else
		{
			setDirection(angle);
			moveAngle(getDirection(), 3);
		}
	}
	else
	{
		int randomDirection = randInt(0, 359);
		setDirection(randomDirection);
		modifyMovementPlanDistance(10 - getMovementPlanDistance());
	}
}
AggressiveSalmonella::AggressiveSalmonella(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth, int inputHP)
	:Bacteria(startX, startY, inputStudentWorld, IID_SALMONELLA, dir, depth, inputHP)	
{}

//this also takes into account the sounds
void AggressiveSalmonella::modifyHP(int modifyAmount)
{
	if (modifyAmount < 0)	//means it took damage
	{
		if (getHP() + modifyAmount > 0)
		{
			getStudentWorld()->playSound(SOUND_SALMONELLA_HURT);
		}
		else if(getHP() + modifyAmount <= 0)
		{
			getStudentWorld()->playSound(SOUND_SALMONELLA_DIE);
			getStudentWorld()->increaseScore(100);
			//getStudentWorld()->modifyNumOfBacteria(-1);
		}
	}
	ActorBaseClass::modifyHP(modifyAmount);
}
void AggressiveSalmonella::doSomething()
{
	SetAsDeadIfLessThan0HP();

	bool chasedAfterSocrates = false;
	bool overlappedWithSocratesThisTick = false;
	bool hasDividedThisTick = false;
	double tempXDistance;
	double tempYDistance;
	//goes after Socrates if its less than 72 units away
	if (getStudentWorld()->getDistanceFromSocrates(this) <= 72)
	{

		double tempSocratesX = 1;
		double tempSocratesY = 1;
		if (getStudentWorld()->findSocratesWithinDistance(getX(), getY(), tempSocratesX, tempSocratesY, 72))
		{
			const double PI = 4 * atan(1);
			double angle = (180.00000 / PI) * atan2(tempSocratesX - getY(), tempSocratesY - getX());
			double newXAfterSocratesFound;
			double newYAfterSocratesFound;
			getPositionInThisDirection(angle, 3, newXAfterSocratesFound, newYAfterSocratesFound);
			if ((getStudentWorld()->getEuclideanDistance(newXAfterSocratesFound, newYAfterSocratesFound, (VIEW_WIDTH / 2), (VIEW_HEIGHT / 2)) < VIEW_RADIUS) && !(getStudentWorld()->wentOverDirtPile(newXAfterSocratesFound, newYAfterSocratesFound)))
			{
				setDirection(angle);
				moveAngle(angle, 3);
			}
		}
	}

	overlappedWithSocratesThisTick = checkIfOverlappedWithSocratesAndModifySocratesHP(-2);

	if (!overlappedWithSocratesThisTick)
	{
		if (getFoodEaten() >= 3)
		{
			int newX = newXAfter3Food(getX());
			int newY = newYAfter3Food(getY());
			getStudentWorld()->addToActorsVector(new AggressiveSalmonella(newX, newY, getStudentWorld()));
			//getStudentWorld()->modifyNumOfBacteria(1);
			modifyFoodEaten(-1 * getFoodEaten());
			hasDividedThisTick = true;
		}

	}


	//step 5
	if (!overlappedWithSocratesThisTick && !hasDividedThisTick)//TODO:  &&
	{
		checkIfWentOverFoodAndIncrementIfSo();
	}

	//step 6

	if (getMovementPlanDistance() > 0 && !chasedAfterSocrates)
	{
		movementPlanMoveForward3AvoidDirt();
	}
	else
	{
		//step 7
		lookAndGoAfterFoodWithin128();
	}

}
Salmonella::Salmonella(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth, int inputHP)
	:Bacteria(startX, startY, inputStudentWorld, imageID, dir, 0, IID_SALMONELLA)
{}

void Salmonella::modifyHP(int modifyAmount)
{
	if (modifyAmount < 0)
	{
		if (getHP() + modifyAmount > 0)
		{
			getStudentWorld()->playSound(SOUND_SALMONELLA_HURT);
		}
		else
		{
			getStudentWorld()->playSound(SOUND_SALMONELLA_DIE);
			cerr << "bacteria died, current num of bac: " << getStudentWorld()->getNumOfBacteria();
			getStudentWorld()->increaseScore(100);
			//getStudentWorld()->modifyNumOfBacteria(-1);
		}
	}
	ActorBaseClass::modifyHP(modifyAmount);
}
void Salmonella::doSomething()
{
	SetAsDeadIfLessThan0HP();

	bool overlappedWithSocratesThisTick = false;
	bool hasDividedThisTick = false;
	//this allows us to skip steps later on
	overlappedWithSocratesThisTick = checkIfOverlappedWithSocratesAndModifySocratesHP(-1);

	if (!overlappedWithSocratesThisTick)
	{
		if (getFoodEaten() >= 3)
		{
			int newX = newXAfter3Food(getX());
			int newY = newYAfter3Food(getY());
			getStudentWorld()->addToActorsVector(new Salmonella(newX, newY, getStudentWorld()));
			//getStudentWorld()->modifyNumOfBacteria(1);
			modifyFoodEaten(-3);
			hasDividedThisTick = true;
		}
		
	}

	//only do so if it hasn't overlapped with Socrates or divided this tick
	if (!overlappedWithSocratesThisTick && !hasDividedThisTick)
	{

		checkIfWentOverFoodAndIncrementIfSo();
	}


	double possibleFoodX;
	double possibleFoodY;
	if (getMovementPlanDistance() > 0)
	{
		movementPlanMoveForward3AvoidDirt();
	}
	else
	{
		lookAndGoAfterFoodWithin128();
	}
}


EColi::EColi(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir, int depth, int inputHP)
	:Bacteria(startX, startY, inputStudentWorld, imageID, dir, depth, inputHP)
{}

void EColi::modifyHP(int modifyAmount)
{
	if (modifyAmount < 0)
	{
		if (getHP() + modifyAmount > 0)
		{
			getStudentWorld()->playSound(SOUND_ECOLI_HURT);
		}
		else
		{
			getStudentWorld()->playSound(SOUND_ECOLI_DIE);
			getStudentWorld()->increaseScore(100);
			//getStudentWorld()->modifyNumOfBacteria(-1);
		}
	}
	ActorBaseClass::modifyHP(modifyAmount);
}


void EColi::doSomething()
{
	SetAsDeadIfLessThan0HP();

	//these bools allow us to skip steps
	bool overlappedWithSocratesThisTick = false;
	bool hasDividedThisTick = false;

	overlappedWithSocratesThisTick = checkIfOverlappedWithSocratesAndModifySocratesHP(-4);

	if (!overlappedWithSocratesThisTick)
	{
		if (getFoodEaten() >= 3)
		{
			int newX = newXAfter3Food(getX());
			int newY = newYAfter3Food(getY());
			EColi* newEColi = new EColi(newX, newY, getStudentWorld());
			getStudentWorld()->addToActorsVector(newEColi);
			//getStudentWorld()->modifyNumOfBacteria(1);
			modifyFoodEaten(-1 * getFoodEaten());
			hasDividedThisTick = true;
		}

	}

	if (!overlappedWithSocratesThisTick && !hasDividedThisTick)//TODO: && 
	{

		checkIfWentOverFoodAndIncrementIfSo();
	}

	//step 5

	double tempSocratesX;
	double tempSocratesY;
	if (getStudentWorld()->findSocratesWithinDistance(getX(), getY(), tempSocratesX, tempSocratesY, 256))
	{
		const double PI = 4 * atan(1);
		double angle = (180.00000 / PI) * atan2(tempSocratesY - getY(), tempSocratesX - getX());

		double newXChasingSocrates;
		double newYChasingSocrates;
		getPositionInThisDirection(angle, 2, newXChasingSocrates, newYChasingSocrates);
		if (!(getStudentWorld()->getEuclideanDistance(newXChasingSocrates, newYChasingSocrates, (VIEW_WIDTH / 2), (VIEW_HEIGHT / 2)) > VIEW_RADIUS) && !(getStudentWorld()->wentOverDirtPile(newXChasingSocrates, newYChasingSocrates)))
		{
			setDirection(angle);
			moveAngle(angle, 2);
			return;
		}
		//TODO:IS THIS THE RIGHT WAY?

	}
}
