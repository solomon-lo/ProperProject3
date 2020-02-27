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


	virtual void doSomething() = 0;

	virtual bool getAliveStatus();

	virtual void setAsDead();

	virtual StudentWorld* getStudentWorld();

	virtual ~ActorBaseClass();

	virtual double getHP();

	virtual bool SetAsDeadIfLessThan0HP();

	virtual void modifyHP(int modifyAmount);

	virtual bool checkAliveAndIfOverlapWithSocrates();

	virtual bool sprayWillHarm();
	virtual bool flameWillHarm();

	virtual bool isEdible();

	virtual bool blocksBacteriumMovement() const;


private:

	bool aliveStatus;
	StudentWorld* m_StudentWorld;
	double HP;
};


class Pit : public ActorBaseClass
{
public:
	Pit(double startX, double startY, StudentWorld* inputStudentWorld, int imageID = IID_PIT, Direction dir = 0, int depth = 1);
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

	virtual bool sprayWillHarm();

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

	void changePositionalAngle(int change);

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