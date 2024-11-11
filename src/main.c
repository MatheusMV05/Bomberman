#include "screen.h"   // Inclui o cabeçalho para funções relacionadas à tela
#include "timer.h"    // Inclui o cabeçalho para funções relacionadas ao temporizador
#include "keyboard.h" // Inclui o cabeçalho para funções relacionadas ao teclado
#include <stdlib.h>   // Inclui a biblioteca padrão para funções de alocação de memória, controle de processos, etc.
#include <stdio.h>    // Inclui a biblioteca padrão para entrada e saída
#include <string.h>   // Inclui a biblioteca padrão para manipulação de strings

// Definições de símbolos usados no jogo
#define PLAYER_SYMBOL '^'
#define INVADER_SYMBOL 'W'
#define BULLET_SYMBOL '|'
#define ALIEN_BULLET_SYMBOL 'v'

// Definições de dimensões da tela e raio da hitbox
#define SCREEN_WIDTH 75
#define SCREEN_HEIGHT 23
#define HITBOX_RADIUS 1

// Variáveis globais para posições do jogador, balas e invasores
int playerX, playerY;           // Posição do jogador
int bulletX, bulletY;           // Posição da bala do jogador
int invaders[5][10];            // Matriz para armazenar o estado dos invasores
int invaderPosX[5][10];         // Posições X dos invasores
int invaderPosY[5][10];         // Posições Y dos invasores
int alienBulletX, alienBulletY; // Posição da bala dos invasores
int score = 0;                  // Pontuação do jogador

// Declarações das funções que serão implementadas posteriormente
void drawBorders();             // Função para desenhar as bordas da tela
void drawPlayer();              // Função para desenhar o jogador
void drawInvaders();            // Função para desenhar os invasores
void drawBullet();              // Função para desenhar a bala do jogador
void drawAlienBullet();         // Função para desenhar a bala dos invasores
void updateBullet();            // Função para atualizar a posição da bala do jogador
void updateAlienBullet();       // Função para atualizar a posição da bala dos invasores
void updateInvaders();          // Função para atualizar a posição dos invasores
void movePlayer(int direction); // Função para mover o jogador em uma direção específica
void shootBullet();             // Função para o jogador atirar uma bala
void alienShoot();              // Função para os invasores atirarem uma bala

// Função para salvar o score em um arquivo
void saveScore(const char *playerName, int score)
{
    FILE *file = fopen("score.txt", "a"); // Abre o arquivo "score.txt" em modo de adição
    if (file != NULL)
    {                                                                   // Verifica se o arquivo foi aberto com sucesso
        fprintf(file, "Nome: %s - Pontuação: %d\n", playerName, score); // Escreve o nome do jogador e a pontuação no arquivo
        fclose(file);                                                   // Fecha o arquivo
    }
    else
    {
        printf("Erro ao abrir o arquivo de pontuação.\n"); // Exibe uma mensagem de erro se o arquivo não puder ser aberto
    }
}

// Função para exibir o score mais alto no menu
void showHighScore()
{
    FILE *file = fopen("score.txt", "r"); // Abre o arquivo "score.txt" em modo de leitura
    char line[100];                       // Buffer para armazenar cada linha lida do arquivo
    int highestScore = 0;                 // Variável para armazenar a maior pontuação
    char highestPlayer[30] = "";          // Variável para armazenar o nome do jogador com a maior pontuação

    if (file != NULL)
    { // Verifica se o arquivo foi aberto com sucesso
        while (fgets(line, sizeof(line), file))
        {                        // Lê cada linha do arquivo
            char playerName[30]; // Buffer para armazenar o nome do jogador
            int playerScore;     // Variável para armazenar a pontuação do jogador

            // Extrai o nome do jogador e a pontuação da linha lida
            if (sscanf(line, "Nome: %29[^-] - Pontuação: %d", playerName, &playerScore) == 2)
            {
                // Se a pontuação do jogador atual for maior que a maior pontuação registrada
                if (playerScore > highestScore)
                {
                    highestScore = playerScore;                                // Atualiza a maior pontuação
                    strncpy(highestPlayer, playerName, sizeof(highestPlayer)); // Atualiza o nome do jogador com a maior pontuação
                }
            }
        }
        fclose(file); // Fecha o arquivo após a leitura
    }

    // Exibe a maior pontuação na tela
    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 6);
    printf("High Score: %s - %d", highestPlayer, highestScore);
}

