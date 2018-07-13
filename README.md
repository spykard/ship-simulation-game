# Ship Game

A Ship Simulation Game in C++.

<img src="https://raw.githubusercontent.com/kardakis/ShipSimulationGame/master/Screenshots/World_Resized.png">

Everything in the Simulation uses the Mersenne Twister number generator instead of C++'s built-in generator ( http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html )

<img src="https://raw.githubusercontent.com/kardakis/ShipSimulationGame/master/Screenshots/Stats_Resized.png">

<img src="https://raw.githubusercontent.com/kardakis/ShipSimulationGame/master/Screenshots/Map_Resized.png">

Map Size can be as *huge* as the user wants it to be.

<img src="https://raw.githubusercontent.com/kardakis/ShipSimulationGame/master/Screenshots/BigWorld.png">


## Drawing a scalable Grid in C++:

```
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
```


## Returning which directions are blocked just with 1 Integer    
  
```  
(appendedDirections / 1000 % 10, appendedDirections / 100 % 10, appendedDirections / 10 % 10, appendedDirections % 10)
```


## Abilities of each Ship Type:

* Ship::Pirate & Ship::BigPirate  :  Attacks nearby ships damages them and steals a piece of their treasure.
    Stats: Damage = 1, Speed = 1, Health = 15-20, StartingTreasure = 0 
    
* Ship::Cargo & Ship::BigCargo  :  Increases the amount of treasure its carrying when next to a Dock.
    Stats: Damage = 0, Speed = 1-3, Health = 8, StartingTreasure = 1-40

* Ship::Repairer  :  Repairs a nearby ship by increasing its health, in exchange for treasure.
    Stats: Damage = 0, Speed = 1-3, Health = 9, StartingTreasure = 0, Repair = 3

* Ship::Explorer  :  Has 50% chance of withstanding Environment damage.
    Stats: Damage = 0, Speed = 1, Health = 4, StartingTreasure = 1-35
   
   
## Abilities of Environment:

* Treasure Chests  :  Can be picked up by Ships
     Value: 2
* Docks  :  Damages nearby Ships if they are Pirates, else repairs Ships
     Value: 1
* Weather  :  Damages a nearby Ship. Weather changes each Round
     Value: 1
     
* Dead Ships are converted to Treasure Chests
