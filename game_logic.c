
#ifdef __WIN32
#include <Windows.h>
//#include <direct.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "utilities.h"
#include "game_logic.h"
#include "game_utils.h"
#include "file_management.h"
#include "enum_names.h"

const char* PLAYERS_NAMES[] = {"Player1", "Computer", "Player2"};
const char* BOAT_NAMES[] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer", "Empty"};
const char* DIRECTION_NAMES[] = {"LeftToRight", "RightToLeft", "UpDown", "DownUp"};
const char* FILENAME_PREFIX = "battleship_";
#define MAX_FILENAME_SIZE       (35)
#define MAX_STEP_STRING_SIZE    (80)


static void GameLogic__stepReplay(tsGameLogic* gameLogic);


static void GameLogic__stepPlay(tsGameLogic* gameLogic);


static tenGameTurn GameLogic__getPlayersTurn(char* step_string);


static tsPosition GameLogic__getShotPosition(char* step_string);


static bool GameLogic__getHit(char* step_string);


static tenBoatType GameLogic__getBoat(char* step_string);


static tenBoatDirection GameLogic__getDirection(char* step_string);


static bool GameLogic__isTurnStep(char* step_string);


/******************************
 * Public Functions
 ******************************/
bool GameLogic_isFromPreviousGame(tsGameLogic* gameLogic)
{
    char input[20];
    bool finished = FALSE;
    bool result = FALSE;
    uint32_t tempValue = 0;
    uint32_t size = 0;

    while(finished == FALSE) {
        printf("Actions:\n");
        printf("1. Jouer au jeu\n");
        printf("2. Rejouer une partie precedemment sauvegardee\n");
        size = Utils_getUserInput("Qu'est-ce que vous voulez faire?", input, 20);
        if (Utils_isANumber(input, size) && atoi(input) > 0 && atoi(input) < 3) {
            finished = TRUE;
            tempValue = (uint32_t)atoi(input);
        } else {
            printf("Option non valide selectionnee. Reessayer\n");
        }
    }

    if (tempValue == 2) {
        gameLogic->gameType = GameType_Replay;
        result = TRUE;
    }


    if (gameLogic->gameType == GameType_Replay) {
        const char* folderName = "SavedGames";
        FileMgmt_stFiles* files = FileMgmt_getListOfFiles(folderName, strlen(folderName), 10);
        if (files) {
            for (uint32_t i = 0; i < files->size; i++) {
                printf("%d. %s\n", i + 1, files->files[i]->name);
            }
            finished = FALSE;
            printf("Quel est le nom du fichier a partir de ces options:\n");
            while (finished == FALSE) {
                size = Utils_getUserInput("faite votre choix", input, 20);
                if ((Utils_isANumber(input, size)) &&
                    (atoi(input) > 0) &&
                    (atoi(input) <= files->size)) {
                    finished = TRUE;
                    tempValue = (uint32_t)atoi(input);
                } else {
                    printf("Option non valide selectionnee. Reessayer\n");
                }
            }
            gameLogic->file_handler = FileMgmt_open(folderName,
                                                    files->files[tempValue - 1]->name,
                                                    strlen(files->files[tempValue - 1]->name),
                                                    FileMgmt_Read);
            if (gameLogic->file_handler == NULL)
            {
                printf("Erreur lors de l'ouverture du fichier\n");
                gameLogic->status = GameStatus_invalid;
            }
            FileMgmt_cleanFiles(files);
        } else {
            printf("Impossible de voir les fichiers de ce dossier\n");
        }

    }

    return result;
}

void GameLogic_init(tsGameLogic *gameLogic)
{
    gameLogic->gameType = GameType_invalid;
    gameLogic->turn = GameTurn_Player1;
    gameLogic->status = GameStatus_init;
}

