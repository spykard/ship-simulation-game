#include "stdafx.h"
#include "world.h"

// ### GAME BALANCING ###
#define DOCKS 5    // Treasure given by Docks
#define DAMAGE 1   // Damage of Pirate Ships
#define POWER1 15  // Health of Pirate Ships
#define POWER2 8   // Health of Cargo Ships
#define POWER3 9   // Health of Repairer Ships
#define POWER4 4   // Health of Explorer Ships
#define SPEED 3    // Speed 1-3
#define SUPPLY 35  // Starting Supply 1-35

#define BIGPOWER 20     // Health of BigPirate Ships
#define BIGSUPPLY 40    // Starting Supply of BigCargo
#define TREASURECHEST 2 // Value of Treasure Chest
#define WEATHERDAMAGE 6 // Minimum Weather Value that causes Damage
// ######################

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;

void ColourSet(WORD *, WORD);
void ColourReset(WORD);

int World::simCount = 0;

World::World(int x, int y)
{	// Creates World Environment - Weather, Treasure, Docks

	int32 seed = (int32)time(0);
	TRandomMersenne rg(seed);

    for (int i=1; i<=x; i++) {
        vector<Element> row;
        for (int j=1; j<=y; j++) {
            row.push_back(Element(i,j));
			row[j-1].setWeather(rg.IRandom(0, 9));
			row[j-1].setTreas(rg.IRandom(1, 20));
			row[j-1].setDock(rg.IRandom(1, 20));
			// If Dock and Treasure on same Tile, remove Treasure
			if (row[j-1].getDock() == true) row[j-1].setTreas(10); 
		}                                                         
    Ground.push_back(row);
    }
	simCount++;
    Height = x;
    Width = y;
}

World::World() {}
World::~World(){}

void World::CreateShips(int x, int y, int z, int m, int k, int p)
{	// Create Ships of all Types
	
	int32 seed = (int32)time(0);
	TRandomMersenne rg(seed);

    int i = 0;
    for (; i<x; i++) {
        CreateShipsRoll(&rg);
        Ships.push_back(Pirate(ShipCreation[i].getPositionX(), ShipCreation[i].getPositionY(), DAMAGE, POWER1));}
    for (; i<x+y; i++) {
        CreateShipsRoll(&rg);
        Ships.push_back(BigPirate(ShipCreation[i].getPositionX(), ShipCreation[i].getPositionY(), DAMAGE, BIGPOWER));}
    for (; i<x+y+z; i++) {
        CreateShipsRoll(&rg);
        Ships.push_back(Cargo(ShipCreation[i].getPositionX(), ShipCreation[i].getPositionY(), POWER2, rg.IRandom(1, SPEED), rg.IRandom(1, SUPPLY)));}
    for (; i<x+y+z+m; i++) {
        CreateShipsRoll(&rg);
        Ships.push_back(BigCargo(ShipCreation[i].getPositionX(), ShipCreation[i].getPositionY(), POWER2, rg.IRandom(1, SPEED), BIGSUPPLY));}
    for (; i<x+y+z+m+k; i++) {
        CreateShipsRoll(&rg);
        Ships.push_back(Repairer(ShipCreation[i].getPositionX(), ShipCreation[i].getPositionY(), POWER3, rg.IRandom(1, SPEED)));}
    for (; i<x+y+z+m+k+p; i++) {
        CreateShipsRoll(&rg);
        Ships.push_back(Eksereunisis(ShipCreation[i].getPositionX(), ShipCreation[i].getPositionY(), POWER4, 1, rg.IRandom(1, SUPPLY)));}
}

void World::CreateShipsRoll(TRandomMersenne * mersenneRand) 
{	// Get Ship starting Coordinates by rolling a Number Generator

	int RX = mersenneRand->IRandom(1, Height);
	int RY = mersenneRand->IRandom(1, Width);
    if (Ground[RX-1][RY-1].getDock() == true) {
        CreateShipsRoll(mersenneRand);  // RE-ROLL
        return;}
    for (unsigned int i=0; i<ShipCreation.size(); i++) {
        if (ShipCreation[i].getPositionX()==RX && ShipCreation[i].getPositionY()==RY) {
                CreateShipsRoll(mersenneRand);  // RE-ROLL
                return;}}
	// Found, save them as any type of Ship
    ShipCreation.push_back(Repairer(RX, RY, 1, 1));  
}

