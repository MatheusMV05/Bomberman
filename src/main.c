#include "screen.h"   // Inclui o cabeçalho para funções relacionadas à tela
#include "timer.h"    // Inclui o cabeçalho para funções relacionadas ao temporizador
#include "keyboard.h" // Inclui o cabeçalho para funções relacionadas ao teclado
#include <stdlib.h>   // Inclui a biblioteca padrão para funções de alocação de memória, controle de processos, etc.
#include <stdio.h>    // Inclui a biblioteca padrão para entrada e saída
#include <string.h>   // Inclui a biblioteca padrão para manipulação de strings

// Definindo sprites como blocos de texto de várias linhas
const char *PLAYER_SPRITE[] = {
    "   A   ",
    "  /|\\  ",
    " / | \\ ",
    "/__|__\\"};

const char *INVADER_SPRITE[] = {
    "  MMM  ",
    " (o o) ",
    "<(   )>",
    " /---\\ "};

const char *BULLET_SPRITE = "|";
const char *ALIEN_BULLET_SPRITE = "v";

// Aumentando a tela para acomodar os sprites
#define SCREEN_WIDTH 145
#define SCREEN_HEIGHT 50
#define HITBOX_RADIUS 5

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

void inputPlayerName(int score)
{
    char playerName[20];
    FILE *file;

    screenClear();
    drawBorders();
    screenGotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2);
    printf("Digite seu nome: ");
    scanf("%19s", playerName); // Lê o nome do jogador com limite de 19 caracteres

    // Abre (ou cria) o arquivo para salvar o score e o nome do jogador
    file = fopen("score.txt", "a");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo de scoreboard.\n");
        return;
    }

    // Escreve o nome do jogador e a pontuação no arquivo
    fprintf(file, "%s %d\n", playerName, score);
    fclose(file); // Fecha o arquivo após salvar

    printf("Pontuação salva com sucesso!\n");
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

    inputPlayerName(score); // Chama a função para permitir que o jogador insira seu nome

    while (!keyhit())
    {
    } // Aguarda até que uma tecla seja pressionada
    readch(); // Lê a tecla pressionada
}

int screenDrawBorders();

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
        if (keyhit())
        {                          // Verifica se uma tecla foi pressionada
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
    playerY = SCREEN_HEIGHT - 4; // Ajustado para acomodar o sprite
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
            invaderPosX[i][j] = 5 + j * 8; // Define a posição X do invasor
            invaderPosY[i][j] = 3 + i * 4; // Define a posição Y do invasor
        }
    }
}

void drawPlayer()
{
    for (int i = 0; i < 4; i++)
    {                                       // Itera sobre as linhas do sprite do jogador
        screenGotoxy(playerX, playerY + i); // Move o cursor para a posição do jogador
        printf("%s", PLAYER_SPRITE[i]);     // Desenha a linha do sprite do jogador na tela
    }
}

void drawInvaders()
{
    for (int i = 0; i < 5; i++)
    { // Itera sobre as linhas de invasores
        for (int j = 0; j < 10; j++)
        { // Itera sobre as colunas de invasores
            if (invaders[i][j] == 1)
            { // Verifica se o invasor está ativo
                for (int k = 0; k < 4; k++)
                {                                                           // Itera sobre as linhas do sprite do invasor
                    screenGotoxy(invaderPosX[i][j], invaderPosY[i][j] + k); // Move o cursor para a posição do invasor
                    printf("%s", INVADER_SPRITE[k]);                        // Desenha a linha do sprite do invasor na tela
                }
            }
        }
    }
}

void drawBullet()
{
    if (bulletY > 0)
    {                                   // Verifica se a bala do jogador está ativa
        screenGotoxy(bulletX, bulletY); // Move o cursor para a posição da bala do jogador
        printf("%s", BULLET_SPRITE);    // Desenha o símbolo da bala do jogador na tela
    }
}

