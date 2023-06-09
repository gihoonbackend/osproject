---
title: "컴퓨터 알고리즘 중간고사 과제"
subtitle: "Fast Fourier Transform 알고리즘"
layout: post
author: "Gihoonsong"
date: 2023-01-05 01:00:04
header-style: text
hidden: true
tags:
  - 알고리즘
---
**전체 코드와 함께 주석을 달아 코드를 설명하겠습니다.**, 이후 실제 작동하였던 결과화면을 통해 하키게임 구현을 해보겠습니다.  

## 작업환경
우선 windows 운영체제를 사용하여 진행하였기 때문에 **MinGw를 사용하여 GCC 컴파일러를 설치하고 환경변수를 등록하였습니다.**

## 코드 구현:
1. 필요한 라이브러리 및 헤더파일을 가져와야 함으로 stdio,conio,stdlib등을 가져왔으며 rand함수를 이용하기 위해 time 헤더파일을 가져왔습니다. 
2. 게임 보드를 구현하기 위해 필요한 조건은. 직 사각형의 보드를 사용하며, 각 양측에 움직일 수 있는 바를 구현해야 합니다. 또한 보드는 키보드 입력에
따라 움직일 수 있는 위치를 업데이트 해야합니다. 
3. 우선 주어진 조건에 맞게 사용자 측면은 키보드 입력, 컴퓨터는 업 다운을 하는 알고리즘을 사용해 자동 움직임으로 설정합니다.
4. 스테이지가 올라갈 수록 컴퓨터의 bar와 공의 속도를 빨라지게 설정하였습니다. 여기서 스테이지는 양쪽의 점수의 합이 5점이 넘어갈때마다 스테이지가 올라가도록 구현하였습니다. 
5. 부가적으로 공이 bar에 닿으면 반사되도록 구현하였으며, 그렇지 않을경우 상대팀의 점수가 올라가게 구현하였습니다.
6. 마지막으로 esc를 누르면 해당 게임을 종료하도록 추가하였습니다. 게임 종료를 하게 되면 현재 점수를 기반으로 자신이 졌는이 이겼는지 출력되도록 구현하였습니다.
다음은 실제 구현한 c언어 기반 코드입니다.