bool GameLogic_isAgainstComputer(tsGameLogic *gameLogic)
{
    char input[20];
    bool finished = FALSE;
    bool result = FALSE;
    uint32_t tempValue = 0;
    tenGameType gameType = GameType_invalid;

    while (finished == FALSE) {
        printf("Aide en ligne :https://fr.wikipedia.org/wiki/Bataille_navale_(jeu)\n");
        printf("Types de jeux:\n");
        printf("1. Player vs. Computer\n");
        printf("2. Player vs. Player\n");

        uint32_t size = Utils_getUserInput("Choose the game type:", input, 20);
        if (Utils_isANumber(input, size) && atoi(input) > 0 && atoi(input) < 3) {
            finished = TRUE;
            tempValue = (uint32_t) atoi(input);
        } else {
            printf("Option non valide selectionnee. Essayer a nouveau\n");
        }
    }
    if (tempValue == 1) {
        gameType = GameType_PlayerVsComputer;
        result = TRUE;
    } else {
        gameType = GameType_PlayerVsPlayer;
    }
    gameLogic->gameType = gameType;
    return result;
}

void GameLogic_startGame(tsGameLogic *gameLogic)
{
    gameLogic->status = GameStatus_playing;
    gameLogic->turn = GameTurn_Player1;
    if (gameLogic->gameType != GameType_Replay) {
        char filename[MAX_FILENAME_SIZE] = { 0 };
        char temp[5] = { 0 };
        time_t t = time(NULL);
        struct tm tm;
        (void)localtime_s(&tm, &t);
        strcpy_s(filename, MAX_FILENAME_SIZE, FILENAME_PREFIX);
        _itoa_s(tm.tm_year + 1900, temp, 5, 10);
        strcat_s(filename, MAX_FILENAME_SIZE, temp);
        _itoa_s(tm.tm_mon, temp, 5, 10);
        strcat_s(filename, MAX_FILENAME_SIZE, temp);
        _itoa_s(tm.tm_mday, temp, 5, 10);
        strcat_s(filename, MAX_FILENAME_SIZE, temp);
        _itoa_s(tm.tm_hour, temp, 5, 10);
        strcat_s(filename, MAX_FILENAME_SIZE, temp);
        _itoa_s(tm.tm_min, temp, 5, 10);
        strcat_s(filename, MAX_FILENAME_SIZE, temp);
        strcat_s(filename, MAX_FILENAME_SIZE, ".txt");
        gameLogic->file_handler = FileMgmt_open(NULL, filename, strlen(filename), FileMgmt_Write);
        if (gameLogic->file_handler == NULL) {
            gameLogic->status = GameStatus_invalid;
            printf("Erreur lors de la creation du fichier de jeu\n");
        }
    }
}

void GameLogic_endGame(tsGameLogic* gameLogic)
{
    char stepString[MAX_STEP_STRING_SIZE] = { 0 };
    if (gameLogic->gameType != GameType_Replay) {
        printf("Joueur %s a gagner la partie!!\n", PLAYERS_NAMES[gameLogic->turn]);
        strcpy_s(stepString, MAX_STEP_STRING_SIZE, "Winner:");
        strcat_s(stepString, MAX_STEP_STRING_SIZE, PLAYERS_NAMES[gameLogic->turn]);
        FileMgmt_write(gameLogic->file_handler, stepString, strlen(stepString));
    }
    if (gameLogic->file_handler != NULL) {
        FileMgmt_close(gameLogic->file_handler);
    }
}

void GameLogic_step(tsGameLogic* gameLogic)
{
    if (gameLogic->gameType == GameType_Replay) {
        GameLogic__stepReplay(gameLogic);
    } else {
        GameLogic__stepPlay(gameLogic);
    }
}

bool GameLogic_isFinished(tsGameLogic *gameLogic)
{
    uint32_t counter = 0;
    bool result = FALSE;
    tsTargetGrid* target = NULL;

    if (gameLogic->turn == GameTurn_Player1) {
        target = gameLogic->player1_target;
    } else {
        target = gameLogic->computer_target;
    }

    for (uint32_t y = 0; y < GRID_SIDE_SIZE; y++) {
        for (uint32_t x = 0; x < GRID_SIDE_SIZE; x++) {
            if (target->points[y * GRID_SIDE_SIZE + x].status == Status_hit) {
                counter++;
            }
        }
    }
    if (counter == SHOTS_NEEDED_TO_WIN) {
        result = TRUE;
    }
    return result;
}

