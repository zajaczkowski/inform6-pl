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
kompiluj build/advent_test.inf build/advent_test.ulx -G '$DICT_CHAR_SIZE=4' || blad "advent_test.ulx"

# Każdą grę przechodzimy dwa razy: skryptem z polskimi literami i tym samym
# skryptem bez ogonków (build/*.ascii) - gracz może pisać "wez lampe".
bez_ogonkow() {
    sed 's/ą/a/g; s/ć/c/g; s/ę/e/g; s/ł/l/g; s/ń/n/g; s/ó/o/g; s/ś/s/g; s/ź/z/g; s/ż/z/g;
         s/Ą/A/g; s/Ć/C/g; s/Ę/E/g; s/Ł/L/g; s/Ń/N/g; s/Ó/O/g; s/Ś/S/g; s/Ź/Z/g; s/Ż/Z/g' "$1"
}

echo "Przejścia:"
# Glulx (jeśli jest glulxe): oba skrypty, z polskimi literami i bez nich.
graj_glulx() {  # graj_glulx <plik gry> <ziarno> <skrypt>
    "$GLULXE" -q -u --rngseed "$2" "$1" < "$3" 2>&1
}

sprawdz() {     # sprawdz <gra> <plik gry> <ziarno> <oczekiwany tekst>
    bez_ogonkow "games/$1.przejscie" > "build/$1.ascii"
    for skrypt in "games/$1.przejscie" "build/$1.ascii"; do
        opis=$1; [ "$skrypt" = "build/$1.ascii" ] && opis="$1 (bez ogonków)"
        if graj "$2" "$3" "$skrypt" | tr '\n' ' ' | grep -q "$4"; then
            ok "$opis: $4"
        else
            blad "$opis: brak „$4”"
        fi
    done
    if [ -n "$GLULXE" ]; then
        ulx=$(echo "$2" | sed 's/\.z5$/.ulx/')
        for skrypt in "games/$1.przejscie" "build/$1.ascii"; do
            opis="$1 (Glulx)"; [ "$skrypt" = "build/$1.ascii" ] && opis="$1 (Glulx, bez ogonków)"
            if graj_glulx "$ulx" "$3" "$skrypt" | tr '\n' ' ' | grep -q "$4"; then
                ok "$opis: $4"
            else
                blad "$opis: brak „$4”"
            fi
        done
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
bez_ogonkow games/museum_pl.przejscie > build/museum_pl.ascii
for skrypt in games/museum_pl.przejscie build/museum_pl.ascii; do
    opis=museum_pl; [ "$skrypt" = build/museum_pl.ascii ] && opis="museum_pl (bez ogonków)"
    n=$(graj build/museum_pl.z5 1 "$skrypt" |
        grep -cE 'Nie znam|Nie widzisz niczego|Nie zrozumiałem|Zrozumiałem tylko')
    if [ "$n" -eq 1 ]; then ok "$opis: wycieczka bez błędów parsera"
    else blad "$opis: $n błędów parsera (oczekiwano 1)"; fi
done
if [ -n "$GLULXE" ]; then
    # Wyjście z Sali Zmysłów prowadzi losowo; generator glulxe daje przy
    # ziarnie 1 inną drogę niż dfrotz, przy ziarnie 5 tę samą co skrypt.
    for skrypt in games/museum_pl.przejscie build/museum_pl.ascii; do
        opis="museum_pl (Glulx)"; [ "$skrypt" = build/museum_pl.ascii ] && opis="museum_pl (Glulx, bez ogonków)"
        n=$(graj_glulx build/museum_pl.ulx 5 "$skrypt" |
            grep -cE 'Nie znam|Nie widzisz niczego|Nie zrozumiałem|Zrozumiałem tylko')
        if [ "$n" -eq 1 ]; then ok "$opis: wycieczka bez błędów parsera"
        else blad "$opis: $n błędów parsera (oczekiwano 1)"; fi
    done
else
    echo "  (brak glulxe - przejścia Glulx pominięte; zob. ./przygotuj.sh)"
fi

echo
if [ "$bledy" -eq 0 ]; then echo "Wszystkie testy przeszły."; else echo "Błędów: $bledy"; fi
[ "$bledy" -eq 0 ]
