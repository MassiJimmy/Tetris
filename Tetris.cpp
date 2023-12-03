// Tetris.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.

#include <iostream>
using namespace std;
#include <chrono>
#include <thread>
#include <stdlib.h> 
#include <Windows.h>
#include "Tetris.h"
#include <vector>
#include <Mmsystem.h>
#include <mciapi.h>
//these two headers are already included in the <Windows.h> header
#include <ctime>


wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;

int nScreenWidth = 80;
int nScreenHeight = 30;
bool GameBegin = false;


int Rotate(int px, int py, int r)
{
    switch (r % 4)
    {
    case 0: return py * 4 + px;             //Forme de base à 0°
    case 1: return 12 + py - (px * 4);      //Rotation 90°
    case 2: return 15 - (py * 4) - px;      //Rotation 180°
    case 3: return 3 - py + (px * 4);       //Rotation 270°
    }
    return 0;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    for(int px = 0; px < 4; px++)
        for (int py = 0; py < 4; py++)
        {
            //Get index into space
            int pi = Rotate(px, py, nRotation);

            //Get index into field
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

            if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
            {
                if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                {
                    if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
                    {
                        return false;
                    }
                }
            }

        }


    return true;
}



int jeu()
{
    // Les assets:
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");

    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");

    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");

    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"....");

    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"..X.");

    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");

    pField = new unsigned char[nFieldWidth * nFieldHeight];         //création de l'écran du jeu
    for (int x = 0; x < nFieldWidth; x++) // Board Boundary
        for (int y = 0; y < nFieldHeight; y++)
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)  screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    bool bGameOver = false;

    int nCurrentPiece = rand() % 7;
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth / 2;
    int nCurrentY = 0;

    bool bKey[4];
    bool bRotateHold = false;

    int nSpeed = 20;
    int nSpeedCounter = 0;
    bool bForceDown = false;
    int nPieceCount = 0;
    int nScore = 0;

    

    vector<int> vLines;

   
    while (!bGameOver && GameBegin == true)
    {
        //Game Timming ===================================
        this_thread::sleep_for(50ms);
        nSpeedCounter++;
        bForceDown = (nSpeed == nSpeedCounter);

        //Inputs =========================================
        for (int k = 0; k < 4; k++)
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

        //Logique ========================================
        /* manière pro
        nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1:  0;
        nCurrentX += (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
        nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

        nCurrentRotation += (bKey[3] && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
        */
        if (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY))
            {
                nCurrentX = nCurrentX + 1;
            }

        if (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY))
            {
                nCurrentX = nCurrentX - 1;
            }
        

        if (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX , nCurrentY + 1))
            {
                nCurrentY = nCurrentY + 1;
            }
        if (bKey[3] && !bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY))
        {
            nCurrentRotation = nCurrentRotation + 1;
            bRotateHold = true;
        }
        else
        {
            bRotateHold = false;
        }
        
        if (bForceDown)
        {
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation,nCurrentX, nCurrentY + 1))
            {
                nCurrentY ++;
            }
            else
            {
                //Bloquer la piece actuel
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

                //Augmentation de la vitesse/difficulté 
                nPieceCount++;
                if (nPieceCount % 10 == 0)
                    if (nSpeed >= 10) nSpeed--;

                //Check si on a une ligne
                for (int py = 0; py < 4;py++)
                    if (nCurrentY + py < nFieldHeight - 1)
                    {
                        bool bLine = true;
                        for (int px = 1; px < nFieldWidth - 1; px++)
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

                        if (bLine)
                        {
                            //On remplace la ligne par =
                            for (int px = 1; px < nFieldWidth - 1; px++)
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;

                            vLines.push_back(nCurrentY + py);
                        }
                    }
                nScore += 25;
                if (!vLines.empty()) nScore += (1 << vLines.size()) * 100;


                //Prochaine Pièce
                nCurrentPiece = rand() % 7;
                nCurrentRotation = 0;
                nCurrentX = nFieldWidth / 2;
                nCurrentY = 0;

                //SI la piece ne passe pas...

                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }
            nSpeedCounter = 0;
        }
        //Output =========================================

        //Terrain
        for (int x = 0; x < nFieldWidth; x++)
            for (int y = 0; y < nFieldHeight; y++)
                screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];

        // Draw Current Piece
        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                    screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

        //Afficher le score
        swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: ", nScore);


        if (!vLines.empty())
        {
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
            this_thread::sleep_for(400ms);

            for (auto &v : vLines)
                for (int px = 1; px < nFieldWidth - 1; px++)
                {
                    for (int py = v; py > 0; py--)
                        pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
                    pField[px] = 0;
                }
            vLines.clear();
        }
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);   
    }

    //Bah rip
    CloseHandle(hConsole);
    cout << "Game Over! Votre Score:" << nScore << endl;
    system("pause");

    return 0;
}

int main()
{
    if (GameBegin == false)
    {
        string confirm;
        cout << " " << endl;
        cout << " " << endl;
        cout << "  ##################################################################" << endl;
        cout << "  #                                                                #" << endl;
        cout << "  #  ---------  ---------  ---------  ---------  ----   ---------  #" << endl;
        cout << "  #  ---------  ---------  ---------  ---------  ----   ---------  #" << endl;
        cout << "  #     ---     ----         ---      ---   ---         ---        #" << endl;
        cout << "  #     ---     ----         ---      ---   ---  ----   ---        #" << endl;
        cout << "  #     ---     -------      ---      ------     ----   ---------  #" << endl;
        cout << "  #     ---     -------      ---      ------     ----   ---------  #" << endl;
        cout << "  #     ---     ----         ---      ---   ---  ----         ---  #" << endl;
        cout << "  #     ---     ----         ---      ---   ---  ----         ---  #" << endl;
        cout << "  #     ---     ---------    ---      ---   ---  ----   ---------  #" << endl;
        cout << "  #     ---     ---------    ---      ---   ---  ----   ---------  #" << endl;
        cout << "  #                                                                #" << endl;
        cout << "  ##################################################################" << endl;
        cout << "                                                                    " << endl;
        cout << "                       *  Par Jimmy Massi  *                        " << endl;
        cout << "                                                                    " << endl;
        cout << "                                                                    " << endl;
        cin >> confirm;
        if (confirm != "")
        {
            GameBegin = true;
            jeu();
        }
    }
    return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage
