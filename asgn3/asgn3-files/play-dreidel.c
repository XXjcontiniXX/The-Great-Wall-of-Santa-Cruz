#include <stdio.h>
#include <stdlib.h>
#include "dreidel.h"
#include "mtrand.h"
#include <unistd.h>
#include <stdint.h>

#define OPTIONS "vp:c:s:"



int rounds = 0; // defining an extern global variable
int play_game (int n_players , int coins_per_player , int * n_rounds); 

int coins = 3;
int players = 4; // defining an extern global variable
int v = 0; // defining an extern global variable 

int *ptr_v = &v; // points to extern v

int *ptr_coins = &coins; 

int *ptr_players = &players; // points to extern players

int main(int argc, char **argv) {
        int opt = 0;
        while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		mtrand_seed(613);
		switch(opt) {
		case 'p':
			*ptr_players = atoi(optarg);
		      	break; 
		case 'c':
		      	*ptr_coins = atoi(optarg);
			break;
		case 's':
			mtrand_seed(strtoul(optarg, NULL, 10));
			break;
		case 'v':
			*ptr_v = 1; 	
			break;


		}
	}
	
	play_game(players, coins, &rounds); // &rounds will point to extern rounds



}
