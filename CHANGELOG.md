# Historia zmian

Numer wydania składa się z wersji biblioteki standardowej Inform, na której
oparta jest polska wersja, i kolejnego numeru polskiego wydania:
`6.12.8-pl1` to pierwsze wydanie dla biblioteki 6.12.8.

## Niewydane

- Odmiana: narzędnik „krwią”, „brwią”, „głębią”; dopełniacz mnogi „pieców”,
  „koców”, „kupców”; celownik „kotu”, „psu”, „panu”, „bratu”, „ojcu”, „chłopcu”
  i podobne.
- „weź klucza” (potoczny dopełniacz) nie jest już brane za liczbę mnogą.
- Podręcznik: zapis rodzaju męskoosobowego i `+y` dla żeńskich w liczbie mnogiej.
- Gramatyka: tylko „włóż płaszcz” znaczy ubieranie („schowaj klucz” pyta, do czego),
  tylko „zdejmij”/„ściągnij” znaczy rozbieranie („wyjmij klucz” bierze);
  „ustaw pokrętło na 4”.
- Dokumentacja: przykłady kodu i zapisy sesji zgodne z obecnymi grami (polskie
  cudzysłowy, półpauza).
- Paczka wydania zawiera skompilowane gry raz, obok źródeł w `games/` (bez
  katalogu `skompilowane/`).
- Porządki w kodzie biblioteki: wspólne procedury zamiast powtórzeń
  (`PL_Wpis`, `PL_KosztZ`, `PL_Zyw`, `PL_NaLiscieBie`), poprawione komentarze;
  parser przyjmuje i „kotu”, i „kotowi”; na Glulxie wybór kierunku po nazwie
  z polskimi literami („północ”).

## 6.12.8-pl3 – 9 listopada 2026

**Uwaga dla autorów gier.** Biblioteka ma teraz własne polecenia `użyj`, `stan`,
`wróć`, `cześć`, `witaj`, `żegnaj`, `dziękuję`, `dzięki`, `rozbierz`, `ukryj`,
`złam`, `łam`, `zepsuj`, `mów`, `graj`, `budź`, `zamykaj`, `otwieraj`, `siadaj`
i `wstawaj`.  Gra, która definiuje któreś z nich sama dyrektywą `Verb`, nie
skompiluje się („Two different verb definitions”) i musi użyć `Extend ... replace`
albo `Extend only ... replace` (tak zrobiły „Przygoda” i „Równowaga”).
`Kto` przyjmuje opcjonalny czwarty argument; stare wywołania działają jak dotąd.

Nowe gry i dodatki:

- Gry z *The Inform Beginner's Guide* w `games/ibg/`: „Heidi”, „Wilhelm Tell”
  i „Kapitan Fate”, na licencji przewodnika (zob. `POCHODZENIE.md`).
- Katalog `extensions/` na dodatki spoza biblioteki standardowej; pierwszy
  dodatek `szyfr.h` – zamki szyfrowe („otwórz sejf kodem 15432”, „wpisz 15432”,
  „zamknij sejf na szyfr”) z instrukcją `szyfr_instrukcja.md` i grą przykładową.
- Skompilowane gry leżą w `games/` obok źródeł, a README ma linki do gry
  w przeglądarce (Parchment).

Odmiana i liczebniki:

- Liczebniki na listach: `plural "złot/e monet/y"` daje „dwie złote monety”,
  „pięć złotych monet”, „dwadzieścia dwa małe klucze”; czasownik zgadza się
  z liczbą także we wtrąceniu („(w którym jest pięć monet)”).
- Wołacz rzeczowników na -a („kucharko”, „królowo”, „Kasiu”) i celownik na -u
  („daj kotu”, „psu”, „bratu”).
- Liczba mnoga w poleceniach bez `plural` i `//p`: „weź klucze”, „zbadaj kotki”;
  „oba”, „obaj”, „obydwa”, „obydwie”.
- Przymiotniki męskoosobowe („dzielni rycerze są zamknięci”, „dzielnych
  rycerzy”), automatyczne „ze”/„we”, miękkie spółgłoski i „j” w odmianie
  (kość – kości, zbroja – zbroi), -ia w wyrazach rodzimych (ziemia – ziemi).
- Krótkie ostatnie słowa nazw w poleceniach („zbiór do gry”, „krzesło Ludwika
  XV”); poprawiona odmiana nazw w grach (rój, plan, kierunek, kogut i inne).

Polecenia i parser:

- Bezokoliczniki wszystkich czasowników w „Zrozumiałem tylko tyle, że chcesz…”,
  z dopełniaczem i przyimkiem („dotykać kota”, „dmuchnąć w lampę”).
- „daj mi klucz”, „znajdź klucz”, „szukaj klucza”, „wyjmij wszystko ze skrzyni”,
  „weź klucz i potem upuść go”, „!” i „?” na końcu polecenia, „załóż kotu czapkę”,
  `stan` jak `wynik`.
- Nowe polecenie zamiast odpowiedzi na pytanie gry działa także bez ogonków
  i na Glulxie; „zapytaj kota” i „powiedz kotu” dopytują o treść.
- Zamki: „otwórz drzwi kluczem” otwiera zamek i drzwi, samo „otwórz drzwi”
  używa właściwego klucza, „zamknij drzwi na klucz” wybiera pasujący klucz;
  usunięte „zablokuj”, dodane „zaklucz”.  Opis: suplement, rozdział 16.

Komunikaty i tekst:

