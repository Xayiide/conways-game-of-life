#include <stdlib.h> /* atoi, rand, malloc */
#include <stdint.h> /* uint8_t            */
#include <errno.h>  /* errno              */
#include <stdio.h>

void evolve(uint8_t* tabla, uint8_t ALTO, uint8_t ANCHO) {
	
}

/* ANSI ESCAPE CODES */
void render(uint8_t *tabla, uint8_t ALTO, uint8_t ANCHO) {
	uint8_t f, c;

	for (f = 0; f < ALTO; f++) {
		for (c = 0; c < ANCHO; c++) {
			printf(tabla[ANCHO * f + c] == 1 ? "\033[07m \033[m" : " ");
		}
		printf("\n");
	}
	fflush(stdout);
}

void randtabla(uint8_t *tabla, uint8_t ALTO, uint8_t ANCHO) {
	uint8_t f, c;

	for (f = 0; f < ALTO; f++)
		for (c = 0; c < ANCHO; c++)
			tabla[ANCHO * f + c] = rand() % 2 == 0 ? 1 : 0;

}

void cgol(uint8_t alto, uint8_t ancho) {
	uint8_t *tabla;
	tabla = malloc(alto * ancho * sizeof(uint8_t));
	if (tabla == 0) {
		errno = ENOMEM;
		printf("Error al obtener espacio en memoria (malloc)\n");
		return;
	}

	randtabla(tabla, alto, ancho);
	render(tabla, alto, ancho);
}


int main(int argc, char** argv) {
	uint8_t alto  = 24;
	uint8_t ancho = 80;

	if (argc == 3) {
		alto  = atoi(argv[1]);
		ancho = atoi(argv[2]);
		if (ancho <= 0 || alto <= 0) {
			alto  = 24;
			ancho = 40; /* mitad de 80 */
		}
	}

	cgol(alto, ancho);

}
