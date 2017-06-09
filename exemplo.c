#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"

#define MAX_BUFFER		10240
#define TAM				10
#define ESCALA			40

/**
@file exemplo.c
Ficheiro que contem a logica principal do jogo
*/

/** */
void save(ESTADO e) {

    FILE *fp; 
    char *line;
    line = estado2str(e);

    fp = fopen("./score/state.txt", "w");

    if(fp) {
        fputs(line, fp);
    }

    fclose(fp);
}

/** \brief Verifica se uma posicao e valida
	@param x a coordenada x
	@param y a coordenada y
	@returns Um inteiro */
int posicao_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < TAM && y < TAM);
}
/** \brief Imprime as casas no tabuleiro
	@param x a coordenada x
	@param y a coordenada y
*/
void imprime_casa(int x, int y) {
	char *cor[] = {"#7C7474", "#FFFFFF"};
	int idx = (x + y) % 2;
	QUADRADO(x, y,ESCALA, cor[idx]);
}
/** \brief Funciona como a anterior, mas nao e vista, e simplesmente da para carregar
	@param x a coordenada x
	@param y a coordenada y */
void imprime_casa_transparente(int x, int y) {
	QUADRADO_TRANS(x, y, ESCALA);
}
/** \brief verifica se duas posicoes sao iguais
		@param p Posicao p
		@param x a coordenada x
		@param y a coordenada y
		@returns 0 ou 1  */
int posicao_igual(POSICAO p, int x, int y) {
	return p.x == x && p.y == y;
}
/** \brief Verifica se ha um obstaculo numa posicao
	@param e Estado e
	@param x a coordenada x
	@param y a coordenada y
	@returns Que da return a 1 se houver um obstaculo e da return a 0 se nao houver */
int tem_obstaculo(ESTADO e, int x, int y) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		if(posicao_igual(e.obstaculo[i], x, y))
			return 1;
	return 0;
}
/** \brief Verifica se tem inimigo numa posicao
	@param e Estado e
	@param x a coordenada x
	@param y a coordenada y
	@returns Que da return a 1 se houver um obstaculo e da return a 0 se nao houver */
int tem_inimigo(ESTADO e, int x, int y) {
	int i;
	for(i = 0; i < e.num_inimigos; i++)
		if(posicao_igual(e.inimigo[i], x, y))
			return 1;
	return 0;
}
/** \brief Verifica se ha um jogador numa posicao
	@param e Estado e
	@param x a coordenada x
	@param y a coordenada y
	@returns 1 se houver um jogador ou 0 se nao houver */
int tem_jogador(ESTADO e, int x, int y) {

	return posicao_igual(e.jog, x, y);
}
/** \brief Verifica se o jogador esta a uma casa de distancia de algo (obstaculo, inimigo, jogador ou saida)
	@param e Estado e
	@param x a coordenada x
	@param y a coordenada y
	@returns 1 se houver algo e 0 se nao houver*/
int close_player(ESTADO e, int x, int y) {

	int dx, dy;

	for(dx = -2; dx <= 2; dx++)
		for(dy = -2; dy <= 2; dy++)
			if(tem_jogador(e, x + dx, y + dy))
				return 1;

	return 0;
}
/** \brief Verifica se esta na casa de saida
	@param e Estado e
	@param x a coordenada x
	@param y a coordenada y
	@returns 1 se estiver ou 0 caso contrario*/
