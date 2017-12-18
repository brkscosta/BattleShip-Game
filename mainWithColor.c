#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <locale.h>


//declare consts colors
#define CL_RED "\x1b[31;1m"
#define CL_BLUE "\x1b[36;1m"
#define CL_BLACK_WHITE "\x1b[30;47m"
#define CL_RESET "\x1b[0m"

//declare consts
#define GSIZE 10 //height and width of game (can not be changed)
#define PLAYERS 2 //total players (can not be changed)
#define SHIPNUM 5 //total ships (can not be changed)
#define TOTALSHOTS 15 //total shots to hit (can not be changed)
#define NOTHIT 'O' //field that was not hit
#define ONTARGET 'x' //missile hit a ship
#define ONWATER '~' //missile hit water

//Declare ships
typedef struct {
    int sh_size;
    char sh_ori;
    char sh_cords[2];
} stt_ships;

//Declare Players
typedef struct {
    int result[GSIZE][GSIZE]; //solution table
    char current[GSIZE][GSIZE]; //playing table
    int totalShots; //shots received
    int targetShots; //shots on tragets received
    stt_ships ships[SHIPNUM]; //player ships
} stt_gamePlayer;
//////////////

//function prototypes
int gmLoad(stt_gamePlayer gmPlayer[]);
int checkShipsPos(char cords[], char ori, int s);
int setShipsPl(stt_gamePlayer gmPlayer[], int p, int s);

//Main funtion (get menu)
void main() {
    char opt;
    setlocale(LC_ALL, "english");
    
    printf("----------------------------------\n");
    printf("Welcome to BattleShip Game\n\n");
    printf("This game was made by:\n");
    printf("Diogo Sénico\n");
    printf("João Zeferino\n");
    printf("----------------------------------\n");
	system("pause");
	
	do {
		system("cls");
		printf("\n Menu:\n");
	    printf(" 1 - New game\n");
	    printf(" 2 - Continue\n");
	    printf(" 3 - Exit\n");
	    opt = getch();
    
	    switch(opt) {
		    case '1':
		        start(0); break;
		    case '2':
		        start(1); break;
		    case '3':
		        return;
		}
    } while(opt != 3);
}


//start game (load=1 -> load saved game // load=0 ->create new game)
void start(int load) {
    int p, winner = -1, loaded = 0, loser = -1;
    
    stt_gamePlayer gmPlayer[PLAYERS];
    if(load == 0) {
    	system("cls");
        newGame(&gmPlayer);
        getShipsPl(&gmPlayer);
        system("cls");
    } else if(load == 1) {
    	loaded = gmLoad(&gmPlayer);
    	//check if file exists
    	if(loaded == 0){
    		printf("\n\nNo save files to load\n\n");
    		system("pause");
    		return;
		}
    	getCurGameData(&gmPlayer);
	}
	//upload to savefile
    uploadToFile(&gmPlayer);
    
   
	system("cls");
	printf("Lets start the game!!!\n");
    printf("------------------------------------------\n");
	printf("The player should introduce values between:\n");
	printf("Coords x (0-9)\n");
	printf("Coords y (a-j)\n");
	printf("\nEX:\nCoords:3f \n\n");
	system("pause");
	
	//run game. repeat until winner is found
	do {
		runGame(&gmPlayer, &winner, &loser);
		uploadToFile(&gmPlayer);
	} while (winner == -1);
    system("cls");
    
    //present winner
    printf("-----------------------\n");
	printf("Congrats Player %d\n", winner+1);
	printf("Shots made by winner: %d\n", gmPlayer[loser].totalShots);
    printf("-----------------------\n");
	system("pause");

}

