#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "battleship_types.h"
#include "grid.h"

/**************************
 * Constantes
 * ***********************/
extern const uint32_t SHOTS_NEEDED_TO_WIN;

/**************************
 * Typedefs
 * ***********************/
/* Enum utilisé pour différencier les différents types de jeux */
typedef enum GameType
{
    GameType_PlayerVsComputer,
    GameType_PlayerVsPlayer,
    GameType_Replay,
    GameType_invalid
} tenGameType;

/* Enum utilisé pour différencier les tours du jeu */
typedef enum GameTurn
{
    GameTurn_Player1,
    GameTurn_Computer,
    GameTurn_Player2
} tenGameTurn;

/* Enum utilisé pour indiquer le status actuel du jeu */
typedef enum GameStatus
{
    GameStatus_invalid,
    GameStatus_init,
    GameStatus_playing,
    GameStatus_finished
} tenGameStatus;

/* Structure utilisée pour sauvegarder toutes les informations du jeu*/
typedef struct GameLogic
{
    tenGameType gameType;
    tenGameTurn turn;
    tenGameStatus status;
    tsOceanGrid* player1_ocean;
    tsTargetGrid* player1_target;
    tsOceanGrid* computer_ocean;
    tsTargetGrid* computer_target;
    FILE* file_handler;
} tsGameLogic;


/* Fonction permettant de demander à l'utilisateur si le jeu est un replay d'un jeu précédent ou non */
bool GameLogic_isFromPreviousGame(tsGameLogic* gameLogic);

/* Fonction utilisée pour initialiser la structure utilisée pour la logique du jeu */
void GameLogic_init(tsGameLogic* gameLogic);

/* Fonction utilisée pour demander à l'utilisateur si le jeu est contre l'ordinateur ou non */
bool GameLogic_isAgainstComputer(tsGameLogic *gameLogic);

/* fonction pour finir la partie */
void GameLogic_endGame(tsGameLogic* gameLogic);

/* Fonction pour démarrer le jeu */
void GameLogic_startGame(tsGameLogic* gameLogic);

/* Fonction pour effectuer le prochain mouvement dans le jeu*/
void GameLogic_step(tsGameLogic* gameLogic);

/* Fonction pour déterminer si tous les bateaux de l'adversaire titulaire du tour actuel sont coulés */
bool GameLogic_isFinished(tsGameLogic* gameLogic);

#endif //GAME_LOGIC_H