/******************************
 * Static Functions
 ******************************/
void GameLogic__stepReplay(tsGameLogic* gameLogic)
{
    tsPosition position = { 0 };
    bool hitResult = FALSE;
    bool isTurnString = FALSE;
    tenBoatType boat = Boat_empty;
    tenBoatDirection direction = Direction_invalid;

    if (gameLogic && gameLogic->file_handler) {
        char buffer[160] = { 0 };
        uint32_t result = Utils_getLine(gameLogic->file_handler, buffer, 160);
        if (result == 0) {
            gameLogic->status = GameStatus_finished;
        } else {
            Utils_stTokenizer* step_tokenizer = Utils_tokenize(buffer, strlen(buffer), ';');
            if (step_tokenizer == NULL) return;

            if (step_tokenizer->size == 1)
            {

                if (strcmp(step_tokenizer->tokens[0]->data, "StartGame") == 0)
                {
                    printf("Démarrage du jeu\n");
                }
            }
            else if (step_tokenizer->size == 2)
            {

                gameLogic->turn = GameLogic__getPlayersTurn(step_tokenizer->tokens[1]->data);
            }
            else
            {
                isTurnString = GameLogic__isTurnStep(step_tokenizer->tokens[0]->data);

                if (isTurnString) {

                    gameLogic->turn = GameLogic__getPlayersTurn(step_tokenizer->tokens[0]->data);
                    position = GameLogic__getShotPosition(step_tokenizer->tokens[1]->data);
                    hitResult = GameLogic__getHit(step_tokenizer->tokens[2]->data);

                    printf("%s Cible avant le tire:\n", PLAYERS_NAMES[gameLogic->turn]);

                    if (gameLogic->turn == GameTurn_Player1)
                    {
                        Grid_printTarget(gameLogic->player1_target);
                        Grid_assignShotInOcean(gameLogic->computer_ocean, position, &boat);
                        if (hitResult == TRUE)
                        {
                            Grid_assignStatusInTarget(gameLogic->player1_target, position, Status_hit, boat);
                        }
                        else
                        {
                            Grid_assignStatusInTarget(gameLogic->player1_target, position, Status_noHit, boat);
                        }
                        printf("%s Cible après le tire:\n", PLAYERS_NAMES[gameLogic->turn]);
                        Grid_printTarget(gameLogic->player1_target);
                    }
                    else
                    {
                        Grid_printTarget(gameLogic->computer_target);
                        Grid_assignShotInOcean(gameLogic->player1_ocean, position, &boat);
                        if (hitResult == TRUE)
                        {
                            Grid_assignStatusInTarget(gameLogic->computer_target, position, Status_hit, boat);
                        }
                        else
                        {
                            Grid_assignStatusInTarget(gameLogic->computer_target, position, Status_noHit, boat);
                        }
                        printf("%s Cible après le tire:\n", PLAYERS_NAMES[gameLogic->turn]);
                        Grid_printTarget(gameLogic->computer_target);
                    }
                    if (hitResult)
                    {
                        printf("*****************\n");
                        printf("***** H I T *****\n");
                        printf("*****************\n");
                    }
                    else
                    {
                        printf("***********************\n");
                        printf("***** N O   H I T *****\n");
                        printf("***********************\n");
                    }
                } else {
                    boat = GameLogic__getBoat(step_tokenizer->tokens[0]->data);
                    position = GameLogic__getShotPosition(step_tokenizer->tokens[1]->data);
                    direction = GameLogic__getDirection(step_tokenizer->tokens[2]->data);
                    printf("Ocean %s avant le placement du bateau:\n", PLAYERS_NAMES[gameLogic->turn]);
                    if (gameLogic->turn == GameTurn_Player1) {
                        Grid_printOcean(gameLogic->player1_ocean);
                        (void) Grid_assignBoatInOcean(gameLogic->player1_ocean, position, boat, direction);
                        printf("Ocean %s après le placement du bateau:\n", PLAYERS_NAMES[gameLogic->turn]);
                        Grid_printOcean(gameLogic->player1_ocean);
                    } else {
                        Grid_printOcean(gameLogic->player1_ocean);
                        (void) Grid_assignBoatInOcean(gameLogic->computer_ocean, position, boat, direction);
                        printf("Ocean %s après le placement du bateau:\n", PLAYERS_NAMES[gameLogic->turn]);
                        Grid_printOcean(gameLogic->player1_ocean);
                    }
                }
            }
            Utils_cleanTokenizer(step_tokenizer);

        }
    }
}

