
#include <malloc.h>
#include <stdio.h>
#include "game.h"
#include "grid.h"
#include "game_utils.h"
#include "game_logic.h"

/* Pointeurs vers les grilles oc�aniques utilis�es pour le jeu */
static tsOceanGrid* s_pOceanGrid_player1;
static tsOceanGrid* s_pOceanGrid_computer;

/*Pointeurs vers les grilles cibles utilis�es pour le jeu */
static tsTargetGrid* s_pTargetGrid_player1;
static tsTargetGrid* s_pTargetGrid_computer;

void Game_init()
{
    /* Cr�e les grilles oc�aniques pour le joueur et l'ordinateur*/
    s_pOceanGrid_player1 = (tsOceanGrid*) malloc(sizeof (tsOceanGrid));
    s_pOceanGrid_computer = (tsOceanGrid*) malloc(sizeof (tsOceanGrid));

    /* Initialise les grilles oc�aniques */
    Grid_initOcean(s_pOceanGrid_player1);
    Grid_initOcean(s_pOceanGrid_computer);

    /* Cr�e les grilles cibles pour le joueur et l'ordinateur */
    s_pTargetGrid_player1 = (tsTargetGrid*) malloc(sizeof (tsTargetGrid));
    s_pTargetGrid_computer = (tsTargetGrid*) malloc(sizeof (tsTargetGrid));

    /* Initialise les grilles cibles*/
    Grid_initTarget(s_pTargetGrid_player1);
    Grid_initTarget(s_pTargetGrid_computer);
}

void Game_exit()
{
    printf("fin de la partie\n");
    if (s_pOceanGrid_player1)
        free(s_pOceanGrid_player1);
    if (s_pOceanGrid_computer)
        free(s_pOceanGrid_computer);
    if (s_pTargetGrid_player1)
        free(s_pTargetGrid_player1);
    if (s_pTargetGrid_computer)
        free(s_pTargetGrid_computer);
}

void Game_play()
{
    /* Initialise la structure dans le jeu*/
    tsGameLogic gameLogic;
    bool tempBool = FALSE;
    bool isReplay = FALSE;

    /* D�finir les pointeurs de la structure comme ceux utilis�s dans le jeu*/
    gameLogic.player1_ocean = s_pOceanGrid_player1;
    gameLogic.player1_target = s_pTargetGrid_player1;
    gameLogic.computer_ocean = s_pOceanGrid_computer;
    gameLogic.computer_target = s_pTargetGrid_computer;
    /* Initialise le jeu */
    GameLogic_init(&gameLogic);
    isReplay = GameLogic_isFromPreviousGame(&gameLogic);

    if (isReplay == TRUE) {
        /* C'est le processus pour revisionner une partie */
        if (gameLogic.status != GameStatus_invalid) {
            GameLogic_startGame(&gameLogic);
        }
    } else {
        GameLogic_startGame(&gameLogic);
        /* Ceci est le traitement d'un jeu en cours */
        tempBool = GameLogic_isAgainstComputer(&gameLogic);
        /* Ajout des bateaux pour le Joueur 1*/
        GameUtil_addBoats(s_pOceanGrid_player1, GameTurn_Player1, gameLogic.file_handler);

        /* Ajout des bateaux pour le joueur deux/ordinateur */
        if (tempBool == TRUE) {
            /* ajout automatique des bateaux pour l'ordinateur */
            GameUtil_automaticSelectBoats(s_pOceanGrid_computer, gameLogic.file_handler);
        } else {
            GameUtil_addBoats(s_pOceanGrid_computer, GameTurn_Player2, gameLogic.file_handler);
        }
    }

    /* Boucle pour jouer le jeu jusqu'� ce qu'il soit termin� */
    while (gameLogic.status == GameStatus_playing) {

        GameLogic_step(&gameLogic);
    }
    /* fin de la partie */
    GameLogic_endGame(&gameLogic);
}


