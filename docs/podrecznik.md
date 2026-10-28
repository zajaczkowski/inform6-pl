# Pisanie gier po polsku w Inform 6

### Podręcznik biblioteki inform6-pl na przykładzie gry „Ruiny”

Ten podręcznik uczy pisania gier tekstowych po polsku w języku Inform 6 z biblioteką **inform6-pl**. Omawia krok po kroku kod gry `games/ruins_pl.inf`, polskiej wersji „Ruins” – niewielkiej przygody archeologicznej w ruinach majańskiej świątyni. Wszystkie fragmenty kodu pochodzą z tego pliku; dłuższe są skrócone, co zaznacza wielokropek. Warto mieć plik otwarty podczas lektury i zaglądać do pełnej wersji.

Podręcznik nie zastępuje opisu samego języka Inform 6 – jego składni, wyrażeń i procedur. Te znajdziesz w *The Inform Designer's Manual* Grahama Nelsona (DM4). Tutaj chodzi o budowę gry i o wszystko, co w polskiej wersji działa inaczej niż w angielskiej: o odmianę nazw przez przypadki, polską gramatykę poleceń i teksty, które muszą się zgadzać z rodzajem i liczbą.

---

## Spis treści

1. [Przygotowanie: kompilacja i uruchamianie](#1-przygotowanie-kompilacja-i-uruchamianie)
2. [Szkielet gry](#2-szkielet-gry)
3. [Pierwsze miejsce: Wielki Plac](#3-pierwsze-miejsce-wielki-plac)
4. [Pierwszy przedmiot: grzyb](#4-pierwszy-przedmiot-grzyb)
5. [Odmiana nazw przez przypadki](#5-odmiana-nazw-przez-przypadki)
6. [Teksty, które się zgadzają: przypadki, rodzaj i liczba](#6-teksty-które-się-zgadzają-przypadki-rodzaj-i-liczba)
7. [Pojemniki, podstawki i dekoracje](#7-pojemniki-podstawki-i-dekoracje)
8. [Klasa skarbów i punktacja](#8-klasa-skarbów-i-punktacja)
9. [Drzwi i klucze](#9-drzwi-i-klucze)
10. [Światło, ciemność i demony](#10-światło-ciemność-i-demony)
11. [Reguły w miejscach i reakcje przedmiotów](#11-reguły-w-miejscach-i-reakcje-przedmiotów)
12. [Pchanie przedmiotów i zdarzenia co turę](#12-pchanie-przedmiotów-i-zdarzenia-co-turę)
13. [Postać: zmumifikowany kapłan](#13-postać-zmumifikowany-kapłan)
14. [Zmiana gracza: guziec](#14-zmiana-gracza-guziec)
15. [Księga do sprawdzania: słownik glifów](#15-księga-do-sprawdzania-słownik-glifów)
16. [Własne czasowniki](#16-własne-czasowniki)
17. [Początek i koniec gry](#17-początek-i-koniec-gry)
18. [Testowanie](#18-testowanie)
19. [Ściągawki](#19-ściągawki)

---

## 1. Przygotowanie: kompilacja i uruchamianie

### Co jest w projekcie

```
inform6-pl/
  lib/polskiz.h     polskie litery: alfabet maszyny Z (dołączany jako pierwszy)
  lib/polski.h      plik języka: słownictwo, komunikaty, przypadki, pomocnicze procedury
  lib/odmiana.h     automatyczna odmiana nazw zapisanych z "/"
  lib/polskig.h     polska gramatyka poleceń
  games/            gry przykładowe (*_pl.inf) i ich skrypty przejść (*.przejscie)
  przygotuj.sh      pobiera i buduje narzędzia
  build.sh          kompiluje jedną grę
  test.sh           kompiluje wszystkie gry i sprawdza je skryptami przejść
  narzedzia/        tworzony przez przygotuj.sh:
    bin/inform6       kompilator Inform 6.44
    bin/dfrotz        interpreter tekstowy do testów
    inform6lib/       angielska biblioteka bazowa 6.12.8
```

Biblioteka bazowa (parser, verblib) pozostaje angielska i nie jest w żaden sposób zmieniana. Polską część stanowią plik języka i gramatyka, które zajmują miejsce angielskich plików `english.h` i `grammar.h`. Dzięki temu przejście na nowsze wydanie biblioteki bazowej nie wymaga przenoszenia żadnych poprawek.

### Kompilacja

Za pierwszym razem trzeba pobrać i zbudować narzędzia (wystarczą `git`, kompilator C i `make`):

```bash
./przygotuj.sh
```

Potem, z katalogu `inform6-pl`:

```bash
./build.sh ruins_pl
```

Skrypt kompiluje `games/ruins_pl.inf` do `build/ruins_pl.z5` (maszyna Z, wersja 5). Wersja dla Glulx:

```bash
./build.sh ruins_pl -G
```

Glulx oferuje znacznie więcej pamięci, ma jednak poważne ograniczenie: **polskie litery można wpisywać tylko w maszynie Z**. W Glulx biblioteka 6.12 odczytuje wiersz poleceń w kodowaniu Latin-1, w którym polskich liter nie ma. Nazwy przedmiotów zostaną wtedy rozpoznane także bez ogonków, ale czasowniki takie jak „weź” czy „otwórz” – już nie. Grę najlepiej więc pisać i testować w maszynie Z, a kompilację dla Glulx traktować jako sprawdzian przenośności kodu.

### Uruchamianie

Do grania wystarczy dowolny interpreter maszyny Z, na przykład frotz:

```bash
frotz build/ruins_pl.z5
```

Do testów – `dfrotz`, który czyta polecenia ze standardowego wejścia:

```bash
narzedzia/bin/dfrotz -m -Z 0 -s 1 build/ruins_pl.z5 < games/ruins_pl.przejscie
```

Opcja `-s 1` ustala ziarno generatora liczb losowych, dzięki czemu ten sam skrypt za każdym razem daje ten sam przebieg gry. Opcja `-m` wyłącza pauzy „[MORE]”, a `-Z 0` – zgłaszanie drobnych błędów gry przez interpreter. Testowaniu poświęcony jest rozdział 18.

---

## 2. Szkielet gry

Najkrótsza poprawna gra po polsku wygląda tak:

```inform
!% -Cu
!% +language_name=polski

Include "PolskiZ";

Constant Story "MOJA GRA";
Constant Headline "^Opowieść interaktywna^";

Include "Parser";
Include "VerbLib";

Object  Pokoj "Pokój"
  with  description "Mały, pusty pokój.",
  has   light;

[ Initialise; location = Pokoj; ];

Include "PolskiG";
```

Każdy z tych wierszy jest potrzebny:

- **`!% -Cu`** – informuje kompilator, że plik źródłowy jest zapisany w UTF-8. Bez tej opcji polskie litery w tekstach i słowach słownika zostałyby odczytane błędnie.
- **`!% +language_name=polski`** – sprawia, że biblioteka dołączy `polski.h` zamiast `english.h`.
- **`Include "PolskiZ";`** musi być **pierwszym** dołączeniem, przed jakimkolwiek tekstem. Plik przestawia alfabet maszyny Z tak, żeby polskie litery zajmowały w słowniku jak najmniej miejsca. Zmienia przy tym sposób kodowania wszystkich napisów, więc tekst skompilowany przed nim byłby zniekształcony. Jeśli o nim zapomnisz, kompilator przerwie pracę z komunikatem o brakującym `PolskiZ`.
- **`Parser`** i **`VerbLib`** – standardowe części biblioteki, jak w każdej grze Inform 6.
- **`Include "PolskiG";`** na końcu – polska gramatyka. Dołącza się ją po obiektach gry, a własne czasowniki (rozdział 16) dopisuje się dopiero po niej, bo często rozszerzają czasowniki w niej zdefiniowane.

Ruiny zaczynają się dokładnie tak; dochodzą tylko dwie stałe dla biblioteki:

```inform
Include "PolskiZ";

Constant Story "RUINY";
Constant Headline "^Interaktywny przykład z podręcznika^
                    Copyright (c) 1999 by Angela M. Horns.^
                    Wersja polska.^";

Constant MAX_CARRIED = 7;
Constant MAX_SCORE = 30;

Include "Parser";
Include "VerbLib";
```

`MAX_CARRIED` ogranicza liczbę przedmiotów, które gracz może nieść naraz. `MAX_SCORE` to najwyższy możliwy wynik; biblioteka potrzebuje go do komunikatu „Do tej pory zdobyłeś 15 punktów na 30 możliwych…”.

---

## 3. Pierwsze miejsce: Wielki Plac

Miejsca (pomieszczenia) są w Inform zwykłymi obiektami, które nie mają rodzica. Gra zaczyna się na Wielkim Placu:

```inform
Object  Forest "~Wielki Plac~"
  with  description
            "Tak przynajmniej nazywasz w notatkach tę niską wapienną skarpę,
             choć las deszczowy dawno już ją sobie odebrał. ...",
        d_to steps,
        in_to steps,
        u_to
            "Drzewa są kolczaste. Próba wspinaczki skończyłaby się dłońmi
             pociętymi do krwi.",
        cant_go
            "Las deszczowy jest gęsty, a nie po to przedzierasz się przez
             niego od wielu dni, żeby teraz porzucić swoje odkrycie. ...",
        before [; Listen: "Wyjce, nietoperze, papugi, ary."; ],
  has   light;
```

- **Nazwa miejsca** (`"~Wielki Plac~"`) pojawia się w nagłówku opisu i w linii statusu. Znak `~` oznacza w napisach Inform cudzysłów. Nazw miejsc nie trzeba zapisywać w notacji odmiany, bo biblioteka wypisuje je zawsze w mianowniku.
- **`description`** to opis wyświetlany po wejściu do miejsca i po poleceniu „rozejrzyj się”.
- **Wyjścia** to właściwości `n_to`, `s_to`, `e_to`, `w_to`, `ne_to`, `nw_to`, `se_to`, `sw_to`, `u_to`, `d_to`, `in_to` i `out_to`. Wartością może być inne miejsce, drzwi (tutaj schody) albo tekst. W tym ostatnim przypadku gracz zostaje na miejscu i czyta podany tekst – tak jest przy próbie wspięcia się na drzewa (`u_to`).
- **`cant_go`** to odpowiedź na próbę pójścia w kierunku, w którym nie ma wyjścia. Bez niej biblioteka odpowie ogólnym „Nie możesz pójść w tę stronę.”
- **`before`** przechwytuje akcje, zanim biblioteka je wykona. Tutaj każda próba słuchania („słuchaj”, „posłuchaj”) daje opis odgłosów lasu. Sam napis zakończony średnikiem jest w Inform skrótem: drukuje tekst, przechodzi do nowego wiersza i kończy regułę wartością „prawda”. Prawda oznacza „akcja obsłużona”, więc biblioteka nie dodaje już własnej odpowiedzi.
- **`has light`** – miejsce jest oświetlone. Bez tego atrybutu gracz byłby w ciemności (rozdział 10).

### Jak gracz wpisuje kierunki

Polska gramatyka rozumie zarówno pełne nazwy kierunków, jak i skróty:

| Kierunek | Skrót | Przykłady poleceń |
|---|---|---|
| północ | `pn` | `idź na północ`, `pn` |
| południe | `pd` | `pd` |
| wschód | `w` | `idź na wschód`, `w` |
| zachód | `z` | `z` |
| północny wschód / zachód | `pnw` / `pnz` | `pnw` |
| południowy wschód / zachód | `pdw` / `pdz` | `pdz` |
| góra / dół | `g` / `d` | `idź w górę`, `wejdź do góry`, `zejdź`, `d` |

Uwaga: `w` oznacza **wschód**, a nie zachód, a samo `n` znaczy „nie” (odpowiedź na pytanie gry). Na Wielkim Placu gracz zejdzie do ruin poleceniem `zejdź`, `d`, `idź w dół` albo `zejdź po schodach`.

---

## 4. Pierwszy przedmiot: grzyb

Przedmiot, który na początku gry leży w danym miejscu, zapisuje się bezpośrednio po tym miejscu i poprzedza strzałką `->`:

```inform
Object  -> mushroom "cętkowan/y grzyb/"
  with  name 'grzybek' 'grzybka' 'grzybkowi' 'grzybkiem' 'grzybku'
             'muchomor' 'muchomora' 'muchomorowi' 'muchomorem' 'muchomorze',
        initial
            "Z rozmokłej ziemi wyrasta na długiej nóżce cętkowany grzyb.",
        description
            "Kapelusz grzyba pokrywają plamy i wcale nie masz pewności,
             czy to nie muchomor.",
        after [;
            Take:
                if (self.mushroom_picked)
                    "Podnosisz powoli rozpadający się grzyb.";
                self.mushroom_picked = true;
                "Zrywasz grzyb, zgrabnie przecinając jego cienką nóżkę.";
            Drop:
                "Grzyb spada na ziemię, lekko poobijany.";
            Eat:
                steps.rubble_filled = false;
                "Nadgryzasz brzeg kapelusza ...";
            ],
        mushroom_picked false,
  has   edible;
```

### Nazwa z ukośnikami

`"cętkowan/y grzyb/"` to **nazwa krótka** zapisana w notacji odmiany: ukośniki oddzielają rdzeń od końcówki mianownika. Na tej podstawie biblioteka sama tworzy formy wszystkich przypadków: *cętkowanego grzyba*, *cętkowanemu grzybowi*, *cętkowany grzyb*, *cętkowanym grzybem*, *cętkowanym grzybie*. Rozpoznaje też każdą z tych form, gdy gracz ją wpisze, nawet bez polskich liter („zjedz cetkowany grzyb”). Szczegóły notacji opisuje rozdział 5.

### `name` – tylko synonimy

W angielskiej bibliotece `name` to lista wszystkich słów, którymi gracz może nazwać przedmiot. W inform6-pl słowa z nazwy krótkiej są rozpoznawane automatycznie, więc `name` służy już tylko **synonimom**, czyli słowom, których w nazwie nie ma. Synonimy nie są odmieniane, dlatego trzeba podać wszystkie formy, jakich gracz może użyć – tutaj „grzybek” i „muchomor”.

### Opisy i reguły

- **`initial`** – zdanie opisujące przedmiot w opisie miejsca, dopóki gracz go nie ruszy. Gdy przedmiot zostanie przeniesiony, biblioteka wymienia go już tylko na liście „Widzisz tutaj …”.
- **`description`** – tekst po „zbadaj grzyb”.
- **`after`** – reguły wykonywane po akcji, zamiast zwykłego komunikatu biblioteki. `Take` (weź), `Drop` (upuść) i `Eat` (zjedz) to nazwy akcji biblioteki, wspólne dla wszystkich języków; polska gramatyka przypisuje im polskie czasowniki.
- **`mushroom_picked`** to właściwość zdefiniowana przez autora gry. Zapamiętuje, czy grzyb został już zerwany, żeby przy kolejnym podniesieniu wypisać inny tekst.
- **`has edible`** – grzyb można zjeść.

Zjedzenie grzyba usuwa gruz ze schodów (`steps.rubble_filled = false`); na tym polega pierwsza zagadka gry.

---

## 5. Odmiana nazw przez przypadki

To najważniejszy rozdział dla autora polskiej gry. Polszczyzna ma siedem przypadków; gra potrzebuje sześciu, bo wołacza nie używa się w opisach (gracz używa go tylko, zwracając się do postaci – zob. rozdział 13). Biblioteka korzysta z przypadków w komunikatach: „Bierzesz **cętkowany grzyb**”, „Nie masz **mosiężnej lampy**”, „Wkładasz **pigmejską figurkę** do **skrzyni transportowej**”, „Na **kamiennym ołtarzu** jest …”.

### Zapis jednego słowa

```
rdzeń / końcówka mianownika
```

| Zapis | Mianownik | Dopełniacz | Miejscownik |
|---|---|---|---|
| `kamyk/` | kamyk | kamyka | kamyku |
| `lamp/a` | lampa | lampy | lampie |
| `pudełk/o` | pudełko | pudełka | pudełku |
| `cętkowan/y` | cętkowany | cętkowanego | cętkowanym |

Pusta końcówka (`kamyk/`) też jest końcówką: tak zapisuje się rzeczownik męski zakończony spółgłoską. Ukośnik jest wtedy potrzebny, bo bez niego słowo w ogóle by się nie odmieniało – tak jak „z jadeitu” w `"mozaikow/a mask/a z jadeitu"`, które ma pozostać w każdym przypadku bez zmian.

Końcówki przypadków biblioteka dobiera sama, na podstawie rodzaju obiektu, końcówki mianownika i ostatnich liter rdzenia. Zna też regularne wymiany spółgłosek: *lampa – lampie*, *półka – półce*, *noga – nodze*, *stół – stole*, *gazeta – gazecie*, *krzesło – krześle*.

### Które słowo jest rzeczownikiem

Biblioteka przyjmuje, że ostatnie słowo z ukośnikiem jest rzeczownikiem, a słowa z ukośnikiem przed nim są przymiotnikami, które odmieniają się zgodnie z rodzajem obiektu. Ta reguła zawodzi w dwóch sytuacjach: gdy przymiotnik stoi po rzeczowniku („lampa sodowa”, „skrzynia transportowa”) i gdy rzeczownik odmienia się jak przymiotnik („królowa – królowej”). Takie słowo oznacza się wykrzyknikiem, który każe odmieniać je jak przymiotnik:

```inform
Object  sodium_lamp "lamp/a sodow/a!"          ! lampy sodowej, lampą sodową
Object  -> packing_case "skrzyni/a transportow/a!"
```

Rzeczownikiem jest wtedy ostatnie słowo z ukośnikiem **bez** `!`.

### Wymiany w rdzeniu

Wiele polskich słów ma w mianowniku inny rdzeń niż w pozostałych przypadkach. Notacja przewiduje na to dwa zapisy:

| Zapis | Znaczenie | Przykład z Ruin |
|---|---|---|
| `/x/` | litery tylko w mianowniku | `"starożytn/y plast/e/r/ miodu"` – plaster, plastra |
| `/x=y/` | `x` w mianowniku, `y` w pozostałych przypadkach | `"przerażając/a przepa/ść=ści/"` – przepaść, przepaści |

Oba zapisy można łączyć w jednym słowie. W nazwie guźca, w którego zmienia się gracz, „ie” znika poza mianownikiem, a „z” przechodzi w „ź”:

```inform
Object  warthog "Gu/zie=ź/c/"        ! Guziec, Guźca, Guźcowi, Guźcem, Guźcu
```

### Wskazówki po `+`

Końcówki dopełniacza są w polszczyźnie najmniej przewidywalne, dlatego na końcu słowa można dopisać wskazówkę:

| Wskazówka | Działanie | Przykład |
|---|---|---|
| `+u` | dopełniacz męski na -u zamiast -a | `"aparat/+u na mokre płyty"` – aparatu |
| `+e`, `+ie` | w liczbie mnogiej wstaw samogłoskę przed ostatnią spółgłoską | `"malowidł/a+e"` – malowideł |
| `+y` | dopełniacz mnogi na -y/-i | `"wyryt/e inskrypcj/e+y"` – wyrytych inskrypcji |
| `+ów` | dopełniacz mnogi na -ów | |
| `+0` | dopełniacz mnogi bez końcówki (sam rdzeń) | `"wesoł/e imienin/y+0"` – wesołych imienin (bez `+0`: „imieninów”) |

### Rodzaj i liczba

Rodzaj gramatyczny obiektu ustalają atrybuty:

| Atrybuty | Rodzaj | Przykład |
|---|---|---|
| *(brak)* | męski | `kamienn/y ołtarz/` |
| `female` | żeński | `pigmejsk/a figurk/a` |
| `neuter` | nijaki | `wysok/ie krzesełk/o` |
| `pluralname` | mnogi niemęskoosobowy | `kamienn/e schod/y` |
| `pluralname animate male` | męskoosobowy | „starzy rybacy” |
| `animate` | żywy (biernik męski = dopełniacz) | `zmumifikowan/y kapłan/` – widzisz zmumifikowanego kapłana |

Nazwę obiektu w liczbie mnogiej zapisuje się w mianowniku liczby mnogiej:

```inform
Object  -> steps "kamienn/e schod/y"        ! kamiennych schodów, kamiennymi schodami
  ...
  has   scenery door open pluralname;
```

Rodzaj trzeba podać **zawsze**, gdy nie jest męski, bo biblioteka nie zgaduje go z końcówki. Bez `female` „figurka” odmieniałaby się jak rzeczownik męski, a przymiotniki w komunikatach miałyby męskie końcówki.

### Gdy automat się myli: `desc_*`

Polszczyzna ma mnóstwo wyjątków: *pies – psu*, *dom – domu*, *miasto – w mieście*, *ludzie – ludźmi*. Żadne reguły nie obejmą ich wszystkich, dlatego formę dowolnego przypadku można podać ręcznie we właściwości `desc_dop`, `desc_cel`, `desc_bie`, `desc_nar` albo `desc_mie`. Forma ręczna ma pierwszeństwo przed automatyczną, a parser rozpoznaje ją tak samo:

```inform
Object  -> pies "wielk/i p/ie/s/"
  with  desc_cel "wielkiemu psu",        ! automat dałby "wielkiemu psowi"
  has   animate male;
```

W Ruinach żaden obiekt tego nie potrzebuje. W „Przygodzie” przykładem są liście (`desc_nar "liśćmi"`) i gniazdo (`desc_mie "gnieździe ze złotymi jajami"`).

### Jak sprawdzić odmianę

Najprościej dodać na czas pisania polecenie testowe, które wypisuje formy wszystkich przypadków (gotowe jest w `games/test.inf`):

```inform
[ OdmienSub;
    print (mian) noun, " / ", (dop) noun, " / ", (cel) noun, " / ",
          (bie) noun, " / ", (nar) noun, " / ", (mie) noun, "^";
];
Verb 'odmień' * noun -> Odmien;
```

```
>odmień przepaść
przerażająca przepaść / przerażającej przepaści / przerażającej przepaści /
przerażającą przepaść / przerażającą przepaścią / przerażającej przepaści
```

---

## 6. Teksty, które się zgadzają: przypadki, rodzaj i liczba

### Nazwy w przypadkach

W angielskim kodzie pisze się `(the) noun` albo `(The) noun`. Po polsku trzeba za każdym razem wybrać przypadek, jakiego wymaga zdanie. Służą do tego reguły wydruku:

| Reguła | Przypadek | Pytanie | Wielka litera |
|---|---|---|---|
| `(mian)` | mianownik | kto? co? | `(CMian)` |
| `(dop)` | dopełniacz | kogo? czego? | `(CDop)` |
| `(cel)` | celownik | komu? czemu? | `(CCel)` |
| `(bie)` | biernik | kogo? co? | `(CBie)` |
| `(nar)` | narzędnik | z kim? z czym? | `(CNar)` |
| `(mie)` | miejscownik | o kim? o czym? | `(CMie)` |

Ostatni skarb pakowany do skrzyni kończy grę zdaniem w bierniku:

```inform
print_ret "Gdy ostrożnie pakujesz ", (bie) noun,
          ", z wierzchołków drzew sfruwa ara ...";
```

Przy fotografowaniu naświetla się „zdjęcie czego?”, więc potrzebny jest dopełniacz:

```inform
print_ret "Rozstawiasz ... i cierpliwie naświetlasz zdjęcie ",
           (dop) noun, ".";
```

Wersji z wielką literą (`CMian` i pozostałych) używa się na początku zdania. Gdy obiektem jest sam gracz, reguły wypisują zaimki: *ty, siebie, sobie, sobą*.

### Czasownik zgodny z liczbą: `PL`

Jeśli podmiotem zdania jest obiekt, który może występować w liczbie pojedynczej albo mnogiej, czasownik trzeba do niego dopasować. Służy do tego `PL(obiekt, "pojedyncza", "mnoga")`:

```inform
Receive:                                    ! przepaść: coś wrzucono do środka
    remove noun;
    print (CMian) noun, " "; PL(noun, "spada", "spadają");
    " bezgłośnie w ciemność przepaści.";
```

Kamienny klucz *spada*, kamienne drzwi *spadają*. Tak samo działa nora w Robaczych Chodnikach: upuszczony przedmiot do niej *wpada*, a przedmioty w liczbie mnogiej *wpadają*.

### Końcówka zgodna z rodzajem: `KoncY` i `Forma`

`KoncY(obiekt)` drukuje końcówkę przymiotnika lub imiesłowu w mianowniku: -y, -a albo -e. Tak gra opisuje skarb zapakowany do skrzyni:

```inform
print "Bezpiecznie zapakowan"; KoncY(noun); ".";
```

Wynikiem jest *zapakowany*, *zapakowana* albo *zapakowane*. Ogólniejsza procedura `Forma(obiekt, męski, żeński, nijaki, mnogi, męskoosobowy)` drukuje ten z podanych tekstów, który odpowiada rodzajowi i liczbie obiektu; przydaje się, gdy końcówki są inne niż -y/-a/-e.

### Rodzaj gracza: `Gracz`

Formy czasu przeszłego i niektóre przymiotniki zależą od płci postaci gracza. `Gracz("forma męska", "forma żeńska")` sprawdza, czy gracz ma atrybut `female`, i drukuje odpowiednią formę:

```inform
[ DeathMessage;
    if (deadflag == 3) Gracz("Zostałeś pojmany", "Zostałaś pojmana");
];
```

Tak samo kapłan mówi „Odpowiedź musisz znaleźć sam” albo „sama”. Najwygodniej jednak w ogóle unikać takich konstrukcji i pisać w czasie teraźniejszym („Spadasz w pustkę”) albo bezosobowo („Próba wspinaczki skończyłaby się …”). Tekst jest wtedy krótszy, a gra poprawna niezależnie od płci bohatera. Ruiny robią tak niemal wszędzie.

### Pozostałe pomocnicze procedury

| Procedura | Drukuje | Użycie |
|---|---|---|
| `JestSa(o)` | jest / są / jesteś | „Lampa jest wyłączona” |
| `GoJaJe(o)` | go / ją / je / ich | „weź go” |
| `JegoJejIch(o)` | go / jej / ich | „nie masz jej” |
| `MuJejIm(o)` | mu / jej / im | |
| `NimNiejNich(o)` | nim / niej / nich | „na niej” |
| `CTenTaTo(o)` | Ten / Ta / To / Te / Ci | początek zdania |
| `Kto(aktor, "Bierzesz", "bierze")` | „Bierzesz” albo „Kowal bierze” | komunikaty wspólne dla gracza i postaci |
| `Liczebnik(n, "punkt", "punkty", "punktów")` | forma zgodna z liczbą | „3 punkty”, „5 punktów” |

---

## 7. Pojemniki, podstawki i dekoracje

### Skrzynia: pojemnik

```inform
Object  -> packing_case "skrzyni/a transportow/a!"
  with  name 'skrzynka' 'skrzynki' ... 'kufer' ... 'pudło' ...,
        initial
            "Stoi tu twoja skrzynia transportowa, gotowa pomieścić każde
             cenne znalezisko ...",
        before [;
            Take, Remove, PushDir:
                "Skrzynia jest zbyt ciężka, żeby się z nią szarpać, dopóki
                 wyprawa nie jest zakończona.";
            ],
  has   static container open openable female;
```

- **`container`** – skrzynia jest pojemnikiem, można więc do niej wkładać przedmioty („włóż figurkę do skrzyni”).
- **`open openable`** – jest otwarta, ale można ją zamknąć.
- **`static`** – nie da się jej podnieść. Próbę podniesienia przechwytuje wprawdzie reguła `before` z ciekawszą odpowiedzią, ale atrybut i tak jest potrzebny: dzięki niemu biblioteka pomija skrzynię przy poleceniu „weź wszystko”.

Na początku gry w skrzyni leżą aparat i gazeta. Zapisuje się je z dwiema strzałkami `-> ->`, bo są o jeden poziom głębiej: w skrzyni, która jest w miejscu.

### Ołtarz: podstawka

```inform
Object  -> stone_table "kamienn/y ołtarz/"
  with  name 'stół' 'stołu' 'stołowi' 'stołem' 'stole'
             'płyta' 'płyty' 'płycie' 'płytę' 'płytą' ...,
        initial
            "Sanktuarium dominuje wielka kamienna płyta - stół albo ołtarz.",
  has   enterable supporter static;
```

**`supporter`** oznacza podstawkę: rzeczy kładzie się *na* niej („połóż lampę na ołtarzu”). **`enterable`** pozwala na nią wejść („wejdź na ołtarz”, a potem „zejdź”). Biblioteka sama buduje zdanie „Na kamiennym ołtarzu jest mozaikowa maska z jadeitu”, z nazwą podstawki w miejscowniku („na czym?”).

### Dekoracje

**`scenery`** oznacza przedmiot, który jest częścią opisu miejsca: nie pojawia się na liście „Widzisz tutaj…” i nie da się go wziąć. Snop światła w Kwadratowej Komnacie jest dekoracją, która na wszystko poza oglądaniem odpowiada tym samym zdaniem:

```inform
Object  -> sunlight "snop/ światła"
  with  name 'słońce' 'słońca' ... 'promień' ... 'kurz' 'kurzu' 'powietrze',
        description
            "Pyłki kurzu migoczą w rozświetlonym powietrzu ...",
        before [;
            Examine, Search:
                ;
            default:
                "To tylko nieuchwytny snop słonecznego światła.";
            ],
  has   scenery;
```

Pusta reguła `Examine, Search: ;` zwraca fałsz, więc oglądanie i przeszukiwanie obsługuje biblioteka (wypisuje `description`). `default:` łapie wszystkie pozostałe akcje.

Dekoracja widoczna w kilku miejscach ma właściwość `found_in` z listą tych miejsc. Niska mgła unosi się zarówno na Placu, jak i w Komnacie:

```inform
        found_in Square_Chamber Forest,
```

---

## 8. Klasa skarbów i punktacja

Pięć przedmiotów w Ruinach to skarby: figurka, maska, stela, kość i plaster miodu. Wszystkie podlegają tym samym regułom, więc dostają wspólną **klasę**:

```inform
Class   Treasure
  with  before [;
            Take, Remove:
                if (self in packing_case)
                    "Rozpakowywanie tak bezcennego zabytku lepiej zostawić
                     ludziom z Instytutu Carnegie.";
                if (self.photographed_in_situ == false)
                    "Są lata trzydzieste, a nie dawne złe czasy. ...";
            Photograph:
                if (self has moved) "Co, i sfałszować dokumentację wykopalisk?";
                if (self.photographed_in_situ) "Nie po raz drugi.";
            ],
        after [;
            Insert:
                if (second == packing_case) {
                    score = score + self.cultural_value;
                    ...
                    print "Bezpiecznie zapakowan"; KoncY(noun); ".";
                }
            Photograph:
                self.photographed_in_situ = true;
            ],
        cultural_value 5,
        photographed_in_situ false;
```

Skarb tworzy się, pisząc nazwę klasy w miejscu słowa `Object`:

```inform
Treasure -> statuette "pigmejsk/a figurk/a"
  with  ...
        initial "Spoczywa tu cenna majańska figurka!",
  has   female;
```

Klasa nadaje każdemu skarbowi dwie reguły gry:

1. Skarbu nie można zabrać, dopóki nie zostanie sfotografowany na miejscu znalezienia.
2. Skarb włożony do skrzyni dodaje do wyniku (`score`) tyle punktów, ile wynosi jego `cultural_value`.

Maska jest warta więcej, więc nadpisuje wartość odziedziczoną z klasy: `cultural_value 10`. Gdy wynik osiągnie `MAX_SCORE`, gra ustawia `deadflag = 2`, co oznacza zwycięstwo.

---

## 9. Drzwi i klucze

### Schody jako drzwi

Z Wielkiego Placu w dół prowadzą kamienne schody. W Inform są one **drzwiami**, czyli obiektem łączącym dwa miejsca, który może przepuścić gracza albo go zatrzymać:

```inform
Object  -> steps "kamienn/e schod/y"
  with  ...
        door_to [;
            if (self.rubble_filled)
                "Już po kilku stopniach drogę tarasuje gruz.";
            return Square_Chamber;
            ],
        door_dir d_to,
        rubble_filled true,
  has   scenery door open pluralname;
```

- **`door_to`** – dokąd prowadzą. Wartością może być procedura: dopóki schody zasypuje gruz, wypisuje ona tekst i nie prowadzi nigdzie.
- **`door_dir`** – w którym kierunku prowadzą. Plac ma `d_to steps`, więc „zejdź” prowadzi przez schody.
- **`door open`** – drzwi, które są stale otwarte (nie mają atrybutu `openable`).

### Kamienne drzwi z kluczem

Drzwi między korytarzem a Sanktuarium są widoczne z obu stron i zamknięte na klucz:

```inform
Object  -> StoneDoor "kamienn/e drzwi/"
  with  ...
        when_closed
            "Przejście zagradzają masywne drzwi z żółtego kamienia.",
        when_open
            "Wielkie drzwi z żółtego kamienia stoją otworem.",
        door_to [; if (self in Corridor) return Shrine; return Corridor; ],
        door_dir [; if (self in Shrine) return n_to; return s_to; ],
        with_key stone_key,
        found_in Corridor Shrine,
  has   static door openable lockable locked pluralname;
```

`found_in` umieszcza je w obu miejscach naraz. `door_to` i `door_dir` są procedurami, bo odpowiedź zależy od tego, po której stronie stoi gracz. `with_key` wskazuje klucz, który pasuje do zamka.

Gracz otwiera je tak:

```
>odblokuj drzwi kluczem
Otwierasz zamek kamiennych drzwi.
>otwórz drzwi
Otwierasz kamienne drzwi.
```

Działają też polecenia „otwórz drzwi kluczem” i „odblokuj drzwi za pomocą klucza”.

---

## 10. Światło, ciemność i demony

### Ciemne miejsca

Miejsce bez `has light` jest ciemne. Gracz widzi w nim cokolwiek tylko wtedy, gdy ma przy sobie albo obok siebie przedmiot z atrybutem `light`. W ciemności biblioteka zastępuje prawdziwe miejsce obiektem `thedark`; Ruiny ustawiają jego opis w `Initialise`:

```inform
thedark.description =
    "Ciemność wieków napiera na ciebie ze wszystkich stron i zaczynasz
     czuć klaustrofobię.";
```

### Lampa sodowa

Lampa świeci tylko wtedy, gdy jest włączona. Reguły `after` nadają jej atrybut `light` po włączeniu i odbierają po wyłączeniu:

```inform
after [;
    SwitchOn:  give self light;
    SwitchOff: give self ~light;
    ],
```

Gracz może napisać „włącz lampę”, „zapal lampę”, „wyłącz lampę” albo „zgaś lampę”. Dla przedmiotów z atrybutem `switchable` „zapal” znaczy „włącz”, a dla pozostałych – „podpal”.

Świecącej lampy nie da się podnieść, a przed włączeniem trzeba ją postawić. Dlatego w ciemne korytarze trzeba ją **pchać** (rozdział 12).

### Demon baterii

**Demon** to procedura obiektu, którą biblioteka wywołuje co turę, od włączenia aż do wyłączenia, niezależnie od tego, gdzie jest gracz. Lampa ma demona, który zużywa baterię:

```inform
daemon [;
    if (self hasnt on) return;
    if (--self.battery_power == 0) give self ~light ~on;
    if (self in location) {
        switch (self.battery_power) {
            10: "^Lampa sodowa przygasa!";
            5:  "^Lampa sodowa długo już nie wytrzyma.";
            0:  "^Lampa sodowa blednie i nagle gaśnie.";
        }
    }
    ],
battery_power 100,
```

Demona włącza wywołanie `StartDaemon(sodium_lamp)` w `Initialise`. Sam demon sprawdza, czy lampa jest włączona, więc może działać przez całą grę.

### Drapanie pazurków

Chrobot pazurków znajduje się w obiekcie `thedark`, więc gracz styka się z nim tylko w ciemności. Jego `each_turn` – procedura wywoływana co turę, gdy obiekt jest w zasięgu gracza – uruchamia demona. Demon liczy tury spędzone w ciemności, a w czwartej zabija gracza. Gdy gracz wyjdzie na światło, demon zeruje licznik i sam się wyłącza:

```inform
Object  tiny_claws "chrobot/+u drobnych pazurków" thedark
  with  ...
        each_turn [; StartDaemon(self); ],
        daemon [;
            if (location ~= thedark) {
                self.turns_active = 0;
                StopDaemon(self);
                rtrue;
            }
            switch (++(self.turns_active)) {
                1:  "^Chrobot zbliża się odrobinę, ...";
                ...
                4:  deadflag = true;
                    "^Nagle czujesz drobne ukłucie kła ...";
            }
            ],
        turns_active 0;
```

`deadflag = true` (czyli 1) oznacza śmierć gracza. Gra kończy się wtedy komunikatem „Zginąłeś” albo „Zginęłaś”.

---

## 11. Reguły w miejscach i reakcje przedmiotów

### Reguła `before` w miejscu

Reguła `before` miejsca obejmuje każdą akcję, którą gracz wykonuje w tym miejscu, niezależnie od przedmiotu. Kwadratowa Komnata reaguje w ten sposób na włożenie kokonu do snopa światła, co rozwiązuje zagadkę klucza:

```inform
Object  Square_Chamber "Kwadratowa Komnata"
  with  ...
        before [;
            Insert:
                if (noun == eggsac && second == sunlight) {
                    remove eggsac;
                    move stone_key to self;
                    "Upuszczasz kokon w jaskrawy snop słonecznego światła. ...";
                }
            ],
  has   light;
```

W akcji `Insert` zmienna `noun` wskazuje wkładany przedmiot, a `second` – pojemnik. Gracz może to wyrazić na różne sposoby: „wrzuć kokon w snop światła”, „włóż kokon do snopa”, „wsadź worek w smugę”. Polska gramatyka zamienia każde z tych zdań na tę samą akcję, więc reguła wystarczy jedna.

### `react_before`: reakcja przedmiotu na cudzą akcję

Reguła `before` przedmiotu działa tylko wtedy, gdy akcja dotyczy tego przedmiotu. **`react_before`** działa przy każdej akcji gracza, o ile przedmiot jest w pobliżu. Dzięki temu kokon może zareagować na wyjście gracza na słońce, choć polecenie „idź w górę” w ogóle go nie wymienia:

```inform
react_before [;
    Go:
        if (location == Square_Chamber && noun == u_obj) {
            deadflag = true;
            "W chwili gdy pada na niego naturalne światło, ...";
        }
    ];
```

Przy akcji `Go` zmienna `noun` wskazuje kierunek, czyli jeden z obiektów `u_obj` (góra), `d_obj`, `n_obj` i tak dalej.

---

## 12. Pchanie przedmiotów i zdarzenia co turę

### Pchanie w kierunku

Kulę z pumeksu można przetaczać między miejscami („pchnij kulę na południe”). Biblioteka domyślnie na to nie pozwala, dlatego reguła `before` dla akcji `PushDir` musi wywołać `AllowPushDir()` i zwrócić prawdę:

```inform
Object  -> huge_ball "ogromn/a kul/a z pumeksu"
  with  ...
        before [;
            PushDir:
                if (location == Junction && second == ne_obj)
                    "Wejście do sanktuarium jest dużo węższe niż osiem stóp.";
                AllowPushDir();
                rtrue;
            ...
            ],
        after [;
            PushDir:
                if (second == n_obj) "Z wysiłkiem pchasz kulę pod górę.";
                if (second == s_obj) "Raz rozpędzoną kulę trudno zatrzymać.";
                ...
            ],
  has   static female;
```

W akcji `PushDir` zmienna `second` wskazuje kierunek. W ten sam sposób gracz pcha lampę sodową w ciemne korytarze.

### `each_turn` przepaści

Przepaść co turę sprawdza, czy kula nie wtoczyła się do miejsca, w którym przepaść się znajduje. Jeśli tak, kula klinuje się w przepaści i tworzy przejście na Półkę z pumeksu:

```inform
each_turn [;
    if (huge_ball in parent(self)) {
        remove huge_ball;
        Canyon_S.s_to = On_Ball;
        Canyon_S.description =
            "Południowy koniec kanionu przechodzi teraz w kulę z pumeksu,
             zaklinowaną w przepaści.";
        "^Kula z pumeksu toczy się bezwładnie ...";
    }
    ],
```

Przykład pokazuje, że właściwości miejsc – wyjścia i opisy – można zmieniać w czasie gry zwykłym przypisaniem.

---

## 13. Postać: zmumifikowany kapłan

Postać to obiekt z atrybutem `animate`. Reakcje na rozmowę i inne działania gracza (pytanie, dawanie, atak) opisuje właściwość **`life`**, a reakcje na rozkazy – właściwość **`orders`**:

```inform
Object  priest "zmumifikowan/y kapłan/"
  with  ...
        life [;
            Answer:
                "Kapłan kaszle i omal nie rozpada się na kawałki.";
            Ask: switch (second) {
                'słownik', 'słownika', 'słowniku', 'książkę', 'książka', ...:
                    ...
                'ruiny', 'ruinach', 'ruinę':
                    "~Ruiny zawsze pokonają złodziei. ...~";
                'xibalba', 'xibalbę', 'xibalbie', 'xibalby':
                    ...
                    Shrine.sw_to = Junction;
                    "Kapłan wyciąga kościsty palec na południowy zachód ...";
                }
                print "~Odpowiedź musisz znaleźć ";
                Gracz("sam", "sama");
                ".~";
            Tell:
                "Kapłana nie interesuje twoje nędzne życie.";
            Attack, Kiss:
                remove self;
                "Kapłan wysycha i rozsypuje się w pył ...";
            Show, Give:
                ...
            ],
        orders [;
            Go:            "~Nie wolno mi opuszczać sanktuarium.~";
            NotUnderstood: "~Mówisz zagadkami.~";
            default:       "~Nie twoim rozkazom służę.~";
            ],
  has   animate male;
```

### Pytanie o temat: wszystkie formy słowa

„Zapytaj kapłana o ruiny” daje akcję `Ask`, a pierwsze słowo tematu trafia do `second`. Po polsku temat może wystąpić w różnych przypadkach („o ruiny”, „o ruinach”, „o Xibalbę”), więc w `switch` trzeba wymienić **każdą formę**, jakiej gracz może użyć. Temat jest zwykłym słowem słownika, a nie nazwą obiektu, dlatego nie działa tu ani automatyczna odmiana, ani rozpoznawanie słów bez polskich liter. Jeśli gracz może napisać „o xibalbe”, trzeba dopisać także `'xibalbe'`.

### Akcje rozmowy po polsku

| Polecenie | Akcja | `noun` | `second` |
|---|---|---|---|
| `zapytaj kapłana o ruiny` | `Ask` | kapłan | słowo tematu |
| `powiedz kapłanowi o lampie` | `Tell` | kapłan | słowo tematu |
| `odpowiedz kapłanowi tak`, `powiedz tak do kapłana` | `Answer` | słowo tematu | kapłan |
| `pokaż słownik kapłanowi` | `Show` | słownik | kapłan |
| `daj gazetę kapłanowi` | `Give` | gazeta | kapłan |
| `kapłanie, idź na północ` | rozkaz: `Go` w `orders` | | |

W rozkazach gracz zwraca się do postaci w wołaczu: „kapłanie”. Biblioteka nie tworzy wołacza automatycznie, więc jeśli różni się on od mianownika, trzeba go dopisać do `name` (w „Sklepie z zabawkami”: `'krzysztofie'`).

---

## 14. Zmiana gracza: guziec

Wejście do żelaznej klatki zamienia gracza w guźca:

```inform
after [;
    Enter:
        print "Szkielety zamieszkujące klatkę ożywają, ...^";
        move warthog to Antechamber;
        remove skeletons;
        give self ~open;
        give warthog light;
        self.after = 0;
        ChangePlayer(warthog, 1);
        <<Look>>;
    ],
```

**`ChangePlayer(obiekt, 1)`** sprawia, że gracz od tej chwili steruje podanym obiektem. Druga wartość, 1, każe bibliotece dopisywać w nagłówku miejsca „(jako Guziec)”, żeby gracz pamiętał, kim jest. Pierwotna postać gracza jest zawsze dostępna jako `selfobj`; w Szybie grobowym gra przywraca ją wywołaniem `ChangePlayer(selfobj)`.

`<<Look>>` wykonuje akcję rozejrzenia się, a potem kończy regułę wartością „prawda”.

Guziec ma własną właściwość `orders`. Gdy obiekt jest postacią gracza, `orders` decyduje, które polecenia gracza zostaną wykonane: zwrócenie fałszu przepuszcza akcję, a wypisanie tekstu ją odrzuca. Guziec może chodzić, oglądać, wąchać i robić jeszcze kilka prostych rzeczy; na wszystko inne gra odpowiada „Guźce nie potrafią robić niczego tak skomplikowanego…”.

```inform
orders [;
    Go, Look, Examine, Smell, Taste, Touch, Search, Jump, Enter:
        rfalse;
    Eat:
        "Nie masz jeszcze wprawy w wywęszaniu jedzenia.";
    default:
        "Guźce nie potrafią robić niczego tak skomplikowanego. ...";
    ],
```

---

## 15. Księga do sprawdzania: słownik glifów

Słownik Waldecka obsługuje akcję **`Consult`**. Gracz pisze „sprawdź ptaka w słowniku”, „poszukaj półksiężyca w słowniku” albo „przeczytaj o glifie q1 w słowniku”. Biblioteka nie rozpoznaje tematu jako obiektu, tylko podaje jego położenie w poleceniu: temat zaczyna się od słowa o numerze `consult_from` i liczy `consult_words` słów:

```inform
before [ w1 w2 glyph;
    Consult:
        wn = consult_from;
        w1 = NextWord();
        w2 = NextWord();
        if (consult_words == 1 && (~~PL_Glif(w1))) glyph = w1;
        else if (consult_words == 2 && PL_Glif(w1)) glyph = w2;
        else if (consult_words == 2 && PL_Glif(w2)) glyph = w1;
        else "Spróbuj ~sprawdź <nazwa glifu> w słowniku~.";
        switch (glyph) {
            'q1':
                "(To jedyny glif, który znasz na pamięć!)^^
                 Q1: ~miejsce święte~.";
            'ptak', 'ptaka', 'ptaku':
                if (self.correct) "Ptak: ~martwy jak kamień~.";
                "Ptak: ~bogaty, zamożny?~.";
            ...
            default:
                "Tego glifu jak dotąd nie odnotowano.";
        }
    ],
```

`PL_Glif` sprawdza, czy słowo jest którąś z form wyrazu „glif”; dzięki temu „sprawdź glif ptak” i „sprawdź ptaka” dają ten sam wynik. Tak jak przy rozmowie, każdy glif trzeba wymienić we wszystkich formach, które gracz może wpisać: w mianowniku („sprawdź ptak”), w dopełniaczu lub bierniku („sprawdź ptaka”) i w miejscowniku („o ptaku”).

**Pułapka składni Inform:** operator zaprzeczenia `~~` wiąże słabiej niż `&&`. Zapis `a && ~~b` działa zgodnie z oczekiwaniem, ale `~~a && b` znaczy `~~(a && b)`. Dlatego w kodzie zaprzeczenie stoi w nawiasie: `(~~PL_Glif(w1))`.

---

## 16. Własne czasowniki

Ruiny dodają czasownik fotografowania. Najpierw powstaje procedura akcji; jej nazwa to nazwa akcji z przyrostkiem `Sub`:

```inform
[ PhotographSub;
    if (camera notin player) "Nie bez aparatu.";
    if (noun == player) "Lepiej nie. Nie golisz się od wyjazdu z Meksyku.";
    if (children(player) > 1)
        "Fotografowanie to kłopotliwe zajęcie, wymagające obu rąk. ...";
    if (location == Forest) "W tym przesiąkniętym deszczem lesie lepiej nie.";
    if (location == thedark) "Jest o wiele za ciemno.";
    if (AfterRoutines()) return;
    print_ret "Rozstawiasz ... i cierpliwie naświetlasz zdjęcie ",
               (dop) noun, ".";
];
```

Biblioteka sama wywołuje reguły `before`, ale reguły `after` nowej akcji trzeba uruchomić ręcznie, wywołując `AfterRoutines()`. Dzięki temu klasa `Treasure` zapamiętuje, że skarb został sfotografowany. Jeśli któraś reguła `after` zwróci prawdę, procedura kończy się bez wypisywania własnego tekstu.

Potem gramatyka, którą umieszcza się **po** `Include "PolskiG";`:

```inform
Verb 'sfotografuj' 'fotografuj' 'uwiecznij'
    * noun                                      -> Photograph;

Verb 'zrób'
    * 'zdjęcie'/'fotografię' noun               -> Photograph;
```

### Zasady polskiej gramatyki

- **Przypadek nie ma znaczenia dla parsera.** Token `noun` przyjmuje nazwę obiektu w każdym przypadku: „zrób zdjęcie figurki” (dopełniacz) i „sfotografuj figurkę” (biernik) działają tak samo. Osobne tokeny dla przypadków nie są potrzebne. Przypadek ma znaczenie tylko w pytaniach parsera („Co chcesz sfotografować?”), a ten dobierają tabele w `polski.h`.
- **Czasownik w trybie rozkazującym.** Gracz pisze „weź”, „otwórz”, „zbadaj”, a nie „brać” czy „biorę”. Nazwy przedmiotów biblioteka rozpozna również bez polskich liter („zjedz cetkowany grzyb”), ale czasowniki i słowa w liniach gramatyki muszą być w słowniku dokładnie w tej postaci, w jakiej gracz je wpisze. Jeśli ma działać „zrob zdjecie”, trzeba dopisać `'zrob'` i `'zdjecie'` jako osobne słowa.
- **Słowo „się” jest usuwane** przed analizą polecenia, więc „rozejrzyj się” i „wespnij się na roślinę” trafiają do parsera jako „rozejrzyj” i „wespnij na roślinę”. Linia gramatyki z `'się'` nigdy by się więc nie dopasowała.
- **Jednoliterowe przyimki** zapisuje się z `//`: `'w//'`, `'z//'`, `'o//'`. Bez tego dopisku kompilator potraktowałby `'w'` jako stałą znakową, a nie słowo słownika.
- **Istniejący czasownik** rozszerza się przez `Extend`. Zmiana obejmuje wtedy wszystkie jego synonimy. Jeśli ma dotyczyć tylko jednego słowa, trzeba użyć `Extend only`, które odłącza je od grupy. Przykład z „Przygody”: „złap” jest synonimem „weź”, a łapanie ptaka ma działać tylko dla „złap”:

  ```inform
  Extend only 'złap' first
      * creature              -> Catch
      * creature 'do'/'w//' held -> Catch;
  ```

Biblioteka czasem wypisuje czasownik w bezokoliczniku, np. w pytaniu „Co chcesz **sfotografować**?”. Bezokolicznik bierze z tabeli w `LanguageVerb` (w `polski.h`). Jeśli czasownika tam nie ma, wypisuje słowo ze słownika, czyli tryb rozkazujący, co daje niezgrabne „Co chcesz sfotografuj?”. Słowo słownika jest przy tym obcięte do dziewięciu z-znaków, więc w Ruinach wyszłoby nawet „Co chcesz sfotograf?”. Dlatego gra definiuje punkt wejścia `PrintVerb`, który dla własnych czasowników drukuje bezokolicznik i zwraca prawdę, a dla pozostałych zwraca fałsz i zostawia sprawę bibliotece. Musi on stać **przed** `Include "PolskiG"`, bo `PolskiG` definiuje pustą wersję zastępczą dla gier, które go nie mają:

```inform
[ PrintVerb v;
    if (v == 'sfotografuj' or 'fotografuj' or 'uwiecznij' or 'zrób') {
        print "sfotografować"; rtrue;
    }
    rfalse;
];
```

---

## 17. Początek i koniec gry

### `Initialise`

Procedura `Initialise` ustawia stan początkowy gry: miejsce startowe, przedmioty gracza i demony. Tekst wypisany na jej końcu staje się wstępem do gry:

```inform
[ Initialise;
    TitlePage();
    location = Forest;
    move map to player;
    move sodium_lamp to player;
    move dictionary to player;
    StartDaemon(sodium_lamp);
    thedark.description = "Ciemność wieków napiera na ciebie ...";
    "^^^Dni poszukiwań, dni przedzierania się przez cierniste zarośla
     lasu z wyschniętym gardłem, ale w końcu twoja cierpliwość została
     nagrodzona. Odkrycie!^";
];
```

### Strona tytułowa, która działa i w maszynie Z, i w Glulx

Strona tytułowa czyści ekran, wyśrodkowuje tytuł, wyświetla cytat w ramce i czeka na naciśnięcie klawisza. Zamiast instrukcji maszyny Z używa procedur biblioteki (`ClearScreen`, `ScreenWidth`, `KeyCharPrimitive`), dlatego kompiluje się także dla Glulx:

```inform
[ TitlePage i;
    ClearScreen(WIN_ALL); print "^^^^^^^^^^^^^";
    i = ScreenWidth(); if (i > 30) i = (i-30)/2;
    style bold; font off; spaces(i);
    print " RUINY^";
    style roman; print "^^"; spaces(i);
    print "[Naciśnij SPACJĘ, aby rozpocząć.]^";
    font on;
    box "Lecz Aligator nie kopał dna jamy,"
        ...
        "-- z Popol Vuh";
    KeyCharPrimitive();
    ClearScreen(WIN_ALL);
];
```

### Koniec gry: `deadflag`

| `deadflag` | Znaczenie | Komunikat |
|---|---|---|
| 1 | śmierć | „Zginąłeś” / „Zginęłaś” |
| 2 | zwycięstwo | „Zwyciężyłeś” / „Zwyciężyłaś” |
| 3 i więcej | własne zakończenie | tekst z `DeathMessage` |

W Ruinach skok w przepaść ustawia `deadflag = 3`, a `DeathMessage` drukuje wtedy „Zostałeś pojmany” albo „Zostałaś pojmana”.

### Rangi

`PrintRank` dopisuje rangę do komunikatu o wyniku. Zdanie biblioteki kończy się liczbą ruchów, bez kropki, więc tekst rangi musi zaczynać się od przecinka i sam zamknąć zdanie:

```inform
[ PrintRank;
    print ", co czyni cię ";
    if (score == 30) "dyrektorem Instytutu Carnegie.";
    if (score >= 20) "archeologiem.";
    if (score >= 10) "poszukiwaczem osobliwości.";
    if (score >= 5) "odkrywcą.";
    "turystą.";
];
```

Wynik: „Do tej pory zdobyłeś 15 punktów na 30 możliwych, w 42 ruchach, co czyni cię poszukiwaczem osobliwości.” Nazwy rang stoją w narzędniku, bo tego przypadku wymaga „czyni cię (kim?)”.

---

## 18. Testowanie

### Skrypt przejścia

Najskuteczniejszym testem jest plik z poleceniami, które przechodzą grę od początku do końca. Po każdej zmianie wystarczy go uruchomić, żeby sprawdzić, czy gra wciąż daje się ukończyć. Dla Ruin jest to `games/ruins_pl.przejscie`:

```bash
./build.sh ruins_pl
narzedzia/bin/dfrotz -m -Z 0 -s 1 build/ruins_pl.z5 < games/ruins_pl.przejscie
```

Pierwszy, pusty wiersz skryptu zastępuje naciśnięcie klawisza na stronie tytułowej. Szybki przegląd problemów daje przefiltrowanie wyniku:

```bash
narzedzia/bin/dfrotz -m -Z 0 -s 1 build/ruins_pl.z5 < games/ruins_pl.przejscie \
  | grep -E "Nie widzisz|Nie znam|Nie rozumiem|Zwyci|Zgin"
```

„Nie widzisz niczego takiego” i „Nie znam takiego czasownika” oznaczają zwykle błąd w skrypcie albo brakujący synonim. Jeśli na końcu brakuje „Zwyciężyłeś”, coś po drodze potoczyło się inaczej, niż zakładał skrypt.

### Polecenia debugowania

Po kompilacji z opcją `-D` (`./build.sh ruins_pl -D`) dostępne są polecenia debugowania biblioteki:

| Polecenie | Działanie |
|---|---|
| `purloin X` | daje graczowi przedmiot, gdziekolwiek jest |
| `gonear X` | przenosi gracza do przedmiotu |
| `tree` | drzewo obiektów |
| `actions` | wypisuje każdą wykonywaną akcję |
| `trace 2` | śledzenie parsera |
| `showobj X` | wszystkie właściwości obiektu |

`trace 2` pokazuje, jak parser podzielił polecenie i którą linię gramatyki wybrał. Słowa spoza słownika widać w nim jako `xxnieznan`. Tak biblioteka oznacza każde słowo, którego nie ma w słowniku, w tym formy z automatycznej odmiany; rozpoznaje je później, porównując znaki. To nie jest błąd.

### Typowe pułapki

- **Ograniczenia maszyny Z.** Właściwość `name` może mieć najwyżej 32 słowa. Słowa słownika są obcinane do 9 z-znaków, a polska litera zajmuje dwa, dlatego „wspaniałego” i „wspaniałemu” są dla parsera tym samym słowem.
- **Brak rodzaju.** Bez `female`, `neuter` albo `pluralname` każda nazwa odmienia się jak męska, a komunikaty używają męskich końcówek.
- **`PolskiZ` nie na początku.** Polskie litery w tekstach skompilowanych przed nim będą zniekształcone.
- **`~~` bez nawiasów** (rozdział 15).
- **Wejście w Glulx.** Glulx nie przyjmuje na wejściu polskich liter, a czasowniki bez ogonków nie są rozpoznawane. Grę w Glulx da się skompilować i uruchomić, ale wygodnie gra się tylko w maszynie Z.

---

## 19. Ściągawki

### Notacja odmiany

| Zapis | Co robi | Przykład |
|---|---|---|
| `rdzeń/końcówka` | odmiana według końcówki | `lamp/a` |
| `rdzeń/` | pusta końcówka | `kamyk/` |
| `/x/` | litery tylko w mianowniku | `kłęb/e/k/` |
| `/x=y/` | wymiana w rdzeniu | `st/ó=o/ł/` |
| `+u` | dopełniacz -u | `aparat/+u` |
| `+e`, `+ie` | wstawka w dopełniaczu mnogim | `malowidł/a+e`, `okn/a+ie` |
| `+y`, `+ów`, `+0` | wymuszony dopełniacz mnogi: -y/-i, -ów, bez końcówki | `inskrypcj/e+y`, `imienin/y+0` |
| `!` | odmiana przymiotnikowa | `lamp/a sodow/a!` |
| bez `/` | słowo nieodmienne | `z jadeitu` |
| `desc_dop` … `desc_mie` | ręczna forma przypadku | `desc_cel "wielkiemu psu"` |

### Reguły wydruku

`(mian)` `(dop)` `(cel)` `(bie)` `(nar)` `(mie)` i ich wersje z wielką literą `(CMian)` … `(CMie)`.

### Zgoda gramatyczna

`PL(o, "jest", "są")` · `KoncY(o)` · `Forma(o, m, f, n, p, pm)` · `Gracz("zrobiłeś", "zrobiłaś")` · `JestSa(o)` · `GoJaJe(o)` · `Liczebnik(n, "punkt", "punkty", "punktów")`

### Atrybuty rodzaju

brak = męski · `female` · `neuter` · `pluralname` · `animate` (żywy) · `pluralname animate male` (męskoosobowy)

### Szkielet pliku

```inform
!% -Cu
!% +language_name=polski
Include "PolskiZ";
Constant Story "…";
Constant Headline "…";
Include "Parser";
Include "VerbLib";
! … obiekty, klasy, procedury, Initialise …
Include "PolskiG";
! … własne czasowniki …
```

### Gdzie szukać dalej

- [Podręcznik gracza](podrecznik_gracza.md) – wszystkie polecenia z punktu widzenia gracza; przydaje się jako spis tego, co gra powinna rozumieć.
- [Suplement: jak zbudować różne mechanizmy](suplement.md) – techniki ze wszystkich gier przykładowych: pojemniki, demony i zegary, światło, zmienne nazwy, identyczne przedmioty, `parse_name`, tokeny gramatyki, postacie i rozkazy, zasięg, zaklęcia, drzwi i pojazdy, własny wygląd, punktacja i wskrzeszanie.
- `games/ruins_pl.inf` – cała gra opisana w tym podręczniku.
- `games/alice_pl.inf` – „Po drugiej stronie lustra”: jeden pokój, dwa kotki z demonami i własną klasą, nowe czasowniki (oglądanie w lustrze, zwijanie wełny), gracz w rodzaju żeńskim.
- `games/toyshop_pl.inf` – zbiór nietypowych obiektów: pojazd, wiele identycznych przedmiotów (osiem świec), rękawiczki, które bywają parą, pisanie na tablicy, postać wykonująca polecenia.
- `games/balances_pl.inf`, `games/museum_pl.inf`, `games/adventureland_pl.inf` – programowanie parsera, zaklęcia, rozkazy dla maszyn i własny wygląd gry; omawia je suplement.
- `games/advent_pl.inf` – duża gra: labirynty, krasnoludy, pirat, magiczne słowa, wskrzeszanie, teleportacja po nazwach miejsc.
- `lib/odmiana.h` – opis notacji odmiany w komentarzu na początku pliku.
- `lib/polski.h` – wszystkie komunikaty biblioteki po polsku i procedury pomocnicze.
