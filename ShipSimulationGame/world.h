#ifndef KOSMOS_H
#define KOSMOS_H
#include "elements.h"

using namespace std;

class World
{
    private:
    public:
        static int simCount;
        int Height;
        int Width;
        vector <vector <Element>> Ground;
        vector <Ship> Ships;
        vector <Ship> ShipCreation;
        void CreateShips(int, int, int, int, int, int);
        void CreateShipsRoll(TRandomMersenne *);
        void Movement(int, int);
		void UserMovement(int, int, int, int);
        int CheckAroundRoll(Ship, int, TRandomMersenne *);  // 1 = UP, 2 = RIGHT, 3 = DOWN, 4 = LEFT
        void DrawEnvironmentMap();
        void DrawShipsMap(int);
        void PrintWeather();
        void PrintShipStats();
		void PrintHealthBar(Ship *);
        void ApplyWeather();
        void ApplyTreasure();
        void ApplyDocks();
        void CheckAroundShips();
		int ShipDestroyer(int);
		int AssignPlayerShip(int);
        World();
        World(int, int);
        ~World();
};

void ColourSet(WORD *, WORD);
void ColourReset(WORD);

#endif