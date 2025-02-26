/****************
 * Yehonatan Hadar
****************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 10
#define MAX_PLAYER_NAME_LENGTH 20
#define MAX_OBJECT_NAME_LENGTH 7
#define THERE_IS_TIE 2
#define INVALID 1
#define THERE_IS_WINNER 1
#define STILL_NO_WINNER 0

void printBoardStart(int row, int column);
void printObjectsList(char *objectsArray[], int boardSize);
void fillPlayersNames(char* namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH],char* playersNames);
void insertObjectsPositions(int boardSize, int positionsArray[(boardSize*boardSize)/2][2], char* argv[]);

void gameProcess(int *turnIndex, int boardSize, char* namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH],
                 int positionArray[boardSize*boardSize][2], int positionMatchArray1[],int positionMatchArray2[],
                 int numOfPlayers, int playersScore[numOfPlayers], char* gameBoard[boardSize][boardSize],
                 char* objectsNamesArray[(boardSize*boardSize)/2][MAX_OBJECT_NAME_LENGTH]);

void fillObjectsNames(int boardSize, char *objectsArray[],
                      char* objectsNamesArray[(boardSize*boardSize)/2][MAX_OBJECT_NAME_LENGTH]);

void printDynamicGameBoard(int boardSize, char* gameBoard[boardSize][boardSize]);
void switchGameBoardToX(int boardSize, char* gameBoard[boardSize][boardSize]);
int checkWinner(int boardSize, int numOfPlayers, int playersScore[numOfPlayers]);
int maxScore(int numOfPlayers, int playersScore[numOfPlayers]);
int checkPositionsInput(int boardSize, char *argv[]);
int checkNumOfPlayers(char *namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH],int numOfPlayers, char* playersNames);
int checkIfMatch(int boardSize, int positionMatchArray1[],int positionMatchArray2[],int mergedcor1,int mergedcor2);
void initializationMatchArray(int boardSize, int positionMatchArray1[],int positionMatchArray2[]);

void checkEndGame (int boardSize, int numOfPlayers, int playersScore[numOfPlayers],
                   char *namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH]);

int checkInvalidInput(int argc, int boardSize, char *argv[], char *namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH],
                      int numOfPlayers, char* playersNames);

int main(int argc, char *argv[])
{
    //Check if argv array is without any parameters
    if(argv[1] == (void *)0)
    {
        printf("INVALID INPUT");
        return 0;
    }

    // Board size
    int boardDim = atoi(argv[1]);
    //The number of players
    int numOfPlayers = atoi(argv[2]);
    //Array with the score of every player
    int playersScore[numOfPlayers];
    //Index that tracking after the turns in the game in accordance to the matches in the game
    int turnIndex=0;
    //The string of names from the arguments array (argv)
    char *playersNames = argv[3];
    //The array that containing the players's names
    char *namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH];
    //The array that containing the objects's names
    char *objectsNamesArray[(boardDim * boardDim)/2][MAX_OBJECT_NAME_LENGTH];
    //The board of the game (updated after every match)
    char *gameBoard[boardDim][boardDim];
    //The array that containing the objects's positions on the board
    int positionsArray[(boardDim * boardDim)/2][2];
    //The array that contains the positions where there were matches (with the first pair of coordinates)
    int positionsMatchArray1[(boardDim * boardDim)/2];
    //The array that contains the positions where there were matches (with the second pair of coordinates)
    int positionsMatchArray2[(boardDim * boardDim)/2];

    //Check if there is invalid input from the arguments
    if((checkInvalidInput(argc, boardDim, argv, namesArray, numOfPlayers, playersNames))==INVALID)
    {
        printf("INVALID INPUT");
        return 0;
    }

    for (int i = 0; i < numOfPlayers; i++)
    {
    playersScore[i] = 0;
    }

    initializationMatchArray(boardDim,positionsMatchArray1,
                             positionsMatchArray2);

    fillPlayersNames(namesArray, playersNames);
    insertObjectsPositions(boardDim, positionsArray, argv + (4 + ((boardDim * boardDim)/2)));
    fillObjectsNames(boardDim, argv + 4, objectsNamesArray);
    switchGameBoardToX(boardDim, gameBoard);

    printf("Welcome to the Memory Card Game!\nThe game will be played on the following board:\n\n");
    printBoardStart(boardDim, boardDim);

    printf("\nAnd the following objects:\n\n");
    printObjectsList(argv, boardDim);

    printf("\nLet the game begin!\n");

    while (((checkWinner(boardDim, numOfPlayers, playersScore)) == STILL_NO_WINNER))
    {
        gameProcess(&turnIndex,boardDim, namesArray, positionsArray,
                    positionsMatchArray1,positionsMatchArray2, numOfPlayers,
                    playersScore, gameBoard, objectsNamesArray);
    }

    checkEndGame (boardDim, numOfPlayers, playersScore, namesArray);
}

//This function print who is the winner in the game or there is a tie
void checkEndGame (int boardSize, int numOfPlayers, int playersScore[numOfPlayers],
                   char *namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH])
{
    if ((checkWinner(boardSize, numOfPlayers, playersScore)) == 1)
    {
        for (int i = 0; i < numOfPlayers; i++)
        {
            for (int j = 0; j < 1; j++)
            {
                if (playersScore[i] == ((boardSize * boardSize) / 2))
                {
                    printf("\nCongratulations %s! You won!", namesArray[i][j]);
                    break;
                }
            }
        }
    }
    else
    {
        if (((checkWinner(boardSize, numOfPlayers, playersScore)) == 2))
            printf("\nIt's a tie!");
        else
            printf("\nCongratulations %s! You won!", namesArray[maxScore(numOfPlayers, playersScore)][0]);
    }
}

//This function checks if there is invalid input in the arguments that the program receive
int checkInvalidInput(int argc, int boardSize, char *argv[], char *namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH],
                      int numOfPlayers, char* playersNames)
{
    if((argc<10)||(argc!=(((boardSize*boardSize)/2)+(boardSize*boardSize)+4)))
        return INVALID;

    if((checkPositionsInput(boardSize, argv + (4 + ((boardSize * boardSize)/2))))==1)
        return INVALID;

    else
    {
        if((argc-(4 + (boardSize * boardSize)))!=((boardSize * boardSize)/2))
            return INVALID;
    }

    if((checkNumOfPlayers(namesArray, numOfPlayers, playersNames))==1)
        return INVALID;

    return 0;
}

//This function checks if there is the exact amount of players's names according to the number defined
int checkNumOfPlayers(char *namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH],int numOfPlayers, char* playersNames)
{
    char *name= strtok(playersNames,"#");
    int index=0,count=0;
    //This whileLoop (and if after it) checks if there is the required amount of names
    while(name!=NULL)
    {
        namesArray[index][0]=name;
        name= strtok(NULL, "#");
        count++;
        index++;
    }

    if(count!=numOfPlayers)
        return INVALID;

    return 0;
}

//This function checks if the object's positions in the arguments array are invalid
int checkPositionsInput(int boardSize, char *argv[])
{
    int checkPos,countObjects=0,index=0;
    char *ptr;

    //This forLoop checks if there are positions with the same value
    for (int i = 0; i < ((boardSize*boardSize)-1) ; i++)
    {
        checkPos = (atoi(argv[i]));
        for (int j = 1; j < (boardSize*boardSize); j++)
        {
            if(j<=i)
                continue;
            if(checkPos == (atoi(argv[j])))
                return INVALID;
        }
    }

    //This forLoop checks if there is position with an invalid value
    for (int t = 0; t < (boardSize*boardSize); t++)
    {
        if((atoi(argv[t]))>((boardSize*boardSize)-1))
            return INVALID;
        strtol(argv[t], &ptr, 10);
        if(ptr==0)
            return INVALID;
        if((atoi(argv[t]))<0)
            return INVALID;
    }

    //This whileLoop (and the if after it) checks if there is the required amount of positions
    while(argv[index]!=(void *)0)
    {
        countObjects++;
        index++;
    }

    if(countObjects!=(boardSize*boardSize))
        return INVALID;

    //If everything correct
    return 0;
}

//This function checks what is the highest score in the game at the moment
int maxScore(int numOfPlayers, int playersScore[numOfPlayers])
{
    int maxSco;
    for (int i = 0; i < numOfPlayers; i++)
    {
        if(playersScore[i]>maxSco)
            maxSco=i;
    }
    return maxSco;
}

//This function checks if there is a winner in the game or there is a tie
int checkWinner(int boardSize, int numOfPlayers, int playersScore[numOfPlayers])
{
    int tieCheck=0;
    int sumScore=0;
    for (int i = 0; i < numOfPlayers; i++)
    {
        if(playersScore[i]==((boardSize*boardSize)/2))
            return THERE_IS_WINNER;
        else if(playersScore[i]==((boardSize*boardSize)/4))
            tieCheck++;
        if(tieCheck==2)
            return THERE_IS_TIE;

        sumScore=sumScore+playersScore[i];
    }

    if(sumScore==((boardSize*boardSize)/2))
        return THERE_IS_TIE;

    return STILL_NO_WINNER;
}

//This function print example of the game board to the players before the start of the game
void printBoardStart(int row, int column)
{
    printf("|");
    for (int j = 0; j < column; j++)
    {
        printf("  %d  |", j);
    }
    printf("\n");
    for (int k = 0; k < ((5 * column) + column + 3); k++)
        printf("-");
    printf("\n");
    for (int i = 0; i < row; i++)
    {
        printf("%d |", i);
        for (int j = 0; j < column; j++)
            printf("(%d,%d)|", i,j);
        printf("\n");
        for (int k = 0; k < ((5 * column) + column + 3); k++)
            printf("-");
        printf("\n");
    }
}

//This function print list of the object's names in the beginning of the game
void printObjectsList(char *objectsArray[], int boardSize)
{
    int numIndex=1,indexObj=4;
    for (int i = 0; i < ((boardSize*boardSize)/2); i++)
    {
        printf("%d. ", numIndex);
        printf("%s\n", objectsArray[indexObj]);
        numIndex++;
        indexObj++;
    }
}

//This function insert the object's names into array
void fillObjectsNames(int boardSize, char *objectsArray[],
                      char* objectsNamesArray[(boardSize*boardSize)/2][MAX_OBJECT_NAME_LENGTH])
{
    for (int i = 0; i < ((boardSize*boardSize)/2); i++)
    {
        for (int j = 0; j < 1; ++j)
            objectsNamesArray[i][j] = objectsArray[i];
    }
}

//This function prints the updated game board
void printDynamicGameBoard(int boardSize, char* gameBoard[boardSize][boardSize])
{
    int printCheck=0;

    for (int k = 0; k < ((15 * boardSize) + boardSize + 1 ); k++)
        printf("-");
    printf("\n|");

    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize ; j++)
        {
            //Responsible to print the object's name according to the length of his name
            switch (strlen(gameBoard[i][j]))
            {
                case 1:
                    printf("       %s       |", gameBoard[i][j]);
                    break;
                case 2:
                    printf("       %s      |", gameBoard[i][j]);
                    break;
                case 3:
                    printf("      %s      |", gameBoard[i][j]);
                    break;
                case 4:
                    printf("      %s     |", gameBoard[i][j]);
                    break;
                case 5:
                    printf("     %s     |", gameBoard[i][j]);
                    break;
                case 6:
                    printf("     %s    |", gameBoard[i][j]);
                    break;
                case 7:
                    printf("    %s    |", gameBoard[i][j]);
                    break;
                default:
                    break;
            }
        }
        printf("\n");
        for (int k = 0; k < ((15 * boardSize) + boardSize + 1 ); k++)
            printf("-");

        if((printCheck+1)!=boardSize)
            printf("\n|");
        printCheck++;
    }
}

//This function initialise the game board in 'X' for the beginning of the game
void switchGameBoardToX(int boardSize, char* gameBoard[boardSize][boardSize])
{
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
            gameBoard[i][j] = "X";
    }
}

//This function takes the players's names from argv and insert them into an array
void fillPlayersNames(char* namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH],char* playersNames)
{
    char *name= strtok(playersNames,"#");
    int index=0;
    while(name!=NULL)
    {
        namesArray[index][0]=name;
        name= strtok(NULL, "#");
        index++;
    }
}

/*
This function takes the objects's positions from argv and insert them into an array So that each cell in the array
contains a small array in which cell 0 contains one coordinate and cell 1 contains the 2nd coordinate of an object
*/
void insertObjectsPositions( int boardSize, int positionsArray[(boardSize*boardSize)/2][2], char* argv[])
{
    int objectsIndex=0;
    for (int i = 0; i < ((boardSize*boardSize)/2); i++)
    {
        for (int j = 0; j < 2; j++)
        {
        positionsArray[i][j] = atoi(argv[objectsIndex]);
        objectsIndex++;
        }
    }
}

