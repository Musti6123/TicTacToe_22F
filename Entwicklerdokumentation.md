# Tic-Tac-Toe Entwicklerdokumentation
### Beschreibung:
Das beliebte und einfache Spiel "Tic-Tac-Toe" als command line Spiel implementiert, mithilfe der Programmiersprache C. Außerdem gibt es
die Möglichkeit Spielernamen, und deren Score, Rang und Spieleanzahl in einer datenbank (TXT-Datei) abzuspeichern.
---

### Code Funktionen:
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
---
### Beschreibung einzelne Funktionen

    void displayBoard();
* Gibt das Spielfeld aus
* X in rot
* O in blau
---
    char checkWin();
* Bei aufruf dieser Funktion wird überprüft ob Spieler X oder Spieler O gewonnen hat
---
    void init();
* Initialisierung des Spiels
* Spieler werden aus Datenbank geladen und in spielerListe geschrieben
---
    int sortieren();
* Sortiert die spielerListe nach Score (Bubblesort)
---
    int auswahl();
* Anzeigen des Spiel-Menus
* Benutzer verschiedene Optionen auswählen lassen 

---
    int nameEingabe();
* Funktion legt neuen Nutzer im Array an
* Erhöht spielerAnzahl um 1
* Alle Atribute des Spielers werden auf 0 gesetzt
---
    int highScoreList();
* Gibt die Highscore Liste aus aller Spieler Sortiert nach Score
---
    void getPlayerInformation(char name[]);
* Sucht nach einem Spieler mit Name und gibt dessen Attribute aus

---
    int resetDisplayBoard();
* Resettet das board array (Spielfeld)
* Setzt alle Werte von board auf " "
---
    int getPlayerIndexByName(char name[]);
* Sucht nach einem Spieler mit Name
* @return index des gesuchten Spielers
---
    void addNewPlayer(char name[]);
* addNewPlayer adds a new player to the array
---
    int delete();
* Eingabe von Spielername
* aufruf von getPlayerIndexByName Funktion
* Wenn die Index gefunden wird, wird deleteByIndex Funktion aufgerufen um den Spieler zu löchen
---
    int deleteByIndex(int index);
* Löcht die Daten von dem Spieler mit dem eingegebenen Index
---
    edit();
* Änderung des Spielernames mit Behaltung von Spielerscore 
---
    int close();
* Speichert der spielerListe in einer text Datei
* Format: name;score;anzahlDerSpiele;platzierung
---
