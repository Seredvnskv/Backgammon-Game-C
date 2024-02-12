#include <iostream>
#include "conio2.h"
#include <math.h>
#include <cstdio>
#include <time.h>
using namespace std;

#define RED_PLAYER '@'
#define WHITE_PLAYER '$'
#define EMPTY_SPACE ' '
#define FIELDS 24
#define INDEX_0 0
#define INDEX_5 5
#define INDEX_7 7
#define INDEX_11 11
#define INDEX_12 12
#define INDEX_16 16
#define INDEX_18 18
#define INDEX_23 23

struct Fields {
    int positionX;
    int positionY;
    int numberOfpawns;
    char player; // @ -> RedPlayer  $ -> WhitePlayer
};

struct Roll {
    int firstMove;
    int secondMove;
    int remainMoves;
    bool isDouble;
    bool isFirstUsed;
    bool isSecondUsed;
    bool isUsedForBearOff;
};

struct Bar {
    int whitePlayer;
    int redPlayer;
};

struct Outdoor {
    int whitePawns;
    int redPawns;
    bool whiteScored;
    bool redScored;
};

struct Score {
    int whitePlayerScore;
    int redPlayerScore;
};

void printFull(Fields startingpositions[FIELDS], Bar& bar, Outdoor& out);
void printPawns(const Fields& Checkers, int fieldIndex);
void printUpperPawns(Fields startingpositions[FIELDS]);
void printUpperPawns_(Fields startingpositions[FIELDS]);
void printLowerPawns(Fields startingpositions[FIELDS]);
void printLowerPawns_(Fields startingpositions[FIELDS]);
void initPawnsOnBoard(Fields startingpositions[FIELDS]);
int userInput(Fields startingpositions[FIELDS], char& turn, Bar& bar, Outdoor& out);
void makePlayerMove(Fields& position1, Fields& position2, char turn, Bar& bar);
bool isOnBar(Bar& bar, char turn);
bool isIndexOut(Roll& roll, int index1, int index2, char turn);
bool isMoveOut(Fields& position1, Fields& position2, char turn);
void nextPlayer(char& turn);
char startingPlayer();
Roll diceRoll(char& turn);
char startingPlayer();
void movePlayer(Fields startingpositions[FIELDS], char& turn, Bar& bar, Outdoor& out);
void saveGame(Fields fields[], Bar bar, char turn, Outdoor& out);
bool loadGame(Fields fields[], Bar& bar, char& turn, Outdoor& out);
void removeFromBar(Bar& bar, char turn);
bool isBarOut(Fields& position2, char turn);
void makeMoveFromBar(Fields& position2, char turn, Bar& bar);
void moveFromBar(Fields startingpositions[FIELDS], Bar& bar, char& turn, Roll& roll);
int selectDice(Roll& roll);
int calcMoveId(int diceChoice, Roll& roll, char turn);
bool validMove(Fields& position, char turn, int& moveId);
void executeMove(Fields& position, char turn, Bar& bar, Roll& roll, int diceChoice);
void checkIsMoveValid(Fields startingpositions[FIELDS], char& turn, Bar& bar, Roll& roll);
bool canMoveFromBar(Fields startingpositions[FIELDS], Bar& bar, Roll& roll, char turn);
bool canBearOff(Fields startingpositions[FIELDS], char turn);
void bearOffPawns(Fields startingpositions[FIELDS], char& turn, Bar& bar, Outdoor& out, Roll& roll);
void updateScoresAndCheckForWin(Bar& bar, Outdoor& out, Score& score);

void printUpperlevel() {
    int numbers[12] = { 13,14,15,16,17,18,19,20,21,22,23,24 };
    for (int i = 0; i < 6; i++) {
        gotoxy(6 + (i * 6), 2); // wspolrzedne
        cout << " " << numbers[i];
    }
    for (int i = 6; i < 12; i++) {
        gotoxy(12 + (i * 6), 2); // wspolrzedne
        cout << " " << numbers[i];
    }
    for (int i = 0; i < 6; i++) {
        for (int k = 0; k < 5; k++) {
            int x = 6 + (i * 6);
            int y = 3 + k;
            gotoxy(x, y); // wspolrzedne
            cout << "<";
            gotoxy(x + 1, y);
            cout << " ";
            gotoxy(x + 2, y);
            cout << ">";
        }
    }
    for (int i = 6; i < 12; i++) {
        for (int k = 0; k < 5; k++) {
            int x = 13 + (i * 6);
            int y = 3 + k;
            gotoxy(x, y);  // wspolrzedne
            cout << "<";
            gotoxy(x + 1, y); 
            cout << " ";
            gotoxy(x + 2, y);
            cout << ">";
        }
    }
}

