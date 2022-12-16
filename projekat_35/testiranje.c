/*
 * testiranje.c
 *
 *  Created on: Oct 18, 2018
 *      Author: rtrk
 */

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>

#ifdef __GNUC__
#include <dlfcn.h>
#endif
#pragma RESET_MISRA ("all")

#include "testiranje.h"

#include "konstante.h"

#include "red.h"
#include "fajlovi.h"
#include "komsije.h"

void testiranjeRedova(const int64_t broj_elemenata_za_unos)
{
	assert((broj_elemenata_za_unos >= 0) && (broj_elemenata_za_unos <= INT64_MAX));

	int64_t i; /* Upotreba potencijalno ne prenosivog tipa */

	void* handle_red;

	struct s_Red red;
	int8_t red_pun = -1;
	int8_t red_prazan = -1;
	struct s_Koordinate* element_get;

	void (*RedInit)(struct s_Red*);
	int8_t (*RedIsEmpty)(const struct s_Red*);
	int8_t (*RedIsFull)(const struct s_Red*);
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

	RedInit = dlsym(handle_red, "RedInit");
	if ((error = dlerror()) != NULL)
	{
		fputs(error, stderr);
		exit(ERROR_DLSYM);
	}

	RedIsEmpty = dlsym(handle_red, "RedIsEmpty");
	if ((error = dlerror()) != NULL)
	{
		fputs(error, stderr);
		exit(ERROR_DLSYM);
	}

	RedIsFull = dlsym(handle_red, "RedIsFull");
	if ((error = dlerror()) != NULL)
	{
		fputs(error, stderr);
		exit(ERROR_DLSYM);
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

	/* Inicijalizacija reda */
	RedInit(&red);

	/* Provere popunjenost reda */
	red_prazan = RedIsEmpty(&red);
	assert(red_prazan == 1);

	red_pun = RedIsFull(&red);
	assert(red_pun == 0);

	/* Unos vrednosti u red */
	for (i = 0; i < broj_elemenata_za_unos; i++)
	{
		struct s_Koordinate koordinate;
		koordinate.x = i;
		koordinate.y = i;

		RedPut(&red, koordinate);
	}

	/* Preuzimanje elementa, FIFO */
	element_get = RedGet(&red);
	if (element_get == NULL)
	{
#ifdef __GNUC__
		fprintf(stderr, "%s: Getting element from red unsuccessful.\n", __func__);
#endif
		exit(ERROR_GETTING_ELEMENT);
	}
	assert(element_get != NULL);

	printf("(%"PRIi8", %"PRIi8")\n", element_get->x, element_get->y);
	assert(red.zauzeto_pozicija == broj_elemenata_za_unos - 1);

	/* Dinamicko "odvezivanje" tokom izvrsavanja */
#ifdef __GNUC__
	dlclose(handle_red);
#endif
}

void testiranjeUcitavanjaSlike(const char* naziv_fajla)
{
	int32_t i;
	int32_t j;

	int8_t slika[SLIKA_Y][SLIKA_X];
	int8_t ucitavanje_slike_status = -1;

	ucitavanje_slike_status = UcitajSliku(naziv_fajla, slika);
	if (ucitavanje_slike_status == 0)
	{
#ifdef __GNUC__
		fprintf(stderr, "%s: Loading image into matrix not successful.\n", __func__);
#endif
		return;
	}

	/* Pročitaj matricu samo ako je slika učitana */
	if (ucitavanje_slike_status == 1)
	{
		for (i = 0; i < SLIKA_Y; i++)
		{
			for (j = 0; j < SLIKA_X; j++)
			{
				printf("%"PRIi8"", slika[i][j]);
			}
			printf("\n");
		}
	}
}

void testiranjePronalazenjaKomsija(const char* naziv_fajla, const char* naziv_modula, const int8_t k_x, const int8_t k_y)
{
	assert((k_x >= INT8_MIN) && (k_x <= INT8_MAX));
	assert((k_y >= INT8_MIN) && (k_y <= INT8_MAX));

	int64_t i; /* Upotreba potencijalno ne prenosivog tipa */

	int8_t slika[SLIKA_Y][SLIKA_X];
	int8_t ucitavanje_slike_status = -1;

	void* handle_red;
	struct s_Red red;
	void (*RedInit)(struct s_Red*);
	char* error;

	struct s_Koordinate koordinate;

	int64_t broj_unetih_koordinata; /* Upotreba potencijalno ne prenosivog tipa */

	/* Učitavanje slike iz fajla */
	ucitavanje_slike_status = UcitajSliku(naziv_fajla, slika);
	if (ucitavanje_slike_status == 0)
	{
#ifdef __GNUC__
		fprintf(stderr, "%s: Loading image into matrix not successful.\n", __func__);
#endif
		return;
	}

	/* Dinamičko povezivanje modula */
#ifdef __GNUC__
	handle_red = dlopen(naziv_modula, RTLD_LAZY);
	if (handle_red == NULL)
	{
		fputs(dlerror(), stderr);
		exit(ERROR_DLOPEN);
	}

	RedInit = dlsym(handle_red, "RedInit");
	if ((error = dlerror()) != NULL)
	{
		fputs(error, stderr);
		exit(ERROR_DLSYM);
	}
#endif

	/* Inicijalizacija reda */
	RedInit(&red);

	/* Početne koordinate */
	koordinate.x = k_x;
	koordinate.y = k_y;

	/* Pronalaženje komšija */
	broj_unetih_koordinata = pronadjiKomsije(slika, &red, koordinate);

	/* Ispis reda */
	for (i = 0; i < broj_unetih_koordinata; i++)
	{
		printf("(%"PRIi8", %"PRIi8")\n", red.a_red[i].x, red.a_red[i].y);
	}

	/* Dinamicko "odvezivanje" tokom izvrsavanja */
#ifdef __GNUC__
	dlclose(handle_red);
#endif
}

void proveraPocetnihZahteva(void)
{
	/* Pretpostavka 3 */

	assert(SLIKA_X <= INT32_MAX);
	assert(SLIKA_Y <= INT32_MAX);
}
