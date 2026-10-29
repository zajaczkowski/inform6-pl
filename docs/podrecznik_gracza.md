# Jak grać w polskie gry tekstowe

### Podręcznik gracza dla gier napisanych z biblioteką inform6-pl

Ten podręcznik jest przeznaczony dla **graczy**, a nie dla autorów. Wyjaśnia, na czym polega gra tekstowa i jakie polecenia rozumieją gry zbudowane na bibliotece inform6-pl. Większość przykładów pochodzi z dwóch gier z katalogu `games/`:

- **„Ruiny”** (`ruins_pl.inf`) – krótkiej wyprawy archeologicznej do majańskiej świątyni w dżungli,
- **„Przygody”** (`advent_pl.inf`) – polskiej wersji klasycznej *Colossal Cave Adventure*, pierwszej gry tego gatunku.

W katalogu znajdują się też:

- „Po drugiej stronie lustra” (`alice_pl.inf`) – gra w jednym pokoju, na motywach Lewisa Carrolla; dobra na początek,
- „Sklep z zabawkami” (`toyshop_pl.inf`) – sklep pełen zabawek, z którymi można robić zaskakujące rzeczy,
- „Równowaga” (`balances_pl.inf`) – krótka opowieść o czarodzieju, który uczy się i rzuca zaklęcia,
- „Muzeum Informu” (`museum_pl.inf`) – muzeum sztuczek programistycznych do swobodnego zwiedzania; gra nie ma zakończenia,
- „Kraina Przygód” (`adventureland_pl.inf`) – klasyczna gra Scotta Adamsa o zbieraniu skarbów.

We wszystkich grach podstawowe polecenia działają tak samo. Niektóre gry mają też własne, nietypowe polecenia – opisuje je rozdział 12.

Podręcznik nie zdradza rozwiązań zagadek. Przykłady pokazują tylko, *jak* pisać polecenia, a nie *jakie* polecenia rozwiązują grę.

---

## Spis treści

