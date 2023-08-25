
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "utilities.h"
#include "game_utils.h"
#include "grid.h"
#include "enum_names.h"
#include "file_management.h"


#define STRING_BUFFER_SIZE      (20)
#define MAX_STEP_STRING_SIZE    (80)

void GameUtil_addBoats(tsOceanGrid *grid, tenGameTurn turn, FILE *file_handler)
{
    char step_string[MAX_STEP_STRING_SIZE] = { 0 };
    strcpy_s(step_string, MAX_STEP_STRING_SIZE, "BoatSelection;Player:");
    strcat_s(step_string, MAX_STEP_STRING_SIZE, PLAYERS_NAMES[turn]);
    strcat_s(step_string, MAX_STEP_STRING_SIZE, ";\n");
    FileMgmt_write(file_handler, step_string, strlen(step_string));

    GameUtil_askBoatPlacement(grid, Boat_carrier, "Carrier", file_handler);
    GameUtil_askBoatPlacement(grid, Boat_cruiser, "Cruiser", file_handler);
    GameUtil_askBoatPlacement(grid, Boat_submarine, "Submarine", file_handler);
    GameUtil_askBoatPlacement(grid, Boat_battleship, "Battleship", file_handler);
    GameUtil_askBoatPlacement(grid, Boat_destroyer, "Destroyer", file_handler);
}

void GameUtil_askBoatPlacement(tsOceanGrid *grid, tenBoatType boat, const char *boatName, FILE *file_handler)
{
    char input[STRING_BUFFER_SIZE];
    char step_string[MAX_STEP_STRING_SIZE] = { 0 };
    char temp[20] = { 0 };
    uint32_t size;
    tsPosition position;
    bool finished;
    bool isNumber;
    bool isValid = FALSE;
    tenBoatDirection direction;
    strcpy_s(step_string, MAX_STEP_STRING_SIZE, "Boat:");
    strcat_s(step_string, MAX_STEP_STRING_SIZE, boatName);
    strcat_s(step_string, MAX_STEP_STRING_SIZE, ";Position:");

    /* Tant que la sélection de l'utilisateur n'est toujours pas valide, continue à demander*/
    while (FALSE == isValid) {
        position.horizontal = 0;
        position.vertical = 0;
        finished = FALSE;
        isNumber = FALSE;
        isValid = FALSE;
        direction = Direction_invalid;

        /* Imprime la grille, il est donc plus facile pour l'utilisateur de sélectionner la position */
        Grid_printOcean(grid);
        /* Boucle jusqu'à ce que l'utilisateur entre une coordonnée valide */
        while (FALSE == finished) {
            printf("What is the X coordinate you want to place your %s?\n", boatName);
            size = Utils_getUserInput("", input, STRING_BUFFER_SIZE);
            isNumber = Utils_isANumber(input, size);

            /*Si l'entrée est un nombre compris dans les plages*/
            if (TRUE == isNumber && atoi(input) < GRID_SIDE_SIZE) {
                finished = TRUE;
                position.horizontal = (uint8_t) atoi(input);
            } else {
                printf("Invalid input. Try again\n");
            }
        }

        finished = FALSE;

        /* Boucle jusqu'à ce que l'utilisateur entre une coordonnée Y valide */
        while (FALSE == finished) {
            printf("What is the Y coordinate you want to place your %s?\n", boatName);
            size = Utils_getUserInput("", input, STRING_BUFFER_SIZE);
            isNumber = Utils_isANumber(input, size);

           /* Si l'entrée est un nombre compris dans les plages */
            if (isNumber == TRUE && atoi(input) < GRID_SIDE_SIZE) {
                finished = TRUE;
                position.vertical = (uint8_t) atoi(input);
            }
        }
        finished = FALSE;

        while (FALSE == finished) {
            printf("Directions:\n");
            printf("1. De gauche a droite\n");
            printf("2. De droite a gauche\n");
            printf("3. De haut en bas\n");
            printf("4. De bas en haut\n");
            size = Utils_getUserInput("Choisissez l'une des options:", input, STRING_BUFFER_SIZE);
            isNumber = Utils_isANumber(input, size);

            if (isNumber == TRUE && atoi(input) < 5) {
                finished = TRUE;
                direction = (tenBoatDirection)(atoi(input) - 1);
            } else {
                printf("Entree invalide. Reessayez\n");
            }
        }

        /*  Vérifie si l'affectation du bateau a pu être effectuée */
        isValid = Grid_assignBoatInOcean(grid, position, boat, direction);
        /* Cela imprime le message d'erreur pour informer l'utilisateur qu'il s'agit d'un emplacement non valide */
        if (FALSE == isValid) {
            printf("Cette position de depart et cette direction ne sont pas valides. Reessayez\n");
        }
    }
    GameUtil_positionToString(&position, temp, 20);
    strcat_s(step_string, MAX_STEP_STRING_SIZE, temp);
    memset(temp, 0, strlen(temp));
    strcat_s(step_string, MAX_STEP_STRING_SIZE, ";Direction:");
    GameUtil_directionToString(direction, temp, 20);
    strcat_s(step_string, MAX_STEP_STRING_SIZE, temp);
    FileMgmt_write(file_handler, step_string, strlen(step_string));
}

