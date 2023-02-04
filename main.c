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
int init();
int sortieren();
int auswahl();
int nameEingabe();
int highScoreList();
int getPlayerInformation();
int resetDisplayBoard();
int getPlayerIndexByName(char name[]);
void addNewPlayer(char name[]);
int close();


int main() {
    init();
    char yn = 1;
    char createNameInput; // to save y or n from user input
    while (yn !=110)
    {
        auswahl();

        // get player names
        char playerX[NAME_LEN];
        int playerXIndex = PLAYER_NAME_NOT_FOUND;

        char playerO[NAME_LEN];
        int playerOIndex = PLAYER_NAME_NOT_FOUND;

        printf("Enter the name of the player X: ");
        scanf("%s", playerX);
        playerXIndex = getPlayerIndexByName(playerX);

        if(playerXIndex == PLAYER_NAME_NOT_FOUND){
            printf("\na player with the name %s was not found \na new player with the name %s will be created, do you want to proceed? (y/n): ",playerX, playerX);
            scanf(" %c", &createNameInput);

            if(createNameInput == 121 || createNameInput == 89) {// if y or Y
                addNewPlayer(playerX); // create the player
            }
            else{
                continue; // skip one iteration of the while loop
            }
        }
        printf("Enter the name of the player O: ");
        scanf("%s", playerO);
        playerOIndex = getPlayerIndexByName(playerO);

        if(playerOIndex == PLAYER_NAME_NOT_FOUND){
            printf("\na player with the name %s was not found\na new player with the name %s will be created, do you want to proceed? (y/n): ",playerO, playerO);
            scanf(" %c", &createNameInput);

            if(createNameInput == 121 || createNameInput == 89) {// if y or Y
                addNewPlayer(playerO); // create the player
            }
            else{
                continue; // skip one iteration of the while loop
            }
        }

        int x, y;
        char turn = 'X';

        displayBoard();
        for (int i = 0; i < 9; i++) {
            printf("Player %c turn. Enter row and column (x y): ", turn);
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

            char checkwin = checkWin();
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
int init()
{
    FILE *file = fopen(DATABASE_FILEPATH, "r");

    char line[MAX_LINE_LENGTH];
    spielerAnzahl = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        for (int i = 0; i < MAX_LINE_LENGTH; i++) {
            if(line[i] != 59)
            {
                spielerListe[spielerAnzahl].name[i] = line[i];
            } else
            {
                int hscore = 0;
                int n = 0;
                i++;
                if(line[i] == 45)
                {
                    i++;
                    n = 1;
                }
                for (int j = 0; j < 10; ++j) {


                    if(line[i] != 59)
                    {
                        hscore *= 10;
                        hscore += (int)(line[i]) - 48;
                    } else
                    {
                        if (n == 1)
                            hscore *= -1;

                        spielerListe[spielerAnzahl].score = hscore;
                        break;

                    }
                    i++;
                }
                int hplatzierung = 0;
                for (int j = 0; j < 10; ++j) {
                    i++;
                    if(line[i] != 59)
                    {
                        hplatzierung *= 10;
                        hplatzierung += (int)(line[i]) - 48;
                    } else
                    {
                        spielerListe[spielerAnzahl].platzierung = hplatzierung;
                        break;
                    }
                }
                int hanzahlDerSpiele = 0;
                for (int j = 0; j < 10; ++j) {
                    i++;
                    if(line[i] != 59)
                    {
                        hanzahlDerSpiele *= 10;
                        hanzahlDerSpiele += (int)(line[i]) - 48;
                    } else
                    {
                        spielerListe[spielerAnzahl].anzahlDerSpiele = hanzahlDerSpiele;
                        i++;
                        break;
                    }
                }
                break;
            }
        }
        spielerAnzahl++;
    }
    fclose(file);
    return 0;
}

/*
 * Sortiert die spielerListe nach Score
 */
int sortieren()
{
    int t = 1;
    char tname[NAME_LEN];
    int tscore, tplatzierung, tanzahlDerSpiele;
    while(t != 0) // solange die sortierung nicht stimmt, also solange es ein score[i] gibt der größer ist als score[i+1]
    {
        t = 0;
        for (int i = 0; i < spielerAnzahl-1; i++) {  // alle Spieler durchiterieren
            if(spielerListe[i].score < spielerListe[i+1].score) // wenn score von i größer ist als der von i+1
            {
                t = 1;
                strcpy(tname, spielerListe[i].name); // zwischenspeicherung der Attribute
                tscore = spielerListe[i].score;
                tplatzierung = spielerListe[i].platzierung;
                tanzahlDerSpiele = spielerListe[i].anzahlDerSpiele;
                strcpy(spielerListe[i].name, spielerListe[i+1].name); // tauschen der Attribute von i und i+1
                spielerListe[i].score = spielerListe[i+1].score;
                spielerListe[i].platzierung = spielerListe[i+1].platzierung;
                spielerListe[i].anzahlDerSpiele = spielerListe[i+1].anzahlDerSpiele;
                strcpy(spielerListe[i+1].name, tname);
                spielerListe[i+1].score = tscore;
                spielerListe[i+1].platzierung = tplatzierung;
                spielerListe[i+1].anzahlDerSpiele = tanzahlDerSpiele;
            }
        }
    }

    for (int i = 0; i < spielerAnzahl; ++i) { // platzierung in Array umschreiben
        spielerListe[i].platzierung = i+1;
    }

    return 0;
}

/*
 * Anzeigen des Spiel-Menus
 */
int auswahl()
{
    int cas = 5;
    while (cas != 0)
    {
        system("cls");
        printf("1: Neuen Spieler erstellen\n2: High-Score Liste erstellen\n3: Ausgabe der Daten eines Nutzers via Namen\n4: Spielen\n0: exit\n");
        scanf("%d", &cas);
        switch(cas) {
            case 1: printf("Neuen Nutzer anlegen:\n"); nameEingabe(); break;
            case 2: highScoreList(); break;
            case 3: printf("Gebe den zu suchenden Namen ein: "); char nameSearch[NAME_LEN]; scanf("%s", &nameSearch[0]); getPlayerInformation(nameSearch); break;
            case 4: resetDisplayBoard(); return 0;
            case 5: printf("Geben Sie bitte eine Zahl ein (0-4)"); break;
            case 0:
                // Daten speichern before vor dem Beenden von Programm
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
    while (yn != 121)
    {
        printf("Gebe den Namen ein: ");
        scanf("%s", &name);
        for (int i = 0; i < NAME_LEN; i++)
        {
            if (name[i] == 59)
            {
                name[i] = 44;
            }
        }
        printf("Stimmt die Eingabe %s (y/n)?", name);
        scanf(" %c", &yn);
    }

    addNewPlayer(name);

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
        printf("Spieler \"%s\" wurde schon registstriert.\n", spielerListe[i].name);
        printf("Drücke eine Taste, um fortzufahren...\n");
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
    printf("Platzierung\t| Name                      | Score\t| Anzahl der Spiele\n");
    for (int i = 0; i < spielerAnzahl; i++)
    {
        printf("%d.\t\t| %s", spielerListe[i].platzierung, spielerListe[i].name);
        for (int j = 0; j < 25 - strlen(spielerListe[i].name); j++) // Abstände gleich machen
        {
            printf(" ");
        }
        printf(" | %d\t", spielerListe[i].score);

        if (spielerListe[i].score < 10 && spielerListe[i].score >= 0)
            printf("\t");

        printf("| %d\n", spielerListe[i].anzahlDerSpiele);
    }
    printf("Drücke eine Taste, um fortzufahren...\n");
    fflush ( stdin );
    getchar();
    return 0;
}

/*
 * Sucht nach einem Spieler und gibt dessen Attribute aus
 * @return index des gesuchten Spielers
 */
int getPlayerInformation(char name[NAME_LEN])
{
    int i = getPlayerIndexByName(name);

    printf("Der gesuchte name hat den Index: %d\n\n", i);
    printf("Name:              %s\n", spielerListe[i].name);
    printf("Platzierung:       %d\n", spielerListe[i].platzierung);
    printf("Score:             %d\n", spielerListe[i].score);
    printf("Anzahl der Spiele: %d\n", spielerListe[i].anzahlDerSpiele);
    printf("\n");
    printf("Drücke eine Taste, um fortzufahren...\n");
    fflush ( stdin );
    getchar();
    return i;

    return 0;
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
 * Bei aufruf dieser FUnktion wird überprüft ob Spieler X oder Spieler O gewonnen hat
 */
char checkWin()
{
    char winchecker[4];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            winchecker[j] = board[j][i];
        }
        if (strcmp(board[i], "XXX") == 0 || strcmp(winchecker, "XXX") == 0)
        {
            return 88;
        }
        if (strcmp(board[i], "OOO") == 0 || strcmp(winchecker, "OOO") == 0)
        {
            return 79;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        winchecker[i] = board[i][i];
    }
    if (strcmp(winchecker, "XXX") == 0)
    {
        return 88;
    }
    if (strcmp(winchecker, "OOO") == 0)
    {
        return 79;
    }
    int j = 2;
    for (int i = 0; i < 3; i++)
    {
        winchecker[i] = board[i][j];
        j--;
    }
    if (strcmp(winchecker, "XXX") == 0)
    {
        return 88;
    }
    if (strcmp(winchecker, "OOO") == 0)
    {
        return 79;
    }
    return 78;
}

int getPlayerIndexByName(char name[NAME_LEN]){
    for (int i = 0; i < spielerAnzahl; i++)
    {
        if (strncasecmp(spielerListe[i].name, name, NAME_LEN) == 0)
        {
            return i;
        }
    }
    return PLAYER_NAME_NOT_FOUND;
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
        printf("Fehler beim öffenen der Datenbank!");
        return 0;
    }

    // Die Variable save_format zum speichern der formattierte String
    char save_format[64];
    for (int i = 0; i < spielerAnzahl; i++) {

        // alle daten formattieren und in die Variable save_format speichern
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
