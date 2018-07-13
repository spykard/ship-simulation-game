#include "stdafx.h"

#include "elements.h"
#include "world.h"

// ### GAME BALANCING ###
#define MAXHEIGHT 2000   // Max Map Height
#define MAXWIDTH 2000	// Max Map Width
#define MAXSHIPS 2000	// Max Ship Count
#define ENDGAME 100     // Treasure Amount for Game Over
// ######################

using namespace std;

World Ko;
int Height;

int GameOver()
{	// Check if All Ships are Dead or a Ship has Reached Max Treasure

	if (Ko.Ships.size() <= 0) return 2;

    for (unsigned int i=0; i<Ko.Ships.size(); i++){
        if(Ko.Ships[i].getSupply() >= ENDGAME) return 1;
	}
    return 0;
}

void PrintGameOver()
{
	WORD Attributes = 0;

	ColourSet(&Attributes, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << R"(
   _____                         ____                 
  / ____|                       / __ \                
 | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ 
 | | |_ |/ _` | '_ ` _ \ / _ \ | |  | \ \ / / _ \ '__|
 | |__| | (_| | | | | | |  __/ | |__| |\ V /  __/ |   
  \_____|\__,_|_| |_| |_|\___|  \____/  \_/ \___|_|   
                                                                                                     
    )" << '\n';
	ColourReset(Attributes);
}

int StartUserInput()
{	// User Enters Map Size and Ship Count

	int shipCount;
	WORD Attributes = 0;

	do {
		system("cls");
		cout << "ENTER MAP " ;
		ColourSet(&Attributes, FOREGROUND_RED | FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE);
		cout << "SIZE";
		ColourReset(Attributes);
		cout << " (1 for Default) : ";

		cin >> Height;
		if (Height == 1) Height = 8;  // Default
	} while(Height <= 3 || Height > MAXHEIGHT);


	do {
		system("cls");
		cout << "ENTER ";
		ColourSet(&Attributes, FOREGROUND_GREEN | FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE);
		cout << "NUMBER";
		ColourReset(Attributes);
		cout << " OF SHIPS (1 for Default): ";

		cin >> shipCount;
		if (shipCount == 1) shipCount = 6;
	} while (shipCount <= 1 || shipCount > (Height*Height*0.68) || shipCount >MAXSHIPS);  // Ships SHOULDN'T MAKE UP MORE THAN 68% of the Map

	return shipCount;
}

void SetConsoleSize(int Height, int Width)
{	// Console Settings

	SetConsoleTitle(_T("Ship Game"));
	COORD coord;
	coord.X = Height;
	coord.Y = Width;

	SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND console = GetConsoleWindow();
	SetConsoleScreenBufferSize(hstdout, coord);
	SetConsoleWindowInfo(hstdout, TRUE, &Rect);
	ShowWindow(console, SW_MAXIMIZE);	//  Full Screen
}

void PrintAll(bool * hide, bool player1, int player1Ship)
{	// All Game Engine Calls

	unsigned char SpecialChar1 = 176;
	unsigned char SpecialChar2 = 166;
	unsigned char SpecialChar3 = 219;
	WORD Attributes = 0;

	// Use Abilities
	Ko.CheckAroundShips();
	printf("\n\n");

	// Treasure Map
	if (*hide == true) {
		ColourSet(&Attributes, FOREGROUND_RED | FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE);
		printf("\tTreasure Map | Docks (%c) and Treasures (%c)\n", SpecialChar1, SpecialChar2);
		ColourReset(Attributes);
		if (Height <= 75)
			Ko.DrawEnvironmentMap();
		else
			printf("\tMAP TOO BIG TO DISPLAY\n\n");
	}

	// Scoreboard
	printf("\n");
	if (*hide == true) {
		Ko.PrintShipStats();
		printf("\n");
	}

	// Ship Map
	ColourSet(&Attributes, FOREGROUND_RED | FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE);
	printf("\tShips (%c) and Docks (%c)\n", SpecialChar3, SpecialChar1);
	ColourReset(Attributes);
	if (Height <= 75)
		if (player1 == false)
			Ko.DrawShipsMap(-1);
		else
			Ko.DrawShipsMap(player1Ship);
	else
		printf("\tMAP TOO BIG TO DISPLAY\n\n");
}

int UserMovementFromInput(int left, int down, int right, int up)
{	// 4 = Available Direction, 2 = Blocked Direction

	WORD Attributes = 0;
	char input;

	if (up == 4) {
		ColourSet(&Attributes, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << endl << "\t\t | W |" << endl;
		ColourReset(Attributes);
	}
	else cout << endl << "\t\t | W |" << endl;
	if (left == 4) {
		ColourSet(&Attributes, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "\t    | A |";
		ColourReset(Attributes);
	}
	else cout << "\t    | A |" ;
	if (down == 4) {
		ColourSet(&Attributes, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "| S |";
		ColourReset(Attributes);
	}
	else cout << "| S |";
	if (right == 4) {
		ColourSet(&Attributes, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "| D |";
		ColourReset(Attributes);
	}
	else cout << "| D |";

	cout << endl << "\t\t   ";
	cin >> input;

	// Input Direction won't be Applied because it's Blocked
	if (input == 65 || input == 97)	if (left == 2) return 0;  // LEFT (A)
	if (input == 83 || input == 115) if (down == 2) return 0; // DOWN (S)
	if (input == 68 || input == 100) if (right == 2) return 0; // RIGHT (D)
	if (input == 87 || input == 119) if (up == 2) return 0; // UP (W)

	return input;
}

int InputNumber(char W)
{
	int x;
	do {
		cout << W << " : ";
		cin >> x;
	} while (x < 1 || x > Height);
	return x;
}

int main(void) 
{
	int Round = 0;
	int shipCount, appendedDirections, inputDirection;
	int normalMode = 2600;   // Msec
	int fastMode = 600;      // Msec
	bool skipDelay = false; //Skip Delay an molis dothike eisodos
	char key = ' ';
	bool hide = true;
	bool player1 = false;
	int player1Ship = -1;
	int destroyandRefresh;
	WORD Attributes = 0;

	int32 seed = (int32)time(0);
	TRandomMersenne rg(seed);

	SetConsoleSize(9000, 9000); // Buffer and Fullscreen
	shipCount = StartUserInput();
	
	Ko = World(Height,Height);

	// Ships divided by 4 in each Category, remaining are Pirates
	if (shipCount<6) Ko.CreateShips((shipCount/4)+(shipCount%4), 0, (shipCount/4), 0, shipCount/4, shipCount/4);
	else Ko.CreateShips((shipCount/4)+(shipCount%4)-1, 1, (shipCount/4)-1, 1, shipCount/4, shipCount/4);

	while (GameOver() == 0) {
		if (player1 == false)
			skipDelay = false;
		++Round;
		SetConsoleTitle(_T("[RUNNING] Ship Game"));

		system("cls");
		// Environment Abilities
		Ko.ApplyWeather();
		Ko.ApplyTreasure();
		Ko.ApplyDocks();
		// Engine Calls
		PrintAll(&hide, player1, player1Ship);
		destroyandRefresh = Ko.ShipDestroyer(player1Ship); // destroyandRefresh has the refreshed ID of the Player's Ship
		
		// Player's Ship was Destroyed
		if (destroyandRefresh == -1) {
			skipDelay = false;
			player1 = false;
			player1Ship = -1;
			PrintGameOver();
		}
		// Player exists and ID was Refreshed
		else if (destroyandRefresh >= 0)
			player1Ship = destroyandRefresh;

		if (player1 == true) {
			appendedDirections = Ko.CheckAroundRoll(Ko.Ships[player1Ship], 3, &rg);
			inputDirection = UserMovementFromInput(appendedDirections / 1000 % 10, appendedDirections / 100 % 10, appendedDirections / 10 % 10, appendedDirections % 10);
			Ko.UserMovement(player1Ship, inputDirection, -1, -1);
			Ko.Movement(player1Ship, -1);
		}
		else {
			cout << endl << "\tPress Space to Pause/Menu" << endl;
			Ko.Movement(-1, -1);
		}

		// Live Input
		while(_kbhit()) 
		{
			key = _getch();
			if (key == 72 || key == 104)
				hide = !hide;
			else if (key == 13)
			{	// PLAYER 1 JOINED
				skipDelay = true;
				player1 = true;
				player1Ship = Ko.AssignPlayerShip(-1);
				printf("\nYou have been Assigned Ship #%d!", player1Ship+1);
				Sleep(1000);
			}
			else if (key == 32 || key == 8)
			{
				SetConsoleTitle(_T("[PAUSED] Ship Game"));
				cout << "\n   [PAUSED - ROUND " << Round << "]\n\tMAIN MENU\n"
				<< "A - Show Ship Statistics\n"
				<< "S - Change Game Speed (Fast/Normal Mode)\n"
				<< "C - Add/Remove Ship\n"
				<< "D - Add/Remove Dock\n"
				<< "E - Add/Remove Treasure Chest\n"
				<< "W - Show Weather Map\n"
				<< "Q - Quit Game\n"
				<< "Other - Unpause\n\n";
				int x, y;
				char menuInput;
				cin >> menuInput;
				if (menuInput == 'A') {
					cout << "A total of " << Ship::count1 << " Pirate Ship has caused " << Ship::FunFact2 << " Damage and Stolen " << Ship::FunFact1 << " Treasure\n";
					cout << "A total of " << Ship::FunFact3 << " Treasure has been earned by Repairer Ships\n";
					Ko.PrintShipStats();
				}
				else if (menuInput =='S') swap(fastMode,normalMode);
				else if (menuInput =='C') {
					cout << "1 to Add Pirate, 2 to Add Cargo, 3 to Add Repairer, 4 to Add Explorer, 5 to Remove last Ship\n";
					cin >> x;
					if (x==1) Ko.CreateShips(1,0,0,0,0,0);
					else if (x==2) Ko.CreateShips(0,0,1,0,0,0);
					else if (x==3) Ko.CreateShips(0,0,0,0,1,0);
					else if (x==4) Ko.CreateShips(0,0,0,0,0,1);
					else if (x==5) Ko.Ships.pop_back();
				}
				else if (menuInput =='D') {
					cout << "Add/Remove Dock on Tile\n";
					x = InputNumber('X');
					y = InputNumber('Y');
					if (Ko.Ground[x-1][y-1].getDock() == true)
						Ko.Ground[x-1][y-1].setDock(10);  // Add
					else Ko.Ground[x-1][y-1].setDock(1);} // Remove
				else if (menuInput =='E') {
					cout << "Add/Remove Treasure Chest on Tile ";
					x = InputNumber('X');
					y = InputNumber('Y');
					if (Ko.Ground[x-1][y-1].getTreas()==true)
						Ko.Ground[x-1][y-1].setTreas(10);  // Add
					else Ko.Ground[x-1][y-1].setTreas(1);} // Remove
				else if (menuInput =='W')
					Ko.PrintWeather();
				else if (menuInput =='Q')
					return 1;
				system("pause");
				skipDelay = true;
			}
		}

		if (skipDelay == false) Sleep(normalMode);}  //Delay

	// Print one Final Time
	system("cls");
	PrintAll(&hide, player1, player1Ship);

	SetConsoleTitle(_T("[GAME OVER] Ship Game"));
	PrintGameOver();
	if (GameOver() == 1) cout << "\nA Ship accumulated Maximum Treasure " << ENDGAME << "\n\n";
	else cout << "\nNo Ships left Alive\n\n";

	system("pause");
	return 0;
}