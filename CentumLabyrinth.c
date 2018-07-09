#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<math.h>	
//Map dimensions
#define MAP_HEIGHT 15
#define MAP_WIDTH 15
//Map Values
#define MAP_FLOOR 0 
#define MAP_WALL 1
#define PLAYER 9
#define MONSTER 2
#define TREASURE 51
#define ARMOR 101
#define WEAPON 151

struct weaponTag{
	char name[32];
	int damage;
	int value;
	int hp;
};
typedef struct weaponTag Weapon;

struct armorTag{
	char name[32];
	int defense;
	int value;
	int hp;
};
typedef struct armorTag Armor;

struct treasureTag{
	char name[32];
	int value;
};
typedef struct treasureTag Treasure;

struct statsTag{ //Structure for player stats
	char name[31];
	int level;
	int damage;
	int defense;
	int hitpoints;
	int constitution;
	int credits;
	int points;
	Weapon playerWeapon;
	Armor playerArmor;
	Treasure playerTreasure;
	int counter;
	
}player;
typedef struct statsTag Stats;

struct monsterTag{ //Monster stats
	char name[30];
	int hp;
	int dmg;
	int def;
};
typedef struct monsterTag Enemy;

struct room_Tag{
	int roomCount, roomType;
	int gotoNorth, gotoSouth, gotoWest, gotoEast;
}; typedef struct room_Tag Room;

struct roomnodeTag{
	Room data;
	struct roomnodeTag *room_North;//Adjacent rooms
	struct roomnodeTag *room_East;
	struct roomnodeTag *room_South;
	struct roomnodeTag *room_West;
}; typedef struct roomnodeTag roomNode;
	roomNode *nextRoom; //Additional global pointers for rooms
	roomNode *prevRoom;
	roomNode *currRoom;
	roomNode *baseRoom;

struct weaponnodeTag{
	struct weaponTag data;
	struct weaponnodeTag *next;
	struct weaponnodeTag *prev;
};
typedef struct weaponnodeTag weaponNode;

struct armornodeTag{
	struct armorTag data;
	struct armornodeTag *next; 
	struct armornodeTag *prev;
};
typedef struct armornodeTag armorNode;

struct treasurenodeTag{
	struct treasureTag data;
	struct treasurenodeTag *next;
	struct treasurenodeTag *prev;
};
typedef struct treasurenodeTag treasureNode;
/*
void addInventory(weaponNode *ptr, struct weaponTag data)//Allocate memory if player finds a weapon
{
	while(ptr->next!=NULL){
		ptr = ptr->next;
	}
	ptr->next = malloc(sizeof(weaponNode));
	(ptr->next)->prev = ptr;
	ptr =  ptr->next;
	ptr->data = data;
	ptr->next = NULL;
}	

void displayInventory(weaponNode *ptr)//To display inventory
{
	while(ptr->next != NULL)
	{
		printf("%s", ptr->data.name);
		ptr = ptr->next;
	}
}*/

int randomInt(int num)//Function for random number generation
{
	srand(time(NULL));
	int n = rand() % num;
	return n;
}

int randomInt2(int lowerBound, int upperBound)//Function which accepts two parameters for RNG
{
	srand(time(NULL));
	return(lowerBound + rand() % (upperBound + 1 + lowerBound));
}

int roundUp(float num)//Function for rounding up values
{
	int n = ceil(num);
	return n;
}

typedef char String31[32];
//Preset monsters
String31 monsterList[30] = {"Proto-behemoth", "Alchemic Ooze", "Borgbear", "Circuitron", "Paradox", "Warmech", "Protoflorian", "Skeletron", "Waspinator", "Gorgon", "Corrupted Spirit",
							"Spiderbot","Gargoyle","Stone Golem", "Dark Mage", "Death Worm", "Zombie", "Bat"}; 
//Preset treasures						
String31 treasureNames[30] = {"Embroidered Flag", "Furiamond", "Centum Quartz", "Dynasty Vase", "Golden Cocoon", "Schradite", "Peach Clover","Lovely Locust", "Moonstone", 
							 "Silver Pearl", "Emerald", "Black Star Sapphire", "Golden Yellow Topaz", "Jade Pendant", "Gold Ring", "Banded Agate", "Amethyst", "Amber", "Deep Blue Spinel",
							 "Lapis Lazuli", "Obsidian", "Chrysoberyl", "Blood Stone", "Azurite", "Peridot", "Malachite", "Tourmaline", "Ivory Crown", "Alexandrite", "Jasper"};
//Preset armors
String31 armorNames[30] = {"Standard Issue Salvager Uniform", "Void Defender Suit", "Golden Centurion Armor", "Crimson Paladin Armor", "Evolved Solarflux", "Hunter Uniform", 
							"Hollow Soldier Armor", "Crusader Armor", "Cold Bones Armor", "Corvus Corax", "Blackhorn Runes Armor", "Apocalyptic Warlord Armor", "Youkono" 
							"Dragon Acolyte Uniform", "Royal Arcana Robes", "Shadow Shinobi Morph", "Phoenix Plate", "Plate of the Fallen", "Protosartorium", "Robes of Rayst", 
							"Adventurer Uniform", "Lightwielder's Plate", "Nightmare Plate"};
//Preset weapons							
String31 weaponNames[30] = {"Miraculous Serpent Blade", "Black Faerie Fan", "Radiant Flash Spear", "Purgatory Flailblade", "Brilliant Flower Chakram", "Scorching Chain",
							"Grand Crossbow", "Thunderbolt Pike", "Sharp Destruction Hammer", "Strike Sealing Sword", "Spiral Thunder Lance", "Spark Cannon", "Death Shroud Claws",
							"Divine Oracle Scroll", "Manic Chieftain Gauntlets", "Soaring Blaze Bow", "Rupture Trident", "Soaring Pike", "Verdant Axe", "Shard Boomerang", 
							"Conjurer's Whip", "Mystic Staff", "Imperial Mace", "Demon Halberd", "Eclipse Javelin", "Crystal Scales Spikeshield", "Faerie Moon Orb and Scepter", 
							"Scarlet Rapier", "Crimson Nunchaku", "Infernal Tonfa"};
							
