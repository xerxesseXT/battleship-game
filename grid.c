
#include <stdio.h>
#include "grid.h"
#include "utilities.h"

/* Constantes utilis�es pour calculer le placement des bateaux */
#define CARRIER_SIZE        (5)
#define BATTLESHIP_SIZE     (4)
#define CRUISER_SIZE        (3)
#define SUBMARINE_SIZE      (3)
#define DESTROYER_SIZE      (2)

const uint32_t SHOTS_NEEDED_TO_WIN = CARRIER_SIZE + BATTLESHIP_SIZE + CRUISER_SIZE + SUBMARINE_SIZE + DESTROYER_SIZE;

/* Fonction statique pour obtenir la cellule de grille de la grille dans la position donn�e */
static tsGridCell *getGridCell(tsGrid *grid, tsPosition position);

void Grid_initTarget(tsTargetGrid *grid)
{
    for (uint32_t i = 0; i < (GRID_SIDE_SIZE * GRID_SIDE_SIZE); i++) {
        grid->points[i].boat = Boat_empty;
        grid->points[i].type = Type_target;
        grid->points[i].status = Status_empty;
    }
}

void Grid_initOcean(tsOceanGrid *grid)
{
    for (uint32_t i = 0; i < (GRID_SIDE_SIZE * GRID_SIDE_SIZE); i++) {
        grid->points[i].boat = Boat_empty;
        grid->points[i].type = Type_ocean;
        grid->points[i].status = Status_empty;
    }
}

bool Grid_isGridEmptyAt(tsGrid *grid, tsPosition position)
{
    bool result = FALSE;
    tsGridCell *gridCell = getGridCell(grid, position);

    if (gridCell) {
        result = (gridCell->status == Status_empty);
    }
    return result;
}

bool Grid_assignBoatInOcean(tsOceanGrid *grid, tsPosition startPosition, tenBoatType boat, tenBoatDirection direction)
{
    bool result = FALSE;
    bool parametersValid = FALSE;
    tsGridCell *gridCell = getGridCell(grid, startPosition);
    tsPosition tempPosition = startPosition;
    uint32_t boatSize = 0;

    if (gridCell && boat < Boat_empty && direction < Direction_invalid) {
        parametersValid = TRUE;
        /* Calcule si le placement du bateau est valide selon le type */
        if (boat == Boat_carrier)
            boatSize = CARRIER_SIZE;
        else if (boat == Boat_battleship)
            boatSize = BATTLESHIP_SIZE;
        else if (boat == Boat_cruiser)
            boatSize = CRUISER_SIZE;
        else if (boat == Boat_destroyer)
            boatSize = DESTROYER_SIZE;
        else if (boat == Boat_submarine)
            boatSize = SUBMARINE_SIZE;

        for (uint32_t i = 0; i < boatSize; i++) {
            /* Si la cellule de la grille n'est pas vide ou est hors limites, nous ne
             * enregistrer le bateau � cet endroit */
            if (!Grid_isGridEmptyAt(grid, tempPosition)) {
                parametersValid = FALSE;
                break;
            }
            /* Modifie la position de l'�l�ment suivant dans la grille en fonction de la direction*/
            if (direction == Direction_left_right)
                tempPosition.horizontal++;
            else if (direction == Direction_right_left)
                tempPosition.horizontal--;
            else if (direction == Direction_up_down)
                tempPosition.vertical++;
            else if (direction == Direction_down_up)
                tempPosition.vertical--;
        }
    }

    /* Cela signifie qu'il est possible d'ins�rer le bateau � cet endroit, il faut donc ranger le
     * date dans la grille */
    if (parametersValid == TRUE)
    {
        result = TRUE;
        /* Red�marrez la position au point initial et mettez � jour la grille avec les informations du bateau */
        tempPosition = startPosition;
        for (uint32_t i = 0; i < boatSize; i++)
        {
            grid->points[tempPosition.vertical * GRID_SIDE_SIZE + tempPosition.horizontal].status = Status_boat;
            grid->points[tempPosition.vertical * GRID_SIDE_SIZE + tempPosition.horizontal].boat = boat;

            /* Modifie la position de l'�l�ment suivant dans la grille en fonction de la direction*/
            if (direction == Direction_left_right)
                tempPosition.horizontal++;
            else if (direction == Direction_right_left)
                tempPosition.horizontal--;
            else if (direction == Direction_up_down)
                tempPosition.vertical++;
            else if (direction == Direction_down_up)
                tempPosition.vertical--;
        }
    }

    return result;
}