//This function initialise the array of the positions that were in them matches during the game
void initializationMatchArray(int boardSize, int positionMatchArray1[],int positionMatchArray2[])
{
    for (int i = 0; i < ((boardSize*boardSize)/2); i++)
    {
        positionMatchArray1[i]=-1;
        positionMatchArray2[i]=-1;
    }
}

//This function checks if the coordinates that the player entered are identical to those that had a match before
int checkIfMatch(int boardSize, int positionMatchArray1[],int positionMatchArray2[],int mergedcor1,int mergedcor2)
{
    for (int i = 0; i < ((boardSize*boardSize)/2); i++)
    {
        if((positionMatchArray1[i]==mergedcor1)||(positionMatchArray1[i]==mergedcor2))
            return 1;
        if((positionMatchArray2[i]==mergedcor1)||(positionMatchArray2[i]==mergedcor2))
            return 1;
    }
    return 0;
}

/*
This function is the biggest function in the code, and it is responsible for managing the game and
runs until there is a match or mismatch
*/
void gameProcess(int *turnIndex, int boardSize, char* namesArray[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH],
                 int positionArray[boardSize*boardSize][2],int positionMatchArray1[],int positionMatchArray2[],
                 int numOfPlayers, int playersScore[numOfPlayers],char* gameBoard[boardSize][boardSize],
                 char* objectsNamesArray[(boardSize*boardSize)/2][MAX_OBJECT_NAME_LENGTH])