void printMenu() {
    gotoxy(88, 10); // wspolrzedne 
    cout << "Sambor Seredynski s198035" << endl;
    gotoxy(88, 11);
    cout << "M) Move" << endl;
    gotoxy(88, 12);
    cout << "Q) Quit" << endl;
    gotoxy(88, 13);
    cout << "S) Save Game" << endl;
    gotoxy(88, 14);
    cout << "L) Load Game" << endl;
}

void printBarHome(Bar& bar, Outdoor& out) {
    for (int i = 0; i < 13; i++) {
        int x = 43;
        int y = 3;
        gotoxy(x, y + i);
        cout << "::";
    }
    gotoxy(88, 2);
    cout << "BAR " << "@ " << "[" << bar.redPlayer << "]" << ", $ " << "[" << bar.whitePlayer << "]";
    gotoxy(88, 4);
    cout << "OUTDOOR " << "@ " << "[" << out.redPawns << "]" << ", $ " << "[" << out.whitePawns << "]";
}

void printLowerlevel() {
    int numbers[12] = { 12,11,10,9,8,7,6,5,4,3,2,1 };
    for (int i = 0; i < 6; i++) {
        for (int k = 0; k < 5; k++) {
            int x = 6 + (i * 6);
            int y = 11 + k;
            gotoxy(x, y); // wspolrzedne
            cout << "<";
            gotoxy(x + 1, y);
            cout << " ";
            gotoxy(x + 2, y);
            cout << ">";
        }
    }
    for (int i = 6; i < 12; i++) {
        for (int k = 0; k < 5; k++) {
            int x = 13 + (i * 6);
            int y = 11 + k;
            gotoxy(x, y); // wspolrzedne
            cout << "<";
            gotoxy(x + 1, y);
            cout << " ";
            gotoxy(x + 2, y);
            cout << ">";
        }
    }
    for (int i = 0; i < 6; i++) {
        gotoxy(7 + (i * 6), 16); // wspolrzedne
        cout << numbers[i];
    }
    for (int i = 6; i < 12; i++) {
        gotoxy(13 + (i * 6), 16); // wspolrzedne
        cout << " " << numbers[i];
    }
}

void printPawns(const Fields& Checkers, int fieldIndex) {
    int maxVisiblePawns = 5; // Maksymalna liczba widocznych pionków

    for (int i = 0; i < Checkers.numberOfpawns && i < maxVisiblePawns; i++) {
        int x = Checkers.positionX;
        int y = Checkers.positionY;
        if (fieldIndex >= 12 && fieldIndex <= 23) {
            y += i;
        }
        else {
            y -= i;
        }
        gotoxy(x, y);
        cout << Checkers.player;
    }

    // jezeli jest wiecej pionkow niz maxVisiblePawns, wyswetla ich liczbe
    if (Checkers.numberOfpawns > maxVisiblePawns) {
        int x = Checkers.positionX;
        int y = Checkers.positionY + ((fieldIndex >= 12 && fieldIndex <= 23) ? maxVisiblePawns : -maxVisiblePawns);
        gotoxy(x, y);
        cout << Checkers.numberOfpawns;
    }
}

void printUpperPawns(Fields startingpositions[FIELDS]) {
    for (int i = 12; i <= 17; i++) {
        int x = 7 + (6 * (i - 12)); // wspolrzedne x pionka
        int y = 3; // wspolrzedne 1 pionka liczac od gory 

        startingpositions[i].positionX = x;
        startingpositions[i].positionY = y;
        // Ustawienie szczegolowych wartosci dla konkretnego indeksu pola itd...
        if (i == INDEX_12) {
            startingpositions[i].numberOfpawns = 5;
            startingpositions[i].player = RED_PLAYER;
        }
        else if (i == INDEX_16) {
            startingpositions[i].numberOfpawns = 3;
            startingpositions[i].player = WHITE_PLAYER;
        }
        else {
            startingpositions[i].numberOfpawns = 0;
            startingpositions[i].player = EMPTY_SPACE;
        }
    }
}

