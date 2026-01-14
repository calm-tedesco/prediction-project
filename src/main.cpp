#include <iostream>
#include <vector>
#include "deming.hpp"

int main(int argc, char **argv)
{
	if (argc  < 2)
	{
		std::cerr << "ERROR: Falta un argumento" << std::endl;
		std::cout << "Sintaxis: <programa> <path al archivo de datos>" << std::endl;

		std::exit (EXIT_FAILURE);
	}

	std::ifstream infile( argv[1] );
	if ( infile )
	{
		Deming mi_ajuste( infile );

		mi_ajuste.ImprimeResultado();
	}
	else
	{
    std::cerr << "ERROR: Problema leyendo archivo" << std::endl;
		std::cout << "Asegúrese que el path existe y el archivo se puede leer" << std::endl;

    std::exit (EXIT_FAILURE);
	}

	infile.close();

  return 0;
}
