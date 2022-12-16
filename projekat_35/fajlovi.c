/*
 * fajlovi.c
 *
 *  Created on: Oct 18, 2018
 *      Author: rtrk
 */

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#pragma RESET_MISRA ("all")

#include "fajlovi.h"
#include "konstante.h"

int8_t UcitajSliku(const char* fajl_ime, int8_t slika[SLIKA_Y][SLIKA_X])
{
	int32_t i = 0;
	int32_t j = 0;

	FILE* fajl_slika;

	int8_t procitana_vrednost;

	/* Otvaranje fajla */
	fajl_slika = fopen(fajl_ime, "r");
	if (fajl_slika == 0)
	{
#ifdef __GNUC__
		fprintf(stderr, "%s: File %s opening error.\n", __func__, fajl_ime);
#endif
		return 0;
	}

	/* Čitanje iz fajla vrednosti */
	while ((feof(fajl_slika)) != 1)
	{
		procitana_vrednost = fgetc(fajl_slika);

		assert(procitana_vrednost <= INT8_MAX);

		if (procitana_vrednost != '\n')
		{
			slika[i][j++] = procitana_vrednost - ASCII_BROJEVI;
		}
		else
		{
			i++;
			j = 0;
		}
	}

	/* Zatvaranje fajla */
	fclose(fajl_slika);

	return 1;
}