void printUpperPawns_(Fields startingpositions[FIELDS]) {

    for (int i = 18; i <= 23; i++) {
        int x = 14 + (6 * (i - 12));
        int y = 3;

        startingpositions[i].positionX = x;
        startingpositions[i].positionY = y;
        // Ustawienie szczegolowych wartosci dla konkretnego indeksu pola itd...
        if (i == INDEX_18) {
            startingpositions[i].numberOfpawns = 5;
            startingpositions[i].player = WHITE_PLAYER;
        }
        else if (i == INDEX_23) {
            startingpositions[i].numberOfpawns = 2;
            startingpositions[i].player = RED_PLAYER;
        }
        else {
            startingpositions[i].numberOfpawns = 0;
            startingpositions[i].player = EMPTY_SPACE;
        }
    }
}

void printLowerPawns(Fields startingpositions[FIELDS]) {

    for (int i = 11; i > 5; i--) {

        int x = 7 + (6 * (11 - i));
        int y = 15;
        startingpositions[i].positionX = x;
        startingpositions[i].positionY = y;
        // Ustawienie szczegolowych wartosci dla konkretnego indeksu pola itd...
        if (i == INDEX_11) {
            startingpositions[i].numberOfpawns = 5;
            startingpositions[i].player = WHITE_PLAYER;
        }
        else if (i == INDEX_7) {
            startingpositions[i].numberOfpawns = 3;
            startingpositions[i].player = RED_PLAYER;
        }
        else {
            startingpositions[i].numberOfpawns = 0;
            startingpositions[i].player = EMPTY_SPACE;
        }

    }
}

void printLowerPawns_(Fields startingpositions[FIELDS]) {
    for (int i = 5; i >= 0; i--) {

        int x = 50 + (6 * (5 - i));
        int y = 15;
        startingpositions[i].positionX = x;
        startingpositions[i].positionY = y;
        // Ustawienie szczegolowych wartosci dla konkretnego indeksu pola itd...
        if (i == INDEX_5) {
            startingpositions[i].numberOfpawns = 5;
            startingpositions[i].player = RED_PLAYER;
        }
        else if (i == INDEX_0) {
            startingpositions[i].numberOfpawns = 2;
            startingpositions[i].player = WHITE_PLAYER;
        }
        else {
            startingpositions[i].numberOfpawns = 0;
            startingpositions[i].player = EMPTY_SPACE;
        }
    }
}

void initPawnsOnBoard(Fields startingpositions[FIELDS]) {
    for (int i = 0; i < FIELDS; i++) { // zerujemy plansze na start
        startingpositions[i].positionX = 0;
        startingpositions[i].positionY = 0;
        startingpositions[i].numberOfpawns = 0;
    }
    printUpperPawns(startingpositions);
    printUpperPawns_(startingpositions);
    printLowerPawns(startingpositions);
    printLowerPawns_(startingpositions);
}

void gameLoop() {
    char turn = startingPlayer();
    Fields startingpositions[FIELDS];
    initPawnsOnBoard(startingpositions);
    Bar bar;
    bar.redPlayer = 0;
    bar.whitePlayer = 0;

    Outdoor out;
    out.redPawns = 0;
    out.whitePawns = 0;

    Score score = { 0, 0 };

    while (1) {
        printFull(startingpositions, bar, out);
        int result = userInput(startingpositions, turn, bar, out);
        if (result == -1) {
            break;
        }

        gotoxy(1, 1);
        clrscr();

        updateScoresAndCheckForWin(bar, out, score);

        if (out.whitePawns == 15 || out.redPawns == 15) {
            cout << "Koniec gry! Wynik: Bialy - " << score.whitePlayerScore << ", Czerwony - " << score.redPlayerScore << endl;
            break;
        }
    }
}

