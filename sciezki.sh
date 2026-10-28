# Ustala położenie narzędzi; dołączany (". ./sciezki.sh") przez build.sh i test.sh.
#
# Kolejność szukania:
#   1. zmienne środowiskowe INFORM6, INFORM6LIB, DFROTZ, GLULXE,
#   2. katalog narzedzia/ utworzony przez ./przygotuj.sh,
#   3. katalogi obok projektu (../bin, ../upstream), układ autora,
#   4. polecenia inform6, dfrotz i glulxe w PATH.
#
# Ścieżka do biblioteki musi być krótka (kompilator po cichu pomija
# +include_path dłuższe niż 127 znaków), dlatego domyślne są względne.

pierwszy() {
    for p in "$@"; do
        if [ -e "$p" ]; then echo "$p"; return 0; fi
    done
    return 1
}

: "${INFORM6:=$(pierwszy narzedzia/bin/inform6 ../bin/inform6 || command -v inform6 || true)}"
: "${INFORM6LIB:=$(pierwszy narzedzia/inform6lib ../upstream/inform6lib || true)}"
: "${DFROTZ:=$(pierwszy narzedzia/bin/dfrotz ../bin/dfrotz || command -v dfrotz || true)}"
: "${GLULXE:=$(pierwszy narzedzia/bin/glulxe ../bin/glulxe || command -v glulxe || true)}"

if [ -z "$INFORM6" ] || [ -z "$INFORM6LIB" ]; then
    echo "Nie znaleziono kompilatora Inform 6 albo biblioteki bazowej." >&2
    echo "Uruchom ./przygotuj.sh albo ustaw zmienne INFORM6 i INFORM6LIB." >&2
    exit 1
fi
