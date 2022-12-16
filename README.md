```text
Univerzitet u Novom Sadu
Fakultet tehničkih nauka
Računarska tehnika i računarske komunikacije
```

## Programska podrška u realnom vremenu 1
### Projektni zadatak broj 35

### Kratak opis

Program učitava sliku iz datoteke u formi matrice jedinica i nula.

Na osnovu zadatih koordinata pronalazi i ispisuje susede/piksele istih vrednosti kao i piksele koji su susedi njegovim istovrednosnim susedima...

```text
Ulaz: Slika/tekstualni dokument u formi matrice, početne koordinate
```

```text
Izlaz: Ispis strukture podataka red sa vrednostima koordinata suseda
```

#### Algoritam

```text
0123456789
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
```

#### Pretpostavke

1. Pretpostavljamo da će vrednosti unete u fajl biti vrednosti iz ASCII tabele (ne veće od `0x7F`).
Funkcija fgetc dokvata vrednost od koje se oduzima 48, što je korak pretvaranja pročitane vrednosti na binarnu vrednost (matrica nula i jedinica).

2. Pretpostavljamo da je veličina slike unapred poznata.

3. Pretpostavljamo da veličina slike u jednom smeru neće biti veća od maksimalne vrednosti koju može da primi promenjiva tipa `int32_t`.

### Bildovanje

#### Build proces

1. Nakon što uvezemo projekte u workspace potrebno je povratiti sistemske putanje za oba projekata, `red` i `projekat_35`.

```text
Project -> Properties -> C/C++ General -> Paths and Symbols -> Restore Defaults
```

2. Potrebno je podesiti Include Paths iz uvezenih projekata.

```text
Project -> Properties -> C/C++ Build -> Settings -> Tool Settings -> GCC C Compiler -> Includes -> Include paths (-I)
```

Projektu `projekat_35` potrebno je dodati:

```text
${workspace_loc:/red}
${workspace_loc:/${ProjName}}
```

Projektu `red` potrebno je dodati:

```text
${workspace_loc:/projekat_35}
${workspace_loc:/${ProjName}}
```

3. Potrebno je povezati biblioteku za dinamičko uvezivanje modula.

```
Project -> Properties -> C/C++ Build -> Settings -> Tool Settings -> -> GCC C Linker -> Libraries -> Libraries (-l)
```

Projektu `projekat_35` dodajemo:

```text
dl
```

#### Podešavanje konstante.h

Prilikom buildovanja potrebno je u zaglavlju konstante.h podesiti sledeće parametre:

1. `IME_FAJLA_SLIKE`, putanja i ime

2. `IME_BIBLIOTEKE_RED`, ime

3. Dimenzije slike `SLIKA_X` i `SLIKA_Y`, pretpostavka `3`

#### Debug configuration

Potrebno je i da podesimo putanju do biblioteke koju dinamički uvezujemo

Putanju podešavamo na sledeći način:

```text
Run -> Debug Configurations... -> C/C++ Application -> projekt_35 ->
Environment -> New ->

Name: LD_LIBRARY_PATH

Value: ${workspace_loc:/red/Debug}

-> OK -> Apply
```

#### Post build

Nakon kompajliranja, linkovanja i dobijanja izvršne datoteke, potrebno je
pokrenitu TI C6000 kompajler koji će kod samo da kompajlira i obavesti
nas o usklađenosti sa MISRA standardom.

```text
Project -> Properties -> C/C++ Build -> Settings -> Build Steps -> Post-build steps -> Command
```

```text
$TI_MISRA_PUTANJA = /home/rtrk/tools/c6000_7.4.23
```

Projektu `projekat_35` i `red` dodajemo:

```make
$TI_MISRA_PUTANJA/bin/cl6x
--include_path=$TI_MISRA_PUTANJA/include
--include_path=${workspace_loc:/projekat_35}
--include_path=${workspace_loc:/red}
--compile_only --check_misra=all $(C_SRCS)
```

### Upotreba programa

Prvi korak je podešavanje `konstante.h`.

Korisnik u `main.c` funkciji postavlja vrednosti koordinata za koje želi da pronađe istovrednosne susede. Deo koda je obeležen sa `KORISNIK PODEŠAVA: Početne koordinate`.

