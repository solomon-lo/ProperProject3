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

	//Not sure if we have to implement these virtual functions from GraphObject
	////virtual function to move, MUST HAVE
	//virtual void moveTo(double x, double y);

	////virtual function, MUST HAVE
	//virtual void moveAngle(Direction angle, int units = 1);

	////virtual function, MUST HAVE
	//virtual void getPositionInThisDirection(Direction angle, int units, double& dx, double& dy);

	//virtual function that's pure
	virtual void doSomething() = 0;

	//returns true if alive, false if dead
	virtual bool getAliveStatus();

	//sets the bool tracker of alive or not to false to symbolize death
	virtual void setAsDead();

	//returns pointer to the studentWorld
	virtual StudentWorld* getStudentWorld();

	//virtual destructor, essential due to inheritance
	virtual ~ActorBaseClass();

	//returns the hitpoints of the actor
	virtual double getHP();

	//checks if there's less than 0 HP, logically sets it to dead
	virtual bool SetAsDeadIfLessThan0HP();

	//changes the modifyHP by added whatever modifyAmount is to it
	virtual void modifyHP(int modifyAmount);

	//sees if it overlaps with Socrates
	virtual bool checkAliveAndIfOverlapWithSocrates();

	//checks to see if spray will harm it, returns true if it will
	virtual bool sprayWillHarm();

	//checks to see if flame will harm it, returns true if it willx
	virtual bool flameWillHarm();

	//returns whether or not the object can be eaten and then destroyed by Socrates
	virtual bool isEdible();

	//returns whether or not it will cause the bacteria to stop and change direction, then go
	virtual bool blocksBacteriumMovement() const;


private:

	bool aliveStatus;
	StudentWorld* m_StudentWorld;
	double HP;
};


class Pit : public ActorBaseClass
{
public:
	//constrcutor
	Pit(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_PIT, Direction dir = 0, int depth = 1);

	//doSomething
	void doSomething();
private:
	int RegularSalmonellaInventory;
	int AggressiveSalmonellaInventory;
	int EColiInventory;
};
class DirtPile : public ActorBaseClass
{
public:
	DirtPile(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_DIRT, Direction dir = 0, int depth = 1);

	virtual void doSomething();

	//returns how sprays will do damage to dirtpiles
	virtual bool sprayWillHarm();


	//returns how flames will do damage to dirtpiles
	virtual bool flameWillHarm();

	virtual bool blocksBacteriumMovement() const;

private:
};

class Socrates : public ActorBaseClass
{
public:
	Socrates(StudentWorld* inputStudentWorld, int imageID = IID_PLAYER, Direction dir = 0, double startX = 0, double startY = 128, int depth = 0);

	void doSomething();

	int getPositionalAngle();



	int getNumOfSprayProjectiles();

	int getNumOfFlameThrowerCharges();

	void modifyNumOfFlameThrowerCharges(int changeAmount);

	virtual void restoreSocratesFullHP();

	virtual void modifyHP(int modifyAmount);



private:
	int numOfSprayProjectiles;
	int numOfFlameThrowerCharges;
	int positionalAngle;

};


class SprayProjectile : public ActorBaseClass
{
public:
	SprayProjectile(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_SPRAY, Direction dir = 0, int depth = 1, int inputHP = 1);

	void doSomething();

private:
	int distanceTraveled;
};

class FlameProjectile : public ActorBaseClass
{
public:
	FlameProjectile(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_FLAME, Direction dir = 0, int depth = 1, int inputHP = 1);

	void doSomething();

private:
	int distanceTraveled;
};

class Food : public ActorBaseClass
{
public:
	Food(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_FOOD, Direction dir = 90, int depth = 1, int inputHP = 1);

	void doSomething();
	virtual bool sprayWillHarm();

	virtual bool flameWillHarm();

	virtual bool isEdible();
};

class GoodieBaseClass : public ActorBaseClass
{
public:

	GoodieBaseClass(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir = 0, int depth = 1);



	virtual void baseActionsIfOverlapWithSocrates(int pointsChange);
	virtual void trackAndDieIfExceedLifeTimeThenIncTick();

	bool sprayWillHarm();
	bool flameWillHarm();



private:
	int lifetimeTicksTracker;
	int ticksBeforeSetAsDead;
};

class RestoreHealthGoodie : public GoodieBaseClass
{
public:

	RestoreHealthGoodie(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_RESTORE_HEALTH_GOODIE, Direction dir = 0, int depth = 1);

	void doSomething();
};

class FlameThrowerGoodie : public GoodieBaseClass
{
public:

	FlameThrowerGoodie(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_FLAME_THROWER_GOODIE, Direction dir = 0, int depth = 1);

	void doSomething();
};

class ExtraLifeGoodie : public GoodieBaseClass
{
public:

	ExtraLifeGoodie(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_EXTRA_LIFE_GOODIE, Direction dir = 0, int depth = 1);

	void doSomething();
};

class Fungus : public GoodieBaseClass
{
public:

	Fungus(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_FUNGUS, Direction dir = 0, int depth = 1);

	void doSomething();

	bool flameWillHarm();
	bool sprayWillHarm();
};

class Bacteria : public ActorBaseClass
{
public:


	Bacteria(double startX, double startY, StudentWorld* inputStudentWorld, int imageID, Direction dir = 90, int depth = 0, int inputHP = 4);
	virtual bool sprayWillHarm();
	virtual bool flameWillHarm();
	void modifyFoodEaten(int modifyAmount);
	void modifyMovementPlanDistance(int modifyAmount);
	double newXAfter3Food(double inputX);
	double newYAfter3Food(double inputY);
	int getFoodEaten();
	virtual ~Bacteria();
	void movementPlanMoveForward3AvoidDirt();

	void lookAndGoAfterFoodWithin128();

	void checkIfWentOverFoodAndIncrementIfSo();
	bool checkIfOverlappedWithSocratesAndModifySocratesHP(int socratesHPModifyAmount);

	//virtual bool preventsLevelCompleting() const;
	int getMovementPlanDistance();
	void doSomething() = 0;
private:
	int foodEaten;
	int movementPlanDistance;
};

class Salmonella : public Bacteria
{
public:

	Salmonella(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_SALMONELLA, Direction dir = 90, int depth = 0, int inputHP = 4);
	void doSomething();
	virtual void modifyHP(int modifyAmount);
};

class AggressiveSalmonella : public Bacteria
{
public:
	AggressiveSalmonella(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_SALMONELLA, Direction dir = 90, int depth = 0, int inputHP = 10);
	void doSomething();
	virtual void modifyHP(int modifyAmount);
};

class EColi : public Bacteria
{
public:
	EColi(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_ECOLI, Direction dir = 90, int depth = 0, int inputHP = 5);
	virtual void modifyHP(int modifyAmount);
	void doSomething();

};


#endif // ACTOR_H_