```
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define BOARD_WIDTH 60
#define BOARD_HEIGHT 20
#define PADDLE_SIZE 4
#define BALL_SIZE 1

int main() {
    srand(time(NULL));

    // 윈도우 생성
    HWND hwnd = GetConsoleWindow();
    RECT rect;
    GetWindowRect(hwnd, &rect);
    MoveWindow(hwnd, rect.left, rect.top, 800, 600, TRUE);

    // 게임 보드 초기화
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (j == 0 || j == BOARD_WIDTH - 1) {
                board[i][j] = '#';
            } else if (i == 0 || i == BOARD_HEIGHT - 1) {
                board[i][j] = '=';
            } else {
                board[i][j] = ' ';
            }
        }
    }

    // 사용자 및 컴퓨터 측면의 바 초기화
    int user_paddle = BOARD_HEIGHT / 2;
    int computer_paddle = BOARD_HEIGHT / 2;

    // 공 초기화
    int ball_x = BOARD_WIDTH / 2;
    int ball_y = BOARD_HEIGHT / 2;
    int ball_dx = rand() % 2 == 0 ? 1 : -1;
    int ball_dy = rand() % 2 == 0 ? 1 : -1;

    // 게임 루프
    int stage = 1;
    int user_score = 0;
    int computer_score = 0;
    int rally_count = 0;
    int difficulty = 1;
    int computer_speed = 1;

    while (1) {
        // 사용자 및 컴퓨터 측면의 바 업데이트
        if (GetAsyncKeyState(VK_UP)) {
            if (user_paddle > 1) {
                user_paddle--;
            }
        }
        if (GetAsyncKeyState(VK_DOWN)) {
            if (user_paddle < BOARD_HEIGHT - PADDLE_SIZE - 1) {
                user_paddle++;
            }
        }
        if (computer_paddle < ball_y - PADDLE_SIZE / 2) {
            computer_paddle += computer_speed;
        }
        if (computer_paddle > ball_y + PADDLE_SIZE / 2) {
            computer_paddle -= computer_speed;
        }

        // 공 업데이트
        ball_x += ball_dx;
        ball_y += ball_dy;
        if (ball_x < 1) {
            computer_score++;
            ball_x = BOARD_WIDTH / 2;
            ball_y = BOARD_HEIGHT / 2;
                        ball_dx = rand() % 2 == 0 ? 1 : -1;
            ball_dy = rand() % 2 == 0 ? 1 : -1;
            rally_count = 0;
        }
        if (ball_x > BOARD_WIDTH - BALL_SIZE - 1) {
            user_score++;
            ball_x = BOARD_WIDTH / 2;
            ball_y = BOARD_HEIGHT / 2;
            ball_dx = rand() % 2 == 0 ? 1 : -1;
            ball_dy = rand() % 2 == 0 ? 1 : -1;
            rally_count = 0;
        }
        if (ball_y < 1 || ball_y > BOARD_HEIGHT - BALL_SIZE - 1) {
            ball_dy = -ball_dy;
        }
        if ((ball_x == 1 && ball_y >= user_paddle && ball_y < user_paddle + PADDLE_SIZE) || (ball_x == BOARD_WIDTH - BALL_SIZE - 2 && ball_y >= computer_paddle && ball_y < computer_paddle + PADDLE_SIZE)) {
            ball_dx = -ball_dx;
            rally_count++;
        }
//위의 코드에서 많은 오류를 겪음. , 공이 경기장의 왼쪽 벽에 부딪히면 computer_score를 1 증가시키고, 공의 위치를 다시 중앙으로 이동. 또한, 공의 방향을 무작위로 선택하고 rally_count를 0으로 초기화. 마지막으로, 공이 사용자나 컴퓨터 측면의 바에 부딪치면 ball_dx를 반대 방향으로 바꾸고 rally_count를 1 증가시. ball_dx를 바꾸는 것으로 인해 공의 방향이 바뀌므로, 다음 번 업데이트에서 공이 반대쪽으로 이동함.
   	 // 게임 종료 조건
    	 if (GetAsyncKeyState(VK_ESCAPE)) {
       	break; // 루프를 빠져나가서 게임 종료
   	 }              

               // 게임 보드 그리기
        system("cls");  // 콘솔 창을 지우는 명령어. 게임보드를 계속해서 업데이트 하기위해 사용함.
        printf("*** Stage %d ***\n", stage);
        printf("User Score: %d | Computer Score: %d\n\n", user_score, computer_score); // 점수 표시 추가
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                if (i >= user_paddle && i < user_paddle + PADDLE_SIZE && j == 1) {
                    printf("|");
                } else if (i >= computer_paddle && i < computer_paddle + PADDLE_SIZE && j == BOARD_WIDTH - 2) {
                    printf("|");
                } else if (i == ball_y && j == ball_x) {
                    printf("O");
                } else {
                    printf("%c", board[i][j]);
                }
            }
            printf("\n");
        }

        // 난이도 조절
        if (rally_count % 10 == 0 && rally_count != 0) {
            difficulty++;
            computer_speed = difficulty;
            rally_count = 0;
        }

        // 게임 속도 조절 : 게임 속도 조절을 하는 부분으로서 많은 수정을 겪음. 스테이지의 조건을 여러번 바꾸기도 하였으며, 공의 속도 역시 눈에 보이는 속도로 바뀌지 않는 결과가 많아서 여러 번 결과 값을 바꿔가며 오류 수정하였음.
        Sleep(50 - stage * 10);
        if (user_score + computer_score >= 5 && stage == 1) {
            stage++;
        }
        if (user_score + computer_score >= 10 && stage == 2) {
            stage++;
        }
        // 스테이지가 올라가면 속도 올라가기
        if (user_score + computer_score >= 5 && stage == 1) {
            stage++;
            ball_dx *= 2;
            ball_dy *= 2;
            computer_speed++;
        }
        if (user_score + computer_score >= 10 && stage == 2) {
            stage++;
            ball_dx *= 2;
            ball_dy *= 2;
            computer_speed++;
        }
    }

    // 게임 종료
    printf("GAME OVER\n");
    if (user_score > computer_score) {
        printf("YOU WIN!\n");
    } else {
        printf("YOU LOSE!\n");
    }
    printf("Press any key to exit.\n");
    getch();

    return 0;
}
```


## 실행결과:
![img](https://github.com/gihoonbackend/gihoonbackend.github.io/blob/master/images/%EC%8B%A4%ED%96%89%EB%B0%A9%EB%B2%95.png?raw=true)
위의 화면은 직접 vs code의 터미널 환경에서 ./test.exe를 입력하여 실행하는 화면입니다. 
![img](https://github.com/gihoonbackend/gihoonbackend.github.io/blob/master/images/SmartSelectImage_2023-05-06-18-02-21.png?raw=true)
게임이 실행되면 맨 위에는 stage의 단계, 유저, 컴퓨터의 점수를 볼수 있습니다. 
![img](https://github.com/gihoonbackend/gihoonbackend.github.io/blob/master/images/%EC%A2%85%EB%A3%8C.png?raw=true)
이 사진에서 보이는 것과 같이 ESC를 누르게 되면 게임이 종료되며, 게임이 종료됬다는 메시지와 함께 게임의 승패를 출력합니다.