int userInput(Fields startingpositions[FIELDS], char& turn, Bar& bar, Outdoor& out) {
    char c; // wybor
    gotoxy(88, 16);
    cout << "Wybierz: " << endl;
    gotoxy(88, 17);
    cout << "M-move, Q-quit, L-load, S-save" << endl;
    gotoxy(88, 18);
    c = getch(); // odczyt wyboru gracza

    switch (c) {
    case 'M':
    case 'm':
        movePlayer(startingpositions, turn, bar, out);
        nextPlayer(turn);
        break;
    case 'S':
    case 's':
        saveGame(startingpositions, bar, turn, out);
        break;
    case 'L':
    case 'l':
        if (!loadGame(startingpositions, bar, turn, out)) {
        }
        break;
    case 'Q':
    case 'q':
        return -1; // zakoncz
    default:
        cout << "Wrong" << endl;
        break;
    }

    return 0;
}

void printFull(Fields startingpositions[FIELDS], Bar& bar, Outdoor& out) {
    printMenu();
    printBarHome(bar, out);
    printUpperlevel();
    printLowerlevel();
    for (int i = 0; i < FIELDS; i++) {
        printPawns(startingpositions[i], i);
    }
}

char startingPlayer() {
    int red, white;
    do {
        cout << "Losowanie poczatkowego gracza: " << endl;
        getch();

        red = rand() % 6 + 1; // rzut dla gracza czerwonego
        white = rand() % 6 + 1; // rzut dla gracza bialego

        cout << "Gracz czerwony: " << RED_PLAYER << " wyrzucil: " << red << endl;
        cout << "Gracz bialy: " << WHITE_PLAYER << " wyrzucil: " << white << endl;

        if (red > white) {
            cout << "Zaczyna gracz: " << RED_PLAYER << endl;
            getch();
            clrscr();
            return RED_PLAYER;
        }
        else if (white > red) {
            cout << "Zaczyna gracz: " << WHITE_PLAYER << endl;
            getch();
            clrscr();
            return WHITE_PLAYER;
        }
        else {
            cout << "Remis. Rzucamy ponownie." << endl;
        }
    } while (red == white);
}

void nextPlayer(char& turn) {
    if (turn == RED_PLAYER) {
        turn = WHITE_PLAYER;
    }
    else {
        turn = RED_PLAYER;
    }
}

void movePlayer(Fields startingpositions[FIELDS], char& turn, Bar& bar, Outdoor& out) {
    gotoxy(1, 17);
    Roll roll = diceRoll(turn);

    if (isOnBar(bar, turn)) {
        if (canMoveFromBar(startingpositions, bar, roll, turn)) {
            moveFromBar(startingpositions, bar, turn, roll);
        }
        else {
            cout << endl;
            cout << "Nie ma mozliwosci ruchu, tracisz ture." << endl;
            getch();
        }
    }
    else if (canBearOff(startingpositions, turn)) {
        cout << endl;
        cout << "Mozesz wyjsc pionkiem na dwor: " << endl;
        getch();
        bearOffPawns(startingpositions, turn, bar, out, roll);
    }
    else {
        checkIsMoveValid(startingpositions, turn, bar, roll);
    }
}

int selectDice(Roll& roll) { // Wybor kostki do ruchu z bandy
    int diceChoice;
    while (true) {
        cout << endl;
        cout << "Wybierz kostke (1 lub 2), 0 aby zakonczyc: ";
        cin >> diceChoice;

        if (diceChoice == 0 || ((diceChoice == 1 || diceChoice == 2) &&
            !((diceChoice == 1 && roll.isFirstUsed) || (diceChoice == 2 && roll.isSecondUsed)))) {
            return diceChoice;
        }

        cout << "Niepoprawny wybor. Sprobuj jeszcze raz." << endl;
    }
}

int calcMoveId(int diceChoice, Roll& roll, char turn) { // Obliczenie indeksu ruchu
    int moveId = 0;
    if (turn == WHITE_PLAYER) {
        moveId = (diceChoice == 1) ? roll.firstMove - 1 : roll.secondMove - 1;
    }
    else if (turn == RED_PLAYER) {
        moveId = (diceChoice == 1) ? 24 - roll.firstMove : 24 - roll.secondMove;
    }
    return moveId;
}

bool validMove(Fields& position, char turn, int& moveId) { 
    return isBarOut(position, turn);
}

