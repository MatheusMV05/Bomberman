#include "screen.h"
#include "timer.h"
#include "keyboard.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PLAYER_SYMBOL '^'
#define INVADER_SYMBOL 'W'
#define BULLET_SYMBOL '|'
#define ALIEN_BULLET_SYMBOL 'v'

#define SCREEN_WIDTH 75
#define SCREEN_HEIGHT 23
#define HITBOX_RADIUS 1

int playerX, playerY;
int bulletX, bulletY;
int invaders[5][10];
int invaderPosX[5][10];
int invaderPosY[5][10];
int alienBulletX, alienBulletY;
int score = 0;

// Declarações das funções
void drawBorders();
void drawPlayer();
void drawInvaders();
void drawBullet();
void drawAlienBullet();
void updateBullet();
void updateAlienBullet();
void updateInvaders();
void movePlayer(int direction);
void shootBullet();
void alienShoot();

// Função para salvar o score em um arquivo
void saveScore(const char *playerName, int score) {
    FILE *file = fopen("score.txt", "a");
    if (file != NULL) {
        fprintf(file, "Nome: %s - Pontuação: %d\n", playerName, score);
        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo de pontuação.\n");
    }
}

// Função para exibir o score mais alto no menu
void showHighScore() {
    FILE *file = fopen("score.txt", "r");
    char line[100];
    int highestScore = 0;
    char highestPlayer[30] = "";

    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            char playerName[30];
            int playerScore;

            if (sscanf(line, "Nome: %29[^-] - Pontuação: %d", playerName, &playerScore) == 2) {
                if (playerScore > highestScore) {
                    highestScore = playerScore;
                    strncpy(highestPlayer, playerName, sizeof(highestPlayer));
                }
            }
        }
        fclose(file);
    }

    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 6);
    printf("High Score: %s - %d", highestPlayer, highestScore);
}

void displayScore() {
    screenGotoxy(2, 1);
    printf("Score: %d", score);
}

void gameOver() {
    char playerName[30];
    screenClear();
    drawBorders();
    
    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 2);
    printf("Game Over! Seu score: %d", score);

    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2);
    printf("Digite seu nome: ");
    fflush(stdout);
    fgets(playerName, sizeof(playerName), stdin);

    size_t len = strlen(playerName);
    if (len > 0 && playerName[len - 1] == '\n') {
        playerName[len - 1] = '\0';
    }

    saveScore(playerName, score);
    printf("Score salvo! Pressione qualquer tecla para voltar ao menu...");
    while (!keyhit()) {}
    readch();
}

void drawBorders() {
    screenDrawBorders();
}

void showMenu() {
    screenClear();
    drawBorders();
    showHighScore();

    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 4);
    printf("== SPACE INVADERS ==");

    screenGotoxy(SCREEN_WIDTH / 2 - 8, SCREEN_HEIGHT / 2 - 2);
    printf("1. Start Game");
    screenGotoxy(SCREEN_WIDTH / 2 - 8, SCREEN_HEIGHT / 2 - 1);
    printf("2. Quit");

    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 1);
    printf("Controls:");
    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 2);
    printf("A - Move Left");
    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 3);
    printf("D - Move Right");
    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 4);
    printf("W - Shoot");
    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 5);
    printf("Q - Quit Game");

    screenUpdate();
}

int mainMenu() {
    showMenu();
    while (1) {
        if (keyhit()) {
            int choice = readch();
            if (choice == '1') {
                return 1;
            } else if (choice == '2') {
                return 0;
            }
        }
    }
}

void initGame() {
    screenInit(1);
    timerInit(200);
    keyboardInit();
    playerX = SCREEN_WIDTH / 2;
    playerY = SCREEN_HEIGHT - 2;
    bulletX = -1;
    bulletY = -1;
    alienBulletX = -1;
    alienBulletY = -1;
    score = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            invaders[i][j] = 1;
            invaderPosX[i][j] = 5 + j * 3;
            invaderPosY[i][j] = 3 + i * 2;
        }
    }
}

