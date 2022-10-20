#ifndef _CONWAY_H_
#define _CONWAY_H_

#include <stdint.h>


void cgol     (uint8_t, uint8_t);
void randtabla(uint8_t t[][]); /* error */
void imprime  (uint8_t t[][]); /* error */
void itera    (uint8_t t[][]); /* error */

uint8_t noSeSale(uint8_t, uint8_t);
uint8_t vecinosVivos(uint8_t t[][], uint8_t, uint8_t); /* error */



#endif /* _CONWAY_H_ */

