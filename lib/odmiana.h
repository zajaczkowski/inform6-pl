! ==============================================================================
!   ODMIANA:  automatyczna odmiana nazw obiektów przez przypadki
!   (prototyp)
!
!   Copyright Bartosz Zajączkowski 2026
!   Licencja: tradycyjna licencja Inform (DM4) lub Artistic License 2.0.
!   Metoda wzorowana na rosyjskiej bibliotece RInform (Andrey Grankin 2002,
!   Denis Gayev 2003-2004): znacznik "/" między rdzeniem a końcówką
!   w nazwie krótkiej i tabele końcówek.  Ręczne formy desc_* pochodzą
!   z inform-pl 6/11/PL.  Szczegóły w POCHODZENIE.md.
!
!   Dołączany przez polski.h.  Autor zaznacza w nazwie obiektu, gdzie kończy
!   się rdzeń, a biblioteka sama dobiera końcówki przypadków:
!
!       Object lampa "mosiężn/a lamp/a"  has female;
!       Object stol  "st/ó=o/ł/+u"       has male;
!
!   Zapis jednego słowa:  rdzeń / [wymiana /  rdzeń /]... końcówka [+wsk] [!]
!
!     lamp/a          rdzeń "lamp", końcówka mianownika "a"
!     kamyk/          pusta końcówka (rzeczownik męski spółgłoskowy)
!     kłęb/e/k/       "e" występuje tylko w mianowniku (kłębek - kłębka)
!     st/ó=o/ł/       "ó" w mianowniku, "o" w pozostałych przypadkach
!     st/ó=o/ł/+u     dopełniacz męski na -u zamiast -a (stołu)
!     sodow/a!        odmieniaj jak przymiotnik (lampy sodowej, lampą sodową)
!
!   Liczba mnoga (pluralname): zapisuje się mianownik liczby mnogiej, a
!   wskazówka po "+" dotyczy dopełniacza:
!
!     mosiężn/e klucz/e      mosiężnych kluczy, mosiężnymi kluczami
!     kamyk/i                kamyków (męski twardy: -ów)
!     star/e książk/i+e      starych książek (+e, +ie: wstawka przed ostatnią
!     brudn/e okn/a+ie       brudnych okien    spółgłoską w dopełniaczu)
!     noc/e+y                nocy (+y: -y/-i, +ów: -ów, +0: bez końcówki)
!     sta/rz=r/y ryba/c=k//y starych rybaków (męskoosobowy: animate male)
!
!   Domyślny dopełniacz mnogi: rodzaj żeński i nijaki (oraz pusta końcówka:
!   drzwi/) bez końcówki (lamp, pudełek), męski miękki -y/-i (kluczy, kowali),
!   męski twardy -ów (stołów).
!
!   Rzeczownikiem jest ostatnie słowo ze znakiem "/" bez "!"; słowa przed
!   nim są przymiotnikami (zgadzają się z rodzajem obiektu), słowa z "!" też
!   ("lamp/a sodow/a!", "Czarn/a! Królow/a!").  Słowa bez "/" ("półk/a nad
!   kominkiem") nie są odmieniane.
!
!   Rodzaj: female, neuter, w przeciwnym razie męski; animate wpływa na
!   biernik rodzaju męskiego (widzisz kowala, wielkiego psa).  W liczbie
!   mnogiej pluralname + animate + male oznacza rodzaj męskoosobowy
!   (widzisz starych rybaków); dla zwierząt ("psy") nie należy dawać male.
!
!   Właściwości desc_dop ... desc_mie mają pierwszeństwo przed odmianą
!   automatyczną, więc wyjątki ("wielkiemu psu") wpisuje się ręcznie.
!
!   Parser rozpoznaje wszystkie wygenerowane formy (także wpisane bez
!   polskich liter), więc właściwość name potrzebna jest tylko dla synonimów.
!
!   Ograniczenia prototypu: nazwa do PL_ZN_MAX znaków.
! ==============================================================================

System_file;

Constant PL_ZN_MAX 160;
Constant PL_SL_MAX 40;

Array pl_zn  --> PL_ZN_MAX;     ! surowa nazwa obiektu, znak po znaku
Global pl_zn_len;
Array pl_por --> PL_SL_MAX;     ! wygenerowana forma słowa do porównania
Global pl_por_len;
Array pl_tmp --> PL_ZN_MAX;     ! druga nazwa (Identical) albo forma z desc_*
Global pl_tmp_len;

Array pl_s --> PL_SL_MAX;       ! rdzeń przypadków zależnych
Global pl_s_len;
Array pl_n --> PL_SL_MAX;       ! pełna forma mianownika
Global pl_n_len;
Array pl_k --> 8;               ! końcówka mianownika
Global pl_k_len;
Global pl_wskazowka;            ! pierwszy znak wskazówki po "+" ('u', 'ó', 'y', 'e', ...)
Array pl_wsk --> 4;               ! cała wskazówka
Global pl_w_len;
Global pl_przym;                ! '!': odmieniaj jak przymiotnik

Global pl_g1; Global pl_g2; Global pl_g3; Global pl_g4; Global pl_g5; Global pl_g6;

#Ifdef TARGET_ZCODE;
Array pl_bajty -> PL_ZN_MAX + 2;
#Ifnot; ! TARGET_GLULX
Array pl_wynik --> 2;
#Endif; ! TARGET_

[ PL_Znak c;
    #Ifdef TARGET_ZCODE;
    print (char) c;
    #Ifnot; ! TARGET_GLULX
    @streamunichar c;
    #Endif; ! TARGET_
];

! Wywołuje r(a) i zapisuje wydrukowany tekst do tablicy znaków tab.
#Ifdef TARGET_ZCODE;
[ PL_Renderuj tab max r a  i len;
    @output_stream 3 pl_bajty;
    r(a);
    @output_stream -3;
    len = pl_bajty-->0; if (len > max) len = max;
    for (i=0 : i<len : i++) tab-->i = pl_bajty->(i+2);
    return len;
];
#Ifnot; ! TARGET_GLULX
[ PL_Renderuj tab max r a  len str s;
    str = glk_stream_get_current();
    s = glk_stream_open_memory_uni(tab, max, filemode_Write, 0);
    glk_stream_set_current(s);
    r(a);
    glk_stream_set_current(str);
    glk_stream_close(s, pl_wynik);
    len = pl_wynik-->1; if (len > max) len = max;
    return len;
];
#Endif; ! TARGET_

[ PL_Surowa o;
    if (o.&short_name ~= 0) print (string) o.short_name;
    else print (object) o;
];

! Czy nazwa obiektu używa zapisu z "/"?  Przy okazji wczytuje ją do pl_zn.
[ PL_Odmienna o  i;
    pl_zn_len = 0;
    if (~~(o ofclass Object)) rfalse;
    if (o.&short_name ~= 0 && metaclass(o.short_name) ~= String) rfalse;
    pl_zn_len = PL_Renderuj(pl_zn, PL_ZN_MAX, PL_Surowa, o);
    for (i=0 : i<pl_zn_len : i++) if (pl_zn-->i == '/') rtrue;
    rfalse;
];

[ PL_MaUkosnik b e  i;
    for (i=b : i<e : i++) if (pl_zn-->i == '/') rtrue;
    rfalse;
];

! Numer (licząc od 0) słowa ze znakiem "/", które jest rzeczownikiem:
! ostatniego bez "!" ("lamp/a sodow/a!" - rzeczownikiem jest "lampa").
[ PL_Glowa  i j k n g wykrz;
    g = -1;
    while (i < pl_zn_len) {
        if (pl_zn-->i == ' ') { i++; continue; }
        j = i; while (j < pl_zn_len && pl_zn-->j ~= ' ') j++;
        if (PL_MaUkosnik(i, j)) {
            wykrz = false;
            for (k=i : k<j : k++) if (pl_zn-->k == '!') wykrz = true;
            if (~~wykrz) g = n;
            n++;
        }
        i = j;
    }
    if (g < 0) return n-1;
    return g;
];

! Rozbiera słowo pl_zn-->b .. pl_zn-->(e-1) na rdzeń, mianownik i końcówkę.
[ PL_Rozbierz b e  i k c seg alt h;
    pl_s_len = 0; pl_n_len = 0; pl_k_len = 0; pl_w_len = 0; pl_przym = false;
    k = -1; for (i=b : i<e : i++) if (pl_zn-->i == '/') k = i;
    for (i=k+1 : i<e : i++) {
        c = pl_zn-->i;
        if (c == '!') pl_przym = true;
        else if (c == '+') h = true;
        else if (h) { if (pl_w_len < 4) pl_wsk-->(pl_w_len++) = c; }
        else if (pl_k_len < 8) pl_k-->(pl_k_len++) = c;
    }
    pl_wskazowka = 0; if (pl_w_len) pl_wskazowka = pl_wsk-->0;
    for (i=b : i<k : i++) {
        c = pl_zn-->i;
        if (c == '/') { seg = 1 - seg; alt = false; continue; }
        if (seg == 0) {
            pl_s-->(pl_s_len++) = c; pl_n-->(pl_n_len++) = c;
            continue;
        }
        if (c == '=') { alt = true; continue; }
        if (alt) pl_s-->(pl_s_len++) = c;
        else     pl_n-->(pl_n_len++) = c;
    }
    for (i=0 : i<pl_k_len : i++) pl_n-->(pl_n_len++) = pl_k-->i;
];

! Końcówka mianownika typowa dla przymiotnika w rodzaju r?
[ PL_KoncPrzym r  a b;
    a = pl_k-->0; b = pl_k-->1;
    if (pl_k_len == 2 && a ~= 'i') rfalse;
    if (pl_k_len == 2) a = b;
    if (pl_k_len == 0 || pl_k_len > 2) rfalse;
    switch (r) {
      0: if (pl_k_len == 1 && a == 'y' or 'i') rtrue;
      1: if (a == 'a') rtrue;
      2, 3: if (a == 'e') rtrue;
      4: if (pl_k_len == 1 && a == 'y' or 'i') rtrue;
    }
    rfalse;
];

! "-i" na początku końcówki należy do rdzenia (ostatni/a, zdani/e).
[ PL_PrzeniesI  i;
    if (pl_k_len == 0 || pl_k-->0 ~= 'i') return;
    pl_s-->(pl_s_len++) = 'i';
    for (i=1 : i<pl_k_len : i++) pl_k-->(i-1) = pl_k-->i;
    pl_k_len--;
];

[ PL_Ost n; if (pl_s_len < n) return 0; return pl_s-->(pl_s_len-n); ];

[ PL_Tylny  c;     ! rdzeń na k, g, ch
    c = PL_Ost(1);
    if (c == 'k' or 'g') rtrue;
    if (c == 'h' && PL_Ost(2) == 'c') rtrue;
    rfalse;
];

[ PL_Miekki  c d;  ! rdzeń miękki lub stwardniały
    c = PL_Ost(1); d = PL_Ost(2);
    if (c == 'i' or 'j' or 'l' or 'c' or 'ż' or 'ń' or 'ć' or 'ś' or 'ź') rtrue;
    if (c == 'z' && d == 'c' or 's' or 'r' or 'd') rtrue;
    rfalse;
];

[ PL_DrukS n  i; for (i=0 : i<pl_s_len-n : i++) PL_Znak(pl_s-->i); ];
[ PL_DrukN  i;   for (i=0 : i<pl_n_len : i++)   PL_Znak(pl_n-->i); ];

! Rdzeń + "e" z wymianą spółgłoski: lampie, ścianie, stole, półce, lustrze.
[ PL_E  c d;
    c = PL_Ost(1); d = PL_Ost(2);
    if (d == 's' && c == 't') { PL_DrukS(2); print "ście";  return; }
    if (d == 'z' && c == 'd') { PL_DrukS(2); print "ździe"; return; }
    if (d == 's' && c == 'ł') { PL_DrukS(2); print "śle";   return; }
    if (d == 's' && c == 'n') { PL_DrukS(2); print "śnie";  return; }
    if (d == 'z' && c == 'n') { PL_DrukS(2); print "źnie";  return; }
    if (d == 'c' && c == 'h') { PL_DrukS(2); print "sze";   return; }
    switch (c) {
      't': PL_DrukS(1); print "cie";
      'd': PL_DrukS(1); print "dzie";
      'r': PL_DrukS(1); print "rze";
      'ł': PL_DrukS(1); print "le";
      'k': PL_DrukS(1); print "ce";
      'g': PL_DrukS(1); print "dze";
      'n', 's', 'z', 'w', 'm', 'p', 'b', 'f': PL_DrukS(0); print "ie";
      default: PL_DrukS(0); print "e";
    }
];

! Rdzeń + "-y" albo "-i": lampy, półki, kuchni, nocy, soli.
[ PL_YI miekkie  c d;
    c = PL_Ost(1); d = PL_Ost(2);
    PL_DrukS(0);
    ! -ia po n, c, s, z: kuchni, kości; po innych spółgłoskach: biżuterii
    if (c == 'i') { if (d ~= 'n' or 'c' or 's' or 'z' or 'ń' && d ~= 0) print "i"; return; }
    if (c == 'k' or 'g' or 'l' or 'j') { print "i"; return; }
    if (c == 'c' or 'ż' || (c == 'z' && d == 'c' or 's' or 'r' or 'd')) { print "y"; return; }
    if (miekkie) print "i"; else print "y";
];

[ PL_Przymiotnik r c anim  v i;
    if (c == PRZYP_MIAN || (c == PRZYP_BIE && r ~= 1 && ~~(anim && r == 0))) {
        PL_DrukN(); return;
    }
    v = PL_Tylny(); i = (PL_Ost(1) == 'i');
    PL_DrukS(0);
    if (r == 1) {
        if (c == PRZYP_BIE or PRZYP_NAR) { print "ą"; return; }
        if (v) print "i";
        print "ej"; return;
    }
    if (c == PRZYP_NAR or PRZYP_MIE) {
        if (v) print "im"; else if (i) print "m"; else print "ym";
        return;
    }
    if (v) print "i";
    if (c == PRZYP_CEL) print "emu"; else print "ego";
];

[ PL_Rzeczownik r c anim  k;
    if (c == PRZYP_MIAN) { PL_DrukN(); return; }
    if (pl_k_len) k = pl_k-->0;
    if (k == 'a') {                         ! lampa, półka, kuchnia, poeta
        switch (c) {
          PRZYP_DOP: PL_YI(false);
          PRZYP_BIE: PL_DrukS(0); print "ę";
          PRZYP_NAR: PL_DrukS(0); print "ą";
          default:   if (PL_Miekki()) PL_YI(false); else PL_E();
        }
        return;
    }
    if (r == 1) {                           ! noc, mysz, sól, kość
        switch (c) {
          PRZYP_BIE: PL_DrukN();
          PRZYP_NAR: PL_DrukS(0); print "ą";
          default:   PL_YI(true);
        }
        return;
    }
    switch (c) {                            ! kamyk, stół, kowal; pudełko, pole
      PRZYP_DOP:
        PL_DrukS(0);
        if (r == 0 && pl_wskazowka == 'u' && ~~anim) print "u"; else print "a";
      PRZYP_CEL:
        PL_DrukS(0);
        if (r == 0) print "owi"; else print "u";
      PRZYP_BIE:
        if (r == 0 && anim) { PL_DrukS(0); print "a"; }
        else PL_DrukN();
      PRZYP_NAR:
        PL_DrukS(0);
        if (PL_Ost(1) == 'k' or 'g') print "iem"; else print "em";   ! kamykiem, łańcuchem
      PRZYP_MIE:
        if (PL_Tylny() || PL_Miekki()) { PL_DrukS(0); print "u"; }
        else PL_E();
    }
];

! Liczba mnoga przymiotnika: mosiężnych, wielkim, ostatnimi, starych.
[ PL_PrzymiotnikMn c wir  v i;
    if (c == PRZYP_MIAN || (c == PRZYP_BIE && ~~wir)) { PL_DrukN(); return; }
    v = PL_Tylny(); i = (PL_Ost(1) == 'i');
    PL_DrukS(0);
    if (v) print "i"; else if (~~i) print "y";
    switch (c) {
      PRZYP_CEL: print "m";
      PRZYP_NAR: print "mi";
      default:   print "ch";
    }
];

! Dopełniacz liczby mnogiej rzeczownika (także biernik męskoosobowy).
[ PL_DopMn zen  typ;
    switch (pl_wskazowka) {
      'ó':      typ = 1;
      'y':      typ = 2;
      '0', 'e', 'i': typ = 0;
      default:
        if (pl_k_len == 0 || pl_k-->0 == 'a' || zen) typ = 0;
        else if (PL_Miekki()) typ = 2;
        else typ = 1;
    }
    switch (typ) {
      0:
        if (pl_wskazowka == 'e' or 'i' && pl_s_len > 1) {
            PL_DrukS(1);
            if (pl_wskazowka == 'i') print "ie"; else print "e";
            PL_Znak(PL_Ost(1));
        }
        else PL_DrukS(0);
      1: PL_DrukS(0); print "ów";
      2: PL_YI(true);
    }
];

! Liczba mnoga rzeczownika: lamp/lampom/lampami/lampach, kluczy, rybaków.
[ PL_RzeczownikMn c wir zen;
    if (c == PRZYP_MIAN || (c == PRZYP_BIE && ~~wir)) { PL_DrukN(); return; }
    switch (c) {
      PRZYP_CEL: PL_DrukS(0); print "om";
      PRZYP_NAR: PL_DrukS(0); print "ami";
      PRZYP_MIE: PL_DrukS(0); print "ach";
      default:   PL_DopMn(zen);
    }
];

! Drukuje jedno słowo (pl_zn-->b .. e-1) w przypadku c.
[ PL_Slowo b e c ostatnie r anim  przym zen;
    zen = (anim & 2 ~= 0); anim = anim & 1;
    PL_Rozbierz(b, e);
    przym = pl_przym || ((~~ostatnie) && PL_KoncPrzym(r));
    if (przym || pl_k_len > 1) PL_PrzeniesI();
    if (r >= 3) {
        if (przym) PL_PrzymiotnikMn(c, r == 4);
        else       PL_RzeczownikMn(c, r == 4, zen);
        return;
    }
    if (przym) PL_Przymiotnik(r, c, anim);
    else       PL_Rzeczownik(r, c, anim);
];

[ PL_SlowoG; PL_Slowo(pl_g1, pl_g2, pl_g3, pl_g4, pl_g5, pl_g6); ];

! 0 męski, 1 żeński, 2 nijaki, 3 mnogi niemęskoosobowy, 4 męskoosobowy.
[ PL_Rodzaj o;
    if (o has pluralname) {
        if (o has animate && o has male) return 4;
        return 3;
    }
    if (o has female) return 1;
    if (o has neuter) return 2;
    return 0;
];

! Drukuje nazwę wczytaną przez PL_Odmienna(o) w przypadku c.
[ PL_DrukujOdmiane o c  i j k n idx r anim;
    r = PL_Rodzaj(o); anim = (o has animate) + 2*(o has female);   ! bity: 1 żywotny, 2 żeński
    n = PL_Glowa();
    while (i < pl_zn_len) {
        if (pl_zn-->i == ' ') { print " "; i++; continue; }
        j = i; while (j < pl_zn_len && pl_zn-->j ~= ' ') j++;
        if (PL_MaUkosnik(i, j)) {
            PL_Slowo(i, j, c, idx == n, r, anim);
            idx++;
        }
        else for (k=i : k<j : k++) PL_Znak(pl_zn-->k);
        i = j;
    }
];

[ PL_Porownaj adr len  i;
    if (len ~= pl_por_len) rfalse;
    for (i=0 : i<len : i++)
        if (BezOgonka(LowerCase(adr->i)) ~= BezOgonka(LowerCase(pl_por-->i))) rfalse;
    rtrue;
];

! Czy słowo adr/len występuje w tablicy pl_tmp (słowa oddzielone spacjami)?
[ PL_WTmp adr len  i j k;
    while (i < pl_tmp_len) {
        if (pl_tmp-->i == ' ') { i++; continue; }
        j = i; while (j < pl_tmp_len && pl_tmp-->j ~= ' ') j++;
        pl_por_len = j-i;
        if (pl_por_len > PL_SL_MAX) pl_por_len = PL_SL_MAX;
        for (k=0 : k<pl_por_len : k++) pl_por-->k = pl_tmp-->(i+k);
        if (PL_Porownaj(adr, len)) rtrue;
        i = j;
    }
    rfalse;
];

[ PL_Desc o; PrintOrRun(o, pl_g1, true); ];

! Czy słowo wnum to jakaś forma któregoś słowa z nazwy wczytanej przez
! PL_Odmienna(o), albo słowo z ręcznej formy desc_*?  Słowa nieodmienne
! ("wełny", "nad kominkiem") pasują w niezmienionej postaci.
[ PL_Pasuje o wnum  adr len i j n idx c r anim p;
    adr = WordAddress(wnum); len = WordLength(wnum);
    r = PL_Rodzaj(o); anim = (o has animate) + 2*(o has female);   ! bity: 1 żywotny, 2 żeński
    n = PL_Glowa();
    while (i < pl_zn_len) {
        if (pl_zn-->i == ' ') { i++; continue; }
        j = i; while (j < pl_zn_len && pl_zn-->j ~= ' ') j++;
        if (PL_MaUkosnik(i, j)) {
            pl_g1 = i; pl_g2 = j; pl_g4 = (idx == n); pl_g5 = r; pl_g6 = anim;
            for (c=PRZYP_MIAN : c<=PRZYP_MIE : c++) {
                pl_g3 = c;
                pl_por_len = PL_Renderuj(pl_por, PL_SL_MAX, PL_SlowoG);
                if (PL_Porownaj(adr, len)) rtrue;
            }
            idx++;
        }
        else {
            pl_por_len = j-i;
            if (pl_por_len > PL_SL_MAX) pl_por_len = PL_SL_MAX;
            for (p=0 : p<pl_por_len : p++) pl_por-->p = pl_zn-->(i+p);
            ! Krótkie słowo nieodmienne ("do", "nad", "z") pasuje tylko wtedy,
            ! gdy następne wpisane słowo też należy do nazwy: "piłeczkę do
            ! żonglowania" tak, ale w "włóż piłeczkę do pudełka" "do" to
            ! przyimek polecenia.
            if (PL_Porownaj(adr, len)) {
                if (j-i > 3) rtrue;
                if (wnum < NumberWords() && PL_Pasuje(o, wnum+1)) rtrue;
            }
        }
        i = j;
    }
    for (c=PRZYP_DOP : c<=PRZYP_MIE : c++) {
        switch (c) {
          PRZYP_DOP: p = desc_dop;
          PRZYP_CEL: p = desc_cel;
          PRZYP_BIE: p = desc_bie;
          PRZYP_NAR: p = desc_nar;
          PRZYP_MIE: p = desc_mie;
        }
        if (o provides p && metaclass(o.p) == String) {
            pl_g1 = p;
            pl_tmp_len = PL_Renderuj(pl_tmp, PL_ZN_MAX, PL_Desc, o);
            if (PL_WTmp(adr, len)) rtrue;
        }
    }
    rfalse;
];

[ PL_Adres; print (address) pl_g1; ];

! Czy słowo spoza słownika (wnum) to bez ogonków któreś ze słów właściwości
! prop ("miekki" pasuje do 'miękki')?  Słowa słownika są obcięte, więc dłuższe
! wpisane słowo pasuje też, jeśli zaczyna się od całego słowa słownika
! mającego co najmniej 6 liter.
[ PL_WSlowach obj prop wnum  adr len k n m i;
    if (~~(obj provides prop)) rfalse;
    adr = WordAddress(wnum); len = WordLength(wnum);
    k = obj.&prop; n = (obj.#prop)/WORDSIZE;
    for (m=0 : m<n : m++) {
        pl_g1 = k-->m;
        if (pl_g1 == 0) continue;
        pl_por_len = PL_Renderuj(pl_por, PL_SL_MAX, PL_Adres);
        if (PL_Porownaj(adr, len)) rtrue;
        if (pl_por_len >= 6 && len > pl_por_len) {
            for (i=0 : i<pl_por_len : i++)
                if (BezOgonka(LowerCase(adr->i)) ~= BezOgonka(LowerCase(pl_por-->i))) break;
            if (i == pl_por_len) rtrue;
        }
    }
    rfalse;
];

! Obiekty bez właściwości name, rozróżniane tylko przez odmienianą nazwę,
! parser uznałby za identyczne ("Best guess"), więc porównujemy też nazwy.
! Przy "Replace X Y" wygrywa ostatnia definicja X, dlatego samo Identical
! jest zdefiniowane w polskig.h (po parser.h): najpierw woła PL_Identical,
! a potem oryginalne Identical biblioteki (PL_IdenticalOryg).  Wywołania
! PL_IdenticalOryg nie może być tutaj: odwołanie w przód do procedury
! przemianowanej przez Replace kompilator zamienia na 0.
Replace Identical PL_IdenticalOryg;
Replace Locale PL_LocaleOryg;      ! nowa wersja w polskig.h

! Fałsz, gdy obiekty mają różne nazwy zapisane z "/"; prawda w pozostałych
! przypadkach (wtedy rozstrzyga oryginalne Identical).
[ PL_Identical o1 o2  i;
    if (o1 ~= o2 && o1 ofclass Object && o2 ofclass Object) {
        i = PL_Odmienna(o2);
        pl_tmp_len = pl_zn_len;
        for (i=0 : i<pl_zn_len : i++) pl_tmp-->i = pl_zn-->i;
        if (PL_Odmienna(o1) || PL_MaUkosnikTmp()) {
            if (pl_zn_len ~= pl_tmp_len) rfalse;
            for (i=0 : i<pl_zn_len : i++) if (pl_zn-->i ~= pl_tmp-->i) rfalse;
        }
    }
    rtrue;
];

[ PL_MaUkosnikTmp  i;
    for (i=0 : i<pl_tmp_len : i++) if (pl_tmp-->i == '/') rtrue;
    rfalse;
];

! Odmienione formy nazw zwykle nie są w słowniku.  Dla słowa o wartości 0
! parser 6.12 czyta flagi słownikowe spod adresu 0 i dostaje przypadkowe bity
! (np. liczbę mnogą: "Tego czasownika nie można użyć z kilkoma przedmiotami"),
! więc takie słowa dostają neutralną wartość PL_NIEZNANE.  Wywoływane
! z rozszerzenia biblioteki w polskig.h, po tokenizacji polecenia.
Constant PL_NIEZNANE = 'xxnieznane';

[ PL_NieznaneSlowa  i;
    for (i=1 : i<=NumberWords() : i++) {
        #Ifdef TARGET_ZCODE;
        if (parse-->(i*2-1) == 0) parse-->(i*2-1) = PL_NIEZNANE;
        #Ifnot; ! TARGET_GLULX
        if (parse-->(i*3-2) == 0) parse-->(i*3-2) = PL_NIEZNANE;
        #Endif; ! TARGET_
    }
    rfalse;
];
