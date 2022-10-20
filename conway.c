#include <stdlib.h> /* atoi, rand, malloc */
#include <stdint.h> /* uint8_t            */
#include <errno.h>  /* errno              */
#include <time.h>   /* time               */
#include <unistd.h> /* usleep             */
#include <stdio.h>
// #include "inc/conway.h"
/* 
 * Al añadir el include (que no es necesario en este caso), no le es posible
 * compilar.
 * Por simplicidad estoy usando las dimensiones del array en dos variables
 * globales, y es por este motivo por el que el compilador no se queja (se le
 * está engañando, creo, porque aunque no se asignen dichas variables no se
 * quejará siempre y cuando estén declaradas).
 * Sin embargo si añado el include tal y como está, da error de compilación
 * porque el compilador necesita saber todas las dimensiones de las matrices
 * excepto la de más a la izda, pero yo no puedo saberlas en tiempo de
 * compilación (las obtengo durante la ejecución a través de los parámetros
 * introducidos).
*/

#define VIVA   1
#define MUERTA 0

#define TRUE  1
#define FALSE 0

uint8_t g_alto;
uint8_t g_ancho;

uint8_t evalua(uint8_t celula, uint8_t vvivos) {
	uint8_t ret;
	if ((celula == VIVA) && (vvivos == 2 || vvivos == 3))
		ret = VIVA;
	else if ((celula == MUERTA) && (vvivos == 3))
		ret = VIVA;
	else
		ret = MUERTA;

	return ret;
}

uint8_t noSeSale(uint8_t f, uint8_t c) {
	uint8_t ret = TRUE;

	if ((f >= g_alto) || (c >= g_ancho)) /* al usar uint no comprobamos < 0 */
		ret = FALSE;

	return ret;
}

uint8_t vecinosVivos(uint8_t tabla[g_alto][g_ancho],
					 uint8_t fila, uint8_t col) {
	uint8_t vecinosVivos = 0; /* rango: [0, 8]*/
	uint8_t f, c, i;

	uint8_t despl[8][2] = {
		{-1, -1}, {-1,  0}, {-1,  1},
		{ 0, -1},           { 0,  1},
		{ 1, -1}, { 1,  0}, { 1,  1}
	};

	if (noSeSale(fila, col) == FALSE) {
		printf("Fuera de rango\n");
		return 255;
	}

	for (i = 0; i < 8; i++) {
		f = fila + despl[i][0];
		c = col  + despl[i][1];
		if (noSeSale(f, c) == TRUE)
			if (tabla[f][c] == VIVA)
				vecinosVivos++;
	}

	return vecinosVivos;
}


void itera(uint8_t tabla[g_alto][g_ancho]) {
	uint8_t nueva[g_alto][g_ancho];
	uint8_t celula;
	uint8_t vvivos = 0;
	uint8_t f, c;

	for (f = 0; f < g_alto; f++) {
		for (c = 0; c < g_ancho; c++) {
			celula = tabla[f][c]; /* viva o muerta */
			vvivos = vecinosVivos(tabla, f, c);
			celula = evalua(celula, vvivos);
			nueva[f][c] = celula;
		}
	}

	for (f = 0; f < g_alto; f++)
		for (c = 0; c < g_ancho; c++)
			tabla[f][c] = nueva[f][c];

}

/* ANSI ESCAPE CODES */
void imprime(uint8_t tabla[g_alto][g_ancho]) {
	uint8_t f, c;

	printf("\033[H");
	for (f = 0; f < g_alto; f++) {
		for (c = 0; c < g_ancho; c++)
			printf(tabla[f][c] == 1 ? "\033[07m \033[m" : " ");
		printf("\033[E");
	}
	fflush(stdout);
}

void randtabla(uint8_t tabla[g_alto][g_ancho]) {
	uint8_t f, c;

	srand(time(NULL));

	for (f = 0; f < g_alto; f++)
		for (c = 0; c < g_ancho; c++)
			tabla[f][c] = rand() % 4 == 0 ? 1 : 0;

}

void cgol(uint8_t alto, uint8_t ancho) {
	uint8_t tabla[alto][ancho];

	randtabla(tabla);

	while (1) {
		imprime(tabla);
		itera(tabla);
		usleep(200000);
	}
}


int main(int argc, char** argv) {
	g_alto  = 24;
	g_ancho = 80;

	if (argc == 3) {
		g_alto  = atoi(argv[1]);
		g_ancho = atoi(argv[2]);
		if (g_ancho <= 0 || g_alto <= 0) {
			g_alto  = 24; /* ancho terminal */
			g_ancho = 80; /* mitad de 80 (alto terminal) */
		}
	}

	cgol(g_alto, g_ancho);

}
