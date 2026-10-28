#!/bin/sh
# Testy biblioteki: kompiluje wszystkie gry dla maszyny Z i Glulx (każde
# ostrzeżenie kompilatora jest błędem), przechodzi gry ich skryptami
# (games/*.przejscie) i sprawdza wynik końcowy.
# Użycie: ./test.sh            (narzędzia: zob. sciezki.sh)
cd "$(dirname "$0")"
. ./sciezki.sh
if [ -z "$DFROTZ" ]; then
    echo "Nie znaleziono dfrotz.  Uruchom ./przygotuj.sh albo ustaw DFROTZ." >&2
    exit 1
fi

bledy=0
ok()   { printf '  ok    %s\n' "$1"; }
blad() { printf '  BŁĄD  %s\n' "$1"; bledy=$((bledy + 1)); }

kompiluj() {    # kompiluj <plik .inf> <plik wynikowy> [opcje]
    zrodlo=$1; wynik=$2; shift 2
    log=$("$INFORM6" -s "+include_path=lib,$INFORM6LIB" "$@" "$zrodlo" "$wynik" 2>&1)
    if [ $? -ne 0 ] || printf '%s' "$log" | grep -qi 'warning'; then
        printf '%s\n' "$log" | grep -iE 'warning|error' | head -5
        return 1
    fi
}

graj() {        # graj <plik gry> <ziarno> <skrypt>
    "$DFROTZ" -m -Z 0 -s "$2" "$1" < "$3" 2>&1
}

mkdir -p build

echo "Kompilacja:"
for zrodlo in games/*.inf; do
    g=$(basename "$zrodlo" .inf)
    if kompiluj "$zrodlo" "build/$g.z5"; then ok "$g.z5"; else blad "$g.z5"; fi
    if kompiluj "$zrodlo" "build/$g.ulx" -G '$DICT_CHAR_SIZE=4'; then ok "$g.ulx"; else blad "$g.ulx"; fi
done

# "Przygoda" ma losowych krasnoludów i pirata, więc skrypt przechodzi wersję
# testową (TEST_VERSION), w której polecenie xdeterm je wyłącza.
sed 's/^! Constant TEST_VERSION;/Constant TEST_VERSION;/' games/advent_pl.inf > build/advent_test.inf
kompiluj build/advent_test.inf build/advent_test.z5 || blad "advent_test.z5"

echo "Przejścia:"
sprawdz() {     # sprawdz <gra> <plik gry> <ziarno> <oczekiwany tekst>
    if graj "$2" "$3" "games/$1.przejscie" | tr '\n' ' ' | grep -q "$4"; then
        ok "$1: $4"
    else
        blad "$1: brak „$4”"
    fi
}
sprawdz ruins_pl         build/ruins_pl.z5         1 "zdobyłeś 30 punktów na 30"
sprawdz alice_pl         build/alice_pl.z5         1 "Zwyciężyłaś"
sprawdz toyshop_pl       build/toyshop_pl.z5       1 "zdobyłeś 6 punktów na 6"
sprawdz balances_pl      build/balances_pl.z5      1 "zdobyłeś 50 punktów na 51"
sprawdz adventureland_pl build/adventureland_pl.z5 1 "zdobyłeś 100 punktów na 100"
sprawdz advent_pl        build/advent_test.z5      3 "zdobyłeś 350 punktów na 350"

# "Muzeum" nie ma zakończenia: skrypt to wycieczka, w której parser ma
# zrozumieć wszystko poza jednym celowo błędnym poleceniem
# ("weź pomidor smażony czerwony" - pomidor wymaga przymiotników przed nazwą).
n=$(graj build/museum_pl.z5 1 games/museum_pl.przejscie |
    grep -cE 'Nie znam|Nie widzisz niczego|Nie zrozumiałem|Zrozumiałem tylko')
if [ "$n" -eq 1 ]; then ok "museum_pl: wycieczka bez błędów parsera"
else blad "museum_pl: $n błędów parsera (oczekiwano 1)"; fi

echo
if [ "$bledy" -eq 0 ]; then echo "Wszystkie testy przeszły."; else echo "Błędów: $bledy"; fi
[ "$bledy" -eq 0 ]