// Função para exibir a pontuação atual do jogador na tela
void displayScore()
{
    screenGotoxy(2, 1);         // Move o cursor para a posição onde a pontuação será exibida
    printf("Score: %d", score); // Exibe a pontuação atual do jogador
}

// Função para exibir a tela de fim de jogo
void gameOver()
{
    char playerName[30]; // Buffer para armazenar o nome do jogador
    screenClear();       // Limpa a tela
    drawBorders();       // Desenha as bordas da tela

    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 2); // Move o cursor para a posição central da tela
    printf("Game Over! Seu score: %d", score);                  // Exibe a mensagem de fim de jogo e a pontuação do jogador

    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2); // Move o cursor para a posição onde o nome será solicitado
    printf("Digite seu nome: ");                            // Solicita o nome do jogador
    fflush(stdout);                                         // Garante que a mensagem seja exibida imediatamente
    fgets(playerName, sizeof(playerName), stdin);           // Lê o nome do jogador

    size_t len = strlen(playerName); // Obtém o comprimento do nome do jogador
    if (len > 0 && playerName[len - 1] == '\n')
    { // Remove o caractere de nova linha, se presente
        playerName[len - 1] = '\0';
    }

    saveScore(playerName, score);                                           // Salva a pontuação do jogador
    printf("Score salvo! Pressione qualquer tecla para voltar ao menu..."); // Informa que a pontuação foi salva
    while (!keyhit())
    { // Aguarda até que uma tecla seja pressionada
    }
    readch(); // Lê a tecla pressionada
}

// Declaração da função para desenhar as bordas da tela
void screenDrawBorders(); // Declaração da função

// Função para desenhar as bordas da tela
void drawBorders()
{
    screenDrawBorders(); // Chama a função para desenhar as bordas da tela
}

// Função para exibir o menu principal do jogo
void showMenu()
{
    screenClear();   // Limpa a tela
    drawBorders();   // Desenha as bordas da tela
    showHighScore(); // Exibe a maior pontuação

    // Exibe o título do jogo no centro da tela
    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 4);
    printf("== SPACE INVADERS ==");

    // Exibe as opções do menu
    screenGotoxy(SCREEN_WIDTH / 2 - 8, SCREEN_HEIGHT / 2 - 2);
    printf("1. Start Game");
    screenGotoxy(SCREEN_WIDTH / 2 - 8, SCREEN_HEIGHT / 2 - 1);
    printf("2. Quit");

    // Exibe os controles do jogo
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

    screenUpdate(); // Atualiza a tela para exibir as mudanças
}

int mainMenu()
{
    showMenu(); // Exibe o menu principal
    while (1)
    {
        if (keyhit()) // Verifica se uma tecla foi pressionada
        {
            int choice = readch(); // Lê a tecla pressionada
            if (choice == '1')
            {
                return 1; // Retorna 1 para iniciar o jogo
            }
            else if (choice == '2')
            {
                return 0; // Retorna 0 para sair do jogo
            }
        }
    }
}

void initGame()
{
    screenInit(1);               // Inicializa a tela
    timerInit(200);              // Inicializa o temporizador com um intervalo de 200 ms
    keyboardInit();              // Inicializa o teclado
    playerX = SCREEN_WIDTH / 2;  // Define a posição inicial do jogador no centro da tela
    playerY = SCREEN_HEIGHT - 2; // Define a posição inicial do jogador na parte inferior da tela
    bulletX = -1;                // Inicializa a posição X da bala do jogador como inativa
    bulletY = -1;                // Inicializa a posição Y da bala do jogador como inativa
    alienBulletX = -1;           // Inicializa a posição X da bala dos invasores como inativa
    alienBulletY = -1;           // Inicializa a posição Y da bala dos invasores como inativa
    score = 0;                   // Inicializa a pontuação do jogador como 0

    // Inicializa a matriz de invasores e suas posições
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            invaders[i][j] = 1;            // Define o estado do invasor como ativo
            invaderPosX[i][j] = 5 + j * 3; // Define a posição X do invasor
            invaderPosY[i][j] = 3 + i * 2; // Define a posição Y do invasor
        }
    }
}

