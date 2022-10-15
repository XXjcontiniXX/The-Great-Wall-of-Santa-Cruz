#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
uint64_t seed = 613;

int *ptr_v = &v; // points to extern v

int *ptr_coins = &coins; 

int *ptr_players = &players; // points to extern players 

uint64_t *ptr_seed = &seed;

int main(int argc, char **argv) {
        int opt = 0;
        while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		mtrand_seed(613);
		switch(opt) {
		case 'p':
			if (atoi(optarg) < 2 || atoi(optarg) > 8) {
				printf("./play-dreidel: error: players must be 2-8\n"); // I'm using same error phrase as Ben showed in discord
				exit(1);
			} 
			*ptr_players = atoi(optarg);
		      	break; 
		case 'c':
			if (atoi(optarg) < 1 || atoi(optarg) > 20) {
				printf("./play_dreidel: error: coins must be 1-20\n"); 
				exit(1);
			}
		      	*ptr_coins = atoi(optarg);
			break;
		case 's': 
			if (strlen(optarg) > 10 || 0 > strtoll(optarg, NULL, 10)) { // long long because because long int can't hold -1e10 
				printf("./play_dreidel: error: seed must be positive and less than 1e10\n");
				exit(1);
			}
			*ptr_seed = strtoul(optarg, NULL, 10); 
			mtrand_seed(strtoul(optarg, NULL, 10));
			break;
		case 'v':
			*ptr_v = 1; 	
			break;

		}
	}
	
	play_game(players, coins, &rounds);	// &rounds will point to extern rounds
	printf("%s %d %d %d %lu\n", names[winner], players, coins, rounds, seed);		



}