//load "saves.txt" file
int gmLoad(stt_gamePlayer gmPlayer[]) {
    FILE *fileCont;
    int i=0, p=0, t=1;

    fileCont = fopen("saves.txt", "r");
    //check if file exists
    if (fileCont) {
    	while(!feof(fileCont)){
    		switch(t) {
    			case 1: //fscanf table 1
		    		fscanf(fileCont, "%c %c %c %c %c %c %c %c %c %c \n",
		                   &gmPlayer[0].current[i][0],
		                   &gmPlayer[0].current[i][1],
		                   &gmPlayer[0].current[i][2],
		                   &gmPlayer[0].current[i][3],
		                   &gmPlayer[0].current[i][4],
		                   &gmPlayer[0].current[i][5],
		                   &gmPlayer[0].current[i][6],
		                   &gmPlayer[0].current[i][7],
		                   &gmPlayer[0].current[i][8],
		                   &gmPlayer[0].current[i][9]
		                  );
    				break; 
    			case 2: //fscanf table 2
		    		fscanf(fileCont, "%d %d %d %d %d %d %d %d %d %d \n",
		                   &gmPlayer[0].result[i][0],
		                   &gmPlayer[0].result[i][1],
		                   &gmPlayer[0].result[i][2],
		                   &gmPlayer[0].result[i][3],
		                   &gmPlayer[0].result[i][4],
		                   &gmPlayer[0].result[i][5],
		                   &gmPlayer[0].result[i][6],
		                   &gmPlayer[0].result[i][7],
		                   &gmPlayer[0].result[i][8],
		                   &gmPlayer[0].result[i][9]
		                  );
    				break;
    			case 3: //fscanf table 3
    				fscanf(fileCont, "%c %c %c %c %c %c %c %c %c %c \n",
		                   &gmPlayer[1].current[i][0],
		                   &gmPlayer[1].current[i][1],
		                   &gmPlayer[1].current[i][2],
		                   &gmPlayer[1].current[i][3],
		                   &gmPlayer[1].current[i][4],
		                   &gmPlayer[1].current[i][5],
		                   &gmPlayer[1].current[i][6],
		                   &gmPlayer[1].current[i][7],
		                   &gmPlayer[1].current[i][8],
		                   &gmPlayer[1].current[i][9]
		                  );
    				break;
    			case 4: //fscanf table 4
    				fscanf(fileCont, "%d %d %d %d %d %d %d %d %d %d \n",
		                   &gmPlayer[1].result[i][0],
		                   &gmPlayer[1].result[i][1],
		                   &gmPlayer[1].result[i][2],
		                   &gmPlayer[1].result[i][3],
		                   &gmPlayer[1].result[i][4],
		                   &gmPlayer[1].result[i][5],
		                   &gmPlayer[1].result[i][6],
		                   &gmPlayer[1].result[i][7],
		                   &gmPlayer[1].result[i][8],
		                   &gmPlayer[1].result[i][9]
		                  );
    				break;
			}
			i++;
			//cicle throw tables
			if(i==GSIZE){
				t++;
				i=0;
			}
			
			if(t == 5) {
				break;
			}
		}
	} else {
		return 0;
	}
	fclose(fileCont);
	return 1;

}

//get total shots received for each player after the save is loaded
void getCurGameData (stt_gamePlayer gmPlayer[]) {
	int iCur, iRes, jCur, jRes, p;
    for(p=0; p<PLAYERS; p++) {
		gmPlayer[p].totalShots = 0;
		gmPlayer[p].targetShots = 0;
    	for(iCur=0; iCur<GSIZE; iCur++) {
    		for(jCur=0; jCur<GSIZE; jCur++){
				if(gmPlayer[p].current[iCur][jCur] != NOTHIT) {
					gmPlayer[p].totalShots++;
				}
				if(gmPlayer[p].current[iCur][jCur] == ONTARGET) {
					gmPlayer[p].targetShots++;
				}
			}
    	}
    }
}


//inicialize players tables and shots
void newGame(stt_gamePlayer gmPlayer[]) {
    int i, j, p;

    for(p=0; p<PLAYERS; p++) {
		gmPlayer[p].totalShots = 0;
		gmPlayer[p].targetShots = 0;
        for(i=0; i<GSIZE; i++) {
            for(j=0; j<GSIZE; j++) {
                gmPlayer[p].result[i][j] = 0;
                gmPlayer[p].current[i][j] = NOTHIT;
            }
        }
    }
}

//upload players tables to save.txt
void uploadToFile (stt_gamePlayer gmPlayer[]) {
	
    FILE *fileCont;
    int iCur, iRes, jCur, jRes, p;

    fileCont = fopen("saves.txt", "w");
    for(p=0; p<PLAYERS; p++) {
    	for(iCur=0; iCur<GSIZE; iCur++) {
    		for(jCur=0; jCur<GSIZE; jCur++){
				fprintf(fileCont, "%c ", gmPlayer[p].current[iCur][jCur]);
			}
			fprintf(fileCont, "%s", "\n");
    	}
    	for(iRes=0; iRes<GSIZE; iRes++) {
    		for(jRes=0; jRes<GSIZE; jRes++){
				fprintf(fileCont, "%d ", gmPlayer[p].result[iRes][jRes]);
			}
			fprintf(fileCont, "%s", "\n");
    	}
    }
    fclose(fileCont);
}

