#pragma once
#include <stdint.h>

int play_game(int n_players , int coins_per_player , int * n_rounds);

char spin_dreidel(void);

void play_round(int players[], int *coins_in_pot, int num_players, int *finished);

int chk_fin(int users[], int number_players, int *won);

static int pot;

static int winner;

static int fin;

extern int rounds;

extern int players;

extern int v;

static char * names[] = {"Aharon", "Batsheva", "Chanah", "David", "Ephraim", "Faige", "Gamaliel", "Hannah"};



