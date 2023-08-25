#ifndef GAME_H
#define GAME_H

#include "battleship_types.h"

/***************************
 * D�clarations de fonction
 **************************/
/* Fonction utilis�e pour initialiser toutes les structures n�cessaires au jeu */
void Game_init();
/* Fonction utilis�e pour initialiser toutes les structures n�cessaires au jeu */
void Game_exit();
/* Fonction utilis�e pour Game_play du jeu , retourne TRUE si le jeu est toujours en cours */
void Game_play();

#endif //GAME_H