void GameLogic__stepPlay(tsGameLogic *gameLogic)
{
    static bool startGame = TRUE;
    tsPosition position = { 0 };
    tenBoatType boat = Boat_empty;
    bool hitResult = FALSE;
    bool isFinished = FALSE;
    tsTargetGrid* target = NULL;
    tsOceanGrid * ocean = NULL;
    char stepString[MAX_STEP_STRING_SIZE] = { 0 };
    char temp[20] = { 0 };

    /* signale le debut de la partie*/
    if (startGame == TRUE) {
        startGame = FALSE;
        strcpy_s(stepString, MAX_STEP_STRING_SIZE, "StartGame;");
        FileMgmt_write(gameLogic->file_handler, stepString, strlen(stepString));
        memset(stepString, 0, strlen(stepString));
    }


    switch(gameLogic->turn)
    {
        case GameTurn_Player1:
            printf("\ntour Joueure 1\n");
            Sleep(1000);
            target = gameLogic->player1_target;
            ocean = gameLogic->computer_ocean;
            GameUtil_askNextMove(target, &position);
            break;
        case GameTurn_Player2:
            printf("\ntour Joueur 2\n");
            Sleep(1000);
            target = gameLogic->computer_target;
            ocean = gameLogic->player1_ocean;
            GameUtil_askNextMove(target, &position);
            break;
        case GameTurn_Computer:
            printf("tour de l'ordinateur \n");
            printf("reflechie...\n");
            Sleep(1000);
            target = gameLogic->computer_target;
            ocean = gameLogic->player1_ocean;
            GameUtil_automaticNextMove(target, &position);
            break;
        default:

            break;
    }

    strcpy_s(stepString, MAX_STEP_STRING_SIZE, "Turn:");
    strcat_s(stepString, MAX_STEP_STRING_SIZE, PLAYERS_NAMES[gameLogic->turn]);
    strcat_s(stepString, MAX_STEP_STRING_SIZE, ";");
    strcat_s(stepString, MAX_STEP_STRING_SIZE, "ShotOn:");
    GameUtil_positionToString(&position, temp, 20);
    strcat_s(stepString, MAX_STEP_STRING_SIZE, temp);
    strcat_s(stepString, MAX_STEP_STRING_SIZE, ";");


    hitResult = Grid_assignShotInOcean(ocean, position, &boat);
    if (TRUE == hitResult) {
        Grid_assignStatusInTarget(target, position, Status_hit, boat);
        printf("*****************\n");
        printf("***** H I T *****\n");
        printf("*****************\n");
        strcat_s(stepString, MAX_STEP_STRING_SIZE, "Result:HIT;\n");
    } else {
        Grid_assignStatusInTarget(target, position, Status_noHit, boat);
        printf("***********************\n");
        printf("***** N O   H I T *****\n");
        printf("***********************\n");
        strcat_s(stepString, MAX_STEP_STRING_SIZE, "Result:NO_HIT;\n");
    }

    FileMgmt_write(gameLogic->file_handler, stepString, strlen(stepString));

    isFinished = GameLogic_isFinished(gameLogic);


    if (isFinished == TRUE) {
        gameLogic->status = GameStatus_finished;
    } else {

        if (gameLogic->gameType == GameType_PlayerVsComputer) {
            if (gameLogic->turn == GameTurn_Computer) {
                gameLogic->turn = GameTurn_Player1;
            } else {
                gameLogic->turn = GameTurn_Computer;
            }
        } else {
            if (gameLogic->turn == GameTurn_Player2) {
                gameLogic->turn = GameTurn_Player1;
            } else {
                gameLogic->turn = GameTurn_Player2;
            }
        }
    }
    Sleep(1000);
}