void SaveState(Stats player)//Function for saving the game
{
	FILE *SaveData;
	SaveData = fopen("horologium.txt", "w");
	
	fprintf(SaveData, "%s\n", player.name);
	fprintf(SaveData, "%d\n", player.level);
	fprintf(SaveData, "%d\n", player.hitpoints);
	fprintf(SaveData, "%d\n", player.damage);
	fprintf(SaveData, "%d\n", player.defense);
	fprintf(SaveData, "%d\n", player.constitution);
	fprintf(SaveData, "%d\n", player.points);
	printf("Saved successfully!\n");
	fclose(SaveData);
}

void LoadState(Stats player)//Function for loading the game
{
	FILE *SaveData;
	SaveData = fopen("horologium.txt", "r");
	
	if(SaveData == NULL){
		system("cls");
		printf("ERROR: File does not exist.\n");
		system("pause");
	}
	else{
		fscanf(SaveData, "%s", player.name);
		fscanf(SaveData, "%d", &player.level);
		fscanf(SaveData, "%d", &player.hitpoints);
		fscanf(SaveData, "%d", &player.damage);
		fscanf(SaveData, "%d", &player.defense);
		fscanf(SaveData, "%d", &player.constitution);
		fscanf(SaveData, "%d", &player.points);
		printf("File opened successfully.\n");
		fclose(SaveData);
	}
}

void damageCalc(Stats *character) //Computes for the overall damage stat
{
	character->damage = roundUp(character->level*0.8) + character->playerWeapon.damage;	
}

void defenseCalc(Stats *character) //Computes for the overall defense stat
{
	character->defense = roundUp(character->level*0.2) +character->playerArmor.defense;
}

void HPCalc(Stats *character) //Computes for the overall HP stat
{
	character->hitpoints = character->level + character->playerArmor.hp + character->playerWeapon.hp;
}

void constitutionCalc(Stats *character) //Computes for the overall constitution stat
{
	character->constitution = roundUp(character->hitpoints*0.2);
}

void expCounter(Stats *character) //Counter to know when to level up
{
	int count = roundUp((character->level)*0.3);
	if(character->counter == count ){
		character->level += 1;
		character->counter = 0;
		count = 0;}
	else	
		character->level +=0;
}

void main_menu(Stats *character)//Initializes the main menu window
{
	int menuOptions;
	printf("èúíúíúíúíúíúíúíúíúíúíúíúíúíúíúíúè");
	printf("\n±                               ±");
	printf("\n±     ÅÅOROLOGIUM LABYRINTÅÅ    ±\n");
	printf("±                               ±");
	printf("\nèúíúíúíúíúíúíúíúíúíúíúíúíúíúíúíúè\n\n");
	printf("================================\n");
	printf("º    1: Start a new game       º\n");
	printf("º    2: Load an existing game  º\n");
	printf("º    0: Exit game              º\nº");
	printf("                              º");
	printf("\n================================");
	printf("\n    Enter your choice: ");
	scanf("%d", &menuOptions);
	
	switch(menuOptions)
	{
		case 1:
		{
			printf("    Enter your player name: ");
			scanf("%s", character->name);	
			
			printf("\nHello %s! Welcome to the world of Horologium Aeternalis!\nYou are chosen as a salvager to look for the lost technology\n in the web-like underground labyrinth.\n", character->name);}
			system("pause"); 
			printf("\nThis lost technology contains the collective memories of all civilizations from the past, which will serve as warning for future civilizations.\n"); system("pause");
			printf("\nIf found, this item could stop the impending Great War, \nand usher in an infinite era of peace for the whole world.\n"); system("pause");
			printf("\nGood luck on your journey %s! \n", character->name); system("pause");
			system("cls");break;
			
		case 2: LoadState(player);
		
		case 0: exit(1);
		default: printf("Input invalid. Please try again\n"); system("cls");main_menu(character);
	}
}

void displayStats(Stats *character)//To display necessary stats above while playing
{
	printf("\n=====================================================\n");
	printf("NAME: %s\tHP: %d\tCredits: %d", character->name, character->hitpoints,character->credits);
	printf("\n=====================================================\n");
}

