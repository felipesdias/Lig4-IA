
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <algorithm>
#include <vector>
#include <queue>
#include <iomanip>
#include "Jogada_t.h"
using namespace std;


/**-------- VARI�VEIS GLOBAIS --------**/

int tabuleiro[8][9];   // Tabuleiro para guardar as jogadas.
int linha[8];   // Vetor que indica quantas pe�as (valor) podem ser jogadas em cada coluna (�ndice).
int numero_simulacoes;   // M�ximo de simula��es que podem ser feitas pelas heur�sticas.
clock_t clock_inicial;   // Vari�vel para marcar o tempo.



/**----------- CABE�ALHOS ------------**/

int menuInicial();
void limpaTela();
void cabecalho(int opcao, string nome_player_1, string nome_player_2);
void imprimeTabuleiro(int tabuleiro_aux[8][9]);   // Imprime o tabuleiro na tela.

void inicializaVariaveis(int tabuleiro_aux[8][9], int linhas_aux[8], double &tempo_aux1, double &tempo_aux2, int opcao, string &nome_player_1, string &nome_player_2, int &heuristica_computador_1, int &heuristica_computador_2, bool &poda_alpha_beta_computador_1, bool &poda_alpha_beta_computador_2, int &simulacoes_computador_1, int &simulacoes_computador_2); // Inicializa as vari�veis.
bool empate(int linhas_aux[8]);   // Verifica se o jogo empatou.
Jogada_t minMax(Jogada_t jogada, int profundidade, double tempo_atual, double tempo_limite, int heuristica, int numero_simulacoes, bool poda_alpha_beta, int poda);   // Algoritmo minMax.
int monteCarlo(Jogada_t jogada_inicial);   // Heur�stica de Monte Carlo.


/**-------------- MAIN ---------------**/

