# Historia zmian

Numer wydania składa się z wersji biblioteki standardowej Inform, na której
oparta jest polska wersja, i kolejnego numeru polskiego wydania:
`6.12.8-pl1` to pierwsze wydanie dla biblioteki 6.12.8.

## Niewydane

- Polszczyzna komunikatów i odmiany (przegląd wszystkich komunikatów biblioteki
  dla obiektów każdego rodzaju):
  - przymiotniki w rodzaju męskoosobowym: „rybacy są zamknięci/puści/otwarci”
    zamiast „zamknięte/puste/otwarte” (`PL_PrzymZ`, `PL_PrzymZL`),
  - automatyczne „ze”/„we”: „ze stołu”, „ze skrzyni”, „we wraku” (wcześniej
    tylko przez atrybuty `z_article`/`w_article`),
  - odmiana: miękkie spółgłoski na końcu rdzenia (kość – kości, koń – konia),
    „j” po samogłosce (zbroja – zbroi, pokój – pokoi), -ia w wyrazach rodzimych
    (ziemia – ziemi, ale historia – historii),
  - „Miałeś w rękach: … lampę sodową” (biernik), „Odwiedzone miejsca: …”,
    szyk zaimków („Przecież go nie trzymasz”, „Masz go już na sobie”),
    „Ten interpreter nie pozwala cofać ruchów”, przecinek przed „aby”.
- Komentarze w kodzie biblioteki w całości po polsku; usunięte angielskie
  pozostałości (słowo „the” w `ConTopic`).
- Wydania tworzy automatycznie GitHub Actions po wypchnięciu tagu
  (`wydanie.sh`, `.github/workflows/wydanie.yml`).
- `CONTRIBUTING.md`, szablon zgłoszenia błędu, link do pobrania gier w README,
  rozpoznawanie języka repozytorium (`.gitattributes`).

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
