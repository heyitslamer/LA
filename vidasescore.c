#include "cgi.h"
int main() {

	COMECAR_HTML;

	ABRIR_SVG(600, 600);

	QUADRADO(0, 0, 600, "#2D2D2D");
	printf("<text x=1 y=20 fill = \"#C4C3C3\" font-family = \"serif\">Vidas:</text>\n");
	IMAGEM(2, 0, 25, "heart.png");
	IMAGEM(3, 0, 25, "heart.png");
	IMAGEM(4, 0, 25, "heart.png");

	printf("<text x=1 y=40 fill = \"#C4C3C3\" font-family = \"serif\">Score:</text>\n");
	printf("<text x=55 y=40 fill = \"#C4C3C3\" font-family = \"serif\">50</text>\n");
	FECHAR_SVG;

}