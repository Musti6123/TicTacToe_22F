#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define AC_BLUE "\033[0;34m"
#define AC_NORMAL "\033[0m"
#define AC_RED "\033[0;31m"
#define NAME_LEN 25
#define MAX_LINE_LENGTH 100
#define MAX_SPIELER 50
#define DATABASE_FILEPATH "database.txt"
#define PLAYER_NAME_NOT_FOUND 1000

char board[4][4] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
char rBoard[4][4] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
int spielerAnzahl = 0;
int cas = 6;

typedef struct
{
    char name[NAME_LEN];
    int score;
    int platzierung;
    int anzahlDerSpiele;
} TSPIELER;

TSPIELER spielerListe[MAX_SPIELER];


void displayBoard();
char checkWin();
void init();
int sortieren();
int auswahl();
int nameEingabe();
int highScoreList();
void getPlayerInformation(char name[]);
int resetDisplayBoard();
int getPlayerIndexByName(char name[]);
void addNewPlayer(char name[]);
int delete();
int deleteByIndex(int index);
void edit();
int close();


int main() {
    init();
    char yn = 1;
    char createNameInput; // to save y or n from user input
    while (yn !=110)
    {
        yn = 1; // reset yn! Important :)
        auswahl();

        // get player names
        char playerX[NAME_LEN];
        int playerXIndex = PLAYER_NAME_NOT_FOUND;

        char playerO[NAME_LEN];
        int playerOIndex = PLAYER_NAME_NOT_FOUND;

        printf("Enter the name of the player X: ");
        fflush(stdin);
        fgets(playerX, NAME_LEN, stdin);
        playerX[strcspn(playerX, "\n")] = 0; // removing "\n"
        playerXIndex = getPlayerIndexByName(playerX);

        if(playerXIndex == PLAYER_NAME_NOT_FOUND){
            printf("\na player with the name %s was not found \na new player with the name %s will be created, do you want to proceed? (y/n): ",playerX, playerX);
            scanf(" %c", &createNameInput);

            if(createNameInput == 121 || createNameInput == 89) {// if y or Y
                addNewPlayer(playerX); // create the player
                playerXIndex = getPlayerIndexByName(playerX); // get the index after creation
            }
            else{
                continue; // skip one iteration of the while loop
            }
        }
        printf("Enter the name of the player O: ");
        fflush(stdin);
        fgets(playerO, NAME_LEN, stdin);
        playerO[strcspn(playerO, "\n")] = 0; // removing "\n"
        playerOIndex = getPlayerIndexByName(playerO);

        if(playerOIndex == PLAYER_NAME_NOT_FOUND){
            printf("\na player with the name %s was not found\na new player with the name %s will be created, do you want to proceed? (y/n): ",playerO, playerO);
            scanf(" %c", &createNameInput);

            if(createNameInput == 121 || createNameInput == 89) {// if y or Y
                addNewPlayer(playerO); // create the player
                playerOIndex = getPlayerIndexByName(playerO); // get the index after creation
            }
            else{
                continue; // skip one iteration of the while loop
            }
        }

        int x, y;
        char turn = 'X';
        char checkwin;

        displayBoard();
        for (int i = 0; i < 9; i++) {
            printf("Player %c (%s) turn. Enter row and column (x y): ", turn, (i % 2 == 0? playerX : playerO));
            scanf("%d %d", &x, &y);
            if (board[x][y] == ' ') {
                board[x][y] = turn;

                if (turn == 'X') {
                    turn = 'O';
                } else {
                    turn = 'X';
                }
            } else {
                printf("This cell is already occupied. Try again.\n");
                i--;
            }
            displayBoard();

            checkwin = checkWin();
            if(checkwin == 88)
            {
                printf("\nPlayer X (%s) won\n", playerX);
                spielerListe[playerXIndex].score++;
                spielerListe[playerOIndex].score--;
                spielerListe[playerXIndex].anzahlDerSpiele++;
                spielerListe[playerOIndex].anzahlDerSpiele++;
                i = 10;
            }
            if(checkwin == 79)
            {
                printf("\nPlayer O (%s) won\n", playerO);
                spielerListe[playerOIndex].score++; // score anpassen
                spielerListe[playerXIndex].score--;
                spielerListe[playerXIndex].anzahlDerSpiele++; // anzahl der Spiele anpassen
                spielerListe[playerOIndex].anzahlDerSpiele++;
                i = 10;
            }
        }

        while (yn != 121)
        {
            printf("y: Back to menu\nn: Exit\n");
            scanf(" %c", &yn);
            if (yn == 110)
                close();
        }

    }

    return 0;
}