void GameUtil_askNextMove(tsTargetGrid *grid, tsPosition *position)
{
    char input[STRING_BUFFER_SIZE];
    bool finished = FALSE;
    bool isNumber = FALSE;
    bool isValid = FALSE;
    uint32_t size = 0;

    while (FALSE == isValid) {
        printf("Grille cible:\n");
        Grid_printTarget(grid);
        /* Boucle jusqu'à ce que l'utilisateur entre une coordonnée valide */
        while (FALSE == finished) {
            size = Utils_getUserInput("Quelle est la coordonnee X a atteindre?\n", input, STRING_BUFFER_SIZE);
            isNumber = Utils_isANumber(input, size);


            if (TRUE == isNumber && atoi(input) < GRID_SIDE_SIZE) {
                finished = TRUE;
                position->horizontal = (uint8_t) atoi(input);
            }
            else {
                printf("Entree non valide. Reessayez\n");
            }
        }
        finished = FALSE;

        while (FALSE == finished) {
            size = Utils_getUserInput("Quelle est la coordonnee Y a atteindre?\n", input, STRING_BUFFER_SIZE);
            isNumber = Utils_isANumber(input, size);

            if (TRUE == isNumber && atoi(input) < GRID_SIDE_SIZE) {
                finished = TRUE;
                position->vertical = (uint8_t) atoi(input);
            }
            else {
                printf("Entree non valide. Reessayez\n");
            }
        }
        isValid = Grid_isValidShopInTarget(grid, *position);
        if (isValid == FALSE) {
            finished = FALSE;
            printf("Cette position est deja occupe\n");
        }
    }
}

void GameUtil_automaticSelectBoats(tsOceanGrid *grid, FILE *file_handler)
{
    tsPosition position = { 0 };
    char step_string[MAX_STEP_STRING_SIZE] = { 0 };
    char temp[20] = { 0 };
    tenBoatDirection direction = Direction_invalid;
    bool isValid = FALSE;
    srand((unsigned) time(NULL));

    strcpy_s(step_string, MAX_STEP_STRING_SIZE, "BoatSelection;Player:Computer;");
    FileMgmt_write(file_handler, step_string, strlen(step_string));
    memset(step_string, 0, strlen(step_string));

    for (uint32_t i = Boat_carrier; i < Boat_empty; i++)
    {
        strcpy_s(step_string, MAX_STEP_STRING_SIZE, "Boat:");
        strcat_s(step_string, MAX_STEP_STRING_SIZE, BOAT_NAMES[i]);
        strcat_s(step_string, MAX_STEP_STRING_SIZE, ";Position:");
        isValid = FALSE;
        while (isValid == FALSE) {
            position.horizontal = rand() % (GRID_SIDE_SIZE);
            position.vertical = rand() % (GRID_SIDE_SIZE);


            /* nous définissons la direction horizontale ou verticale*/
            if ((rand() % 2) == 0) {
                if (rand() % 2 == 0) {
                    direction = Direction_right_left;
                } else {
                    direction = Direction_left_right;
                }
            } else {
                if (rand() % 2 != 2) {
                    direction = Direction_up_down;
                } else {
                    direction = Direction_down_up;
                }
            }

            isValid = Grid_assignBoatInOcean(grid, position, (tenBoatType)i, direction);
        }
        GameUtil_positionToString(&position, temp, 20);
        strcat_s(step_string, MAX_STEP_STRING_SIZE, temp);
        memset(temp, 0, strlen(temp));
        strcat_s(step_string, MAX_STEP_STRING_SIZE, ";Direction:");
        GameUtil_directionToString(direction, temp, 20);
        strcat_s(step_string, MAX_STEP_STRING_SIZE, temp);
        FileMgmt_write(file_handler, step_string, strlen(step_string));
        memset(step_string, 0, strlen(step_string));
    }
}

void GameUtil_automaticNextMove(tsTargetGrid *grid, tsPosition *position)
{
    bool isValid = FALSE;
    srand((unsigned) time(NULL));

    while (isValid == FALSE) {
        position->horizontal =  rand() % GRID_SIDE_SIZE;
        position->vertical = rand()  % GRID_SIDE_SIZE;
        isValid = Grid_isValidShopInTarget(grid, *position);
    }
}

void GameUtil_positionToString(tsPosition* position, char* string, uint32_t max_size)
{
    char temp[2] = { 0 };
    strcpy_s(string, max_size, "Hor:");
    _itoa_s(position->horizontal, temp, 2, 10);
    strcat_s(string, max_size, temp);
    strcat_s(string, max_size, ",Ver:");
    _itoa_s(position->vertical, temp, 2, 10);
    strcat_s(string, max_size, temp);
}

void GameUtil_directionToString(tenBoatDirection direction, char* string, uint32_t max_size)
{
    if (direction < Direction_invalid) {
        strcpy_s(string, max_size, DIRECTION_NAMES[direction]);
        strcat_s(string, max_size, ";\n");
    }
}