/*NOT WORKING FUNCTIONS I DID MY BEST :(
void dungeonWalls(int dungeon[][MAP_WIDTH])//To create dungeon walls for generated dungeon room
{
	int i, j;
	
	for(i = 0; i < MAP_HEIGHT; i++)
	{
		for(j = 0; j < MAP_WIDTH; j++)
		{
			if(i == 0 || j ==0 || i == MAP_HEIGHT - 1 || j == MAP_WIDTH - 1)
				dungeon[i][j] = MAP_WALL;
			else
				dungeon[i][j] = MAP_FLOOR;
		}
	}
}

void dungeonDoors(int dungeon[][MAP_WIDTH], roomNode **Dungeon) //To create doors for the generated dungeon room
{
	if((*Dungeon)->data.gotoNorth == MAP_FLOOR)
		dungeon[0][7] = MAP_FLOOR;
	if((*Dungeon)->data.gotoSouth == MAP_FLOOR)
		dungeon[14][7] = MAP_FLOOR;
	if((*Dungeon)->data.gotoEast == MAP_FLOOR)
		dungeon[7][14] = MAP_FLOOR;
	if((*Dungeon)->data.gotoWest == MAP_FLOOR)
		dungeon[7][0] = MAP_FLOOR;
}

void dungeonType(int dungeon[][MAP_WIDTH], roomNode **Dungeon) //To create room type for the generated dungeon room 
{
	if((*Dungeon)->data.roomType == MONSTER)
		dungeon[(MAP_HEIGHT-1)/2][(MAP_WIDTH-1)/2] = 2;
	if((*Dungeon)->data.roomType == WEAPON)
		dungeon[(MAP_HEIGHT-1)/2][(MAP_WIDTH-1)/2] = 3;
	if((*Dungeon)->data.roomType == ARMOR)
		dungeon[(MAP_HEIGHT-1)/2][(MAP_WIDTH-1)/2] = 4;
	if((*Dungeon)->data.roomType == TREASURE)
		dungeon[(MAP_HEIGHT-1)/2][(MAP_WIDTH-1)/2] = 5;
}

void createRoom(int dungeon[][MAP_WIDTH], roomNode **Dungeon) //To create a room
{
	dungeonWalls(dungeon);
	dungeonDoors(dungeon, Dungeon);
	dungeonType(dungeon, Dungeon);
}
void initializeDungeon(roomNode **Dungeon)// To initialize the game
{
	*Dungeon = malloc(sizeof(roomNode));
	(*Dungeon)->data.gotoNorth = MAP_FLOOR;
	(*Dungeon)->data.gotoEast = MAP_FLOOR;
	(*Dungeon)->data.gotoSouth = MAP_FLOOR;
	(*Dungeon)->data.gotoWest = MAP_FLOOR;
	(*Dungeon)->room_North = NULL;
	(*Dungeon)->room_East = NULL;
	(*Dungeon)->room_South = NULL;
	(*Dungeon)->room_West = NULL;
}

void randomizeRooms(roomNode **Dungeon) //To randomize room type and room doors
{
	*Dungeon = malloc(sizeof(roomNode));
	(*Dungeon)->room_North = NULL;
	(*Dungeon)->room_East = NULL;
	(*Dungeon)->room_South = NULL;
	(*Dungeon)->room_West = NULL;
	(*Dungeon)->data.roomType = randomInt(4);
	(*Dungeon)->data.gotoSouth = randomInt(2);
	(*Dungeon)->data.gotoNorth = randomInt(2);
	(*Dungeon)->data.gotoEast = randomInt(2);
	(*Dungeon)->data.gotoWest = randomInt(2);
}*/

void initializeMap(int dungeon[][MAP_WIDTH], Stats *character)//To draw a room
{
	displayStats(character);
	int x, y;
	
	for(y = 0; y < MAP_HEIGHT; y++)
	{
		printf("\n");
		for(x = 0; x < MAP_WIDTH; x++)
		{
			//Initialize the tiles
			switch(dungeon[y][x])
			{
				case 0: printf(".");break;
				case 1: printf("#");break;
				case 2: printf("M");break;
				case 9: printf("@"); break;
				case 51: printf("T"); break;
				case 101: printf("A"); break;
				case 151:printf("W");break;
			}
		}
	}
	printf("\n\n\nPress 'U' to view player stats.\n");
	printf("Press 'C' to distribute points to player stats.\n");
	printf("Press 'V' to save your progress.");
}

void playerMove(int dungeon[][MAP_WIDTH], Stats *character) //For player movement or / check self or stats
{
	int i, j;
	char input = getch();
	int temp;
	
	for(i = 0; i < MAP_HEIGHT; i++)
	{
		for(j = 0; j < MAP_WIDTH; j++)
		{
			if(dungeon[i][j] == PLAYER)//To look for the player's location 
			switch(input)
			{	
				//Go up
				case 'w': temp = dungeon[i][j]; 
						  dungeon[i][j] = dungeon[i-1][j]; 
						  dungeon[i-1][j] = temp;break;
				case 'W': temp = dungeon[i][j]; 
						  dungeon[i][j] = dungeon[i-1][j]; 
						  dungeon[i-1][j] = temp;break;
						  
				//Go left
				case 'a': temp = dungeon[i][j]; 	
						  dungeon[i][j] = dungeon[i][j-1]; 
						  dungeon[i][j-1] = temp;break;
				case 'A': temp = dungeon[i][j]; 	
						  dungeon[i][j] = dungeon[i][j-1]; 
						  dungeon[i][j-1] = temp;break;
				//CHECK SELF or DISPLAY STATS
				case 'u': expCounter(character);
						  damageCalc(character);
						  defenseCalc(character);
						  HPCalc(character);
						  constitutionCalc(character);
						  system("cls"); 
						  puts("\n*--------------------*\n");
						  puts("|     YOUR STATS     |\n");
						  puts("*--------------------*\n");
						  printf("NAME: %s\n", character->name);
						  printf("\nWeapon: %s\n", character->playerWeapon.name);
						  printf("Armor: %s\n", character->playerArmor.name);
						  printf("\nLevel: %d\n", character->level);
						  printf("Damage: %d\n", character->damage);
				          printf("Defense: %d\n", character->defense);
				          printf("HP: %d\n", character->hitpoints);
				          printf("Constitution: %d\n", character->constitution); 
				          printf("Credits: %d\n", character->credits);
						  printf("Points to distribute: %d\n", character->points);system("pause");break;
				
				case 'c': system("cls");
						  printf("\nPoints to distribute: %d\n\n", character->points); 
						  int x;
						  printf("1: Damage\n2: Defense\n3: HP\n4: Constitution\n5: Level\n");
						  printf("Enter the number corresponding to the stats to distribute points: ");
						  if(character->points <= 0){
							printf("\nYou ran out of points to distribute\n");system("pause");	
						  	break;}
						  scanf("%d", &x);
						  if(x == 1){
						  	character->damage += 1;
						  	character->points -= 1;}
						  if(x == 2){
						  	character->defense += 1;
						  	character->points -= 1;}
						  if(x == 3){
						  	character->hitpoints += 1;
						  	character->points -= 1;}
						  if(x == 4){
						  	character->constitution += 1;
						  	character->points -= 1;}
						  if(x == 5){
						  	character->level+=1;
						  	character->points-=1;
						  }
						  break;
						  	
				case 'v': SaveState(player); system("pause");break;
						  		  
				//Ignore other key inputs
				default: break;
			}
		}	
	}
	for(i = MAP_HEIGHT-1; i >= 0; i--)
	{
		for(j = MAP_WIDTH-1; j >= 0; j--)
		{
			if(dungeon[i][j] == 9)//To look for the player's location
			switch(input)
			{   
			   //Go down
				case 's': temp = dungeon[i][j]; 
						  dungeon[i][j] = dungeon[i+1][j]; 
						  dungeon[i+1][j] = temp;break;
				case 'S': temp = dungeon[i][j]; 
						  dungeon[i][j] = dungeon[i+1][j]; 
						  dungeon[i+1][j] = temp;break;
			
				//Go right
				case 'd': temp = dungeon[i][j]; 
						  dungeon[i][j] = dungeon[i][j+1]; 
						  dungeon[i][j+1] = temp;break;	
				case 'D': temp = dungeon[i][j]; 
						  dungeon[i][j] = dungeon[i][j+1]; 
						  dungeon[i][j+1] = temp;break;
			}
		}
	}
	i = 0; j = 0;
	system("cls");
}

