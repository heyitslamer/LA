#include "cgi.h"

int main() {

	COMECAR_HTML;

	ABRIR_SVG(600, 600);

	QUADRADO(0, 0, 600, "#2D2D2D");

		printf("<text x=285 y=20 fill = \"#C4C3C3\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\">Jogo inspirado num coffee break roguelike desenvolvido no ambito da</text>\n");
		printf("<text x=115 y=40 fill = \"#C4C3C3\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\">cadeira de LA no ano 2017.</text>\n");

		printf("<text x=90 y=80 fill = \"#C4C3C3\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\">Cesar Silva      -     A79008</text>\n");
		printf("<text x=125 y=100 fill = \"#C4C3C3\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\">Bernardo Rodrigues - A79008</text>\n");
		printf("<text x=100 y=120 fill = \"#C4C3C3\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\">Tigas Pereira    -   A79008</text>\n");

		printf("<text x=285 y=160 fill = \"#C4C3C3\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\">Carrega nas casas a tua volta para te movimentares se carregares em</text>\n");
		printf("<text x=1 y=180 fill = \"#C4C3C3\" font-family = \"serif\">cima de um monstro vais mata-lo mas cuidado pois se te movimentares</text>\n");
		printf("<text x=1 y=200 fill = \"#C4C3C3\" font-family = \"serif\">para perto de um levas dano.</text>\n");

		printf("<text x=1 y=240 fill = \"#C4C3C3\" font-family = \"serif\">O objetivo do jogo e conseguires o maximo de pontos possiveis.</text>\n");
		printf("<text x=1 y=280 fill = \"#C4C3C3\" font-family = \"serif\">Para veres os highscores abre o ficheiro de texto guardo em</text>\n");
		printf("<text x=1 y=300 fill = \"#C4C3C3\" font-family = \"serif\">/usr/lib/cgi-bin/score.</text>\n");

		ABRIR_LINK("http://localhost/cgi-bin/menu");
		printf("<rect x=5 y=350 width=120 height=35 ry=5 fill = \"rgb(196, 195, 195)\"/>\n");
		printf("<text x=38 y=375 font-family = \"serif\" font-weight = \"bold\">Back</text>\n");
		FECHAR_LINK;

	FECHAR_SVG;

}

