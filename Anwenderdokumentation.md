# Tic-Tac-Toe Anwenderdokumentation

## Getting started
Wenn man die .exe ausführt begegnet man einem Auswahl-Menu:<br>
1: Create a new player<br>
2: Show the High-Score List<br>
3: Show player data<br>
4: Play<br>
5: Edit Player name<br>
6: Delete player by name<br>
0: Exit<br>

## Auswahlmenü:
Nach dem Erscheinen des Menüs muss man eine Zahl von 0 - 6 eingeben.
### 1: Create a new player
Die 1. erstellt einen neuen Nutzer. Erst muss man seinen Namen eingeben und den danach bestätigen.<br>
Wenn es schon einen Nutzer mit dem gleichen Namen gibt, wird man aufgefordert den Namen neu einzugeben.<br>
Nach der Eingabe wird man zurück auf das Auswahlmenü geleitet.
### 2: Show the High-Score List
Die 2. zeigt eine High-Score Liste von allen Spielern mit:<br>
Rank            | Name                      | Score     | Games played<br><br>
Mit der Entertaste wird man wieder ins Auswahlmenü geleitet.
### 3: Show player data
Die 3. fordert den Nutzer auf einen Spielernamen einzugeben. Nach der Eingaben werden alle Attribute des gesuchten Spieler angezeigt.<br>
Mit der Entertaste wird man wieder ins Auswahlmenü geleitet.
### 4: Play
Die 4. startet das Spiel Tic-Tac-Toe. Man wird aufgefordert 2 Spielernamen einzugeben, welche gegeneinander antreten. Falls ein Spielername in der Datenbank nicht exsistiert wird gefragt, ob ein neuer Nutzer mit dem eingegebenen Namen erstellt werden soll.<br>
Dann wird das Feld angezeigt und eine Eingabe von Spieler X gefordert. Die Eingabe hat folgendes Format: X-Koordinate Y-Koordinate. Dann wechselt der Spieler. Das geht dann so lange bis einer 3 in einer Reihe hat oder alle Felder voll sind.<br>
Am Ende wird der Score des Gewinners um 1 erhört und der Score des Verlierers um 1 verringert.
### 5: Edit Player name
Mit der 5. kann man einen Spieler mit dem Namen auswählen und dann wird man aufgefordert einen neuen Namen einzugeben. Dann wird nur der Name umgeändert der Score bleibt unverändert.
### 6: Delete player by Name
Mit der 6. kann man einen Spieler mit dem Namen auswählen und dann mit einer Bestätigung Löschen.
### 0: Exit
Mit Exit werden alle Dateien abgespeichert und das Programm wird geschlossen.