void isPassable(int dungeon[][MAP_WIDTH])//Walls become unpassable
{
	//Upper horizontal walls
	if(dungeon[0][1] == PLAYER)
	{
		dungeon[0][1] = MAP_WALL;
		dungeon[1][1] = PLAYER;
	}
	if(dungeon[0][2] == PLAYER)
	{
		dungeon[0][2] = MAP_WALL;
		dungeon[1][2] = PLAYER;
	}
	if(dungeon[0][3] == PLAYER)
	{ 
		dungeon[0][3] = MAP_WALL;
		dungeon[1][3] = PLAYER;
	}
	if(dungeon[0][4] == PLAYER)
	{
		dungeon[0][4] = MAP_WALL;
		dungeon[1][4] = PLAYER;
	}
	if(dungeon[0][5] == PLAYER)
	{
		dungeon[0][5] = MAP_WALL;
		dungeon[1][5] = PLAYER;
	}
	if(dungeon[0][9] == PLAYER)
	{
		dungeon[0][9] = MAP_WALL;
		dungeon[1][9] = PLAYER;
	}
	if(dungeon[0][10] == PLAYER)
	{
		dungeon[0][10] = MAP_WALL;
		dungeon[1][10] = PLAYER;
	}
	if(dungeon[0][11] == PLAYER)
	{
		dungeon[0][11] = MAP_WALL;
		dungeon[1][11] = PLAYER;
	}
	if(dungeon[0][12] == PLAYER)
	{
		dungeon[0][12] = MAP_WALL;
		dungeon[1][12] = PLAYER;
	}
	if(dungeon[0][13] == PLAYER)
	{
		dungeon[0][13] = MAP_WALL;
		dungeon[1][13] = PLAYER;
	}
	//Left vertical walls
	if(dungeon[1][0] == PLAYER)
	{
		dungeon[1][0] = MAP_WALL;
		dungeon[1][1] = PLAYER;
	}
	if(dungeon[2][0] == PLAYER)
	{
		dungeon[2][0] = MAP_WALL;
		dungeon[2][1] = PLAYER;
	}
	if(dungeon[3][0] == PLAYER)
	{
		dungeon[3][0] = MAP_WALL;
		dungeon[3][1] = PLAYER;
	}
	if(dungeon[4][0] == PLAYER)
	{
		dungeon[4][0] = MAP_WALL;
		dungeon[4][1] = PLAYER;
	}
	if(dungeon[5][0] == PLAYER)
	{
		dungeon[5][0] = MAP_WALL;
		dungeon[5][1] = PLAYER;
	}
	if(dungeon[9][0] == PLAYER)
	{
		dungeon[9][0] = MAP_WALL;
		dungeon[9][1] = PLAYER;
	}
	if(dungeon[10][0] == PLAYER)
	{
		dungeon[10][0] = MAP_WALL;
		dungeon[10][1] = PLAYER;
	}
	if(dungeon[11][0] == PLAYER)
	{
		dungeon[11][0] = MAP_WALL;
		dungeon[11][1] = PLAYER;
	}
	if(dungeon[12][0] == PLAYER)
	{
		dungeon[12][0] = MAP_WALL;
		dungeon[12][1] = PLAYER;
	}
	if(dungeon[13][0] == PLAYER)
	{
		dungeon[13][0] = MAP_WALL;
		dungeon[13][1] = PLAYER;
	}
	//Right vertical walls
	if(dungeon[1][14] == PLAYER)
	{
		dungeon[1][14] = MAP_WALL;
		dungeon[1][13] = PLAYER;
	}
	if(dungeon[2][14] == PLAYER)
	{
		dungeon[2][14] = MAP_WALL;
		dungeon[2][13] = PLAYER;
	}
	if(dungeon[3][14] == PLAYER)
	{
		dungeon[3][14] = MAP_WALL;
		dungeon[3][13] = PLAYER;
	}
	if(dungeon[4][14] == PLAYER)
	{
		dungeon[4][14] = MAP_WALL;
		dungeon[4][13] = PLAYER;
	}
	if(dungeon[5][14] == PLAYER)
	{
		dungeon[5][14] = MAP_WALL;
		dungeon[5][13] = PLAYER;
	}
	if(dungeon[9][14] == PLAYER)
	{
		dungeon[9][14] = MAP_WALL;
		dungeon[9][13] = PLAYER;
	}
	if(dungeon[10][14] == PLAYER)
	{
		dungeon[10][14] = MAP_WALL;
		dungeon[10][13] = PLAYER;
	}
	if(dungeon[11][14] == PLAYER)
	{
		dungeon[11][14] = MAP_WALL;
		dungeon[11][13] = PLAYER;
	}
	if(dungeon[12][14] == PLAYER)
	{
		dungeon[12][14] = MAP_WALL;
		dungeon[12][13] = PLAYER;
	}
	if(dungeon[13][14] == PLAYER)
	{
		dungeon[13][14] = MAP_WALL;
		dungeon[13][13] = PLAYER;
	}
	//Bottom horizontal walls
	if(dungeon[14][1] == PLAYER)
	{
		dungeon[14][1] = MAP_WALL;
		dungeon[13][1] = PLAYER;
	}
	if(dungeon[14][2] == PLAYER)
	{
		dungeon[14][2] = MAP_WALL;
		dungeon[13][2] = PLAYER;
	}
	if(dungeon[14][3] == PLAYER)
	{
		dungeon[14][3] = MAP_WALL;
		dungeon[13][3] = PLAYER;
	}
	if(dungeon[14][4] == PLAYER)
	{
		dungeon[14][4] = MAP_WALL;
		dungeon[13][4] = PLAYER;
	}
	if(dungeon[14][5] == PLAYER)
	{
		dungeon[14][5] = MAP_WALL;
		dungeon[13][5] = PLAYER;
	}
	if(dungeon[14][9] == PLAYER)
	{
		dungeon[14][9] = MAP_WALL;
		dungeon[13][9] = PLAYER;
	}
	if(dungeon[14][10] == PLAYER)
	{
		dungeon[14][10] = MAP_WALL;
		dungeon[13][10] = PLAYER;
	}
	if(dungeon[14][11] == PLAYER)
	{
		dungeon[14][11] = MAP_WALL;
		dungeon[13][11] = PLAYER;
	}
	if(dungeon[14][12] == PLAYER)
	{
		dungeon[14][12] = MAP_WALL;
		dungeon[13][12] = PLAYER;
	}
	if(dungeon[14][13] == PLAYER)
	{
		dungeon[14][13] = MAP_WALL;
		dungeon[13][13] = PLAYER;
	}
}

