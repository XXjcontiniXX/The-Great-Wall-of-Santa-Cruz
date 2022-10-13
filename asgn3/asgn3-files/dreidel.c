#include "dreidel.h"
#include <stdio.h>
#include <stdlib.h>
#include "mtrand.h"

extern int pot;

extern int winner;

extern int fin;




int play_game (int n_players , int coins_per_player , int * n_rounds) {
        int players[8] = {0, 0, 0, 0, 0, 0, 0, 0};  //maximum 8 players
        
	for (int i = 0; i < n_players; ++i) {   //for every player give them their coins
                players[i] = coins_per_player;
		printf("Name: %s, player index: %d player's coins: %d-- \n", names[i], i, players[i]);
	}
	
	
	
	do {

	++*n_rounds;
	printf("\n\n\n////ROUND: %d////\n", *n_rounds);
	play_round(players, &pot, n_players, &fin);   // plays round: changes pot value, changes fin value, changes players value
	
	} while (!!fin == 0);
 	printf("\nrounds: %d\n", *n_rounds);	
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
	printf("\n\n\n%d users are still in\n\n\n", los_demas);
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
			printf("\n\nLetter %c\n", mod);
			switch(mod) {
				case 'N':
					printf("User %s had %d coins\n", names[i], round_players[i]);
					round_players[i] = round_players[i]; // nothing
					printf("User now has %d coins", round_players[i]);
					break;

				case 'G': 
					printf("User %s had %d coins\n", names[i], round_players[i]);
					round_players[i] = round_players[i] + pot; //player gets pot
					*coins_in_pot = 0; // pot is zeroed
					printf("User now has %d coins", round_players[i]);
					break;

				case 'H':
					printf("Pot had %d coins\n", *coins_in_pot);
					printf("User %s had %d coins\n", names[i], round_players[i]);
					round_players[i] = round_players[i] + (pot/2); // player gets half pot
					*coins_in_pot = *coins_in_pot - (*coins_in_pot/2); //pot is half less
					printf("User now has %d coins\n", round_players[i]);
					printf("pot has %d coins", *coins_in_pot);
					break;

				case 'S':
					if (round_players[i] == 0) {
						round_players[i] = round_players[i] - 1; //players coins become negative
						if (!!chk_fin(round_players, num_players, &winner)) {
							*finished = 1;
							}
					}else{
						printf("Pot had %d coins\n", *coins_in_pot);
                                        	printf("User %s had %d coins\n", names[i], round_players[i]);
                                        	round_players[i] = round_players[i] - 1; //player loses one coin
                                        	*coins_in_pot = *coins_in_pot + 1; //pot gains one coin
                                        	printf("User now has %d coins\n", round_players[i]);
                                        	printf("Pot has %d coins", *coins_in_pot);			
						
						
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

