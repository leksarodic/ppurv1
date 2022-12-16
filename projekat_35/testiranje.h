/*
 * testiranje.h
 *
 *  Created on: Oct 18, 2018
 *      Author: rtrk
 */

#ifndef TESTIRANJE_H_
#define TESTIRANJE_H_

#pragma CHECK_MISRA ("none")
#include <stdint.h>
#pragma RESET_MISRA ("all")

/* Upotreba potencijalno ne prenosivog tipa, INT64_MAX */
void testiranjeRedova(const int64_t broj_elemenata_za_unos);

void testiranjeUcitavanjaSlike(const char* naziv_fajla);
void testiranjePronalazenjaKomsija(const char* naziv_fajla, const char* naziv_modula, const int8_t k_x, const int8_t k_y);

void proveraPocetnihZahteva(void);

#endif /* TESTIRANJE_H_ */
