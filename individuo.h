#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>

class individuo
{
	private:
		static int tamanho_real; //tamanho real do vetor
		static int probabilidade_mutacao;
		
		int _aptidao = -1;
	public:
		individuo(); //Construtor
		individuo(individuo* original); //
		
		int* valores;  //Vetor com os valores 
 		
		int aptidao(); //Aptidao do individuo
		void mutacao(); //Faz a mutação em um invididuo

		std::string toString(); //Methodo gera uma string de representação do objeto

		static char letras[8];
		static int tamanho; //tamanho do vetor 
		static int aptidao_maxima; //aptidao maxima possivel

		static individuo* IndividuoAleatorio();
		static void crossover(individuo *individuo1, individuo *individuo2);

		friend bool operator< (individuo lhs, individuo rhs);
		friend bool operator== (individuo lhs, individuo rhs);
		friend bool operator!= (individuo lhs, individuo rhs);
};