{   //The coordinates that the player enter
    int cor1,cor2,cor3,cor4;
    //Count the times that player has match in his turn
    int matchCounter=0;
    //Indicate if there is a match in player's turn
    int matchStatus=0;
    //Saves the array's positions that was there match
    int saveCor1,saveCor2;

    printf("\nCurrent board state:\n");
    printDynamicGameBoard(boardSize, gameBoard);
    printf("\n\nIt is %s's turn.\n", namesArray[*turnIndex][0]);

    printf("Enter the coordinates of the first card (row col): ");
    scanf("%d%d", &cor1,&cor2);
    printf("Enter the coordinates of the second card (row col): ");
    scanf("%d%d", &cor3,&cor4);

    int mergedCor1=cor1*boardSize+cor2;
    int mergedCor2=cor3*boardSize+cor4;
    int match = checkIfMatch(boardSize, positionMatchArray1,positionMatchArray2,mergedCor1,mergedCor2);

    //Checks if the input from the player is invalid
    if(cor1>=boardSize||cor2>=boardSize||cor3>=boardSize||cor4>=boardSize||((cor1==cor3)&&(cor2==cor4))||(match==1))
    {
        printf("Invalid move. Try again.\n");
        printf("Enter the coordinates of the first card (row col): ");
        scanf("%d%d", &cor1,&cor2);
        printf("Enter the coordinates of the second card (row col): ");
        scanf("%d%d", &cor3,&cor4);

        mergedCor1=cor1*boardSize+cor2;
        mergedCor2=cor3*boardSize+cor4;
        match = checkIfMatch(boardSize, positionMatchArray1,positionMatchArray2,mergedCor1,mergedCor2);

        //This whileLoop will run as long as the user enters invalid input
        while(cor1>=boardSize||cor2>=boardSize||cor3>=boardSize||cor4>=boardSize||((cor1==cor3)&&(cor2==cor4))||
        (match==1))
        {
            printf("Invalid move. Try again.\n");

            printf("Enter the coordinates of the first card (row col): ");
            scanf("%d%d", &cor1,&cor2);
            printf("Enter the coordinates of the second card (row col): ");
            scanf("%d%d", &cor3,&cor4);

            mergedCor1=cor1*boardSize+cor2;
            mergedCor2=cor3*boardSize+cor4;
            match = checkIfMatch(boardSize, positionMatchArray1,positionMatchArray2,mergedCor1,mergedCor2);
        }

        //This forLoop checks if there is a match with object's positions in the game board
        for (int i = 0; i < ((boardSize*boardSize)/2); i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if((positionArray[i][j]==mergedCor1)||(positionArray[i][j]==mergedCor2))
                {
                    matchCounter++;
                    if(matchCounter==1)
                        saveCor1=i;
                    if(matchCounter==2)
                        saveCor2=i;
                }
            }
            if(matchCounter==2)
            {
                positionMatchArray1[i]=mergedCor1;
                positionMatchArray2[i]=mergedCor2;

                printf("Match!\n\n");
                gameBoard[cor1][cor2]=objectsNamesArray[saveCor1][0];
                gameBoard[cor3][cor4]=objectsNamesArray[saveCor2][0];
                matchStatus++;
                break;
            }
            else
                matchCounter=0;
        }
    }
    //If the player entered a valid input, checks if there is a match with object's positions in the game board
    else
    {
        for (int i = 0; i < ((boardSize*boardSize)/2); i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if((positionArray[i][j]==mergedCor1)||(positionArray[i][j]==mergedCor2))
                {
                    matchCounter++;
                    if(matchCounter==1)
                        saveCor1=i;
                    if(matchCounter==2)
                        saveCor2=i;
                }
            }
            if(matchCounter==2)
            {
                positionMatchArray1[i]=mergedCor1;
                positionMatchArray2[i]=mergedCor2;

                printf("Match!\n\n");
                gameBoard[cor1][cor2]=objectsNamesArray[saveCor1][0];
                gameBoard[cor3][cor4]=objectsNamesArray[saveCor2][0];
                matchStatus++;
                break;
            }
            else
                matchCounter=0;
        }
    }
    //If there is a match to the current player
    if(matchStatus==1)
    {
        playersScore[*turnIndex]++;
        printf("The scores are:\n");
        for (int i = 0; i < numOfPlayers ; i++)
        {
            if(i!=0)
                printf(", ");
            for (int j = 0; j < 1; j++)
            {
                printf("%s: %d",namesArray[i][j],playersScore[i]);
            }
        }
    }
    //If there is no match to the current player
    else
    {
        printf("No match. Try again.\n\n");
        //If there is no match to the last player in the list, pass the turn to the first player on the list
        if((*turnIndex+1)==numOfPlayers)
            *turnIndex=0;
        else
            *turnIndex=(*turnIndex+1);

        printf("The scores are:\n");
        for (int i = 0; i < numOfPlayers ; i++)
        {
            if(i!=0)
                printf(", ");
            for (int j = 0; j < 1; j++)
            {
                printf("%s: %d",namesArray[i][j],playersScore[i]);
            }
        }
    }
}