#include "cgi.h"


int main() {

	COMECAR_HTML;

	ABRIR_SVG(600, 600);

	QUADRADO(0, 0, 600, "#7C7474");


	ABRIR_LINK("http://localhost/cgi-bin/exemplo");
	printf("<rect x=200 y=200 width=100 height=50 ry=5 fill = \"rgb(255,120,0)\"/>\n");
	printf("<text x=250 y=230 text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\">NEW GAME</text>\n");
	FECHAR_LINK;

	/**
	ABRIR_LINK("");
	<rect x=50 y=50 width=100 height=50 ry=5 fill = "rgb(255,120,0)" />
	<text x=100 y=80 text-anchor = "middle" text-align = "center" font-family = "serif" font-weight = "bold">Teste</text>
	FECHAR_LINK;
	*/
	FECHAR_SVG;
}

/**
50 50
100 80
*/