//get ships position and orientation
void getShipsPl(stt_gamePlayer gmPlayer[]) {

    int s, p, iRes, jRes, res = 0;
    char cords[2], ori;
	
	//Define Ships size
	for(p=0; p<PLAYERS; p++){
	    gmPlayer[p].ships[0].sh_size = 2;
	    gmPlayer[p].ships[1].sh_size = 2;
	    gmPlayer[p].ships[2].sh_size = 3;
	    gmPlayer[p].ships[3].sh_size = 3;
	    gmPlayer[p].ships[4].sh_size = 5;
	}
	printf("Insert ship coords and orientation\n");
    printf("----------------------------------------------\n");
	printf("Ship 1: (Size 2)\n");
    printf("Ship 2: (Size 2)\n");
    printf("Ship 3: (Size 3)\n");
    printf("Ship 4: (Size 3)\n");
    printf("Ship 5: (Size 5)\n");
    printf("----------------------------------------------\n\n");
    printf("Coords y (0-9)\n");
    printf("Coords x (a-j)\n");
    printf("Orientation (v->Vertical / h->Horizontal)\n");
    printf("----------------------------------------------\n");
    printf("\nEX:\nShip 1: 3a h\n\n");
    
    system("pause");
    system("cls");

	for(p=0; p<PLAYERS; p++) {
	    for(s=0; s<SHIPNUM; s++) {
	    	//print current player table to screen
		    printf("\n Field Player %d:\n", p+1);
	    	printf(" "CL_BLACK_WHITE"   a b c d e f g h i j\n"CL_RESET);
			for(iRes=0; iRes<GSIZE; iRes++) {
				printf(" "CL_BLACK_WHITE" %d "CL_RESET, iRes);
				for(jRes=0; jRes<GSIZE; jRes++){
					if(p == 0) {
						printf(CL_RED"%c "CL_RESET, gmPlayer[p].result[iRes][jRes] ? ONTARGET : ONWATER);
					} else {
						printf(CL_BLUE"%c "CL_RESET, gmPlayer[p].result[iRes][jRes] ? ONTARGET : ONWATER);
					}			
				}
				printf("\n");
			}
			res = 0;
			//test if cords are inserted properly
			do {
	    		do {
		    		printf("\n Ship %d: ", s+1);
		    		scanf("%c%c %c", &cords[0], &cords[1], &ori);
		    		fflush(stdin);
				} while (!checkShipsPos(cords, ori, s));
		        gmPlayer[p].ships[s].sh_cords[0] = cords[0];
				gmPlayer[p].ships[s].sh_cords[1] = cords[1];
				gmPlayer[p].ships[s].sh_ori = ori;
				res = setShipsPl(gmPlayer, p, s);
			} while(res);
			system("cls");
	    }
	}

}

//game display
void runGame(stt_gamePlayer gmPlayer[], int *winner, int *loser) {
    int pPlaying, pField, verif = 0, p, n=PLAYERS-1;
    char cords[2];
	pPlaying = 0;
	pField = PLAYERS-1;
	
	//check if already exists a winner
	for(p=0; p<PLAYERS; p++){
		if(gmPlayer[n].targetShots == TOTALSHOTS) {
			*winner = p;
			*loser = n;
			return;
		}
		n--;
	}
	
	//cicle between players
	for(pPlaying=0; pPlaying<PLAYERS; pPlaying++) {
		//test if exists already a winner
		if(gmPlayer[pField].targetShots == TOTALSHOTS) {
			*winner = pPlaying;
			*loser = pField;
			return;
		}
		system("cls");
		printf("\n\n Field Player %d\n\n", pField+1);
		
		//show table os oponent
    	showGameField(gmPlayer, pField);
    	
    	printf("\nTurn Player %d (TotalShots: %d)", pPlaying + 1, gmPlayer[pField].totalShots);
    	
    	//verifo cords
    	do {
	    	printf("\nCoords:");
    		scanf("%c%c", &cords[0], &cords[1]);
    		fflush(stdin);
			if(cords[0] > '9' || cords[0] < '0' || cords[1] > 'j' || cords[1] < 'a'){
				printf("Invalid Coords");
				verif = 0;
    		} else if(gmPlayer[pField].current[cords[0]-48][cords[1]-97] != NOTHIT) {
    			printf("Coords already inserted");
    			verif = 0;
			} else {
				verif = 1;
			}
    	} while(!verif);
    	
		//reload game field with message
		system("cls");
		
    	if(gmPlayer[pField].result[cords[0]-48][cords[1]-97] == 1) {
    		gmPlayer[pField].current[cords[0]-48][cords[1]-97] = ONTARGET;
    		gmPlayer[pField].targetShots++;
    		printf(CL_BLACK_WHITE" SHOT ON TARGET\n\n"CL_RESET);
		} else {
			gmPlayer[pField].current[cords[0]-48][cords[1]-97] = ONWATER;
			printf(CL_BLACK_WHITE" SHOT ON WATER\n\n"CL_RESET);
		}
		gmPlayer[pField].totalShots++;
		printf(" Field Player %d\n\n", pField+1);
		
		showGameField(gmPlayer, pField);
    	
		pField--;
		system("pause");
	}
	
	

}