int main() {
    //system("start musica.mp3");

	//Vari�veis dos Jogadores
    int coluna, opcao; 
    string nome_player_1, nome_player_2;
    
    //Variaveis dos computadores
    double tempo_limite_computador_1, tempo_limite_computador_2, tempo; //Define tempo limite dos jogadores
    int heuristica_computador_1, heuristica_computador_2; //Define heuristica a ser utilizada
    int simulacoes_computador_1, simulacoes_computador_2; //Define nume de simula��es a ser utilizada na heuristica
    bool poda_alpha_beta_computador_1, poda_alpha_beta_computador_2; //Define se usa Poda Alpha-Beta

    srand(time(NULL)); //Inicializa semente
    

    do { // Repete enquanto quer jogar
        Jogada_t jogada(0, 0, 0, 0); //Cria jogada
        opcao = menuInicial(); //Recebe modo de jogo

		//Configura estado inicial da jogada
        if(opcao == 1 || opcao == 4) {
            jogada.setJogador(1);
            jogada.setNumero(1);
        } else if(opcao == 2 || opcao == 3)
            jogada.setJogador(-1);
        else {
            cout << "Tremeu nas base, vlw, flws\n";
            return 0;
        }

		//Configura jogo
        inicializaVariaveis(tabuleiro, linha, tempo_limite_computador_1, tempo_limite_computador_2, opcao, nome_player_1, nome_player_2, heuristica_computador_1, heuristica_computador_2, poda_alpha_beta_computador_1, poda_alpha_beta_computador_2, simulacoes_computador_1, simulacoes_computador_2);
        
        //Imprime tela
        limpaTela();
        cabecalho(opcao, nome_player_1, nome_player_2);
        imprimeTabuleiro(tabuleiro);

		// Enquanto puder jogar
        for(int it=1; it<=42; it+=2) { 
			//Se existe jogador 1, ele joga
            if(opcao == 1 || opcao == 4) { 
				//L� uma coluna at� que ela seja v�lida
                do { 
                    cout << "Vez de " << nome_player_1 << ": ";
                    cin >> coluna;
                    if(coluna < 1 || coluna > 7 || linha[coluna] < 1)
                        cout << "Numero invalido.\n";
                } while(coluna < 1 || coluna > 7 || linha[coluna] < 1);

				//Configura a jogada
                jogada.setI(linha[coluna]);
                jogada.setJ(coluna);

				//Faz a jogada
                jogada.fazJogada(tabuleiro, linha);

				//Imprime tela
                limpaTela();
                cabecalho(opcao, nome_player_1, nome_player_2);
                imprimeTabuleiro(tabuleiro);

				//Verifica se jogador 1 venceu
                if(jogada.vitoria(tabuleiro)) { 
                    cout << nome_player_1 << " venceu!\n\n";
                    break;
                }

				//Passa a jogada para um jogador ou para um computador
                if(opcao == 4)
                    jogada++; //Para um jogador
                else
                    cout << "Computador 2 jogando...\n"; //Para um computador

            } 
            else { //Se n�o, � um computador, logo ele joga
                
                Jogada_t jogada_aux(jogada); //Jogada auxiliar para n�o ter perda de valores
                jogada.setAlpha(INT_MIN); //Computador 1 deseja maximizar o alpha
                clock_inicial = clock(); //Inicializa clock
                tempo = double(clock()-clock_inicial)/CLOCKS_PER_SEC;//Come�a contar o tempo

				//Aumenta a profundidade enquanto tiver tempo e for possivel aumentar profundidade
                for(int profundidade=1; tempo < tempo_limite_computador_1 && profundidade <= 43-jogada_aux.getNumero(); profundidade++) {
                    //Recebe melhor jogada, de acordo com as configura��es estabelecidas
                    jogada_aux.setJ(0);
                    jogada = max(jogada, minMax(jogada_aux, profundidade, tempo, tempo_limite_computador_1, heuristica_computador_1, simulacoes_computador_1, poda_alpha_beta_computador_1, INT_MAX));
                    
                    //Atualiza o tempo
                    tempo = double(clock()-clock_inicial)/CLOCKS_PER_SEC;
                }

				//Aplica a jogada
                jogada.fazJogada(tabuleiro, linha);

				//Imprime tela
                limpaTela();
                cabecalho(opcao, nome_player_1, nome_player_2);
                imprimeTabuleiro(tabuleiro);

				//Verifica se Computador 1 venceu
                if(jogada.vitoria(tabuleiro)) {
                    cout << "Computador 1 venceu!\n\n";
                    break;
                }

				//Passa jogada para um Jogador ou um Computador
                if(opcao == 2)
                    jogada++; //Para um jogador
                else
                    cout << "Computador 2 jogando...\n"; //Para um computador
            }

            if(opcao == 2 || opcao == 4) { //An�logo para a parte superior, com Jogador 2 e Computador 2
                do {
                    cout << "Vez de " << nome_player_2 << ": ";
                    cin >> coluna;
                    if(coluna < 1 || coluna > 7 || linha[coluna] < 1)
                        cout << "Numero invalido.\n";
                } while(coluna < 1 || coluna > 7 || linha[coluna] < 1);

                jogada.setI(linha[coluna]);
                jogada.setJ(coluna);

                jogada.fazJogada(tabuleiro, linha);

                limpaTela();
                cabecalho(opcao, nome_player_1, nome_player_2);
                imprimeTabuleiro(tabuleiro);

                if(jogada.vitoria(tabuleiro)) {
                    cout << nome_player_2 << " venceu!\n\n";
                    break;
                }

                if(opcao == 4)
                    jogada++;
                else
                    cout << "Computador 1 jogando...\n";

            } else {
                Jogada_t jogada_aux(jogada);
                jogada.setAlpha(INT_MAX);
                clock_inicial = clock();
                tempo = double(clock()-clock_inicial)/CLOCKS_PER_SEC;

                for(int profundidade=1; tempo < tempo_limite_computador_2 && profundidade <= 43-jogada_aux.getNumero(); profundidade++) {
                    jogada_aux.setJ(0);
                    jogada = min(jogada, minMax(jogada_aux, profundidade, tempo, tempo_limite_computador_2, heuristica_computador_2, simulacoes_computador_2, poda_alpha_beta_computador_2, INT_MIN));
                    tempo = double(clock()-clock_inicial)/CLOCKS_PER_SEC;
                }

                jogada.fazJogada(tabuleiro, linha);

                limpaTela();
                cabecalho(opcao, nome_player_1, nome_player_2);
                imprimeTabuleiro(tabuleiro);

                if(jogada.vitoria(tabuleiro)) {
                    cout << "Computador 2 venceu!\n\n";
                    break;
                }

                if(opcao == 1)
                    jogada++;
                else
                    cout << "Computador 1 jogando...\n";
            }
        }

		//Verifica se o jogo empatou
        if(empate(linha)) {
            limpaTela();
            cabecalho(opcao, nome_player_1, nome_player_2);
            imprimeTabuleiro(tabuleiro);
            cout << "Empate!\n\n";
        }

		//Verifica se quer jogar de novo
		cout << "Digite 1 para sair, ou qualquer outro numero para jogar novamente: ";
		cin >> opcao;
		
		limpaTela();
    } while (opcao != 1);

    return 0;
}