void noexit_West(int dungeon[][MAP_WIDTH])//Closes any exit in the left side of the room
{
	if(dungeon[6][0] == PLAYER)
	{
		dungeon[6][0] = MAP_WALL;
		dungeon[6][1] = PLAYER;
	}
	if(dungeon[7][0] == PLAYER)
	{
		dungeon[7][0] = MAP_WALL;
		dungeon[7][1] = PLAYER;
	}
	if(dungeon[8][0] == PLAYER)
	{
		dungeon[8][0] = MAP_WALL;
		dungeon[8][1] = PLAYER;
	}
}

void noexit_North(int dungeon[][MAP_WIDTH])//Closes any exit in the upper part of the room
{
	if(dungeon[0][6] == PLAYER)
	{
		dungeon[0][6] = MAP_WALL;
		dungeon[1][6] = PLAYER;
	}
	if(dungeon[0][7] == PLAYER)
	{
		dungeon[0][7] = MAP_WALL;
		dungeon[1][7] = PLAYER;
	}
	if(dungeon[0][8] == PLAYER)
	{
		dungeon[0][8] = MAP_WALL;
		dungeon[1][8] = PLAYER;
	}
}

void noexit_East(int dungeon[][MAP_WIDTH])//Closes any exit in the right side of the room
{
	if(dungeon[6][14] == PLAYER)
	{
		dungeon[6][14] = MAP_WALL;
		dungeon[6][13] = PLAYER;
	}
	if(dungeon[7][14] == PLAYER)
	{
		dungeon[7][14] = MAP_WALL;
		dungeon[7][13] = PLAYER;
	}
	if(dungeon[8][14] == PLAYER)
	{
		dungeon[8][14] = MAP_WALL;
		dungeon[8][13] = PLAYER;
	}
}

void noexit_South(int dungeon[][MAP_WIDTH])
{
	if(dungeon[14][6] == PLAYER)
	{
		dungeon[14][6] = MAP_WALL;
		dungeon[13][6] = PLAYER;
	}
	if(dungeon[14][7] == PLAYER)
	{
		dungeon[14][7] = MAP_WALL;
		dungeon[13][7] = PLAYER;
	}
	if(dungeon[14][8] == PLAYER)
	{
		dungeon[14][8] = MAP_WALL;
		dungeon[13][8] = PLAYER;
	}
}

void generateMonster(Enemy *monster, Stats *character)//To generate random monsters
{
	int lowerBoundDMG, upperBoundDMG, lowerBoundHP, upperBoundHP;
	int x = randomInt(17);
	strcpy(monster->name, monsterList[x]);
	lowerBoundDMG = character->level - 3;
	upperBoundDMG = character->level + 1;
	lowerBoundHP = character->level - 5;
	upperBoundHP = character->level;
	if(lowerBoundDMG <= 0)//Generate monster's DAMAGE
	     monster->dmg = roundUp(randomInt2(1, upperBoundDMG));
	else
		monster->dmg = roundUp(randomInt2(lowerBoundDMG, upperBoundDMG) * 0.3);
	if(lowerBoundHP > 0)//Generate mosnter's HP
		monster->hp = roundUp(randomInt2(lowerBoundHP, upperBoundHP));
	else
		monster->hp = roundUp(randomInt2(1, upperBoundHP));
}