void drawPlayer()
{
    screenGotoxy(playerX, playerY); // Move o cursor para a posição do jogador
    printf("%c", PLAYER_SYMBOL);    // Desenha o símbolo do jogador na tela
}

void drawInvaders()
{
    for (int i = 0; i < 5; i++) // Itera sobre as linhas de invasores
    {
        for (int j = 0; j < 10; j++) // Itera sobre as colunas de invasores
        {
            if (invaders[i][j] == 1) // Verifica se o invasor está ativo
            {
                screenGotoxy(invaderPosX[i][j], invaderPosY[i][j]); // Move o cursor para a posição do invasor
                printf("%c", INVADER_SYMBOL);                       // Desenha o símbolo do invasor na tela
            }
        }
    }
}

void drawBullet()
{
    if (bulletY > 0) // Verifica se a bala do jogador está ativa
    {
        screenGotoxy(bulletX, bulletY); // Move o cursor para a posição da bala do jogador
        printf("%c", BULLET_SYMBOL);    // Desenha o símbolo da bala do jogador na tela
    }
}

void drawAlienBullet()
{
    if (alienBulletY > 0) // Verifica se a bala dos invasores está ativa
    {
        screenGotoxy(alienBulletX, alienBulletY); // Move o cursor para a posição da bala dos invasores
        printf("%c", ALIEN_BULLET_SYMBOL);        // Desenha o símbolo da bala dos invasores na tela
    }
}

void updateBullet()
{
    if (bulletY > 0) // Verifica se a bala do jogador está ativa
    {
        bulletY--; // Move a bala para cima
        if (bulletY == 0)
            bulletX = bulletY = -1; // Desativa a bala se ela sair da tela
    }
}

void updateAlienBullet()
{
    if (alienBulletY > 0) // Verifica se a bala dos invasores está ativa
    {
        alienBulletY++;                                         // Move a bala para baixo
        if (alienBulletY >= playerY && alienBulletX == playerX) // Verifica se a bala atingiu o jogador
        {
            gameOver(); // Termina o jogo se a bala atingir o jogador
        }
        if (alienBulletY >= SCREEN_HEIGHT)
            alienBulletY = -1; // Desativa a bala se ela sair da tela
    }
}

void shootBullet()
{
    if (bulletY < 0) // Verifica se a bala do jogador está inativa
    {
        bulletX = playerX;     // Define a posição X da bala como a posição do jogador
        bulletY = playerY - 1; // Define a posição Y da bala logo acima do jogador
    }
}

void alienShoot()
{
    for (int i = 4; i >= 0; i--) // Itera sobre as linhas de invasores de baixo para cima
    {
        for (int j = 0; j < 10; j++) // Itera sobre as colunas de invasores
        {
            if (invaders[i][j] == 1) // Verifica se o invasor está ativo
            {
                alienBulletX = invaderPosX[i][j];     // Define a posição X da bala dos invasores como a posição do invasor
                alienBulletY = invaderPosY[i][j] + 1; // Define a posição Y da bala dos invasores logo abaixo do invasor
                return;                               // Sai da função após disparar a bala
            }
        }
    }
}

void movePlayer(int direction)
{
    screenGotoxy(playerX, playerY); // Move o cursor para a posição atual do jogador
    printf(" ");                    // Apaga o símbolo do jogador na posição atual
    playerX += direction;           // Atualiza a posição X do jogador com base na direção
    if (playerX < 1)
        playerX = 1; // Garante que o jogador não saia da borda esquerda da tela
    if (playerX > SCREEN_WIDTH - 2)
        playerX = SCREEN_WIDTH - 2; // Garante que o jogador não saia da borda direita da tela
}