/**------------- FUN��ES -------------**/


int menuInicial() {
    int opcao;
    // Imprime Menu
    cout << "1 - Jogador vs Computador" << endl;
    cout << "2 - Computador vs Jogador" << endl;
    cout << "3 - Computador vs Computador" << endl;
    cout << "4 - Jogador vs Jogador" << endl;
    cout << "5 - Sair" << endl;
    // L� e trata op��o
    do {
        cout << endl << "Digite uma opcao: ";
        cin >> opcao;

        if(opcao < 1 || opcao > 5)
            cout << "Opcao invalidade" << endl;
    } while(opcao < 1 || opcao > 5);
    cout << endl;

    return opcao; // Retorna op��o
}

void limpaTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void cabecalho(int opcao, string nome_player_1, string nome_player_2) {
    //Imprime Cabe�alho
    cout << "==========================================" << endl;
    cout << "||           Modo de jogo: " << opcao << "            ||" << endl;
    switch(opcao) {
        case 1:
            cout << "|| " << setw(9) << nome_player_1 << " (X)  vs  Computador 2 (O)  ||" << endl;
            break;
        case 2:
            cout << "|| Computador 1 (X)  vs  " << setw(9) << nome_player_2 << " (O)  ||" << endl;
            break;
        case 3:
            cout << "|| Computador 1 (X) vs Computador 2 (O) ||" << endl;
            break;
        case 4:
            cout << "|| " << setw(9) << nome_player_1 << " (X)    vs    " << setw(9) << nome_player_2 << " (O) ||" << endl;
            break;
    }
    cout << "==========================================" << endl << endl;
}

void imprimeTabuleiro(int tabuleiro_aux[8][9]) {
	cout << "\t";
	for(int i=1; i<=7; i++)
		cout << i << " ";
	cout << "-  colunas\n";

    cout << "\t";
	for(int i=1; i<8; i++)
        cout << "- ";
	cout << endl;

	for(int i=1; i<=6; i++) {
        cout << "\t";
		for(int j=1; j<=7; j++) {
			if(tabuleiro_aux[i][j] == -1)
				cout << "O ";
            else if(tabuleiro_aux[i][j] == 1)
                cout << "X ";
			else
				cout << "  ";
		}
		cout << endl;
	}

    cout << "\t";
	for(int i=1; i<8; i++)
        cout << "- ";
    cout << endl;

    cout << "\t";
	for(int i=1; i<=7; i++)
		cout << i << " ";
	cout << "-  colunas\n\n";
}

void inicializaVariaveis(int tabuleiro_aux[8][9], int linhas_aux[8], double &tempo_aux1, double &tempo_aux2, int opcao, string &nome_player_1, string &nome_player_2, int &heuristica_computador_1, int &heuristica_computador_2, bool &poda_alpha_beta_computador_1, bool &poda_alpha_beta_computador_2, int &simulacoes_computador_1, int &simulacoes_computador_2) {
    limpaTela();

    // Se op��o for 2 ou 3, ent�o computador 1 participa do jogo
    if(opcao == 2 || opcao == 3) {
        cout << "Qual o tempo maximo (em segundos) que o computador 1 pode gastar por jogada? ";
        cin >> tempo_aux1;
        cout << "Qual heuristica usada para o computador 1 (0 - MonteCarlo, 1 - Otimizada): ";
        cin >> heuristica_computador_1;
        cout << "Qual numero de simulacoes para o computador 1: ";
        cin >> simulacoes_computador_1;
        cout << "Deseja usar a poda Alpha-Beta para computador 1: (0 - Nao, 1 - Sim): ";
        cin >> poda_alpha_beta_computador_1;
        cout << endl;
    }

    // Se op��o for 3 ou 1, ent�o computador 2 participa do jogo
    if(opcao == 3 || opcao == 1) {
        cout << "Qual o tempo maximo (em segundos) que o computador 2 pode gastar por jogada? ";
        cin >> tempo_aux2;
        cout << "Qual heuristica usada para o computador 2 (0 - MonteCarlo, 1 - Otimizada): ";
        cin >> heuristica_computador_2;
        cout << "Qual numero de simulacoes para o computador 2: ";
        cin >> simulacoes_computador_2;
        cout << "Deseja usar a poda Alpha-Beta para computador 2: (0 - Nao, 1 - Sim): ";
        cin >> poda_alpha_beta_computador_2;
        cout << endl;
    }

    // Se op��o for 1 ou 4, ent�o jogador 1 participa do jogo
    if(opcao == 1 || opcao == 4) {
        do {
            cout << "Digite o nome do jogador 1 (Max: 9 caracteres): ";
            cin >> nome_player_1;
        } while(nome_player_1.size() > 9);
        cout << endl;
    }

    // Se op��o for 2 ou 4, ent�o jogador 2 participa do jogo
    if(opcao == 2 || opcao == 4) {
        do {
            cout << "Digite o nome do jogador 2 (Max: 9 caracteres): ";
            cin >> nome_player_2;
        } while(nome_player_2.size() > 9);
        cout << endl;
    }

    //Cerca matriz do tabuleiro e inicializa a posi��o das colunas
	for(int i=0; i<8; i++)
		tabuleiro_aux[i][0] = tabuleiro_aux[i][8] = 4;

	for(int i=0; i<9; i++)
		tabuleiro_aux[0][i] = tabuleiro_aux[7][i] = 4;

	for(int i=1; i<=6; i++)
		for(int j=1; j<=7; j++) {
			tabuleiro_aux[i][j] = 0;
            linhas_aux[j] = 6;
		}
}