/*
 * Initialisierung des Spiels
 * Spieler werden aus Datenbank geladen und in spielerListe geschrieben
 */
void init()
{
    FILE *file = fopen(DATABASE_FILEPATH, "r");
    char line[MAX_LINE_LENGTH];
    int countSemicolon;
    _Bool negativeScore = 0; // store 1 if the Score is negative
    spielerAnzahl = 0;

    for (int i = 0; fgets(line, MAX_LINE_LENGTH, file) != NULL; ++i) {
        countSemicolon = 0; // reset counter
        // set initial values of the player to 0
        spielerListe[spielerAnzahl].score = 0;
        spielerListe[spielerAnzahl].platzierung = 0;
        spielerListe[spielerAnzahl].anzahlDerSpiele = 0;

        for(int j= 0; countSemicolon <= 3; ++j){
            if(line[j] == ';'){
                countSemicolon++;
                continue;
            }
            switch (countSemicolon) {
                case 0:
                    spielerListe[spielerAnzahl].name[j] = line[j];
                    break;
                case 1:
                    if(line[j] == 45){
                        negativeScore = 1;
                        break;
                    }
                    spielerListe[spielerAnzahl].score *= 10;
                    spielerListe[spielerAnzahl].score += (int)(line[j]) - 48;
                    break;
                case 2:
                    // doing the negative score here
                    // where the score is already calculated and
                    // to avoid running it multiple times in case 1
                    if(negativeScore){
                        spielerListe[spielerAnzahl].score *= -1; // turn score to a negative number
                        negativeScore = 0;  // reset to false
                    }
                    spielerListe[spielerAnzahl].anzahlDerSpiele *= 10;
                    spielerListe[spielerAnzahl].anzahlDerSpiele += (int)(line[j]) - 48;
                    break;
                case 3:
                    spielerListe[spielerAnzahl].platzierung *= 10;
                    spielerListe[spielerAnzahl].platzierung += (int)(line[j]) - 48;
                    break;
                default:
                    break;
            }
        }
        spielerAnzahl++;
    }
    fclose(file);
}

/*
 * Sortiert die spielerListe nach Score (Bubblesort)
 */
int sortieren()
{
    TSPIELER temp;
    int i,j,k;
    for(i = 0; i < spielerAnzahl - 1; i++){
        for(j = 0; j < spielerAnzahl - i - 1 ;j++){
            if(spielerListe[j].score < spielerListe[j+1].score){
                temp = spielerListe[j];
                spielerListe[j] = spielerListe[j+1];
                spielerListe[j+1] = temp;
            }
        }
    }
    for (k = 0; k < spielerAnzahl; k++) { // platzierung in Array umschreiben
        spielerListe[k].platzierung = k+1;
    }
    return 0;
}

/*
 * Anzeigen des Spiel-Menus
 */
int auswahl()
{
    cas = 6;
    while (cas != 0)
    {
        system("cls");
        printf("1: Create a new player\n2: Show the High-Score List\n3: Show player data\n4: Play\n5: Delete player by name\n6: Edit player name\n0: Exit\n");
        scanf("%d", &cas);
        cas = cas > 6 || cas < 0? 7 : cas; // check if input is valid -> (0-5)
        switch(cas) {
            case 1: printf("Create a new Player:\n"); nameEingabe(); break;
            case 2: highScoreList(); break;
            case 3:
                printf("Enter the player name to search for: ");
                char nameSearch[NAME_LEN];
                fflush(stdin);
                fgets(nameSearch, NAME_LEN, stdin);
                nameSearch[strcspn(nameSearch, "\n")] = 0; // removing "\n"
                getPlayerInformation(nameSearch);
                break;
            case 4: resetDisplayBoard(); return 0;
            case 5:
                delete();
                break;
            case 6:
                edit();
                break;
            case 7:
                printf("Wrong input!!\nPlease enter numbers between (0-5)\n");
                printf("Click any key to continue...\n");
                fflush ( stdin );
                getchar();
                break;
            case 0:
                // Daten speichern vor dem Beenden von Programm
                close();
                exit(0);
        }
    }
    return 0;
}

