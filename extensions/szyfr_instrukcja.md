# Zamki szyfrowe – dodatek `szyfr.h`

Dodatek do polskiej biblioteki inform6-pl, który pozwala otwierać drzwi, sejfy i skrzynie kodem: „otwórz sejf kodem 15432”, „wpisz 15432”. Biblioteka standardowa Inform 6 nie ma zamków szyfrowych, dlatego nie są one częścią biblioteki, tylko osobnym plikiem, który dołącza gra, która ich potrzebuje.

## Spis treści

1. [Dołączenie](#1-dołączenie)
2. [Obiekt z zamkiem szyfrowym](#2-obiekt-z-zamkiem-szyfrowym)
3. [Osobny obiekt zamka](#3-osobny-obiekt-zamka)
4. [Co może wpisać gracz](#4-co-może-wpisać-gracz)
5. [Jak wygląda kod](#5-jak-wygląda-kod)
6. [Zamykanie](#6-zamykanie)
7. [Własne reakcje i komunikaty](#7-własne-reakcje-i-komunikaty)
8. [Kod i klucz naraz](#8-kod-i-klucz-naraz)
9. [Co dodatek definiuje](#9-co-dodatek-definiuje)
10. [Przykład](#10-przykład)

---

## 1. Dołączenie

Dodatek dołącza się **po** `PolskiG`, bo rozszerza polskie czasowniki „otwórz” i „zamknij”:

```inform
Include "PolskiG";
Include "szyfr";
```

Kompilator musi znaleźć plik w katalogu `extensions/`. `build.sh` i `test.sh` mają go już na ścieżce. Przy kompilacji ręcznej trzeba go dopisać do `+include_path`:

```bash
narzedzia/bin/inform6 +include_path=lib,extensions,narzedzia/inform6lib moja_gra.inf
```

## 2. Obiekt z zamkiem szyfrowym

Zamek szyfrowy ma zwykły obiekt z zamkiem: atrybut `lockable` (ma zamek), zwykle `locked` (na początku jest zamknięty) i – jeśli da się go otworzyć jak drzwi czy sejf – `openable`. Do tego właściwość `with_code` – odpowiednik bibliotecznego `with_key`, tyle że zamiast klucza podaje kod:

```inform
Object -> sejf "stalow/y sejf/"
  with with_code "15432"
  has  static container openable lockable locked;
```

Kod może być napisem (`with_code "15432"`) albo liczbą (`with_code 42`); różnicę opisuje [rozdział 5](#5-jak-wygląda-kod). Kod można zmienić w czasie gry zwykłym przypisaniem, np. wylosować go na początku:

```inform
[ Initialise;
    ...
    sejf.with_code = 1000 + random(8999);     ! kod czterocyfrowy
];
```

W maszynie Z liczba może mieć najwyżej 32 767; dłuższe kody trzeba zapisać jako napis.

## 3. Osobny obiekt zamka

Jeśli w grze jest osobny przedmiot, do którego gracz wpisuje kod – klawiatura, tarcza, panel – wystarczy mu nadać właściwość `lock_for`, która wskazuje obiekt z kodem:

```inform
Object -> drzwi "pancern/e drzwi/"
  with with_code 42, door_to Korytarz, door_dir e_to
  has  scenery door openable lockable locked pluralname;

Object -> klawiatura "klawiatur/a"
  with name 'zamek' 'zamka' 'zamkiem' 'zamku',
       lock_for drzwi
  has  scenery female;
```

Wtedy działają i „otwórz drzwi kodem 42”, i „otwórz zamek kodem 42”, i „wpisz 42 na klawiaturze”. Kod (`with_code`) zawsze jest właściwością obiektu zamykanego (tu: drzwi), a nie zamka.

## 4. Co może wpisać gracz

| Polecenie | Co się dzieje |
|---|---|
| `otwórz sejf kodem 15432` | wprowadza kod; dobry kod otwiera zamek i sam sejf |
| `otwórz sejf szyfrem 15432`, `otwórz sejf kombinacją 15432` | to samo |
| `otwórz sejf 15432` | to samo |
| `otwórz zamek kodem 42` | to samo dla obiektu, który wskazuje `lock_for` |
| `wpisz 15432`, `wprowadź kod 15432`, `wystukaj 15432` | wprowadza kod do jedynego zamka szyfrowego w pobliżu; przy kilku gra pyta „Gdzie chcesz wprowadzić ten kod?” |
| `wpisz 42 na klawiaturze`, `wprowadź kod 42 w zamku` | wprowadza kod do wskazanego zamka |
| `zamknij sejf na szyfr` | zamyka sejf (najpierw go przymyka, jeśli jest otwarty) i zamek |
| `zamknij sejf kodem 15432` | to samo, ale tylko z dobrym kodem |

Odpowiedzi gry:

```
>otwórz sejf
Stalowy sejf jest zamknięty. Bez kodu nie da się go otworzyć.
>otwórz sejf kodem 11111
Wprowadzasz kod 11111, ale nic się nie dzieje.
>otwórz sejf kodem 15432
(najpierw wprowadzasz kod 15432)
Otwierasz stalowy sejf. W środku nic nie ma.
>zamknij sejf na szyfr
(najpierw zamykasz stalowy sejf)
Zamykasz stalowy sejf. Zamek zatrzaskuje się.
```

Obiekt z zamkiem bez `openable` (np. zamek szyfrowy na rowerze) po dobrym kodzie tylko się otwiera: „Wprowadzasz kod 4711 i zamek ustępuje.”

## 5. Jak wygląda kod

Biblioteczny token liczbowy (`number`) przyjmuje liczby tylko do 10 000 i gubi zera z przodu, dlatego dodatek czyta kod jako ciąg cyfr, do 16:

- „15432”, „0042” – zwykły zapis,
- „15-43-2” – myślniki są pomijane,
- „15 43 2” – kilka słów z samych cyfr też tworzy jeden kod.

Kod w grze zapisany **jako liczba** (`with_code 42`) nie zwraca uwagi na zera z przodu: pasuje „42”, „042” i „0042”. Kod zapisany **jako napis** (`with_code "0042"`) wymaga dokładnie tych cyfr: „0042” pasuje, „42” – nie. Myślniki i inne znaki w napisie kodu też są pomijane, więc `with_code "15-43-2"` znaczy to samo co `with_code "15432"`.

## 6. Zamykanie

„Zamknij sejf na szyfr” nie wymaga kodu – tak jak w prawdziwym sejfie wystarczy zatrzasnąć drzwiczki i przekręcić pokrętło. „Zamknij sejf kodem 15432” wymaga dobrego kodu. Jeśli gra chce, żeby sejf zamykał się na szyfr sam przy zwykłym „zamknij sejf”, wystarczy reguła `after`:

```inform
       after [; Close: give self locked; ],
```

„Zamknij sejf na klucz” przy obiekcie, który ma tylko kod, daje: „Ten zamek otwiera się kodem, a nie kluczem.”

## 7. Własne reakcje i komunikaty

Polecenia z kodem dają akcje `UnlockWithCode` (także „wpisz …”) i `LockWithCode`. Reguły `before` i `after` pisze się dla nich jak dla każdej innej akcji, w obiekcie zamykanym (sejfie, drzwiach).

**Własny tekst po otwarciu.** Reguła `after` dla `UnlockWithCode` działa po dobrym kodzie, gdy zamek i sejf są już otwarte, i zastępuje cały komunikat:

```inform
       after [;
           UnlockWithCode: "Zamek sejfu cicho szczęka i drzwiczki odskakują.";
       ],
```

**Sprawdzenie wpisanego kodu.** Reguła `before` może zareagować na wpisane cyfry, zanim dodatek sprawdzi kod. Służą do tego dwie procedury:

- `SzyfrDlugosc()` – ile cyfr wpisał gracz,
- `SzyfrWpisano(x)` – czy wpisał kod `x` (napis albo liczbę, z tymi samymi zasadami co `with_code`).

```inform
       before [;
           UnlockWithCode:
               if (SzyfrDlugosc() ~= 5) "Klawiatura przyjmuje tylko pięć cyfr.";
               if (SzyfrWpisano("00000")) "Sejf nie jest aż tak naiwny.";
       ],
```

W tekstach można też wypisać kod, który wpisał gracz: `print "Na wyświetlaczu miga ", (SzyfrWpisany) 0, ".";`. (Zmiennych dodatku nie da się użyć w kodzie gry bezpośrednio, bo dodatek dołącza się na końcu, a Inform wymaga zadeklarowania zmiennej przed użyciem; procedury można wywoływać z wyprzedzeniem.)

**Zmiana komunikatów dodatku.** Komunikaty dodatku zmienia się tak samo jak komunikaty biblioteki, w obiekcie `LibraryMessages` gry (zdefiniowanym między `Include "Parser"` a `Include "VerbLib"`):

```inform
Object LibraryMessages
  with before [;
           UnlockWithCode: if (lm_n == 2) "Klawiatura gniewnie piszczy.";
       ];
```

Numery komunikatów `UnlockWithCode`:

| `lm_n` | Kiedy | Tekst |
|---|---|---|
| 1 | obiekt nie ma kodu | „Tego nie otwiera się kodem.” |
| 2 | zły kod | „Wprowadzasz kod 11111, ale nic się nie dzieje.” |
| 3 | dobry kod, przed otwarciem | „(najpierw wprowadzasz kod 15432)” |
| 4 | dobry kod, obiekt bez `openable` | „Wprowadzasz kod 4711 i zamek ustępuje.” |
| 5 | „wpisz” bez wskazania, kilka zamków | „Gdzie chcesz wprowadzić ten kod?” |

`LockWithCode`, numer 1: „Zamykasz stalowy sejf. Zamek zatrzaskuje się.” Dodatek zmienia też cztery komunikaty biblioteki dla obiektów z kodem, żeby nie mówiły o kluczu: „Stalowy sejf jest zamknięty. Bez kodu nie da się go otworzyć.” (`Open` 2), „Stalowy sejf jest już zamknięty.” (`Lock` 2), „Stalowy sejf nie jest zamknięty.” (`Unlock` 2) i „Ten zamek otwiera się kodem, a nie kluczem.” (`Lock` 6).

## 8. Kod i klucz naraz

Obiekt może mieć jednocześnie `with_code` i `with_key` – jak sejf, który otwiera się kodem albo kluczem awaryjnym. Działa wtedy wszystko naraz: „otwórz sejf kodem 1234”, „otwórz sejf kluczem”, samo „otwórz sejf” z kluczem przy sobie, „zamknij sejf na klucz” i „zamknij sejf na szyfr”.

## 9. Co dodatek definiuje

Dodatek dodaje do gry nazwy, których gra nie może używać po swojemu:

- właściwości `with_code` i `lock_for` (gra może ich używać w swoich obiektach – właśnie po to są); nazwy są angielskie, jak wszystkie właściwości biblioteki (`with_key`, `door_to`),
- akcje `UnlockWithCode`, `LockWithCode` i `EnterCode`,
- czasowniki `wpisz`, `wprowadź`, `wystukaj` – gra, która chce dodać do nich własne znaczenia, musi użyć `Extend 'wpisz'`, a nie `Verb 'wpisz'` (inaczej kompilator zgłosi błąd), i dołączyć `szyfr` przed tym `Extend`,
- linie gramatyki dla „otwórz” i „zamknij” (przez `Extend`),
- procedury i zmienne z przedrostkiem `Szyfr` / `szyfr_` oraz obiekt `szyfr_rozszerzenie` w `LibraryExtensions`.

Bezokoliczniki nowych czasowników („Co chcesz wpisać?”) dodatek podaje sam, przez mechanizm rozszerzeń biblioteki, więc gra nie musi o nie dbać w swoim `PrintVerb`.

## 10. Przykład

`extensions/szyfr_przyklad.inf` to mała gra pokazująca wszystko powyższe: sejf z kodem w postaci napisu i własnym tekstem otwarcia, drzwi z kodem liczbowym i osobna klawiatura. `extensions/szyfr_przyklad.przejscie` przechodzi ją do zwycięstwa; `test.sh` sprawdza ją w maszynie Z i w Glulx, z polskimi literami i bez nich.

```bash
narzedzia/bin/inform6 +include_path=lib,extensions,narzedzia/inform6lib extensions/szyfr_przyklad.inf build/szyfr_przyklad.z5
narzedzia/bin/dfrotz build/szyfr_przyklad.z5
```