void World::Movement(int userShip, int user2Ship)
{	// Apply Movement to 1 of 4 Directions, userInput is optional

	int32 seed = (int32)time(0);
	TRandomMersenne rg(seed);

    for (unsigned int i=0; i<Ships.size(); i++) {
        for (int y=0; y<Ships[i].getSpeed(); y++) {
			if (userShip != i && user2Ship != i) {
				int rr=CheckAroundRoll(Ships[i], 2, &rg);  // Mode 2
				if (rr==0) {}
				else if (rr==1) {Ships[i].setPositionY(Ships[i].getPositionY()-1); Ships[i].TotalMoves++;}  // 1 = LEFT
				else if (rr==2) {Ships[i].setPositionX(Ships[i].getPositionX()+1); Ships[i].TotalMoves++;}  // 2 = DOWN
				else if (rr==3) {Ships[i].setPositionY(Ships[i].getPositionY()+1); Ships[i].TotalMoves++;}  // 3 = RIGHT
				else if (rr==4) {Ships[i].setPositionX(Ships[i].getPositionX()-1); Ships[i].TotalMoves++;}  // 4 = UP
			}
		}
	}
}

void World::UserMovement(int userShip, int userInput, int user2Ship, int user2Input)
{	// Apply Movement to a Player Ship only

	int i = userShip;

	if (userShip >= 0) {
		if (userInput == 65 || userInput == 97) { Ships[i].setPositionY(Ships[i].getPositionY()-1); Ships[i].TotalMoves++; }       // LEFT (A)
		else if (userInput == 83 || userInput == 115) { Ships[i].setPositionX(Ships[i].getPositionX()+1); Ships[i].TotalMoves++; } // DOWN (S)
		else if (userInput == 68 || userInput == 100) { Ships[i].setPositionY(Ships[i].getPositionY()+1); Ships[i].TotalMoves++; } // RIGHT (D)
		else if (userInput == 87 || userInput == 119) { Ships[i].setPositionX(Ships[i].getPositionX()-1); Ships[i].TotalMoves++; } // UP (W)
	}

	// There is a 2nd Player
	//i = user2Ship;

	//if (user2Ship >= 0) { 
	//	if (user2Input == 65 || user2Input == 97) { Ships[i].setPositionY(Ships[i].getPositionY()-1); Ships[i].TotalMoves++; }       // LEFT (A)
	//	else if (user2Input == 83 || user2Input == 115) { Ships[i].setPositionX(Ships[i].getPositionX()+1); Ships[i].TotalMoves++; } // DOWN (S)
	//	else if (user2Input == 68 || user2Input == 100) { Ships[i].setPositionY(Ships[i].getPositionY()+1); Ships[i].TotalMoves++; } // RIGHT (D)
	//	else if (user2Input == 87 || user2Input == 119) { Ships[i].setPositionX(Ships[i].getPositionX()-1); Ships[i].TotalMoves++; } // UP (W)
	//}
}