1. [Czym jest gra tekstowa](#1-czym-jest-gra-tekstowa)
2. [Uruchamianie gry](#2-uruchamianie-gry)
3. [Pierwsze minuty](#3-pierwsze-minuty)
4. [Jak pisać polecenia](#4-jak-pisać-polecenia)
5. [Rozglądanie się](#5-rozglądanie-się)
6. [Poruszanie się](#6-poruszanie-się)
7. [Przedmioty](#7-przedmioty)
8. [Postacie](#8-postacie)
9. [Światło i niebezpieczeństwa](#9-światło-i-niebezpieczeństwa)
10. [Polecenia o samej grze](#10-polecenia-o-samej-grze)
11. [Gdy gra cię nie rozumie](#11-gdy-gra-cię-nie-rozumie)
12. [Rady dla początkujących](#12-rady-dla-początkujących)
13. [Ściągawka](#13-ściągawka)

---

## 1. Czym jest gra tekstowa

Gra tekstowa (ang. *interactive fiction*) to opowieść, w której to ty decydujesz, co robi bohater. Gra opisuje miejsce, w którym się znajdujesz, a ty wpisujesz polecenie po znaku `>`. Gra odpowiada, co się stało, i czeka na następne polecenie.

```
"Wielki Plac"
Tak przynajmniej nazywasz w notatkach tę niską wapienną skarpę, choć las
deszczowy dawno już ją sobie odebrał. ...
Z rozmokłej ziemi wyrasta na długiej nóżce cętkowany grzyb.

>zbadaj grzyb
Kapelusz grzyba pokrywają plamy i wcale nie masz pewności, czy to nie muchomor.
```

Nie ma tu obrazków ani klikania: całą akcję prowadzisz słowami. Celem jest zwykle rozwiązanie zagadek i dotarcie do zakończenia. W „Ruinach” trzeba zebrać i zapakować skarby, a w „Przygodzie” – odnaleźć wszystkie skarby jaskini i wynieść je na powierzchnię.

U góry ekranu widać **linię statusu**: nazwę miejsca, w którym jesteś, wynik i liczbę wykonanych ruchów.

---

## 2. Uruchamianie gry

Gry są zapisane w formacie maszyny Z (pliki `.z5`) albo Glulx (`.ulx`). Do grania potrzebny jest **interpreter**, czyli program, który otwiera takie pliki. Do wyboru jest między innymi:

- **Frotz** w terminalu (macOS, Linux): `frotz build/ruins_pl.z5`,
- **Gargoyle**, **Lectrote** albo **Spatterlight** – programy okienkowe,
- **Parchment** – interpreter w przeglądarce.

Obie wersje działają tak samo. Jeśli interpreter otwiera oba formaty, wybierz `.z5`: tylko w nim tekst wpisany w cudzysłowie (np. napis na tablicy) zachowa polskie litery.

---

## 3. Pierwsze minuty

Każdą grę warto zacząć tak samo:

1. **Przeczytaj opis miejsca.** Wymienione w nim przedmioty i kierunki są wskazówkami.
2. **Sprawdź, co masz przy sobie:** `ekwipunek` albo `i`.
3. **Zbadaj wszystko, co ciekawe:** `zbadaj mapę`, `x słownik`.
4. **Obejrzyj okolicę:** spróbuj pójść w kierunkach wymienionych w opisie.

W „Ruinach” wygląda to mniej więcej tak:

```
>i
Masz przy sobie:
  słownik dialektu Majów Waldecka
  lampę sodową
  szkicową mapę Quintana Roo

>zbadaj mapę
Mapa nie zaznacza wiele więcej niż strumień, który cię tu przywiódł, ...

>zbadaj schody
Już po kilku stopniach drogę tarasuje gruz.
```

A w „Przygodzie”, na początku drogi:

```
Koniec drogi
Stoisz na końcu drogi, przed małym budynkiem z cegły. Dookoła rośnie las.
Z budynku wypływa niewielki strumień i spływa w dół parowem.

>wejdź do budynku
W budynku
Jesteś w budynku - w domku zbudowanym nad dużym źródłem.
Na ziemi leżą jakieś klucze.
Leży tu smaczne jedzenie.
...
```

„Przygoda” ma wbudowaną pomoc z instrukcją i historią gry – wystarczy wpisać `pomoc`. W pozostałych grach `pomoc` pokazuje krótką listę najważniejszych poleceń.

---

## 4. Jak pisać polecenia

### Tryb rozkazujący

Polecenia pisze się tak, jak wydaje się rozkaz bohaterowi: **najpierw czasownik w trybie rozkazującym, a po nim przedmiot**.

| Tak | Nie tak |
|---|---|
| `weź lampę` | `biorę lampę`, `brać lampę` |
| `otwórz drzwi` | `otwieram drzwi` |
| `zbadaj kokon` | `co to za kokon?` |

### Przypadki – jak w zwykłym zdaniu

Pisz zwykłą polszczyzną i odmieniaj słowa tak, jak wymaga tego czasownik. Gra rozpozna każdą formę:

```
>weź skrzynię ze skarbem
>daj gniazdo trollowi
>połóż kamyk na ołtarzu
>wsadź kokon w snop światła
>zapytaj kapłana o malowidła
```

Gra nie wymaga poprawnych przypadków – „weź lampa” też zostanie zrozumiane – ale naturalna polszczyzna działa najpewniej, bo wtedy przyimki i formy słów pomagają grze zrozumieć, o co chodzi.

### Wielkość liter i polskie znaki

Wielkość liter nie ma znaczenia, a polskie litery można pomijać: `wez lampe`, `zjedz cetkowany grzyb` i `idz na polnocny zachod` działają tak samo jak wersje z ogonkami.

### Przymiotniki rozstrzygają wątpliwości

Gdy w pobliżu są dwa podobne przedmioty, gra zapyta, o który z nich chodzi:

```
>zbadaj lampę
Co masz na myśli: mosiężną lampę czy srebrną lampę?

>mosiężną
```

Wystarczy odpowiedzieć jednym słowem, które je odróżnia. Można też od razu napisać `zbadaj mosiężną lampę` i uniknąć pytania.

### Zaimki

Zamiast powtarzać nazwę, można użyć zaimka: **go, jego, mu, nim** (rodzaj męski i nijaki), **ją, jej, nią, niej** (żeński), **je, ich, im, nich** (liczba mnoga) albo **to**:

```
>weź klucz
Wzięte.
>zbadaj go
```

Polecenie `zaimki` pokazuje, do czego odnosi się teraz każdy zaimek.

### Kilka przedmiotów i kilka poleceń naraz

```
>weź lampę i klucze
>weź wszystko
>upuść wszystko oprócz lampy
>weź klucze. idź na zachód
>weź grzyb, potem zjedz go
```

Kolejne polecenia oddziela kropka albo słowo „potem”. Słowo `wszystko` działa przy braniu, odkładaniu i wkładaniu.

### „Się” możesz pomijać

`rozejrzyj się`, `wespnij się na roślinę`, `napij się wody` – gra po prostu pomija słowo „się”, więc polecenie działa zarówno z nim, jak i bez niego.

---

## 5. Rozglądanie się

| Polecenie | Skrót | Co robi |
|---|---|---|
| `rozejrzyj się` | `l` | opisuje miejsce jeszcze raz |
| `zbadaj X`, `obejrzyj X`, `przyjrzyj się X` | `x X` | dokładny opis przedmiotu lub postaci |
| `przeczytaj X` | | czyta napisy, kartki, tablice |
| `zajrzyj do X`, `zajrzyj pod X` | | co jest w środku albo pod spodem |
| `przeszukaj X` | | przeszukuje pojemnik albo miejsce |
| `spójrz na X` | | to samo co „zbadaj” |
| `posłuchaj`, `posłuchaj X` | | odgłosy |
| `powąchaj`, `powąchaj X` | | zapachy |
| `dotknij X` | | dotyk |

Przykłady z „Ruin”:

```
>posłuchaj
Wyjce, nietoperze, papugi, ary.

>powąchaj mgłę
Mgła pachnie trochę jak tortille.
```

**Badaj wszystko.** W grach tekstowych opisy przedmiotów często zawierają wskazówki. Glify w „Ruinach” można poznać właśnie z opisów rzeźb i kamieni.

### Księgi i słowniki

W niektórych książkach można sprawdzić hasło. W „Ruinach” służy do tego słownik glifów:

```
>sprawdź ptaka w słowniku
Ptak: "bogaty, zamożny?".
```

Działa też `poszukaj X w słowniku`, `znajdź X w słowniku` i `przeczytaj o X w słowniku`.

---

## 6. Poruszanie się

### Kierunki

| Kierunek | Pełne polecenie | Skrót |
|---|---|---|
| północ | `idź na północ`, `na północ`, `północ` | `pn` |
| południe | `idź na południe` | `pd` |
| wschód | `idź na wschód` | `w` |
| zachód | `idź na zachód` | `z` |
| północny wschód | `idź na północny wschód` | `pnw` |
| północny zachód | `idź na północny zachód` | `pnz` |
| południowy wschód | `idź na południowy wschód` | `pdw` |
| południowy zachód | `idź na południowy zachód` | `pdz` |
| góra | `idź w górę`, `w górę`, `do góry` | `g` |
| dół | `idź w dół`, `na dół`, `zejdź` | `d` |
| do środka / na zewnątrz | `wejdź` / `wyjdź` | |

Uwaga na dwie pułapki: **`w` oznacza wschód** (a nie zachód), a samo **`n` znaczy „nie”**.

### Wchodzenie i schodzenie

| Polecenie | Przykład |
|---|---|
| `wejdź do X` | `wejdź do budynku`, `wejdź do klatki` |
| `wejdź na X`, `wespnij się na X` | `wejdź na ołtarz`, `wespnij się na roślinę` |
| `zejdź`, `zejdź z X` | z ołtarza – zejdzie z niego, na schodach – zejdzie w dół |
| `zejdź po X` | `zejdź po schodach` |
| `wyjdź`, `wstań` | wychodzi z pojazdu, klatki, wstaje z krzesła |
| `przejdź przez X` | `przejdź przez drzwi` |
| `przeskocz X`, `przeskocz przez X` | `przeskocz przepaść` |

### Czego nie ma

Nie ma polecenia „wróć”: gra nie pamięta drogi za ciebie. Zapamiętuj, skąd przychodzisz, albo rysuj mapę (zob. rozdział 12).

### Przygoda: szybkie przejścia

W „Przygodzie” do miejsca, w którym już byłeś, można wrócić, wpisując jego nazwę albo `idź do` i nazwę:

```
>budynek
>idź do budynku
```

---

## 7. Przedmioty

### Branie i odkładanie

| Polecenie | Znaczenie |
|---|---|
| `weź X`, `podnieś X` | bierze przedmiot |
| `weź X z Y`, `wyjmij X z Y` | wyjmuje z pojemnika albo zdejmuje z podstawki |
| `upuść X`, `odłóż X`, `zostaw X` | kładzie na ziemi |
| `włóż X do Y`, `wrzuć X do Y` | wkłada do pojemnika |
| `połóż X na Y`, `postaw X na Y` | kładzie na czymś |
| `ekwipunek`, `i`, `co mam`, `co mam przy sobie` | lista tego, co masz |

```
>weź aparat ze skrzyni
Wzięte.
>włóż figurkę do skrzyni
```

Bohater może nieść tylko kilka rzeczy naraz. Gdy ma pełne ręce, gra odpowie:

```
Masz już przy sobie za dużo rzeczy.
```

Trzeba wtedy odłożyć coś niepotrzebnego.

### Otwieranie, zamykanie, klucze

```
>otwórz pudełko
>zamknij klatkę
>otwórz drzwi kluczem
>otwórz kratę kluczami
>zamknij drzwi na klucz
>zamknij bramę na łańcuch
```

Drzwi zamknięte na klucz otwiera się kluczem: `otwórz drzwi kluczem`. Jeśli masz właściwy klucz przy sobie, wystarczy `otwórz drzwi` – gra sama otworzy zamek. Zamykasz `zamknij drzwi na klucz` albo `zamknij drzwi kluczem`; przy „na klucz” gra sama wybierze klucz, który pasuje.

### Włączanie i wyłączanie

| Polecenie | Przykład |
|---|---|
| `włącz X`, `zapal X` | `włącz lampę`, `zapal lampę` |
| `wyłącz X`, `zgaś X` | `zgaś lampę` |
| `zapal X` (rzeczy palne) | `zapal zapałkę` – podpala |
| `podpal X` | podpala |

„Zapal” działa jak „włącz” w przypadku lamp, a jak „podpal” w przypadku zapałek czy świec.

### Ubrania

```
>załóż maskę
>zdejmij maskę
```

### Jedzenie, picie, ciecze

| Polecenie | Przykład |
|---|---|
| `zjedz X` | `zjedz grzyb` |
| `wypij X`, `napij się X` | `wypij wodę`, `napij się wody` |
| `napełnij X`, `napełnij X Y-iem`, `nalej do X` | `napełnij butelkę`, `napełnij butelkę wodą` |
| `opróżnij X`, `wylej X` | wylewa zawartość |
| `podlej X`, `polej X wodą/olejem`, `naoliw X` | w „Przygodzie” |

### Manipulowanie

| Polecenie | Uwagi |
|---|---|
| `pchnij X`, `przesuń X`, `naciśnij X` | |
| `pchnij X na północ` (itd.) | przetacza duży przedmiot do sąsiedniego miejsca |
| `pociągnij X`, `wyciągnij X` | |
| `przekręć X`, `obróć X` | |
| `potrzyj X`, `wytrzyj X` | |
| `pomachaj X` | `pomachaj prętem` |
| `rzuć X`, `rzuć X w Y`, `rzuć X Y-owi` | `rzuć samorodek trollowi` |
| `przywiąż X do Y`, `przetnij X` | |
| `kop`, `kop w X` | |

Niektóre gry mają własne czasowniki. W „Ruinach” można fotografować (`sfotografuj figurkę`, `zrób zdjęcie maski`), a w „Przygodzie” są między innymi `policz`, `uwolnij`, `złap`, `wysadź` oraz kilka magicznych słów, które trzeba odkryć samemu.

---

## 8. Postacie

W grach spotkasz postacie, z którymi można rozmawiać, którym można coś dać albo wydać polecenie.

### Rozmowa

Rozmowa polega na pytaniu i mówieniu **o konkretnych rzeczach**; swobodnej pogawędki gry nie rozumieją:

| Polecenie | Przykład z „Ruin” |
|---|---|
| `zapytaj X o Y` | `zapytaj kapłana o ruiny` |
| `powiedz X o Y` | `powiedz kapłanowi o słowniku` |
| `powiedz X Y`, `odpowiedz X Y` | `powiedz kapłanowi cześć` |
| `pozdrów X`, `przywitaj się z X` | `pozdrów kapłana` |

```
>zapytaj kapłana o ruiny
"Ruiny zawsze pokonają złodziei. W podziemnym świecie rabusie są torturowani
przez całą wieczność." Pauza. "Tak samo jak archeolodzy."
```

Temat podawaj jednym, najważniejszym słowem: „o ruiny”, „o malowidła”, „o słownik”. Pytaj o to, co widzisz w opisach – nazwy miejsc, przedmiotów i znaków często prowadzą do ciekawych odpowiedzi. Polecenie `porozmawiaj z X` przypomni, jak prowadzić rozmowę.

### Dawanie i pokazywanie

```
>daj gniazdo trollowi
>pokaż gazetę kapłanowi
>nakarm niedźwiedzia jedzeniem
```

### Polecenia dla postaci

Postaci można wydać polecenie: najpierw jej imię lub nazwa i przecinek, potem samo polecenie. Można użyć wołacza albo mianownika:

```
>kapłanie, idź na północ
"Nie wolno mi opuszczać sanktuarium."
```

Nie każda postać cię posłucha – większość ma własne zdanie.

W „Muzeum Informu” polecenia przyjmują także maszyny, w ten sam sposób – nazwa, przecinek, polecenie:

```
>budzik, 7:30
>komputer, gdzie jest kapitan picard
>zen, ustaw kurs na centauro
```

Niektóre postacie mają własne zasady: Charlotte słucha tylko poleceń zaczynających się od „szymon mówi” (`charlotte, szymon mówi pomachaj`).

### Tak i nie

Na niektóre pytania gry odpowiada się `tak` albo `nie` (w skrócie `t`, `n`). Na przykład w „Przygodzie”:

```
>zabij smoka
Czym? Gołymi rękami?

>tak
```

---

## 9. Światło i niebezpieczeństwa

### Ciemność

Wiele miejsc pod ziemią jest ciemnych. Bez światła gra powie tylko tyle:

```
Ciemność
Jest zupełnie ciemno i nic nie widzisz.
```

W ciemności nie widać przedmiotów, a chodzenie bywa niebezpieczne. W „Przygodzie” błądzenie po omacku grozi upadkiem do jamy, a w „Ruinach” z ciemności coś nadciąga. **Zanim zejdziesz pod ziemię, zadbaj o źródło światła.**

Lampy mają ograniczony zapas energii, a gdy zaczynają przygasać, gra ostrzega. Wyłączaj lampę w miejscach, które są oświetlone:

```
>wyłącz lampę
```

### Śmierć

W grach tekstowych bohater może zginąć. Gra zaproponuje wtedy kilka możliwości:

```
    *** Zginąłeś ***
W tej grze zdobyłeś 0 punktów na 6 możliwych, w 8 ruchach, ...
Czy chcesz zacząć od nowa (RESTART), wczytać zapisaną grę (WCZYTAJ), zobaczyć
pełny wynik (PEŁNY) czy zakończyć (KONIEC)?
```

Jeśli na liście jest też COFNIJ, możesz cofnąć ostatni, śmiertelny ruch. Zależy to jednak od interpretera, więc lepiej na to nie liczyć i mieć zapisaną grę.

Najprostsza rada brzmi więc: **często zapisuj grę** (rozdział 10). W „Przygodzie” po śmierci gra zaproponuje wskrzeszenie – ale nie za darmo.

---

## 10. Polecenia o samej grze

Poza `czekaj` żadne z tych poleceń nie zajmuje czasu w świecie gry: po nich nie mija żadna tura.

| Polecenie | Co robi |
|---|---|
| `zapisz` | zapisuje stan gry do pliku |
| `wczytaj` | wczytuje zapisaną grę |
| `cofnij` | cofa ostatni ruch (jeśli interpreter to obsługuje) |
| `powtórz`, `p` | powtarza ostatnie polecenie |
| `ups X` | poprawia literówkę w poprzednim poleceniu (`ups lampa`) |
| `restart` | zaczyna grę od nowa |
| `koniec`, `q` | kończy grę |
| `wynik` | pokazuje wynik |
| `pełny wynik` | wynik z wyszczególnieniem |
| `opisy pełne` | pełny opis miejsca przy każdej wizycie |
| `opisy skrócone` | sama nazwa miejsca przy ponownej wizycie |
| `opisy` | domyślny tryb opisów |
| `zapis`, `zapis wyłącz` | zapisuje przebieg gry do pliku tekstowego |
| `powiadamiaj tak` / `nie` | czy informować o zmianie wyniku |
| `zaimki` | pokazuje, do czego odnoszą się zaimki |
| `pomoc` | krótka pomoc (w „Przygodzie” – pełne menu) |
| `wersja` | informacje o grze i bibliotece |
| `czekaj`, `c` | mija jedna tura (gdy na coś czekasz) |

---

## 11. Gdy gra cię nie rozumie

| Komunikat | Co znaczy | Co zrobić |
|---|---|---|
| **Nie znam takiego czasownika.** | pierwsze słowo nie jest czasownikiem, który gra zna | użyj innego czasownika w trybie rozkazującym („weź”, „zbadaj”, „otwórz”) |
| **Nie widzisz niczego takiego.** | przedmiotu nie ma w zasięgu albo nazwa jest inna | przeczytaj opis jeszcze raz; spróbuj innego słowa z opisu |
| (literówka) | gra również odpowie „Nie widzisz niczego takiego.” | popraw błędne słowo poleceniem `ups` z poprawnym słowem: `ups grzyb` |
| **Zrozumiałem tylko tyle, że chcesz …** | początek polecenia jest dobry, reszta nie pasuje | uprość polecenie, sprawdź przyimek („do”, „na”, „w”) |
| **Nie zrozumiałem tego polecenia.** | polecenie nie pasuje do żadnego wzoru znanego grze | zacznij od czasownika i jednego przedmiotu |
| **Co masz na myśli: …?** | kilka przedmiotów pasuje | odpowiedz jednym słowem, które je odróżnia |
| **Nie możesz pójść w tę stronę.** | w tym kierunku nie ma wyjścia | sprawdź kierunki w opisie |
| **Tego nie da się …** | czynność nie ma sensu albo przedmiot na nią nie pozwala | spróbuj czegoś innego |

**Mniej znaczy więcej.** Gry rozumieją krótkie polecenia lepiej niż długie zdania. Zamiast „może spróbuję delikatnie otworzyć tę starą skrzynię” wystarczy napisać `otwórz skrzynię`.

---

## 12. Rady dla początkujących

1. **Rysuj mapę.** Zapisuj nazwy miejsc i kierunki, które je łączą. W jaskiniach „Przygody” korytarze się wiją: jeśli wyszedłeś na północ, droga na południe nie zawsze prowadzi z powrotem. W labiryntach wszystkie miejsca wyglądają tak samo, więc zostawiaj w nich przedmioty jako znaczniki.
2. **Zapisuj grę** przed każdym ryzykownym krokiem: `zapisz`.
3. **Badaj wszystko**, co pojawia się w opisach – także rzeczy w tle (ściany, napisy, mgłę).
4. **Czytaj odpowiedzi gry uważnie.** Odmowa często zawiera wskazówkę, np. „Rozpakowywanie tak bezcennego zabytku lepiej zostawić ludziom z Instytutu Carnegie” albo „Są lata trzydzieste … Zabranie zabytku bez udokumentowania jego kontekstu to zwykły rabunek”.
5. **Próbuj zwyczajnych czynności:** zjedz, wypij, zapal, pchnij, otwórz, daj, zapytaj. W „Ruinach” rozwiązaniem bywa coś zupełnie prostego.
6. **Rozmawiaj z postaciami** i pytaj je o to, co widzisz.
7. **Pilnuj światła** pod ziemią i nie marnuj baterii.
8. **Nie bój się porażki.** `cofnij` zwykle naprawi błąd z ostatniego ruchu, a zapisana gra – każdy wcześniejszy.

### O „Ruinach”

Jesteś archeologiem w latach trzydziestych XX wieku. Skarby trzeba odnaleźć, **sfotografować na miejscu**, a dopiero potem zabrać i zapakować do skrzyni transportowej. Słownik glifów Waldecka pomaga odczytać znaki na ścianach – sprawdzaj w nim każdy glif, który zobaczysz. Gra jest krótka; można w niej zdobyć 30 punktów.

### O „Przygodzie”

To duża gra z labiryntami, stworzeniami i magią. Pełną liczbę punktów za skarb dostaje się dopiero wtedy, gdy zostanie on **złożony w budynku, od którego zaczyna się gra**. Można zdobyć 350 punktów, a ostatnie z nich przyznaje się dopiero na samym końcu. Pomoc w grze (`pomoc`) zawiera instrukcję i historię gry.

### O „Równowadze”

Grasz czarodziejem. Zaklęcie trzeba najpierw **zapamiętać** z księgi zaklęć, a po rzuceniu wylatuje ono z pamięci. Zaklęcie wyuczone kilka razy wystarczy na tyle samo rzutów.

```
>naucz się frotz
>frotz monetę                (albo: rzuć frotz na monetę)
>zaklęcia                    (co masz w pamięci)
>zbadaj frotz                (co robi zaklęcie z twojej księgi)
>gnusto zwój                 (przepisuje zaklęcie ze zwoju do księgi)
>napisz kamień na sześcianie (rylcem; napis w cudzysłowie też działa)
```

Gra odpowiada na kilka pytań o świat: `kim jest helistar`, `co to jest grue`, `jak działają zaklęcia`. Pytanie `jak się czuję` pokazuje, w jakim jesteś stanie.

### O „Muzeum Informu”

To nie jest gra z zagadkami, tylko zbiór eksponatów. Tabliczki i ulotki mówią, co można z nimi zrobić. Oprócz poleceń dla maszyn (rozdział 8) muzeum zna kilka specjalnych czasowników, opisanych w Leksykonie na trzecim piętrze, między innymi `czas 6:30` albo `czas wpół do ósmej` (ustawia godzinę), `megaspójrz` (opis wszystkiego wokół), `powiększ X` (z lupą w ręku), `fp 3,14` i `wybierz 0-800-123`.

### O „Krainie Przygód”

Gra naśladuje styl gier Scotta Adamsa z końca lat siedemdziesiątych: krótkie opisy, lista widocznych wyjść i przedmioty w jednym wierszu. Nazwy skarbów są ujęte w gwiazdki (\*ZŁOTA KORONA\*), a skarby trzeba złożyć w jednym miejscu; `wynik` pokazuje postęp, a do zdobycia jest 100 punktów. Możesz nieść najwyżej sześć rzeczy. Lampa ma ograniczony zapas energii, więc gaś ją, kiedy nie jest potrzebna. Magiczne słowa, które znajdziesz w grze, wpisuje się tak, jak są napisane (po angielsku), bez żadnego czasownika.

---

## 13. Ściągawka

```
ROZGLĄDANIE   rozejrzyj się (l) · zbadaj X (x X) · przeczytaj X · zajrzyj do/pod X
              przeszukaj X · posłuchaj · powąchaj X · dotknij X

RUCH          pn pd w z pnw pnz pdw pdz g d · idź na północ · na północ · w górę
              wejdź (do/na X) · wyjdź · zejdź (z/po X) · wespnij się na X
              przejdź przez X · przeskocz X

PRZEDMIOTY    weź X · weź X z Y · upuść X · włóż X do Y · połóż X na Y
              ekwipunek (i) · co mam · weź wszystko · upuść wszystko oprócz X
              otwórz/zamknij X · otwórz X kluczem · zamknij X na klucz
              włącz/zapal X · wyłącz/zgaś X · załóż/zdejmij X
              zjedz X · wypij X · napełnij X · opróżnij X
              pchnij X (na północ) · pociągnij X · przekręć X · rzuć X w Y

POSTACIE      zapytaj X o Y · powiedz X o Y · powiedz X cześć · pozdrów X
              daj Y X · pokaż Y X · nakarm X Y · X, zrób coś · tak/nie

SPECJALNE     naucz się X · X przedmiot · zaklęcia (Równowaga)
              budzik, 7:30 · komputer, gdzie jest X (Muzeum)

GRA           zapisz · wczytaj · cofnij · powtórz (p) · ups X · restart · koniec (q)
              wynik · pełny wynik · opisy pełne/skrócone · zapis · pomoc · czekaj (c)
```
