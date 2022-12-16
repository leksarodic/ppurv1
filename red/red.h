/*
 * red.h
 *
 *  Created on: Oct 18, 2018
 *      Author: rtrk
 */

#ifndef RED_H_
#define RED_H_

#pragma CHECK_MISRA ("none")
#include <stdint.h>
#pragma RESET_MISRA ("all")

#include "konstante.h"
#include "koordinate.h"

/*
 * Implementacija reda korišćenjem niza kao kružni bafer
 *
 * Generalizacija modula red se narušava upotrebom zaglavlja
 * konstante.h (TIP_REDA_t i RED_VELICINA) i kooridnate.h
 * potrebne zbog strukture koja se koristi za tip reda.
 *
 * C kao strogo tipiziran jezik nam ne dozvoljava da ovo uradimo
 * na neki drugi način.
 *
 * Prenosivost se narušava upotrebom stdint.h zaglavlja.
 *
 * Prilikom ponovne upotrebe (reuse) modula red potrebno je izmeniti zaglavlje.
 *
 */

struct s_Red
{
	TIP_REDA_t a_red[RED_VELICINA];

	int64_t zauzeto_pozicija; /* Upotreba potencijalno ne prenosivog tipa */

	TIP_REDA_t* p_upis;
	TIP_REDA_t* p_citanje;
};

void RedInit(struct s_Red* red);
int8_t RedIsEmpty(const struct s_Red* red);
int8_t RedIsFull(const struct s_Red* red);
int8_t RedPut(struct s_Red* red, const TIP_REDA_t element);
TIP_REDA_t* RedGet(struct s_Red* red);

#endif /* RED_H_ */
