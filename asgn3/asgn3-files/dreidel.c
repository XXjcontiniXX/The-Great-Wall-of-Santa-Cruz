#include "dreidel.h"
#include <stdio.h>
#include <stdlib.h>
#include "mtrand.h"

int pot;

int winner;

int fin;


int play_game (int n_players , int coins_per_player , int * n_rounds) {
     
       	int players[8] = {0, 0, 0, 0, 0, 0, 0, 0};  //maximum 8 players
        
	for (int i = 0; i < n_players; ++i) {   //for every player give them their coins
                players[i] = coins_per_player;
	//	printf("Name: %s, player index: %d player's coins: %d-- \n", names[i], i, players[i]);
	}
	
	
	
	do {

	++*n_rounds;
	//printf("\n\n\n////ROUND: %d////\n", *n_rounds);
	play_round(players, &pot, n_players, &fin);   // plays round: changes pot value, changes fin value, changes players value
	
	} while (!!fin == 0);
 	//printf("\nrounds: %d\n", *n_rounds);	
	return winner;

}

int chk_fin(int users[], int number_players, int *won) {
	int los_demas = 0;
	int *ptr = &los_demas;
	for (int i = 0; i < number_players; ++i) {
		if (users[i] > 0) {
			*ptr = los_demas + 1;
			*won = i;
		}
		
	


	}
	//printf("\n\n\n%d users are still in\n\n\n", los_demas);
	if(los_demas == 1) {
		return 1;
	}else{
		return 0;
	}
}


void play_round(int round_players[], int *coins_in_pot, int num_players, int *finished) {

	for (int i = 0; i < num_players; ++i) {

		if (round_players[i] != -1) {
			int mod;
			mod = spin_dreidel();
			switch(mod) {
				case 'N':
					round_players[i] = round_players[i]; // nothing
					break;

				case 'G': 
					round_players[i] = round_players[i] + pot; //player gets pot
					*coins_in_pot = 0; // pot is zeroed
					break;

				case 'H':
					round_players[i] = round_players[i] + (pot/2); // player gets half pot
					*coins_in_pot = *coins_in_pot - (*coins_in_pot/2); //pot is half less
					break;

				case 'S':
					if (round_players[i] == 0) {
						round_players[i] = round_players[i] - 1; //players coins become negative
						
						if (v == 1) {
							printf("%s: eliminated in round %d of a %d player game.\n", names[i], rounds, players);
							}

						if (!!chk_fin(round_players, num_players, &winner)) {
							*finished = 1;
							}
					}else{
                                        	round_players[i] = round_players[i] - 1; //player loses one coin
                                        	*coins_in_pot = *coins_in_pot + 1; //pot gains one coin	
					}
					break;
			}
		}
	

	}
}

        		


char spin_dreidel(void) {
	switch (mtrand_rand64() % 4) {
		case 0:
			return 'G';
			break;

		case 1:
			return 'H';
			break;

		case 2:
			return 'N';
			break;
		
		case 3:
			return 'S';
			break;
		
		default :
			return 'B';
			break;
	
	}

}

