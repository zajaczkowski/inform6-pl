# Suplement: jak zbudować różne mechanizmy

### Na przykładach z gier przykładowych Inform 6

Ten suplement uzupełnia [podręcznik](podrecznik.md). Podręcznik prowadzi przez całe „Ruiny”, od pierwszego miejsca po zakończenie, ale jedna mała gra nie może pokazać wszystkiego, co da się zbudować w Inform. Tutaj zebrano mechanizmy z pozostałych gier przykładowych i ułożono je według tematów, a nie według gier. Każdy rozdział omawia jedną technikę na jednym lub kilku przykładach i wskazuje, w której grze można ją znaleźć.

| Plik | Gra | Czego warto w niej szukać |
|---|---|---|
| `games/toyshop_pl.inf` | „Sklep z zabawkami” | zbiór nietypowych obiektów: worek gracza, demony i zegary, identyczne przedmioty, para rękawiczek, postać wykonująca polecenia |
| `games/alice_pl.inf` | „Po drugiej stronie lustra” | samouczek: kotki z demonami i stanem, własne `parse_name`, nowa akcja z dwoma dopełnieniami |
| `games/advent_pl.inf` | „Przygoda” | duża gra: skarby i punktacja, postacie chodzące za graczem, magiczne słowa, wskrzeszanie |
| `games/balances_pl.inf` | „Równowaga” | programowanie parsera: zaklęcia jako czasowniki, przedmioty nazywane przez gracza, tworzenie obiektów w czasie gry, pytania |
| `games/museum_pl.inf` | „Muzeum Informu” | rozwiązane ćwiczenia z podręcznika projektanta: zmiana postaci gracza, rozkazy do maszyn, własne tokeny (godzina, liczby), linia statusu |
| `games/adventureland_pl.inf` | „Kraina Przygód” | gra w stylu Scotta Adamsa: własny wygląd opisów i ekwipunku, lampa z ograniczonym zapasem, skarby liczone w jednym miejscu |

Każda gra ma skrypt przejścia `games/*.przejscie`, który pozwala sprawdzić ją po każdej zmianie:

```bash
./build.sh balances_pl
narzedzia/bin/dfrotz -m -Z 0 -s 1 build/balances_pl.z5 < games/balances_pl.przejscie
```

„Muzeum” nie ma zakończenia, więc jego skrypt jest wycieczką po wszystkich eksponatach. W „Krainie Przygód” i w „Równowadze” zdarzają się rzeczy losowe, dlatego skrypty sprawdzono z ustalonym ziarnem losowania (`-s 1`); przy innym ziarnie przebieg może się rozminąć ze skryptem.

---

## Spis treści