void drawAlienBullet()
{
    if (alienBulletY > 0)
    {                                             // Verifica se a bala dos invasores está ativa
        screenGotoxy(alienBulletX, alienBulletY); // Move o cursor para a posição da bala dos invasores
        printf("%s", ALIEN_BULLET_SPRITE);        // Desenha o símbolo da bala dos invasores na tela
    }
}

void updateBullet()
{
    if (bulletY > 0)
    {                 // Verifica se a bala do jogador está ativa
        bulletY -= 2; // Move a bala para cima
        if (bulletY == 0)
            bulletX = bulletY = -1; // Desativa a bala se ela sair da tela
    }
}

void updateAlienBullet()
{
    if (alienBulletY > 0)
    {                      // Verifica se a bala dos invasores está ativa
        alienBulletY += 2; // Move a bala para baixo
        if (alienBulletY >= playerY && alienBulletX == playerX)
        {               // Verifica se a bala atingiu o jogador
            gameOver(); // Termina o jogo se a bala atingir o jogador
        }
        if (alienBulletY >= SCREEN_HEIGHT)
            alienBulletY = -1; // Desativa a bala se ela sair da tela
    }
}

void shootBullet()
{
    if (bulletY < 0)
    {                          // Verifica se a bala do jogador está inativa
        bulletX = playerX + 3; // Ajustado para centralizar a bala
        bulletY = playerY - 1; // Define a posição Y da bala logo acima do jogador
    }
}

void alienShoot()
{
    for (int i = 4; i >= 0; i--)
    { // Itera sobre as linhas de invasores de baixo para cima
        for (int j = 0; j < 10; j++)
        { // Itera sobre as colunas de invasores
            if (invaders[i][j] == 1)
            {                                         // Verifica se o invasor está ativo
                alienBulletX = invaderPosX[i][j] + 3; // Ajustado para centralizar
                alienBulletY = invaderPosY[i][j] + 4; // Define a posição Y da bala logo abaixo do invasor
                return;                               // Sai da função após disparar a bala
            }
        }
    }
}

void movePlayer(int direction)
{
    for (int i = 0; i < 4; i++)
    {                                       // Itera sobre as linhas do sprite do jogador
        screenGotoxy(playerX, playerY + i); // Move o cursor para a posição do jogador
        printf("       ");                  // Apaga a linha do sprite do jogador na posição atual
    }
    playerX += direction * 4; // Ajusta o movimento para o sprite largo
    if (playerX < 1)
        playerX = 1; // Garante que o jogador não saia da borda esquerda da tela
    if (playerX > SCREEN_WIDTH - 7)
        playerX = SCREEN_WIDTH - 7; // Garante que o jogador não saia da borda direita da tela
}

void updateInvaders()
{
    static int direction = 1; // Direção de movimento dos invasores (1 para direita, -1 para esquerda)
    static int stepDown = 0;  // Flag para indicar se os invasores devem descer uma linha

    if (stepDown)
    { // Se os invasores devem descer uma linha
        for (int i = 0; i < 5; i++)
        { // Itera sobre as linhas de invasores
            for (int j = 0; j < 10; j++)
            {                           // Itera sobre as colunas de invasores
                invaderPosY[i][j] += 1; // Move o invasor para baixo
                if (invaderPosY[i][j] >= playerY - 4)
                {               // Verifica se algum invasor atingiu a linha do jogador
                    gameOver(); // Termina o jogo se um invasor atingir a linha do jogador
                }
            }
        }
        stepDown = 0; // Reseta a flag após mover os invasores para baixo
    }
    else
    {
        int atEdge = 0; // Flag para indicar se algum invasor atingiu a borda da tela
        for (int i = 0; i < 5 && !atEdge; i++)
        {                                                      // Itera sobre as linhas de invasores
            if ((invaderPosX[i][0] <= 1 && direction == -1) || // Verifica se algum invasor atingiu a borda esquerda
                (invaderPosX[i][9] >= SCREEN_WIDTH - 2 && direction == 1))
            {                    // Verifica se algum invasor atingiu a borda direita
                atEdge = 1;      // Define a flag indicando que um invasor atingiu a borda
                direction *= -1; // Inverte a direção de movimento dos invasores
                stepDown = 1;    // Define a flag para mover os invasores para baixo na próxima atualização
            }
        }
        if (!atEdge)
        { // Se nenhum invasor atingiu a borda
            for (int i = 0; i < 5; i++)
            { // Itera sobre as linhas de invasores
                for (int j = 0; j < 10; j++)
                {                                   // Itera sobre as colunas de invasores
                    invaderPosX[i][j] += direction; // Move o invasor na direção atual
                }
            }
        }
    }
}