bool Grid_isValidShopInTarget(tsTargetGrid *grid, tsPosition position)
{
    bool result = FALSE;
    tsGridCell* gridCell = getGridCell(grid, position);

    if (gridCell)
    {
        if (gridCell->status == Status_empty)
            result = TRUE;
    }
    return result;
}

bool Grid_assignShotInOcean(tsOceanGrid *grid, tsPosition position, tenBoatType* boat)
{
    bool result = FALSE;
    tsGridCell* gridCell = getGridCell(grid, position);

    if (gridCell)
    {
        /* Si le pointeur pass� est une grille oc�anique et qu'il y a un bateau � cette position*/
        if (gridCell->type == Type_ocean && gridCell->status == Status_boat)
        {
            /* Nous renvoyons le coup r�ussi, mettons � jour le type de bateau et d�finissons le
             * statut � atteindre dans la grille oc�anique*/
            result = TRUE;
            *boat = gridCell->boat;
            gridCell->status = Status_hit;
        }
    }

    return result;
}

bool Grid_assignStatusInTarget(tsTargetGrid *grid, tsPosition position, tenCellStatus status, tenBoatType boat)
{
    bool result = FALSE;
    tsGridCell* gridCell = getGridCell(grid, position);

    if (gridCell)
    {
        gridCell->status = status;
        gridCell->boat = boat;
        result = TRUE;
    }

    return result;
}

void Grid_printOcean(tsOceanGrid *grid)
{
    char element;
    printf("   0   1   2   3   4   5   6   7   8   9\n");
    for (uint32_t y = 0; y < GRID_SIDE_SIZE; y++) {
        printf(" -----------------------------------------\n");
        printf("%d", y);
        for (uint32_t x = 0; x < GRID_SIDE_SIZE; x++) {
            element = ' ';
            if (grid->points[y * GRID_SIDE_SIZE + x].status == Status_hit)
                element = 'X';
            else if (grid->points[y * GRID_SIDE_SIZE + x].status == Status_boat)
                element = 'B';
            printf("| %c ", element);
        }
        printf("|\n");
    }
    printf(" -----------------------------------------\n");
}

void Grid_printTarget(tsTargetGrid *grid)
{
    char element;
    printf("\n");
    printf("   0   1   2   3   4   5   6   7   8   9\n");
    for (uint32_t y = 0; y < GRID_SIDE_SIZE; y++) {
        printf(" -----------------------------------------\n");
        printf("%d", y);
        for (uint32_t x = 0; x < GRID_SIDE_SIZE; x++) {
            element = ' ';
            if (grid->points[y * GRID_SIDE_SIZE + x].status == Status_hit) {
                element = 'X';
            } else if (grid->points[y * GRID_SIDE_SIZE + x].status == Status_noHit) {
                element = 'O';
            }
            printf("| %c ", element);
        }
        printf("|\n");
    }
    printf(" -----------------------------------------\n");
}

tsGridCell *getGridCell(tsGrid *grid, tsPosition position)
{
    tsGridCell *gridCell = NULL;

    if (grid && position.vertical < GRID_SIDE_SIZE && position.horizontal < GRID_SIDE_SIZE) {
        gridCell = &grid->points[position.vertical * GRID_SIDE_SIZE + position.horizontal];
    }

    return gridCell;
}
