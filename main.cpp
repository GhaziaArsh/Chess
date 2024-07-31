#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>

using namespace std;

const int BOARD_SIZE = 64;

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void sleep(int m) {
    for (int j = 0; j < m * 21000; j++)
    {
    }
}

void Print_Box(int rdim, int cdim, int row, int col, char ch)
{
    for (int r = 1; r <= rdim; r++) {
        for (int c = 1; c <= cdim; c++) {
            if (c == 1 || r == 1 || r == 5 || c == 5) {
                gotoRowCol((r - 1) + row, (c - 1) + col);
                cout << ch;
            }
        }
    }
}

void displayBoard(char chessboard[BOARD_SIZE])
{
    system("cls");
    int size = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Print_Box(5, 5, i * 5 + 1, j * 5 + 1, 219);
            char box = chessboard[size++];
            gotoRowCol(i * 5 + 3, j * 5 + 3);
            if (box == ' ') {
                cout << "- ";
            } else {
                cout << box << " ";
            }
        }
    }
}

bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, char chessboard[BOARD_SIZE], bool whiteTurn);
bool isValidPawnMove(int fromRow, int fromCol, int toRow, int toCol, char chessboard[BOARD_SIZE], bool whiteTurn);
bool isPathClear(int fromRow, int fromCol, int toRow, int toCol, char chessboard[BOARD_SIZE]);
void saveGame(char chessboard[BOARD_SIZE], bool whiteTurn);
bool loadGame(char chessboard[BOARD_SIZE], bool &whiteTurn);



bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, char chessboard[BOARD_SIZE], bool whiteTurn)
 {

    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
        return false;
    }

    char box = chessboard[fromRow * 8 + fromCol];
    char targetgoti = chessboard[toRow * 8 + toCol];


    if ((whiteTurn && islower(box)) || (!whiteTurn && isupper(box)))
    {
        return false;
    }


    if ((whiteTurn && isupper(targetgoti)) || (!whiteTurn && islower(targetgoti)))
    {
        return false;
    }

    switch (toupper(box))
    {
        case 'K':
            return (abs(toRow - fromRow) <= 1 && abs(toCol - fromCol) <= 1) && (toupper(targetgoti) != 'K');
        case 'Q':
            return ((fromRow == toRow || fromCol == toCol || abs(toRow - fromRow) == abs(toCol - fromCol)) &&
                    !((fromRow == toRow) && (fromCol == toCol))) && (toupper(targetgoti) != 'Q');
        case 'R':
            return ((fromRow == toRow || fromCol == toCol) && !((fromRow == toRow) && (fromCol == toCol))) &&
                   (toupper(targetgoti) != 'R') && isPathClear(fromRow, fromCol, toRow, toCol, chessboard);
        case 'N':
            return ((abs(toRow - fromRow) == 2 && abs(toCol - fromCol) == 1) ||
                    (abs(toRow - fromRow) == 1 && abs(toCol - fromCol) == 2)) && (toupper(targetgoti) != 'N');
        case 'B':
            return (abs(toRow - fromRow) == abs(toCol - fromCol)) && (toupper(targetgoti) != 'B') &&
                   isPathClear(fromRow, fromCol, toRow, toCol, chessboard);
        case 'P':
            return isValidPawnMove(fromRow, fromCol, toRow, toCol, chessboard, whiteTurn);
        default:
            return false;
    }
}

bool isValidPawnMove(int fromRow, int fromCol, int toRow, int toCol, char chessboard[BOARD_SIZE], bool whiteTurn)
 {
    char targetgoti = chessboard[toRow * 8 + toCol];

    if (whiteTurn) {
        // White pawn moves
        if (fromCol == toCol)
        {
            if (fromRow == 1 && toRow - fromRow <= 2 && toRow - fromRow > 0 && targetgoti == ' ')
            {
                return isPathClear(fromRow, fromCol, toRow, toCol, chessboard);
            } else if (toRow - fromRow == 1 && targetgoti == ' ')
            {
                return isPathClear(fromRow, fromCol, toRow, toCol, chessboard);
            }
        }

        // Capturing diagonally
        if (abs(toCol - fromCol) == 1 && toRow - fromRow == 1 && islower(targetgoti))
        {
            return true;
        }
    } else {
        // Black pawn moves
        if (fromCol == toCol)
            {
            if (fromRow == 6 && fromRow - toRow <= 2 && fromRow - toRow > 0 && targetgoti == ' ')
            {
                return isPathClear(fromRow, fromCol, toRow, toCol, chessboard);
            } else if (fromRow - toRow == 1 && targetgoti == ' ')
            {
                return isPathClear(fromRow, fromCol, toRow, toCol, chessboard);
            }
        }

        // Capturing diagonally
        if (abs(toCol - fromCol) == 1 && fromRow - toRow == 1 && isupper(targetgoti))
        {
            return true;
        }
    }
    return false;
}

