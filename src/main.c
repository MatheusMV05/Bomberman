#include <string.h>
#include <stdlib.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

//Criação da struct para o jogador
//A struct pode ser utilizada para dois
typedef struct player{
    int x;
    int y;
    char character[1]; //pode aumentar se for fazer multiplayer
}Player;


//definem as posições iniciais
//também os incrementos para movimentar nos eixos
//Testar depois com valores diferentes 
int x = 34, y = 12;
int incX = 1, incY = 1;

//A lógica do portrait serve para a moldura 
void portrait(){
    screenSetColor(RED, WHITE);
    //Os laços imprimem as linhas horizontais e verticais da moldura
    //respectivamente e seguem do 0 no eixo até o valor máximo do eixo
    for (int x = 0; x < MAXX; x++){
        screenGotoxy(x, 0);
        printf("-");
        screenGotoxy(x, MAXY);
        printf("-");
    }
    for (int y = 0; y < MAXY; y++){
        screenGotoxy(0, y);
        printf("|");
        screenGotoxy(MAXX, y);
        printf("|");
    }
    //Estes setam valores as pontas da moldura
    //MAXX e MAXY são as dimensões máximas da tela no X e Y
    screenGotoxy(0, 0); //canto superior esquerdo
    printf("@");
    screenGotoxy(MAXX, 0); //canto superior direito
    printf("@");
    screenGotoxy(0, MAXY); //canto inferior esquerdo
    printf("@");
    screenGotoxy(MAXX, MAXY); //canto inferior direito
    printf("@\n");

    //screenGotoxy: função que posiciona o cursor na posição
    //(x, y) na tela.
}

void game(){
    printf("Iniciar\n");
    //aqui o código do nosso game
}

void credits() {
    screenClear();
    portrait();
    
    screenGotoxy(MAXX / 2 - 10, MAXY / 2 - 4); 
    printf("Desenvolvedores:\n");
    screenGotoxy(MAXX / 2 - 10, MAXY / 2 - 2);
    printf("- Julia Torres\n");
    screenGotoxy(MAXX / 2 - 10, MAXY / 2 - 1);
    printf("- Matheus Martins\n");
    screenGotoxy(MAXX / 2 - 10, MAXY / 2);
    printf("- Vinícius Marques\n");

    screenGotoxy(MAXX / 2 - 10, MAXY / 2 + 2);
    printf("Mentores:\n");
    screenGotoxy(MAXX / 2 - 10, MAXY / 2 + 3);
    printf("- Pâmela Thays Lins\n");
    screenGotoxy(MAXX / 2 - 10, MAXY / 2 + 4);
    printf("- Thiago Guedes Ferreira\n");

    screenGotoxy(MAXX / 2 - 20, MAXY / 2 + 6); 
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    while (!keyhit()) {} // Espera uma tecla para retornar
    readch(); // Limpa a tecla do buffer
}

// Nosso menu tem somente 3 opções:
// -jogar
// -créditos
// -sair
// Também temos as instruções logo abaixo do menu
void menu() {
    int opcao = 1; // Começa na primeira opção
    int previous_option = 0; // Para verificar se a opção mudou
    screenClear();
    portrait(); // Desenha a moldura uma vez

    while (1) {
        // Exibe as opções do menu
        screenGotoxy(MAXX / 2 - 7, 3);
        printf("Menu Principal\n");

        // Opções do menu com indicador "->" à direita
        screenGotoxy(MAXX / 2 - 6, 5);
        printf("%sJogar\n", opcao == 1 ? "-> " : "   ");
        screenGotoxy(MAXX / 2 - 6, 7);
        printf("%sCréditos\n", opcao == 2 ? "-> " : "   ");
        screenGotoxy(MAXX / 2 - 6, 9);
        printf("%sSair\n", opcao == 3 ? "-> " : "   ");

        screenGotoxy(MAXX / 2 - 10, 13);
        printf("Use W e S para navegar");
        screenGotoxy(MAXX / 2 - 10, 15);
        printf("Enter para selecionar");

        // Verifica se há uma tecla pressionada
        if (keyhit()) {
            char ch = readch(); // Lê a tecla pressionada

            switch (ch) {
                case 'w':
                    opcao--;
                    if (opcao < 1) opcao = 3;  // Volta para a última opção
                    break;
                case 's':
                    opcao++;
                    if (opcao > 3) opcao = 1;  // Retorna para a primeira opção
                    break;
                case 'k':
                case '\n':
                case '\r':
                    switch (opcao) {
                        case 1:
                            game(); // Chama a função do jogo
                            break;
                        case 2:
                            credits(); // Chama a função de créditos
                            screenClear();
                            portrait();
                            break;
                        case 3:
                            printf("Saindo do jogo...\n");
                            exit(0); // Sai do programa
                    }
                    break;
            }
        }

        // Redesenhar apenas se a opção mudou
        if (opcao != previous_option) {
            // Limpa a área das opções
            screenGotoxy(MAXX / 2 - 6, 5);
            printf("      "); // Limpa a linha da opção 1
            screenGotoxy(MAXX / 2 - 6, 7);
            printf("      "); // Limpa a linha da opção 2
            screenGotoxy(MAXX / 2 - 6, 9);
            printf("      "); // Limpa a linha da opção 3

            // Desenha novamente as opções com o indicador correto
            screenGotoxy(MAXX / 2 - 6, 5);
            printf("%sJogar\n", opcao == 1 ? "-> " : "   ");
            screenGotoxy(MAXX / 2 - 6, 7);
            printf("%sCréditos\n", opcao == 2 ? "-> " : "   ");
            screenGotoxy(MAXX / 2 - 6, 9);
            printf("%sSair\n", opcao == 3 ? "-> " : "   ");

            previous_option = opcao; // Atualiza a opção anterior
        }
    }
}


int main() {
    printf("\e[?25l"); // Oculta o cursor
    menu();
    printf("\e[?25h"); // Mostra o cursor antes de sair
    return 0;

}


// gcc ./src/*.c -I./include -o bomberman