//load game table
void showGameField(stt_gamePlayer gmPlayer[], int p) {
	int iCur, jCur;
	printf(" "CL_BLACK_WHITE"   a b c d e f g h i j\n"CL_RESET);
	for(iCur=0; iCur<GSIZE; iCur++) {
		printf(" "CL_BLACK_WHITE" %d "CL_RESET, iCur);
		for(jCur=0; jCur<GSIZE; jCur++){
			if(p == 0) {
				printf(CL_RED"%c "CL_RESET, gmPlayer[p].current[iCur][jCur]);
			} else {
				printf(CL_BLUE"%c "CL_RESET, gmPlayer[p].current[iCur][jCur]);
			}
		}
		printf("\n");
	}
	printf("\nWater -> %c\n", ONWATER);
	printf("Shot -> %c\n", ONTARGET);
	printf("Not Shoted -> %c\n", NOTHIT);
}

//check inserted cords for ships positon
int checkShipsPos(char cords[], char ori, int shipNumber) {
	if(cords[0] > '9' || cords[0] < '0' || cords[1] > 'j' || cords[1] < 'a' || !(ori == 'v' || ori == 'h')){
		printf("Coords or orientation out of limits or incorrect");
	} else {
		switch(shipNumber) {
			case 0: //verify ship 1
				if((cords[0] > '8') && (ori == 'v')){
					printf("Logitude to high");
				} else if((cords[1] > 'i') && (ori == 'h')){
					printf("Latitude to high");
				} else {
					return 1;
				}
				break;
			case 1: //verify ship 2
				if((cords[0] > '8') && (ori == 'v')){
					printf("Logitude to high");
				} else if((cords[1] > 'i') && (ori == 'h')){
					printf("Latitude to high");
				} else {
					return 1;
				} 
				break;
			case 2: //verify ship 3
				if((cords[0] > '7') && (ori == 'v')){
					printf("Logitude to high");
				} else if((cords[1] > 'h') && (ori == 'h')){
					printf("Latitude to high");
				} else {
					return 1;
				}
				break;
			case 3: //verify ship 4
				if((cords[0] > '7') && (ori == 'h')){
					printf("Logitude to high");
				} else if((cords[1] > 'h') && (ori == 'v')){
					printf("Latitude to high");
				} else {
					return 1;
				}
				break;
			case 4: //verify ship 5
				if((cords[0] > '5') && (ori == 'v')){
					printf("Logitude to high");
				} else if((cords[1] > 'f') && (ori == 'h')){
					printf("Latitude to high");
				} else {
					return 1;
				}
				break;
		}
	}
	return 0;
}

//set ships and check colision between then
int setShipsPl(stt_gamePlayer gmPlayer[], int p, int shipNumber) {
    
    int i, x, y;
    
    //pass char coords to their respective numbers
    x = gmPlayer[p].ships[shipNumber].sh_cords[0]-48;
    y = gmPlayer[p].ships[shipNumber].sh_cords[1]-97;
	
    for(i=0; i<gmPlayer[p].ships[shipNumber].sh_size; i++) {
        if(gmPlayer[p].ships[shipNumber].sh_ori == 'v') {
        	if(gmPlayer[p].result[x + i][y] != 1) {
            	gmPlayer[p].result[x + i][y] = 1;
        	} else {
        		printf("Overlapped Boats");
        		return 1;
			}
        } else if(gmPlayer[p].ships[shipNumber].sh_ori == 'h') {
        	if(gmPlayer[p].result[x][y + i] != 1){
            	gmPlayer[p].result[x][y + i] = 1;
        	} else {
        		printf("Overlapped Boats");
        		return 1;
			}
        }
    }
    return 0;
}

