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
           	 // 게임 종료 조건
    	if (GetAsyncKeyState(VK_ESCAPE)) {
       	break; // 루프를 빠져나가서 게임 종료
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

               // 게임 보드 그리기
        system("cls");
        printf("***** Stage %d *****\n", stage);
        printf("Speed up! (%d)\n\n", rally_count);
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

        // 게임 속도 조절
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
