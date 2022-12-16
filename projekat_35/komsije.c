/*
 * komsije.c
 *
 *  Created on: Oct 20, 2018
 *      Author: rtrk
 */

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#ifdef __GNUC__
#include <dlfcn.h>
#endif
#pragma RESET_MISRA ("all")

#include "red.h"
#include "koordinate.h"
#include "konstante.h"
#include "komsije.h"

int32_t pronadjiKomsije(int8_t slika[SLIKA_Y][SLIKA_X], struct s_Red* red, const struct s_Koordinate koordinate)
{
	assert((koordinate.x >= 0) && (koordinate.x < SLIKA_X));
	assert((koordinate.y >= 0) && (koordinate.y < SLIKA_Y));

	const struct s_Koordinate flag_ne_citaj = {-1, -1};

	int32_t i;

	/* Samo za prolaz kroz red */
	int64_t j; /* Upotreba potencijalno ne prenosivog tipa */

	/* Samo za while petlju */
	int32_t k = 0;

	struct s_Koordinate* radne_koordinate;
	struct s_Koordinate komsijske_koordinate[4];

	int8_t vrednost_na_pocetnoj_koordinati = slika[koordinate.y][koordinate.x];

	/* Služi za indeksiranje i proveru, rešavanje duplikata */
	int64_t upisanih_u_red = 1; /* Upotreba potencijalno ne prenosivog tipa */

	void* handle_red;
	int8_t (*RedPut)(struct s_Red*, const TIP_REDA_t);
	TIP_REDA_t* (*RedGet)(struct s_Red*);
	char* error;

	/* Dinamičko povezivanje modula */
#ifdef __GNUC__
	handle_red = dlopen(IME_BIBLIOTEKE_RED, RTLD_LAZY);
	if (handle_red == NULL)
	{
		fputs(dlerror(), stderr);
		exit(ERROR_DLOPEN);
	}

	RedPut = dlsym(handle_red, "RedPut");
	if ((error = dlerror()) != NULL)
	{
		fputs(error, stderr);
		exit(ERROR_DLSYM);
	}

	RedGet = dlsym(handle_red, "RedGet");
	if ((error = dlerror()) != NULL)
	{
		fputs(error, stderr);
		exit(ERROR_DLSYM);
	}
#endif

	/* Početne koordinate na početku smeštamo u red i čitamo prve koordinate */
	RedPut(red, koordinate);
	radne_koordinate = RedGet(red);

	/* Prolazimo kroz sve susede */
	while (k != upisanih_u_red)
	{
		/* Pozivaj while dok god ne prođeš kroz sve validno upisane koordinate u redu */
		k++;

		/* Indeksiramo komšije */
		komsijske_koordinate[0].x = radne_koordinate->x - 1;
		komsijske_koordinate[0].y = radne_koordinate->y;
		komsijske_koordinate[1].x = radne_koordinate->x + 1;
		komsijske_koordinate[1].y = radne_koordinate->y;
		komsijske_koordinate[2].x = radne_koordinate->x;
		komsijske_koordinate[2].y = radne_koordinate->y - 1;
		komsijske_koordinate[3].x = radne_koordinate->x;
		komsijske_koordinate[3].y = radne_koordinate->y + 1;

		/* Rešavamo granične slučajeve, obeležavamo komšije van opsega slike */
		for (i = 0; i < 4; i++)
		{
			if ((komsijske_koordinate[i].x == -1) || (komsijske_koordinate[i].x >= SLIKA_X))
			{
				komsijske_koordinate[i] = flag_ne_citaj;
			}
			if ((komsijske_koordinate[i].y == -1) || (komsijske_koordinate[i].y >= SLIKA_Y))
			{
				komsijske_koordinate[i] = flag_ne_citaj;
			}
		}

		/* Provera da li postoje komšije istih vrednosti kao na početnoj koordinati */
		for (i = 0; i < 4; i++)
		{
			if ((komsijske_koordinate[i].x != flag_ne_citaj.x) && (komsijske_koordinate[i].y != flag_ne_citaj.y))
			{
				if (slika[komsijske_koordinate[i].y][komsijske_koordinate[i].x] != vrednost_na_pocetnoj_koordinati)
				{
					komsijske_koordinate[i] = flag_ne_citaj;
				}
			}
		}

		/* Provera da li koordinate validnih komšija postoje u redu i upis u red */
		for (i = 0; i < 4; i++)
		{
			int8_t ima_u_redu = 0;

			/* Ako struktura nije označena flagom, gledaj je */
			if ((komsijske_koordinate[i].x != flag_ne_citaj.x) && (komsijske_koordinate[i].y != flag_ne_citaj.y))
			{
				/* Provera da li se potencijalna nova koordinata već nalazi u redu */
				for (j = 0; j < upisanih_u_red; j++)
				{
					if ((red->a_red[j].x == komsijske_koordinate[i].x) && (red->a_red[j].y == komsijske_koordinate[i].y))
					{
						ima_u_redu = 1;
						break;
					}
				}

				/* Upis u red */
				if (ima_u_redu == 0)
				{
					RedPut(red, komsijske_koordinate[i]);
					upisanih_u_red++;
				}
			}
		}

		/* Čitamo naredne koordinate iz reda */
		radne_koordinate = RedGet(red);
	}

	/* Dinamicko "odvezivanje" tokom izvrsavanja */
#ifdef __GNUC__
	dlclose(handle_red);
#endif

	/* Povratna vrednost funkcije je broj unetih koordinata */
	return upisanih_u_red;

}