void executeMove(Fields& position, char turn, Bar& bar, Roll& roll, int diceChoice) { // Wykonanie ruchu
    makeMoveFromBar(position, turn, bar);

    removeFromBar(bar, turn);
    if (roll.isDouble) {
        roll.remainMoves -= roll.firstMove;
    }
    else {
        roll.remainMoves -= (diceChoice == 1 ? roll.firstMove : roll.secondMove);
    }

    if (diceChoice == 1) roll.isFirstUsed = true;
    if (diceChoice == 2) roll.isSecondUsed = true;
}

void moveFromBar(Fields startingpositions[FIELDS], Bar& bar, char& turn, Roll& roll) { // Ruch z bandy
    while (roll.remainMoves > 0 && ((turn == WHITE_PLAYER && bar.whitePlayer > 0) || (turn == RED_PLAYER && bar.redPlayer > 0))) {
        int c = selectDice(roll); // wybierz kostke

        if (c == 0) {
            break;
        }

        int id = calcMoveId(c, roll, turn); // index

        if (validMove(startingpositions[id], turn, id)) {
            executeMove(startingpositions[id], turn, bar, roll, c);
        }
        else {
            cout << "Nielegalny ruch z bandy. ";
            if (!canMoveFromBar(startingpositions, bar, roll, turn)) {
                cout << "Nie ma mozliwosci ruchu, tracisz ture." << endl;
                getch();
                break;
            }
            else {
                cout << "Sprobuj jeszcze raz." << endl;
                getch();
            }
        }
    }
}

bool isOnBar(Bar& bar, char turn) {
    if (turn == WHITE_PLAYER) {
        if (bar.whitePlayer > 0) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (bar.redPlayer > 0) {
            return true;
        }
        else {
            return false;
        }
    }
}

// czy ruch jest mozliwy
bool isBarOut(Fields& position2, char turn) {
    return position2.numberOfpawns <= 1 || position2.player == turn;
}

//ruch z bandy
void makeMoveFromBar(Fields& position2, char turn, Bar& bar) {
    // czy na pozycji docelowej znajduje sie pojedynczy pionek przeciwnika
    if (position2.numberOfpawns == 1 && position2.player != turn && position2.player != EMPTY_SPACE) {
        // Dodaj pionek przeciwnika do baru
        if (position2.player == RED_PLAYER) {
            bar.redPlayer++;
        }
        else {
            bar.whitePlayer++;
        }

        // usun pionek przeciwnika
        position2.numberOfpawns = 0;
        position2.player = EMPTY_SPACE;
    }

    // umiesc pionek gracza na pole
    position2.numberOfpawns++;
    position2.player = turn;
}

//usuniecie z bandy
void removeFromBar(Bar& bar, char turn) {
    if (turn == WHITE_PLAYER) {
        bar.whitePlayer--;
    }
    else if (turn == RED_PLAYER) {
        bar.redPlayer--;
    }
}

bool canMoveFromBar(Fields startingpositions[FIELDS], Bar& bar, Roll& roll, char turn) {
    if (turn == WHITE_PLAYER && bar.whitePlayer > 0) {
        // Sprawdzanie dla gracza bialego
        if ((isBarOut(startingpositions[roll.firstMove - 1], turn) ||
            isBarOut(startingpositions[roll.secondMove - 1], turn))) {
            return true;
        }
    }
    else if (turn == RED_PLAYER && bar.redPlayer > 0) {
        // Sprawdzanie dla gracza czerwonego
        if ((isBarOut(startingpositions[FIELDS - roll.firstMove], turn) ||
            isBarOut(startingpositions[FIELDS - roll.secondMove], turn))) {
            return true;
        }
    }
    return false;
}

bool isMoveOut(Fields& position1, Fields& position2, char turn) {
    if (position1.numberOfpawns > 0 && position1.player == turn) {
        if (position2.numberOfpawns > 1 && position2.player != turn) {
            return false;
        }
        else if (position2.numberOfpawns == 1 && position2.player != turn) {
            return true;
        }
        else {
            return true;
        }
    }
    else {
        return false;
    }
}