/*
 * Methode legt neuen Nutzer im Array an
 * Erhöht spielerAnzahl um 1
 * Alle Atribute des Spielers werden auf 0 gesetzt
 */
int nameEingabe()
{
    char name[NAME_LEN];

    char yn = 110;
    while (yn != 121 && yn !=101)
    {
        printf("Please enter a name: ");
        fflush(stdin);
        fgets(name, NAME_LEN, stdin);
        name[strcspn(name, "\n")] = 0; // removing "\n"
        for (int i = 0; i < NAME_LEN; i++)
        {
            if (name[i] == 59)
            {
                name[i] = 44;
            }
        }
        printf("Player name \"%s\": \n- Enter \'y\' to save the player \n- Enter \'n\' to enter the name again \n- Enter \'e\' to exit (changes will not be saved!)\n", name);
        scanf(" %c", &yn);
    }
    if(yn == 121){
        addNewPlayer(name);
    }

    return 0;
}

// addNewPlayer adds a new player without the unneeded printf and scanf
// it is the same logic but placed in a function to be used in other places
void addNewPlayer(char name[NAME_LEN])
{
    int i = getPlayerIndexByName(name);
    if (i == PLAYER_NAME_NOT_FOUND) // überprüfen ob es diesen Spielername nicht gibt
    {
        spielerAnzahl++;
        strcpy(spielerListe[spielerAnzahl - 1].name, name);
        spielerListe[spielerAnzahl - 1].score = 0;
        spielerListe[spielerAnzahl - 1].platzierung = spielerAnzahl;
        spielerListe[spielerAnzahl - 1].anzahlDerSpiele = 0;
    } else
    {
        printf("Player \"%s\" is already registered.\n", spielerListe[i].name);
        printf("Click any key to continue...\n");
        fflush ( stdin );
        getchar();
    }

}

/*
 * Gibt die Highscore Liste aus aller Spieler
 * Sortiert nach Score
 */
int highScoreList()
{
    sortieren();
    printf("Rank\t\t| Name                      | Score\t| Games played\n");
    for (int i = 0; i < spielerAnzahl; i++)
    {
        printf("%d.\t\t| %s", spielerListe[i].platzierung, spielerListe[i].name);
        for (int j = 0; j < NAME_LEN - strlen(spielerListe[i].name); j++) // Abstände gleich machen
        {
            printf(" ");
        }
        printf(" | %d\t", spielerListe[i].score);

        if (spielerListe[i].score < 10 && spielerListe[i].score >= 0)
            printf("\t");

        printf("| %d\n", spielerListe[i].anzahlDerSpiele);
    }
    printf("Click any key to continue...\n");
    fflush ( stdin );
    getchar();
    return 0;
}

/*
 * Sucht nach einem Spieler und gibt dessen Attribute aus
 * @return index des gesuchten Spielers
 */
void getPlayerInformation(char name[NAME_LEN])
{
    int i = getPlayerIndexByName(name);
    if(i == PLAYER_NAME_NOT_FOUND){
        printf("The Player with the name %s was not found!\n", name);
        printf("Click any key to continue...");
        fflush ( stdin );
        getchar();
        return;
    }
    printf("The player has the index: %d\n\n", i);
    printf("Name:              %s\n", spielerListe[i].name);
    printf("Rank:              %d\n", spielerListe[i].platzierung);
    printf("Score:             %d\n", spielerListe[i].score);
    printf("Games played:      %d\n", spielerListe[i].anzahlDerSpiele);
    printf("\n");
    if (cas != 5)
    {
        printf("Click any key to continue...\n");
        fflush(stdin);
        getchar();
    }
}

/*
 * Gibt das Spielfeld aus
 * X in rot
 * O in blau
 */
void displayBoard()
{
    system("cls");
    printf("\n");
    printf("  y 0   1   2\n");
    printf("x\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%d   ", i);
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == 88)
            {
                printf("%s%c%s", AC_RED, board[i][j], AC_NORMAL);
            } else
            {
                if (board[i][j] == 79)
                {
                    printf("%s%c%s", AC_BLUE, board[i][j], AC_NORMAL);
                } else
                {
                    printf(" ");
                }
            }

            if (j != 2)
            {
                printf(" | ");
            } else
            {
                if (i != 2)
                {
                    printf("\n");
                    printf("   ---+---+---\n");
                } else printf("\n\n");
            }
        }
    }

}

/*
 * Resettet das board array (Spielfeld)
 * Setzt alle Werte von board auf " "
 */
