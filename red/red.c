/*
 * red.c
 *
 *  Created on: Oct 18, 2018
 *      Author: rtrk
 */

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#pragma RESET_MISRA ("all")

#include "red.h"

void RedInit(struct s_Red* red)
{
	red->zauzeto_pozicija = 0;

	red->p_upis = red->a_red;
	red->p_citanje = red->a_red;
}

int8_t RedIsEmpty(const struct s_Red* red)
{
	int8_t is_empty = -1;

	if (red->zauzeto_pozicija == 0)
	{
		is_empty = 1;
	}
	else
	{
		is_empty = 0;
	}

	return is_empty;
}

int8_t RedIsFull(const struct s_Red* red)
{
	int8_t is_full = -1;

	if (red->zauzeto_pozicija == RED_VELICINA)
	{
		is_full = 1;
	}
	else
	{
		is_full = 0;
	}

	return is_full;
}

int8_t RedPut(struct s_Red* red, const TIP_REDA_t element)
{
	int8_t is_put = -1;

	/* Proveravamo popunjenost */
	if (RedIsFull(red) == 1)
	{
		is_put = 0;
	}
	else
	{
		/* Umecemo vrednost */
		*(red->p_upis) = element;

		/* Povecavamo broj zauzetih mesta */
		red->zauzeto_pozicija++;

		/* Pomeramo pokazivač upisa */
		if (red->p_upis == &(red->a_red[RED_VELICINA]))
		{
			red->p_upis = red->a_red;
		}
		else
		{
			(red->p_upis)++;
		}

		is_put = 1;
	}

	return is_put;
}

TIP_REDA_t* RedGet(struct s_Red* red)
{
	TIP_REDA_t* element;

	if (RedIsEmpty(red) == 1)
	{
		element = NULL;
	}
	if (red->p_citanje == red->p_upis)
	{
		/* Ne možemo da čitamo sa lokacije na kojoj se očekuje upis */
		element = NULL;
	}
	else
	{
		TIP_REDA_t tmp;

		/* Uzimamo vrednost pokazivaca */
		tmp = *(red->p_citanje);

		/* Umanjujemo vrednost zauzetih pozicija */
		red->zauzeto_pozicija--;

		/* Pomeramo pokazivac za citanje */
		if (red->p_citanje == &(red->a_red[RED_VELICINA]))
		{
			red->p_citanje = red->a_red;
		}
		else
		{
			(red->p_citanje)++;
		}

		element = &tmp;
	}

	return element;
}
