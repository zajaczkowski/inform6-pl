#!/bin/sh
# Składa pliki wydania w katalogu dist/:
#   - inform6-pl-<wersja>.zip: repozytorium w stanie z tagu, ze skompilowanymi
#     grami obok źródeł w games/,
#   - te same gry osobno (*.z5, *.ulx),
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

nazwa=inform6-pl-$wersja
mkdir -p dist
rm -f dist/*

# Skompilowane gry są w repozytorium obok źródeł (games/, games/ibg/; pilnuje
# tego hak .githooks/pre-commit), więc paczka zawiera je raz, a do wydania
# trafiają te same pliki.
git archive --format=zip --prefix="$nazwa/" -o "dist/$nazwa.zip" "$wersja"
cp games/*_pl.z5 games/*_pl.ulx games/ibg/*_pl.z5 games/ibg/*_pl.ulx dist/

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
