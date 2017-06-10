#include <stdio.h>
#include "estado.h"

#define MAX_BUFFER		10240

/**
\brief Função que transforma um estado numa string que contem a sua informação
@param Estado O estado a ser transformado
@returns char* Um apontador para uma string do estado transformado 
*/
char *estado2str(ESTADO e) {
	static char buffer[MAX_BUFFER];
	char *p = (char *) &e;
	int unsigned i;

	buffer[0] = 0;

	for(i = 0; i < sizeof(ESTADO); i++)
		sprintf(buffer, "%s%02x", buffer, p[i]);
	
	return buffer;
}

/**
\brief Função que faz o inverso da anterior, transforma uma string no estado equivalente
@param char* A string a ser transformada para um estado
@returns Estado O estado produzido da transformação da String
*/
ESTADO str2estado(char *argumentos) {
	ESTADO e;
	char *p = (char *) &e;
	int unsigned i;

	for(i = 0; i < sizeof(ESTADO); i++, argumentos += 2) {
		int unsigned d;
		sscanf(argumentos, "%2x", &d);
		p[i] = (char) d;
	}
	
	return e;
}