Potrebno je podesiti `LD_LIBRARY_PATH` putanju.

### Testiranje

#### Upotreba assert makro funkcije

Kako bi bili sigurni da smo zadovoljili pretpostavke sa početka prilikom testiranja upotrebljavamo asserte koji je okončati izvršavanje programa ukoliko neki od uslova nije ispunjen.

U cilju temeljnijeg testiranja, koristio sam kratak tutorial https://ptolemy.berkeley.edu/~johnr/tutorials/assertions.html.

#### Nekoliko ulaza

Funkcijama deklaisanim u testiranje.h možemo manuelno da vršimo provere funkcionalnosti.

Automatsko testiranje zasnovano na proverama ulaza i izlaza nije implementirano.

### MISRA

U izradi zadatka kršimo sledeća pravila:

1. 20.9
    * Kako bi kompletirali zadatak ("...onda program treba da pronađe i ispiše koordinate osenčenih piksela.") kršimo pravilo 20.9 korišćenjem funkcija: `fopen`, `fputs`, `stderr`, `feof`, `fgetc`, `fclose`, `printf`, `fprintf`

2. 5.2
    * Koristimo dinamičko povezivanje i pokazivač na funkciju RedInit. Potrebno nam je da funkcije nose isto ime kako bi nam bilo jasnije koju funkciju pozivamo

3. Razrešiti sledeća pravila: 17.6, 17.4, 10.1, 14.7, 16.3, 9.1, 8.5

### Neprenosivi delovi koda

1. Upotreba int64 tipa podataka. Postoji mogućnost da na nekoj platformi za koju na koju budemo preneli projekat, tip int64 ne postoji. Upotreba int64 tipa je naznačena na svakom mestu primene.

2. Postojenje dlfnc biblioteke zavisi od platforme, ne spada u C99 standardne biblioteke https://linux.die.net/man/3/dlopen.

3. Postojanje `__func__` zavisi do kompajlera https://gcc.gnu.org/onlinedocs/gcc/Function-Names.html.

### Potencijalni problemi

Program može da koristi veliku količinu memorije, zavisno od dimenzija slike.

Brzina programa i zauzetost memorije rastu zbog statičkog zauzimanja niza.

Ukoliko algoritam pronalazi mnogo polja postoji opravdanost za statičko zauzimanje, ali ukoliko pronalazi samo jednu vrednost, opravdanost zauzimanja velike količine resursa je potpuno neopravdana.

### Beleške sa predavanja