int tem_casa_saida(ESTADO e, int x, int y) {
	return posicao_igual(e.fim, x, y);
}
/**
\brief Função que verifica se uma posição está ocupada ou não
@param Estado O estado atual do jogo
@param x Coordenada x da posição a verificar
@param y Coordenada y da posição a verificar
@returns 1 em caso de estar ocupada 0 em caso de estar livre
*/
int posicao_ocupada(ESTADO e, int x, int y) {
	if (tem_inimigo(e, x, y) || tem_obstaculo(e, x, y) || tem_jogador(e, x, y) || tem_casa_saida(e, x, y))
		return 1;

	return 0;
}
/**
\brief Função para gerar um inimigo numa posição random e a colocar no array de inimigos
@param Estado O estado atual do jogo
@return Estado do jogo após gerar um inimigo
*/
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
/**
\brief Função para chamada da função inicializar_inimigo() para gerar a totalidade dos inimigos a serem gerados
@param Estado O estado atual do jogo
@param num Numero de inimigos a serem gerados
@returns O estado já com os inimigos gerados
*/
ESTADO inicializar_inimigos(ESTADO e, int num) {
	int i;

	for(i = 0; i < num; i++) {
		e = inicializar_inimigo(e);
	}

	return e;
}
/**
\brief Função para gerar um obstáculo numa posição random e a colocar no array de obstáculos
@param Estado O estado atual do jogo
@return Estado do jogo após gerar um obstáculo
*/
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
/**
\brief Função para chamada da função inicializar_obstaculo() para gerar a totalidade dos obstáculos a serem gerados
@param Estado O estado atual do jogo
@param num Numero de obstáculos a serem gerados
@returns O estado já com os obstáculos gerados
*/
ESTADO inicializar_obstaculos(ESTADO e, int num) {
	int i;

	for(i = 0; i < num; i++) {
		e = inicializar_obstaculo(e);
	}

	return e;
}
/**
\brief Função que inicializa a casa de saida
@param Estado O estado atual do jogo
*/
ESTADO inicializar_saida(ESTADO e) {
	e.fim.x = 9;
	e.fim.y = 0;

	return e;
}
/**
\brief Função que inicializa o estado de jogo
@returns O estado inicial do jogo
*/
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
/**
\brief Função que faz level up quando um jogador finaliza um nivel
@param Estado O estado atual do jogo
*/
ESTADO level_up(ESTADO e) {
	ESTADO new = inicializar();
	new.score = e.score + 10;
	return new;
}
/**
\brief Função que processa o movimento do jogador
@param Estado O estado atual do jogo
@param dx Deslocamento em x do jogador
@param dy Deslocamento em y do jogador
*/
void imprime_movimento(ESTADO e, int dx, int dy) {
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	char link[MAX_BUFFER];
	if(posicao_valida(x, y) && !tem_obstaculo(e, x, y)) {
		if(tem_casa_saida(e, x, y)) {
			save(level_up(e));
			sprintf(link, "http://localhost/cgi-bin/exemplo");
		}
		else {
			sprintf(link, "http://localhost/cgi-bin/exemplo?x=%d&y=%d", x, y);
		}
		ABRIR_LINK(link);
		imprime_casa_transparente(x, y);
		FECHAR_LINK;
	}
}
/**
\brief Função que imprime o movimento feito pelo jogador
@param Estado O estado atual do jogo
*/
void imprime_movimentos(ESTADO e) {
	int dx, dy;

	for(dx = -1; dx <= 1; dx++)
		for(dy = -1; dy <= 1; dy++)
			if (dx != 0 || dy != 0)
				imprime_movimento(e, dx, dy);
}
/**
\brief Função que imprime o jogador no tabuleiro
@param Estado O estado atual do jogo
*/
void imprime_jogador(ESTADO e) {
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "DwellerN_03.png");
	imprime_movimentos(e);
}
/**
\brief Função para ler o estado do jogo a partir de uma query string
@param
*/
ESTADO ler_estado(char *args) {
	if(strlen(args) == 0)
		return inicializar();
	return str2estado(args);
}
/**
\brief Função que imprime no tabuleiro os inimigos
@param Estado O estado atual do jogo
*/
void imprime_inimigos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_inimigos; i++)
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Driders_04.png");
}
/**
\brief Função para imprimir no tabuleiro a casa de saida(casa de finalização de nivel)
@param Estado O estado atual do jogo
*/
void imprime_saida(ESTADO e) {
	IMAGEM(e.fim.x, e.fim.y, ESCALA, "trapdoor1.png");
}
/**
\brief Função que imprime obstáculos no tabuleiro
@param Estado Recebe o estado do jogo atual
*/
void imprime_obstaculos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "lava_pool1.png");
}
/**
\brief Função que troca a posição de um inimigo no array de inimigos para a ultima posição do array
@param Estado Estado atual do jogo
@param x Posição do inimigo a trocar no array de inimigos
@returns O estado depois de fazer a troca do inimigo
*/
ESTADO swap_ini(ESTADO e, int x) {

	POSICAO tmp = e.inimigo[x];
	e.inimigo[x] = e.inimigo[(int)e.num_inimigos - 1];
	e.inimigo[(int)e.num_inimigos - 1] = tmp;

	return e;

}
/**
\brief Função que mata inimigos
@param Estado Recebe o estado atual do jogo
@returns O estado do jogo após matar o inimigo
*/
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
/**
\brief Função que verifica a vida do jogador e o mata caso esta seja menor do que 0
@param Estado O estado atual do jogo
@returns O estado do jogo após a verificação da vida
*/
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
/**
\brief Função que move inimigos
@param Estado Estado atual do jogo
@param i Posição de um inimigo no array de inimigos
@param dx Deslocamento em x
@param dy Deslocamento em y
@returns Estado depois da movimentação do inimigo
*/
ESTADO move_inimigo(ESTADO e, int i, int dx, int dy) {

	e.inimigo[i].x += dx;
	e.inimigo[i].y += dy;

	return e;

}
/**
\brief Função que verifica se uma posição é valida
@param Estado Estado atual do jogo
@param i Posição de um inimigo no array de inimigos
@param dx Deslocamento em x
@param dy Deslocamento em y
@returns 1 se posição válida, 0 se contrário
*/