void gameVictory()
{
    screenClear();                                         // Limpa a tela
    drawBorders();                                         // Desenha as bordas
    screenGotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2); // Centraliza a mensagem
    printf("VOCÊ VENCEU!\n SCORE: %d\n\n (Clique em qualquer tecla para continuar)", score);             // Exibe a mensagem de vitória e a pontuação

    // Aguarda o jogador pressionar uma tecla para continuar
    while (!keyhit())
    {
    } // Espera até que uma tecla seja pressionada
    readch(); // Captura a tecla pressionada para sair do loop

    // Após a tecla ser pressionada, o jogador será direcionado para o scoreboard
    inputPlayerName(score); // Função para permitir que o jogador insira seu nome
}

void checkCollisions()
{
    int remainingInvaders = 0; // Contador de invasores restantes

    for (int i = 0; i < 5; i++)
    { // Itera sobre as linhas de invasores
        for (int j = 0; j < 10; j++)
        { // Itera sobre as colunas de invasores
            if (invaders[i][j] == 1)
            {                        // Verifica se o invasor está ativo
                remainingInvaders++; // Incrementa se houver um invasor ativo

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

    if (remainingInvaders == 0)
    {              // Se não houver mais invasores
        gameVictory(); // Exibe a tela de vitória
    }
}

void gameLoop()
{
    int ch; // Variável para armazenar a tecla pressionada
    while (1)
    { // Loop principal do jogo
        if (timerTimeOver())
        {                        // Verifica se o tempo do temporizador acabou
            screenClear();       // Limpa a tela
            drawBorders();       // Desenha as bordas da tela
            drawPlayer();        // Desenha o jogador
            drawInvaders();      // Desenha os invasores
            drawBullet();        // Desenha a bala do jogador
            drawAlienBullet();   // Desenha a bala dos invasores
            updateBullet();      // Atualiza a posição da bala do jogador
            updateAlienBullet(); // Atualiza a posição da bala dos invasores
            updateInvaders();    // Atualiza a posição dos invasores
            checkCollisions();   // Verifica colisões entre balas e invasores
            displayScore();      // Exibe a pontuação do jogador
            if (rand() % 20 == 0)
                alienShoot(); // Faz com que os invasores atirem aleatoriamente
            screenUpdate();   // Atualiza a tela para exibir as mudanças
        }

        if (keyhit())
        {                  // Verifica se uma tecla foi pressionada
            ch = readch(); // Lê a tecla pressionada
            if (ch == 'a')
                movePlayer(-1); // Move o jogador para a esquerda
            else if (ch == 'd')
                movePlayer(1); // Move o jogador para a direita
            else if (ch == 'w')
                shootBullet(); // Dispara uma bala
            else if (ch == 'q')
                break; // Sai do loop e termina o jogo
        }
    }
    gameOver(); // Exibe a tela de fim de jogo
}

int main()
{
    if (mainMenu())
    {               // Exibe o menu principal e verifica se o jogador escolheu iniciar o jogo
        initGame(); // Inicializa o jogo
        gameLoop(); // Executa o loop principal do jogo
    }
    keyboardDestroy(); // Destrói a configuração do teclado
    timerDestroy();    // Destrói a configuração do temporizador
    screenDestroy();   // Destrói a configuração da tela
    return 0;          // Retorna 0 para indicar que o programa terminou com sucesso
}