bool empate(int linhas_aux[8]) {
    for(int it=1; it<=7; it++)   // Para cada coluna
        if(linhas_aux[it] > 0)   //se existir alguma coluna que ainda n�o est� cheia
            return false;   //ainda n�o empatou

    return true;   // Caso contr�rio � um empate.
}

int monteCarlo(Jogada_t jogada_inicial, int numero_simulacoes) {
    int numero_vitorias = 0;   // Vari�vel que contabiliza o n�mero de vit�rias das simula��es.
	int linhas_aux[8];   // Vetor auxiliar para simular as jogadas.
	int tabuleiro_aux[8][9];   // Tabuleiro auxiliar para simular as jogadas.
	Jogada_t jogada;   // Jogada auxiliar

    for(int i=0; i<numero_simulacoes; i++) {    // La�o para realizar as simula��es
		for(int p=0; p<8; p++)   // Copia o vetor "linha" para o auxiliar
			linhas_aux[p] = linha[p];

		for(int p=0; p<8; p++)   // Copia o tabuleiro para o auxiliar
			for(int k=0; k<9; k++)
				tabuleiro_aux[p][k] = tabuleiro[p][k];

		jogada.setJogador(jogada_inicial.getJogador()*-1);   // Inverte o jogador para simular a jogada do oponente.

		for(int j=jogada_inicial.getNumero()+1; j<=42; j++) {    // Enquanto existirem jogadas v�lidas
            jogada.aleatoriaMonteCarlo(tabuleiro_aux, linhas_aux);   // Cria uma jogada aleat�ria

			if(jogada.vitoria(tabuleiro_aux)) {   // Se a jogada for vitoriosa
				if(jogada_inicial.getJogador() == jogada.getJogador())   //e for realizada pelo jogador inicial
					numero_vitorias++;   //significa que a vit�ria � dele.
				else   // Caso contr�rio
					numero_vitorias--;   //a vit�ria � do oponente.
				break;   // Termina a simula��o atual.
			}

			jogada.fazJogada(tabuleiro_aux, linhas_aux);   // Realiza a jogada no tabuleiro auxiliar.

            jogada.setJogador(jogada.getJogador()*-1);   // Inverte o jogador.
		}
    }

    return numero_vitorias;   // Retorna o numero de vit�rias menos o n�mero de derrotas.
}

int vouTeGanhei(Jogada_t jogada_inicial, int numero_simulacoes) { //An�logo ao monteCarlo
    int numero_vitorias = 0;
	int linhas_aux[8];
	int tabuleiro_aux[8][9];
	Jogada_t jogada;

    for(int i=0; i<numero_simulacoes; i++) {
		for(int p=0; p<8; p++)
			linhas_aux[p] = linha[p];

		for(int p=0; p<8; p++)
			for(int k=0; k<9; k++)
				tabuleiro_aux[p][k] = tabuleiro[p][k];

		jogada.setJogador(jogada_inicial.getJogador()*-1);

		for(int j=jogada_inicial.getNumero()+1; j<=42; j++) {
            jogada.aleatoriaVouTeGanhei(tabuleiro_aux, linhas_aux); //Diferen�a para o monteCarlo

			if(jogada.vitoria(tabuleiro_aux)) {
				if(jogada_inicial.getJogador() == jogada.getJogador())
					numero_vitorias++;
				else
					numero_vitorias--;
				break;
			}

			tabuleiro_aux[jogada.getI()][jogada.getJ()] = jogada.getJogador();
			linhas_aux[jogada.getJ()]--;

            jogada.setJogador(jogada.getJogador()*-1);
		}
    }

    return numero_vitorias;
}

