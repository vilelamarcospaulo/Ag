#include "individuo.h"

int individuo::tamanho = 8;
int individuo::tamanho_real = 10;
int individuo::aptidao_maxima = 100000;
int individuo::probabilidade_mutacao = 30;

char individuo::letras[8] = { 'S', 'E', 'N', 'D', 'M','O','R','Y' };

individuo::individuo() 
{
	this->valores = new int[this->tamanho_real];
	for (int i = 0; i < individuo::tamanho_real; i++)
		this->valores[i] = 0;
}

individuo::individuo(individuo* origem)
{ 
	this->valores = new int[this->tamanho_real];

	for (int i = 0; i < individuo::tamanho_real; i++)
		this->valores[i] = origem->valores[i];
}

int individuo::aptidao()
{
	if (this->_aptidao == -1)
	{
		int send = this->valores[0] * 1000 + this->valores[1] * 100 + this->valores[2] * 10 + this->valores[3];
		int more = this->valores[4] * 1000 + this->valores[5] * 100 + this->valores[6] * 10 + this->valores[1];
		int money = this->valores[4] * 10000 + this->valores[5] * 1000 + this->valores[2] * 100 + this->valores[1] * 10 + this->valores[7];

		this->_aptidao = std::abs((send + more) - money);
	}
	return _aptidao;
}

void individuo::mutacao()
{
	std::minstd_rand0 random(std::chrono::system_clock::now().time_since_epoch().count());

	int dice = random() % 100;
	if (dice >= this->probabilidade_mutacao)
		return;

	for (int i = 0; i < 2; i++) {
		int posA = random() % individuo::tamanho_real;
		int posB = random() % individuo::tamanho_real;

		for (; posA == posB; posB = random() % individuo::tamanho_real);

		//swap
		int aux = this->valores[posA];
		this->valores[posA] = this->valores[posB];
		this->valores[posB] = aux;
	}
}

std::string individuo::toString()
{
	std::string str = "|";
	for (int i = 0; i < this->tamanho; i++)
		str += std::to_string(this->valores[i]) + "|";

	str += " = " + std::to_string(this->aptidao());
	return str;
}

individuo* individuo::IndividuoAleatorio()
{
	std::minstd_rand0 random(std::chrono::system_clock::now().time_since_epoch().count());

	individuo *ind = new individuo();
	
	for (int i = 0; i < individuo::tamanho_real; i++){
		for (bool valor_unico = false; !valor_unico; )
		{
			valor_unico = true;
			ind->valores[i] = random() % 10; //atribui a posição um valor aleatorio entre 0 e 10

			for (int j = 0; j < i; j++) //Verifica se o numero ja se encontra no vetor
			{
				if (ind->valores[i] == ind->valores[j])
				{	
					valor_unico = false;
					break;
				}
			}
		}
	}

	return ind;
}

void individuo::crossover(individuo *individuo1, individuo *individuo2)
{
	std::minstd_rand0 random(std::chrono::system_clock::now().time_since_epoch().count());

	int pos = random() % 10; //Sorteia a posição inicial do crossover ciclico
	int valorI = individuo1->valores[pos]; //Valor de inicio do ciclo
	int valorA = individuo2->valores[pos]; // Valor atual;
	
	//std::cout << individuo1->toString() << std::endl << individuo2->toString() << std::endl;
	//std::cout << pos << std::endl;
	while (valorI != valorA)
	{
		valorA = individuo2->valores[pos];
		
		individuo2->valores[pos] = individuo1->valores[pos]; //SWAP 
		individuo1->valores[pos] = valorA;

		for (int i = 0; i < individuo::tamanho_real; i++)
		{
			if (i != pos && individuo1->valores[i] == valorA)
			{
				pos = i;
				break;
			}
		}
	}

	//std::cout << individuo1->toString() << std::endl << individuo2->toString() << std::endl;
}

bool operator<(individuo lhs, individuo rhs)
{
	return lhs.aptidao() < rhs.aptidao();
}

bool operator==(individuo lhs, individuo rhs)
{
	bool equal = true;
	for (int i = 0; i < individuo::tamanho_real; i++)
	{
		if (lhs.valores[i] != rhs.valores[i])
		{
			equal = false;
			break;
		}
	}

	return equal;
}

bool operator!=(individuo lhs, individuo rhs)
{
	return !(lhs == rhs);
}
