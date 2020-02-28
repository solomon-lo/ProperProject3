#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include "GraphObject.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

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

	// all of these return true if they actually go over the object, and false otherwise
	bool wentOverSprayableObject(int centerActorX, int centerActorY);
	bool wentOverFlammableObject(int centerActorX, int centerActorY);
	bool wentOverFood(int centerActorX, int centerActorY);
	bool wentOverDirtPile(int centerActorX, int centerActorY);


	bool isThisCoordinateFilled(double testX, double textY);
	void updateDisplayText();
	//helper functions
	void removeDeadActors();
	double getPlayerObjectHealth();
	int getPlayerObjectSpraysLeft();
	int getPlayerObjectFlamesLeft();

	//uses trigonmetry to calcuate Euclidean idstance
	double getEuclideanDistance(double baseX, double baseY, double newX, double newY);
	double getDistanceFromSocrates(ActorBaseClass* targetActor);


	void makeSocratesFullHP();
	void modifySocratesHP(int modifyAmount);
	void flameThrowerGoodieEffect();
	void fungusEffect();
	bool findFoodWithin128(double bacteriaX, double bacteriaY, double& foodX, double& foodY);
	bool findSocratesWithinDistance(double bacteriaX, double bacteriaY, double& SocratesX, double& SocratesY, int inputDistance);
	
	//needed for win condition
	int getNumOfPits();
	int getNumOfBacteria();
	void modifyNumOfPits(int modifyAmount);
	void modifyNumOfBacteria(int modifyAmount);

private:
	Socrates* playerObject;
	std::vector<ActorBaseClass*> ActorsVector;

	int numOfPits;
	int numOfBacteria;

};

#endif // STUDENTWORLD_H_	