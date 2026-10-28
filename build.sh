#!/bin/sh
# Kompiluje grę z katalogu games/ z polską biblioteką.
# Użycie: ./build.sh test          -> build/test.z5
#         ./build.sh test -G       -> build/test.ulx (Glulx)
#         ./build.sh test -D       -> wersja z poleceniami debugowania
# Narzędzia: zob. sciezki.sh (najprościej najpierw ./przygotuj.sh).
set -e
cd "$(dirname "$0")"
. ./sciezki.sh

game=${1:?Podaj nazwę gry z katalogu games/, np. test}
shift
ext=z5
case " $* " in *" -G "*) ext=ulx; set -- "\$DICT_CHAR_SIZE=4" "$@" ;; esac
mkdir -p build
"$INFORM6" -s "+include_path=lib,$INFORM6LIB" "$@" "games/$game.inf" "build/$game.$ext"
