#!/bin/sh
# Pobiera i buduje narzędzia potrzebne do kompilacji i testów, w wersjach,
# z którymi biblioteka jest sprawdzana:
#   - biblioteka standardowa Inform 6.12.8  -> narzedzia/inform6lib
#   - kompilator Inform 6.44                -> narzedzia/bin/inform6
#   - interpreter dfrotz (Frotz 2.55)       -> narzedzia/bin/dfrotz
# Wymaga git, kompilatora C i make.  Katalog narzedzia/ nie trafia do repozytorium.
set -e
cd "$(dirname "$0")"

LIB_TAG=6.12.8
INFORM6_TAG=v6.44
FROTZ_TAG=2.55

mkdir -p narzedzia/bin narzedzia/src

pobierz() {     # pobierz <adres> <tag> <katalog>
    if [ ! -d "$3" ]; then
        git -c advice.detachedHead=false clone --quiet --depth 1 --branch "$2" "$1" "$3"
    fi
}

echo "Biblioteka Inform $LIB_TAG..."
pobierz https://gitlab.com/DavidGriffith/inform6lib.git "$LIB_TAG" narzedzia/inform6lib

echo "Kompilator Inform $INFORM6_TAG..."
pobierz https://github.com/DavidKinder/Inform6.git "$INFORM6_TAG" narzedzia/src/inform6
cc -O2 -o narzedzia/bin/inform6 narzedzia/src/inform6/*.c

echo "Interpreter dfrotz $FROTZ_TAG..."
pobierz https://gitlab.com/DavidGriffith/frotz.git "$FROTZ_TAG" narzedzia/src/frotz
( cd narzedzia/src/frotz && make -s dumb CURSES_CONFIG=true HOMEBREW_PREFIX=/usr >/dev/null )
cp narzedzia/src/frotz/dfrotz narzedzia/bin/dfrotz

echo "Gotowe.  Teraz: ./build.sh ruins_pl   albo   ./test.sh"
