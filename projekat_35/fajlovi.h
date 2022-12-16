/*
 * fajlovi.h
 *
 *  Created on: Oct 18, 2018
 *      Author: rtrk
 */

#ifndef FAJLOVI_H_
#define FAJLOVI_H_

#pragma CHECK_MISRA ("none")
#include <stdint.h>
#pragma RESET_MISRA ("all")

#include "konstante.h"

int8_t UcitajSliku(const char* fajl_ime, int8_t slika[SLIKA_Y][SLIKA_X]);

#endif /* FAJLOVI_H_ */
