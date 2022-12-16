/*
 * Programska podrška u realnom vremenu
 * Projektni zadatak broj 35
 *
 * Aleksa Rodić
 * RA218-2015
 *
 * oktobar, 2018.
 *
 * https://svn.riouxsvn.com/ppurv_35_2018
 *
 * # Zadatak
 * ## Kratak opis
 * Program učitava sliku iz datoteke u formi matrice jedinica i nula.
 * Na osnovu zadatih koordinata pronalazi i ispisuje susede/piksele istih
 * vrednosti kao i piksele koji su susedi njegovim istovrednosnim susedima...
 *
 * Ulaz: Slika/tekstualni dokument u formi matrice, početne koordinate
 * Izlaz: Ispis strukture podataka red sa vrednostima koordinata suseda
 *
 * ## Algoritam
 *
 *   0123456789
 *  +----------+  0                  (4,4)
 * 0|0000000000|
 * 1|0000000000|  1         (3,4) (4,5) (5,4) (4,3)
 * 2|0011011000|
 * 3|1001111000|  2         (3,3) (4,6) (5,5) (5,3)
 * 4|0101110011|
 * 5|0100110011|  3         (3,2) (4,7) (5,2) (6,3)
 * 6|0000101000|
 * 7|0001100000|  4            (2,2) (3,7) (6,2)
 *  +----------+
 *
 * # Pretpostavke
 *
 * 1
 * Pretpostavljamo da će vrednosti unete u fajl biti vrednosti iz ASCII tabele (ne veće od 0x7F).
 * Funkcija fgetc dokvata vrednost od koje se oduzima 48, što je korak pretvaranja pročitane
 * vrednosti na binarnu vrednost (matrica nula i jedinica).
 *
 * 2
 * Pretpostavljamo da je veličina slike unapred poznata.
 *
 * 3
 * Pretpostavljamo da veličina slike u jednom smeru neće biti veća od maksimalne vrednosti
 * koju može da primi promenjiva tipa int32_t.
 *
 * # Bildovanje
 * ## Build proces
 *
 * 1
 * Nakon što uvezemo projekte u workspace potrebno je povratiti sistemske putanje
 * za oba projekata, "red" i "projekat_35".
 *
 * Project -> Properties -> C/C++ General -> Paths and Symbols -> Restore Defaults
 *
 * 2
 * Potrebno je podesiti Include Paths iz uvezenih projekata.
 *
 * Project -> Properties -> C/C++ Build -> Settings -> Tool Settings -> GCC C Compiler ->
 * Includes -> Include paths (-I)
 *
 * Projektu "projekat_35" potrebno je dodati:
 * "${workspace_loc:/red}"
 * "${workspace_loc:/${ProjName}}"
 *
 * Projektu "red" potrebno je dodati:
 * "${workspace_loc:/projekat_35}"
 * "${workspace_loc:/${ProjName}}"
 *
 * 3
 * Potrebno je povezati biblioteku za dinamičko uvezivanje modula.
 *
 * Project -> Properties -> C/C++ Build -> Settings -> Tool Settings -> -> GCC C Linker ->
 * Libraries -> Libraries (-l)
 *
 * Projektu "projekat_35" dodajemo:
 * dl
 *
 * ## Podešavanje konstante.h
 * Prilikom buildovanja potrebno je u zaglavlju konstante.h podesiti sledeće parametre:
 *
 * 1
 * IME_FAJLA_SLIKE, putanja i ime
 *
 * 2
 * IME_BIBLIOTEKE_RED, ime
 *
 * 3
 * Dimenzije slike SLIKA_X i SLIKA_Y, pretpostavka 3
 *
 * ## Debug configuration
 * Potrebno je i da podesimo putanju do biblioteke koju dinamički uvezujemo
 *
 * Putanju podešavamo na sledeći način:
 * Run -> Debug Configurations... -> C/C++ Application -> projekt_35 ->
 * Environment -> New ->
 * Name: LD_LIBRARY_PATH
 * Value: ${workspace_loc:/red/Debug}
 * -> OK -> Apply
 *
 * ## Post build
 * Nakon kompajliranja, linkovanja i dobijanja izvršne datoteke, potrebno je
 * pokrenitu TI C6000 kompajler koji će kod samo da kompajlira i obavesti
 * nas o usklađenosti sa MISRA standardom.
 *
 * Project -> Properties -> C/C++ Build -> Settings -> Build Steps -> Post-build steps -> Command
 *
 * $TI_MISRA_PUTANJA = /home/rtrk/tools/c6000_7.4.23
 *
 * Projektu "projekat_35" i "red" dodajemo:
 * $TI_MISRA_PUTANJA/bin/cl6x
 * --include_path=$TI_MISRA_PUTANJA/include
 * --include_path=${workspace_loc:/projekat_35}
 * --include_path=${workspace_loc:/red}
 * --compile_only --check_misra=all $(C_SRCS)
 *
 * # Upotreba programa
 * Prvi korak je podešavanje konstante.h.
 *
 * Korisnik u main.c funkciji postavlja vrednosti koordinata za koje želi da pronađe
 * istovrednosne susede. Deo koda je obeležen sa "KORISNIK PODEŠAVA: Početne koordinate".
 *
 * Potrebno je podesiti LD_LIBRARY_PATH putanju.
 *
 * # Testiranje
 * ## Upotreba assert makro funkcije
 * Kako bi bili sigurni da smo zadovoljili pretpostavke sa početka prilikom testiranja
 * upotrebljavamo asserte koji je okončati izvršavanje programa ukoliko neki od
 * uslova nije ispunjen.
 *
 * U cilju temeljnijeg testiranja, koristio sam kratak tutorial
 * https://ptolemy.berkeley.edu/~johnr/tutorials/assertions.html
 *
 * ## Nekoliko ulaza
 * Funkcijama deklaisanim u testiranje.h možemo manuelno da vršimo provere funkcionalnosti.
 * Automatsko testiranje zasnovano na proverama ulaza i izlaza nije implementirano.
 *
 * # MISRA
 * U izradi zadatka kršimo sledeća pravila:
 *
 * 1
 * 20.9
 * Kako bi kompletirali zadatak ("...onda program treba da pronađe i ispiše
 * koordinate osenčenih piksela.") kršimo pravilo 20.9 korišćenjem funkcija:
 * fopen, fputs, stderr, feof, fgetc, fclose, printf, fprintf
 *
 * 2
 * 5.2
 * Koristimo dinamičko povezivanje i pokazivač na funkciju RedInit. Potrebno nam
 * je da funkcije nose isto ime kako bi nam bilo jasnije koju funkciju pozivamo
 *
 * 3
 * TODO: Razrešiti sledeća pravila: 17.6, 17.4, 10.1, 14.7, 16.3, 9.1, 8.5
 *
 * # Neprenosivi delovi koda
 *
 * 1
 * Upotreba int64 tipa podataka. Postoji mogućnost da na nekoj platformi za koju
 * na koju budemo preneli projekat, tip int64 ne postoji.
 * Upotreba int64 tipa je naznačena na svakom mestu primene.
 *
 * 2
 * Postojenje dlfnc biblioteke zavisi od platforme, ne spada u C99 standardne biblioteke.
 * https://linux.die.net/man/3/dlopen
 *
 * 3
 * Postojanje __func__ zavisi do kompajlera
 * https://gcc.gnu.org/onlinedocs/gcc/Function-Names.html
 *
 * # Potencijalni problemi
 * Program može da koristi veliku količinu memorije, zavisno od dimenzija slike.
 * Brzina programa i zauzetost memorije rastu zbog statičkog zauzimanja niza.
 * Ukoliko algoritam pronalazi mnogo polja postoji opravdanost za statičko zauzimanje,
 * ali ukoliko pronalazi samo jednu vrednost, opravdanost zauzimanja velike količine
 * resursa je potpuno neopravdana.
 *
 */

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#ifdef __GNUC__
#include <dlfcn.h>
#endif
#pragma RESET_MISRA ("all")