Jogada_t minMax(Jogada_t jogada, int profundidade, double tempo_atual, double tempo_limite, int heuristica, int numero_simulacoes, bool poda_alpha_beta, int poda) {
    tempo_atual = double(clock()-clock_inicial)/CLOCKS_PER_SEC;

    if(jogada.getJ() != 0) {   // "jogada.getJ()" s� ser� 0 quando minMax for chamada da fun��o main();
        if(jogada.vitoria(tabuleiro)) {   // Se a jogada analisada for uma vit�ria
            jogada.setAlpha(jogada.getJogador()*(INT_MAX-jogada.getNumero()));   //retorna um valor m�ximo para o jogador 1 ou m�nimo para o jogador -1.
            return jogada;
        }

        if(profundidade == 0 || tempo_atual >= tempo_limite) {   // Se o aprofundamento iterativo chegou ao fim ou o tempo de busca esgotou
            if(heuristica == 0)
                jogada.setAlpha(jogada.getJogador()*monteCarlo(jogada, numero_simulacoes));   //retorna o valor heur�stico para o n�.
            else
                jogada.setAlpha(jogada.getJogador()*vouTeGanhei(jogada, numero_simulacoes));   //retorna o valor heur�stico (parte 3) para o n�.
            return jogada;
        }

        if(jogada.getNumero() == 42) {   // Se for a 42� jogada, significa que o jogo empatou.
            jogada.setAlpha(0);   // Retorna 0, que � pior que a vit�ria, mas melor que a derrota.
            return jogada;
        }
    }

    Jogada_t melhor_jogada;   // Objeto auxiliar para guardar a jogada com maior valor heur�stico.

    jogada++;   // Inverte o jogador e incrementa o n�mero da jogada, para calcular o valor de minMax para seus filhos.

    if(jogada.getJogador() == 1) {   // Caso esteja na vez do jogador 1, deseja-se receber o valor m�ximo
        melhor_jogada.setAlpha(INT_MIN);   // Inicializa-se o "alpha" da melhor jogada com menos infinito.

        for(int it=1; it<=7; it++) {   // Para cada coluna
            if(linha[it] > 0) {   //se ela n�o est� cheia
                jogada.setI(linha[it]);
                jogada.setJ(it);
                jogada.fazJogada(tabuleiro, linha);   //simula a jogada,

                jogada.setAlpha(minMax(jogada, profundidade-1, tempo_atual, tempo_limite, heuristica, numero_simulacoes, poda_alpha_beta, melhor_jogada.getAlpha()).getAlpha());   //calcula-se seu valor de minMax,

                jogada.desfazJogada(tabuleiro, linha);   //desfaz a jogada simulada,

                melhor_jogada = max(melhor_jogada, jogada);   // e atualiza a melhor jogada de acordo com a jogada simulada.

                if(poda_alpha_beta)
                    if(poda < melhor_jogada.getAlpha())
                        return melhor_jogada;
            }
        }
    }
    else {   // Funciona de modo an�logo para o jogador -1, entretanto deseja-se minimizar o valor de "alpha".
        melhor_jogada.setAlpha(INT_MAX);

        for(int it=1; it<=7; it++) {
            if(linha[it] > 0) {
                jogada.setI(linha[it]);
                jogada.setJ(it);
                jogada.fazJogada(tabuleiro, linha);

                jogada.setAlpha(minMax(jogada, profundidade-1, tempo_atual, tempo_limite, heuristica, numero_simulacoes, poda_alpha_beta, melhor_jogada.getAlpha()).getAlpha());

                jogada.desfazJogada(tabuleiro, linha);

                melhor_jogada = min(melhor_jogada, jogada);

                if(poda_alpha_beta)
                    if(poda > melhor_jogada.getAlpha())
                        return melhor_jogada;
            }
        }
    }

    return melhor_jogada;   // Caso o tempo n�o tenha se esgotado retorna a melhor jogada encontrada.
}
