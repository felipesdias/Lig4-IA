
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Jogada_t.h"
using namespace std;


/**---- CONSTRUTORES E DESTRUTOR -----**/

// Construtor padrao
Jogada_t::Jogada_t() {}

// Atribui ao novo objeto a posi��o (linha e coluna), o jogador e o n�mero da jogada.
Jogada_t::Jogada_t(int arg, int arg2, int arg3, int arg4) : i(arg), j(arg2), jogador(arg3), numero(arg4) {}

// Construtor de c�pia.
Jogada_t::Jogada_t(const Jogada_t &arg) : i(arg.getI()), j(arg.getJ()), jogador(arg.getJogador()), numero(arg.getNumero()), alpha(arg.getAlpha()) {}

// Destrutor
Jogada_t::~Jogada_t() {}



/**-------- M�TODOS IMPL�CITOS -------**/

bool Jogada_t::verificaVitoria(int tabuleiro_aux[8][9], int mov_i, int mov_j) {
    int cont, aux_i, aux_j;

    // Inicia as vari�veis para percorrer o tabuleiro na dire��o dos par�metros.
    cont = 1;
    aux_i = this->getI() + mov_i;
    aux_j = this->getJ() + mov_j;

    // Percorre o tabuleiro.
    while(tabuleiro_aux[aux_i][aux_j] == this->getJogador()) {
        cont++;
        aux_i += mov_i;
        aux_j += mov_j;
    }

    // Se existem quatro pedras da mesma cor significa que a jogada foi vencedora.
    if(cont >= 4)
        return true;

    // Inicia as vari�veis para percorrer na dire��o oposta.
    aux_i = this->getI() - mov_i;
    aux_j = this->getJ() - mov_j;

    // Percorre o tabuleiro na dire��o oposta.
    while(tabuleiro_aux[aux_i][aux_j] == this->getJogador()) {
        cont++;
        aux_i -= mov_i;
        aux_j -= mov_j;
    }

    // Se existem quatro pedras da mesma cor significa que a jogada foi vencedora.
    if(cont >= 4)
        return true;

    // Se nenhuma das condi��es anteriores foram satisfeitas significa que a jogada n�o foi vencedora.
    return false;
}



/**------------- SETTERS -------------**/

void Jogada_t::setI(int arg) {
    this->i = arg;
}

void Jogada_t::setJ(int arg) {
    this->j = arg;
}

void Jogada_t::setJogador(int arg) {
    this->jogador = arg;
}

void Jogada_t::setNumero(int arg) {
    this->numero = arg;
}

void Jogada_t::setAlpha(int arg) {
    this->alpha = arg;
}



/**------------- GETTERS -------------**/

int Jogada_t::getI() const {
    return this->i;
}

int Jogada_t::getJ() const {
    return this->j;
}
int Jogada_t::getJogador() const {
    return this->jogador;
}

int Jogada_t::getNumero() const {
    return this->numero;
}

int Jogada_t::getAlpha() const {
    return this->alpha;
}



/**--------- DEMAIS M�TODOS ----------**/

void Jogada_t::fazJogada(int tabuleiro_aux[8][9], int linhas_aux[8]) {
    tabuleiro_aux[this->getI()][this->getJ()] = this->getJogador();   // Atribui a jogada ao tabuleiro.
    linhas_aux[this->getJ()]--;   // Decrementa o n�mero de jogadas poss�veis na coluna j.
}

void Jogada_t::desfazJogada(int tabuleiro_aux[8][9], int linhas_aux[8]) {
    tabuleiro_aux[this->getI()][this->getJ()] = 0;   // Retira o jogador da posi��o i, j.
    linhas_aux[this->getJ()]++;   // Incrementa o n�mero de jogadas poss�veis na coluna j.
}

