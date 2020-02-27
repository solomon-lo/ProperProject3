#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "GraphObject.h"
#include <vector>
#include <iostream>
#include <string>

class Socrates;
class ActorBaseClass;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

//using Direction = int;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	~StudentWorld();
	void addToActorsVector(ActorBaseClass* actorToAdd);
	//Socrates* getPlayerObject();


	bool wentOverSprayableObject(int centerActorX, int centerActorY);
	bool wentOverFlammableObject(int centerActorX, int centerActorY);
	bool wentOverFood(int centerActorX, int centerActorY);
	bool wentOverDirtPile(int centerActorX, int centerActorY);
	bool isThisCoordinateFilled(double testX, double textY);

	//helper functions
	void removeDeadActors();
	double getPlayerObjectHealth();
	int getPlayerObjectSpraysLeft();
	int getPlayerObjectFlamesLeft();
	double getEuclideanDistance(double baseX, double baseY, double newX, double newY);
	double getDistanceFromSocrates(ActorBaseClass* targetActor);
	void makeSocratesFullHP();
	void modifySocratesHP(int modifyAmount);
	void flameThrowerGoodieEffect();
	void fungusEffect();
	bool findFoodWithin128(double bacteriaX, double bacteriaY, double& foodX, double& foodY);
	bool findSocratesWithinDistance(double bacteriaX, double bacteriaY, double& SocratesX, double& SocratesY, int inputDistance);

private:
	Socrates* playerObject;
	std::vector<ActorBaseClass*> ActorsVector;

};

#endif // STUDENTWORLD_H_	