int resetDisplayBoard()
{
    memcpy(board, rBoard, sizeof rBoard);

    return 0;
}

/*
 * Bei aufruf dieser Funktion wird überprüft ob Spieler X oder Spieler O gewonnen hat
 */
char checkWin()
{
    // check rows and columns
    for(int i = 0; i < 3; i++)
    {
        if(board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != ' ')
        {
            return board[i][0];
        }
        if(board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] != ' ')
        {
            return board[0][i];
        }
    }
    // check diagonals
    if(board[0][0] == board[1][1] & board[0][0] == board[2][2] && board[0][0] != ' ')
    {
        return board[0][0];
    }
    if(board[0][2] == board[1][1] & board[0][2] == board[2][0] && board[0][2] != ' ')
    {
        return board[0][2];
    }

    return ' ';
}

int getPlayerIndexByName(char name[NAME_LEN])
{
    for (int i = 0; i < spielerAnzahl; i++)
    {
        if (strncasecmp(spielerListe[i].name, name, NAME_LEN) == 0)
        {
            return i;
        }
    }
    return PLAYER_NAME_NOT_FOUND;
}


int delete()
{
    printf("Please enter the name to be deleted: ");
    char deleteName[NAME_LEN];
    fflush(stdin);
    fgets(deleteName, NAME_LEN, stdin);
    deleteName[strcspn(deleteName, "\n")] = 0; // removing "\n"
    int i = getPlayerIndexByName(deleteName);
    if(i == PLAYER_NAME_NOT_FOUND){
        printf("The Player with the name %s was not found!\n", deleteName);
        printf("Click any key to continue...");
        fflush ( stdin );
        getchar();
        return 0;
    }
    printf("Here are the Information of %s:\n", deleteName);
    getPlayerInformation(deleteName);
    char yn;
    while (yn != 121 && yn != 110)
    {
        printf("\nAre you sure you want to delete this player? (y/n): ");
        fflush(stdin);
        yn = getchar();
    }
    if (yn == 121)
    {
        deleteByIndex(i);
    }
    return 0;
}


int deleteByIndex(int index)
{
    for (int i = index; i < spielerAnzahl - 1; i++)
    {
        spielerListe[i] = spielerListe[i+1];
    }
    spielerAnzahl--;
    return 0;
}

void edit(){
    printf("Please enter the name to be edited: ");
    char editName[NAME_LEN];
    char newName[NAME_LEN];
    fflush(stdin);
    fgets(editName, NAME_LEN, stdin);
    editName[strcspn(editName, "\n")] = 0; // removing "\n"
    int i = getPlayerIndexByName(editName);
    if(i == PLAYER_NAME_NOT_FOUND){
        printf("The Player with the name \"%s\" was not found!\n", editName);
        printf("Click any key to continue...");
        fflush ( stdin );
        getchar();
        return;
    }
    printf("Here are the Information of %s:\n", editName);
    getPlayerInformation(editName);
    char yn;
    while (yn != 121 && yn != 110)
    {
        printf("Please enter the new name: ");

        fflush(stdin);
        fgets(newName, NAME_LEN, stdin);
        newName[strcspn(newName, "\n")] = 0; // removing "\n"

        printf("\nAre you sure you want to change the name from \"%s\" to \"%s\"? (y/n): ", editName, newName);
        fflush(stdin);
        yn = getchar();
    }
    if (yn == 121)
    {
        strcpy(spielerListe[i].name,newName);
    }
    return;
}


/*
 * Speichern der spielerListe in einer text Datei
 * Format: name;score;anzahlDerSpiele;platzierung
*/
int close(){
    sortieren();

    FILE *database;
    // Datei in write mode öffnen
    database = fopen(DATABASE_FILEPATH, "w");

    if(database == NULL){
        printf("Error opening database!");
        return 0;
    }

    // Die Variable save_format zum speichern der formattierte String
    char save_format[100];
    for (int i = 0; i < spielerAnzahl; i++) {

        // alle daten formatieren und in die Variable save_format speichern
        snprintf(save_format, sizeof(save_format), "%s;%i;%i;%i;\n",
                 spielerListe[i].name,
                 spielerListe[i].score,
                 spielerListe[i].anzahlDerSpiele,
                 spielerListe[i].platzierung
        );

        // save_format in datenbank speichern
        fputs(save_format,database);
    }

    fclose(database);
    exit(0);
    return 1;
}