#include "konstante.h"

#include "testiranje.h"
#include "koordinate.h"
#include "red.h"
#include "komsije.h"
#include "fajlovi.h"

#define POKRENI_TESTOVE 0
#define POKRENI_PROJEKAT 1

int main(void);

int main(void)
{
	printf("Projektni zadatak broj 35\n");

	/* Provera koju izvravamo na početku po pretpostavci 3 */
	proveraPocetnihZahteva();

#if POKRENI_TESTOVE
	printf(">> Testiranje redova\n");

	/* Unosimo 0 elemenata, a kroz funkciju hoćemo da pročitamo jedan/prvi */
	testiranjeRedova(0);

	/* Unosimo mnogo elemenata iako smo definisali kao maksimum SLIKA_X * SLIKA_Y */
	testiranjeRedova(INT16_MAX);

	/* Unosimo negativan broj elemenata */
	testiranjeRedova(-1);

	printf(">> Testiranje učitavanja slike\n");

	/* Fajl ne postoji */
	testiranjeUcitavanjaSlike("fajl_koji_ne_postoji.txt");

	/* Fajl postoji */
	testiranjeUcitavanjaSlike("slika.txt");

	printf(">> Testiranje pronalaženja komšija\n");

	/* Fajl ne postoji */
	testiranjePronalazenjaKomsija("fajl_koji_ne_postoji.txt", "libred.so", 9, 5);

	/* Vrednosti su validne ali negativne */
	testiranjePronalazenjaKomsija("slika.txt", "libred.so", -100, -100);

	/* Pogrešan naziv modula */
	testiranjePronalazenjaKomsija("slika.txt", "pogresan_modul.so", 9, 5);

	/* Pogrešan naziv fajla slike, modula i koordinate van opsega */
	testiranjePronalazenjaKomsija("fajl_koji_ne_postoji.txt", "pogresan_modul.so", 100, 100);

#endif

#if POKRENI_PROJEKAT

	int64_t i; /* Upotreba potencijalno ne prenosivog tipa */

	int8_t slika[SLIKA_Y][SLIKA_X];
	int8_t ucitavanje_slike_status = 0;

	void* handle_red;
	struct s_Red red;
	void (*RedInit)(struct s_Red*);
	char* error;

	struct s_Koordinate koordinate;

	int64_t broj_unetih_koordinata; /* Upotreba potencijalno ne prenosivog tipa */

	/* Učitavanje slike iz fajla */
	ucitavanje_slike_status = UcitajSliku(IME_FAJLA_SLIKE, slika);
	if (ucitavanje_slike_status == 0)
	{
#ifdef __GNUC__
		fprintf(stderr, "%s: Loading image into matrix not successful.\n", __func__);
#endif
		return ERROR_IMAGE_INTO_MATRIX;
	}

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
#endif

	/* Inicijalizacija reda */
	RedInit(&red);

	/* KORISNIK PODEŠAVA: Početne koordinate */
	koordinate.x = 4;
	koordinate.y = 4;

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

#endif

	return 0;
}