int World::CheckAroundRoll(Ship x, int Mode, TRandomMersenne * mersenneRand)
{   // Mode 1 = Check for nearby Docks , Mode 2 = Aditionally check for other Nearby Things , Mode 3 = User is Playing no need for Random
	// Output a random unblocked Direction

    bool Blocked[4] = {false, false, false, false};
    vector<int> UnBlockedRoll;    // Remaining False/UnBlocked gonna be used in this vector
	int toreturn;
    int xx = (x.getPositionX()) - 1;
    int yy = (x.getPositionY()) - 1;
    if ((yy-1)<0) {
        if (Mode==2 || Mode==3) Blocked[0]=true;  // Out of Bounds
        if ((xx+1)<=Height-1) {if (Ground[xx+1][yy].getDock() == true) Blocked[1]=true;}
		if ((xx-1)>=0) {if (Ground[xx-1][yy].getDock() == true) Blocked[3]=true;}
        if (Ground[xx][yy+1].getDock() == true) Blocked[2]=true;  // Blocked by Dock
        }
    else if ((yy+1)>Width-1) {
        if (Mode==2 || Mode == 3) Blocked[2]=true;  // Out of Bounds
		if ((xx+1)<=Height-1) {if (Ground[xx+1][yy].getDock() == true) Blocked[1]=true;}
		if ((xx-1)>=0){if (Ground[xx-1][yy].getDock() == true) Blocked[3]=true;}
        if (Ground[xx][yy-1].getDock() == true) Blocked[0]=true;  // Blocked by Dock
        }

    if ((xx-1)<0){
        if (Mode==2 || Mode == 3) Blocked[3]=true;  // Out of Bounds
        if ((yy-1)>=0) {if (Ground[xx][yy-1].getDock() == true) Blocked[0]=true;}
		if ((yy+1)<=Width-1) {if (Ground[xx][yy+1].getDock() == true) Blocked[2]=true;}
        if (Ground[xx+1][yy].getDock() == true) Blocked[1]=true;  // Blocked by Dock
        }
    else if ((xx+1)>Height-1){
        if (Mode==2 || Mode == 3) Blocked[1]=true;  // Out of Bounds
        if ((yy-1)>=0) {if (Ground[xx][yy-1].getDock() == true) Blocked[0]=true;}
		if ((yy+1)<=Width-1) {if (Ground[xx][yy+1].getDock() == true) Blocked[2]=true;}
		if (Ground[xx-1][yy].getDock() == true) Blocked[3]=true;  // Blocked by Dock
        }

    if ((yy-1)>=0 && (xx+1)<=Height-1 && (yy+1)<=Width-1 && (xx-1)>=0){
        if (Ground[xx][yy-1].getDock() == true) Blocked[0]=true;  // Dock Left
        if (Ground[xx+1][yy].getDock() == true) Blocked[1]=true;  // Dock Down
        if (Ground[xx][yy+1].getDock() == true) Blocked[2]=true;  // Dock Right
        if (Ground[xx-1][yy].getDock() == true) Blocked[3]=true;} // Dock Up
	// Mode 1
    if (Mode == 1) {
        for (int hh=0; hh<=3; hh++) {
            if (Blocked[hh]==true) return 1;}  // Port Nearby
		return 98;
    }
	// Mode 2
    if (Mode == 2) {
		for (unsigned int i=0; i<Ships.size(); i++){
			if (Ships[i].getPositionX() == x.getPositionX()) {
				if (Ships[i].getPositionY() == x.getPositionY()-1)
					Blocked[0]=true;
				if (Ships[i].getPositionY() == x.getPositionY()+1)
					Blocked[2]=true;}
			if (Ships[i].getPositionY()==x.getPositionY()){
				if (Ships[i].getPositionX() == x.getPositionX()+1)
					Blocked[1]=true;
				if (Ships[i].getPositionX() == x.getPositionX()-1)
					Blocked[3]=true;}
		}

		// Check for unblocked Direction and return a random Direction
		for (int i=0; i<4; i++) {
			if (Blocked[i] == false) UnBlockedRoll.push_back(i+1);}
		
		if (UnBlockedRoll.size() == 0) return 0;
		else {
			int finalDirection = mersenneRand->IRandom(0, UnBlockedRoll.size()-1);
			return UnBlockedRoll[finalDirection];}}
	// Mode 3
	if (Mode == 3) {
		for (unsigned int i=0; i<Ships.size(); i++){
			if (Ships[i].getPositionX() == x.getPositionX()) {
				if (Ships[i].getPositionY() == x.getPositionY()-1)
					Blocked[0]=true;
				if (Ships[i].getPositionY() == x.getPositionY()+1)
					Blocked[2]=true;}
			if (Ships[i].getPositionY()==x.getPositionY()){
				if (Ships[i].getPositionX() == x.getPositionX()+1)
					Blocked[1]=true;
				if (Ships[i].getPositionX() == x.getPositionX()-1)
					Blocked[3]=true;}
		}
		// "Encrypt" 4 integers into 1
		if (Blocked[0] == false) toreturn = 1000 * 4;
		else toreturn = 1000 * 2;
		if (Blocked[1] == false)	toreturn += 100 * 4;
		else toreturn += 100 * 2;
		if (Blocked[2] == false)	toreturn += 10 * 4;
		else toreturn += 10 * 2;
		if (Blocked[3] == false)	toreturn += 1 * 4;
		else toreturn += 1 * 2;

		return toreturn;
	}

	return 99;
}

