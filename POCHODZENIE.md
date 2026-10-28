# Pochodzenie, inspiracje i autorzy

Ten plik opisuje, skąd pochodzą poszczególne części inform6-pl: co kontynuuje moją poprzednią polską bibliotekę, co zainspirowały inne tłumaczenia Inform 6, co pochodzi z projektów źródłowych i w czym pomagała sztuczna inteligencja.

## Poprzednia wersja: inform-pl 6/11/PL

inform6-pl to druga polska biblioteka Inform 6 mojego autorstwa. Pierwsza, **inform-pl w wersji 6/11/PL** (Bartosz Zajączkowski, 2011–2014), była oparta na bibliotece 6/11 i znajduje się w repozytorium [inform-pl](https://github.com/zajaczkowski/inform-pl). Pozostała niedokończona: miała poważne braki, z którymi nie umiałem wtedy sobie poradzić.

Z tamtej wersji pochodzą i zostały przeniesione na bibliotekę 6.12.8:

- **polskie tłumaczenie komunikatów biblioteki** – przeniesione do `lib/polski.h` i dostosowane do zmian w 6.12,
- **polskie słownictwo poleceń**, czyli czasowniki i ich synonimy – przeniesione do `lib/polskig.h`,
- **sposób zapisu przypadków w obiektach**: właściwości `name_dop` … `name_mie` (słowa rozpoznawane przez parser) i `desc_dop` … `desc_mie` (formy wypisywane w komunikatach). W nowej wersji służą do ręcznego nadpisywania automatycznej odmiany w wyjątkowych przypadkach i zapewniają zgodność ze starymi grami,
- **reguły wydruku nazw w przypadkach** oraz `Cname`, zachowane jako alias,
- **tokeny gramatyki dla przypadków** (`bierniknoun`, `dopelniacznoun` itd.), zachowane jako aliasy zwykłych tokenów, żeby stare gry kompilowały się bez zmian,
- **polskie tłumaczenia gier** „Alice”, „Ruins” i „Advent” (to ostatnie niepełne). Zostały w katalogu `Games` tamtego repozytorium; gry w `games/` są nowymi tłumaczeniami tych samych oryginałów.

Nowe w tej wersji są między innymi:

- obsługa polskich liter w alfabecie maszyny Z (`lib/polskiz.h`),
- automatyczna odmiana nazw (`lib/odmiana.h`),
- dobór przypadka do czasownika i przyimka w pytaniach parsera („Do czego chcesz włożyć…?”),
- rozpoznawanie słów wpisanych bez polskich liter,
- zgoda czasownika i dopisków z listami przedmiotów („Na stole **leżą** dwie świece”),
- polskie wersje tych procedur biblioteki, które mają tekst wpisany na sztywno (`Locale`, `Identical`),
- nowe tłumaczenia wszystkich siedmiu gier przykładowych z IF Archive oraz dokumentacja w `docs/`.

## Inspiracje: metoda odmiany przez końcówki

Zanim zaprojektowałem odmianę, przejrzałem tłumaczenia Inform 6 na języki fleksyjne dostępne w IF Archive (`if-archive/infocom/compilers/inform6/library/translations/`).

### Główna inspiracja: RInform

Metoda zastosowana w `lib/odmiana.h` jest wzorowana na rosyjskiej bibliotece **RInform** (Andrey Grankin 2002, Denis Gayev 2003–2004; wersja 0.75 dla biblioteki 6/11). Rosyjski, podobnie jak polski, ma sześć przypadków, a RInform jako jedyne z przejrzanych tłumaczeń odmienia nazwy automatycznie.

Z RInform pochodzą następujące pomysły:

- **znacznik w nazwie krótkiej**, oddzielający rdzeń od końcówki mianownika (`"медн/ая ламп/а"` w RInform, `"mosiężn/a lamp/a"` u nas),
- **tabele końcówek** zależne od rodzaju i postaci końcówki mianownika, z których biblioteka składa formy przypadków,
- **litery występujące tylko w mianowniku**, zapisywane między ukośnikami (`"клуб/о/к/"` w RInform, `"kłęb/e/k/"` u nas),
- **znacznik wyboru innego wzorca odmiany** (`!` w RInform) oraz **ręczne nadpisanie odmiany** dla pojedynczego obiektu,
- **rozpoznawanie przez parser każdej formy przypadka** w `LanguageRefers`.

Czym inform6-pl różni się od RInform:

- RInform przechowuje w słowniku rdzenie i sprawdza, czy reszta wpisanego słowa jest poprawną końcówką. inform6-pl **generuje** wszystkie formy nazwy i porównuje je z wpisanym słowem, także z pominięciem polskich liter. Dzięki temu wymiany w rdzeniu (*stół – stołu*, *przepaść – przepaści*) zapisuje się tylko raz, w samej nazwie.
- Doszły zapisy potrzebne polszczyźnie: wymiana w rdzeniu `/x=y/`, wskazówki dopełniacza `+u`, `+e`, `+ie`, `+y`, `+ów`, `+0`, liczba mnoga z rodzajem męskoosobowym oraz polskie wymiany spółgłosek przed końcówką -e (*lampa – lampie*, *półka – półce*, *stół – stole*).
- Znak `!` oznacza u nas „odmieniaj jak przymiotnik”, a nie numer alternatywnego wzorca.
- Ręcznym nadpisaniem odmiany są właściwości `desc_*` z poprzedniej wersji inform-pl, a nie osobna procedura.

### Inne przejrzane tłumaczenia

- **Deform** (niemieckie, Martin Oehm) – znaczniki końcówek przymiotników w nazwie krótkiej; pomysł podobny, ale ograniczony do czterech przypadków o prostej odmianie.
- **Tłumaczenie słoweńskie** (Benjamin Lipovšek) – formy przypadków wpisywane ręcznie w obiekcie, podobnie jak w inform-pl 6/11/PL.
- **INFSP** (hiszpańskie, Sarganar) i **tłumaczenie francuskie** (Lionel Ange) – bez przypadków; przydatne jako przykłady tłumaczeń dostosowanych do biblioteki 6.12.

Rozważałem też użycie słownika fleksyjnego **Morfeusz 2** do generowania form w czasie kompilacji. Ostatecznie wybrałem metodę opartą na końcówkach, bo nie wymaga ona żadnych zewnętrznych narzędzi: autorowi gry wystarczy kompilator i biblioteka.

## Projekty źródłowe

- **Kompilator Inform 6.44** i **biblioteka standardowa 6.12.8** – Graham Nelson, David Griffith, David Kinder i współtwórcy; [Inform6](https://github.com/DavidKinder/Inform6) i [inform6lib](https://gitlab.com/DavidGriffith/inform6lib). Biblioteka bazowa jest używana bez zmian. Pliki `lib/polski.h` i `lib/polskig.h` powstały na podstawie jej plików `english.h` i `grammar.h`.
- **Frotz** (dfrotz) – Stefan Jokisch, David Griffith i współtwórcy; [frotz](https://gitlab.com/DavidGriffith/frotz), interpreter używany do testów.
- **Gry przykładowe** przetłumaczone na polski:
  - `ruins_pl.inf` – „Ruins”, przykład z podręcznika *The Inform Designer's Manual* (4. wydanie) Grahama Nelsona, odtworzony z fragmentów podręcznika przez Rogera Firtha (2001); w grze przypisany fikcyjnej autorce Angeli M. Horns,
  - `alice_pl.inf` – „Through the Looking-Glass”, samouczek Inform Garetha Reesa (wersja 3, 2003, „All rights given away”), na motywach powieści Lewisa Carrolla,
  - `toyshop_pl.inf` – „Toyshop”, Graham Nelson (1994),
  - `advent_pl.inf` – „Adventure” (Colossal Cave), Will Crowther (1976) i Don Woods (1977); wersja Inform Grahama Nelsona, oparta na wersji Donalda Ekmana i Davida M. Baggetta,
  - `balances_pl.inf` – „Balances”, Graham Nelson (1994–1996), na motywach trylogii Enchanter firmy Infocom,
  - `museum_pl.inf` – „Museum of Inform”, Graham Nelson (1995–1996), zbiór rozwiązanych ćwiczeń z *The Inform Designer's Manual*,
  - `adventureland_pl.inf` – „Adventureland”, Scott Adams (ok. 1979); wersja Inform Grahama Nelsona (1994–1997).

Angielskie oryginały gier przykładowych są dostępne w [IF Archive](https://www.ifarchive.org/indexes/if-archive/games/source/inform/).

## Wykorzystanie sztucznej inteligencji

Przy tworzeniu tej wersji korzystałem z asystenta AI – **Claude** firmy Anthropic, używanego w narzędziu Claude Code. Asystent pomagał przy:

- przeniesieniu biblioteki z wersji 6/11/PL na bibliotekę 6.12.8 i kompilator 6.44,
- projekcie i kodzie automatycznej odmiany (`lib/odmiana.h`) oraz poprawkach w `lib/polski.h` i `lib/polskig.h`,
- tłumaczeniu wszystkich gier w `games/`,
- przeglądzie innych tłumaczeń Inform 6,
- pisaniu dokumentacji (`docs/podrecznik.md`, `docs/suplement.md`, `docs/podrecznik_gracza.md`, README),
- testowaniu: przygotowaniu skryptów przejścia gier i wyszukiwaniu błędów językowych.

Kierunek prac, decyzje projektowe i ocena wyników należały do mnie. Kod i teksty przygotowane z pomocą AI sprawdzałem przez kompilację, przejście gier skryptami testowymi i przegląd polszczyzny. Mimo to mogą zawierać błędy – zgłoszenia i poprawki są mile widziane.
