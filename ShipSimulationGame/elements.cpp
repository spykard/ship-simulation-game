#include "stdafx.h"
#include "elements.h"

// ### GAME BALANCING ###
#define STEAL 10	// Stealing amount by Pirate
#define REPAIR 5    // Repair cost on Repair

#define CHANCET 3   // Chance of Treasure
#define CHANCEL 1   // Chance of Port
// ######################

using namespace std;

int Ship::count1 = 0; //
int Ship::count2 = 0; // Count of each Ship Type
int Ship::count3 = 0; // since the very Start
int Ship::count4 = 0; //
int Ship::FunFact1 = 0;   // FUN FACT 1 : Stole
int Ship::FunFact2 = 0; // FUN FACT 2 : Afflicted Damage
int Ship::FunFact3 = 0; // FUN FACT 3 : Profit from Repairs

// ### Functions of Element ###
void Element::setPosition(int x, int y){
     tx = x;
     ty = y;}

int Element::getPositionX(){
	return tx;}

int Element::getPositionY(){
	return ty;}

void Element::setWeather(int x){
     tWeather = x;}

int Element::getWeather(){
	return tWeather;}

void Element::setTreas(int x){
    if (x <= CHANCET)
        treas = true;
    else
        treas = false;}

bool Element::getTreas(){
	return treas;}

void Element::setDock(int x){
    if (x <= CHANCEL)
        tdock = true;
    else
        tdock = false;}

bool Element::getDock(){
	return tdock;}

Element::Element(int x, int y){
	this->setPosition(x, y);}

Element::Element(){}
Element::~Element(){}

// ### Functions of Ship ###
void Ship::setPositionX(int x){
	px = x;}

int Ship::getPositionX(){
	return px;}

void Ship::setPositionY(int y){
	py = y;}

int Ship::getPositionY(){
	return py;}

void Ship::setDamage(int x){
	Damage = x;}

int Ship::getDamage(){
	return Damage;}

void Ship::setMaxPower(int x){
	tMaxPower = x;}

int Ship::getMaxPower(){
	return tMaxPower;}

void Ship::setPower(int x){
	tPower = x;}

int Ship::getPower(){
	return tPower;}

void Ship::setSpeed(int x){
	tSpeed = x;}

int Ship::getSpeed(){
	return tSpeed;}

void Ship::setSupply(int x){
    if (x < 0) tSupply = 0;	  // Making Sure
	else tSupply = x;}

int Ship::getSupply(){
	return tSupply;}

void Ship::setType(int x){
	tType = x;}

int Ship::getType(){
	return tType;}

void Ship::UseAbility(Ship * x)
{	// -----------------------
	// tType == 1  Pirate
	// tType == 2  BigPirate
	// tType == 3  Cargo
	// tType == 4  BigCargo
	// tType == 5  Repairer
	// tType == 6  Explorer
	// -----------------------

    if (this->getType() == 1 || this->getType() == 2)
	{
        cout << "| Pirate Attacked ";
		// Damage
		x->setPower((x->getPower()) - this->getDamage());
		// Steal
		int stealAmount;
		if (x->getSupply()-STEAL >= 0) {
			stealAmount = STEAL;
			this->setSupply((this->getSupply()) + stealAmount);
			x->setSupply((x->getSupply()) - stealAmount);
			FunFact1 += stealAmount;}
        else {
			stealAmount = x->getSupply();
			this->setSupply((this->getSupply()) + stealAmount);
			x->setSupply((x->getSupply()) - stealAmount);
			FunFact1 += stealAmount;}
		FunFact2++;
	}
    else if (this->getType() == 3 || this->getType() == 4) 
	{  /* Was Applied in CheckAroundShips() */  }
    else if (this->getType() == 5)
	{  	
        if (x->getSupply()-REPAIR >= 0) {  // Check if client has enough Supply
            this->setSupply((this->getSupply()) + REPAIR);
            cout << "| Repairer helped nearby Ship ";
            x->setPower((x->getPower()) + 3);
			x->setPower((x->getMaxPower()) + 3);
			FunFact3 += REPAIR;}
        else {
            cout << "| Repairer couldn't help since Ship couldn't pay ";}}
    else if (this->getType() == 6)
	{  /* Was Applied in CheckAroundShips() */  }
}

Ship::Ship(){}
Ship::~Ship(){}

// ### Functions of Pirate ###
Pirate::Pirate(int x, int y, int z, int k)
{	this->setPositionX(x);
	this->setPositionY(y);
	this->setDamage(z);
	this->setMaxPower(k);
	this->setPower(k);
	this->setSpeed(1);
	this->setSupply(0);
	this->setType(1);
	count1++;
	TotalMoves = 0;
}

Pirate::Pirate(){}
Pirate::~Pirate(){}

// ### Functions of BigPirate ###
BigPirate::BigPirate(int x, int y, int z, int k)
{	this->setPositionX(x);
	this->setPositionY(y);
	this->setDamage(z);
	this->setMaxPower(k);
	this->setPower(k);
	this->setSpeed(1);
	this->setSupply(0);
	this->setType(2);
    count1++;
	TotalMoves = 0;
}

BigPirate::~BigPirate(){}

// ### Functions of Cargo ###
Cargo::Cargo(int x, int y, int z, int k, int p)
{	this->setPositionX(x);
	this->setPositionY(y);
	this->setDamage(0);
	this->setMaxPower(z);
	this->setPower(z);
	this->setSpeed(k);
	this->setSupply(p);
	this->setType(3);
	count2++;
	TotalMoves = 0;
}

Cargo::Cargo(){}
Cargo::~Cargo(){}

// ### Functions of BigCargo ###
BigCargo::BigCargo(int x, int y, int z, int k, int p)
{	this->setPositionX(x);
	this->setPositionY(y);
	this->setDamage(0);
	this->setMaxPower(z);
	this->setPower(z);
	this->setSpeed(k);
	this->setSupply(p);
	this->setType(4);
	count2++;
	TotalMoves = 0;
}

BigCargo::~BigCargo(){}

// ### Functions of Repairer ###
Repairer::Repairer(int x, int y, int z, int k)
{	this->setPositionX(x);
	this->setPositionY(y);
	this->setDamage(0);
	this->setMaxPower(z);
	this->setPower(z);
	this->setSpeed(k);
	this->setSupply(0);
	this->setType(5);
	count3++;
	TotalMoves = 0;
}

Repairer::~Repairer(){}

// ### Functions of Explorer ###
Eksereunisis::Eksereunisis(int x, int y, int z, int k, int p)
{	this->setPositionX(x);
	this->setPositionY(y);
	this->setDamage(0);
	this->setMaxPower(z);
	this->setPower(z);
	this->setSpeed(k);
	this->setSupply(p);
	this->setType(6);
	count4++;
	TotalMoves = 0;
}

Eksereunisis::~Eksereunisis(){}