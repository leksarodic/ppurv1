/*
 * komsije.h
 *
 *  Created on: Oct 20, 2018
 *      Author: rtrk
 */

#ifndef KOMSIJE_H_
#define KOMSIJE_H_

#pragma CHECK_MISRA ("none")
#include <stdint.h>
#pragma RESET_MISRA ("all")

#include "red.h"
#include "koordinate.h"
#include "konstante.h"

int32_t pronadjiKomsije(int8_t slika[SLIKA_Y][SLIKA_X], struct s_Red* red, const struct s_Koordinate koordinate);

#endif /* KOMSIJE_H_ */
