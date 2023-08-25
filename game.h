#ifndef GAME_H
#define GAME_H

#include "battleship_types.h"

/***************************
 * Déclarations de fonction
 **************************/
/* Fonction utilisée pour initialiser toutes les structures nécessaires au jeu */
void Game_init();
/* Fonction utilisée pour initialiser toutes les structures nécessaires au jeu */
void Game_exit();
/* Fonction utilisée pour Game_play du jeu , retourne TRUE si le jeu est toujours en cours */
void Game_play();

#endif //GAME_H