void World::CheckAroundShips()
{   // Uses Abilities by checking for Nearby Ships

	int32 seed = (int32)time(0);
	TRandomMersenne rg(seed);

    for (unsigned int y=0; y<Ships.size(); y++) {
        for (unsigned int i=0; i<Ships.size(); i++) {
            if (Ships[i].getPositionX()==Ships[y].getPositionX()) {
                if (Ships[i].getPositionY()==Ships[y].getPositionY()-1) {
                    Ships[y].UseAbility(&Ships[i]);   // By Pointer
                    break;}
                if (Ships[i].getPositionY()==Ships[y].getPositionY()+1){
                    Ships[y].UseAbility(&Ships[i]);
                    break;}}
            if (Ships[i].getPositionY()==Ships[y].getPositionY()){
                if (Ships[i].getPositionX()==Ships[y].getPositionX()+1){
                    Ships[y].UseAbility(&Ships[i]);
                    break;}
                if (Ships[i].getPositionX()==Ships[y].getPositionX()-1){
                    Ships[y].UseAbility(&Ships[i]);
                    break;}}}
		// Cargo Ability
        if (CheckAroundRoll(Ships[y], 1, &rg) == 1){   // Input with Mode 1, Output = 1 if Port Nearby
			if (Ships[y].getType() == 3 || Ships[y].getType() == 4) {
                cout << "| Cargo Found Dock ";
                Ships[y].setSupply(Ships[y].getSupply() + DOCKS);}}
		// Explorer Ability
		if (Ships[y].getType() == 6) {
			if (Ground[Ships[y].getPositionX()-1][Ships[y].getPositionY()-1].getWeather() >= WEATHERDAMAGE) 
				if (rg.IRandom(1, 2) == 1) {
					Ships[y].setPower(Ships[y].getPower() + 1);
					Ships[y].setPower(Ships[y].getMaxPower() + 1);
					printf("| Explorer withstood Weather Damage ");}}
	}
}

void World::DrawEnvironmentMap()
{	// Print map of Treasures and Docks to the User

	unsigned char SpecialChar1 = 176; // Symbol for Dock
	unsigned char SpecialChar2 = 166; // Symbol for Treasure
	int c1 = 0;  // X
	int c2 = 0;  // Y
	
	// y +- 2 changes X by 1
	// x +- 3 changes Y by 1
	for(int y=0; y<(Width*2)+2; y++)
	{
		for (int x=0; x<(Height*3)+2; x++)
		{
			if (y == 0) {
				if (x == (Height*3) + 1) printf(" \n");
				//else if (x%3 == 2) putchar(49 + (x/3));            // Print Horizontal Axis Values
				else  putchar(' ');}
			else if (y == 1) {
				if (x == 0) putchar(9);
				else if (x == 1) putchar(218);
				else if (x == (Height*3) + 1) printf("%c\n", 191);
				else if (x%3 == 1) putchar(194);
				else putchar(196);}
			else if (y == (Width*2) + 1) {
				if (x == 0) putchar(9);
				else if (x == 1) putchar(192);
				else if (x == (Height*3) + 1) printf("%c\n", 217);
				else if (x%3 == 1) putchar(193);
				else putchar(196);}
			else if (y%2 == 1) {
				if (x == 0) putchar(9);
				else if (x == 1) putchar(195);
				else if (x == (Height*3) + 1) 
					{putchar(180); if (y == Width+1) printf("\tPress H to Hide/Show\n"); else printf("\n");}
				else if (x%3 == 1) putchar(197);
				else putchar(196);}
			else {
				if (x == 0) {printf("\t\b"); putchar(48 + (y/2));}  // Print Vertical Axis Values
				else if (x == (Height*3) + 1) printf("%c\n", 179);
				else if (x%3 == 1) putchar(179);
				else if (x%3 == 2) {
					// ### INSIDE A GRID CELL ###
					if (Ground[c1][c2].getDock() == true) 
					{
						printf("%c", SpecialChar1);
						printf(" ");
					}
					else 
					{
						printf(" ");
						if (Ground[c1][c2].getTreas() == true) printf("%c", SpecialChar2);
						else printf(" ");
					}
					c2++; // Y
					if (c2 == Width) {
						c2 = 0;
						c1++; // X
					}
				}	// ##########################
			}
		}
	}
}

