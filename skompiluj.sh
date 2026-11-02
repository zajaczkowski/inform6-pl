#!/bin/sh
# Kompiluje wszystkie gry i zapisuje je obok źródeł: games/*.z5, games/*.ulx
# oraz games/ibg/*.z5, games/ibg/*.ulx.  Te pliki są w repozytorium, żeby można
# je było pobrać i uruchomić w przeglądarce (zob. README).  Hak .githooks/pre-commit
# wywołuje ten skrypt przed commitem.
set -e
cd "$(dirname "$0")"
. ./sciezki.sh

for zrodlo in games/*_pl.inf games/ibg/*_pl.inf; do
    g=${zrodlo%.inf}
    "$INFORM6" -s "+include_path=lib,extensions,$INFORM6LIB" "$zrodlo" "$g.z5" >/dev/null
    "$INFORM6" -s "+include_path=lib,extensions,$INFORM6LIB" -G '$DICT_CHAR_SIZE=4' \
        "$zrodlo" "$g.ulx" >/dev/null
done
