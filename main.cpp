#include "AG.h"
#include <iostream>
#include <iomanip> 
#include <random>
#include <chrono>

void main(int argc, char** argv)
{
	int count = 0, n_exec = 1000;
	AG ag = AG(100, 80, 200, modo_selecao::_roleta, modo_reinsercao::_elitismo);

	double time_elapsed = 0;

	for (int i = 1; i <= n_exec; i++)
	{
		std::clock_t start_aux = std::clock();
		ag.run();

		double t = ((std::clock() - start_aux) / (double)(CLOCKS_PER_SEC / 1000));
		time_elapsed += t;

		std::cout << "Iteracao " << i << " resultado " << ag.melhorIndividuo().toString() << std::endl;		
		std::cout << "Tempo gasto: " << t << " ms" << std::endl << std::endl;

		if (ag.melhorIndividuo().aptidao() == 0)
			count++;	
	}

	std::cout << "\n\nTempo Total: " << time_elapsed << " ms" << std::endl;
	std::cout << "Tempo Medio: " << time_elapsed / n_exec << " ms" << std::endl;
	std::cout << "% Acerto: " << ((float)count / n_exec) * 100 << std::endl;

	getchar();
}	