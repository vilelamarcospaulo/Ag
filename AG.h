#include "individuo.h"
#include <iostream>
#include <iomanip>
#include <random>

enum modo_selecao { _roleta, _torneio };
enum modo_reinsercao { _ordenacao, _elitismo };

class AG
{
private:
	std::minstd_rand0 random;
	
	modo_selecao mselecao;
	modo_reinsercao mreinsercao;

	int i_geracao;
	int n_geracoes;
	
	int tam_populacao;         //Tamanho da população inicial
	int n_filhos;			   //Tamanho da população dos filhos
	int probabilidade_mutacao; //

	int roleta(int sum);
	int torneio(int size);
	void selecao(individuo **pai1, individuo **pai2, int aptidao_total);

	void ordenacao();
	void elitismo(int porcentagem_elitismo);
	void reinsercao();

	individuo** populacao;  //populacao do AG
	
public:
	AG(int tam_populacao, int n_filhos, int n_geracoes, modo_selecao selecao, modo_reinsercao reinsercao); //constructor
	
	bool CondicaoParada();

	void gerarPopulacaoAleatoria();
	void run();
	void gerarNovaPopulacao();
	individuo melhorIndividuo();

	void printPopulacao();	
};
