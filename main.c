#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define AC_BLUE "\033[0;34m"
#define AC_NORMAL "\033[0m"
#define AC_RED "\033[0;31m"
#define NAME_LEN 50
#define MAX_SPIELER 50
#define DATABASE_FILEPATH "database.txt"


char board[4][4] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
char rBoard[4][4] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
int spielerAnzahl = 0;

typedef struct
{
    char name[NAME_LEN];
    int score;
    int plazierung;
    int anzahlDerSpiele;
} TSPIELER;

TSPIELER spielerListe[MAX_SPIELER];


void displayBoard();
char checkWin();
int init();
int sortieren();
int auswahl();
int eingabe();
int highScoreList();
int sucheName();
int resetDisplayBoard();
int writeData();


int main() {
    char yn = 1;
    while (yn !=110)
    {
        auswahl();

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
                printf("\nPlayer X won\n");
                i = 10;
            }
            if(checkwin == 79)
            {
                printf("\nPlayer O won\n");
                i = 10;
            }

        }

        printf("y: Back to menu\nn: Exit");
        scanf(" %c", &yn);
        if (yn == 110)
            return 0;
    }


    return 0;
}

int init()
{


    return 0;
}

int sortieren()
{

    return 0;
}

int auswahl()
{
    int cas = 5;
    while (cas != 0)
    {
        system("cls");
        printf("1: Neuen Spieler erstellen\n2: High-Score Liste erstellen\n3: Ausgabe der Daten eines Nutzers via Namen\n4: Spielen\n0: exit\n");
        scanf("%d", &cas);
        switch(cas) {
            case 1: printf("Neuen Nutzer anlegen:\n"); eingabe(); break;
            case 2: highScoreList(); break;
            case 3: printf("Gebe den zu suchenden Namen ein: "); char nameSearch[NAME_LEN]; scanf("%s", &nameSearch[0]); sucheName(nameSearch); break;
            case 4: resetDisplayBoard(); return 0;
            case 5: printf("Geben Sie bitte eine Zahl ein (0-4)"); break;
            case 0:
                // Daten speichern before vor dem Beenden von Programm
                writeData();
                exit(0);
        }
    }
    return 0;
}

int eingabe()
{
    char name[NAME_LEN];
    // die drei variablen sind unbenutzt!!
    int score = 0;
    int plazierung = 0;
    int anzahlDerSpiele = 0;

    char yn = 110;
    while (yn != 121)
    {
        printf("Gebe den Namen ein: ");
        scanf("%s", &name);
        printf("Stimmt die Eingabe %s (y/n)?", name);
        scanf(" %c", &yn);
    }

    spielerAnzahl++;
    strcpy(spielerListe[spielerAnzahl - 1].name, name);
    spielerListe[spielerAnzahl - 1].score = 0;
    spielerListe[spielerAnzahl - 1].plazierung = spielerAnzahl;
    spielerListe[spielerAnzahl - 1].anzahlDerSpiele = 0;


    return 0;
}

int highScoreList()
{
    printf("Platzierung   |   Name   |   Score   |   Anzahl der Spiele");
    for (int i = 0; i < spielerAnzahl; i++)
    {
        printf("%d.   |   %s   |   %d   |   %d", spielerListe[i].plazierung, spielerListe[i].name, spielerListe[i].score, spielerListe[i].anzahlDerSpiele);
    }
    return 0;
}

int sucheName(char name[NAME_LEN])
{
    for (int i = 0; i < spielerAnzahl; i++)
    {
        if (strncmp(spielerListe[i].name, name, NAME_LEN) == 0)
        {
            printf("Der gesuchte name hat den Index: %d\n\n", i);
            printf("Name:              %s\n", spielerListe[i].name);
            printf("Platzierung:       %d\n", spielerListe[i].plazierung);
            printf("Score:             %d\n", spielerListe[i].score);
            printf("Anzahl der Spiele: %d\n", spielerListe[i].anzahlDerSpiele);
            return i;
        }
    }

    return 0;
}

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

int resetDisplayBoard()
{
    memcpy(board, rBoard, sizeof rBoard);

    return 0;
}

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

int writeData(){

    FILE *database;
    // Datei in write mode öffnen
    database = fopen(DATABASE_FILEPATH, "w");

    if(database == NULL){
        printf("Fehler beim öffenen der Datenbank!");
        return 0;
    }

    // Die Variable save_format zum speichern der formattierte String
    char save_format[64];
    for (int i = 0; i < spielerAnzahl; ++i) {

        // alle daten formattieren und in die Variable save_format speichern
        snprintf(save_format, sizeof(save_format), "%s\n%i\n%i\n%i\n",
                 spielerListe[i].name,
                 spielerListe[i].score,
                 spielerListe[i].anzahlDerSpiele,
                 spielerListe[i].plazierung
                 );

        // save_format in datenbank speichern
        fputs(save_format,database);
    }

    fclose(database);

    return 1;
}