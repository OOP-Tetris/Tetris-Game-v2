#include "Game.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <cstring>
using namespace std;

int Game::makeNewBlock() {
    int r = rand() % 100;
    if (r <= stages[level].stickRate) return 0;
    return (rand() % 6) + 1;
}

bool Game::inputLevel() {
    char buf[5];
    Console::setColor(GRAY);
    Console::gotoxy(10, 7);
    printf("┏━━━━━━━━━<GAME KEY>━━━━━━━━━┓");
    Sleep(10);
    Console::gotoxy(10, 8);
    printf("┃ UP   : Rotate Block        ┃");
    Sleep(10);
    Console::gotoxy(10, 9);
    printf("┃ DOWN : Move One-Step Down  ┃");
    Sleep(10);
    Console::gotoxy(10, 10);
    printf("┃ SPACE: Move Bottom Down    ┃");
    Sleep(10);
    Console::gotoxy(10, 11);
    printf("┃ LEFT : Move Left           ┃");
    Sleep(10);
    Console::gotoxy(10, 12);
    printf("┃ RIGHT: Move Right          ┃");
    Sleep(10);
    Console::gotoxy(10, 13);
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    while (true) {
        Console::gotoxy(10, 3);
        cout << "Select Start level[1-8]:       \b\b\b\b\b\b\b";
        fflush(stdout);
        fgets(buf, sizeof(buf), stdin);
        if (strchr(buf, '\n')) buf[strcspn(buf, "\n")] = '\0';
        else while (getchar() != '\n');

        if (strlen(buf) == 1 && buf[0] >= '1' && buf[0] <= '8') {
            level = buf[0] - '1';
            return true;
        }
        else {
            Console::gotoxy(10, 4);
            cout << "올바른 숫자를 입력해주세요.";
            Sleep(1000);
            Console::gotoxy(10, 4);
            cout << "                            ";
            system("cls");
        }
    }
}




Game::Game() : level(0), score(0), lines(0), ab_x(5), ab_y(1), isCurrentBlockActive(true), map(ab_x, ab_y) {
    for (int i = 0; i < 10; ++i)
        stages[i] = Stage(40 - i * 4, 20 - i, 1);
    memset(totalBlock, 0, sizeof(totalBlock));
    for (int i = 0; i < 21; ++i)
        totalBlock[i][0] = totalBlock[i][13] = 1;
    for (int j = 0; j < 14; ++j) 
        totalBlock[20][j] = 1;
}

//커서를 숨기는 함수
void Game::hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;  // 커서 크기 설정 (1~100)
    cursorInfo.bVisible = FALSE;  // 커서 숨김
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void Game::update_ReverseWorld()
{
    Block temp = currentBlock;
    temp.y -= 1;

    if (checkCollision_ReverseWorld(temp)) {
        // 충돌 → 블럭 고정
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (Block::SHAPES[currentBlock.shape][currentBlock.angle][i][j]) {
                    totalBlock[currentBlock.y + i][currentBlock.x + j] = 1;
                }
            }
        }

        isCurrentBlockActive = false;
        clearLines();
        map.drawFixedBlocks(totalBlock);
        currentBlock = nextBlock;
        nextBlock = Block(makeNewBlock());
        nextBlock.y = 17;
        isCurrentBlockActive = true;
        map.drawNextBlock(nextBlock.shape, nextBlock.angle);
        map.drawGameStatus(level, score, stages[level].clearLine - lines);

        if (checkCollision_ReverseWorld(currentBlock)) {
            //gameOver();
            for (int j = 0; j < 14; j++) {
               
                totalBlock[0][j] = 0;
            }
            exit(0);
            level += 1;
            return;
        }
    }
    else {
        // 한 칸 아래로
        eraseBlock(currentBlock.shape, currentBlock.angle, currentBlock.x, currentBlock.y);
        currentBlock = temp;
    }
}

void Game::handleInput_ReverseWorld()
{
    int key = _getch();
    Block temp = currentBlock;
    if (key == 224) { // 특수키
        int arrow = _getch();
        switch (arrow) {
        case 72: temp.rotate(); break;
        case 75: temp.x--; break;
        case 77: temp.x++; break;
        case 80: temp.y--; break;
        }
    }
    else if (key == ' ') {
        while (!checkCollision_ReverseWorld(temp)) {
            eraseBlock(currentBlock.shape, currentBlock.angle, currentBlock.x, currentBlock.y);
            currentBlock = temp;
            temp.y--;
        }
        temp.y++;
    }

    if (!checkCollision_ReverseWorld(temp)) {
        eraseBlock(currentBlock.shape, currentBlock.angle, currentBlock.x, currentBlock.y);
        currentBlock = temp;
    }

    draw();
}

bool Game::checkCollision_ReverseWorld(const Block& blk)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (Block::SHAPES[blk.shape][blk.angle][i][j]) {
                int dx = blk.x + j;
                int dy = blk.y + i;
                if (dy > 20) continue;
                if (dy <= -1 || dx < 0 || dx >= 14 || totalBlock[dy][dx])
                    return true;
            }
    return false;
}

void Game::run() {
   hideCursor();
    srand((unsigned)time(NULL));
   // showLogo();
    if (!inputLevel()) return;
    system("cls");

    //거꾸로 나라의 경우 위의 화면이 차 있어야 한다
    if (level % 3 == 0) {
        for (int j = 0; j < 14; j++) {
            totalBlock[20][j] = 0;
        }
     
    }
    else {
        for (int j = 0; j < 14; j++) {
            totalBlock[20][j] = 1;
        }
    }

    currentBlock = Block(makeNewBlock());
    nextBlock = Block(makeNewBlock());
   
    map.drawFrame(level);
    map.drawGameStatus(level, score, stages[level].clearLine-lines);
    map.drawNextBlock(nextBlock.shape, nextBlock.angle);
    draw();

    if (level % 3 == 0 && level < 9) {
        currentBlock.y = 15;
        nextBlock.y = 15;
        while (true) {
            if (_kbhit()) handleInput_ReverseWorld();
            static int frame = 0;
            if (++frame % stages[level].speed == 0) {
                update_ReverseWorld();
                draw();
            }
            Sleep(15);
        }
    }
    else {
        while (true) {
            if (_kbhit()) handleInput();
            static int frame = 0;
            if (++frame % stages[level].speed == 0) {
                update();
                draw();
            }
            Sleep(15);
        }
    }
    
}