void World::DrawShipsMap(int userShip)
{	// Print map of Ships and Docks to the User

	WORD Attributes = 0;
	unsigned char SpecialChar1=219;
	unsigned char SpecialChar2=176;
	bool flagGap = false;
	int c1 = 0;  // X
	int c2 = 0;  // Y

	// y +- 2 changes X by 1
	// x +- 3 changes Y by 1
	for(int y=0; y<(Width*2)+2; y++)
	{
		for (int x=0; x<(Height*3)+2; x++)
		{
			if (y == 0) {
				if (x == (Height*3) + 1) printf(" \n");
				//else if (x%3 == 2) putchar(49 + (x/3));            // Print Horizontal Axis Values
				else  putchar(' ');}
			else if (y == 1) {
				if (x == 0) putchar(9);
				else if (x == 1) putchar(218);
				else if (x == (Height*3) + 1) printf("%c\n", 191);
				else if (x%3 == 1) putchar(194);
				else putchar(196);}
			else if (y == (Width*2) + 1) {
				if (x == 0) putchar(9);
				else if (x == 1) putchar(192);
				else if (x == (Height*3) + 1) printf("%c\n", 217);
				else if (x%3 == 1) putchar(193);
				else putchar(196);}
			else if (y%2 == 1) {
				if (x == 0) putchar(9);
				else if (x == 1) putchar(195);
				else if (x == (Height*3) + 1) 
					{putchar(180);
					if (y == Width+1 && userShip < 0) printf("\tPress Enter to Join\n"); else printf("\n");}
				else if (x%3 == 1) putchar(197);
				else putchar(196);}
			else {
				if (x == 0) {printf("\t\b"); putchar(48 + (y/2));}  // Print Vertical Axis Values
				else if (x == (Height*3) + 1) printf("%c\n", 179);
				else if (x%3 == 1) putchar(179);
				else if (x%3 == 2) {
					// ### INSIDE A GRID CELL ###
					for (unsigned int i=0; i<Ships.size(); i++)
					{
						if (i == userShip)  // Highlight User's Ship
							ColourSet(&Attributes, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						if (Ships[i].getPositionX()==c1+1 && Ships[i].getPositionY()==c2+1)
						{	// Print Ship Character
							printf("%c", SpecialChar1);                  
							if (Ships[i].getType()==1) printf("P");      
							else if (Ships[i].getType()==2) printf("B");
							else if (Ships[i].getType()==3) printf("C");
							else if (Ships[i].getType()==4) printf("X");
							else if (Ships[i].getType()==5) printf("R");
							else if (Ships[i].getType()==6) printf("E");
							flagGap = true;	 // No need for Gap Character
						}
						if (i == userShip)
							ColourReset(Attributes);
					}

					if (Ground[c1][c2].getDock() == true)
					{	// Print Dock Character
						printf("%c ", SpecialChar2);
						flagGap = true;
					}
					if (flagGap == false) printf("  ");
					
					flagGap =false;
					c2++; // Y
					if (c2 == Width) {
						c2 = 0;
						c1++; // X
					}
				}	// ##########################
			}
		}
	}
}

void World::PrintWeather()
{	// Print Weather Map

    unsigned char bound = 219;
    for (int i=0; i<Width+2; i++) // Up
        cout << bound;
    for(int i=0; i<Height; i++)
	{
        cout << endl;
        cout << bound;             // Left
        for(int j=0; j<Width; j++)
            cout << Ground[i][j].getWeather();

        cout << bound;             // Right
    }
    cout << endl;
    for (int i=0;i<Width+2; i++)  // Down
         cout << bound;
    cout << endl;
}

void World::PrintShipStats()
{	// Print detailed Ship Information

	cout << setw(12) << "<TYPE>" << setw(14) << "<X> |" << setw(4) << "<Y>" << setw(12)
	<< "<SPEED " << " TREASURE " << " HEALTH>" << endl;
	for (unsigned int i=0; i<Ships.size(); i++)
	{
		if(Ships[i].getType()==1) cout << "P| " << setw(12) << "Pirate  :" << setw(8) << Ships[i].getPositionX()
			<< setw(6) << Ships[i].getPositionY() << setw(10) << Ships[i].getSpeed() << setw(9) << Ships[i].getSupply();
		else if(Ships[i].getType()==2) cout << "B| " << setw(12) << "Pirate-Big :" << setw(8) << Ships[i].getPositionX()
			<< setw(6) << Ships[i].getPositionY() << setw(10) << Ships[i].getSpeed() << setw(9) << Ships[i].getSupply();
		else if(Ships[i].getType()==3) cout << "C| " << setw(12) << "Cargo   :" << setw(8) << Ships[i].getPositionX()
			<< setw(6) << Ships[i].getPositionY() << setw(10) << Ships[i].getSpeed() << setw(9) << Ships[i].getSupply();
		else if(Ships[i].getType()==4) cout << "X| " << setw(12) << "Cargo-Big :" << setw(8) << Ships[i].getPositionX()
			<< setw(6) << Ships[i].getPositionY() << setw(10) << Ships[i].getSpeed() << setw(9) << Ships[i].getSupply();
		else if(Ships[i].getType()==5) cout << "R| " << setw(12) << "Repairer :" << setw(8) << Ships[i].getPositionX()
			<< setw(6) << Ships[i].getPositionY() << setw(10) << Ships[i].getSpeed() << setw(9) << Ships[i].getSupply();
		else if(Ships[i].getType()==6) cout << "E| " << setw(12) << "Explorer :" << setw(8) << Ships[i].getPositionX()
			<< setw(6) << Ships[i].getPositionY() << setw(10) << Ships[i].getSpeed() << setw(9) << Ships[i].getSupply();
		PrintHealthBar(&Ships[i]);
	}
}

void World::PrintHealthBar(Ship * x)
{	// Prints a Ship's Health Bar that looks like {|||||||   }
	
	unsigned char SpecialCharL = 123; // Left
	unsigned char SpecialCharR = 125; // Right
	unsigned char SpecialCharH = 124; // Health

	cout << setw(6) << SpecialCharL;
	// Print CURRENT Health
	for (int i=0; i<(x->getPower()); i++)
		cout << SpecialCharH;

	// Print MISSING Health
	int missing;

	if (x->getPower() >= 0)
		missing = x->getMaxPower() - x->getPower();
	else
		missing = x->getMaxPower();
	for (int y=0; y<missing; y++)
		cout << " ";

	cout << SpecialCharR;
	cout << endl;
}

void World::ApplyWeather()
{	// Use Environemnt's Ability that causes damage depending on Weather

	int32 seed = (int32)time(0);
	TRandomMersenne rg(seed);

    int countT=0;
    for (unsigned int i=0; i<Ships.size(); i++) 
	{
        if (Ground[Ships[i].getPositionX()-1][Ships[i].getPositionY()-1].getWeather() >= WEATHERDAMAGE) 
		{	// Apply Damage
            Ships[i].setPower(Ships[i].getPower() - 1);
            countT++;
		}
	}
	// REFRESH WEATHER
    for (int y=0; y<Height; y++)
        for (int j=0; j<Width; j++)
            Ground[y][j].setWeather(rg.IRandom(0, 9));
    printf("| Ships damaged by Weather : %d\n", countT);
}

void World::ApplyTreasure()
{	// Make Ships pick up Treasure Chests

    int countT=0;
    for (unsigned int i=0; i<Ships.size(); i++)
	{
		if (Ground[Ships[i].getPositionX()-1][Ships[i].getPositionY()-1].getTreas() == true)
		{
			countT++;
			Ships[i].setSupply(Ships[i].getSupply() + TREASURECHEST);
			Ground[Ships[i].getPositionX()-1][Ships[i].getPositionY()-1].setTreas(10);  // Destroy Treasure
		} 
	}
    printf("| Ships that discovered Treasure Chests : %d\n", countT);
}

void World::ApplyDocks()
{	// Apply Dock Abilities

	int32 seed = (int32)time(0);
	TRandomMersenne rg(seed);

    int countT=0;
    for (unsigned int i=0; i<Ships.size(); i++)
	{
		if (CheckAroundRoll(Ships[i], 1, &rg) == 1)
		{
			countT++;
			if (Ships[i].getType()==1 || Ships[i].getType() == 2)
				Ships[i].setPower(Ships[i].getPower() - 1);
			else {
				Ships[i].setPower(Ships[i].getPower() + 1);
				Ships[i].setPower(Ships[i].getMaxPower() + 1);}
		}
	}
    printf("| Ships that found Dock : %d\n\n", countT);
}

int World::ShipDestroyer(int userShip)
{	// Checks all Ships in order to Destroy ones with no Health
	// OUTPUT : -2 if there is no Player, -1 if Player's Ship was destroyed, else returns Updated userShip ID

	// The ID of the Player's Ship must be Refreshed each round to avoid certain Situations

	int toreturn = -1;
	bool flagDied = false;
	int xPrevious;
	int yPrevious;

	if (userShip == -1) {  // There is no Player
	}
	else {
		// X and Y before Refresh
		xPrevious = Ships[userShip].getPositionX()-1;
		yPrevious = Ships[userShip].getPositionY()-1;
	}

    for (unsigned int i=0; i<Ships.size(); i++)
	{
		if (Ships[i].getPower() < 0)
		{
			if (i == userShip) { flagDied = true; }
            printf("\t| Ship destroyed ");
			// Destroy
			Ground[Ships[i].getPositionX()-1][Ships[i].getPositionY()-1].setTreas(1);  // Convert dead Ship to Treasure
            Ships.erase(Ships.begin() + i);
            ShipCreation.erase(ShipCreation.begin() + i);
		}
	}

	if (userShip == -1) return -2;
	if (flagDied == true) return -1;
	else {
		// Refresh
		for (unsigned int j=0; j<Ships.size(); j++) {
			if (Ships[j].getPositionX()-1 == xPrevious && Ships[j].getPositionY()-1 == yPrevious)
				toreturn = j;
		}

	}
	return toreturn;
}

int World::AssignPlayerShip(int existingPlayer)
{	// Assign Player 1 or 2 a Random Ship with Speed 1

	int32 seed = (int32)time(0);
	TRandomMersenne rg(seed);
	vector<int> Candidates;

	if (existingPlayer == -1)
		for (unsigned int i=0; i<Ships.size(); i++) 
			if (Ships[i].getSpeed() == 1)
				Candidates.push_back(i);

	int temp = rg.IRandom(0, Candidates.size()-1);
	return Candidates[temp];
}

void ColourSet(WORD * Attributes, WORD Colour)
{
	CONSOLE_SCREEN_BUFFER_INFO Info;
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hstdout, &Info);
	*Attributes = Info.wAttributes;
	SetConsoleTextAttribute(hstdout, Colour);
}

void ColourReset(WORD Attributes)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}