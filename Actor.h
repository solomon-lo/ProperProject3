#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class ActorBaseClass : public GraphObject
{
public:
	//constructor
	ActorBaseClass(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* inputStudentWorld, int inputHP = 1);

	virtual void doSomething() = 0;

	//returns true if alive, false if dead
	bool getAliveStatus();	

	//sets the aliveStatus to dead
	void setAsDead();

	//returns a pointer to the studentWorld
	virtual StudentWorld* getStudentWorld();

	//destructs the Actor Base class
	virtual ~ActorBaseClass();

	//returns the health points as a double
	double getHP();

	bool SetAsDeadIfLessThan0HP();

	virtual void modifyHP(int modifyAmount);

	bool checkAliveAndIfOverlapWithSocrates();


	//these return if spray or flame projectile will actually hurt the Actor
	virtual bool sprayWillHarm();
	virtual bool flameWillHarm();

	virtual bool isEdible();

	//returns if bacterium movement IS BLOCKED
	virtual bool blocksBacteriumMovement() const;


private:

	bool aliveStatus;
	StudentWorld* m_StudentWorld;
	double HP;
};


class Pit : public ActorBaseClass
{
public:

	//CONSTRUCTOR FOR PIT
	Pit(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_PIT, Direction dir = 0, int depth = 1);
	void doSomething();
private:

	//these track how much is left to spawn
	int RegularSalmonellaInventory;	
	int AggressiveSalmonellaInventory;
	int EColiInventory;
};
class DirtPile : public ActorBaseClass
{
public:


	DirtPile(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_DIRT, Direction dir = 0, int depth = 1);

	virtual void doSomething();

	virtual bool sprayWillHarm();

	virtual bool flameWillHarm();

	virtual bool blocksBacteriumMovement() const;

private:
};

class Socrates : public ActorBaseClass
{
public:
	Socrates(StudentWorld* inputStudentWorld, int imageID = IID_PLAYER, Direction dir = 0, double startX = 0, double startY = 128, int depth = 0);

	virtual void doSomething();

	int getNumOfSprayProjectiles();

	int getNumOfFlameThrowerCharges();

	void modifyNumOfFlameThrowerCharges(int changeAmount);

	//changes the health points of the socrates
	virtual void modifyHP(int modifyAmount);



private:
	//inventory of how many of each are left to use
	int numOfSprayProjectiles;
	int numOfFlameThrowerCharges;

	int positionalAngle;

};


class SprayProjectile : public ActorBaseClass
{
public:
	SprayProjectile(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_SPRAY, Direction dir = 0, int depth = 1, int inputHP = 1);

	virtual void doSomething();

private:
	int distanceTraveled;
};

class FlameProjectile : public ActorBaseClass
{
public:
	FlameProjectile(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_FLAME, Direction dir = 0, int depth = 1, int inputHP = 1);

	virtual void doSomething();

private:
	int distanceTraveled;
};

class Food : public ActorBaseClass
{
public:
	Food(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_FOOD, Direction dir = 90, int depth = 1, int inputHP = 1);

	virtual void doSomething();
	virtual bool sprayWillHarm();

	virtual bool flameWillHarm();

	virtual bool isEdible();
};

class GoodieBaseClass : public ActorBaseClass
{
public:

	GoodieBaseClass(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir = 0, int depth = 1);


	//the actions that all socrates objects go through, such as dying and changing points
	void baseActionsIfOverlapWithSocrates(int pointsChange);
	
	//kills the goodie if it exceeds the amount of time
	void trackAndDieIfExceedLifeTimeThenIncTick();

	virtual bool sprayWillHarm();
	virtual bool flameWillHarm();



private:
	int lifetimeTicksTracker;
	int ticksBeforeSetAsDead;
};

class RestoreHealthGoodie : public GoodieBaseClass
{
public:

	RestoreHealthGoodie(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_RESTORE_HEALTH_GOODIE, Direction dir = 0, int depth = 1);

	virtual void doSomething();
};

class FlameThrowerGoodie : public GoodieBaseClass
{
public:

	FlameThrowerGoodie(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_FLAME_THROWER_GOODIE, Direction dir = 0, int depth = 1);

	virtual void doSomething();
};

class ExtraLifeGoodie : public GoodieBaseClass
{
public:

	ExtraLifeGoodie(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_EXTRA_LIFE_GOODIE, Direction dir = 0, int depth = 1);

	virtual void doSomething();
};

class Fungus : public GoodieBaseClass
{
public:

	Fungus(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_FUNGUS, Direction dir = 0, int depth = 1);

	virtual void doSomething();

	bool flameWillHarm();
	bool sprayWillHarm();
};

class Bacteria : public ActorBaseClass
{
public:

	Bacteria(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir = 90, int depth = 0, int inputHP = 4);
	virtual bool sprayWillHarm();
	virtual bool flameWillHarm();

	//adds the modifyAmount to the current amount of food eaten
	void modifyFoodEaten(int modifyAmount);
	void modifyMovementPlanDistance(int modifyAmount);

	//for calculating the new spawn location
	double newXAfter3Food(double inputX);
	double newYAfter3Food(double inputY);


	int getFoodEaten();
	~Bacteria();

	void movementPlanMoveForward3AvoidDirt();

	void lookAndGoAfterFoodWithin128();

	void checkIfWentOverFoodAndIncrementIfSo();
	bool checkIfOverlappedWithSocratesAndModifySocratesHP(int socratesHPModifyAmount);

	//virtual bool preventsLevelCompleting() const;
	int getMovementPlanDistance();
	virtual void doSomething() = 0;
private:
	int foodEaten;
	int movementPlanDistance;
};

class Salmonella : public Bacteria
{
public:

	Salmonella(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_SALMONELLA, Direction dir = 90, int depth = 0, int inputHP = 4);
	virtual void doSomething();
	virtual void modifyHP(int modifyAmount);
};

class AggressiveSalmonella : public Bacteria
{
public:
	AggressiveSalmonella(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_SALMONELLA, Direction dir = 90, int depth = 0, int inputHP = 10);
	virtual void doSomething();
	virtual void modifyHP(int modifyAmount);
};

class EColi : public Bacteria
{
public:
	EColi(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_ECOLI, Direction dir = 90, int depth = 0, int inputHP = 5);
	virtual void modifyHP(int modifyAmount);
	virtual void doSomething();

};


#endif // ACTOR_H_