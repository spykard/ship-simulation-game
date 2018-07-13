#ifndef ELEMENTS_H
#define ELEMENTS_H

using namespace std;

class Element
{
	private:
		int tx;
		int ty;
		int tWeather;
		bool treas;
 		bool tdock;

	public:
		void setPosition(int, int);
		void setWeather(int);
		void setTreas(int);
		void setDock(int);
		int getPositionX();
		int getPositionY();
		int getWeather();
		bool getTreas();
		bool getDock();
		Element(int,int);
		Element();
		~Element();
};

class Ship
{
	private:
		int px;
		int py;
		int Damage;
		int tMaxPower;
		int tPower;
		int tSpeed;
		int tSupply;
		int tType;

	public:
		static int count1;
		static int count2;
		static int count3;
		static int count4;
		static int FunFact1;
		static int FunFact2;
		static int FunFact3;
		int TotalMoves; 
		// DamageTaken -> MaxPower - Power
		void setPositionX(int);
		void setPositionY(int);
		void setDamage(int);
		void setMaxPower(int);
		void setPower(int);
		void setSpeed(int);
		void setSupply(int);
		void setType(int);
		void UseAbility(Ship *);

		int getPositionX();
		int getPositionY();
		int getDamage();
		int getMaxPower();
		int getPower();
		int getSpeed();
		int getSupply();
		int getType();

		Ship();
		virtual ~Ship();
};

class Pirate:public Ship
{
	private:
	public:
		Pirate(int, int, int, int);
		Pirate();
		~Pirate();
};

class BigPirate:public Pirate  // Custom 1
{
	private:
	public:
		BigPirate(int, int, int, int);
		~BigPirate();
};

class Cargo:public Ship
{
	private:
	public:
		Cargo(int, int, int, int, int);
		Cargo();
		~Cargo();
};

class BigCargo:public Cargo  // Custom 2
{
	private:
	public:
		BigCargo(int, int, int, int, int);
		~BigCargo();
};

class Repairer:public Ship
{
	private:
	public:
		Repairer(int, int, int, int);
		~Repairer();
};

class Eksereunisis:public Ship
{
	private:
	public:
		Eksereunisis(int, int, int, int, int);
		~Eksereunisis();
};
#endif