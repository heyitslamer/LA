#include "cgi.h"


int main() {

	COMECAR_HTML;

	ABRIR_SVG(600, 600);

	QUADRADO(0, 0, 600, "#2D2D2D");


	ABRIR_LINK("http://localhost/cgi-bin/exemplo");
	printf("<rect x=240 y=205 width=120 height=35 ry=5 fill = \"rgb(196, 195, 195)\"/>\n");
	printf("<text x=300 y=230 text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\">New Game</text>\n");
	FECHAR_LINK;

	ABRIR_LINK("http://localhost/cgi-bin/about");
	printf("<rect x=240 y=305 width=120 height=35 ry=5 fill = \"rgb(196, 195, 195)\"/>\n");
	printf("<text x=300 y=330 text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\">About</text>\n");
	FECHAR_LINK;

	FECHAR_SVG;
}
