/*
 * konstante.h
 *
 *  Created on: Oct 19, 2018
 *      Author: rtrk
 */

#ifndef KONSTANTE_H_
#define KONSTANTE_H_

/* KONSTANTE KOJE KORISTIK MENJA start */

/* Ime ulaznog fajla/slike */
static const char* IME_FAJLA_SLIKE = "slika.txt";

/* Ime biblioteke red */
static const char* IME_BIBLIOTEKE_RED = "libred.so";

/* Dimenzije ulazne slike */
#define SLIKA_X (10)
#define SLIKA_Y (8)

/* KONSTANTE KOJE KORISTIK MENJA end */

/* Tip reda */
typedef struct s_Koordinate TIP_REDA_t;

#define RED_VELICINA (SLIKA_X * SLIKA_Y)
#define SLIKA_VELICINA (SLIKA_X * SLIKA_Y)

/* Ucitavanje slike iz txt fajla */
#define ASCII_BROJEVI 48

/* Vrednosti grešnih izlazaka */
#define ERROR_DLOPEN 2
#define ERROR_DLSYM 3
#define ERROR_GETTING_ELEMENT 4
#define ERROR_IMAGE_INTO_MATRIX 5

#endif /* KONSTANTE_H_ */