void generateWeapon(Stats *character)//To generate random weapons
{
	int lowerBoundVAL, upperBoundVAL, lowerBoundDMG, upperBoundDMG;
	int x = randomInt(30);
	strcpy(character->playerWeapon.name, weaponNames[x]);
	lowerBoundVAL = character->level - 3;
	upperBoundVAL = character->level;
	lowerBoundDMG = roundUp(character->level*0.1);
	upperBoundDMG = roundUp(character->level*0.5);
	if(lowerBoundVAL > 0)//Generate the WEAPON VALUE
		character->playerWeapon.value = randomInt2(lowerBoundVAL, upperBoundVAL) * 10;
	else
		character->playerWeapon.value = randomInt2(0, upperBoundVAL) * 10;
	//Generate the WEAPON DAMAGE and HP
	character->playerWeapon.damage = randomInt2(lowerBoundDMG, upperBoundDMG);
	character->playerWeapon.hp = randomInt2(1, 5);
}

void generateArmor(Stats *character)//To generate random armors
{
	int lowerBoundVAL, upperBoundVAL, lowerBoundDEF, upperBoundDEF;
	int x = randomInt(22);
	strcpy(character->playerArmor.name, armorNames[x]);
	lowerBoundVAL = character->level - 3;
	upperBoundVAL = character->level + 1;
	lowerBoundDEF = roundUp(character->level*0.1);
	upperBoundDEF = roundUp(character->level*0.7);
	if(lowerBoundVAL > 0) //Generate the ARMOR VALUE
		character->playerArmor.value = randomInt2(lowerBoundVAL, upperBoundVAL) * 10;
	else
		character->playerArmor.value = randomInt2(0, upperBoundVAL) * 10;
	//Generate the ARMOR DEFENSE and HP
	character->playerArmor.defense = randomInt2(lowerBoundDEF, upperBoundDEF);
	character->playerArmor.hp = randomInt2(1, 5);
}

void getWeapon(int room_East[][MAP_WIDTH], Stats *character) //To get weapon and update stats right away
{
		if(room_East[7][7] == PLAYER && room_East[6][7] == WEAPON){
			generateWeapon(character);
			printf("You have obtained %s.", character->playerWeapon.name);
			room_East[6][7] = PLAYER; 
			room_East[7][7] = MAP_FLOOR;}
		if(room_East[7][7] == PLAYER && room_East[8][7] == WEAPON){
			generateWeapon(character);
			printf("You have obtained %s.", character->playerWeapon.name);
			room_East[8][7] = PLAYER; 
			room_East[7][7] = MAP_FLOOR;}
		if(room_East[7][7] == PLAYER && room_East[7][6] == WEAPON){
			generateWeapon(character);
			printf("You have obtained %s.", character->playerWeapon.name);
			room_East[7][6] = PLAYER; 
			room_East[7][7] = MAP_FLOOR;}
		if(room_East[7][7] == PLAYER && room_East[7][8] == WEAPON){
			generateWeapon(character);
			printf("You have obtained %s.", character->playerWeapon.name);
			room_East[7][8] = PLAYER; 
			room_East[7][7] = MAP_FLOOR;}
}

void getArmor(int room_South[][MAP_WIDTH], Stats *character)
{
	if(room_South[7][7] == PLAYER && room_South[6][7] == ARMOR){
			generateArmor(character);
			printf("You have obtained %s.", character->playerArmor.name);
			room_South[6][7] = PLAYER; 
			room_South[7][7] = MAP_FLOOR;}
		if(room_South[7][7] == PLAYER && room_South[8][7] == ARMOR){
			generateArmor(character);
			printf("You have obtained %s.", character->playerArmor.name);
			room_South[8][7] = PLAYER; 
			room_South[7][7] = MAP_FLOOR;}
		if(room_South[7][7] == PLAYER && room_South[7][6] == ARMOR){
			generateArmor(character);
			printf("You have obtained %s.", character->playerArmor.name);
			room_South[7][6] = PLAYER; 
			room_South[7][7] = MAP_FLOOR;}
		if(room_South[7][7] == PLAYER && room_South[7][8] == ARMOR){
			generateArmor(character);
			printf("You have obtained %s.", character->playerArmor.name);
			room_South[7][8] = PLAYER; 
			room_South[7][7] = MAP_FLOOR;}
}

void generateTreasure(Stats *character) //To generate random treasures
{
	int lowerBoundVAL, upperBoundVAL;
	int x = randomInt(29);
	strcpy(character->playerTreasure.name, treasureNames[x]);
	lowerBoundVAL = character->level;
	upperBoundVAL = character->level+5;
	character->credits += randomInt2(lowerBoundVAL, upperBoundVAL) * 10;
}

void getTreasure(int room_West[][MAP_WIDTH], Stats *character)
{
		if(room_West[7][5] == PLAYER && room_West[7][6] == TREASURE){
			generateTreasure(character);
			printf("You have obtained %s.", character->playerTreasure.name);
			character->credits += character->playerTreasure.value; 
			character->counter += 1;
			room_West[7][6] = PLAYER; 
			room_West[7][5]= MAP_FLOOR;}
		if(room_West[7][5] == PLAYER && room_West[7][4] == TREASURE){
			generateTreasure(character);
			printf("You have obtained %s.", character->playerTreasure.name);
			character->credits += character->playerTreasure.value; 
			character->counter += 1;
			room_West[7][4] = PLAYER; 
			room_West[7][5] = MAP_FLOOR;}
		if(room_West[7][5] == PLAYER && room_West[6][5] == TREASURE){
			generateTreasure(character);
			printf("You have obtained %s.", character->playerTreasure.name);
			character->credits += character->playerTreasure.value; 
			character->counter += 1;
			room_West[6][5] = PLAYER; 
			room_West[7][5] = MAP_FLOOR;}
		if(room_West[7][5] == PLAYER && room_West[8][5] == TREASURE){
			generateTreasure(character);
			printf("You have obtained %s.", character->playerTreasure.name);
			character->credits += character->playerTreasure.value; 
			character->counter += 1;
			room_West[8][5] = PLAYER; 
			room_West[7][5] = MAP_FLOOR;}	
}