bool isIndexOut(Roll& roll, int index1, int index2, char turn) {
    int x = abs(index1 - index2); // dystans ruchu
    bool mD; // movedirection -> kierunek ruchu
    if (index1 >= 0 && index1 <= 24 && index2 >= 0 && index2 <= 24) {
        if (turn == RED_PLAYER) {
            mD = index2 < index1; // ruch przeciwnie do zegara

        }
        else {
            mD = index2 > index1; //  ruch zgodnie do zegara
        }
        if (roll.isDouble) {
            if (mD && roll.remainMoves >= x && x % roll.firstMove == 0) {
                roll.remainMoves -= x;
                return true;
            }
        }
        else {
            if (mD && (x == roll.firstMove || x == roll.secondMove || x == (roll.firstMove + roll.secondMove))) {
                if (roll.remainMoves == (roll.firstMove + roll.secondMove)) { // wiadomo ze pierwszy ruch
                    roll.remainMoves -= x;
                    return true;
                }
                else if (roll.remainMoves == x) {
                    roll.remainMoves -= x;
                    return true;
                }
            }
        }

        return false;
    }
    return false;
}

void checkIsMoveValid(Fields startingpositions[FIELDS], char& turn, Bar& bar, Roll& roll) {
    int i1; // indeks pola 1
    int i2; // indeks pola 2 
    do {
        cout << endl;
        cout << "Podaj numer pola, z ktorego chesz sie ruszyc: ";
        cin >> i1;
        cout << i1 << endl;
        i1--;

        cout << "Podaj numer pola na ktore chesz sie ruszyc: ";
        cin >> i2;
        cout << i2 << endl;
        i2--;

        Roll cpyroll = roll; //nie modyfikujemy orginalu

        if (!isIndexOut(cpyroll, i1, i2, turn)) {
            cout << "Zly index: ";
            continue;
        }
        if (!isMoveOut(startingpositions[i1], startingpositions[i2], turn)) {
            cout << "Niepoprawne pola: ";
            continue;
        }
        makePlayerMove(startingpositions[i1], startingpositions[i2], turn, bar);
        roll = cpyroll;
    } while (roll.remainMoves > 0);
}

Roll diceRoll(char& turn) {
    cout << "Rzut kostka gracza " << turn << " : ";
    getch();
    int firstRoll = rand() % 6 + 1;
    int secondRoll = rand() % 6 + 1;

    Roll roll;
    roll.firstMove = firstRoll;
    roll.secondMove = secondRoll;
    roll.isDouble = (firstRoll == secondRoll);

    roll.isFirstUsed = false;
    roll.isSecondUsed = false;

    if (roll.isDouble == false) {
        cout << "Pierwszy Rzut: " << firstRoll << " Drugi Rzut: " << secondRoll;
        roll.remainMoves = roll.firstMove + roll.secondMove;
    }
    else {
        cout << "Dublet 4 ruchy o wartosci: " << firstRoll; // gdy dublet first == second
        roll.remainMoves = 4 * roll.firstMove;
    }

    return roll;
}

void makePlayerMove(Fields& position1, Fields& position2, char turn, Bar& bar) {
    position1.numberOfpawns--;

    if (position2.player != turn && position2.numberOfpawns == 1) {
        if (turn == WHITE_PLAYER) {
            bar.redPlayer++;

        }
        else {
            bar.whitePlayer++;
        }
        position2.numberOfpawns--;
    }
    position2.numberOfpawns++;

    position2.player = turn;
};

void saveGame(Fields fields[], Bar bar, char turn, Outdoor& out) {
    FILE* file;
    fopen_s(&file, "GameSave.txt", "w");

    if (file == NULL) {
        cout << "Nie mozna otworzyc pliku do zapisu!" << endl;
        return;
    }

    // Zapisz stan na polach
    for (int i = 0; i < FIELDS; i++) {
        fwrite(&fields[i], sizeof(Fields), 1, file);
    }

    // zapisz bar
    fwrite(&bar, sizeof(Bar), 1, file);

    //zapisz outdoor
    fwrite(&out, sizeof(Outdoor), 1, file);

    // zapisz gracza
    fwrite(&turn, sizeof(char), 1, file);

    fclose(file);
}