bool isPathClear(int fromRow, int fromCol, int toRow, int toCol, char chessboard[BOARD_SIZE])
{

    int Row = (toRow - fromRow) == 0 ? 0 : (toRow - fromRow) > 0 ? 1 : -1;
    int Col = (toCol - fromCol) == 0 ? 0 : (toCol - fromCol) > 0 ? 1 : -1;

    for (int i = 1; i < max(abs(toRow - fromRow), abs(toCol - fromCol)); i++)
        {
        char squareContent = chessboard[(fromRow + i * Row) * 8 + (fromCol + i * Col)];
        if (squareContent != ' ') {
            return false;
        }
    }

    return true;
}

void saveGame(char chessboard[BOARD_SIZE], bool whiteTurn)
 {
    ofstream outFile("chess_save.txt");

    if (outFile.is_open())
        {

        outFile << (whiteTurn ? 'W' : 'B') << endl;
        for(int i=0 ; i< BOARD_SIZE ; i++)
        {
            if(i%8==0)
            {
                outFile<<endl;
            }

        if (chessboard[i]==' ')
                {
                    outFile<<'_'<<" ";
                    continue;
                }
            outFile << chessboard[i]<<" ";

        }

        outFile.close();
    }
     else
     {
        cout << "Unable to save the game.\n";
    }
}

bool loadGame(char chessboard[BOARD_SIZE], bool &whiteTurn)
 {
    ifstream inFile("chess_save.txt");

    if (inFile.is_open())
        {

        char turn;
        inFile >> turn;
        whiteTurn = (turn == 'W');

        for (int i = 0; i < BOARD_SIZE; i++)
        {
            inFile >> chessboard[i];
        }
        inFile.close();
        return true;
    }
    else
        {
        cout << "No saved game found."<<endl;
        return false;
    }
}

int main()
 {
    char chessboard[BOARD_SIZE];
    bool whiteTurn = true;


    for (int i = 0; i < BOARD_SIZE; i++)
        {
        chessboard[i] = ' ';
    }

    chessboard[4] = 'K';
    chessboard[60] = 'k';

    chessboard[3] = 'Q';
    chessboard[59] = 'q';

    chessboard[0] = 'R';
    chessboard[7] = 'R';
    chessboard[56] = 'r';
    chessboard[63] = 'r';

    chessboard[1] = 'N';
    chessboard[6] = 'N';
    chessboard[57] = 'n';
    chessboard[62] = 'n';

    chessboard[2] = 'B';
    chessboard[5] = 'B';
    chessboard[58] = 'b';
    chessboard[61] = 'b';
    for (int i = 0; i < 8; i++)
    {
        chessboard[8 + i] = 'P'; // White pawns
        chessboard[48 + i] = 'p'; // Black pawns
    }


    char choice;
    cout << "Do you want to load a saved game? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
        {

      if (!loadGame(chessboard, whiteTurn))
        {
           return 1;
        }for (int i=0; i<BOARD_SIZE ;i++)
        {
            if(chessboard[i]=='_')
            {
                chessboard[i]=' ';
            }
        }
    }

    while (true)
        {
        displayBoard(chessboard);

        int fromRow, fromCol, toRow, toCol;

            gotoRowCol(21,50);
            cout<<"enter row from where you want to move:";
            gotoRowCol(21,90);
            cin >> fromRow ;

            gotoRowCol(22,50);
            cout<<"enter column from where you want to move:";
            gotoRowCol(22,92);
            cin >> fromCol ;

            gotoRowCol(23,50);
            cout<<"enter row  where you want to go:";
            gotoRowCol(23,82);
            cin>> toRow;

            gotoRowCol(24,50);
            cout<<"enter column  where you want to go:";
            gotoRowCol(24,85);
            cin >> toCol;

            gotoRowCol(26, 50);
        cout << "Enter 'S' to save the game and 'N' not to save the game: ";

        char moveChoice;
        cin>>moveChoice;

        if (moveChoice == 'S' || moveChoice == 's')
        {
            saveGame(chessboard, whiteTurn);
            gotoRowCol(28,50);
            cout << "Game saved successfully."<<endl;

            break;
        }

            if (isValidMove(fromRow, fromCol, toRow, toCol, chessboard, whiteTurn))
            {
                chessboard[toRow * 8 + toCol] = chessboard[fromRow * 8 + fromCol];
                chessboard[fromRow * 8 + fromCol] = ' ';

                whiteTurn = !whiteTurn;  // Switch turns

                sleep(1);
            } else {
                gotoRowCol(29 ,50);
                cout << "Invalid move. Try again."<<endl;
                sleep(1);
            }
        }
             getch();

    return 0;
}