void initiateBattle(int room_North[][MAP_WIDTH], Stats *character, Enemy *monster)
{
	if(room_North[2][7] == PLAYER && room_North[3][7] == MONSTER){ //For attacking monsters
			character->hitpoints -= monster->dmg;
			monster->hp -= character->damage;
			printf("%s has dealt %d damage to %s\n", monster->name, monster->dmg, character->name);
			if(monster->hp <= 0){
				printf("%s has been defeated.\n", monster->name);
				character->counter+=1;
				room_North[3][7] = PLAYER; 
				room_North[2][7] = MAP_FLOOR;}
	}
	if(room_North[2][7] == PLAYER && room_North[2][8] == MONSTER){
			character->hitpoints -= monster->dmg;
			monster->hp -= character->damage;
			printf("%s has dealt %d damage to %s\n", monster->name, monster->dmg, character->name);
			if(monster->hp <= 0){
				printf("%s has been defeated.\n", monster->name);
				character->counter+=1;
				room_North[2][8] = PLAYER; 
				room_North[2][7] = MAP_FLOOR;}
	}
	if(room_North[2][7] == PLAYER && room_North[2][6] == MONSTER){
			character->hitpoints -= monster->dmg;
			monster->hp -= character->damage;
			printf("%s has dealt %d damage to %s\n", monster->name, monster->dmg, character->name);
			if(monster->hp <= 0){
				printf("%s has been defeated.\n", monster->name);
				character->counter+=1;
				room_North[2][6] = PLAYER; 
				room_North[2][7] = MAP_FLOOR;}
	}
	if(room_North[2][7] == PLAYER && room_North[1][7] == MONSTER){
			character->hitpoints -= monster->dmg;
			monster->hp -= character->damage;
			printf("%s has dealt %d damage to %s\n", monster->name, monster->dmg, character->name);
			if(monster->hp <= 0){
				printf("%s has been defeated.\n", monster->name);
				character->counter+=1;
				room_North[1][7] = PLAYER; 
				room_North[2][7] = MAP_FLOOR;}
	}
}

void map_North(int dungeon[][MAP_WIDTH], int room_North[][MAP_WIDTH], Stats *character, Enemy *monster)//Northern room generation
{	int x, y;
	if(dungeon[0][6] == PLAYER ||dungeon[0][7] == PLAYER || dungeon[0][8] == PLAYER)//Going to the northern room
	{
		dungeon[0][7] = MAP_FLOOR;
		dungeon[0][6] = MAP_FLOOR;
		dungeon[0][8] = MAP_FLOOR;
	 	room_North[13][7] = PLAYER;
	 	while(true)
		{
	 	isPassable(room_North);noexit_West(room_North);noexit_North(room_North);noexit_East(room_North);
		initializeMap(room_North, character);
		playerMove(room_North, character);
	 	system("cls");
		if(room_North[14][6] == PLAYER || room_North[14][7] == PLAYER || room_North[14][8] == PLAYER)//Going back to the 1st room
			{
				room_North[14][6] = MAP_FLOOR; 
				room_North[14][7] = MAP_FLOOR; 
				room_North[14][8] = MAP_FLOOR; 
				dungeon[7][7] = MAP_FLOOR; 
				dungeon[1][7] = PLAYER;
				initializeMap(dungeon, character);
				isPassable(dungeon);
				playerMove(dungeon, character);
				map_North(dungeon, room_North, character, monster); 
	 			system("cls"); break; 
			}
		initiateBattle(room_North, character, monster);
		}
	}
}

void map_West(int dungeon[][MAP_WIDTH], int room_West[][MAP_WIDTH], Stats *character)//Western room generation
{
	if (dungeon[6][0] == PLAYER || dungeon[7][0] == PLAYER || dungeon[8][0] == PLAYER)//Going to the western room
	{
		dungeon[6][0] = MAP_FLOOR;
		dungeon[7][0] = MAP_FLOOR;
		dungeon[8][0] = MAP_FLOOR;
		room_West[7][13] = PLAYER;
		while(true)
		{
			isPassable(room_West);noexit_North(room_West);noexit_South(room_West);
			initializeMap(room_West,character);
			playerMove(room_West, character);
			system("cls");
			if(room_West[6][14] == PLAYER || room_West[7][14] == PLAYER || room_West[8][14] == PLAYER)//Going back to the 1st room
			{
				room_West[6][14] = MAP_FLOOR;
				room_West[7][14] = MAP_FLOOR;
				room_West[8][14] = MAP_FLOOR;
				dungeon[7][7] = MAP_FLOOR;
				dungeon[7][1] = PLAYER;
				initializeMap(dungeon,character);
				isPassable(dungeon);
				playerMove(dungeon, character);
				map_West(dungeon, room_West, character);
				system("cls");break;
			}
		getTreasure(room_West, character);
		}
	}
}

