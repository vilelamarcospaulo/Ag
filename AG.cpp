#include "AG.h"

int time_elapsed = 0;

AG::AG(int tam_populacao, int n_filhos, int n_geracoes, modo_selecao selecao, modo_reinsercao reinsercao)
{
	this->mselecao = selecao;
	this->mreinsercao = reinsercao;

	this->random = std::minstd_rand0(std::chrono::system_clock::now().time_since_epoch().count());

	this->n_geracoes = n_geracoes;
	this->tam_populacao = tam_populacao;
	this->n_filhos = n_filhos;
	this->probabilidade_mutacao = probabilidade_mutacao; 

	this->populacao = (individuo**)malloc((tam_populacao + n_filhos) * sizeof(individuo*));
	for (int i = 0; i < this->tam_populacao + this->n_filhos; i++)
		this->populacao[i] = NULL;
}

void AG::gerarPopulacaoAleatoria()
{
	if (tam_populacao <= 0)
		return;

	for (int i = 0; i < this->tam_populacao; i++)
		this->populacao[i] = individuo::IndividuoAleatorio();

	std::sort(this->populacao, this->populacao + this->tam_populacao, [](individuo* a, individuo* b) { return *a < *b; });
}

void AG::printPopulacao(){
	std::cout << "POPULACAO ATUAL" << std::endl;
	for (int i = 0; i < this->tam_populacao; i++)
		std::cout << " Individuo: " << i << " - " << this->populacao[i]->toString() << std::endl;
}

bool AG::CondicaoParada()
{
	this->i_geracao++;
	return this->melhorIndividuo().aptidao() == 0 || this->i_geracao == this->n_geracoes + 1;
}

int AG::roleta(int sum)
{
	int dice = random() % sum;

	sum = 0;
	for (int i = 0; i < this->tam_populacao; i++)
	{
		sum += (individuo::aptidao_maxima - this->populacao[i]->aptidao());
		if (dice < sum)
			return i;
	}
}

int AG::torneio(int size)
{
	std::clock_t start_aux = std::clock();

	int best;
	int *pos = new int[size];
	
	pos[0] = this->random() % this->tam_populacao; //sorteia um invidividuo
	best = pos[0];

	for (int i = 1; i < size; i++)
	{
		pos[i] = this->random() % this->tam_populacao;
		for (bool valor_unico = false; !valor_unico; )
		{
			valor_unico = true;
			pos[i] = this->random() % this->tam_populacao;

			for (int j = 0; j < i; j++) //Verifica se o numero ja se encontra no vetor
			{
				if (pos[i] == pos[j])
				{
					valor_unico = false;
					break;
				}
			}
		}

		if (*(this->populacao[pos[i]]) < *(this->populacao[best]))
			best = pos[i];
	}

	delete[] pos;
	
	return best;
}

void AG::selecao(individuo **pai1, individuo **pai2, int aptidao_total)
{
	int tour_size = 3;

	int posPai1 = 0;
	int posPai2 = 0;
	
	select:
	if (mselecao == modo_selecao::_roleta)
	{
		posPai1 = this->roleta(aptidao_total);
		posPai2 = this->roleta(aptidao_total);
	}												

	else if (mselecao == modo_selecao::_torneio)
	{
		posPai1 = this->torneio(tour_size);
		posPai2 = this->torneio(tour_size);
	}

	*pai1 = new individuo(this->populacao[posPai1]);
	*pai2 = new individuo(this->populacao[posPai2]);

	if (**pai1 == **pai2)
	{
		//goto select;
	}
}

void AG::ordenacao()
{
	std::sort(this->populacao, this->populacao + this->tam_populacao + this->n_filhos, [](individuo* a, individuo* b) {return *a < *b; });
}

void AG::elitismo(int porcentagem_elitismo)
{
	std::sort(this->populacao, this->populacao + this->tam_populacao, [](individuo* a, individuo* b) {return *a < *b; });  //ORDENA OS PAIS
	std::sort(this->populacao + this->tam_populacao, this->populacao + this->tam_populacao + this->n_filhos, [](individuo* a, individuo* b) {return *a < *b; });  //ORDENA OS FILHOS

	int posicao_filhos = ((porcentagem_elitismo * this->tam_populacao) / 100);
	int n_filhos_sobreviventes = this->tam_populacao - posicao_filhos;

	for (int i = 0; i < n_filhos_sobreviventes; i++)
	{
		delete(this->populacao[posicao_filhos + i]->valores);
		delete(this->populacao[posicao_filhos + i]);

		this->populacao[posicao_filhos + i] = this->populacao[this->tam_populacao + i];
		this->populacao[this->tam_populacao + i] = NULL;
	}
}

void AG::reinsercao()
{
	if (this->mreinsercao == modo_reinsercao::_ordenacao) {
		this->ordenacao();
	}

	else if (this->mreinsercao == modo_reinsercao::_elitismo) {
		this->elitismo(20);
	}
				
}

void AG::gerarNovaPopulacao()
{
	int sum = 0;
	if (this->mselecao == modo_selecao::_roleta)
	{
		for (int i = 0; i < this->tam_populacao; i++)
			sum += (individuo::aptidao_maxima - this->populacao[i]->aptidao());
	}

	for (int i = this->tam_populacao; i < this->tam_populacao + this->n_filhos; i+=2)
	{
		individuo *filho1 = NULL;
		individuo *filho2 = NULL;
		this->selecao(&filho1, &filho2, sum);
		
		individuo::crossover(filho1, filho2);

		filho1->mutacao();
		filho2->mutacao();
		
		//libera a memoria dos filhos
		if(this->populacao[i] != NULL)
			delete[] this->populacao[i]->valores;
		
		if (this->populacao[i + 1] != NULL)
			delete[] this->populacao[i + 1]->valores;

		delete(this->populacao[i]);
		delete(this->populacao[i+1]);
		
		this->populacao[i] = filho1;
		this->populacao[i + 1] = filho2;		
	}

	this->reinsercao();
}

void AG::run()
{
	this->i_geracao = 0;
	this->gerarPopulacaoAleatoria();
	
	while (!this->CondicaoParada())
		this->gerarNovaPopulacao();

	//std::cout << "\n\n" << time_elapsed << "ms \n\n";
}

individuo AG::melhorIndividuo()
{
	return this->populacao[0];
}