// Informa se a jogada � uma vit�ria ou n�o.
bool Jogada_t::vitoria(int tabuleiro_aux[8][9]) {
	//Verifica��o Horizontal
    if(this->verificaVitoria(tabuleiro_aux, 0, -1))
        return true;

	//Verifica��o Vertical
    if(this->verificaVitoria(tabuleiro_aux, -1, 0))
        return true;

	//Verifica��o Diagonal Principal
    if(this->verificaVitoria(tabuleiro_aux, -1, -1))
        return true;

	//Verifica��o Diagonal Secund�ria
    if(this->verificaVitoria(tabuleiro_aux, -1, 1))
        return true;

    return false;
}

void Jogada_t::aleatoriaMonteCarlo(int tabuleiro_aux[8][9], int linhas_aux[8]) {
    vector<int> candidatos;   // Vetor para armazenar as posi��es poss�veis para a jogada.

    for(int it=1; it<=7; it++) {   // Para cada uma das colunas
        if(linhas_aux[it] > 0) {   // Se a coluna n�o est� cheia
            this->setJ(it);
            this->setI(linhas_aux[it]);

            candidatos.push_back(it);   // Ela � uma poss�vel candidata para a jogada.
        }
    }

    this->setJ(candidatos[rand()%candidatos.size()]);   // Escolhe uma coluna aleat�ria entre as candidatas.
    this->setI(linhas_aux[this->getJ()]);
}

void Jogada_t::aleatoriaVouTeGanhei(int tabuleiro_aux[8][9], int linhas_aux[8]) {
    Jogada_t jogada(*this);
    jogada.setJogador(jogada.getJogador()*-1);
    vector<int> candidatos;

    for(int it=1; it<=7; it++) {   // La�o para verificar se alguma das pr�ximas sete jogadas � uma vit�ria.
        if(linhas_aux[it] > 0) {
            this->setJ(it);
            this->setI(linhas_aux[it]);

            if(this->vitoria(tabuleiro_aux))   // Se for uma vit�ria, � a jogada �tima, portanto deve ser escolhida.
                return;
        }
    }

    // Se ainda n�o tivermos uma jogada vencedora:
    for(int it=1; it<=7; it++) {   // La�o para verificar se alguma das pr�ximas sete jogadas d�o a vit�ria ao oponente.
        if(linhas_aux[it] > 0) {
            this->setJ(it);
            this->setI(linhas_aux[it]);

            jogada.setJ(it);
            jogada.setI(linhas_aux[it]);

            if(jogada.vitoria(tabuleiro_aux))   // Em caso afirmativo, essa jogada deve ser bloqueada.
                return;
        }
    }

    // Se o oponente tamb�m n�o puder vencer na pr�xima jogada:
    for(int it=1; it<=7; it++) {   // La�o para gerar uma jogada aleat�ria.
        if(linhas_aux[it] > 0)
            candidatos.push_back(it);
    }

    this->setJ(candidatos[rand()%candidatos.size()]);   // Escolhe uma coluna aleat�ria entre as poss�veis.
    this->setI(linhas_aux[this->getJ()]);
}



/**------------ OPERADORES -----------**/

// Copia os elementos do par�metro para si.
void Jogada_t::operator = (const Jogada_t &arg) {
    this->setI(arg.getI());
    this->setJ(arg.getJ());
    this->setJogador(arg.getJogador());
    this->setNumero(arg.getNumero());
    this->setAlpha(arg.getAlpha());
}

// Retorna true caso o "alpha" seja menor que o "alpha" do par�metro.
bool Jogada_t::operator < (const Jogada_t &arg) const {
    return this->getAlpha() < arg.getAlpha();
}

// Incrementa o n�mero da jogada e inverte o jogador.
void Jogada_t::operator ++ (int) {
    this->setJogador(this->getJogador()*-1);
    this->setNumero(this->getNumero()+1);
}

// Decrementa o n�mero da jogada e inverte o jogador.
void Jogada_t::operator -- (int) {
    this->setJogador(this->getJogador()*-1);
    this->setNumero(this->getNumero()-1);
}

// Imprime os atributos do objeto.
ostream& operator << (ostream& os, const Jogada_t& arg) {
    return os << "I: " << arg.getI() << " J: " << arg.getJ() << " JOG: " << arg.getJogador() << " NUM: " << arg.getNumero() << " ALP: " << arg.getAlpha() << std::endl;
}
