#!/bin/sh
# Składa pliki wydania w katalogu dist/:
#   - inform6-pl-<wersja>.zip: repozytorium w stanie z tagu oraz skompilowane
#     gry w katalogu skompilowane/,
#   - gry osobno (*.z5, *.ulx),
#   - opis wydania (opis.md) z sekcji CHANGELOG.md dla tej wersji.
# Użycie: ./wydanie.sh 6.12.8-pl3   (najpierw git tag; drzewo bez zmian)
# Uruchamiany przez .github/workflows/wydanie.yml po wypchnięciu tagu.
set -e
cd "$(dirname "$0")"
. ./sciezki.sh

wersja=${1:?Podaj tag wydania, np. 6.12.8-pl3}
if [ "$(git rev-parse -q --verify "refs/tags/$wersja^{commit}")" != "$(git rev-parse HEAD)" ]; then
    echo "Tag $wersja nie wskazuje bieżącego commita." >&2; exit 1
fi
if ! git diff --quiet HEAD; then
    echo "W drzewie są niezacommitowane zmiany; wydanie musi odpowiadać tagowi." >&2; exit 1
fi

gry="ruins_pl alice_pl toyshop_pl advent_pl balances_pl museum_pl adventureland_pl"
nazwa=inform6-pl-$wersja
katalog=$(pwd)
tmp=$(mktemp -d)
mkdir -p dist
rm -f dist/*

git archive --format=tar --prefix="$nazwa/" "$wersja" | tar xf - -C "$tmp"
mkdir -p "$tmp/$nazwa/skompilowane"
for g in $gry; do
    ./build.sh "$g" >/dev/null
    ./build.sh "$g" -G >/dev/null
    cp "build/$g.z5" "build/$g.ulx" "$tmp/$nazwa/skompilowane/"
    cp "build/$g.z5" "build/$g.ulx" dist/
done

cat > "$tmp/$nazwa/skompilowane/CZYTAJ.txt" <<EOF
Skompilowane gry przykładowe (inform6-pl $wersja)

Pliki .z5 to gry dla maszyny Z, a .ulx - dla Glulx.  Obie wersje działają
tak samo.  Otwiera się je w interpreterze, np. Frotz, Gargoyle, Lectrote
albo Parchment (w przeglądarce).  Polecenia można pisać z polskimi literami
albo bez nich ("weź lampę" i "wez lampe").

  ruins_pl          Ruiny
  alice_pl          Po drugiej stronie lustra
  toyshop_pl        Sklep z zabawkami
  advent_pl         Przygoda
  balances_pl       Równowaga
  museum_pl         Muzeum Informu
  adventureland_pl  Kraina Przygód

Jak grać: docs/podrecznik_gracza.md.

Compiled Polish example games for the Z-machine (.z5) and Glulx (.ulx).
Commands may be typed with or without Polish diacritics.
EOF

( cd "$tmp" && zip -qrX "$katalog/dist/$nazwa.zip" "$nazwa" )
rm -rf "${tmp:?}"

# Opis wydania: sekcja CHANGELOG.md od "## <wersja>" do następnego "## ".
{
    awk -v w="## $wersja" 'index($0, w) == 1 { on = 1; next } /^## / { on = 0 } on' CHANGELOG.md
    cat <<EOF

---

**Do grania:** pobierz pliki \`.z5\` albo \`.ulx\` i otwórz je w interpreterze (Frotz, Gargoyle, Lectrote, Parchment). Polecenia można pisać z polskimi literami albo bez nich.

**Do pisania gier:** pobierz \`$nazwa.zip\` albo sklonuj repozytorium i uruchom \`./przygotuj.sh\`; zob. [podręcznik](https://github.com/zajaczkowski/inform6-pl/blob/main/docs/podrecznik.md).

*English:* Polish translation of the Inform 6 Standard Library 6.12.8 (Inform 6.44). Play the \`.z5\` or \`.ulx\` files in any interpreter.
EOF
} > dist/opis.md

echo "Gotowe: dist/$nazwa.zip, $(ls dist/*.z5 dist/*.ulx | wc -l | tr -d ' ') gier, dist/opis.md"