1. [Nazwy drukowane przez procedury](#1-nazwy-drukowane-przez-procedury)
2. [Pojemniki i worek gracza](#2-pojemniki-i-worek-gracza)
3. [Upływ czasu: demony, zegary i each_turn](#3-upływ-czasu-demony-zegary-i-each_turn)
4. [Światło, ciemność i lampy](#4-światło-ciemność-i-lampy)
5. [Nazwy i opisy zmieniające się w czasie gry](#5-nazwy-i-opisy-zmieniające-się-w-czasie-gry)
6. [Wiele identycznych przedmiotów](#6-wiele-identycznych-przedmiotów)
7. [Własne rozpoznawanie słów: parse_name](#7-własne-rozpoznawanie-słów-parse_name)
8. [Przerabianie polecenia przed analizą](#8-przerabianie-polecenia-przed-analizą)
9. [Tekst, liczby i godziny wpisane przez gracza](#9-tekst-liczby-i-godziny-wpisane-przez-gracza)
10. [Postacie: rozmowa, rozkazy, wędrówki](#10-postacie-rozmowa-rozkazy-wędrówki)
11. [Przedmioty, które słuchają poleceń](#11-przedmioty-które-słuchają-poleceń)
12. [Zmiana postaci gracza](#12-zmiana-postaci-gracza)
13. [Zasięg: co gracz może nazwać](#13-zasięg-co-gracz-może-nazwać)
14. [Zaklęcia, magiczne słowa i czasowniki tworzone w czasie gry](#14-zaklęcia-magiczne-słowa-i-czasowniki-tworzone-w-czasie-gry)
15. [Nowe czasowniki i podmiana akcji biblioteki](#15-nowe-czasowniki-i-podmiana-akcji-biblioteki)
16. [Drzwi, wyjścia i pojazdy](#16-drzwi-wyjścia-i-pojazdy)
17. [Własny wygląd gry](#17-własny-wygląd-gry)
18. [Punktacja, śmierć i zakończenia](#18-punktacja-śmierć-i-zakończenia)
19. [Pułapki przy przenoszeniu gier na polski](#19-pułapki-przy-przenoszeniu-gier-na-polski)

---

## 1. Nazwy drukowane przez procedury

Większość obiektów ma nazwę w notacji odmiany (`"cedrow/a szkatułk/a"`), a końcówki dobiera biblioteka. Niektóre nazwy powstają jednak dopiero w czasie gry: koń dostaje skrzydła, butelka bywa pusta albo pełna pszczół, a los na loterię ma numer. Takie obiekty mają **`short_name` w postaci procedury**, która musi sama wypisać nazwę we właściwym przypadku.

Przed wywołaniem `short_name` biblioteka ustawia zmienną `short_name_case` (`PRZYP_MIAN`, `PRZYP_DOP` … `PRZYP_MIE`). Na listach przedmiotów („Widzisz tutaj …”) obowiązuje jednak inny mechanizm: `short_name_case` wskazuje mianownik, a przypadek listy zapamiętuje `ListaWPrzypadku`, i to tylko dla przedmiotów leżących bezpośrednio w wypisywanym miejscu lub pojemniku. Żeby nie sprawdzać obu rzeczy w każdej procedurze, „Równowaga”, „Muzeum” i „Kraina Przygód” mają dwie małe procedury pomocnicze:

```inform
[ Przyp o;
    if (short_name_case == PRZYP_MIAN && pl_lista_rodzic && parent(o) == pl_lista_rodzic)
        return pl_lista_przyp;
    return short_name_case;
];

[ PoPrzyp o s0 s1 s2 s3 s4 s5;
    switch (Przyp(o)) {
      PRZYP_MIAN: print (string) s0;
      PRZYP_DOP:  print (string) s1;
      PRZYP_CEL:  print (string) s2;
      PRZYP_BIE:  print (string) s3;
      PRZYP_NAR:  print (string) s4;
      PRZYP_MIE:  print (string) s5;
    }
];
```

`Przyp` zwraca przypadek, w którym trzeba w tej chwili wydrukować nazwę obiektu `o`. `PoPrzyp` drukuje tę z sześciu podanych form, która odpowiada temu przypadkowi. Koń z „Równowagi” wygląda tak:

```inform
       short_name
       [;  if (self has general)
               PoPrzyp(self, "skrzydlaty koń", "skrzydlatego konia",
                   "skrzydlatemu koniowi", "skrzydlatego konia",
                   "skrzydlatym koniem", "skrzydlatym koniu");
           else
               PoPrzyp(self, "kasztanowy koń", "kasztanowego konia",
                   "kasztanowemu koniowi", "kasztanowego konia",
                   "kasztanowym koniem", "kasztanowym koniu");
           rtrue;
       ],
```

Procedura kończy się `rtrue`, co mówi bibliotece: „nazwa już wypisana, nie drukuj jej”. Często wystarczy odmienić tylko stałą część nazwy, a resztę dopisać bez zmian. Tak robi los na loterię, bo numer się nie odmienia:

```inform
           PoPrzyp(self, "los", "losu", "losowi", "los", "losem", "losie");
           print " numer ", self.number; rtrue;
```

Jeśli nazwa ma tylko kilka stanów, prościej niż pisać procedurę jest **podmieniać napis w notacji odmiany**. Zapałka ze „Sklepu z zabawkami” ma `short_name "niezapalon/a zapałk/a"`, a po zapaleniu gra przypisuje jej `self.short_name = "płonąc/a zapałk/a"`. Biblioteka odmienia nową nazwę i rozpoznaje jej formy bez żadnego dodatkowego kodu.

Obiekt z procedurą `short_name` nie ma nazwy w notacji odmiany, więc biblioteka nie wie, jakie formy rozpoznawać. Słowa, których użyje gracz, trzeba wtedy podać w `name` (`'koń' 'konia' 'koniowi' 'koniem' 'koniu' …`).

---

## 2. Pojemniki i worek gracza

### Worek gracza

Gdy gracz ma pełne ręce i bierze kolejną rzecz, biblioteka sama przekłada coś z rąk do worka wskazanego stałą `SACK_OBJECT`. Oszczędza to graczowi ciągłego żonglowania ekwipunkiem. Tak działają tornister ze „Sklepu z zabawkami” i torba na próbki z „Muzeum”:

```inform
Constant MAX_CARRIED = 4;
Constant SACK_OBJECT = satchel;
```

```
>weź świece
woskowa świeca: (wkładasz woskową świecę do tornistra, żeby zrobić miejsce)
Wzięte.
```

Obie stałe muszą stać przed `Include "VerbLib"`, bo biblioteka odczytuje je w czasie kompilacji. Tornister ma ponadto właściwości `when_open` i `when_closed`: to dwa zdania, którymi przedmiot przedstawia się w opisie miejsca, zależnie od tego, czy jest otwarty.

### Pojemność i przezroczystość

```inform
Object -> "zwykł/a torb/a na zakupy (mieści tylko 2 rzeczy)"
  with name 'zwykła' 'zwykłą' 'zwykłej' 'torba' 'torbę' 'torby' 'zakupy',
       capacity 2,
  has  container open female;
```

`capacity` ogranicza liczbę przedmiotów, które zmieszczą się w pojemniku. Atrybut **`transparent`** sprawia, że przez zamknięty pojemnik widać zawartość i przechodzi światło. W „Sklepie” i w „Muzeum” stoją obok siebie dwa pudełka, które różnią się tylko tym atrybutem – dobry przykład do sprawdzenia, jak działa:

```inform
Object -> "szklan/e pudełk/o z wieczkiem"
  with name 'wieczko' 'wieczkiem'
  has  container transparent openable open neuter;
Object -> "stalow/e pudełk/o z wieczkiem"
  with name 'wieczko' 'wieczkiem'
  has  container openable open neuter;
```

### Pojemnik z charakterem

Pojemnik może reagować na akcje, w których jest dopełnieniem drugim. Gdy gracz coś do niego wkłada, pojemnik dostaje akcję `Receive`; gdy coś wyjmuje – `LetGo`. Zębata torba („Sklep”, „Muzeum”) korzysta z tego, żeby nie oddawać tego, co połknęła:

```inform
       before
       [; LetGo: "Torba z uporem zatrzaskuje zęby na twojej dłoni, dopóki
                  nie zrezygnujesz.";
          Close: "Torba opiera się wszelkim próbom zamknięcia.";
       ],
       after
       [; Receive:
                 "Torba wije się ohydnie, połykając ", (bie) noun, ".";
       ],
```

W „Sklepie” torba po połknięciu marcepanowego stożka **kasuje własne reguły** (`self.before = 0; self.after = 0`) i od tej chwili zachowuje się jak zwykły pojemnik. To najprostszy sposób na jednorazową zmianę zachowania obiektu.

Butelka z „Krainy Przygód” przyjmuje tylko wodę i pszczoły, a wkładanie zamienia na napełnianie, żeby „włóż wodę do butelki” i „napełnij butelkę” działały tak samo:

```inform
         Receive:
             if (noun==water or bees) <<Fill self>>;
             "To się nie zmieści do butelki.";
```

### Zawartość widoczna, ale niedostępna

Torba z makramy w „Muzeum” jest przezroczysta: widać, co w niej jest, ale nie da się tego wziąć. Reguła **`react_before`** przechwytuje każdą akcję na przedmiocie w środku z wyjątkiem oglądania, słuchania i wąchania:

```inform
       react_before
       [;  Examine, Search, Listen, Smell: ;
           default:
               if (inp1>1 && inp1 in self)
                   print_ret (CMian) inp1, " ", (nop) PL(inp1, "jest", "są"),
                       " schowan", (nop) Forma(inp1, "y", "a", "e", "e", "i"),
                       " w torbie.";
```

Regułę `react_before` biblioteka wywołuje dla wszystkich przedmiotów w pobliżu gracza, a nie tylko dla tych, których dotyczy akcja – dlatego torba może pilnować swojej zawartości. `inp1` i `inp2` to pierwsze i drugie dopełnienie polecenia; wartość 1 oznacza liczbę, a nie obiekt, stąd warunek `inp1>1`. `PL` i `Forma` dobierają końcówki do rodzaju i liczby: *pozytywka jest schowana*, *klocki są schowane*.

---

## 3. Upływ czasu: demony, zegary i each_turn

Biblioteka zna trzy sposoby na to, żeby coś działo się bez udziału gracza:

| Mechanizm | Kiedy działa | Jak włączyć |
|---|---|---|
| `daemon` | co turę, wszędzie | `StartDaemon(obj)`, `StopDaemon(obj)` |
| `time_out` (zegar) | raz, po zadanej liczbie tur | `StartTimer(obj, n)`, `StopTimer(obj)`; obiekt musi mieć `time_left` |
| `each_turn` | co turę, gdy obiekt jest w pobliżu gracza | zawsze włączone |

### Demon: przedmiot, który porusza się sam

Balonik w „Sklepie” dryfuje między częściami sklepu. Demon działa wszędzie, więc sam musi sprawdzić, czy gracz to widzi, i tylko wtedy wypisać komunikat:

```inform
        daemon [ from_room to_room;
            if (random(3) ~= 1) rfalse;
            from_room = parent(self);
            if (from_room == East_End or West_End) to_room = Toyshop;
            if (from_room == Toyshop) {
                if (random(2) == 1) to_room = East_End;
                else to_room = West_End;
            }
            if (to_room == 0) rfalse;
            move self to to_room;
            if (location == from_room)
                "^Podmuch wiatru unosi balonik ", (DokadWSklepie) to_room, ".";
            if (location == to_room)
                "^Podmuch wiatru przywiewa balonik ", (SkadWSklepie) from_room, ".";
        ];
```

Nazwy miejsca nie da się po polsku po prostu wstawić w zdanie: wymaga ona przyimka i odmiany, a nazwa pisana wielką literą wyglądałaby w środku zdania dziwnie („unosi balonik Wschodni koniec”). Dlatego gra ma reguły wydruku `DokadWSklepie` i `SkadWSklepie`, które dla każdego miejsca drukują gotowe wyrażenie: „na wschodni koniec sklepu”, „ze wschodniego końca sklepu”.

### Demon ze stanem: kotki

W „Alicji” każdy kotek ma właściwość `state` – bawi się królową, kłębkiem, kręci się przy fotelu albo siedzi na rękach – i demona, który co drugą turę wybiera losowe zachowanie pasujące do tego stanu:

```inform
        daemon [ i;
            give self ~general;
            self.number = 1 - self.number;
            if (self.number == 1 || random(3) == 2) rtrue;
            new_line;
            print (CMian) self;
            switch (self.state) {
             0: switch (random(5)) {
                 1: " miauczy żałośnie.";
```

`self.number` przełącza się co turę między 0 a 1, a kotki zaczynają z różnymi wartościami (`number 0` i `number 1`). Dzięki temu nigdy nie odzywają się oba w tej samej turze i tekst się nie piętrzy. Flaga `general` pilnuje, żeby wspólny opis zabawy obu kotków nie pojawił się dwa razy.

### Zegar: granat i zaklęcie na pięć tur

Granat w „Sklepie” wybucha pięć tur po wyciągnięciu zawleczki. Zegar uruchamia `StartTimer`, a po upływie czasu biblioteka wywołuje `time_out`:

```inform
        before [;
            Pull: if (self has general) "Na to już za późno.";
                  StartTimer(self, 5); give self general;
                  move the_pin to player;
                  "Wyciągasz zawleczkę. Tego nie da się już cofnąć.";
        ],
        time_left 0,
        time_out [;
            deadflag = 1;
```

W „Równowadze” zegar odlicza działanie zaklęcia lobal (wyostrzony słuch) i cierpliwość cyklopa. Cyklop łączy dwa mechanizmy: `each_turn` co turę wypisuje groźby, gdy gracz jest obok, a `time_out` kończy sprawę atakiem:

```inform
       time_left 0,
       time_out
       [;  if (self notin location)
           {   remove self; rtrue;
           }
           deadflag = true; remove mace; remove cyclops;
```

### Pora dnia

„Muzeum” ustawia zegar gry w `Initialise`: `SetTime(SUNRISE-10,1)` oznacza dziesięć minut przed wschodem słońca i jedną minutę na turę. Balkon w `each_turn` sprawdza `the_time` i ogłasza wschód oraz zachód słońca:

```inform
       each_turn
       [ f;
           if (the_time >= SUNRISE && the_time < SUNSET) f=1;
           if (day_state == f) return;
           if (day_state==2) { day_state = f; return; }
           day_state = f;
           if (f==1) "^Wschodzi słońce i oświetla krajobraz!";
         "^Słońce zachodzi i krajobraz pogrąża się w ciemności.";
       ],
```

Wartość 2 oznacza „jeszcze nie wiadomo”: przy pierwszym wejściu na balkon gra tylko zapamiętuje porę dnia i niczego nie ogłasza, bo gracz nie widział wcześniejszego stanu.

### Zmęczenie dźwiganiem

Sala Ciężarów w „Muzeum” liczy łączny ciężar niesionych rzeczy. Służy do tego właściwość `weight` z wartością domyślną 10, którą deklaracja `Property weight 10;` nadaje wszystkim obiektom. Demon co turę odejmuje graczowi sił, a gdy się wyczerpią, zmusza go do upuszczenia najcięższej rzeczy:

```inform
            if (s==0)
            {   bw=-1;
                objectloop(b in player)
                    if (WeightOf(b) > bw) { bw = WeightOf(b); w=b; }
                self.players_strength = self.players_strength + bw;
                print "^Wyczerpan", (nop) Gracz("y", "a"), " dźwiganiem
                    takiego ciężaru, postanawiasz porzucić ", (bie) w, ": ";
                <<Drop w>>;
            }
```

Demona włącza `initial` sali (`initial [; weight_monitor.activate(); ]`), wywoływane przy każdym wejściu gracza. Gdy gracz wyjdzie z sali, demon sam się wyłącza.

---

## 4. Światło, ciemność i lampy

Miejsce jest oświetlone, gdy ma atrybut `light` albo gdy znajduje się w nim coś świecącego (z atrybutem `light`) i nic tego nie zasłania. Większość gier nadaje `light` wszystkim pomieszczeniom przez wspólną klasę (`Class Place has light;` w „Równowadze”), a ciemnym miejscom go odbiera (`has ~light`).

### Lampa z ograniczonym zapasem

Lampa w „Krainie Przygód” ma właściwość `power` i demona, który ją wyczerpuje. Demon zaczyna działać po zapaleniu lampy, a gdy zastanie ją zgaszoną, sam się wyłącza. Nie trzeba więc pamiętać o `StopDaemon` w każdym miejscu, w którym lampa może zgasnąć:

```inform
       daemon
       [ t; if (self hasnt on) { StopDaemon(self); rtrue; }
          t=--(self.power);
          if (t==0) give self ~on ~light;
          if (self in player or location)
          {   if (t==0) "Lampa się wyczerpała.";
              if (t==30) "Lampa zaczyna przygasać.";
          }
       ],
```

```inform
       after
       [; SwitchOn: give self light; StartDaemon(self);
              "Bezpłomienna lampa świeci.";
          SwitchOff: give self ~light;
       ],
```

Polecenia „zapal lampę” i „zgaś lampę” działają bez dodatkowej gramatyki: „zapal” włącza przedmioty z atrybutem `switchable`, a pozostałe podpala.

### Zaklęcie, które daje światło

W „Równowadze” zaklęcie frotz sprawia, że dowolny przedmiot zaczyna świecić; wystarczy do tego `give second light`. Zaklęcie odwrócone odbiera ten atrybut. Księga zaklęć, na którą gracz rzuci frotz, służy mu potem za latarkę do końca gry.

### Ruch w ciemności

`DarkToDark` to punkt wejścia wywoływany, gdy gracz przechodzi z jednego ciemnego miejsca do drugiego. „Przygoda” i „Kraina Przygód” za pierwszym razem tylko ostrzegają, a potem przy każdym przejściu gracz z prawdopodobieństwem jeden do czterech wpada do jamy:

```inform
[ DarkToDark;
  if (dark_warning==0)
  {   dark_warning=1; "Jest teraz zupełnie ciemno.  Jeśli pójdziesz dalej,
                       pewnie wpadniesz do jamy.";
  }
  if (random(4) == 1)
  {   deadflag=1;
```

Ciemność jest w bibliotece osobnym obiektem `thedark`. Można zmienić jego nazwę i opis (`thedark.short_name = "Dziwnie pachnąca ciemność"` w „Muzeum”) albo nadać mu `initial`, wywoływane przy każdym wejściu w ciemność. W „Muzeum” właśnie wtedy ćma ucieka graczowi:

```inform
  thedark.short_name = "Dziwnie pachnąca ciemność";
  thedark.initial = GoMothGo;
```

Uwaga: w ciemności zmienna `location` wskazuje `thedark`, a nie prawdziwe miejsce. Prawdziwe miejsce przechowuje `real_location`. Magiczne słowo AWAY w „Krainie Przygód” sprawdza `location~=Maze3`, więc działa tylko przy zapalonej lampie.

---

## 5. Nazwy i opisy zmieniające się w czasie gry

Każdą właściwość można w czasie gry zmienić zwykłym przypisaniem. Stożek w „Sklepie” po upuszczeniu dostaje nowy `description`. Bazar w „Równowadze” zmienia opis, gdy z loterii znikną nagrody:

```inform
                         Bazaar.description =
           "To zatłoczony, gwarny bazar.  Tuż przed tobą jest loteria!";
```

### Opis zależny od stanu: describe

`describe` zastępuje cały wpis przedmiotu w opisie miejsca. W przeciwieństwie do `initial`, które działa tylko do pierwszego poruszenia przedmiotu, `describe` działa zawsze, więc może za każdym razem rozstrzygać, który tekst pokazać. Tak opisują się żelazne drzwi w „Równowadze”:

```inform
       describe
       [; if (self has open) "^Żelazne drzwi stoją otworem.";
          if (self hasnt locked) "^Żelazne drzwi są zamknięte, ale nie na klucz.";
          "^Wylot jaskini zagradzają ciężkie żelazne drzwi.";
       ],
```

Platynowa piramida w „Muzeum” ma `describe`, które zawsze wypisuje osobny akapit, dlatego piramida nigdy nie trafia na zwykłą listę przedmiotów.

### Wpis w ekwipunku: invent

Procedurę `invent` biblioteka wywołuje dwa razy: przed wypisaniem nazwy (`inventory_stage == 1`) i po nim (`inventory_stage == 2`). Pudełko zapałek w „Sklepie” dopisuje po nazwie, ile zapałek zostało. Ozdobne pudełko w „Muzeum” na etapie 1 zastępuje nazwę w całości. Zwrócenie prawdy na tym etapie każe jednak bibliotece pominąć cały wpis – także dopiski w nawiasie i zawartość pojemnika – dlatego pudełko samo wypisuje, co jest w środku:

```inform
       invent
       [;  if (inventory_stage==1)
           {   print "to niezwykle bogato zdobione pudełko";
               ...
               if (child(self))
               {   print " (w środku: ";
                   WriteListFrom(child(self), ENGLISH_BIT+CONCEAL_BIT);
                   print ")";
               }
               rtrue;
           }
       ],
```

```
Masz przy sobie:
  to niezwykle bogato zdobione pudełko (w środku: perła)
```

Zagnieżdżone wywołanie `WriteListFrom` jest bezpieczne, bo biblioteka zapamiętuje i odtwarza stan listy, którą właśnie wypisuje. Przedmioty w pudełku nie leżą bezpośrednio w ekwipunku, więc są wymienione w mianowniku, jak wymaga zdanie „w środku: perła”.

`invent` może też być zwykłym napisem. Księga Geoffreya w „Muzeum” podmienia go po obejrzeniu:

```inform
       invent "tę nieszkodliwą starą księgę Geoffreya",
       before
       [;  Examine:
               self.invent = "tę zabójczą starą księgę Geoffreya";
```

Napis w `invent` jest drukowany bez odmiany. Ekwipunek i opisy miejsc wymieniają przedmioty w bierniku („Masz przy sobie rybę”, „Widzisz tutaj rybę”), więc napis trzeba podać w bierniku.

### Zwój, którego nazwa zależy od tego, czy go przeczytano

Zwoje w „Równowadze” ujawniają nazwę zapisanego na nich zaklęcia dopiero po przeczytaniu (flaga `general`). W ekwipunku dopisują ją wtedy w narzędniku: *lśniący zwój (z zaklęciem bozbar)*:

```inform
       invent
       [;   if (inventory_stage==2 && self has general)
            {   if (child(self)==0 || ~~(child(self) ofclass Spell))
                    print " (nieczytelny)";
                else
                {   print " (z ", (nar) child(self), ")"; }
            }
       ];
```

### Księżniczka, która zmienia imię

W „Muzeum” po pocałunku Księżniczka każe się nazywać „/?%?/”. Jej `short_name` działa tylko przed przemianą; potem zwraca fałsz, a biblioteka drukuje zapisaną w obiekcie nazwę, która się nie odmienia:

```inform
       short_name
       [;   if (self hasnt general)
            {   PoPrzyp(self, "Księżniczka", "Księżniczki", "Księżniczce",
                    "Księżniczkę", "Księżniczką", "Księżniczce");
                rtrue;
            }
       ],
```

Nowego imienia nie ma w słowniku, dlatego `parse_name` porównuje je z poleceniem znak po znaku (zob. rozdział 7).

---

## 6. Wiele identycznych przedmiotów

### Obiekty z klasy

Osiem świec w „Sklepie” ma wspólną definicję w klasie, a każdą z nich tworzy krótki wiersz `Candle -> ->;`. Słowo słownika z dopiskiem `//p` jest oznaczone jako forma liczby mnogiej, dzięki czemu „weź świece” bierze wszystkie, a „weź świecę” – jedną:

```inform
        name 'świece//p' 'świec//p' 'świecom//p' 'świecami//p' 'świecach//p'
             'świeczka' 'świeczki' 'świeczkę' 'świeczką',
```

### Kiedy przedmioty są identyczne

Parser uznaje dwa obiekty za identyczne, gdy nie da się ich odróżnić słowami. Nie pyta wtedy „Co masz na myśli: złotą monetę czy złotą monetę?”, tylko bierze którykolwiek. Obiekty z `parse_name` same o tym decydują, bo parser nie wie, jakie słowa rozpoznają: wywołuje `parse_name` z `parser_action == ##TheSame`, a w `parser_one` i `parser_two` podaje oba porównywane obiekty. Monety w „Równowadze” są identyczne, gdy są z tego samego metalu:

```inform
       parse_name
       [ i j;
         if (parser_action==##TheSame)
         {   if (parser_one.typ == parser_two.typ) return -1;
             return -2;
         }
```

Wartość -1 znaczy „identyczne”, a -2 – „różne”. Monety I Ching w „Muzeum” mają własne imiona (koza, jeleń, kogut), więc ich `parse_name` zawsze zwraca -2.

### Wypisywanie grupy: list_together

Angielska biblioteka wypisuje grupę jako „liczebnik + liczba mnoga”, co po polsku dałoby „osiem woskowe świece” i „dwa świece”. Polski liczebnik zgadza się bowiem z rodzajem (*dwie* świece, *dwa* klucze), a forma rzeczownika zależy od liczby (*dwie woskowe świece*, ale *osiem woskowych świec*). Rozwiązaniem jest **`list_together` w postaci procedury**. Biblioteka grupuje obiekty o tej samej wartości `list_together` i przed wypisaniem grupy wywołuje procedurę z `inventory_stage == 1`. Jeśli procedura zwróci prawdę, znaczy to, że sama wypisała całą grupę.

Monety w „Równowadze” są z trzech metali i mogą leżeć razem. Procedura `ListaMonet` liczy je według metalu i wypisuje we właściwym przypadku:

```inform
       list_together
       [;  if (inventory_stage==1)
           {   ListaMonet(parent(parser_one));
               if (c_style & NEWLINE_BIT) new_line;
               rtrue;
           }
           rfalse;
       ],
```

```inform
[ FrazaMonet n rodzaj c;
  if (n==1)
  {   switch (rodzaj) { 1: print "złot"; 2: print "srebrn"; 3: print "brązow"; }
      switch (c)
      {   PRZYP_DOP: print "ej monety";
          PRZYP_BIE: print "ą monetę";
          default:   print "a moneta";
      }
      return;
  }
```

```
Na prawej szalce są trzy złote monety. Na lewej szalce są dwie złote monety
oraz brązowa moneta i pognieciony zwój.
```

W chwili wywołania `parser_one` wskazuje pierwszy obiekt grupy. `c_style & NEWLINE_BIT` sprawdza, czy lista jest wypisywana po jednym przedmiocie w wierszu (jak ekwipunek); wtedy procedura musi sama przejść do nowego wiersza.

Procedura może też **wypisać tylko nagłówek** i zwrócić fałsz. Biblioteka wypisze wtedy sama elementy grupy, a na etapie 2 procedura może dopisać zakończenie. Tak robią plastikowe sztućce w „Muzeum”:

```inform
       list_together
       [;  if (inventory_stage==1)
           { if (c_style & ENGLISH_BIT == 0)   c_style=c_style+ENGLISH_BIT;
             if (c_style & NEWLINE_BIT ~= 0)   c_style=c_style-NEWLINE_BIT;
             if (c_style & INDENT_BIT ~= 0)    c_style=c_style-INDENT_BIT;
             print "plastikowe sztućce (";
           }
           else print ")";
       ];
```

```
Widzisz tutaj plastikowe sztućce (widelec, nóż i łyżkę), trzy kapelusze (fez,
panamę i sombrero), litery X, Y, Z, P, Q i R z zestawu do scrabble, … i osiem
gwiazdek (cztery srebrne, jedna brązowa i trzy złote).
```

Kapelusze mają `list_together "kapelusze"`, czyli zwykły napis. Biblioteka dopisuje przed nim liczebnik, co tu wystarcza, bo „trzy kapelusze” brzmi poprawnie. Przy pięciu kapeluszach napis byłby już błędny („pięć kapelusze”), więc ten sposób nadaje się tylko do grup, których liczebność się nie zmienia. Gwiazdki łączą obie techniki: nagłówek drukuje procedura (`Liczebnik(n, "gwiazdka", "gwiazdki", "gwiazdek")`), a wewnątrz grupy biblioteka łączy identyczne gwiazdki w podgrupy na podstawie właściwości `plural`.

### Czasownik przed listą

Zdanie „W środku **jest** osiem świec” wymaga *jest*, a „W środku **są** dwie świece” – *są*. Polska biblioteka dobiera czasownik do pierwszego elementu listy, także wtedy, gdy jest nim grupa z `list_together`. Autor gry nie musi o to dbać.

### Przedmioty tworzone w czasie gry

Tablica loterii w „Równowadze” ma dziesięć tysięcy losów. Zamiast deklarować tyle obiektów, gra tworzy los dopiero wtedy, gdy gracz go wyciągnie:

```inform
Class Ticket(6)
```

```inform
                i = Ticket.create();
                if (i == 0)
                   "Naganiacz wygląda na metafizycznie zakłopotanego.  ~Yyy,
                    system tworzenia obiektów Informu chyba nie zadziałał.~";

                i.number = last_called; PronounNotice(i);
```

Liczba w nawiasie przy klasie określa, ile obiektów tej klasy może powstać w czasie gry; kompilator rezerwuje na nie miejsce z góry. `create()` zwraca 0, gdy limit się wyczerpie. Oddany naganiaczowi los znika ze świata gry (`remove noun`), ale miejsce w puli zwolniłoby dopiero `Ticket.destroy(noun)`.

### Para, która się rozdziela

Rękawiczki w „Sklepie” to trzy obiekty: para i dwie pojedyncze rękawiczki w jej wnętrzu. Gdy jakakolwiek akcja dotyczy jednej rękawiczki, reguła `before` ich klasy wyjmuje obie, usuwa parę i włącza demona. Demon co turę sprawdza, czy rękawiczki są znowu razem i w tym samym stanie (obie założone albo obie zdjęte), a jeśli tak – łączy je z powrotem w parę.

---

## 7. Własne rozpoznawanie słów: parse_name

`parse_name` to procedura, która sama czyta słowa polecenia (`NextWord()`) i zwraca, ile kolejnych słów odnosi się do obiektu. Wartość 0 znaczy „to nie o mnie”, a -1 – „sprawdź zwykłym sposobem”.

### Połączenie z polską odmianą: Refers

Gdy obiekt ma `parse_name`, biblioteka nie porównuje już słów polecenia z jego nazwą, więc automatyczna odmiana przestaje działać. Przywraca ją wywołanie **`Refers(self, wn-1)`**: sprawdza ono, czy właśnie przeczytane słowo jest którąś formą nazwy obiektu albo słowem z `name`, także wpisanym bez polskich liter. `wn` to numer następnego słowa do przeczytania, więc bieżące ma numer `wn-1`. Klocki ze „Sklepu”:

```inform
        parse_name [ i j;
            for (::) {
                j = NextWord();
                if (j == 'klocki' or 'klocków' or 'klockom' or 'klockami'
                    or 'klockach' or 'sześciany' or 'sześcianów') {
                    parser_action = ##PluralFound; i++;
                }
                else if (j == 'sześcian' or 'sześcianu' or 'sześcianem'
                         or 'kostka' or 'kostkę' or 'kostki' || Refers(self, wn-1))
                    i++;
                else return i;
            }
        ],
```

Słowo w liczbie mnogiej ustawia `parser_action = ##PluralFound`, co mówi parserowi: „chodzi o wszystkie pasujące obiekty, a nie o jeden”.

### Słowa z dodatkowej właściwości

Koń w „Równowadze” reaguje na słowo „skrzydlaty” dopiero wtedy, gdy ma skrzydła. Takie słowa przechowuje w osobnej właściwości `skrzydla`, a sprawdza je pomocnicza procedura `SlowoW`, która akceptuje również słowa wpisane bez ogonków:

```inform
[ SlowoW o prop wnum  w;
    if (~~(o provides prop)) rfalse;
    w = WordValue(wnum);
    if (WordInProperty(w, o, prop)) rtrue;
    if (w == 0 or PL_NIEZNANE) return PL_WSlowach(o, prop, wnum);
    rfalse;
];
```

```inform
       parse_name
       [ i;
          while (wn <= NumberWords())
          {   if (SlowoW(self, name, wn) ||
                  (self has general && SlowoW(self, skrzydla, wn))) { i++; wn++; }
              else return i;
          }
          return i;
       ],
```

`PL_NIEZNANE` to wartość, którą polska biblioteka nadaje słowom spoza słownika. Słowo wpisane bez polskich liter, które odpowiada słowu ze słownika, dostaje zamiast niej wartość tego słowa, więc sprawdzi je już `WordInProperty`. `PL_WSlowach` porównuje pozostałe słowa spoza słownika ze słowami właściwości z pominięciem polskich liter.

### Kolejność słów

Zwykła nazwa dopuszcza słowa w dowolnej kolejności: działa i „zielona papryka”, i „papryka zielona”. Pomidor w kawiarni „Muzeum” wymaga przymiotników przed rzeczownikiem, więc „pomidor smażony czerwony” nie zostanie rozpoznany (skrypt przejścia sprawdza to celowo):

```inform
  with parse_name
       [ i w; w=NextWord();
              while (w=='czerwony' or 'czerwonego' or 'czerwonym' or 'smażony'
                     or 'smażonego' or 'smażonym')
              {   w=NextWord(); i++;
              }
              if (w=='pomidor' or 'pomidora' or 'pomidorem' or 'pomidorze')
                  return i+1;
              return 0;
       ],
```

Figury szachowe w „Alicji” w ten sam sposób dopuszczają najwyżej jeden kolor przed nazwą figury i rozpoznają królową tylko wtedy, gdy nie chodzi o czerwoną, którą gracz już znalazł.

### Przycisk z nazwą napoju

Automat w kawiarni ma jeden obiekt przycisku, który zapamiętuje, o jaki napój chodziło: „naciśnij przycisk z kawą”, „naciśnij herbatę”. `parse_name` przy okazji rozpoznawania zapisuje wybór w `self.number`:

```inform
                switch(NextWord())
                {   'przycisk', 'przycisku', 'guzik', 'guzika', 'z//', 'ze', 'na':
                        flag = 0;
                    'kawa', 'kawę', 'kawy', 'kawą':
                        if (type == 0) { flag = 0; type = 1; }
```

### Przedmioty nazwane przez gracza

Białe sześciany w „Równowadze” nie mają nazw, dopóki gracz czegoś na nich nie napisze („napisz kamień na gładkim sześcianie”). Napis trafia jako ciąg znaków do tablicy we właściwości `number` sześcianu, a `parse_name` porównuje z nim kolejne słowo polecenia:

```inform
                if (flag==0 && ((self.&number)->0) ~= 0)
                {   wn--;
                    if (TextReader(0)==0) return i;
                    for (j=0: j<8: j++)
                        if ((self.&number)->j ~= cube_text_buffer->j)
                            return i;
                    flag=1;
                }
```

`TextReader` kopiuje słowo z bufora wejścia za pomocą `WordAddress(wn)` i `WordLength(wn)`, dzięki czemu działa w obu maszynach. Słowa „gładki” i „pusty” pasują tylko do sześcianów bez napisu, więc „weź gładki sześcian” zawsze wybiera taki, którego gracz jeszcze nie nazwał.

### Odwołanie przez numer obiektu: ParseNoun

`ParseNoun` to punkt wejścia wywoływany przed sprawdzeniem nazwy każdego obiektu. Lupa w „Muzeum” pozwala nazywać rzeczy numerami („obiekt 189”), a znaki `#` i `*` oznaczają odpowiednio dowolny jeden przedmiot i wszystkie przedmioty:

```inform
[ ParseNoun obj;
  if (magnifier notin player) return -1;
  if (NextWord() == 'obiekt' && TryNumber(wn) == obj) return 2;
  wn--;
  if (WordLength(wn)==1 && WordAddress(wn)->0 == '#') return 1;
  if (WordLength(wn)==1 && WordAddress(wn)->0 == '*')
  {   parser_action = ##PluralFound; return 1; }
  return -1;
];
```

---

## 8. Przerabianie polecenia przed analizą

`BeforeParsing` to punkt wejścia wywoływany po podzieleniu polecenia na słowa, ale przed jego analizą. Może zmieniać wartości słownikowe słów w tablicy `parse`. Układ tej tablicy jest inny w maszynie Z i w Glulx, dlatego „Muzeum” zmienia słowa za pomocą małej procedury:

```inform
[ UstawSlowo n w;
    #Ifdef TARGET_ZCODE;
    parse-->(n*2-1) = w;
    #Ifnot;
    parse-->(n*3-2) = w;
    #Endif;
];
```

### Mucha w bursztynie

Polecenie „włóż muchę w bursztynie do pudełka” parser zrozumiałby jako „włóż muchę **w** (bursztynie do pudełka)”, bo „w” jest przyimkiem w gramatyce czasownika „włóż”. `BeforeParsing` zamienia więc to „w” na słowo `'bursztyn'`, które należy do nazwy muchy, i całe wyrażenie staje się nazwą jednego przedmiotu:

```inform
  for (i=1 : i+2<=NumberWords() : i++)
  {   wn=i;
      w = NextWord();
      if ((w=='mucha' or 'muchy' or 'musze' or 'muchę' or 'muchą' ||
           w==0 or PL_NIEZNANE) && NextWord()=='w//' &&
          NextWord()=='bursztynie')
          UstawSlowo(i+1, 'bursztyn');
  }
```

### Dżin, który zamienia biel z czernią

Po potarciu lampy dżina słowa „biały” i „czarny” zamieniają się znaczeniami: „weź biały kamyk” bierze czarny. Taka podmiana działa tylko na słowa ze słownika, a form z automatycznej odmiany w słowniku nie ma. Dlatego kamyki mają wszystkie formy w `name`, a nazwę zapisaną bez ukośników i odmianę podaną ręcznie w `desc_*`:

```inform
Object -> "biały kamyk"
  with name 'biały' 'białego' 'białemu' 'białym' 'kamyk' 'kamyka' 'kamykowi'
            'kamykiem' 'kamyku' 'kamień' 'kamienia',
       desc_dop "białego kamyka", desc_cel "białemu kamykowi",
       desc_nar "białym kamykiem", desc_mie "białym kamyku";
```

Polska biblioteka również przerabia polecenie przed analizą: usuwa słowo „się” (także „sie”), zamienia „na północ” na „północ”, zdejmuje ogonki ze słów spoza słownika, a słowa, które bez ogonków odpowiadają słowu ze słownika („wez”, „xibalbe”), zamienia na to słowo (`'weź'`, `'xibalbę'`). Gra nie musi się tym zajmować, ale autor powinien wiedzieć, że `BeforeParsing` dostaje polecenie już po tych zmianach.

---

## 9. Tekst, liczby i godziny wpisane przez gracza

Token gramatyki może być **procedurą** (*general parsing routine*). Taka procedura sama czyta słowa, zaczynając od `wn`, i zwraca -1 (nie pasuje), 0 (pasuje, bez wartości), 1 (pasuje, liczba w `parsed_number`) albo obiekt.

### Tekst w cudzysłowie

Tablica w biurze „Sklepu” i sześciany w „Równowadze” przyjmują tekst w cudzysłowie: `napisz "ala ma" na gładkim sześcianie`. Procedura `QuotedText` sprawdza, czy następne słowo zaczyna się od cudzysłowu, szuka cudzysłowu zamykającego i zapamiętuje, gdzie w buforze zaczyna się i kończy tekst między nimi:

```inform
[ QuotedText i j f start len;
   #Ifdef TARGET_ZCODE;
   start = 2; len = buffer->1;
   #Ifnot;
   start = WORDSIZE; len = buffer-->0;
   #Endif;
   i = WordAddress(wn++); i=i-buffer;
   if (buffer->i=='"')
   {   for (j=i+1:j<start+len:j++)
           if (buffer->j=='"') f=j;
       if (f==0) return -1;
       from_char = i+1; to_char=f-1;
       if (from_char>to_char) return -1;
       while (wn <= NumberWords() && buffer+f > WordAddress(wn)) wn++;
       wn++;
       return burin;
   }
   return -1;
];
```

Układ bufora wejścia jest inny w maszynie Z i w Glulx, stąd `#Ifdef TARGET_ZCODE`. W „Równowadze” procedura zwraca obiekt `burin` (rylec), który staje się pierwszym dopełnieniem akcji `WriteOn`; drugim jest sześcian.

Polska biblioteka zdejmuje ogonki ze słów spoza słownika, ale tekst w cudzysłowie jest z tego wyłączony, więc na tablicy zostaje dokładnie to, co wpisał gracz. W Glulx polskie litery spoza Latin-1 są zamieniane na litery bez ogonków już przy wczytaniu wiersza, więc tekst w cudzysłowie ich nie zachowa.

### Jedno dowolne słowo

Bez cudzysłowu sześcian przyjmuje jedno słowo („napisz kamień na sześcianie”). Token `AnyWord` zapamiętuje tylko jego numer i przechodzi do następnego słowa:

```inform
[ AnyWord; from_char=0; to_char=0; the_named_word=wn++; return burin; ];

Verb 'napisz' 'wypisz' 'wyryj' 'wygraweruj'
                * AnyWord 'na' held              -> WriteOn
                * QuotedText 'na' held           -> WriteOn;
```

Napis mieści siedem znaków. Słowo spoza słownika straci przy tym ogonki („skała” zmieni się w „skala”), bo biblioteka zdejmuje je przed analizą polecenia; słowo ze słownika („kamień”) zostaje bez zmian. Dlatego skrypt przejścia używa słów, które w grze już występują.

### Godzina

`TimeOfDay` w „Muzeum” rozumie zapis 12:02 i polskie wyrażenia: „7”, „7 30”, „wpół do ósmej”, „kwadrans po 9”, „za kwadrans 7”, „10 po 7”, „za 10 7”, „północ”, „południe”. Wynik, liczbę minut od północy, zapisuje w `parsed_number`:

```inform
  !   "wpół do ósmej" = 7:30
  if (i=='wpół')
  {   if (NextWord()~='do') return -1;
      hr=LiczbaGodziny(wn++); if (hr<1 || hr>24) return -1;
      parsed_number=((hr-1)*60+30)%(2*TWELVE_HOURS); return 1;
  }
```

```inform
Verb 'czas' 'godzina' * TimeOfDay -> SetTheTime;
```

W Polsce godziny podaje się w systemie 24-godzinnym, dlatego w przeciwieństwie do oryginału procedura nie zna oznaczeń „am” i „pm”. Godziny podane słownie („ósmej”) zamienia na liczby procedura `GodzinaSlownie`.

### Liczby ułamkowe i numer telefonu

`FloatingPoint` przyjmuje „3.14”, „3,14” i „3 przecinek 1 4” i zwraca wynik w setnych częściach. `PhoneNumber` składa cyfry z kilku słów („wybierz 0-800-123”) w tablicę bajtów. Oba tokeny pokazują, że procedura może przeczytać dowolnie wiele słów, byle na końcu ustawiła `wn` tuż za ostatnim z nich.

### Trzecie dopełnienie

Akcje w Inform mają najwyżej dwa dopełnienia (`noun` i `second`). Czasownik „potrój” w „Muzeum” pokazuje, jak dodać trzecie: token wywołuje `NounDomain`, czyli zwykłe rozpoznawanie przedmiotu, i zapisuje wynik w zmiennej globalnej:

```inform
[ GhostObject x;
  x=NounDomain(player,location,0);
  if (x==REPARSE_CODE) return x;
  if (x==0 or 1) return -1;
  third = x;
  return 0;
];
Verb 'potrój' * noun noun GhostObject -> Threefold;
```

---

## 10. Postacie: rozmowa, rozkazy, wędrówki

### Rozmowa: Ask, Answer, Tell

W akcji `Ask` („zapytaj naganiacza o nagrody”) `noun` wskazuje postać, a `second` – pierwsze słowo tematu. Temat jest zwykłym słowem słownika, więc w `switch` trzeba wymienić wszystkie formy, jakich gracz może użyć: „o nagrody”, „o nagrodę”, „o nagrodach”:

```inform
           Ask:  switch(second)
                 {   'nagroda', 'nagrodę', 'nagrody', 'nagród', 'nagrodach':
                         "~Tylko jedna srebrna moneta i nagroda może być
                          twoja!~";
```

Oryginalna „Równowaga” porównywała tu `noun`, co w bibliotece 6.12 sprawiało, że naganiacz zawsze odpowiadał „Po prostu graj, koleś”; polska wersja ma ten błąd poprawiony. Odwrotnie jest w akcji `Answer` („powiedz Krzysztofowi cześć”): słowo leży w `noun`, a postać w `second`. W „Sklepie” Krzysztof w ten sposób odpowiada na powitania.

### Rozkazy: orders

Polecenie „Krzysztofie, połóż niebieski klocek na żółtym klocku” trafia do właściwości `orders` postaci. Krzysztof nie układa klocków sam. Żeby nie powielać reguł klocków dla drugiej postaci, gra **pożycza** klocek graczowi, ustawia Krzysztofowi flagę `general` i wykonuje zwykłą akcję gracza:

```inform
    PutOn: if (noun notin Chris) "Krzysztof zupełnie nie wie, o co chodzi.";
           if (~~(noun ofclass Block && second ofclass Block))
               "Krzysztof nie widzi w tym sensu.";
           print "Krzysztof pochyla się w wielkim skupieniu i robi,
                  o co prosisz.^";
           move noun to player; give self general;
           <PutOn noun second>;
           give self ~general; rtrue;
```

Reguły klocków sprawdzają flagę i wiedzą, że to wprawne ręce Krzysztofa, a nie niezgrabne ręce gracza. Gracz zwraca się do postaci w wołaczu, a biblioteka nie tworzy go sama, dlatego `name` Krzysztofa zawiera `'krzysztofie'` i `'krzysiu'`.

### Postać, która rozumie polecenia po swojemu: grammar

Właściwość **`grammar`** postaci jest wywoływana przed analizą skierowanego do niej rozkazu i może podmienić czasownik. Dan Dyslektyk w „Muzeum” myli „weź” z „upuść”:

```inform
       grammar
       [;  if (verb_word == 'weź') { verb_wordnum++; return 'upuść'; }
           if (verb_word == 'upuść') { verb_wordnum++; return 'weź'; }
       ],
```

Charlotte wykonuje tylko polecenia zaczynające się od „szymon mówi”, a samą liczbę traktuje jako polecenie klaśnięcia:

```inform
       grammar
       [;  give self ~general;
           wn=verb_wordnum;
           if (NextWord()=='szymon' && NextWord()=='mówi')
           {   give self general;
               verb_wordnum=verb_wordnum+2;
           }
           self.number=TryNumber(verb_wordnum);
           if (self.number~=-1000)
           {    action=##Clap; noun=0; second=0; rtrue; }
       ],
```

Przesunięcie `verb_wordnum` o dwa każe parserowi uznać za czasownik dopiero słowo następujące po „szymon mówi”. Zwrócenie prawdy z ustawionym `action` pomija analizę polecenia: akcja jest już gotowa.

### Postać, która chodzi za graczem

Niedźwiedź w „Przygodzie” po odwiązaniu chodzi za graczem. Zajmuje się tym demon: jeśli niedźwiedzia nie ma tam, gdzie gracz, przenosi go w to miejsce:

```inform
        daemon [;
            if (location == thedark) rfalse;
            if (self in location) {
                if (location == At_Breath_Taking_View)
                    "^Niedźwiedź ryczy z zachwytu.";
                rfalse;
            }
            move self to location;
            "^Niedźwiedź człapie za tobą.";
        ],
```

Krasnolud w „Przygodzie” działa podobnie, ale pojawia się losowo i tylko w miejscach bez atrybutu `nodwarf`. Własny atrybut (`Attribute nodwarf;`) to wygodny sposób oznaczania miejsc, w których coś nie może się zdarzyć.

### Postać, która wędruje sama

Kustosz w „Muzeum” po obudzeniu przechadza się po budynku. Jego demon przegląda wszystkie kierunki kompasu, liczy wyjścia prowadzące do zwykłych miejsc i losowo wybiera jedno z nich:

```inform
           objectloop (i in compass)
           {   j=p.(i.door_dir);
               if (ZRegion(j)==1 && j hasnt door) n++;
           }
```

Każdy obiekt kierunku ma właściwość `door_dir` (np. `n_to`), więc `p.(i.door_dir)` to wyjście miejsca `p` w tym kierunku. `ZRegion(j)==1` sprawdza, czy wyjście jest obiektem, a nie napisem ani procedurą, a warunek `j hasnt door` wyklucza drzwi, więc kustosz chodzi tylko między salami połączonymi bezpośrednio.

---

## 11. Przedmioty, które słuchają poleceń

Przedmiot z atrybutem **`talkable`** może przyjmować polecenia jak postać („budzik, 7:30”), choć nie jest żywy. W „Muzeum” takich przedmiotów jest kilka: budzik, komputer, trikorder, replikator i komputer Zen.

Każdy z nich ma `grammar`, która zwraca **słowo z przecinkiem**. Takiego słowa gracz nie może wpisać, bo przecinek rozdziela słowa polecenia, ale można je zadeklarować jako czasownik z własną gramatyką:

```inform
       grammar
       [;  return 'alarm,';
       ],
```

```inform
Verb 'alarm,' * 'włącz'    -> SwitchOn
              * 'wyłącz'   -> SwitchOff
              * TimeOfDay  -> SetTo
              * topic      -> Inv;
```

Dzięki temu „budzik, włącz” znaczy co innego niż „włącz budzik”, a gramatyka budzika nie miesza się z gramatyką zwykłych czasowników. Ostatnia linia z `topic` łapie wszystko inne i zamienia na akcję, którą `orders` budzika odrzuca uprzejmym komunikatem.

Zen zwraca `-'zen,'`, z minusem. Ujemne słowo znaczy: „spróbuj najpierw tej gramatyki, a jeśli nie pasuje, użyj zwykłej”. Polecenie, którego nie rozumie żadna z nich, np. „zen, zrób kawę”, trafia do `NotUnderstood` w `orders` Zena:

```inform
Verb 'zen,'   * 'skanuj' number 'po' 'orbicie'         -> Show
              * 'ustaw' 'kurs' 'na' Planet             -> Go
              * 'prędkość' 'standardowa' number        -> SetTo
              * 'podnieś' held                         -> Take
              * 'przygotuj' held 'do' 'strzału'        -> Drop;
```

Akcje są tu tylko etykietami: `orders` Zena dostaje `Show` z liczbą stopni w `noun`, a `SetTo` z prędkością, i sam decyduje, co zrobić. Komputer w 10 Forward korzysta dodatkowo z zasięgu wyznaczanego przez procedurę (rozdział 13).

---

## 12. Zmiana postaci gracza

`ChangePlayer(obj)` sprawia, że gracz zaczyna sterować innym obiektem – postacią, która ma własne położenie, ekwipunek i opis. W „Muzeum” portret zamienia gracza w guźca, a jagoda przywraca mu dawną postać:

```inform
       before
       [;  Examine: <Examine hog>; print "^...i jakimś cudem ty też...^";
               ChangePlayer(hog); <<Look>>;
       ],
```

```inform
               ChangePlayer(selfobj); <<Look>>;
```

`selfobj` to pierwotna postać gracza. Kiedy gracz jest kimś innym, `selfobj` zostaje tam, gdzie był, a opis miejsca pokazuje go jako „twoje dawne ja”.

Każde polecenie gracza przechodzi przez `orders` postaci, którą gracz w tej chwili steruje. Guziec pozwala tylko na proste czynności:

```inform
       orders
       [;  if (player~=self || actor~=self) rfalse;

           ! Teraz został już tylko przypadek, gdy gracz jest guźcem
           ! i wydaje polecenia sam sobie:

           Go, Look, Examine, Eat, Smell, Taste, Touch: rfalse;
           default: "Guźce nie potrafią robić tak skomplikowanych rzeczy!";
       ],
```

Olbrzym wraca do dawnej postaci po poleceniu „pchnij siebie”. Oryginał wymagał pchnięcia obiektu `selfobj`, a do niego gracz nie ma jak się odwołać, bo „siebie” oznacza obecną postać. Polska wersja przyjmuje więc oba obiekty:

```inform
               if (action==##Push && noun==selfobj or self)
               {   ChangePlayer(selfobj); <<Look>>; }
```

Płeć postaci gracza wyraża atrybut `female` obiektu `player`. „Alicja” zaczyna od `give player female;` w `Initialise` i od tej chwili wszystkie komunikaty biblioteki używają form żeńskich („Zwyciężyłaś”). We własnych tekstach gry ten sam efekt daje `Gracz("eś", "aś")`.

---

## 13. Zasięg: co gracz może nazwać

Gracz może odwoływać się tylko do przedmiotów **w zasięgu** (*scope*), zwykle tych, które widzi. Zasięg można zmienić na trzy sposoby.

### Punkt wejścia InScope

Punkt wejścia `InScope(actor)` jest wywoływany przy każdym ustalaniu zasięgu. Może dodać pojedyncze obiekty (`PlaceInScope`) albo całą zawartość miejsca (`ScopeWithin`). W „Muzeum”:

```inform
[ InScope actor;

   if (actor==martha) PlaceInScope(player);
   if (actor==player && scope_reason==TALKING_REASON && player in booth)
       PlaceInScope(martha);

   if (location==window_w && window_e has light)
       ScopeWithin(window_e);
   if (location==window_e && window_w has light)
       ScopeWithin(window_w);
   rfalse;
];
```

- **Rozmowa na odległość.** Martha jest w zamkniętym pokoju w innej części muzeum, ale z kabiny telepatycznej gracz może do niej mówić. Warunek `scope_reason==TALKING_REASON` ogranicza to do rozmowy: Marthy nie da się wziąć ani obejrzeć.
- **Widok przez szybę.** W sali przedzielonej szybą przedmioty z drugiej strony są w zasięgu, więc da się je nazwać i obejrzeć. Pozostałe akcje odrzuca reguła `before` sali.

W „Równowadze” zapamiętane zaklęcia leżą w obiekcie `memory`, którego gracz nie widzi. `InScope` dodaje je do zasięgu, gdy czasownikiem polecenia jest rzucanie zaklęcia:

```inform
[ InScope i;
  if (CzasownikZaklec(verb_word))
      objectloop (i in memory) PlaceInScope(i);
  rfalse;
];
```

### Token scope=Procedura

Pojedyncza linia gramatyki może mieć własny zasięg. W „Równowadze” polecenie „naucz się frotz” widzi tylko zaklęcia zapisane w księdze, którą gracz trzyma:

```inform
[ ReadableSpell i j k;
  if (scope_stage==1)
  {   if (action_to_be==##Examine) rfalse;
      rtrue;
  }
  if (scope_stage==2)
  {   objectloop (i in player)
          if (i ofclass SpellBook)
          {   for (k=0:k<i.capacity && (i.&array_of_spells)-->k~=0:k++)
              {   j=(i.&array_of_spells)-->k; PlaceInScope(j);
              }
          }
      rtrue;
  }
```

Procedura jest wywoływana na kilku etapach (`scope_stage`): na etapie 1 odpowiada, czy wolno podać kilka obiektów naraz; na etapie 2 umieszcza obiekty w zasięgu (zwrócenie prawdy znaczy „tylko te”, fałszu – „te i zwykłe”); na etapie 3 może wypisać własny komunikat o błędzie.

### Pytania o świat

Pytania „kim jest Helistar”, „co to jest grue” i „jak działają zaklęcia” w „Równowadze” to akcja `Query` na obiektach klasy `Question`, które leżą poza światem gry. Zasięg wyznaczany przez procedurę `Topic` zawiera tylko je:

```inform
Verb 'kim' 'kto' 'czym' 'jak'
                * 'czuję'                        -> Diagnose
                * scope=Topic                    -> Query
                * 'jest'/'był'/'była'/'było'/'to' scope=Topic -> Query
                * 'to' 'jest'/'był'/'była'/'było' scope=Topic -> Query;
```

Komputer z „Muzeum” robi to samo z załogą: `scope=Crew` zawiera wszystkie obiekty z atrybutem `crewmember`, gdziekolwiek się znajdują („komputer, gdzie jest kapitan picard”).

---

## 14. Zaklęcia, magiczne słowa i czasowniki tworzone w czasie gry

### Słowo, które jest czasownikiem tylko w tej grze

Najprostsze magiczne słowo to zwykły czasownik bez dopełnień. „Przygoda” ma ich kilka, a „Kraina Przygód” – trzy:

```inform
Verb 'bunyon'   *                                -> Bunyon;
Verb 'away'     *                                -> Away;
Verb 'tsurris'  *                                -> Tsurris;
```

Magiczne słowa pozostały w polskich wersjach w brzmieniu oryginalnym, bo gracz poznaje je z napisów w grze i są częścią zagadek. „Przygoda” dodaje do nich `Extend 'powiedz' first`, żeby działało też „powiedz xyzzy”.

### Czasownik, którego nie ma w gramatyce: UnknownVerb

Punkt wejścia `UnknownVerb(word)` jest wywoływany, gdy pierwsze słowo polecenia nie jest czasownikiem. Może zwrócić inne słowo, które parser potraktuje jako czasownik. W „Równowadze” nazwa zapamiętanego zaklęcia staje się w ten sposób czasownikiem: „frotz monetę” znaczy „rzuć frotz na monetę”:

```inform
[ UnknownVerb word i;
  objectloop (i in memory)
      if (word==(i.&name)-->0) { the_spell_was = i; return 'c,cast'; }
  rfalse;
];
```

`'c,cast'` to słowo z przecinkiem, którego gracz nie może wpisać, z własną gramatyką (`Verb 'c,cast' * -> CastOne * noun -> CastOne …`). Zaklęcie, którego gracz nie pamięta, nie staje się czasownikiem – i o to właśnie chodzi.

„Przygoda” wykorzystuje ten sam punkt wejścia do szybkich przejść: nazwa odwiedzonego już miejsca staje się poleceniem „idź”:

```inform
[ UnknownVerb word
    obj;
    objectloop (obj ofclass Room) {
        if (obj has visited && WordInProperty(word, obj, name)) {
            verb_wordnum = 0;
            return 'idź';
        }
    }
    rfalse;
];
```

`PrintVerb` uzupełnia `UnknownVerb`. Gdy parser wypisuje czasownik („Zrozumiałem tylko tyle, że chcesz …”, „Co chcesz …?”), dla słowa `'c,cast'` drukuje „rzucić zaklęcie na”. Ten sam punkt wejścia podaje bezokoliczniki wszystkich nowych czasowników gry; bez niego parser wypisałby słowo ze słownika, w trybie rozkazującym i obcięte do dziewięciu znaków („Co chcesz zapamięt?”).

### System zaklęć

Zaklęcie w „Równowadze” to obiekt klasy `Spell` z właściwościami `magic` (działanie) i `unmagic` (działanie odwrócone). Przy rzucaniu zaklęcia pierwszeństwo ma **cel** i jego reguła `before`, a dopiero potem samo zaklęcie:

```inform
[ CastSub;
  the_spell_was = noun; memory.forget_spell(noun);

  if (noun has reversed)
  {   give noun ~reversed;
      if (noun.unmagic() ~= 0) return;
      "Nic się nie dzieje.";
  }

  if (second ~= 0)
  {   ResetVagueWords(second);                     ! ustawia zaimki
      if (second provides before
          && second.before() ~= 0) return;         ! reguły before
  }
  if (noun.magic() ~= 0) return;
  "Nic się nie dzieje.";
];
```

Cel sprawdza w `before`, które zaklęcie na niego rzucono (`the_spell_was`). Dzięki temu efekty szczególne są zapisane w obiektach, których dotyczą, a ogólne – w zaklęciach. Tak reaguje wąż przy przepaści:

```inform
       before
       [;  Cast:
               switch(the_spell_was)
               {   urbzig_spell:
                       remove self;
```

Zaklęcie lleps odwraca działanie innego zaklęcia (atrybut `reversed`), a odwrócone zaklęcie mortin przygotowuje zmartwychwstanie (rozdział 18).

### Własne komunikaty parsera: ParserError

Gdy gracz poda nieznany czasownik albo zaklęcie spoza zasięgu, biblioteka odpowiedziałaby „Nie znam takiego czasownika” albo „Nie widzisz niczego takiego”. Punkt wejścia `ParserError` może zastąpić te komunikaty. „Równowaga” rozpoznaje w ten sposób, że gracz próbował rzucić zaklęcie, które zna, ale którego nie ma już w pamięci:

```inform
      if (flag==1)
      {   if (vb==0 or 1)
             "Nie masz tego zaklęcia w pamięci.  [Wpisz ~zaklęcia~, żeby
              zobaczyć, co pamiętasz.]";
```

Argument `etype` mówi, jaki to błąd (`VERB_PE` – nieznany czasownik, `ASKSCOPE_PE` – błąd zasięgu z procedury). Zwrócenie fałszu zostawia komunikat bibliotece.

### Słowa magiczne z warunkiem

BUNYON w „Krainie Przygód” działa tylko z siekierą w rękach, a AWAY – tylko z dywanem w labiryncie. Fee-fie-foe-foo w „Przygodzie” trzeba wypowiedzieć po kolei: licznik `feefie_count` pilnuje kolejności i zeruje się po pomyłce.

---

## 15. Nowe czasowniki i podmiana akcji biblioteki

### Verb i Extend

Nowy czasownik składa się z dyrektywy `Verb` z liniami gramatyki i z procedury akcji, której nazwa kończy się na `Sub`:

```inform
[ ClapSub; "Klaszczesz."; ];
Verb 'klaśnij' 'klaszcz' 'klaskaj' * -> Clap;
```

`Extend` dopisuje linie do istniejącego czasownika: na końcu (domyślnie), na początku (`first`) albo w miejsce wszystkich dotychczasowych (`replace`). Kolejność ma znaczenie, bo parser próbuje linii po kolei i wybiera pierwszą pasującą. `Extend` działa na całą grupę synonimów – `Extend 'obejrzyj'` w „Alicji” zmienia też „zbadaj”, „opisz” i „x”. `Extend only 'kopnij'` w „Przygodzie” odłącza jedno słowo od grupy i rozszerza tylko je.

W polskiej grze nowe czasowniki stoją **po** `Include "PolskiG"`, bo dopiero tam są zdefiniowane czasowniki, które się rozszerza. Słowa „się” nie należy dopisywać, bo biblioteka usuwa je z polecenia („naucz się frotz” trafia do parsera jako „naucz frotz”). Bezokoliczniki nowych czasowników podaje `PrintVerb` (rozdział 14), który musi stać **przed** `Include "PolskiG"`.

### Nowa akcja z dwoma dopełnieniami

„Zbadaj kotka w lustrze” w „Alicji” to nowa akcja `Reflect`, dopisana do istniejących czasowników:

```inform
Extend 'obejrzyj'
    * noun 'w//' noun                           -> Reflect;

Extend 'spójrz'
    * 'na' noun 'w//' noun                      -> Reflect;
```

Jednoliterowy przyimek zapisuje się z `//` (`'w//'`), bo inaczej kompilator uznałby go za stałą znakową.

### Filtr dopełnienia: noun=Procedura

Linia gramatyki może przyjmować tylko obiekty spełniające podany warunek. „Muzeum” korzysta z tego, żeby w poleceniu „zamknij szafkę” domyślić się klucza, gdy gracz ma przy sobie dokładnie jeden:

```inform
[ DefaultLockTest;
  if (noun has open || noun has locked) rfalse;
  return DefaultKeyTest();
];
[ DefaultUnlockTest;
  if (noun hasnt locked) rfalse;
  return DefaultKeyTest();
];

Extend 'zamknij' first * noun = DefaultLockTest -> DefaultLock;
Extend 'otwórz' first * noun = DefaultUnlockTest -> DefaultUnlock;
```

Po angielsku zamykanie i zamykanie na klucz to dwa różne czasowniki (*close* i *lock*), podobnie jak otwieranie i otwieranie kluczem (*open* i *unlock*). Po polsku „zamknij” i „otwórz” służą do obu czynności. Dlatego w odróżnieniu od oryginału filtry sprawdzają też stan przedmiotu: „zamknij” otwartą szafkę po prostu zamyka, a dopiero „zamknij” zamkniętą – zamyka na klucz. Tak samo „otwórz” otwiera kluczem tylko szafkę zamkniętą na klucz.

### Podmiana całej akcji: Replace

„Sklep z zabawkami” i „Kraina Przygód” zastępują biblioteczną procedurę podpalania własną:

```inform
Replace BurnSub;
```

`Replace` musi stać przed `Include "Parser"`, żeby kompilator pominął wersję biblioteczną; nowa procedura może stać w dowolnym miejscu pliku. W „Krainie Przygód” `BurnSub` sprawdza, czy gracz ma krzesiwo, i wysadza nadęty bukłak. Reguły `before` przedmiotów działają tak jak wcześniej, bo biblioteka wywołuje je, zanim dojdzie do `BurnSub`.

Polecenie biblioteki można też całkowicie zastąpić: `Extend 'pomoc' replace * -> Help;` w „Przygodzie” i „Krainie Przygód” zamienia krótką pomoc biblioteki na pomoc gry.

---

## 16. Drzwi, wyjścia i pojazdy

### Wyjście jako tekst, obiekt albo procedura

Właściwość kierunku (`n_to`, `e_to`…) może być miejscem, drzwiami, napisem albo procedurą. W „Muzeum” są przykłady wszystkich czterech:

```inform
       n_to "Ten napis to wartość ~n_to~ tego pomieszczenia.",
       e_to
       [;  if (random(2)==1) return Foyer; return Room10a; ],
```

Procedura zwraca miejsce docelowe albo wypisuje tekst i zwraca prawdę, co zatrzymuje gracza w miejscu. Jaskinia w „Równowadze” nie wypuszcza gracza, dopóki szalki wagi nie są w równowadze:

```inform
       out_to
       [;  if (CoinsIn(left_pan)+CoinsIn(right_pan) < 6)
               "Coś zagradza ci drogę i słyszysz, jak waga wojowniczo pobrzękuje.
                Chciał", (nop) Gracz("eś", "aś"), " ukraść jej monety!";
           if (scales.number~=0) "Coś zagradza ci drogę i słyszysz, jak waga
               pobrzękuje lekko, pełna energii.";
           return Iron_Door;
       ],
```

### Drzwi z dwóch stron

Drzwi widoczne z dwóch miejsc mają `found_in`, a `door_to` i `door_dir` w postaci procedur, których wynik zależy od tego, gdzie stoi gracz:

```inform
       door_dir
       [; if (location==Cave_Mouth) return w_to; return e_to;
       ],
       door_to
       [; if (location==Cave_Mouth) return In_Cave;
          return Cave_Mouth;
       ],
```

Kładka w „Muzeum” to drzwi, których `door_to` może zabić gracza – jeśli coś niesie, jest dla niej za ciężki:

```inform
       door_to
       [;  if (children(player)~=0)
            {   deadflag=1;
```

### Drzwi, które powstają w czasie gry

Cyprys w „Krainie Przygód” pełni rolę drzwi prowadzących w górę. Po jego ścięciu gra usuwa drzewo, wstawia pień i przestawia wyjścia bagna:

```inform
                    remove self; move tree_remains to Dismal_Swamp;
                    Dismal_Swamp.in_to = tree_remains;
                    Dismal_Swamp.u_to = 0;
```

Wysadzone okno w komnacie królewskiej działa tak samo: zamurowane okno znika, a jego miejsce zajmuje okno z dziurą, które jest drzwiami na wschód.

### Odwrócony świat

Lustro w Sali Kierunków „Muzeum” zamienia wschód z zachodem. Gra podmienia `door_dir` obiektów kierunków, a w tekstach używa **zmiennych napisowych** `@00`–`@03`, które wskazują na słowa „wschód”, „zachód”, „wschodnia”, „zachodnia”. Zmiana kilku zmiennych odwraca wtedy wszystkie opisy naraz:

```inform
[ ReversedWorld; string 0 west_str; string 1 east_str;
  string 2 west_adj; string 3 east_adj;
  w_obj.door_dir = e_to; e_obj.door_dir = w_to;
];
```

Opis „To Sala 11, na @00 od holu” wypisuje wtedy „na zachód”. W maszynie Z teksty dla zmiennych napisowych trzeba zadeklarować przez `Lowstring`, bo muszą leżeć w dolnej części pamięci; w Glulx wystarczy zwykła stała napisowa.

### Pojazdy

Samochodzik („Sklep”, „Muzeum”) ma atrybuty `enterable container`. Gdy gracz siedzi w środku i idzie, akcja `Go` trafia najpierw do reguły `before` pojazdu, która może przenieść pojazd razem z graczem. Z zewnątrz „pchnij samochodzik na zachód” to akcja `PushDir`, a `AllowPushDir()` pozwala go przetoczyć.

Latający dywan w „Równowadze” przenosi się wraz z graczem przez `PlayerTo` i od razu sadza go z powrotem na sobie:

```inform
                 move self to i;
                 PlayerTo(i,1); move player to self; <<Look>>;
```

Drugi argument `PlayerTo` (1) pomija opis miejsca, bo dywan wywołuje `Look` sam, dopiero po posadzeniu gracza; inaczej opis pokazałby gracza stojącego obok dywanu. Skrzydlaty koń nie musi nikogo sadzać, więc wystarcza mu jedno `PlayerTo(Edge)`.

---

## 17. Własny wygląd gry

### Opis miejsca w innym stylu

„Kraina Przygód” naśladuje gry Scotta Adamsa: krótki opis, lista wyjść i przedmioty w jednym wierszu. Klasa `ScottRoom` ma `describe`, które zastępuje cały opis miejsca:

```inform
      describe
      [ i c d;  print (string) self.description;
          print "^^Widoczne wyjścia: ";
          for (i=n_to: i<=d_to: i++)
              if (self.i ~= 0) c++;
          if (c==0) print "żadnych!";
```

Właściwości kierunków mają kolejne numery od `n_to` do `d_to`, więc można po nich przejść pętlą. Przedmioty, które `describe` już wymieniło, dostają atrybut `concealed`, żeby biblioteka nie wypisała ich drugi raz; `GamePostRoutine` zdejmuje go po zakończeniu `Look`.

```
Ponure bagno
Jesteś na ponurym bagnie.

Widoczne wyjścia: Północ, Południe, Wschód, Zachód, Góra.

Widzisz też: cyprys - cuchnące błoto - bagienny gaz - rój meszek
```

Ekwipunek ma ten sam styl dzięki `GamePreRoutine`, która przejmuje akcję `Inv`, zanim obsłuży ją biblioteka.

### Własna linia statusu

„Muzeum” zastępuje `DrawStatusLine` własną procedurą (`Replace DrawStatusLine;` przed `Include "Parser"`) i rysuje jedną z czterech linii statusu, wybieranych pokrętłem na dachu: niewidoczną, z wyśrodkowaną nazwą miejsca, standardową albo z różą wiatrów. Procedura używa instrukcji maszyny Z (`@split_window`, `@set_cursor`), dlatego w polskiej wersji jest otoczona `#Ifdef TARGET_ZCODE`, a w Glulx zostaje standardowa linia statusu.

### Wypisywanie list i przypadek

`WriteListFrom(obiekt, styl)` wypisuje listę przedmiotów; styl to suma bitów (`ENGLISH_BIT` – w postaci zdania, `NEWLINE_BIT` – po jednym w wierszu, `RECURSE_BIT` – razem z zawartością…). Maszyna do List w „Muzeum” pozwala przełączać te bity i obserwować skutki.

Lista wypisana przez grę jest domyślnie w mianowniku. Jeśli zdanie wymaga innego przypadku („Widzę **rybę** i kadmowy klucz”), trzeba go ustawić przez `ListaWPrzypadku`, a po wypisaniu przywrócić wywołaniem `ListaWPrzypadku(0)`:

```inform
               print ".  Widzę ";
               ListaWPrzypadku(r, PRZYP_BIE);
               WriteListFrom(child(r),CONCEAL_BIT+ENGLISH_BIT);
               ListaWPrzypadku(0);
```

To samo dotyczy `Locale` z własnym tekstem (widok przez szybę i kwarcowe okno w „Muzeum”): biblioteka wypisuje wtedy listę bez zmiany przypadku, więc biernik musi ustawić gra.

---

## 18. Punktacja, śmierć i zakończenia

### Punkty za przedmioty i miejsca

Najprostsza punktacja polega na tym, że biblioteka dodaje `OBJECT_SCORE` punktów za pierwsze wzięcie przedmiotu z atrybutem `scored` i `ROOM_SCORE` punktów za pierwsze wejście do miejsca z tym atrybutem. Sześciany w „Równowadze” mają atrybut `scored`, a stała `OBJECT_SCORE 5` ustala ich wartość.

### Zadania

„Sklep z zabawkami” korzysta z systemu zadań: każde zadanie ma numer i wartość, a polecenie „pełny wynik” pokazuje listę zaliczonych. Zadanie zalicza wywołanie `Achieved(numer)`, a nazwy zadań podaje `PrintTaskName`. Najlepiej brzmią jako rzeczowniki odczasownikowe w mianowniku („zjedzenie landrynki”), bo biblioteka wypisuje je w kolumnie obok liczby punktów:

```inform
Constant TASKS_PROVIDED;
Constant NUMBER_TASKS = 6;
Array    task_scores  -> 1 1 1 1 1 1;
Constant MAX_SCORE = 6;
```

Wywołanie `Achieved` w grze bez tych stałych powoduje błąd wykonania. Oryginalne „Muzeum” wywołuje je przy przejażdżce samochodzikiem, choć systemu zadań nie ma, dlatego polska wersja to wywołanie pomija.

### Skarby

W „Przygodzie” klasa `Treasure` sama dolicza i odejmuje punkty przy braniu i odkładaniu skarbów, a za złożenie skarbu w budynku daje dodatkowo `depositpoints`. „Kraina Przygód” robi to prościej: punkt wejścia `TimePasses`, wywoływany co turę, przelicza wynik od zera, gdy gracz jest we wnętrzu pnia, i kończy grę po zebraniu 100 punktów:

```inform
[ TimePasses i;
  if (location ~= Inside_Stump) rfalse;

  score=0;
  objectloop (i in Inside_Stump && i ofclass Treasure)
      score = score + i.depositpoints;

  if (score ~= 100) rfalse;

  deadflag=2;
```

### Śmierć i wskrzeszenie

Niezerowa wartość `deadflag` kończy grę: 1 oznacza śmierć, 2 zwycięstwo, a 3 i więcej – zakończenia, które opisuje `DeathMessage` (w „Ruinach”: „Zostałeś pojmany”). Zanim biblioteka ogłosi koniec, wywołuje punkt wejścia `AfterLife`. Jeśli ten wyzeruje `deadflag`, gra toczy się dalej.

„Przygoda” pyta gracza, czy go wskrzesić, i robi to najwyżej trzy razy. „Kraina Przygód” przenosi go do Mglistej Sali. „Równowaga” wskrzesza tylko tego, kto wcześniej rzucił odwrócone zaklęcie mortin, i za każdym razem zmniejsza pojemność jego pamięci:

```inform
[ AfterLife;
  if (~~prepared_flag) rfalse;
```

```inform
  prepared_flag = false; deadflag = false; hearing_good = false;

  if (memory.capacity >= 2) memory.capacity--;

  while (child(player)~=0) move child(player) to parent(player);
  move players_book to player;
```

`deadflag==3` w „Krainie Przygód” oznacza śmierć ostateczną (spotkanie z facetem z widłami): `AfterLife` zamienia ją na zwykłą śmierć, ale gracza nie wskrzesza.

### Tytuł na koniec

`PrintRank` dopisuje tekst po zdaniu z wynikiem. Polska biblioteka kończy to zdanie liczbą ruchów, bez kropki, więc `PrintRank` zaczyna od przecinka i sam zamyka zdanie:

```inform
[ PrintRank;
  print ", co daje ci tytuł: ";
  if (score >= 50)  "Naukowiec.";
```

---

## 19. Pułapki przy przenoszeniu gier na polski

Przy tłumaczeniu gier przykładowych powtarzały się te same problemy. Większość z nich ujawnia się dopiero w czasie gry, a nie przy kompilacji, dlatego skrypty przejść są tak ważne.

- **Nazwa z procedury się nie odmienia.** Obiekt, którego `short_name` jest procedurą, musi sam wypisać nazwę we właściwym przypadku (rozdział 1), a słowa do rozpoznawania podać w `name`.
- **Wartość właściwości nie może być listą wydruku.** `description "Jesteś pewn", (nop) Gracz("y", "a"), "."` się nie skompiluje, bo właściwość przyjmuje jeden napis albo procedurę. Opis zależny od płci gracza musi być procedurą: `description [; "Jesteś pewn", (nop) Gracz("y", "a"), "."; ]`. To samo dotyczy przypisań (`thedark.description = …`) – trzeba przypisać procedurę.
- **`Gracz`, `PL`, `Forma` w zdaniu.** Te procedury same drukują tekst i zwracają 0. W liście wydruku trzeba je więc zapisywać z regułą `(nop)`, która ignoruje zwróconą wartość: `"Wpisał", (nop) Gracz("eś", "aś"), " liczbę"`. Reguła `(string)` próbowałaby jeszcze wypisać napis spod adresu 0, co daje przypadkowe znaki.
- **Form z odmiany nie ma w słowniku.** Parser je rozpoznaje, ale w tablicy `parse` mają wartość `PL_NIEZNANE`. Kod, który porównuje słowa ze stałymi słownikowymi (`switch(NextWord())`, `BeforeParsing`, `Ask` z listą słów), musi wymienić formy jawnie albo użyć `Refers` lub `SlowoW`.
- **Słowo, które jest też przyimkiem.** „W”, „na”, „do” czy „z” w nazwie przedmiotu mogą zostać wzięte za przyimek polecenia (mucha w bursztynie, rozdział 8). Krótkie nieodmienne słowo z nazwy biblioteka uznaje za jej część tylko wtedy, gdy następne słowo również do nazwy pasuje.
- **Listy wypisywane przez grę są w mianowniku.** `WriteListFrom` i `Locale` z własnym tekstem trzeba w razie potrzeby otoczyć wywołaniami `ListaWPrzypadku(…, PRZYP_BIE)` i `ListaWPrzypadku(0)`.
- **Liczebniki.** Wszędzie tam, gdzie oryginał pisze „liczba + liczba mnoga”, po polsku potrzebne jest `Liczebnik(n, "moneta", "monety", "monet")`, a w rodzaju żeńskim także „dwie” zamiast „dwa”.
- **Napisy od gracza tracą ogonki.** W słowach spoza słownika biblioteka zamienia polskie litery na litery bez ogonków, zanim gra je zobaczy. Tekst w cudzysłowie jest z tej zamiany wyłączony.
- **`BeforeParsing` zwraca fałsz.** Jeśli zwróci prawdę (także przez gołe `return;`), biblioteka pomija swoje rozszerzenia, a wśród nich rozpoznawanie słów wpisanych bez ogonków. Gra, która w `BeforeParsing` porównuje słowa ze słownikiem, powinna na początku wywołać `PL_NieznaneSlowa()`, żeby widzieć `'biały'` także wtedy, gdy gracz napisał „bialy”.
- **Słowo w słowniku maszyny Z ma najwyżej 9 z-znaków.** Dłuższe słowa są obcinane, więc „podpowiedź” i „podpowiedz” to dla parsera to samo słowo. Każda polska litera zajmuje w słowniku dwa z-znaki.
- **Bezokoliczniki nowych czasowników.** Bez `PrintVerb` parser pyta „Co chcesz zwiń?” albo, przy dłuższych słowach, „Co chcesz przedziur?”. Każdy nowy czasownik, który może pojawić się w takim pytaniu, powinien mieć bezokolicznik w `PrintVerb` (rozdział 14).
- **Temat rozmowy jest w `second`.** W akcji `Ask` i `Tell` słowo tematu leży w `second`, a postać w `noun`; w `Answer` jest odwrotnie (rozdział 10).
- **Glulx.** Kod, który czyta tablicę `parse` albo `buffer` bezpośrednio (`parse->1`, `parse-->(n*2-1)`, `buffer->1`), działa tylko w maszynie Z. Przenośne są `NumberWords()`, `WordAddress(n)`, `WordLength(n)` i `WordValue(n)`. Tak samo `Lowstring`, `@split_window` i inne instrukcje maszyny Z trzeba otoczyć `#Ifdef TARGET_ZCODE`. Wszystkie polskie gry przykładowe kompilują się dla obu maszyn.

---

## Gdzie szukać dalej

- [Podręcznik](podrecznik.md) – podstawy na przykładzie „Ruin”.
- [Podręcznik gracza](podrecznik_gracza.md) – jak grać i jakie są polecenia.
- `games/*.inf` – pełny kod wszystkich omówionych przykładów. W „Sklepie z zabawkami” każdy obiekt ma znacznik (`>SA`, `>HE`, `>CA`…) odsyłający do spisu w nagłówku pliku, a sale „Muzeum” mają numery odpowiadające rozdziałom podręcznika projektanta (DM4).
