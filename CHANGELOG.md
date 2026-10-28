# Historia zmian

Numer wydania składa się z wersji biblioteki standardowej Inform, na której
oparta jest polska wersja, i kolejnego numeru polskiego wydania:
`6.12.8-pl1` to pierwsze wydanie dla biblioteki 6.12.8.

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
- polskie litery w alfabecie maszyny Z (`polskiz.h`), obsługa Glulx,
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
  wiersz poleceń w Latin-1), a czasowniki bez ogonków nie są rozpoznawane,
- komunikaty są tylko w 2. osobie i w czasie teraźniejszym.