void Game::update() {
    Block temp = currentBlock;
    temp.y += 1;

    if (checkCollision(temp)) {
        // 충돌 → 블럭 고정
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (Block::SHAPES[currentBlock.shape][currentBlock.angle][i][j]) {
                    totalBlock[currentBlock.y + i][currentBlock.x + j] = 1;
                }
            }
        }
        
        isCurrentBlockActive = false;
        clearLines();
        map.drawFixedBlocks(totalBlock);
        currentBlock = nextBlock;
        nextBlock = Block(makeNewBlock());
        isCurrentBlockActive = true;
        map.drawNextBlock(nextBlock.shape, nextBlock.angle);
        map.drawGameStatus(level,score, stages[level].clearLine-lines);

        if (checkCollision(currentBlock)) {
            //gameOver();
            exit(0);
            return;
        }
    }
    else {
        // 한 칸 아래로
        eraseBlock(currentBlock.shape, currentBlock.angle, currentBlock.x, currentBlock.y);
        currentBlock = temp;
    }
}

void Game::handleInput() {
    int key = _getch();
    Block temp = currentBlock;

    if (key == 224) { // 특수키
        int arrow = _getch();
        switch (arrow) {
        case 72: temp.rotate(); break;
        case 75: temp.x--; break;
        case 77: temp.x++; break;
        case 80: temp.y++; break;
        }
    }
    else if (key == ' ') {
        while (!checkCollision(temp)) {
            eraseBlock(currentBlock.shape, currentBlock.angle, currentBlock.x, currentBlock.y);
            currentBlock = temp;
            temp.y++;
        }
        temp.y--;
    }

    if (!checkCollision(temp)) {
        eraseBlock(currentBlock.shape, currentBlock.angle, currentBlock.x, currentBlock.y);
        currentBlock = temp;
    }

    draw();
}


void Game::drawBlock(int shape, int angle, int xpos, int ypos)
{
    int i, j;

    switch (shape)
    {
    case 0:
        Console::setColor(RED);
        break;
    case 1:
        Console::setColor(BLUE);
        break;
    case 2:
        Console::setColor(SKY_BLUE);
        break;
    case 3:
        Console::setColor(WHITE);
        break;
    case 4:
        Console::setColor(YELLOW);
        break;
    case 5:
        Console::setColor(VOILET);
        break;
    case 6:
        Console::setColor(GREEN);
        break;
    }

    for (i = 0;i < 4;i++)
    {
        for (j = 0;j < 4;j++)
        {
            if ((j + ypos) < 0)
                continue;

            if (Block::SHAPES[shape][angle][j][i] == 1)
            {
                Console::gotoxy((i + xpos) * 2 + ab_x, j + ypos + ab_y);
                printf("■");
            }
        }
        Console::gotoxy(77, 23);
        //printf("last: Block Position - x: %d, y: %d\n", block_x, block_y);
    }
    Console::setColor(BLACK);
    Console::gotoxy(77,23);
    return ;
}



int Game::eraseBlock(int shape, int angle, int x, int y)
{
    int i, j;
    for (i = 0;i < 4;i++)
    {
        for (j = 0;j < 4;j++)
        {
            if (Block::SHAPES[shape][angle][j][i] == 1)
            {
                Console::gotoxy((i + x) * 2 + ab_x, j + y + ab_y);
                printf("  ");

            }
        }
    }
    return 0;
}

void Game::draw() {

    // 현재 블럭 출력
    if (isCurrentBlockActive) {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (Block::SHAPES[currentBlock.shape][currentBlock.angle][i][j]) {
                    int dx = currentBlock.x + j;
                    int dy = currentBlock.y + i;
                    if (dy >= 0 && totalBlock[dy][dx] == 0)
                        drawBlock(currentBlock.shape, currentBlock.angle, currentBlock.x, currentBlock.y);
                }
    }

    Console::setColor(WHITE);
    Console::gotoxy(0, 23);
}



bool Game::checkCollision(const Block& blk) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (Block::SHAPES[blk.shape][blk.angle][i][j]) {
                int dx = blk.x + j;
                int dy = blk.y + i;
                if (dy < 0) continue;
                if (dy >= 20 || dx < 0 || dx >= 14 || totalBlock[dy][dx])
                    return true;
            }
    return false;
}

void Game::clearLines() {
    for (int i = 0; i < 20; ++i) {
        bool full = true;
        for (int j = 1; j < 13; ++j)
            if (!totalBlock[i][j]) full = false;

        if (full) {
            for (int k = i; k > 0; --k)
                for (int j = 1; j < 13; ++j)
                    totalBlock[k][j] = totalBlock[k - 1][j];
            for (int j = 1; j < 13; ++j)
                totalBlock[0][j] = 0;
            score += 100 + level * 10;
            lines++;
            if (lines >= stages[level].clearLine) {
                lines = 0;
                if (level < 9) {
                    level++;
                    map.drawFrame(level);
                 
                    map.drawGameStatus(level, score, stages[level].clearLine-lines);
                }
                else {
                    //gameClear();
                    break;
                }
            }
        }
    }
}