tenGameTurn GameLogic__getPlayersTurn(char* step_string)
{
    tenGameTurn result = GameTurn_Computer;
    Utils_stTokenizer* tokenizer = Utils_tokenize(step_string, strlen(step_string), ':');
    if (tokenizer) {
        if (tokenizer->size == 2) {
            for (uint32_t i = 0; i < 3; i++) {
                if (strcmp(tokenizer->tokens[1]->data, PLAYERS_NAMES[i]) == 0) {
                    result = (tenGameTurn)i;
                    break;
                }
            }
        }
        Utils_cleanTokenizer(tokenizer);
    }

    return result;
}

tsPosition GameLogic__getShotPosition(char* step_string)
{
    tsPosition position = { 0 };
    Utils_stTokenizer* tokenizer = Utils_tokenize(step_string, strlen(step_string), ':');
    char number[2] = { 0 };
    if (tokenizer) {
        number[0] = tokenizer->tokens[2]->data[0];
        position.horizontal = atoi(number);
        number[0] = tokenizer->tokens[3]->data[0];
        position.vertical = atoi(number);
        Utils_cleanTokenizer(tokenizer);
    }
    return position;
}

bool GameLogic__getHit(char* step_string)
{
    bool result = FALSE;
    Utils_stTokenizer* tokenizer = Utils_tokenize(step_string, strlen(step_string), ':');

    if (tokenizer) {
        if (strcmp("HIT", tokenizer->tokens[1]->data) == 0) {
            result = TRUE;
        }
        Utils_cleanTokenizer(tokenizer);
    }

    return result;
}

tenBoatType GameLogic__getBoat(char* step_string)
{
    tenBoatType result = Boat_empty;
    Utils_stTokenizer* tokenizer = Utils_tokenize(step_string, strlen(step_string), ':');

    if (tokenizer) {
        if (strcmp(tokenizer->tokens[1]->data, "Carrier") == 0) {
            result = Boat_carrier;
        } else if (strcmp(tokenizer->tokens[1]->data, "Battleship") == 0) {
            result = Boat_battleship;
        } else if (strcmp(tokenizer->tokens[1]->data, "Cruiser") == 0) {
            result = Boat_cruiser;
        } else if (strcmp(tokenizer->tokens[1]->data, "Submarine") == 0) {
            result = Boat_submarine;
        } else if (strcmp(tokenizer->tokens[1]->data, "Destroyer") == 0) {
            result = Boat_destroyer;
        }
        Utils_cleanTokenizer(tokenizer);
    }
    return result;
}

tenBoatDirection GameLogic__getDirection(char* step_string)
{
    tenBoatDirection direction = Direction_invalid;
    Utils_stTokenizer* tokenizer = Utils_tokenize(step_string, strlen(step_string), ':');

    if (tokenizer) {
        if (strcmp(tokenizer->tokens[1]->data, "LeftToRight") == 0) {
            direction = Direction_left_right;
        } else if (strcmp(tokenizer->tokens[1]->data, "RightToLeft") == 0) {
            direction = Direction_right_left;
        } else if (strcmp(tokenizer->tokens[1]->data, "UpDown") == 0) {
            direction = Direction_up_down;
        } else if (strcmp(tokenizer->tokens[1]->data, "DownUp") == 0) {
            direction = Direction_down_up;
        }
        Utils_cleanTokenizer(tokenizer);
    }
    return direction;
}

bool GameLogic__isTurnStep(char* step_string)
{
    bool result = FALSE;
    Utils_stTokenizer* tokenizer = Utils_tokenize(step_string, strlen(step_string), ':');

    if (tokenizer) {
        if (strcmp(tokenizer->tokens[0]->data, "Turn") == 0) {
            result = TRUE;
        }
        Utils_cleanTokenizer(tokenizer);
    }

    return result;
}