void updateInvaders()
{
    static int direction = 1; // Direção de movimento dos invasores (1 para direita, -1 para esquerda)
    static int stepDown = 0;  // Flag para indicar se os invasores devem descer uma linha

    if (stepDown) // Se os invasores devem descer uma linha
    {
        for (int i = 0; i < 5; i++) // Itera sobre as linhas de invasores
        {
            for (int j = 0; j < 10; j++) // Itera sobre as colunas de invasores
            {
                invaderPosY[i][j] += 1;               // Move o invasor para baixo
                if (invaderPosY[i][j] >= playerY - 1) // Verifica se algum invasor atingiu a linha do jogador
                {
                    gameOver(); // Termina o jogo se um invasor atingir a linha do jogador
                }
            }
        }
        stepDown = 0; // Reseta a flag após mover os invasores para baixo
    }
    else
    {
        int atEdge = 0;                        // Flag para indicar se algum invasor atingiu a borda da tela
        for (int i = 0; i < 5 && !atEdge; i++) // Itera sobre as linhas de invasores
        {
            if ((invaderPosX[i][0] <= 1 && direction == -1) ||             // Verifica se algum invasor atingiu a borda esquerda
                (invaderPosX[i][9] >= SCREEN_WIDTH - 2 && direction == 1)) // Verifica se algum invasor atingiu a borda direita
            {
                atEdge = 1;      // Define a flag indicando que um invasor atingiu a borda
                direction *= -1; // Inverte a direção de movimento dos invasores
                stepDown = 1;    // Define a flag para mover os invasores para baixo na próxima atualização
            }
        }
        if (!atEdge) // Se nenhum invasor atingiu a borda
        {
            for (int i = 0; i < 5; i++) // Itera sobre as linhas de invasores
            {
                for (int j = 0; j < 10; j++) // Itera sobre as colunas de invasores
                {
                    invaderPosX[i][j] += direction; // Move o invasor na direção atual
                }
            }
        }
    }
}

void checkCollisions()
{
    for (int i = 0; i < 5; i++) // Itera sobre as linhas de invasores
    {
        for (int j = 0; j < 10; j++) // Itera sobre as colunas de invasores
        {
            if (invaders[i][j] == 1) // Verifica se o invasor está ativo
            {
                // Verifica se a bala do jogador colidiu com o invasor
                if (bulletX >= invaderPosX[i][j] - HITBOX_RADIUS &&
                    bulletX <= invaderPosX[i][j] + HITBOX_RADIUS &&
                    bulletY >= invaderPosY[i][j] - HITBOX_RADIUS &&
                    bulletY <= invaderPosY[i][j] + HITBOX_RADIUS)
                {
                    invaders[i][j] = 0;     // Desativa o invasor
                    bulletX = bulletY = -1; // Desativa a bala do jogador
                    score += 10;            // Incrementa a pontuação do jogador
                }
            }
        }
    }
}

void gameLoop()
{
    int ch;   // Variável para armazenar a tecla pressionada
    while (1) // Loop principal do jogo
    {
        if (keyhit()) // Verifica se uma tecla foi pressionada
        {
            ch = readch(); // Lê a tecla pressionada
            if (ch == 'a' || ch == 'A')
                movePlayer(-1); // Move o jogador para a esquerda
            if (ch == 'd' || ch == 'D')
                movePlayer(1); // Move o jogador para a direita
            if (ch == 'w' || ch == 'W')
                shootBullet(); // Dispara uma bala
            if (ch == 'q' || ch == 'Q')
                break; // Sai do loop e termina o jogo
        }
        updateBullet();      // Atualiza a posição da bala do jogador
        updateAlienBullet(); // Atualiza a posição da bala dos invasores
        updateInvaders();    // Atualiza a posição dos invasores
        checkCollisions();   // Verifica colisões entre balas e invasores
        screenClear();       // Limpa a tela
        drawBorders();       // Desenha as bordas da tela
        drawPlayer();        // Desenha o jogador
        drawInvaders();      // Desenha os invasores
        drawBullet();        // Desenha a bala do jogador
        drawAlienBullet();   // Desenha a bala dos invasores
        displayScore();      // Exibe a pontuação do jogador
        screenUpdate();      // Atualiza a tela para exibir as mudanças
        timerWait();         // Aguarda o próximo ciclo do temporizador
    }
    gameOver(); // Exibe a tela de fim de jogo
}

int main()
{
    if (mainMenu()) // Exibe o menu principal e verifica se o jogador escolheu iniciar o jogo
    {
        initGame(); // Inicializa o jogo
        gameLoop(); // Executa o loop principal do jogo
    }
    keyboardDestroy(); // Destrói a configuração do teclado
    timerDestroy();    // Destrói a configuração do temporizador
    screenDestroy();   // Destrói a configuração da tela
    return 0;          // Retorna 0 para indicar que o programa terminou com sucesso
}
