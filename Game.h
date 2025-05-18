// Game.h
#pragma once

#include "Block.h"
#include "Stage.h"
#include "Console.h"
#include "Map.h"

class Game {
private:
    int level;               // 현재 스테이지 (0~9)
    int score;               // 점수
    int lines;               // 현재 스테이지에서 클리어한 줄 수
    int ab_x, ab_y;          // 게임 맵 출력 시작 위치
    bool isCurrentBlockActive; // 현재 블럭이 고정됐는지 여부

    char totalBlock[21][14]; // 게임 전체 맵 (고정된 블럭 포함)
    Block currentBlock;      // 현재 떨어지는 블럭
    Block nextBlock;         // 다음 블럭
    Stage stages[10];        // 스테이지별 속도, 조건 등
    Map map;

public:
    Game();

    void run();                         // 게임 시작 및 메인 루프
    void update();                      // 블럭 자동 하강 및 고정 처리
    void handleInput();                 // 사용자 키 입력 처리
    void draw();                        // 전체 그리기 (블럭 + 맵 + 상태창)
    void clearLines();                 // 줄 삭제 및 점수 처리
    bool checkCollision(const Block& blk);   // 충돌 여부 확인
    int makeNewBlock();                      // 새 블럭 종류 랜덤 생성
    bool inputLevel();                       // 시작 레벨 입력
    void drawBlock(int shape,int angle, int xpos, int ypos);
    int eraseBlock(int shape, int angle, int x, int y);
};