void drawPlayer() {
    screenGotoxy(playerX, playerY);
    printf("%c", PLAYER_SYMBOL);
}

void drawInvaders() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (invaders[i][j] == 1) {
                screenGotoxy(invaderPosX[i][j], invaderPosY[i][j]);
                printf("%c", INVADER_SYMBOL);
            }
        }
    }
}

void drawBullet() {
    if (bulletY > 0) {
        screenGotoxy(bulletX, bulletY);
        printf("%c", BULLET_SYMBOL);
    }
}

void drawAlienBullet() {
    if (alienBulletY > 0) {
        screenGotoxy(alienBulletX, alienBulletY);
        printf("%c", ALIEN_BULLET_SYMBOL);
    }
}

void updateBullet() {
    if (bulletY > 0) {
        bulletY--;
        if (bulletY == 0) bulletX = bulletY = -1;
    }
}

void updateAlienBullet() {
    if (alienBulletY > 0) {
        alienBulletY++;
        if (alienBulletY >= playerY && alienBulletX == playerX) {
            gameOver();
        }
        if (alienBulletY >= SCREEN_HEIGHT) alienBulletY = -1;
    }
}

void shootBullet() {
    if (bulletY < 0) {
        bulletX = playerX;
        bulletY = playerY - 1;
    }
}

void alienShoot() {
    for (int i = 4; i >= 0; i--) {
        for (int j = 0; j < 10; j++) {
            if (invaders[i][j] == 1) {
                alienBulletX = invaderPosX[i][j];
                alienBulletY = invaderPosY[i][j] + 1;
                return;
            }
        }
    }
}

void movePlayer(int direction) {
    screenGotoxy(playerX, playerY);
    printf(" ");
    playerX += direction;
    if (playerX < 1) playerX = 1;
    if (playerX > SCREEN_WIDTH - 2) playerX = SCREEN_WIDTH - 2;
}

void updateInvaders() {
    static int direction = 1;
    static int stepDown = 0;

    if (stepDown) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 10; j++) {
                invaderPosY[i][j] += 1;
                if (invaderPosY[i][j] >= playerY - 1) {
                    gameOver();
                }
            }
        }
        stepDown = 0;
    } else {
        int atEdge = 0;
        for (int i = 0; i < 5 && !atEdge; i++) {
            if ((invaderPosX[i][0] <= 1 && direction == -1) || 
                (invaderPosX[i][9] >= SCREEN_WIDTH - 2 && direction == 1)) {
                atEdge = 1;
                direction *= -1;
                stepDown = 1;
            }
        }
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 10; j++) {
                invaderPosX[i][j] += direction;
            }
        }
    }
}

void checkCollisions() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (invaders[i][j] == 1) {
                if (bulletX >= invaderPosX[i][j] - HITBOX_RADIUS &&
                    bulletX <= invaderPosX[i][j] + HITBOX_RADIUS &&
                    bulletY >= invaderPosY[i][j] - HITBOX_RADIUS &&
                    bulletY <= invaderPosY[i][j] + HITBOX_RADIUS) {
                    
                    invaders[i][j] = 0;
                    bulletX = bulletY = -1;
                    score += 10;
                }
            }
        }
    }
}

void gameLoop() {
    int ch;
    while (1) {
        if (timerTimeOver()) {
            screenClear();
            drawBorders();
            drawPlayer();
            drawInvaders();
            drawBullet();
            drawAlienBullet();
            updateBullet();
            updateAlienBullet();
            updateInvaders();
            checkCollisions();
            displayScore();
            if (rand() % 20 == 0) alienShoot();  // Alien atira aleatoriamente
            screenUpdate();
        }

        if (keyhit()) {
            ch = readch();
            if (ch == 'a') movePlayer(-1);
            else if (ch == 'd') movePlayer(1);
            else if (ch == 'w') shootBullet();
            else if (ch == 'q') break;
        }
    }
    gameOver();
}

int main() {
    if (mainMenu()) {
        initGame();
        gameLoop();
    }
    keyboardDestroy();
    timerDestroy();
    screenDestroy();
    return 0;
}
