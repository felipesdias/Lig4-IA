
#ifndef JOGADA_T_H
#define JOGADA_T_H

#include <iostream>

class Jogada_t {
private:
    // Atributos
    int i, j;   // Posi��o da jogada no tabuleiro (1 <= i <= 7, 1 <= j <= 8).
    int jogador;   // N�mero que indica qual jogador fez a jogada (1 ou -1).
    int numero;   // N�mero da jogada.
    int alpha;   // Valor utilizado pelo algoritmo minMax para avaliar a jogada.

    // M�todos Impl�citos
    bool verificaVitoria(int tabuleiro_aux[8][9], int mov_i, int mov_j);    // M�todo chamado pelo m�todo p�blico "vitoria" para avaliar se a jogada foi vencedora.

public:
    // Construtores e Destrutor
    Jogada_t();   // Construtor padr�o.
    Jogada_t(int arg, int arg2, int arg3, int arg4);   // Construtor com par�metros para os atributos i, j, jogador e numero.
    Jogada_t(const Jogada_t &arg);   // Construtor de c�pia.
    ~Jogada_t();   // Destrutor.

    // Setters
    void setI(int arg);
    void setJ(int arg);
    void setJogador(int arg);
    void setNumero(int arg);
    void setAlpha(int arg);

    // Getters
    int getI() const;
    int getJ() const;
    int getJogador() const;
    int getNumero() const;
    int getAlpha() const;

    // Demais M�todos
    void fazJogada(int tabuleiro_aux[8][9], int linhas_aux[8]);   // Marca a jogada no tabuleiro.
    void desfazJogada(int tabuleiro_aux[8][9], int linhas_aux[8]);   // Retira a jogada do tabuleiro.
    bool vitoria(int tabuleiro_aux[8][9]);   // Chama o m�todo privado "verificaVitoria" com diferentes par�metros para checar as oito dire��es no tabuleiro.
	void aleatoriaMonteCarlo(int tabuleiro_aux[8][9], int linhas_aux[8]);   // Gera uma jogada alet�ria para a heur�stica de Monte Carlo.
	void aleatoriaVouTeGanhei(int tabuleiro_aux[8][9], int linhas_aux[8]);   // Gera uma jogada para nossa heur�stica.

    // Operadores
    void operator = (const Jogada_t &arg);   // Operador de atribui��o
	bool operator < (const Jogada_t &arg) const;   // Compara o atributo "alpha" entre dois objetos.
	void operator ++ (int);   // Incrementa o n�mero da jogada e inverte o jogador.
	void operator -- (int);   // Decrementa o n�mero da jogada e inverte o jogador.
	friend std::ostream& operator << (std::ostream &os, const Jogada_t& arg);   // Imprime os atributos do objeto.
};

#endif
