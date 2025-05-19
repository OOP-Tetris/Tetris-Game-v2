#include "Map.h"
#include "Block.h"
#include<iostream>

using namespace std;

Map::Map(int x, int y) : ab_x(x), ab_y(y) {}

void Map::drawFrame(int level) {
    int color = (level % 6) + 1;
    for (int i = 0; i <= 20; ++i) {
        Console::setColor(color);
        Console::gotoxy(ab_x, ab_y + i); 
        cout << "■";
        Console::gotoxy(ab_x + 26, ab_y + i); 
        cout << "■";
    }
    for (int j = 0; j < 14; ++j) {
        Console::setColor(color);
        Console::gotoxy(ab_x + j * 2, ab_y + 20);
        cout << "■";
    }
}

void Map::drawFixedBlocks(char totalBlock[21][14]) {
    for (int i = 0; i <= 20; ++i)
        for (int j = 0; j < 14; ++j)
            if (totalBlock[i][j]) {
                Console::setColor(DARK_GRAY);
                Console::gotoxy(j * 2 + ab_x, i + ab_y);
                cout << "■";
            }
}

void Map::drawGameStatus(int level, int score, int remainingLines) {
    Console::setColor(WHITE);
    Console::gotoxy(33, 7);  cout << "STAGE";
    Console::gotoxy(41, 7);  cout << level + 1;
    Console::gotoxy(33, 9);  cout << "SCORE";
    Console::gotoxy(41, 9);  cout << score;
    Console::gotoxy(33, 11); cout << "LINES";
    Console::gotoxy(41, 11); cout << remainingLines;
}

void Map::drawNextBlock(int shape, int angle) {
    Console::setColor(WHITE);
    for (int i = 1; i < 7; ++i) {
        Console::gotoxy(33, i);
        for (int j = 0; j < 8; ++j) {
            if (i == 1 || i == 6 || j == 0 || j == 4) cout << "■";
            else cout << "  ";
        }
    }

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (Block::SHAPES[shape][angle][i][j])
                Console::gotoxy(33 + j * 2 + 2, 2 + i),
                Console::setColor(shape + 1),  // 색상 대응
                cout << "■";
}