
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

/* Structure utilisée pour démontrer la position de la grille. Il contient
 * une paire de nombres, la position horizontale et la position verticale
 * */
typedef struct GridPosition
{
    uint8_t horizontal;
    uint8_t vertical;
} tsPosition;

/* Enum utilisé pour différencier le type de cellule dans les grilles*/
typedef enum CellType
{
    Type_target,
    Type_ocean
} tenCellType;

/* Typedef à utiliser dans les structures */
typedef enum CellStatus
{
    Status_empty,
    Status_boat,
    Status_hit,
    Status_noHit
} tenCellStatus;

/* Enum utilisé pour déterminer le bateau qui est soit touché, soit inséré dans une cellule */
typedef enum BoatType
{
    Boat_carrier,
    Boat_battleship,
    Boat_cruiser,
    Boat_submarine,
    Boat_destroyer,
    Boat_empty
} tenBoatType;

/* Enum pour déterminer la direction dans laquelle le bateau fait face */
typedef enum BoatDirection
{
    Direction_left_right,
    Direction_right_left,
    Direction_up_down,
    Direction_down_up,
    Direction_invalid
} tenBoatDirection;

/* Structure utilisée pour stocker les informations de chaque cellule des grilles */
typedef struct GridCell
{
    tenCellType type;
    tenCellStatus status;
    tenBoatType boat;
} tsGridCell;

/* Structure utilisée pour représenter l'océan ou la grille cible du jeu
 * */
typedef struct Grid
{
    tsGridCell points[GRID_SIDE_SIZE * GRID_SIDE_SIZE];
} tsGrid, tsOceanGrid, tsTargetGrid;

/***************************
 * Function declarations
 **************************/
/* Fonction permettant d'initialiser la grille cible passée en pointeur */
void Grid_initTarget(tsTargetGrid *grid);
/*Fonction permettant d'initialiser la grille océanique passée en pointeur */
void Grid_initOcean(tsOceanGrid *grid);
/* Fonction permettant de déterminer si la position passée en paramètre est vide*/
bool Grid_isGridEmptyAt(tsGrid *grid, tsPosition position);
/* Fonction d'assignation d'un bateau dans la grille Océan. Renverra TRUE si possible pour insérer
 * le bateau, sinon il retournera FALSE*/
bool Grid_assignBoatInOcean(tsOceanGrid *grid,
                            tsPosition startPosition,
                            tenBoatType boat,
                            tenBoatDirection direction);
/* Fonction pour valider si le plan prévu est valide dans la grille*/
bool Grid_isValidShopInTarget(tsTargetGrid *grid, tsPosition position);
/* Fonction permettant d'effectuer un tir à un endroit donné de la grille Océan. Renvoie TRUE si
 * c'était un succès, sinon, il renvoie faux */
bool Grid_assignShotInOcean(tsOceanGrid *grid, tsPosition position, tenBoatType* boat);
/* Fonction pour mettre à jour la grille cible avec le nouveau statut et le type de bateau en cas de succès
 * Renvoie TRUE si la mise à jour a réussi, sinon renvoie FALSE */
bool Grid_assignStatusInTarget(tsTargetGrid* grid, tsPosition position, tenCellStatus status, tenBoatType boat);

void Grid_printOcean(tsOceanGrid *grid);

void Grid_printTarget(tsTargetGrid *grid);

#endif /* GRID_H */
