
#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include "grid.h"
#include "game_logic.h"

/***************************
 * D�clarations de fonction
 **************************/

/* Fonction utilis�e pour ajouter les bateaux dans la grille oc�anique*/
void GameUtil_addBoats(tsOceanGrid *grid, tenGameTurn turn, FILE *file_handler);

/* Fonction utilis�e pour demander � l'utilisateur d'ins�rer les bateaux dans la grille */
void GameUtil_askBoatPlacement(tsOceanGrid *grid, tenBoatType boat, const char *boatName, FILE *file_handler);

/* Fonction utilis�e pour demander � l'utilisateur sa prochainne action sur la grille cible */
void GameUtil_askNextMove(tsTargetGrid* grid, tsPosition* position);

/* Fonction pour s�lectionner automatiquement le placement des bateaux pour l'ordinateur */
void GameUtil_automaticSelectBoats(tsOceanGrid *grid, FILE *file_handler);

/* Fonction pour calculer le prochain tir automatiquement */
void GameUtil_automaticNextMove(tsTargetGrid* grid, tsPosition* position);

/* Convertit un argument de position en une string */
void GameUtil_positionToString(tsPosition* position, char* string, uint32_t max_size);

/* Convertit un argument de direction en une string */
void GameUtil_directionToString(tenBoatDirection direction, char* string, uint32_t max_size);


#endif //GAME_UTILS_H
