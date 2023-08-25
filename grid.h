
#ifndef GRID_H
#define GRID_H

#include "battleship_types.h"

/**************************
 * Constantes
 * ***********************/
#define GRID_SIDE_SIZE (10)

/**************************
 * Typedefs
 * ***********************/

/* Structure utilis�e pour d�montrer la position de la grille. Il contient
 * une paire de nombres, la position horizontale et la position verticale
 * */
typedef struct GridPosition
{
    uint8_t horizontal;
    uint8_t vertical;
} tsPosition;

/* Enum utilis� pour diff�rencier le type de cellule dans les grilles*/
typedef enum CellType
{
    Type_target,
    Type_ocean
} tenCellType;

/* Typedef � utiliser dans les structures */
typedef enum CellStatus
{
    Status_empty,
    Status_boat,
    Status_hit,
    Status_noHit
} tenCellStatus;

/* Enum utilis� pour d�terminer le bateau qui est soit touch�, soit ins�r� dans une cellule */
typedef enum BoatType
{
    Boat_carrier,
    Boat_battleship,
    Boat_cruiser,
    Boat_submarine,
    Boat_destroyer,
    Boat_empty
} tenBoatType;

/* Enum pour d�terminer la direction dans laquelle le bateau fait face */
typedef enum BoatDirection
{
    Direction_left_right,
    Direction_right_left,
    Direction_up_down,
    Direction_down_up,
    Direction_invalid
} tenBoatDirection;

/* Structure utilis�e pour stocker les informations de chaque cellule des grilles */
typedef struct GridCell
{
    tenCellType type;
    tenCellStatus status;
    tenBoatType boat;
} tsGridCell;

/* Structure utilis�e pour repr�senter l'oc�an ou la grille cible du jeu
 * */
typedef struct Grid
{
    tsGridCell points[GRID_SIDE_SIZE * GRID_SIDE_SIZE];
} tsGrid, tsOceanGrid, tsTargetGrid;

/***************************
 * Function declarations
 **************************/
/* Fonction permettant d'initialiser la grille cible pass�e en pointeur */
void Grid_initTarget(tsTargetGrid *grid);
/*Fonction permettant d'initialiser la grille oc�anique pass�e en pointeur */
void Grid_initOcean(tsOceanGrid *grid);
/* Fonction permettant de d�terminer si la position pass�e en param�tre est vide*/
bool Grid_isGridEmptyAt(tsGrid *grid, tsPosition position);
/* Fonction d'assignation d'un bateau dans la grille Oc�an. Renverra TRUE si possible pour ins�rer
 * le bateau, sinon il retournera FALSE*/
bool Grid_assignBoatInOcean(tsOceanGrid *grid,
                            tsPosition startPosition,
                            tenBoatType boat,
                            tenBoatDirection direction);
/* Fonction pour valider si le plan pr�vu est valide dans la grille*/
bool Grid_isValidShopInTarget(tsTargetGrid *grid, tsPosition position);
/* Fonction permettant d'effectuer un tir � un endroit donn� de la grille Oc�an. Renvoie TRUE si
 * c'�tait un succ�s, sinon, il renvoie faux */
bool Grid_assignShotInOcean(tsOceanGrid *grid, tsPosition position, tenBoatType* boat);
/* Fonction pour mettre � jour la grille cible avec le nouveau statut et le type de bateau en cas de succ�s
 * Renvoie TRUE si la mise � jour a r�ussi, sinon renvoie FALSE */
bool Grid_assignStatusInTarget(tsTargetGrid* grid, tsPosition position, tenCellStatus status, tenBoatType boat);

void Grid_printOcean(tsOceanGrid *grid);

void Grid_printTarget(tsTargetGrid *grid);

#endif /* GRID_H */
