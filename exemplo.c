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

int close_player(ESTADO e, int x, int y) {

	int dx, dy;

	for(dx = -2; dx <= 2; dx++)
		for(dy = -2; dy <= 2; dy++)
			if(tem_jogador(e, x + dx, y + dy))
				return 1;

	return 0;
}

int tem_casa_saida(ESTADO e, int x, int y) {
	return posicao_igual(e.fim, x, y);
}

int posicao_ocupada(ESTADO e, int x, int y) {
	if (tem_inimigo(e, x, y) || tem_obstaculo(e, x, y) || tem_jogador(e, x, y) || tem_casa_saida(e, x, y))
		return 1;
	
	return 0;
}

ESTADO inicializar_inimigo(ESTADO e) {

	int x, y;

	do {
		x = rand() % TAM;
		y = rand() % TAM;
	} while (posicao_ocupada(e, x, y) || close_player(e, x, y));

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
		x = rand() % TAM;
		y = rand() % TAM;
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
	ESTADO e = {{0, 0}, (char)0, (char)0, 0, 0, {{0, 0}}, {{0, 0}}, {0, 0}};
	e.jog.x = 5;
	e.jog.y = 9;
	e = inicializar_saida(e);
	e = inicializar_inimigos(e, 10);
	e = inicializar_obstaculos(e, 10);
	e.vida = 3;
	e.score = 0;
	return e;
}

ESTADO level_up(ESTADO e) {
	ESTADO new = inicializar();
	new.score = e.score + 10;
	return new;
}

void imprime_movimento(ESTADO e, int dx, int dy) {
	ESTADO novo = e;
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	char link[MAX_BUFFER];
	if(posicao_valida(x, y) && !tem_obstaculo(e, x, y)) {
		if(tem_casa_saida(e, x, y)) {
			novo = level_up(novo);
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
			e.score += 3;
			break;
		}
	}

	return e;
}

ESTADO mata_jogador(ESTADO e) {

	int dx, dy;

	for(dx = -1; dx <= 1; dx++) {
		if(e.vida <= 0)
			break;
		for(dy = -1; dy <= 1; dy++)
			if (dx != 0 || dy != 0)
				if(tem_inimigo(e, e.jog.x + dx, e.jog.y + dy))
					e.vida--;
	}

	return e;

}

ESTADO move_inimigo(ESTADO e, int i, int dx, int dy) {

	e.inimigo[i].x += dx;
	e.inimigo[i].y += dy;

	return e;

}


int check_pos(ESTADO e, int i, int dx, int dy) {
	if(dx == 0 && dy == 0)
		return 1;
	else {
		return(!posicao_ocupada(e, e.inimigo[i].x + dx, e.inimigo[i].y + dy)
		 && posicao_valida(e.inimigo[i].x + dx, e.inimigo[i].y + dy));
	}
}

ESTADO mover_inimigos(ESTADO e) {

	int i, dx, dy;

	dx = (rand() % 3) - 1;
	dy = (rand() % 3) - 1;

	for(i = 0; i < e.num_inimigos; i++) {
		dx = (rand() % 3) - 1;
		dy = (rand() % 3) - 1;
		if(check_pos(e, i, dx, dy))
			e = move_inimigo(e, i, dx, dy);
	}

	return e;
}

/**
void save_state(char s[]) {

	FILE *f = fopen("./score/state.txt", "r+");
	if(!f) {
		f = fopen("./score/state.txt", "w+");
		
	}

}
*/


void score_manage(int score) {

	FILE *scores = fopen("./score/scores.txt", "r+");
	int fivebest[5];
	int i, tmp1, tmp2;

	tmp1 = tmp2 = 0;

	if(!score) {
		scores = fopen("./score/scores.txt", "w+");
		
		fprintf(scores, "%d\n", score);

		for(i = 0; i < 4; i++)
			fprintf(scores, "0\n");
		
	}
	else {
		for(i = 0; i < 5; i++) {
			if(fscanf(scores, "%d", fivebest + i) == 1)
				fseek(scores, 1, SEEK_CUR);
		}

		for(i = 0; i < 5 && score < fivebest[i]; i++);

		if(i >= 0 && i < 5) {
			tmp1 = fivebest[i];
			fivebest[i] = score;
			i++;
			while(i < 4) {
				tmp2 = fivebest[i];
				fivebest[i] = tmp1;
				tmp1 = tmp2;
				i++;
			}
			fivebest[i] = tmp1;
		}

		fseek(scores, 0, SEEK_SET);

		for(i = 0; i < 5; i++)
			fprintf(scores, "%d\n", fivebest[i]);

	}

	fclose(scores);

}


int main() {
	
	int x, y;
	ESTADO e;

	srand(time(NULL));
	e = ler_estado(getenv("QUERY_STRING"));

	/**
		ler_estado a partir de ficheiro,  query_string passa a ser o nome de um utilizador, accoes sao coordendas, perguntar ao diogo??
	*/

	e = mata_monstros(e);
	
	e = mata_jogador(e);
	
	e = mover_inimigos(e);
	


	COMECAR_HTML;
	ABRIR_SVG(600, 600);

	if(e.vida <= 0) {
		score_manage(e.score);
		ABRIR_LINK("http://localhost/cgi-bin/exemplo");
		QUADRADO(0, 0, 600, "#7C7474");
		FECHAR_LINK;
	}
	else {

	for(y = 0; y < 10; y++)
		for(x = 0; x < 10; x++)
				imprime_casa(x, y);

	imprime_inimigos(e);
	imprime_saida(e);
	imprime_jogador(e);
	imprime_obstaculos(e);

	}
	FECHAR_SVG;

	return 0;
}
