# inform6-pl

[![testy](https://github.com/zajaczkowski/inform6-pl/actions/workflows/testy.yml/badge.svg)](https://github.com/zajaczkowski/inform6-pl/actions/workflows/testy.yml)

Polska wersja biblioteki standardowej **Inform 6**, napisana od nowa dla biblioteki **6.12.8** (grudzień 2025) i kompilatora **Inform 6.44**. Gry napisane z jej pomocą odmieniają nazwy przedmiotów przez przypadki, rozumieją polecenia wpisane naturalną polszczyzną i działają w maszynie Z oraz w Glulx.

Poprzednia wersja, oparta na bibliotece 6/11, znajduje się w repozytorium [inform-pl](https://github.com/zajaczkowski/inform-pl).

> **In English:** a Polish translation of the Inform 6 Standard Library 6.12.8 for the Inform 6.44 compiler. It declines object names automatically through all six cases used in the game (the notation is inspired by the Russian RInform), understands commands typed with or without Polish diacritics, and works on both the Z-machine and Glulx. All example games from the IF Archive are translated and come with walkthrough scripts that `./test.sh` runs. Licensed like the Inform library itself (traditional Inform licence or Artistic License 2.0).

Dokumentacja:

- [docs/podrecznik.md](docs/podrecznik.md) – podręcznik pisania gier z tą biblioteką, prowadzący krok po kroku przez kod polskich „Ruin”,
- [docs/suplement.md](docs/suplement.md) – suplement o budowaniu poszczególnych mechanizmów, z przykładami ze wszystkich gier przykładowych,
- [docs/podrecznik_gracza.md](docs/podrecznik_gracza.md) – podręcznik dla graczy: jak grać i jakie polecenia rozumieją gry,
- [CHANGELOG.md](CHANGELOG.md) – historia wydań,
- [POCHODZENIE.md](POCHODZENIE.md) – skąd pochodzą poszczególne części biblioteki (poprzednia wersja 6/11/PL, metoda odmiany zapożyczona z rosyjskiego RInform, projekty źródłowe) i w czym pomagała sztuczna inteligencja (Claude firmy Anthropic).

## Założenia

- **Biblioteka bazowa pozostaje nietknięta.** Pliki `parser.h`, `verblib.h` i pozostałe są używane bez żadnych zmian, a cała polska część mieści się w plikach językowych w `lib/`. Przejście na kolejne wydanie biblioteki nie wymaga więc przenoszenia żadnych poprawek.
- **Źródła są w UTF-8** (opcja kompilatora `-Cu`). Wersja legacy używała ISO-8859-2, co utrudniało edycję w nowoczesnych edytorach i wymianę plików.
- **Obsługiwane są obie maszyny wirtualne:** Z-machine (`.z5`) i Glulx (`.ulx`), z jednym ograniczeniem opisanym niżej.

## Struktura

```
lib/polskiz.h    polskie znaki: alfabet maszyny Z i tablica Unicode (dołączany jako pierwszy)
lib/polski.h     plik definicji języka: słownictwo, odmiana przez przypadki, komunikaty
lib/odmiana.h    automatyczna odmiana nazw zapisanych z "/" (dołączany przez polski.h)
lib/polskig.h    gramatyka: polskie czasowniki i polecenia meta
games/           gry: ruins_pl, alice_pl, toyshop_pl, advent_pl, balances_pl,
                 museum_pl, adventureland_pl (*.inf), gra testowa test.inf
                 oraz skrypty przejść *.przejscie
docs/            podręcznik autora, suplement i podręcznik gracza
przygotuj.sh     pobiera i buduje narzędzia (do katalogu narzedzia/)
build.sh         kompiluje jedną grę
test.sh          kompiluje wszystkie gry i sprawdza je skryptami przejść
sciezki.sh       ustala, gdzie są narzędzia (używany przez build.sh i test.sh)
```

## Wymagania i kompilacja

Do kompilacji potrzebne są kompilator Inform 6.44 i biblioteka standardowa 6.12.8, a do testów – interpreter dfrotz. Najprościej pobrać i zbudować wszystkie trzy jednym skryptem; potrzebne są do tego tylko `git`, kompilator C i `make`:

```sh
./przygotuj.sh
```

Narzędzia trafiają do katalogu `narzedzia/`, który nie jest częścią repozytorium. Kto ma już własny kompilator i bibliotekę, może zamiast tego ustawić zmienne `INFORM6`, `INFORM6LIB` i `DFROTZ` (szczegóły w `sciezki.sh`).

**Linux i inne systemy rozróżniające wielkość liter.** Pliki bibliotek mają nazwy pisane małymi literami (`parser.h`, `polskiz.h`), a gry dołączają je tradycyjnie jako `"Parser"` czy `"PolskiZ"`. Kompilator nie zmienia wielkości liter, więc – zgodnie z zaleceniem biblioteki Inform – potrzebne są dowiązania `Parser.h`, `VerbLib.h`, `PolskiZ.h`, `PolskiG.h` itd. `przygotuj.sh` tworzy je sam, jeśli wykryje taki system. Kto używa własnej instalacji biblioteki, musi je utworzyć ręcznie. Na macOS i Windows dowiązań tworzyć nie wolno, bo wskazywałyby same na siebie.

Kompilacja gry:

```sh
./build.sh test        # build/test.z5
./build.sh test -G     # build/test.ulx
./build.sh test -D     # wersja z poleceniami debugowania
```

Gra dołącza bibliotekę w takiej kolejności:

```inform
!% -Cu
!% +language_name=polski
Include "PolskiZ";        ! koniecznie pierwszy, przed Constant Story
Constant Story "...";
Include "Parser";
Include "VerbLib";
! ... obiekty gry ...
Include "PolskiG";
```

## Polskie znaki

- **Z-machine.** `polskiz.h` przestawia alfabet maszyny Z tak, że polskie małe litery trafiają do alfabetu A1. Litera spoza alfabetu zajmuje w słowniku cztery z-znaki, a litera z alfabetu A1 tylko dwa, więc słowa z ogonkami mieszczą się w dziewięciu z-znakach słownika i nie tracą końcówek: „ścianę” i „ściany” pozostają dla parsera różnymi słowami. Plik musi być dołączony przed jakimkolwiek tekstem, bo zmienia sposób kodowania wszystkich napisów; tekst skompilowany wcześniej byłby zniekształcony. Bez niego `polski.h` przerywa kompilację z odpowiednim komunikatem.
- **Glulx.** Słownik musi przechowywać znaki Unicode (`$DICT_CHAR_SIZE=4`); `build.sh` dodaje tę opcję samodzielnie, gdy podano `-G`.
- Wpisywanie ą, ę, ł, ś, ź, ż, także wielkimi literami, sprawdzono w dfrotz:

```sh
narzedzia/bin/dfrotz -m build/test.z5
```

## Odmiana przez przypadki

**Automatyczna (`lib/odmiana.h`).** Autor zaznacza w nazwie obiektu ukośnikiem granicę między rdzeniem a końcówką, a biblioteka sama tworzy formy wszystkich przypadków i rozpoznaje każdą z nich w poleceniu gracza, także wpisaną bez polskich liter. Właściwość `name` służy już tylko synonimom.

```inform
Object lampa  "mosiężn/a lamp/a"     has female;           ! mosiężnej lampy, mosiężną lampę…
Object stol   "st/ó=o/ł/+u"          has supporter;        ! stołu, stołowi, stołem, stole
Object klebek "kłęb/e/k/ wełny";                           ! kłębka wełny, kłębkiem wełny
Object pies   "wielk/i p/ie/s/"      has animate male;     ! wielkiego psa, wielkim psie
Object k      "Czarn/a! Królow/a!"    has animate female proper;
```

- `rdzeń/końcówka` – po ukośniku stoi końcówka mianownika; może być pusta (`kamyk/`),
- `/x/` – litery występujące tylko w mianowniku (kłębek – kłębka, pies – psa),
- `/x=y/` – wymiana w rdzeniu (stół – stołu),
- `+u` – dopełniacz męski na -u zamiast -a,
- `!` – słowo odmienia się jak przymiotnik (`lamp/a sodow/a!` – lampy sodowej, `Czarn/a! Królow/a!` – Czarnej Królowej),
- rzeczownikiem jest ostatnie słowo z `/` bez `!`; słowa z `/` przed nim są przymiotnikami, a słowa bez `/` się nie odmieniają.

W liczbie mnogiej (`pluralname`) zapisuje się mianownik liczby mnogiej, a wskazówka po `+` dotyczy dopełniacza:

```inform
Object "mosiężn/e klucz/e"        has pluralname;                ! mosiężnych kluczy, kluczami
Object "star/e książk/i+e"        has pluralname female;         ! starych książek
Object "brudn/e okn/a+ie"         has pluralname neuter;         ! brudnych okien
Object "dębow/e drzwi/"           has pluralname;                ! dębowych drzwi, drzwiami
Object "sta/rz=r/y ryba/c=k//y"   has pluralname animate male;   ! widzisz starych rybaków
```

- Domyślny dopełniacz liczby mnogiej: w rodzaju żeńskim i nijakim bez końcówki (lamp, pudełek), w męskim po spółgłosce miękkiej -y/-i (kluczy), po twardej -ów (kamyków).
- `+e` i `+ie` wstawiają samogłoskę przed ostatnią spółgłoską (książek, okien).
- `+y` i `+ów` wymuszają końcówkę -y/-i albo -ów, a `+0` – dopełniacz bez końcówki, czyli sam rdzeń. Przydaje się to tam, gdzie reguła domyślna zawodzi: `"huczn/e urodzin/y"` dałoby „hucznych urodzinów”, a `"huczn/e urodzin/y+0"` daje poprawne „hucznych urodzin”.
- `pluralname animate male` oznacza rodzaj męskoosobowy, w którym biernik ma postać dopełniacza („widzisz rybaków”). Zwierzęta („psy”) nie powinny mieć `male`, bo mówimy „widzisz psy”, a nie „widzisz psów”.

**Ręczna.** Formy podane we właściwościach `desc_dop` … `desc_mie` mają pierwszeństwo przed odmianą automatyczną, służą więc do wyjątków (`desc_cel "wielkiemu psu"`). Parser rozpoznaje również słowa z `desc_*`. Stare gry, które podają formy w `desc_*` i `name_dop` … `name_mie`, działają bez zmian. Przykład obiektu z synonimami w `name` i z procedurą `parse_name` wołającą `Refers` można znaleźć w `games/alice_pl.inf` (kotki).

Ograniczenia: nazwa krótka może mieć najwyżej 160 znaków. Słowa spoza słownika dostają wartość `'xxnieznane'` (stała `PL_NIEZNANE`); dlatego na nieznane słowo parser odpowiada „Nie widzisz niczego takiego” zamiast „Nie znam słowa”. Polecenie `odmień X` w `games/test.inf` wypisuje wszystkie formy nazwy.

W komunikatach używa się reguł wydruku `(mian)`, `(dop)`, `(cel)`, `(bie)`, `(nar)`, `(mie)` oraz ich wersji z wielką literą `(CMian)` … `(CMie)`. Polska wersja korzysta z haków biblioteki 6.12 (`LanguagePrintShortName`, `LanguageRefers`, `LanguageCommand`), dzięki czemu parsera nie trzeba łatać.

Ograniczenia ogólne:

- komunikaty biblioteki są napisane wyłącznie w 2. osobie i w czasie teraźniejszym,
- w Glulx nie da się wpisywać polskich liter, ponieważ biblioteka 6.12 odczytuje tam wiersz poleceń w kodowaniu Latin-1. Nazwy przedmiotów są rozpoznawane także bez ogonków, ale czasowniki z polskimi literami („weź”, „otwórz”) nie mają odpowiedników bez nich, więc wersje `.ulx` nadają się na razie głównie do sprawdzania, czy gra się kompiluje.

## Gramatyka

`polskig.h` obejmuje wszystkie akcje biblioteki 6.12, np. „weź lampę ze stołu”, „włóż kamyk do pudełka”, „daj kowalowi lampę”, „zapytaj kowala o miecz”, „otwórz drzwi kluczem”, „idź na północ”, „spójrz pod łóżko”.

- Linie gramatyki używają zwykłych tokenów (`noun`, `held`, `multi`…), ponieważ nazwy obiektów są rozpoznawane w każdym przypadku.
- Słowo „się” jest usuwane z polecenia przed analizą, więc „rozejrzyj się”, „napij się wody” czy „połóż się na łóżku” działają bez osobnych linii gramatyki. Z tego powodu w `Extend` nie należy używać `'się'`.
- Jednoliterowe przyimki zapisuje się z `//`: `'w//'`, `'z//'`, `'o//'`; bez tego kompilator potraktowałby je jako stałe znakowe.
- Tokeny z wersji 6/11/PL (`bierniknoun`, `dopelniacznoun`, …) nadal istnieją jako aliasy zwykłych tokenów.
- Gdy brakuje dopełnienia, parser pyta we właściwym przypadku: „Do czego chcesz włożyć mosiężną lampę?”, „Co chcesz dać kowalowi?”. Przypadek dobierają procedury `PrzypadekCzasownika` i `PrzypadekPoPrzyimku` w `polski.h`; gra może je zastąpić przez `Replace`.

## Gry

Wszystkie gry w `games/` to polskie tłumaczenia [przykładowych gier Inform 6 z IF Archive](https://www.ifarchive.org/indexes/if-archive/games/source/inform/), przygotowane od nowa dla tej biblioteki. Nazwy obiektów zapisano w notacji automatycznej odmiany. Każda gra ma skrypt przejścia `*.przejscie`.

| Plik | Gra | Oryginał | Przejście |
|---|---|---|---|
| `ruins_pl.inf` | „Ruiny” | „Ruins” z DM4 (Graham Nelson, odtworzone przez Rogera Firtha) | 30/30 punktów |
| `alice_pl.inf` | „Po drugiej stronie lustra” | „Through the Looking-Glass” (Gareth Rees) | zwycięstwo |
| `toyshop_pl.inf` | „Sklep z zabawkami” | „Toyshop” (Graham Nelson) | 6/6 punktów |
| `advent_pl.inf` | „Przygoda” | „Adventure” (Crowther i Woods, wersja Inform Grahama Nelsona) | 350/350 punktów |
| `balances_pl.inf` | „Równowaga” | „Balances” (Graham Nelson) | zwycięstwo, 50/51 punktów (ostatni punkt, za zaklęcie filfre, wymaga śmierci) |
| `museum_pl.inf` | „Muzeum Informu” | „Museum” (Graham Nelson) | gra nie ma zakończenia; skrypt jest wycieczką po eksponatach |
| `adventureland_pl.inf` | „Kraina Przygód” | „Adventureland” (Scott Adams, wersja Inform Grahama Nelsona) | 100/100 punktów |

„Ruiny” są przykładem, na którym opiera się [podręcznik](docs/podrecznik.md), a [suplement](docs/suplement.md) omawia mechanizmy z pozostałych gier.

## Testy

```sh
./test.sh
```

Skrypt kompiluje wszystkie gry dla maszyny Z i dla Glulx – każde ostrzeżenie kompilatora jest traktowane jak błąd – a potem przechodzi każdą grę jej skryptem i sprawdza wynik końcowy z tabeli powyżej. Te same testy uruchamia GitHub Actions przy każdej zmianie w repozytorium; skompilowane gry można pobrać z zakładki *Actions* jako artefakt `gry`.

Pojedynczą grę można przejść ręcznie:

```sh
./build.sh ruins_pl
narzedzia/bin/dfrotz -m -Z 0 -s 1 build/ruins_pl.z5 < games/ruins_pl.przejscie
```

Opcja `-s 1` ustala ziarno losowania, żeby przebieg był powtarzalny. W „Przygodzie” krasnoludy i pirat poruszają się losowo, dlatego `test.sh` przechodzi ją w wersji testowej: stała `TEST_VERSION` udostępnia polecenie `xdeterm`, które skrypt wydaje na początku i które wyłącza krasnoludy i pirata.

### Gry z wersji 6/11/PL

Dawne tłumaczenia z wersji 6/11/PL (`alice.inf`, `ruins.inf`, `Advent.inf`) znajdują się w katalogu `Games` repozytorium [inform-pl](https://github.com/zajaczkowski/inform-pl). Biblioteka nadal je obsługuje: rozumie ich właściwości `name_*` i `desc_*`, tokeny `bierniknoun` i pokrewne, a także nazwy zapisane bez polskich liter (np. `'krolowa'`), bo słowa spoza słownika porównuje z pominięciem ogonków. Aby je skompilować, trzeba przekodować pliki na UTF-8, dołączyć `PolskiZ` przed `Constant Story` i zastąpić `Grammar` przez `PolskiG`.

## Licencja

Biblioteka jest udostępniona na tych samych warunkach co biblioteka standardowa Inform 6: do wyboru tradycyjna licencja Inform (DM4) albo Artistic License 2.0 (plik [LICENSE](LICENSE)). Gry przykładowe są tłumaczeniami i pozostają na warunkach oryginałów. Szczegóły: [COPYING](COPYING).