int check_pos(ESTADO e, int i, int dx, int dy) {
	if(dx == 0 && dy == 0)
		return 1;
	else {
		return(!posicao_ocupada(e, e.inimigo[i].x + dx, e.inimigo[i].y + dy)
		 && posicao_valida(e.inimigo[i].x + dx, e.inimigo[i].y + dy));
	}
}
/**
	\brief Função que move inimigos
	@param estado O estado do jogo atual
	@returns O estado depois da manipulação
 */
ESTADO mover_inimigos(ESTADO e) {

	int i, dx, dy;

	for(i = 0; i < e.num_inimigos; i++) {
		dx = (rand() % 3) - 1;
		dy = (rand() % 3) - 1;
		if(check_pos(e, i, dx, dy))
			e = move_inimigo(e, i, dx, dy);
	}

	return e;
}

void load(char state[]) {
    FILE *fp;

    fp = fopen("./score/state.txt", "r");

    if(fp) {
    	if(fscanf(fp, "%s", state) == 0) {

    	}
    }

    fclose(fp);

}

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
/**
\brief Função principal do programa
*/
int main() {
	/** Variaveis que auxiliam a impressão do tabuleiro e duas que eram guardar dados relativos a query string */
	int x, y, qx, qy;
	/** Variavel de estado */
	ESTADO e;
	/** String que armazena o estado */
	char state[MAX_BUFFER];
	/** */
	char *querry;
	/** Função que fornece uma seed à função srand */
	srand(time(NULL));
	/** */
	querry = getenv("QUERY_STRING");
	/** */
	if(strlen(querry) > 0) {
		sscanf(querry, "x=%d&y=%d", &qx, &qy);
	}
	else {
		qx = 4;
		qy = 9;
	}
	/** Carrega o estado(sob o formato de string) no ficheiro para uma string */
	load(state);
	/** Transforma o estado carregado sob a forma de uma string para a estrutura estado */
	e = ler_estado(state);
	/** atualizar a posicao do jogador em relaçao ao eixo do x */
	e.jog.x = qx;
	/** atualizar a poscaio do jogador em relacao ao eixo do y */
	e.jog.y = qy;

	/** Chamada da função mata_monstros() */
	e = mata_monstros(e);
	/** Chamada da função mata_jogador() */
	e = mata_jogador(e);
	/** Chamada da função mover_inimigos() */
	e = mover_inimigos(e);
	/** */
	save(e);

	/** Macro para iniciar o formato HTML */
	COMECAR_HTML;
	/**
	Chamada da macro para iniciar um SVG
	@param 600, 600, é o tamanho do SVG
	*/
	ABRIR_SVG(600, 600);
	/**
	Condicional que auxilia a impressão de mapa e a manipulação de scores
	@param e.vida, vida atual do jogador
	*/
	if(e.vida <= 0) {
		/** Chamada da função score_manage()
		@param e.score, score do jogador
		*/
		score_manage(e.score);
		/** guarda um estado novo para ser aberto */
		save(inicializar());
		/**
		Macro para abrir link
		@param link
		*/
		ABRIR_LINK("http://localhost/cgi-bin/exemplo");
		/**
		Macro para desenhar um quadrado
		@param posx, posy, tam, cor, Parametros do quadrado
		*/
		QUADRADO(0, 0, 600, "#7C7474");
		/**
		Macro para fechar link
		*/
		FECHAR_LINK;
	}
	else {
		/** Ciclo para imprimir o tabuleiro */
		for(y = 0; y < 10; y++)
			for(x = 0; x < 10; x++)
				/** Chamada da função imprime_casa()
				@param x, y, Posição a imprimir
				*/
					imprime_casa(x, y);
	/** Chamada da função imprime_inimigos()
	@param e, Estado
	 */
	imprime_inimigos(e);
	/** Chamada da função imprime_saida()
	@param e, Estado
	 */
	imprime_saida(e);
	/** Chamada da função imprime_jogador()
	@param e, Estado
	 */
	imprime_jogador(e);
	/** Chamada da função imprime_obstaculos()
	@param e, Estado
	 */
	imprime_obstaculos(e);

	}
	/** Macro para fechar SVG */
	FECHAR_SVG;

	return 0;
}
