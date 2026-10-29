# Jak pomóc

Zgłoszenia błędów, poprawki polszczyzny i nowe tłumaczenia gier są mile widziane.

## Zgłaszanie błędów

Błąd najłatwiej opisać w zakładce [Issues](https://github.com/zajaczkowski/inform6-pl/issues), wybierając szablon „Błąd”. Najbardziej pomaga dokładny zapis z gry: polecenia, które wpisano, i odpowiedź gry, a do tego nazwa gry, wersja biblioteki (pokazuje ją polecenie `wersja`) i informacja, czy to maszyna Z (`.z5`), czy Glulx (`.ulx`).

Sztywne lub niepoprawne sformułowania w komunikatach to też błędy – warto je zgłaszać razem z propozycją lepszego brzmienia.

## Zmiany w kodzie

1. Przygotuj narzędzia (raz): `./przygotuj.sh`.
2. Wprowadź zmianę. Komunikaty biblioteki są w `lib/polski.h`, gramatyka w `lib/polskig.h`, odmiana nazw w `lib/odmiana.h`.
3. Uruchom testy: `./test.sh`. Skrypt kompiluje wszystkie gry dla maszyny Z i Glulx (każde ostrzeżenie kompilatora jest błędem) i przechodzi każdą grę do końca – z polskimi literami i bez nich. Zmiana powinna zostawić wszystkie testy zielone.
4. Jeśli poprawka dotyczy zachowania widocznego w grze, dopisz polecenie, które je sprawdza, do skryptu przejścia odpowiedniej gry (`games/*.przejscie`).
5. Opisz zmianę w sekcji „Niewydane” na początku `CHANGELOG.md`.

Te same testy uruchamia GitHub Actions przy każdym pushu i pull requeście.

## Dodatki

Funkcje, których nie ma w bibliotece standardowej Inform 6 (np. zamki szyfrowe), trafiają do katalogu `extensions/`, a nie do `lib/`. Dodatek to plik `.h` dołączany po `PolskiG`, z instrukcją `<nazwa>_instrukcja.md` i grą przykładową `<nazwa>_przyklad.inf` ze skryptem `<nazwa>_przyklad.przejscie`, który kończy ją zwycięstwem – `test.sh` sprawdza ją automatycznie. Komunikaty dodatek dodaje przez `LibraryExtensions` (`ext_messages`, `ext_printverb`), bez zmian w `lib/`.

## Styl

- Kod biblioteki naśladuje styl biblioteki Inform 6.12: wcięcia czterema spacjami, komentarze po polsku.
- Teksty dla gracza: druga osoba, czas teraźniejszy, naturalna polszczyzna; formy zależne od rodzaju i liczby przez `KoncY`, `Forma`, `PL`, `Gracz` (zob. [podręcznik](docs/podrecznik.md), rozdział 6).
- Pliki są w UTF-8.

## Wydania

Wydanie powstaje automatycznie: po wpisaniu wersji w `CHANGELOG.md` (nagłówek `## 6.12.8-plN – data`) i w `LanguageVersion` w `lib/polski.h` wystarczy utworzyć i wypchnąć tag, np. `git tag -a 6.12.8-pl3 -m "inform6-pl 6.12.8-pl3"` i `git push origin 6.12.8-pl3`. GitHub Actions przetestuje całość, złoży paczkę skryptem `wydanie.sh` i opublikuje wydanie z grami.