- Postacie jako wykonawcy z czasownikiem w liczbie mnogiej („Rybacy wchodzą”);
  gry bez punktów pokazują „W tej grze wykonałaś 18 ruchów.”.
- Polskie cudzysłowy „ ” i półpauza w bibliotece i grach, jedna spacja po kropce.
- Przegląd polszczyzny wszystkich komunikatów i gier: biernik na listach,
  szyk zaimków, bez kalek („chyba”, „haftowanie”); w „Krainie Przygód” opisy
  miejsc i ekwipunek jako polskie zdania.
- Glulx: nazwa pisana wielką literą nie gubi polskich liter; „połóż się”,
  „potem”, „następnie” działają jak w maszynie Z.
- Poprawione błędy programu: „upuść krzesełko”, siedząc na nim; rozkazy dla
  kotków w „Alicji”; zwycięstwo w „Kapitanie Fate” na Glulxie.

Testy i narzędzia:

- `testy/odmiana.inf` i `testy/komunikaty.inf` porównywane co do znaku
  z `testy/*.oczekiwane`; `test.sh` sprawdza też ekrany gier pod kątem źle
  sklejonego tekstu.
- `skompiluj.sh` i hak `.githooks/pre-commit` kompilują gry przed commitem;
  wydania buduje GitHub Actions po wypchnięciu tagu.

Znane ograniczenia: liczebniki męskoosobowe („dwaj rycerze”) i rodzaj żeński
w napisach `list_together` nie są obsługiwane; na Glulxie tekst wpisany przez
gracza w cudzysłowie i nazwy nadane przez gracza tracą polskie litery.

## 6.12.8-pl2 – 28 października 2026

- **Pisanie bez polskich liter.** Słowo spoza słownika, które bez ogonków
  odpowiada słowu ze słownika, jest zamieniane na to słowo: „wez lampe”,
  „otworz drzwi”, „naucz sie rezrov”, „zapytaj kaplana o xibalbe” działają
  tak samo jak wersje z ogonkami.  Przy kilku obiektach pasujących do jednego
  słowa („wschod”) parser porównuje nazwy z pominięciem ogonków.
- Kierunki: usunięte ręcznie dopisane formy bez ogonków („zachod”), przez które
  „poludniowy zachod” nie pasowało do żadnego kierunku.
- „Ruiny”: snop światła rozumie „w świetle” i „smugę”.
- „Muzeum”: `BeforeParsing` kończyło się gołym `return`, przez co biblioteka
  pomijała swoje rozszerzenia (oznaczanie nieznanych słów).
- **Glulx działa.**  W wydaniu pl1 wersje `.ulx` nie rozumiały żadnego
  polecenia: tokenizer biblioteki 6.12.8 nie obsługuje słownika Unicode.
  Polska wersja zastępuje w Glulx `Tokenise__` (słownik Unicode)
  i `KeyboardPrimitive` (wiersz w Unicode, litery spoza Latin-1 zamieniane na
  litery bez ogonków), a `LowerCase` obsługuje polskie litery.
- `przygotuj.sh` buduje też interpreter glulxe; `test.sh` przechodzi każdą grę
  w maszynie Z i w Glulx, skryptem z polskimi literami i bez nich.

## 6.12.8-pl1 – 28 października 2026

Pierwsze wydanie nowej polskiej biblioteki, napisanej od nowa dla biblioteki
standardowej Inform 6.12.8 i kompilatora Inform 6.44. Zastępuje wersję
6/11/PL z lat 2011–2014 ([inform-pl](https://github.com/zajaczkowski/inform-pl)).

Biblioteka:

- automatyczna odmiana nazw obiektów przez sześć przypadków na podstawie
  zapisu z ukośnikami (`"mosiężn/a lamp/a"`), z wymianami w rdzeniu,
  wskazówkami dopełniacza i rodzajem męskoosobowym; ręczne formy `desc_*`
  dla wyjątków,
- rozpoznawanie przez parser każdej formy nazwy, także wpisanej bez polskich
  liter,
- polska gramatyka wszystkich akcji biblioteki 6.12; słowo „się” usuwane
  przed analizą polecenia,
- pytania parsera we właściwym przypadku i z bezokolicznikiem („Do czego
  chcesz włożyć lampę?”, „Komu chcesz dać klucz?”),
- komunikaty biblioteki zgodne z rodzajem i liczbą, także dla gracza
  w rodzaju żeńskim,
- polskie litery w alfabecie maszyny Z (`polskiz.h`),
- zgodność wsteczna z grami z wersji 6/11/PL (`name_*`, `desc_*`,
  `bierniknoun` i pokrewne).

Gry przykładowe – nowe tłumaczenia wszystkich gier Inform 6 z IF Archive, ze
skryptami przejść: „Ruiny”, „Po drugiej stronie lustra”, „Sklep z zabawkami”,
„Przygoda”, „Równowaga”, „Muzeum Informu”, „Kraina Przygód”.

Dokumentacja: podręcznik autora, suplement o budowaniu mechanizmów,
podręcznik gracza.

Narzędzia: `przygotuj.sh` (pobranie i budowa kompilatora, biblioteki
i dfrotz), `build.sh`, `test.sh` oraz testy w GitHub Actions.

Znane ograniczenia:

- w Glulx nie da się wpisywać polskich liter (biblioteka 6.12 czyta tam
  wiersz poleceń w Latin-1), a czasowniki bez ogonków nie są rozpoznawane
  (w rzeczywistości wersje `.ulx` nie rozumiały żadnego polecenia; poprawione
  w pl2),
- komunikaty są tylko w 2. osobie i w czasie teraźniejszym.