void map_South(int dungeon[][MAP_WIDTH], int room_South[][MAP_WIDTH], Stats *character)//Southern room generation
{
	if(dungeon[14][6] == PLAYER || dungeon[14][7] == PLAYER || dungeon[14][8] == PLAYER)//Going to the southern room
	{
		dungeon[14][6] = MAP_FLOOR;
		dungeon[14][7] = MAP_FLOOR;
		dungeon[14][8] = MAP_FLOOR;
		room_South[1][7] = PLAYER;
		while(true)
		{
			isPassable(room_South); noexit_South(room_South); noexit_West(room_South); noexit_East(room_South);
			initializeMap(room_South,character);
			playerMove(room_South, character);
			system("cls");
			if(room_South[0][6] == PLAYER || room_South[0][7] == PLAYER || room_South[0][8] == PLAYER)//Going back to the 1st room
			{
				room_South[0][6] = MAP_FLOOR;
				room_South[0][7] = MAP_FLOOR;
				room_South[0][8] = MAP_FLOOR;
				dungeon[7][7] = MAP_FLOOR;
				dungeon[13][7] = PLAYER;
				initializeMap(dungeon,character);
				isPassable(dungeon);
				playerMove(dungeon, character);
				map_South(dungeon, room_South, character);
				system("cls");break;
			}
		getArmor(room_South, character);
		}
	}
}

void map_East(int dungeon[][MAP_WIDTH],int room_East[][MAP_WIDTH], Stats *character)//Eastern room generation
{
	if(dungeon[6][14] == PLAYER || dungeon[7][14] == PLAYER || dungeon[8][14] == PLAYER)//Going to the eastern room
	{
		dungeon[6][14] = MAP_FLOOR;
		dungeon[7][14] = MAP_FLOOR;
		dungeon[8][14] = MAP_FLOOR;
		room_East[7][1] = PLAYER;
		while(true)
		{
			isPassable(room_East);noexit_North(room_East);noexit_South(room_East);
			initializeMap(room_East,character);
			playerMove(room_East, character);
			system("cls");
			if(room_East[6][0] == PLAYER||room_East[7][0] == PLAYER||room_East[8][0] == PLAYER)//Going back to the 1st room
			{
				room_East[6][0] = MAP_FLOOR;
				room_East[7][0] = MAP_FLOOR;
				room_East[8][0] = MAP_FLOOR;
				dungeon[7][7] = MAP_FLOOR;
				dungeon[7][13] = PLAYER;
				initializeMap(dungeon,character);
				isPassable(dungeon);
				playerMove(dungeon, character);
				map_East(dungeon, room_East, character);
				system("cls");break;
			}
		getWeapon(room_East, character);
		}
	}
}

void mapRefresh_North(int room_North[][MAP_WIDTH], Stats *character, Enemy *monster)//To generate new monster if there's none
{
	generateMonster(monster, character);
		if(room_North[2][7] != MONSTER){
	 		generateMonster(monster, character);
	 		room_North[2][7] = MONSTER;}
}

void mapRefresh_West(int room_West[][MAP_WIDTH])//To generate new treasure if there's none
{
	if(room_West[7][5] != TREASURE)
		room_West[7][5] = TREASURE;
}

void mapRefresh_South(int room_South[][MAP_WIDTH])//To generate new armor if there's none
{
	if(room_South[7][7] != ARMOR)
		room_South[7][7] = ARMOR;
}

void mapRefresh_East(int room_East[][MAP_WIDTH])//To generate new weapon if there's none
{
	if(room_East[7][7] != WEAPON){
		room_East[7][7] = WEAPON;}
}

void gameover(Stats *character)//If you lost, you will be sent back to the main menu
{
	if(character->hitpoints <= 0){
		printf("GAME OVER\n");
		system("pause");
		system("cls");
		main_menu(character);}
}

int main()
{
	Stats player;
	//Player Stats
	player.level = 1;
	player.damage = 1;
	player.defense = 1;
	player.hitpoints = 1;
	player.constitution = 1;
	player.credits = 10;
	player.points = 7; //Counter for points to distribute to stats
	player.counter = 0; //Counter for the number of treasures obtained and monsters defeated
	strcpy(player.playerArmor.name, "Standard Issue Salvager Uniform");
	player.playerArmor.defense = 1;
	player.playerArmor.hp = 2;
	player.playerArmor.value = 3;
	strcpy(player.playerWeapon.name, "Standard Issue Force Blaster");
	player.playerWeapon.damage = 1;
	player.playerWeapon.hp = 1;
	player.playerWeapon.value = 2;
	strcpy(player.playerTreasure.name, "Sentimental Necklace");
	player.playerTreasure.value = 10;
	
	Enemy monster;
	//Monster stats
	monster.hp = 1;
	monster.dmg = 1;
	monster.def = 1;
	
	//The first room
	int dungeon[MAP_HEIGHT][MAP_WIDTH] = {
	{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
	{0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 
};
	//Northern room
	int room_North[MAP_HEIGHT][MAP_WIDTH] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 
};
	//Western room
	int room_West[MAP_HEIGHT][MAP_WIDTH] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
	{0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 0, 0,0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
};
	//Southern room
	int room_South[MAP_HEIGHT][MAP_WIDTH] = {
	{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0,0,1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
};
	//Eastern room
	int room_East[MAP_HEIGHT][MAP_WIDTH] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
	{0, 0, 0, 0, 0, 0, 0, 151, 0, 0, 0, 0, 0,0,0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
};
		
	main_menu(&player);//Main menu
	
	//Game loop
	while(true)
	{
	expCounter(&player);
	damageCalc(&player);
	defenseCalc(&player);
	HPCalc(&player);
	constitutionCalc(&player);
	initializeMap(dungeon, &player);
	playerMove(dungeon, &player);
	isPassable(dungeon);
	mapRefresh_North(room_North, &player, &monster);
	map_North(dungeon, room_North, &player, &monster);
	map_West(dungeon, room_West, &player);
	mapRefresh_West(room_West);
	map_South(dungeon, room_South, &player);
	mapRefresh_South(room_South);
	mapRefresh_East(room_East);
	map_East(dungeon, room_East, &player);
	gameover(&player);
	}		
	getch();
	return 0;
}