* Bajt - ugriz. Koliko mozemo da dohvatimo memorije u jednom pristupu
* 64 bita racunar - sirina memorije je 64 bita, moze da upise/cita 64 bita, registri su 64 bitni
* `stdint.h` zahlavlje (eleminaciono pitanje). `intN_t`, `uintN_t` ili `int_leastN_t` ili `int_fastN_t`
* `intptr_t` ako je adresa 256 bita, a podatak moze biti 64, tako da nemamo mogucnost da imamo pokazivacku promenjivu 
* U zavisnosti od platforme kada napisemo `char` on moze biti ounacen ili neoznacen, ali kada napisemo `int` on je uvek oznacen
* `limits.h` `CHAR_MIN`
* Ako nam treba char za aritmetiku, uvek koristiti `int8_t`
* `float` i `double`, ako kastujemo `int32` na float koji je 32 bita, mozemo da izgubimo vrednosti zato sto ce celobrojni deo smesta u mantisu koja je kod floata 23 bita
* Koriscenje enum tipa
* Literali imaju svoj tip (broj)
* `c` je tipa int
* `INT16_C` (proveriti sta radi) ?
* `volatile int x;` promenjiva moze biti promenjena od spolja, na neli drugi nacin, promenjiva moze biti promenjena bez njegovog znanja
* Sta je pozivna funkcija (bude na testu), pozivna konvencija
* `sizeof` je operacija kao i plus/minus
* `sizeof` nikada ne generise instrukcije, rezultat je konkretan broj koji se sracunava u toku kompajliranja
* Tip vrednosti koju sizeof sracunava je `size_t`
* `ptr +- n` <> `ptr +- n * sizeof(data_type)` gde je leva strana ceovsko plus i minus, a desna matematicko
* Tip izraza pokazivac minus pokazivac je `ptrdiff_t `
* `[]` je operator, rezultat tipa pokazivac
* `A[B]` <> `*(A + B)`, ima smisla samo ako je A ili B ceo broj
* Operacija unarne zvezdice `* `dereferencijaranje, tip pokazivac
* `x = p[4]` <> `x = 4[p]`
* `char string[] = "abcde"` onda je velicina =6, na kraj se "ispod haube" doda `\0` na kraj
* `char* string = "abcde"`, kompajler vodi racuna gde ce da smesti niz u memoriji. Promena vrednosti moze dovesti do nedefinisanih stanja. 
* `printf` i `scanf` su zabranjeni u namenskim racunarskim sistemima. Problem je kod `printf` sto kada nema `NULL` karakter na kraju ne zna koliko memorije treba da cita. Kod scanf ako upisujemo u niz od 5 mesta, a upisemo vise od 5 onda pisemo po tudjoj memoriji
* `char string[100]` moze da stane `100-1` znakova gde je poslednji `\0`
* Zamena `const char` eksplicitnom konverzijom
* `malloc(100)` zauzima 100 bajta. Ako hocemo `int` onda `malloc(100 * sizeof(data_type)`
* `char* p = (char*) malloc(100)` da li moze (char)?
* `stecmp` vraca razliku od prvog razlicitog znaka, vrednost moze biti pozitivna i negativna. Daje nam leksikografsko poredjenje
* Eleminaciono pitanje, koliko bita ima u bajtu
* Poravnanje - sa kojim brojm je deljiva adresa. `011101011` adresa je deljiva sa 2 ako su najniza 3 bita =0. Poravnanje je hardverska osobina
* Od redosleda polja u strukturi zavisi poravnanje. Da bi smanjili broj rupa, mozemo rucno da preuredimo strukturu. 
* Zakljucavaju se binarne datoteke zato sto tu nemamo redove, SVN nju poslatra kao "monolitnu" jefinstvrnu celinu
* Nece biti SVN na ispitu, ali je bitno za projekt
* Makro koji se ne pise velikim slovom, `va_args`
* Include i define se desavaju na nivou predprocesora
* `#define MIN(a, b) (a > b) ? b : a` | ako napisemo `MIN(x++, y)` to ce se umentnuti u tekst kao `(x++, y) ? y : (x++)` sto nam daje nezeljeno izvrsavanje. Resenje je korscenje `inline` funkcije
* `.so` shared object, deljena biblioteka
* Kod kružnog bafera, popunjenost mozemo pratiti pomocu dodatne promenive i tu imamo potpunu iskoriscenost ovako opada brzina ali ovako znamo koliko elemenata ima u baferu, drugi je ako rear pokazuje na jedan manje od front (onda gubimo jedno mesto/žrtvujemo jednu poziciju) ovde mozemo dobiti informaciju o broje elemenata ali moramo "paziti na memoriju da li je uzastopna"
* Niz je ogranicen najvecim kontinualnim blokom memorije
* Prolazak kroz niz je daleko brzi nego kroz povezanu listu zato sto kompajler uradi optimizaciju pristupa elementima zato sto je for petlja veoma dobro formirana (kompajler zna koliko ce se puta izvrsiti for petlja, mora da postoji hw podrska). Brzina se postize kroz upotrebu kesa i predikciju pristupa
* Jeftinije je proveriti da li je indeks 0 nego -1, primer za stek
* Citanje jedne adrese iza poslednje adrese niza je OK po standardu, ali upis naravno nije ok
* Assert - pretpostavka, pretpostavljamo da sledeci kod treba tacno to da radi. Cilj nije hvatanje gresaka, nego postoji neki sistemski problem
* `assert` radi samo u debug rezimu
* `NDEBUG` postoji u `c99` standardu
* Assert informacije se oslanjaju na, unarnu tarabu, makroe `__FUNC__` i date i file i ostale
* Formalno testiranje
