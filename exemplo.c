#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"

#define MAX_BUFFER		10240
#define TAM				10
#define ESCALA			40

void imprime_saida(ESTADO e);

int posicao_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < TAM && y < TAM);
}

void imprime_casa(int x, int y) {
	char *cor[] = {"#7C7474", "#FFFFFF"};
	int idx = (x + y) % 2;
	QUADRADO(x, y,ESCALA, cor[idx]);
}

void imprime_casa_transparente(int x, int y) {
	QUADRADO_TRANS(x, y, ESCALA);
}

int posicao_igual(POSICAO p, int x, int y) {
	return p.x == x && p.y == y;
}

int tem_obstaculo(ESTADO e, int x, int y) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		if(posicao_igual(e.obstaculo[i], x, y))
			return 1;
	return 0;
}

int tem_inimigo(ESTADO e, int x, int y) {
	int i;
	for(i = 0; i < e.num_inimigos; i++)
		if(posicao_igual(e.inimigo[i], x, y))
			return 1;
	return 0;
}

int tem_jogador(ESTADO e, int x, int y) {

	return posicao_igual(e.jog, x, y);
}

int casa_saida(ESTADO e, int x, int y) {
	return posicao_igual(e.fim, x, y);
}

int posicao_ocupada(ESTADO e, int x, int y) {
	//return tem inimigo(e, x, y) || tem_obstaculo(e, x, y)
	if (tem_inimigo(e, x, y) || tem_obstaculo(e, x, y) || tem_jogador(e, x, y) || casa_saida(e, x, y))
		return 1;

	return 0;
}

ESTADO inicializar_inimigo(ESTADO e) {

	int x, y;

	do {
		x = random() % TAM;
		y = random() % TAM;
	} while (posicao_ocupada(e, x, y));

	e.inimigo[(int)e.num_inimigos].x = x;
	e.inimigo[(int)e.num_inimigos].y = y;

	e.num_inimigos++;

	return e;
}

ESTADO inicializar_inimigos(ESTADO e, int num) {
	int i;

	for(i = 0; i < num; i++) {
		e = inicializar_inimigo(e);
	}

	return e;
}

ESTADO inicializar_obstaculo(ESTADO e) {

	int x, y;

	do {
		x = random() % TAM;
		y = random() % TAM;
	} while (posicao_ocupada(e, x, y));

	e.obstaculo[(int)e.num_obstaculos].x = x;
	e.obstaculo[(int)e.num_obstaculos].y = y;

	e.num_obstaculos++;

	return e;
}

ESTADO inicializar_obstaculos(ESTADO e, int num) {
	int i;

	for(i = 0; i < num; i++) {
		e = inicializar_obstaculo(e);
	}

	return e;
}

ESTADO inicializar_saida(ESTADO e) {
	e.fim.x = 9;
	e.fim.y = 0;

	return e;
}

ESTADO inicializar() {
	ESTADO e = {{0}};
	e.jog.x = 5;
	e.jog.y = 9;
	e = inicializar_saida(e);
	e = inicializar_inimigos(e, 10);
	e = inicializar_obstaculos(e, 10);
	e.vida = 3;
	e.score = 0;
	return e;
}

void imprime_movimento(ESTADO e, int dx, int dy) {
	ESTADO novo = e;
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	char link[MAX_BUFFER];
	if(posicao_valida(x, y) && !tem_obstaculo(e, x, y)) {
		if(casa_saida(e, x, y)) {
			novo = inicializar();
		} else {
			novo.jog.x = x;
			novo.jog.y = y;
		}
		sprintf(link, "http://localhost/cgi-bin/exemplo?%s", estado2str(novo));
		ABRIR_LINK(link);
		imprime_casa_transparente(x, y);
		FECHAR_LINK;
	}
}

//to do imprime_casa_transparente

void imprime_movimentos(ESTADO e) {
	int dx, dy;

	for(dx = -1; dx <= 1; dx++)
		for(dy = -1; dy <= 1; dy++)
			if (dx != 0 || dy != 0)
				imprime_movimento(e, dx, dy);
}

void imprime_jogador(ESTADO e) {
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "DwellerN_03.png");
	imprime_movimentos(e);
}

ESTADO ler_estado(char *args) {
	if(strlen(args) == 0)
		return inicializar();
	return str2estado(args);
}

void imprime_inimigos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_inimigos; i++)
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Driders_04.png");
}

void imprime_saida(ESTADO e) {
	IMAGEM(e.fim.x, e.fim.y, ESCALA, "trapdoor1.png");
}

void imprime_obstaculos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "lava_pool1.png");
}

void imprime_vida(ESTADO e) {
	ABRIR_TEXTO;
	VIDA(e.vida);
	FECHAR_TEXTO;
}

ESTADO swap_ini(ESTADO e, int x) {

	POSICAO tmp = e.inimigo[x];
	e.inimigo[x] = e.inimigo[(int)e.num_inimigos - 1];
	e.inimigo[(int)e.num_inimigos - 1] = tmp;

	return e;

}

ESTADO mata_monstros(ESTADO e) {

	int i;

	for(i = 0; i < e.num_inimigos; i++) {
		if(e.jog.x == e.inimigo[i].x && e.jog.y == e.inimigo[i].y) {
			e = swap_ini(e, i);
			e.num_inimigos--;
			e.score++;
			break;
		}
	}

	return e;
}

void fim_jogo() {
		//COMECAR_HTML;
		//ABRIR_SVG(600, 600);
		IMAGEM(0, 0, 300, "hqdefault.jpg");
		//FECHAR_SVG;

}


void mata_jogador(ESTADO *e) {

	int dx, dy;

	for(dx = -1; dx <= 1; dx++) {
		if(e -> vida <= 0)
			break;
		for(dy = -1; dy <= 1; dy++)
			if (dx != 0 || dy != 0)
				if(tem_inimigo(*e, e->jog.x + dx, e->jog.y + dy))
					e->vida--;
	}

	if(e->vida <= 0)
		//*e = inicializar();
		fim_jogo();
}


int main() {
	srandom(time(NULL));
	int x, y;
	ESTADO e = ler_estado(getenv("QUERY_STRING"));


	e = mata_monstros(e);
	// percorrer array de inimigos, ver se coincide com jogador e remover

	//mata_jogador(&e);

	/*
	e = mover_inimigos(e);
	// ver se pos ta ocupada. mover inicialmente aletoriamente(entre -1 e 1), e.g prov de monstro se mexer
	*/

	COMECAR_HTML;
	ABRIR_SVG(600, 600);
	for(y = 0; y < 10; y++)
		for(x = 0; x < 10; x++)
				imprime_casa(x, y);

	mata_jogador(&e);
	imprime_inimigos(e);
	imprime_saida(e);
	imprime_jogador(e);
	imprime_obstaculos(e);
	imprime_vida(e);


	FECHAR_SVG;

	return 0;
}