bool loadGame(Fields fields[], Bar& bar, char& turn, Outdoor& out) {
    FILE* file;
    fopen_s(&file, "GameSave.txt", "r");

    if (file == NULL) {
        cout << "Nie mozna otworzyc pliku do odczytu!" << endl;
        return false;
    }

    // Wczytaj stan na polach
    for (int i = 0; i < FIELDS; i++) {
        fread(&fields[i], sizeof(Fields), 1, file);
    }

    // wczytaj bar
    fread(&bar, sizeof(Bar), 1, file);

    // wczytaj outdoor
    fread(&out, sizeof(Outdoor), 1, file);

    // wczytaj gracza
    fread(&turn, sizeof(char), 1, file);

    fclose(file);
    return true;
}

bool canBearOff(Fields startingpositions[FIELDS], char turn) {
    int index1 = (turn == WHITE_PLAYER) ? 18 : 0;
    int index2 = (turn == WHITE_PLAYER) ? 23 : 5;

    for (int i = 0; i < FIELDS; i++) {
        if (i < index1 || i > index2) {
            if (startingpositions[i].player == turn && startingpositions[i].numberOfpawns > 0) {
                return false;
            }
        }
    }
    return true;
}

// pionki na dwor
int selectFieldForBearOff() { // wybor pola
    int field;
    cout << "Z ktorego pola chcesz zdjac pionek? (podaj numer pola, 0 aby pominac): ";
    cin >> field;
    return field;
}

bool isValidBearOff(int field, Fields startingpositions[], char turn, Roll& roll) {
    if (field == 0) {
        return false; // pomin ruch
    }

    field--;
    if (field >= 0 && field < FIELDS && startingpositions[field].player == turn && startingpositions[field].numberOfpawns > 0) {
        int moveValue = (turn == WHITE_PLAYER) ? FIELDS - field : field + 1;
        if ((roll.isDouble && moveValue == roll.firstMove && roll.remainMoves >= roll.firstMove && !roll.isUsedForBearOff) ||
            (!roll.isDouble && ((moveValue == roll.firstMove && !roll.isFirstUsed) || (moveValue == roll.secondMove && !roll.isSecondUsed)))) {
            return true;
        }
    }
    return false;
}

void performBearOff(Fields& position, char& turn, Outdoor& out, Roll& roll, int moveValue) { // zdjecie pionka z planszy
    position.numberOfpawns--;
    if (turn == WHITE_PLAYER) {
        out.whitePawns++;
    }
    else {
        out.redPawns++;
    }
    cout << "Pionek zdjety z pola: " << moveValue << endl;

    if (roll.isDouble) {
        roll.remainMoves -= roll.firstMove;
        roll.isUsedForBearOff = true;
    }
    else {
        roll.remainMoves -= moveValue;
        if (moveValue == roll.firstMove) {
            roll.isFirstUsed = true;
        }
        else {
            roll.isSecondUsed = true;
        }
    }
}

void bearOffPawns(Fields startingpositions[FIELDS], char& turn, Bar& bar, Outdoor& out, Roll& roll) {
    while (roll.remainMoves > 0) {
        int field = selectFieldForBearOff();
        if (field == 0) {
            cout << "Ruch pominiety." << endl;
            break; // Zakoncz, gdy gracz zdecyduje sie pominac ruch
        }
        else if (isValidBearOff(field, startingpositions, turn, roll)) {
            performBearOff(startingpositions[field - 1], turn, out, roll, field);
        }
        else {
            cout << "Nieprawidlowe pole! Sprobuj jeszcze raz." << endl;
        }
    }
}

void updateScoresAndCheckForWin(Bar& bar, Outdoor& out, Score& score) {
    if (out.whitePawns == 15) { // Gracz bialy wygral
        score.whitePlayerScore++; // Podstawowy punkt za wygrana
        if (out.redPawns == 0) {
            score.whitePlayerScore++; // Dodatkowy punkt za dwor
        }
        if (bar.redPlayer > 0) {
            score.whitePlayerScore += 2; // Dodatkowe 2 punkty (razem 3)
        }
    }
    else if (out.redPawns == 15) { // Gracz czerwony wygral
        score.redPlayerScore++; 
        if (out.whitePawns == 0) {
            score.redPlayerScore++; 
        }
        if (bar.whitePlayer > 0) {
            score.redPlayerScore += 2; 
        }
    }
}

int main() {
    textcolor(LIGHTCYAN);
    srand(time(NULL));
    gameLoop();
    return 0;
}