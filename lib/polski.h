! ==============================================================================
!   POLSKI:  Plik definicji języka dla Inform 6
!
!   Polska wersja biblioteki standardowej Inform 6.12.8
!   Copyright Bartosz Zajączkowski 2011-2026
!   Kontynuacja inform-pl 6/11/PL; pochodzenie poszczególnych części
!   i wykorzystanie AI opisuje POCHODZENIE.md.
!
!   Oparte na pliku english.h z biblioteki 6.12.8
!   Copyright Graham Nelson 1993-2004 and David Griffith 2012-2025
!   Licencja: tradycyjna licencja Inform (DM4) lub Artistic License 2.0.
!
!   Plik w kodowaniu UTF-8 (kompilować z opcją -Cu).
!   Wybierany opcją kompilatora +language_name=polski
!
!   Ograniczenia tej wersji:
!   - komunikaty są pisane w 2. osobie i czasie teraźniejszym; narrative_voice
!     i narrative_tense nie są (jeszcze) obsługiwane,
!   - przymiotniki w rodzaju męskoosobowym (np. "zamknięci") nie są odmieniane.
! ==============================================================================

System_file;

#Ifndef LIBRARY_ENGLISH;    ! nazwa stałej pozostaje ze względu na zależności
                            ! w bibliotece standardowej

! ------------------------------------------------------------------------------
!   Część I.   Wstęp
! ------------------------------------------------------------------------------

! Bez EnglishNaturalLanguage: stary mechanizm zaimków biblioteki ustawia
! angielskie "it/him/her", których nie ma w polskiej tablicy zaimków
! (Błąd biblioteki 12).  Polskie zaimki obsługuje LanguagePronouns.

#Ifndef POLSKI_ZNAKI;
Message fatalerror "Na samym początku gry, przed Constant Story, dołącz plik biblioteki PolskiZ (Include z nazwą PolskiZ).";
#Endif;

! ------------------------------------------------------------------------------
!   Odmiana nazw obiektów przez przypadki
!
!   Każdy obiekt może podać nazwę w każdym przypadku. Własności desc_* służą
!   do wyświetlania, a name_* do rozpoznawania słów wpisanych przez gracza
!   (słowa z name_* są rozpoznawane niezależnie od przypadku, tak jak name):
!
!   Object -> miecz "żelazny miecz"
!     with name     'żelazny' 'miecz',
!          name_dop 'żelaznego' 'miecza',
!          name_cel 'żelaznemu' 'mieczowi',
!          name_nar 'żelaznym' 'mieczem',
!          name_mie 'mieczu',
!          desc_dop "żelaznego miecza",
!          desc_cel "żelaznemu mieczowi",
!          desc_bie "żelazny miecz",
!          desc_nar "żelaznym mieczem",
!          desc_mie "żelaznym mieczu";
!
!   Brakująca forma jest zastępowana mianownikiem (short_name lub nazwą
!   obiektu). Słowa można też po prostu dopisać do name.
!
!   W komunikatach: (mian) (dop) (cel) (bie) (nar) (mie) obj, a z wielkiej
!   litery: (CMian) (CDop) (CCel) (CBie) (CNar) (CMie) obj.
! ------------------------------------------------------------------------------

Property individual name_dop;
Property individual name_cel;
Property individual name_bie;
Property individual name_nar;
Property individual name_mie;

Property individual desc_dop;
Property individual desc_cel;
Property individual desc_bie;
Property individual desc_nar;
Property individual desc_mie;

Constant PRZYP_MIAN 0;      ! kto? co?
Constant PRZYP_DOP  1;      ! kogo? czego?
Constant PRZYP_CEL  2;      ! komu? czemu?
Constant PRZYP_BIE  3;      ! kogo? co?
Constant PRZYP_NAR  4;      ! (z) kim? (z) czym?
Constant PRZYP_MIE  5;      ! (o) kim? (o) czym?

Constant LanguageCases = 6;

! Obiekty z tym atrybutem dostają przyimek "ze"/"we" zamiast "z"/"w"
! (np. "ze stołu", "we wraku").
Attribute z_article;
Attribute w_article;

! ------------------------------------------------------------------------------
! Kierunki świata
! ------------------------------------------------------------------------------

Class   CompassDirection
  with  number 0, article "",
        description [;
            if (location provides compass_look && location.compass_look(self)) rtrue;
            if (self.compass_look()) rtrue;
            L__M(##Look, 7, self);
        ],
        compass_look false,
        parse_name [; return -1; ]
  has   scenery;

Object Compass "kompas" has concealed;

#Ifndef WITHOUT_DIRECTIONS;
CompassDirection -> n_obj  with short_name "północ",  door_dir n_to,
                                name 'pn' 'północ' 'północy',
                           has female;
CompassDirection -> s_obj  with short_name "południe", door_dir s_to,
                                name 'pd' 'południe' 'południa',
                           has neuter;
CompassDirection -> e_obj  with short_name "wschód",  door_dir e_to,
                                name 'w//' 'wsch' 'wschód' 'wschodu';
CompassDirection -> w_obj  with short_name "zachód",  door_dir w_to,
                                name 'z//' 'zach' 'zachód' 'zachodu';
CompassDirection -> ne_obj with short_name "północny wschód", door_dir ne_to,
                                name 'pnw' 'północny' 'północnego' 'wschód' 'wschodu';
CompassDirection -> nw_obj with short_name "północny zachód", door_dir nw_to,
                                name 'pnz' 'północny' 'północnego' 'zachód' 'zachodu';
CompassDirection -> se_obj with short_name "południowy wschód", door_dir se_to,
                                name 'pdw' 'południowy' 'południowego' 'wschód' 'wschodu';
CompassDirection -> sw_obj with short_name "południowy zachód", door_dir sw_to,
                                name 'pdz' 'południowy' 'południowego' 'zachód' 'zachodu';
CompassDirection -> u_obj  with short_name "góra",    door_dir u_to,
                                name 'g//' 'góra' 'górę' 'góry' 'sufit' 'niebo',
                           has female;
CompassDirection -> d_obj  with short_name "dół",     door_dir d_to,
                                name 'd//' 'dół' 'podłoga' 'podłogę' 'ziemia' 'ziemię';
#Endif; ! WITHOUT_DIRECTIONS

CompassDirection -> in_obj  with short_name "środek", door_dir in_to,
                                 name 'środek' 'środka' 'wnętrze' 'wnętrza';
CompassDirection -> out_obj with short_name "zewnątrz", door_dir out_to,
                                 name 'zewnątrz' 'zewnatrz' 'wyjście';

! ------------------------------------------------------------------------------
!   Część II.   Słownictwo
! ------------------------------------------------------------------------------

Constant AGAIN1__WD     = 'powtórz';
Constant AGAIN2__WD     = 'p//';
Constant AGAIN3__WD     = 'again';
Constant OOPS1__WD      = 'ups';
Constant OOPS2__WD      = 'o//';
Constant OOPS3__WD      = 'oops';
Constant UNDO1__WD      = 'cofnij';
Constant UNDO2__WD      = 'undo';
Constant UNDO3__WD      = 'cofnij';

Constant ALL1__WD       = 'wszystko';
Constant ALL2__WD       = 'wszystkie';
Constant ALL3__WD       = 'wszystkich';
Constant ALL4__WD       = 'każdy';
Constant ALL5__WD       = 'obie';
Constant AND1__WD       = 'i//';
Constant AND2__WD       = 'oraz';
Constant AND3__WD       = 'a//';
Constant BUT1__WD       = 'oprócz';
Constant BUT2__WD       = 'poza';
Constant BUT3__WD       = 'wyjąwszy';
Constant ME1__WD        = 'mnie';
Constant ME2__WD        = 'siebie';
Constant ME3__WD        = 'sobie';
Constant OF1__WD        = 'z//';
Constant OF2__WD        = 'ze';
Constant OF3__WD        = 'spośród';
Constant OF4__WD        = 'spośród';
Constant OTHER1__WD     = 'inny';
Constant OTHER2__WD     = 'inna';
Constant OTHER3__WD     = 'inne';
Constant THEN1__WD      = 'potem';
Constant THEN2__WD      = 'następnie';
Constant THEN3__WD      = 'później';

Constant NO1__WD        = 'n//';
Constant NO2__WD        = 'nie';
Constant NO3__WD        = 'no';
Constant YES1__WD       = 't//';
Constant YES2__WD       = 'tak';
Constant YES3__WD       = 'yes';

Constant AMUSING__WD    = 'zabawne';
Constant FULLSCORE1__WD = 'pełny';
Constant FULLSCORE2__WD = 'wynik';
Constant QUIT1__WD      = 'q//';
Constant QUIT2__WD      = 'koniec';
Constant RESTART__WD    = 'restart';
Constant RESTORE__WD    = 'wczytaj';

Array LanguagePronouns table

  ! word        possible GNAs                   connected
  !             to follow:                      to:
  !             a     i
  !             s  p  s  p
  !             mfnmfnmfnmfn

    'go'      $$101000101000                    NULL    ! weź go
    'jego'    $$101000101000                    NULL
    'mu'      $$101000101000                    NULL
    'nim'     $$101000101000                    NULL    ! połóż na nim
    'ją'      $$010000010000                    NULL    ! otwórz ją
    'jej'     $$010000010000                    NULL
    'nią'     $$010000010000                    NULL
    'niej'    $$010000010000                    NULL
    'je'      $$001011001111                    NULL    ! weź je
    'ich'     $$000111000111                    NULL
    'im'      $$000111000111                    NULL
    'nich'    $$000111000111                    NULL
    'to'      $$111111111111                    NULL;   ! cokolwiek ostatnio

Array LanguageDescriptors table

  ! word        possible GNAs   descriptor      connected
  !             to follow:      type:           to:
  !             a     i
  !             s  p  s  p
  !             mfnmfnmfnmfn

    'mój'       $$100000100000  POSSESS_PK      0
    'moja'      $$010000010000  POSSESS_PK      0
    'moje'      $$001111001111  POSSESS_PK      0
    'moją'      $$010000010000  POSSESS_PK      0
    'mojego'    $$101000101000  POSSESS_PK      0
    'moich'     $$000111000111  POSSESS_PK      0
    'ten'       $$100000100000  POSSESS_PK      0
    'ta'        $$010000010000  POSSESS_PK      0
    'tę'        $$010000010000  POSSESS_PK      0
    'te'        $$000111000111  POSSESS_PK      0
    'tego'      $$101000101000  POSSESS_PK      0
    'tamten'    $$100000100000  POSSESS_PK      1
    'tamta'     $$010000010000  POSSESS_PK      1
    'tamto'     $$001000001000  POSSESS_PK      1
    'tamte'     $$000111000111  POSSESS_PK      1
    'zapalony'  $$100000100000  light           NULL
    'zapalona'  $$010000010000  light           NULL
    'zapalone'  $$001111001111  light           NULL
    'zgaszony'  $$100000100000  (-light)        NULL
    'zgaszona'  $$010000010000  (-light)        NULL
    'zgaszone'  $$001111001111  (-light)        NULL;

Array LanguageNumbers table
    'jeden' 1 'jedna' 1 'jedno' 1 'jedną' 1 'jednego' 1
    'dwa' 2 'dwie' 2 'dwóch' 2 'trzy' 3 'trzech' 3 'cztery' 4 'czterech' 4
    'pięć' 5 'sześć' 6 'siedem' 7 'osiem' 8 'dziewięć' 9 'dziesięć' 10
    'jedenaście' 11 'dwanaście' 12 'trzynaście' 13 'czternaście' 14
    'piętnaście' 15 'szesnaście' 16 'siedemnaście' 17 'osiemnaście' 18
    'dziewiętnaście' 19 'dwadzieścia' 20;

! ------------------------------------------------------------------------------
!   Część III.   Przekład polecenia
! ------------------------------------------------------------------------------

! Polska litera zamieniona na literę bez ogonka (ą -> a, ł -> l, ...).
[ BezOgonka c;
    switch (c) {
      'ą', 'Ą': return 'a';
      'ć', 'Ć': return 'c';
      'ę', 'Ę': return 'e';
      'ł', 'Ł': return 'l';
      'ń', 'Ń': return 'n';
      'ó', 'Ó': return 'o';
      'ś', 'Ś': return 's';
      'ź', 'Ź', 'ż', 'Ż': return 'z';
    }
    return c;
];

! Czy pozycja at w buffer leży wewnątrz tekstu w cudzysłowie (napisz "...")?
[ PL_WCudzyslowie at  i n;
    for (i=2 : i<at : i++) if (buffer->i == '"') n++;
    return n % 2;
];

! Czy słowo słownika w jest nazwą kierunku (północ, pn, górę, dół...)?
[ PL_Kierunek w o;
    if (w == 0) rfalse;
    objectloop (o in Compass) if (WordInProperty(w, o, name)) rtrue;
    rfalse;
];

! Przed analizą polecenia:
! - usuwa słowo "się" ("rozejrzyj się", "napij się wody", "połóż się na
!   łóżku"), dzięki czemu czasowniki zwrotne nie wymagają osobnej gramatyki,
! - zamienia "północny-wschód" na "północny wschód" (itd.), żeby parser widział
!   dwa osobne słowa,
! - słowa, których nie ma w słowniku, próbuje rozpoznać bez polskich liter
!   ("królowa" pasuje do 'krolowa' w starszych grach),
! - przywraca mianownik jako bieżący przypadek nazw.
[ LanguageToInformese i j start len at wlen;
    short_name_case = PRZYP_MIAN;
    ListaWPrzypadku(0);
    #Ifdef TARGET_ZCODE;
    ! "na północ", "w górę", "do góry", "na dół": sam kierunek bez czasownika
    if (parse->1 >= 2 && parse-->1 == 'na' or 'w//' or 'do' &&
        (PL_Kierunek(parse-->3) || (parse-->3 == 0 && PL_Kierunek(PL_SlowoBezOgonkow(2)))))
        for (j=0 : j<parse->4 : j++) buffer->(parse->5 + j) = ' ';
    for (i=0 : i<parse->1 : i++) {
        wlen = parse->(i*4+4); at = parse->(i*4+5);
        ! "weź grzyb, potem zjedz go": przecinek przed "potem" nie łączy przedmiotów
        if (i > 0 && parse-->(i*2+1) == THEN1__WD or THEN2__WD or THEN3__WD
            && buffer->(parse->(i*4+1)) == ',')
            buffer->(parse->(i*4+1)) = ' ';
        ! "połóż się" to leżenie ("kładź się"), a nie kładzenie czegoś
        if (i > 0 && parse-->(i*2+1) == 'się' or 'sie' && parse->(i*4) == 5
            && (parse-->(i*2-1) == 'połóż' || PL_SlowoBezOgonkow(i) == 'połóż')) {
            j = parse->(i*4+1);
            buffer->j = 'k'; buffer->(j+1) = 'ł'; buffer->(j+2) = 'a';
            buffer->(j+3) = 'd'; buffer->(j+4) = 'ź';
        }
        if (parse-->(i*2+1) == 'się' or 'sie' && ~~PL_WCudzyslowie(at))
            for (j=0 : j<wlen : j++) buffer->(at+j) = ' ';
        if (parse-->(i*2+1) == 0 && ~~PL_WCudzyslowie(at))  ! spoza słownika: bez ogonków
            for (j=0 : j<wlen : j++) buffer->(at+j) = BezOgonka(buffer->(at+j));
    }
    start = 2; len = buffer->1 + 2;
    #Ifnot; ! TARGET_GLULX
    if (parse-->0 >= 2 && parse-->1 == 'na' or 'w//' or 'do' &&
        (PL_Kierunek(parse-->4) || (parse-->4 == 0 && PL_Kierunek(PL_SlowoBezOgonkow(2)))))
        for (j=0 : j<parse-->2 : j++) buffer->(parse-->3 + j) = ' ';
    for (i=1 : i<parse-->0 : i++)
        if (parse-->(i*3+1) == THEN1__WD or THEN2__WD or THEN3__WD
            && buffer->(parse-->(i*3)) == ',')
            buffer->(parse-->(i*3)) = ' ';
    for (i=0 : i<parse-->0 : i++)
        if (parse-->(i*3+1) == 'się' or 'sie') {
            wlen = parse-->(i*3+2); at = parse-->(i*3+3);
            for (j=0 : j<wlen : j++) buffer->(at+j) = ' ';
        }
    start = WORDSIZE; len = buffer-->0 + WORDSIZE;
    #Endif; ! TARGET_
    for (i=start+1 : i<len : i++)
        if (buffer->i == '-' && buffer->(i-1) == 'y') buffer->i = ' ';
];

! ------------------------------------------------------------------------------
!   Część IV.   Wypisywanie
! ------------------------------------------------------------------------------

Constant LanguageAnimateGender   = male;
Constant LanguageInanimateGender = male;

Constant LanguageContractionForms = 1;     ! w języku polskim nie ma rodzajników

[ LanguageContraction text;
    text = 0;
    return 0;
];

! Puste "rodzajniki" dla każdego z 6 przypadków, liczba pojedyncza i mnoga.
Array LanguageArticles -->
 !   Cdef   Def    Indef
     ""     ""     ""           ! mianownik, l. poj.
     ""     ""     ""           ! dopełniacz
     ""     ""     ""           ! celownik
     ""     ""     ""           ! biernik
     ""     ""     ""           ! narzędnik
     ""     ""     ""           ! miejscownik
     ""     ""     ""           ! mianownik, l. mn.
     ""     ""     ""
     ""     ""     ""
     ""     ""     ""
     ""     ""     ""
     ""     ""     "";

                   !             a           i
                   !             s     p     s     p
                   !             m f n m f n m f n m f n

Array LanguageGNAsToArticles --> 0 0 0 1 1 1 0 0 0 1 1 1;

! Listy przedmiotów ("Widzisz tutaj mosiężną lampę") są drukowane w innym
! przypadku niż mianownik, ale tylko bezpośrednia zawartość pl_lista_rodzic;
! przedmioty zagnieżdżone ("stół (na którym jest lampa)") zostają w mianowniku.
Global pl_lista_rodzic;
Global pl_lista_przyp;

[ ListaWPrzypadku rodzic c;
    pl_lista_rodzic = rodzic; pl_lista_przyp = c;
];

Include "odmiana";

! Wyświetla nazwę obiektu w bieżącym przypadku (short_name_case): najpierw
! ręczna forma z desc_*, potem automatyczna odmiana nazwy zapisanej z "/".
[ LanguagePrintShortName o p c;
    c = short_name_case;
    if (c == PRZYP_MIAN && pl_lista_rodzic && o ofclass Object && parent(o) == pl_lista_rodzic)
        c = pl_lista_przyp;
    switch (c) {
      PRZYP_DOP: p = desc_dop;
      PRZYP_CEL: p = desc_cel;
      PRZYP_BIE: p = desc_bie;
      PRZYP_NAR: p = desc_nar;
      PRZYP_MIE: p = desc_mie;
    }
    if (p && o provides p && PrintOrRun(o, p, 1) ~= 0) rtrue;
    if (PL_Odmienna(o)) { PL_DrukujOdmiane(o, c); rtrue; }
    rfalse;
];

! Do obiektu pasują: formy nazwy zapisanej z "/" oraz słowa z name_dop,
! name_cel itd. (tak samo jak name); słowa spoza słownika porównywane są
! z name i name_* bez ogonków.
[ LanguageRefers obj wnum  wd k;
    if (PL_Odmienna(obj) && PL_Pasuje(obj, wnum)) rtrue;
    k = wn; wn = wnum; wd = NextWordStopped(); wn = k;
    if (obj provides name_dop && WordInProperty(wd, obj, name_dop)) rtrue;
    if (obj provides name_cel && WordInProperty(wd, obj, name_cel)) rtrue;
    if (obj provides name_bie && WordInProperty(wd, obj, name_bie)) rtrue;
    if (obj provides name_nar && WordInProperty(wd, obj, name_nar)) rtrue;
    if (obj provides name_mie && WordInProperty(wd, obj, name_mie)) rtrue;
    if (wd == 0 or PL_NIEZNANE || (wnum < 64 && pl_podmienione->wnum)) {
        ! słowo spoza słownika: porównaj bez ogonków
        if (PL_WSlowach(obj, name, wnum)) rtrue;
        if (PL_WSlowach(obj, name_dop, wnum) || PL_WSlowach(obj, name_cel, wnum) ||
            PL_WSlowach(obj, name_bie, wnum) || PL_WSlowach(obj, name_nar, wnum) ||
            PL_WSlowach(obj, name_mie, wnum)) rtrue;
    }
    return -1;
];

! Gracz w poszczególnych przypadkach: ty, siebie, sobie, siebie, sobą, sobie.
[ GraczWPrzypadku c caps;
    switch (c) {
      PRZYP_MIAN:           if (caps) print "Ty";     else print "ty";
      PRZYP_DOP, PRZYP_BIE: if (caps) print "Siebie"; else print "siebie";
      PRZYP_NAR:            if (caps) print "Sobą";   else print "sobą";
      default:              if (caps) print "Sobie";  else print "sobie";
    }
];

[ NazwaWPrzypadku o c  old;
    if (o == player) { GraczWPrzypadku(c); return; }
    old = short_name_case; short_name_case = c;
    print (the) o;
    short_name_case = old;
];

[ CNazwaWPrzypadku o c  old;
    if (o == player) { GraczWPrzypadku(c, true); return; }
    if (~~(o ofclass Object)) { print (the) o; return; }
    old = short_name_case; short_name_case = c;
    PrintToBuffer(StorageForShortName, SHORTNAMEBUF_LEN, PSN__, o);
    short_name_case = old;
    PrintFromBuffer(StorageForShortName, true, true);
];

[ mian o; NazwaWPrzypadku(o, PRZYP_MIAN); ];
[ dop o;  NazwaWPrzypadku(o, PRZYP_DOP);  ];
[ cel o;  NazwaWPrzypadku(o, PRZYP_CEL);  ];
[ bie o;  NazwaWPrzypadku(o, PRZYP_BIE);  ];
[ nar o;  NazwaWPrzypadku(o, PRZYP_NAR);  ];
[ mie o;  NazwaWPrzypadku(o, PRZYP_MIE);  ];

[ CMian o; CNazwaWPrzypadku(o, PRZYP_MIAN); ];
[ CDop o;  CNazwaWPrzypadku(o, PRZYP_DOP);  ];
[ CCel o;  CNazwaWPrzypadku(o, PRZYP_CEL);  ];
[ CBie o;  CNazwaWPrzypadku(o, PRZYP_BIE);  ];
[ CNar o;  CNazwaWPrzypadku(o, PRZYP_NAR);  ];
[ CMie o;  CNazwaWPrzypadku(o, PRZYP_MIE);  ];

[ Cname o; CMian(o); ];     ! zgodność z wersją 6/11/PL

! ------------------------------------------------------------------------------

[ LanguageDirection d;
    switch (d) {
      n_to:    print "na północ";
      s_to:    print "na południe";
      e_to:    print "na wschód";
      w_to:    print "na zachód";
      ne_to:   print "na północny wschód";
      nw_to:   print "na północny zachód";
      se_to:   print "na południowy wschód";
      sw_to:   print "na południowy zachód";
      u_to:    print "w górę";
      d_to:    print "w dół";
      in_to:   print "do środka";
      out_to:  print "na zewnątrz";
      default: return RunTimeError(9,d);
    }
];

! ------------------------------------------------------------------------------

[ LanguageNumber n f;
    if (n == 0)    { print "zero"; rfalse; }
    if (n < 0)     { print "minus "; n = -n; }
    if (n >= 1000) {
        f = n/1000;
        if (f == 1) print "tysiąc";
        else {
            LanguageNumber(f);
            Liczebnik(f, "", " tysiące", " tysięcy");
        }
        n = n%1000; f = 1;
    }
    if (n >= 100)  {
        if (f == 1) print " ";
        switch (n/100) {
          1:  print "sto";
          2:  print "dwieście";
          3:  print "trzysta";
          4:  print "czterysta";
          5:  print "pięćset";
          6:  print "sześćset";
          7:  print "siedemset";
          8:  print "osiemset";
          9:  print "dziewięćset";
        }
        n = n%100; f = 1;
    }
    if (n == 0) rfalse;
    if (f == 1) print " ";
    switch (n) {
      1:    print "jeden";
      2:    print "dwa";
      3:    print "trzy";
      4:    print "cztery";
      5:    print "pięć";
      6:    print "sześć";
      7:    print "siedem";
      8:    print "osiem";
      9:    print "dziewięć";
      10:   print "dziesięć";
      11:   print "jedenaście";
      12:   print "dwanaście";
      13:   print "trzynaście";
      14:   print "czternaście";
      15:   print "piętnaście";
      16:   print "szesnaście";
      17:   print "siedemnaście";
      18:   print "osiemnaście";
      19:   print "dziewiętnaście";
      20 to 99:
            switch (n/10) {
              2:  print "dwadzieścia";
              3:  print "trzydzieści";
              4:  print "czterdzieści";
              5:  print "pięćdziesiąt";
              6:  print "sześćdziesiąt";
              7:  print "siedemdziesiąt";
              8:  print "osiemdziesiąt";
              9:  print "dziewięćdziesiąt";
            }
            if (n%10 ~= 0) print " ", (LanguageNumber) n%10;
    }
];

! Wybiera formę rzeczownika po liczebniku: 1 punkt, 2 punkty, 5 punktów
! (ale 12 punktów, 22 punkty).
[ Liczebnik n f1 f2 f5  d;
    if (n < 0) n = -n;
    d = n%100;
    if (n == 1) { print (string) f1; return; }
    if (d >= 12 && d <= 14) { print (string) f5; return; }
    d = n%10;
    if (d >= 2 && d <= 4) { print (string) f2; return; }
    print (string) f5;
];

[ LanguageTimeOfDay hours mins;
    print hours, ":", mins/10, mins%10;
];

! ------------------------------------------------------------------------------
! Bezokoliczniki czasowników (np. "Zrozumiałem tylko tyle, że chcesz wziąć...").
! ------------------------------------------------------------------------------

[ LanguageVerb i;
    switch (i) {
      'i//', 'inv', 'inwentarz', 'ekwipunek':
                                    print "sprawdzić, co masz przy sobie";
      'l//', 'rozejrzyj':           print "się rozejrzeć";
      'x//', 'ob', 'obejrzyj', 'zbadaj':
                                    print "obejrzeć";
      'czekaj', 'poczekaj', 'zaczekaj':
                                    print "poczekać";
      'dmij', 'zadmij':             print "zadąć";
      'dmuchnij', 'dmuchaj':        print "dmuchnąć";
      'ciągnij', 'pociągnij':       print "pociągnąć";
      'czytaj', 'przeczytaj':       print "przeczytać";
      'daj', 'oddaj', 'podaj':      print "dać";
      'nakarm', 'karm':             print "nakarmić";
      'dotknij':                    print "dotknąć";
      'idź', 'pójdź':               print "pójść";
      'biegnij':                    print "biec";
      'pobiegnij':                  print "pobiec";
      'kop':                        print "kopać";
      'kup', 'zakup':               print "kupić";
      'napełnij':                   print "napełnić";
      'napij':                      print "się napić";
      'odblokuj':                   print "odblokować";
      'odłóż':                      print "odłożyć";
      'odpowiedz':                  print "odpowiedzieć";
      'opróżnij':                   print "opróżnić";
      'otwórz':                     print "otworzyć";
      'pchnij':                     print "pchnąć";
      'popchnij':                   print "popchnąć";
      'przesuń':                    print "przesunąć";
      'płyń':                       print "płynąć";
      'popłyń':                     print "popłynąć";
      'całuj', 'pocałuj':           print "pocałować";
      'przytul':                    print "przytulić";
      'uściskaj':                   print "uściskać";
      'podpal':                     print "podpalić";
      'pokaż':                      print "pokazać";
      'połóż':                      print "położyć";
      'pomachaj', 'machaj':         print "pomachać";
      'pomódl', 'módl':             print "się pomodlić";
      'pomyśl', 'myśl':             print "pomyśleć";
      'poproś', 'proś':             print "poprosić";
      'postaw':                     print "postawić";
      'ustaw':                      print "ustawić";
      'potrzyj':                    print "potrzeć";
      'przetrzyj':                  print "przetrzeć";
      'wytrzyj':                    print "wytrzeć";
      'powąchaj', 'wąchaj':         print "powąchać";
      'powiedz':                    print "powiedzieć";
      'przekręć':                   print "przekręcić";
      'dokręć':                     print "dokręcić";
      'obróć':                      print "obrócić";
      'odkręć':                     print "odkręcić";
      'zakręć':                     print "zakręcić";
      'przenieś':                   print "przenieść";
      'przełóż':                    print "przełożyć";
      'przeskocz':                  print "przeskoczyć";
      'przeszukaj':                 print "przeszukać";
      'przeproś':                   print "przeprosić";
      'prześpij':                   print "się przespać";
      'zdrzemnij':                  print "się zdrzemnąć";
      'przetnij', 'tnij':           print "przeciąć";
      'przekrój', 'krój':           print "przekroić";
      'przyjrzyj':                  print "się przyjrzeć";
      'rzuć':                       print "rzucić";
      'upuść':                      print "upuścić";
      'wyrzuć':                     print "wyrzucić";
      'schowaj':                    print "schować";
      'siądź', 'usiądź':            print "usiąść";
      'słuchaj', 'posłuchaj':       print "posłuchać";
      'spójrz', 'sp':               print "spojrzeć";
      'popatrz':                    print "popatrzeć";
      'sprawdź':                    print "sprawdzić";
      'skonsultuj':                 print "skonsultować";
      'spróbuj':                    print "spróbować";
      'skosztuj':                   print "skosztować";
      'szukaj', 'odszukaj', 'poszukaj':
                                    print "poszukać";
      'ściśnij':                    print "ścisnąć";
      'zgnieć':                     print "zgnieść";
      'uruchom':                    print "uruchomić";
      'wdrap':                      print "się wdrapać";
      'wespnij':                    print "się wspiąć";
      'weź':                        print "wziąć";
      'zabierz':                    print "zabrać";
      'podnieś':                    print "podnieść";
      'wejdź':                      print "wejść";
      'włącz':                      print "włączyć";
      'zapal':                      print "zapalić";
      'włóż':                       print "włożyć";
      'wrzuć':                      print "wrzucić";
      'wsiądź':                     print "wsiąść";
      'wstań':                      print "wstać";
      'wyjdź':                      print "wyjść";
      'wyjmij':                     print "wyjąć";
      'wyciągnij':                  print "wyciągnąć";
      'wyłącz':                     print "wyłączyć";
      'zgaś':                       print "zgasić";
      'wypij':                      print "wypić";
      'wyrzuć':                     print "wyrzucić";
      'wysiądź':                    print "wysiąść";
      'zaatakuj':                   print "zaatakować";
      'zniszcz':                    print "zniszczyć";
      'zabij':                      print "zabić";
      'uderz':                      print "uderzyć";
      'walnij':                     print "walnąć";
      'kopnij':                     print "kopnąć";
      'zaklucz':                    print "zamknąć";
      'zajrzyj':                    print "zajrzeć";
      'załóż':                      print "założyć";
      'ubierz':                     print "ubrać";
      'zamknij':                    print "zamknąć";
      'zapytaj', 'pytaj', 'spytaj': print "zapytać";
      'zastanów':                   print "się zastanowić";
      'zaśpiewaj', 'śpiewaj':       print "zaśpiewać";
      'zjedz':                      print "zjeść";
      'połknij':                    print "połknąć";
      'ugryź':                      print "ugryźć";
      'znajdź':                     print "znaleźć";
      'zdejmij':                    print "zdjąć";
      'zejdź':                      print "zejść";
      'zostaw':                     print "zostawić";
      'zsiądź':                     print "zsiąść";
      'szarpnij':                    print "szarpnąć";
      'wyrwij':                      print "wyrwać";
      'wręcz':                       print "wręczyć";
      'podaruj':                     print "podarować";
      'zaoferuj':                    print "zaoferować";
      'zaprezentuj':                 print "zaprezentować";
      'zagraj':                      print "zagrać";
      'dotykaj':                     print "dotykać";
      'pomacaj':                     print "pomacać";
      'pogłaszcz':                   print "pogłaskać";
      'poczuj':                      print "poczuć";
      'kupuj':                       print "kupić";
      'nalej':                       print "nalać";
      'wypełnij':                    print "wypełnić";
      'pij':                         print "pić";
      'łyknij':                      print "łyknąć";
      'opisz':                       print "opisać";
      'zobacz':                      print "zobaczyć";
      'oglądaj':                     print "oglądać";
      'wysyp':                       print "wysypać";
      'przesyp':                     print "przesypać";
      'wylej':                       print "wylać";
      'wypróżnij':                   print "wypróżnić";
      'odsłoń':                      print "odsłonić";
      'zasłoń':                      print "zasłonić";
      'zakryj':                      print "zakryć";
      'rozwiń':                      print "rozwinąć";
      'odklucz':                     print "odkluczyć";
      'naciśnij':                    print "nacisnąć";
      'wciśnij':                     print "wcisnąć";
      'przyciśnij':                  print "przycisnąć";
      'obejmij':                     print "objąć";
      'spal':                        print "spalić";
      'rozpal':                      print "rozpalić";
      'porozmawiaj':                 print "porozmawiać";
      'rozmawiaj':                   print "rozmawiać";
      'pogadaj':                     print "pogadać";
      'pomów':                       print "pomówić";
      'pozdrów':                     print "pozdrowić";
      'przywitaj':                   print "przywitać";
      'powitaj':                     print "powitać";
      'wstaw':                       print "wstawić";
      'zetrzyj':                     print "zetrzeć";
      'wypoleruj':                   print "wypolerować";
      'umyj':                        print "umyć";
      'przetrząśnij':                print "przetrząsnąć";
      'obszukaj':                    print "obszukać";
      'potnij':                      print "pociąć";
      'rozetnij':                    print "rozciąć";
      'pokrój':                      print "pokroić";
      'przywiąż':                    print "przywiązać";
      'zwiąż':                       print "związać";
      'przymocuj':                   print "przymocować";
      'połącz':                      print "połączyć";
      'przyczep':                    print "przyczepić";
      'rozhuśtaj':                   print "rozhuśtać";
      'huśtaj':                      print "huśtać";
      'pobujaj', 'bujaj':            print "pobujać";
      'ciśnij':                      print "cisnąć";
      'wsadź':                       print "wsadzić";
      'wetknij':                     print "wetknąć";
      'posmakuj':                    print "posmakować";
      'poliż':                       print "polizać";
      'stań':                        print "stanąć";
      'kładź':                       print "się położyć";
      'zmiażdż':                     print "zmiażdżyć";
      'atakuj':                      print "zaatakować";
      'rozbij':                      print "rozbić";
      'połam':                       print "połamać";
      'bij':                         print "uderzyć";
      'wspinaj':                     print "się wspinać";
      'wspnij':                      print "się wspiąć";
      'złap':                        print "złapać";
      'trzymaj':                     print "trzymać";
      'wydobądź':                    print "wydobyć";
      'ściągnij':                    print "ściągnąć";
      'nałóż':                       print "nałożyć";
      'wdziej':                      print "wdziać";
      'jedz':                        print "zjeść";
      'przełknij':                   print "przełknąć";
      'nastaw':                      print "nastawić";
      'wyreguluj':                   print "wyregulować";
      'dostosuj':                    print "dostosować";
      default: rfalse;
    }
    rtrue;
];

! ------------------------------------------------------------------------------
! Przypadki w odtwarzanych poleceniach ("Co chcesz włożyć do pudełka?").
!
! PrzypadekCzasownika podaje przypadek dopełnienia bez przyimka: pierwszego
! (nr 1) i kolejnych (nr 2: narzędzie, np. "otwórz drzwi kluczem"),
! PrzypadekPoPrzyimku - przypadek rzeczownika po przyimku. Gra może zastąpić
! obie procedury dyrektywą Replace.
! ------------------------------------------------------------------------------

! Czasowniki dawania i pokazywania: osoba w celowniku, rzecz w bierniku
! ("daj kowalowi lampę").
[ PL_Dawanie v;
    return v == 'daj' or 'oddaj' or 'podaj' or 'wręcz' or 'podaruj' or 'zaoferuj'
                or 'pokaż' or 'zaprezentuj';
];

[ PrzypadekCzasownika v o nr;
    if (PL_Dawanie(v)) {
        if (o ofclass Object && o has animate) return PRZYP_CEL;
        return PRZYP_BIE;
    }
    if (nr > 1) return PRZYP_NAR;
    switch (v) {
      'napij', 'dotknij', 'szukaj', 'poszukaj', 'odszukaj', 'słuchaj',
      'posłuchaj', 'użyj', 'skosztuj', 'spróbuj':
          return PRZYP_DOP;
      'przyjrzyj':
          return PRZYP_CEL;
      'pomachaj', 'machaj':
          return PRZYP_NAR;
    }
    return PRZYP_BIE;
];

[ PrzypadekPoPrzyimku p v;
    switch (p) {
      'do', 'od', 'spod', 'znad', 'zza', 'bez', 'dla', 'koło', 'obok',
      'u//', 'wśród', 'spośród', 'naprzeciw', 'oprócz', 'wokół', 'dookoła':
          return PRZYP_DOP;
      'z//', 'ze':
          if (v == 'porozmawiaj' or 'rozmawiaj' or 'pogadaj' or 'pomów' or 'połącz' or 'zwiąż')
              return PRZYP_NAR;
          return PRZYP_DOP;
      'ku', 'dzięki', 'przeciw':
          return PRZYP_CEL;
      'w//', 'we', 'przy', 'po':
          return PRZYP_MIE;
      'na':
          if (v == 'połóż' or 'postaw' or 'umieść' or 'zostaw' or 'usiądź' or 'siądź' or 'kładź')
              return PRZYP_MIE;
          return PRZYP_BIE;
      'o//':
          if (v == 'zapytaj' or 'pytaj' or 'spytaj' or 'poproś' or 'proś')
              return PRZYP_BIE;
          return PRZYP_MIE;
      'między', 'pomiędzy':
          return PRZYP_NAR;
    }
    return PRZYP_BIE;   ! przez, pod, nad, przed, za, ...
];

Global pl_bez_przyimka;     ! pomija końcowy przyimek w LanguageCommand

[ LanguageCommand from  i k spacing_flag przyp ostatni nr;
    if (from == 0) {
        i = verb_word;
        if (LanguageVerb(i) == 0 && PrintVerb(i) == false && LibraryExtensions.RunWhile(ext_printverb, false, i) == 0)
            print (address) i;
        from++; spacing_flag = true;
    }

    przyp = -1;
    ostatni = pcount - 1;
    while (ostatni >= from && pattern-->ostatni == PATTERN_NULL) ostatni--;

    for (k=from : k<pcount : k++) {
        i = pattern-->k;
        if (i == PATTERN_NULL) continue;
        if (pl_bez_przyimka && k == ostatni && i >= REPARSE_CODE) continue;
        if (spacing_flag) print (char) ' ';
        if (i == 0) { print (string) THOSET__TX; jump TokenPrinted; }
        if (i == 1) { print (string) THAT__TX;   jump TokenPrinted; }
        if (i >= REPARSE_CODE) {
            print (address) No__Dword(i-REPARSE_CODE);
            przyp = PrzypadekPoPrzyimku(No__Dword(i-REPARSE_CODE), verb_word);
        }
        else {
            if (i in Compass && LanguageVerbLikesAdverb(verb_word))
                LanguageDirection(i.door_dir);
            else {
                nr++;
                if (przyp < 0) przyp = PrzypadekCzasownika(verb_word, i, nr);
                NazwaWPrzypadku(i, przyp);
            }
            przyp = -1;
        }
      .TokenPrinted;
        spacing_flag = true;
    }
];

! Zaimek pytający w danym przypadku: co/czego/czemu/co/czym/czym
! albo kto/kogo/komu/kogo/kim/kim.
[ ZaimekPytajacy c osoba caps;
    if (osoba) switch (c) {
      PRZYP_MIAN: if (caps) print "Kto";  else print "kto";
      PRZYP_DOP, PRZYP_BIE:
                  if (caps) print "Kogo"; else print "kogo";
      PRZYP_CEL:  if (caps) print "Komu"; else print "komu";
      default:    if (caps) print "Kim";  else print "kim";
    }
    else switch (c) {
      PRZYP_DOP:  if (caps) print "Czego"; else print "czego";
      PRZYP_CEL:  if (caps) print "Czemu"; else print "czemu";
      PRZYP_NAR, PRZYP_MIE:
                  if (caps) print "Czym";  else print "czym";
      default:    if (caps) print "Co";    else print "co";
    }
];

! Zadaje pytanie o brakujące dopełnienie, np. "Co chcesz wziąć?",
! "Do czego chcesz włożyć klucz?", "Komu chcesz dać klucz?".
[ PytanieOBrak osoba aktor  k i p;
    k = pcount - 1;
    while (k > 0 && pattern-->k == PATTERN_NULL) k--;
    i = pattern-->k;
    if (k > 0 && i >= REPARSE_CODE) {
        p = No__Dword(i-REPARSE_CODE);
        CapAddress(p);      ! przyimek z wielkiej litery, np. "Do"
        print " ";
        ZaimekPytajacy(PrzypadekPoPrzyimku(p, verb_word), osoba);
    }
    else {
        if (k == 0 && verb_word == 'idź' or 'pójdź' or 'biegnij' or 'pobiegnij' or 'płyń' or 'popłyń')
            print "Dokąd";
        else {
            p = PRZYP_NAR;
            if (k == 0) p = PrzypadekCzasownika(verb_word);
            if (PL_Dawanie(verb_word)) {
                if (osoba) p = PRZYP_CEL; else p = PRZYP_BIE;
            }
            ZaimekPytajacy(p, osoba, true);
        }
    }
    print " chcesz ";
    if (aktor ~= player && aktor ~= nothing) print "kazać ", (cel) aktor, " ";
    pl_bez_przyimka = true;
    PrintCommand();
    pl_bez_przyimka = false;
    "?";
];

! Słowo ze słownika z wielkiej litery.
[ PrintAddress p; print (address) p; ];
[ CapAddress p;
    PrintToBuffer(StorageForShortName, SHORTNAMEBUF_LEN, PrintAddress, p);
    PrintFromBuffer(StorageForShortName, true, true);
];

! ----------------------------------------------------------------------------
!  LanguageVerbIsDebugging woła SearchScope.  Zwraca prawdę, jeśli słowo w jest
!  czasownikiem debugowania, dla którego wszystkie obiekty muszą być w zasięgu.
! ----------------------------------------------------------------------------

#Ifdef DEBUG;
[ LanguageVerbIsDebugging w;
    if (w == 'purloin' or 'tree' or 'abstract'
                       or 'gonear' or 'scope' or 'showobj')
        rtrue;
    rfalse;
];
#Endif;

! ----------------------------------------------------------------------------
!  LanguageVerbLikesAdverb: czasowniki, po których kierunek drukuje się jako
!  okolicznik ("pójść na północ"), a nie jako rzeczownik.
! ----------------------------------------------------------------------------

[ LanguageVerbLikesAdverb w;
    if (w == 'idź' or 'pójdź' or 'biegnij' or 'pobiegnij' or 'płyń' or 'popłyń'
          or 'spójrz' or 'popatrz' or 'pchnij' or 'popchnij' or 'przesuń'
          or 'look' or 'go' or 'push' or 'walk')
        rtrue;
    rfalse;
];

! ----------------------------------------------------------------------------
!  LanguageVerbMayBeName: słowa, które mogą być zarówno czasownikiem, jak
!  i przymiotnikiem w nazwie obiektu (odpowiedź na pytanie "Który...").
! ----------------------------------------------------------------------------

[ LanguageVerbMayBeName w;
    if (w == 'normalne' or 'pełne' or 'skrócone' or 'long' or 'short'
                    or 'normal' or 'brief' or 'full' or 'verbose')
        rtrue;
    rfalse;
];

Constant LanguageVersion = "Polska biblioteka inform6-pl 6.12.8-pl2";

Constant NKEY__TX       = "N = następny temat";
Constant PKEY__TX       = "P = poprzedni";
Constant QKEY1__TX      = "  Q = powrót do gry";
Constant QKEY2__TX      = "Q = poprzednie menu";
Constant RKEY__TX       = "ENTER = przeczytaj";

Constant NKEY1__KY      = 'N';
Constant NKEY2__KY      = 'n';
Constant PKEY1__KY      = 'P';
Constant PKEY2__KY      = 'p';
Constant QKEY1__KY      = 'Q';
Constant QKEY2__KY      = 'q';

Constant SCORE__TX      = "Wynik: ";
Constant MOVES__TX      = "Ruchy: ";
Constant TIME__TX       = "Czas: ";
Constant SCORE_S__TX    = "W: ";
Constant MOVES_S__TX    = "R: ";
Constant TIME_S__TX     = "C: ";
Constant CANTGO__TX     = "Nie możesz pójść w tę stronę.";
Constant FORMER__TX     = "twoje dawne ja";
Constant MYFORMER__TX   = "moje dawne ja";
Constant YOURSELF__TX   = "ty";
Constant MYSELF__TX     = "ja";
Constant YOU__TX        = "Ty";
Constant DARKNESS__TX   = "Ciemność";

Constant THOSET__TX     = "te rzeczy";
Constant THAT__TX       = "to";
Constant THE__TX        = "";
Constant OR__TX         = " czy ";
Constant NOTHING__TX    = "nic";
Constant IS__TX         = " jest";
Constant ARE__TX        = " są";
Constant IS2__TX        = "jest ";
Constant ARE2__TX       = "są ";
Constant WAS__TX        = " był";
Constant WERE__TX       = " były";
Constant WAS2__TX       = "był ";
Constant WERE2__TX      = "były ";
Constant AND__TX        = " i ";
Constant WHOM__TX       = "";       ! zaimek "który" drukują ListMiscellany 19-22
Constant WHICH__TX      = "";
Constant COMMA__TX      = ", ";
Constant COLON__TX      = ":";

! Słowa, które biblioteka porównuje z verb_word w EnterSub.
Constant STAND__TX      = 'stań';
Constant SIT__TX        = 'usiądź';
Constant LIE__TX        = 'połóż';

Constant LIBERROR__TX   = "Błąd biblioteki ";
Constant TERP__TX       = "Interpreter ";
Constant VER__TX        = "Wersja ";
Constant STDTERP__TX    = "Standardowy interpreter ";
Constant TERPVER__TX    = "Wersja interpretera ";
Constant LIBSER__TX     = "Numer seryjny biblioteki ";
Constant VM__TX         = "Maszyna wirtualna ";
Constant RELEASE__TX    = "Wydanie ";
Constant SERNUM__TX     = "Numer seryjny ";
Constant INFORMV__TX    = "Inform v";
Constant LIBRARYV__TX   = " Biblioteka v";

! ------------------------------------------------------------------------------
! Odmiana przez rodzaje i liczby
! ------------------------------------------------------------------------------

! 0 = męski, 1 = żeński, 2 = nijaki, 3 = mnogi niemęskoosobowy,
! 4 = mnogi męskoosobowy
[ RodzajObiektu o;
    if (o == player) {
        if (player has female) return 1;
        return 0;
    }
    if (o has pluralname) {
        if (o has animate && o has male) return 4;
        return 3;
    }
    if (o has female) return 1;
    if (o has neuter) return 2;
    return 0;
];

[ Mnoga o; return RodzajObiektu(o) >= 3; ];

! Drukuje jedną z form zależnie od rodzaju obiektu.
! Forma(x1, "otwarty", "otwarta", "otwarte", "otwarte")
[ Forma o m f n p pm;
    switch (RodzajObiektu(o)) {
      0: print (string) m;
      1: print (string) f;
      2: print (string) n;
      3: print (string) p;
      4: if (pm) print (string) pm; else print (string) p;
    }
];

! Forma liczby pojedynczej lub mnogiej: PL(x1, "leży", "leżą")
[ PL o sg pl;
    if (Mnoga(o)) print (string) pl; else print (string) sg;
];

! Forma czasu przeszłego zależna od płci gracza: Gracz("zrobiłeś", "zrobiłaś")
[ Gracz m f;
    if (player has female) print (string) f; else print (string) m;
];

! Końcówka przymiotnika: "otwart", (KoncY) x1 -> otwarty/otwarta/otwarte
[ KoncY o; Forma(o, "y", "a", "e", "e"); ];

! Przymiotnik albo imiesłów z rdzenia; rodzaj męskoosobowy ma inną postać
! rdzenia ("zamknięci", "puści", "włączeni"), więc podaje się ją w całości:
! (nop) PL_PrzymZ(x1, "zamknięt", "zamknięci") -> zamknięty/-a/-e/zamknięci.
[ PL_PrzymZ o rdzen meskoos;
    if (RodzajObiektu(o) == 4) { print (string) meskoos; return; }
    print (string) rdzen; KoncY(o);
];
! To samo w dopisku na liście ("butelkę (pustą)"), zob. KoncYL.
[ PL_PrzymZL o rdzen meskoos;
    if (pl_lista_rodzic && pl_lista_przyp == PRZYP_BIE && o ofclass Object &&
        parent(o) == pl_lista_rodzic) { print (string) rdzen; KoncYL(o); return; }
    PL_PrzymZ(o, rdzen, meskoos);
];

! Końcówka przymiotnika w dopisku na liście ("butelkę (pustą)"): w bierniku,
! jeśli lista jest drukowana w bierniku ("Widzisz tutaj ...", ekwipunek).
[ KoncYL o;
    if (pl_lista_rodzic && pl_lista_przyp == PRZYP_BIE && o ofclass Object &&
        parent(o) == pl_lista_rodzic) {
        if (o has animate && RodzajObiektu(o) == 0) { print "ego"; return; }
        Forma(o, "y", "ą", "e", "e", "ych");
        return;
    }
    KoncY(o);
];

! jest / są (gracz: jesteś)
[ JestSa o;
    if (o == player) { print "jesteś"; return; }
    PL(o, "jest", "są");
];

! Zaimki osobowe
[ GoJaJe o;         ! biernik: weź go/ją/je
    if (o == player) { print "siebie"; return; }
    Forma(o, "go", "ją", "je", "je", "ich");
];
[ JegoJejIch o;     ! dopełniacz: nie masz go/jej/ich
    if (o == player) { print "siebie"; return; }
    Forma(o, "go", "jej", "go", "ich");
];
[ MuJejIm o;        ! celownik
    if (o == player) { print "sobie"; return; }
    Forma(o, "mu", "jej", "mu", "im");
];
[ NimNiejNich o;    ! miejscownik po przyimku: na nim/niej/nich
    if (o == player) { print "sobie"; return; }
    Forma(o, "nim", "niej", "nim", "nich");
];
[ KtorymKtorej o;   ! miejscownik zaimka "który": na którym/której/których
    Forma(o, "którym", "której", "którym", "których");
];
[ CTenTaTo o;       ! mianownik, z wielkiej litery
    if (o == player) { print "Ty"; return; }
    Forma(o, "Ten", "Ta", "To", "Te", "Ci");
];

! Przyimki "z/ze" i "w/we"
! "z"/"ze" i "w"/"we": forma rozszerzona stoi przed zbitką spółgłosek, której
! pierwsza głoska jest podobna do przyimka: "ze stołu", "ze skrzyni", "ze
! źródła", "ze szkła" (ale "z szafy"), "we wraku", "we framudze".  Biblioteka
! wypisuje nazwę w odpowiednim przypadku i sprawdza jej pierwsze litery;
! atrybuty z_article i w_article wymuszają formę rozszerzoną.
#Ifdef TARGET_ZCODE;
Array pl_poczbuf -> WORDSIZE + 200;
#Ifnot;
Array pl_poczbuf --> 200;
#Endif;
Global pl_pocz_przyp;
[ PL_NazwaPocz o; NazwaWPrzypadku(o, pl_pocz_przyp); ];
[ PL_Pocz o c i  n;      ! i-ta litera nazwy o w przypadku c (małą literą)
    pl_pocz_przyp = c;
    #Ifdef TARGET_ZCODE;
    n = PrintToBuffer(pl_poczbuf, 200, PL_NazwaPocz, o);
    if (i >= n) return ' ';
    return LowerCase(pl_poczbuf->(WORDSIZE+i));
    #Ifnot;
    n = PL_Renderuj(pl_poczbuf, 200, PL_NazwaPocz, o);
    if (i >= n) return ' ';
    return LowerCase(pl_poczbuf-->i);
    #Endif;
];
[ PL_Ze o c  c1 c2 c3;
    if (o has z_article) rtrue;
    if (~~(o ofclass Object)) rfalse;
    c1 = PL_Pocz(o, c, 0); c2 = PL_Pocz(o, c, 1);
    if (c1 == 's' && c2 == 'z') { c3 = PL_Pocz(o, c, 2); return c3 ~= ' ' && ~~PL_Samogl(c3); }
    if (c1 == 's' or 'z' or 'ś' or 'ź' or 'ż') return c2 ~= ' ' && ~~PL_Samogl(c2);
    rfalse;
];
[ PL_We o c  c1 c2;
    if (o has w_article) rtrue;
    if (~~(o ofclass Object)) rfalse;
    c1 = PL_Pocz(o, c, 0); c2 = PL_Pocz(o, c, 1);
    if (c1 == 'w' or 'f') return c2 ~= ' ' && ~~PL_Samogl(c2);
    rfalse;
];
[ pl_z o; if (PL_Ze(o, PRZYP_DOP)) print "ze"; else print "z"; ];
[ pl_w o; if (PL_We(o, PRZYP_MIE)) print "we"; else print "w"; ];

! "na" dla podstawek (supporter), "w" dla pozostałych
[ NaW o; if (o has supporter) print "na"; else pl_w(o); ];

! Podmiot zdania: gracz lub inna postać (rozkazy).
! Kto(actor, "Bierzesz", "bierze") -> "Bierzesz" albo "Kowal bierze".
[ Kto a ty on;
    if (a == player || a == 0) print (string) ty;
    else print (CMian) a, " ", (string) on;
];

[ nop x; x = x; ];      ! print rule to absorb unwanted return value

! Procedury używane bezpośrednio przez parser i verblib.
[ ThatOrThose obj;
    if (obj == player) { print "ciebie"; return; }
    Forma(obj, "ten", "ta", "to", "te", "ci");
];

[ ItOrThem obj; GoJaJe(obj); ];

! Lista poprzedzona czasownikiem ("W pudełku jest ...", "Na stole są ...").
! Po polsku czasownik zgadza się z pierwszym elementem listy: "jest lampa
! i kamyk", "są klucze", "są dwie świece", ale "jest osiem świec".
Global pl_lista_od;

[ PL_ListaZCzasownikiem o styl  w;
    pl_lista_od = o;
    w = WriteListFrom(o, styl);
    pl_lista_od = 0;
    return w;
];

[ PL_CzasownikListy o  s n;
    while (o && o has concealed or scenery) o = sibling(o);
    if (o == 0 || o has pluralname) { print (string) ARE__TX; return; }
    if (o.list_together ~= 0) {
        objectloop (s in parent(o)) if (s.list_together == o.list_together) n++;
    }
    else if (o.plural ~= 0) {
        objectloop (s in parent(o)) if (s.plural ~= 0 && Identical(o, s)) n++;
    }
    if (n >= 2 && n%10 >= 2 && n%10 <= 4 && (n%100 < 12 || n%100 > 14)) {
        print (string) ARE__TX; return;
    }
    print (string) IS__TX;
];

[ Tense present past;
    if (present == ARE__TX && pl_lista_od && ~~(player provides narrative_tense &&
        player.narrative_tense == PAST_TENSE)) {
        PL_CzasownikListy(pl_lista_od); return;
    }
    if (player provides narrative_tense && player.narrative_tense == PAST_TENSE) {
        if (past == false) return;
        print (string) past;
    }
    else
        print (string) present;
];

[ DecideAgainst;
    Kto(actor, "Rezygnujesz", "rezygnuje");
    " z tego pomysłu.";
];

! ------------------------------------------------------------------------------
! Wielkie i małe litery. Polskie litery mają w maszynie Z kody 224-231 (małe)
! i 232-239 (wielkie), zgodnie z tablicą znaków z polskiz.h.
! ------------------------------------------------------------------------------

#Ifdef TARGET_ZCODE;

[ LowerCase c;    ! for ZSCII matching ISO 8859-1
   switch (c) {
     'A' to 'Z':                            c = c + 32;
     202, 204, 212, 214, 221:               c--;
     217, 218:                              c = c - 2;
     158 to 160, 167, 168, 208 to 210:      c = c - 3;
     186 to 190, 196 to 200:                c = c - 5 ;
     175 to 180:                            c = c - 6;
     232 to 239:                            c = c - 8;
   }
   return c;
];

[ UpperCase c;    ! for ZSCII matching ISO 8859-1
   switch (c) {
     'a' to 'z':                            c = c - 32;
     201, 203, 211, 213, 220:               c++;
     215, 216:                              c = c + 2;
     155 to 157, 164, 165, 205 to 207:      c = c + 3;
     181 to 185, 191 to 195:                c = c + 5 ;
     169 to 174:                            c = c + 6;
     224 to 231:                            c = c + 8;
   }
   return c;
];

#Ifnot; ! TARGET_GLULX

! glk_char_to_lower obsługuje tylko znaki Latin-1 (do 255), a polskie litery
! ą, ę, ł... leżą dalej, więc dla nich używamy funkcji Unicode.
Array pl_wielkosc --> 1;
[ LowerCase c;
    if (c < 256) return glk_char_to_lower(c);
    pl_wielkosc-->0 = c; glk_buffer_to_lower_case_uni(pl_wielkosc, 1, 1);
    return pl_wielkosc-->0;
];
[ UpperCase c;
    if (c < 256) return glk_char_to_upper(c);
    pl_wielkosc-->0 = c; glk_buffer_to_upper_case_uni(pl_wielkosc, 1, 1);
    return pl_wielkosc-->0;
];

#Endif; ! TARGET_

! ------------------------------------------------------------------------------
! Komunikaty biblioteki
! ------------------------------------------------------------------------------

[ LanguageLM n x1 x2;
  Answer,Ask:
            "Nikt ci nie odpowiada.";
! Ask:      see Answer
  Attack:   "Przemocą nic tu nie zdziałasz.";
  Blow:     "Dmuchanie w ", (bie) x1, " nic nie da.";
  Burn: switch (n) {
        1:  "To niebezpieczny pomysł, a do tego nic by nie dał.";
        2:  DecideAgainst();
    }
  Buy:      "Nic tu nie jest na sprzedaż.";
  Climb: switch (n) {
        1:  "Wspinaczka niewiele by tu dała.";
        2:  DecideAgainst();
    }
  Close: switch (n) {
        1:  "Tego nie da się zamknąć.";
        2:  print_ret (CMian) x1, " ", (JestSa) x1, " już ", (nop) PL_PrzymZ(x1, "zamknięt", "zamknięci"), ".";
        3:  Kto(actor, "Zamykasz", "zamyka"); " ", (bie) x1, ".";
        4:  "(najpierw zamykasz ", (bie) x1, ")";
    }
  CommandsOff: switch (n) {
        1: "[Zapisywanie poleceń wyłączone.]";
        #Ifdef TARGET_GLULX;
        2: "[Zapisywanie poleceń było już wyłączone.]";
        #Endif; ! TARGET_
    }
  CommandsOn: switch (n) {
        1: "[Zapisywanie poleceń włączone.]";
        #Ifdef TARGET_GLULX;
        2: "[Polecenia są właśnie odtwarzane.]";
        3: "[Zapisywanie poleceń było już włączone.]";
        4: "[Nie udało się włączyć zapisywania poleceń.]";
        #Endif; ! TARGET_
    }
  CommandsRead: switch (n) {
        1: "[Odtwarzanie poleceń.]";
        #Ifdef TARGET_GLULX;
        2: "[Polecenia są już odtwarzane.]";
        3: "[Nie można odtworzyć poleceń, bo trwa ich zapisywanie.]";
        4: "[Nie udało się odtworzyć poleceń.]";
        5: "[Odtwarzanie poleceń zakończone.]";
        #Endif; ! TARGET_
    }
  Consult:  Kto(actor, "Nie znajdujesz", "nie znajduje");
            " niczego ciekawego w ", (mie) x1, ".";
  Cut: switch (n) {
        1:  "Przecięcie ", (dop) x1, " niewiele by dało.";
        2:  DecideAgainst();
    }
  Dig:      "Kopanie nic tu nie da.";
  Disrobe: switch (n) {
        1:  "Nie masz ", (JegoJejIch) x1, " na sobie.";
        2:  Kto(actor, "Zdejmujesz", "zdejmuje"); " ", (bie) x1, ".";
        3:  "(najpierw zdejmujesz ", (bie) x1, ")";
        4:  "Najpierw musisz zdjąć ", (bie) x1, ".";
    }
  Drink:    "Nie ma tu nic, co nadawałoby się do picia.";
  Drop: switch (n) {
        1:  print_ret (CMian) x1, " ", (JestSa) x1, " już tutaj.";
        2:  "Nie masz ", (dop) x1, ".";
        3:  "Upuszczone.";
        4:  print "Najpierw musisz ";
            if (x2 has container) print "wyjąć "; else print "zdjąć ";
            print (bie) x1, " ", (pl_z) x2, " ", (dop) x2;
            ".";
    }
  Eat: switch (n) {
        1:  "Tego nie da się zjeść.";
        2:  Kto(actor, "Zjadasz", "zjada"); print " ", (bie) x1;
            if (actor ~= player) ".";
            print ". "; PL_PrzymZ(x1, "Smaczn", "Smaczni"); ".";
    }
  EmptyT: switch (n) {
        1:  print_ret (CMian) x1, " nie ", (nop) PL(x1, "może", "mogą"), " niczego zawierać.";
        2:  print_ret (CMian) x1, " ", (JestSa) x1, " ", (nop) PL_PrzymZ(x1, "zamknięt", "zamknięci"), ".";
        3:  print_ret (CMian) x1, " ", (JestSa) x1, " już ", (nop) PL_PrzymZ(x1, "pust", "puści"), ".";
        4:  "To niczego by nie opróżniło.";
    }
  Enter: switch (n) {
        1:  "Przecież już ", (JestSa) actor, " ", (NaW) x1, " ", (mie) x1, ".";
        2:  switch (x2) {     ! stanąć/usiąść/położyć się na czymś, wejść na coś/do czegoś
              'stań':   "Na ", (mie) x1, " nie da się stanąć.";
              'usiądź': "Na ", (mie) x1, " nie da się usiąść.";
              'połóż':  "Na ", (mie) x1, " nie da się położyć.";
              default:  "Tam nie da się wejść.";
            }
        3:  "Nie możesz wejść do zamknięt", (nop) Forma(x1, "ego", "ej", "ego", "ych"),
            " ", (dop) x1, ".";
        4:  "Możesz wejść tylko do czegoś, co stoi samodzielnie.";
        5:  Kto(actor, "Wchodzisz", "wchodzi");
            if (x1 has supporter) " na ", (bie) x1, ".";
            " do ", (dop) x1, ".";
        6:  if (x1 has supporter) "(schodzisz ", (pl_z) x1, " ", (dop) x1, ")";
            "(wychodzisz ", (pl_z) x1, " ", (dop) x1, ")";
        7:  if (x1 has supporter) "(wchodzisz na ", (bie) x1, ")";
            "(wchodzisz do ", (dop) x1, ")";
    }
  Examine: switch (n) {
        1:  "Ciemność, rzecz. ż.: brak światła, w którym dałoby się cokolwiek zobaczyć.";
        2:  print_ret (CMian) x1, " nie ", (nop) PL(x1, "wyróżnia", "wyróżniają"),
                " się niczym szczególnym.";
        3:  print (CMian) x1, " ", (JestSa) x1, " teraz ";
            if (x1 has on) PL_PrzymZ(x1, "włączon", "włączeni");
            else PL_PrzymZ(x1, "wyłączon", "wyłączeni");
            ".";
    }
  Exit: switch (n) {
        1:  "Nie masz z czego wyjść.";
        2:  "Nie możesz wyjść, bo ", (mian) x1, " ", (JestSa) x1, " ", (nop) PL_PrzymZ(x1, "zamknięt", "zamknięci"), ".";
        3:  if (x1 has supporter) Kto(actor, "Schodzisz", "schodzi");
            else Kto(actor, "Wychodzisz", "wychodzi");
            " ", (pl_z) x1, " ", (dop) x1, ".";
        4:  "Przecież nie jesteś ", (NaW) x1, " ", (mie) x1, ".";
        5:  if (x1 has supporter) "(najpierw schodzisz ", (pl_z) x1, " ", (dop) x1, ")";
            "(najpierw wychodzisz ", (pl_z) x1, " ", (dop) x1, ")";
        6:  Kto(actor, "Wstajesz", "wstaje"); ".";
    }
  Fill: switch (n) {
        1:  "Nie ma tu niczego, czym można by napełnić ", (bie) x1, ".";
        2:  "Napełnianie ", (dop) x1, " ", (pl_z) x2, " ", (dop) x2, " nie miałoby sensu.";
    }
  FullScore: switch (n) {
        1:  if (deadflag) print "Na wynik złożyły się "; else print "Na wynik składają się ";
            "następujące osiągnięcia:^";
        2:  "znalezienie różnych przedmiotów";
        3:  "odwiedzenie różnych miejsc";
        4:  print "łącznie (na ", MAX_SCORE, " możliw";
            Liczebnik(MAX_SCORE, "y", "e", "ych"); ")";
    }
  GetOff:   "Przecież nie jesteś teraz na ", (mie) x1, ".";
  Give: switch (n) {
        1:  "Nie masz ", (dop) x1, ".";
        2:  "Przez chwilę żonglujesz ", (nar) x1, ", ale niewiele z tego wynika.";
        3:  print_ret (CMian) x1, " nie ", (nop) PL(x1, "wydaje", "wydają"),
                " się tym zainteresowan", (nop) Forma(x1, "y", "a", "e", "e", "i"), ".";
        4:  Kto(actor, "Oddajesz", "oddaje"); " ", (bie) x1, ".";
    }
  Go: switch (n) {
        1:  print "Najpierw musisz ";
            if (x1 has supporter) print "zejść "; else print "wyjść ";
            print_ret (pl_z) x1, " ", (dop) x1, ".";
        2:  print_ret (string) CANTGO__TX;
        3:  "Nie dasz rady wspiąć się na ", (bie) x1, ".";
        4:  "Nie dasz rady zejść po ", (mie) x1, ".";
        5:  "Nie możesz, bo na drodze ", (nop) PL(x1, "stoi", "stoją"), " ", (mian) x1, ".";
        6:  "Nie możesz, bo ", (mian) x1, " nigdzie nie ", (nop) PL(x1, "prowadzi", "prowadzą"), ".";
        7:  Kto(actor, "Odchodzisz", "odchodzi"); ".";
    }
  Insert: switch (n) {
        1:  "Musisz najpierw wziąć ", (bie) x1, ", żeby ", (GoJaJe) x1,
            " gdzieś włożyć.";
        2:  print_ret (CMian) x1, " nie ", (nop) PL(x1, "może", "mogą"), " niczego zawierać.";
        3:  print_ret (CMian) x1, " ", (JestSa) x1, " ", (nop) PL_PrzymZ(x1, "zamknięt", "zamknięci"), ".";
        4:  "Najpierw musisz ", (GoJaJe) x1, " zdjąć.";
        5:  "Nie można włożyć czegoś do samego siebie.";
        6:  "(najpierw zdejmujesz ", (GoJaJe) x1, ")";
        7:  "W ", (mie) x1, " nie ma już miejsca.";
        8:  "Zrobione.";
        9:  Kto(actor, "Wkładasz", "wkłada"); " ", (bie) x1, " do ", (dop) x2, ".";
    }
  Inv: switch (n) {
        1:  "Nie masz nic przy sobie.";
        2:  print "Masz przy sobie";
            ListaWPrzypadku(actor, PRZYP_BIE);  ! lista w bierniku
        3:  print ":^";
        4:  ListaWPrzypadku(0); ".";
    }
  Jump:     Kto(actor, "Podskakujesz", "podskakuje"); " w miejscu, ale nic z tego nie wynika.";
  JumpIn: switch (n) {
        1:  "Wskakiwanie do ", (dop) x1, " nic tu nie da.";
        2:  DecideAgainst();
    }
  JumpOn: switch (n) {
        1:  "Wskakiwanie na ", (bie) x1, " nic tu nie da.";
        2:  DecideAgainst();
    }
  JumpOver: switch (n) {
        1:  "Przeskakiwanie przez ", (bie) x1, " nic tu nie da.";
        2:  DecideAgainst();
    }
  Kiss:     "Lepiej skup się na grze.";
  Listen:   Kto(actor, "Nie słyszysz", "nie słyszy"); " niczego niezwykłego.";
  ListMiscellany: switch (n) {
        1:  print " (świeci)";
        2:  print " (", (nop) PL_PrzymZL(x1, "zamknięt", "zamknięci"), ")";
        3:  print " (", (nop) PL_PrzymZL(x1, "zamknięt", "zamknięci"), ", świeci)";
        4:  print " (", (nop) PL_PrzymZL(x1, "pust", "puści"), ")";
        5:  print " (", (nop) PL_PrzymZL(x1, "pust", "puści"), ", świeci)";
        6:  print " (", (nop) PL_PrzymZL(x1, "zamknięt", "zamknięci"), " i ", (nop) PL_PrzymZL(x1, "pust", "puści"), ")";
        7:  print " (", (nop) PL_PrzymZL(x1, "zamknięt", "zamknięci"), ", ", (nop) PL_PrzymZL(x1, "pust", "puści"), ", świeci)";
        8:  print " (świeci, masz ", (GoJaJe) x1, " na sobie";
        9:  print " (świeci";
        10: print " (masz ", (GoJaJe) x1, " na sobie";
        11: print " (";
        12: print (nop) PL_PrzymZL(x1, "otwart", "otwarci");
        13: print (nop) PL_PrzymZL(x1, "otwart", "otwarci"), ", ale ", (nop) PL_PrzymZL(x1, "pust", "puści");
        14: print (nop) PL_PrzymZL(x1, "zamknięt", "zamknięci");
        15: print (nop) PL_PrzymZL(x1, "zamknięt", "zamknięci"), " na klucz";
        16: print " i ", (nop) PL_PrzymZL(x1, "pust", "puści");
        17: print " (", (nop) PL_PrzymZL(x1, "pust", "puści"), ")";
        18: print ", zawiera ";
        19: print " (na ", (KtorymKtorej) x1, " ";
        20: print ", na ", (KtorymKtorej) x1, " ";
        21: print " (w ", (KtorymKtorej) x1, " ";
        22: print ", w ", (KtorymKtorej) x1, " ";
    }
  LMode1:   print "Włączony jest teraz ";
            if (initial_lookmode == 1) print "domyślny ";
            "tryb ~normalny~: pełne opisy miejsc przy pierwszej wizycie,
             a potem skrócone.";
  LMode2:   print "Włączony jest teraz ";
            if (initial_lookmode ~= 1 or 3) print "domyślny ";
            "tryb ~pełny~: zawsze pełne opisy miejsc, nawet tych już
             odwiedzonych.";
  LMode3:   print "Włączony jest teraz ";
            if (initial_lookmode == 3) print "domyślny ";
            "tryb ~skrócony~: zawsze krótkie opisy miejsc, nawet tych
             jeszcze nieodwiedzonych.";
  Lock: switch (n) {
        1:  "Tego nie da się zamknąć na klucz.";
        2:  print_ret (CMian) x1, " ", (JestSa) x1, " już ", (nop) PL_PrzymZ(x1, "zamknięt", "zamknięci"), " na klucz.";
        3:  "Najpierw musisz zamknąć ", (bie) x1, ".";
        4:  print_ret (CMian) x1, " nie ", (nop) PL(x1, "pasuje", "pasują"), " do zamka.";
        5:  Kto(actor, "Zamykasz", "zamyka"); print " ", (bie) x1;   ! "mosiężnym kluczem", "łańcuchem"
            if (second) print " ", (nar) second; else print " na klucz";
            ".";
    }
  Look: switch (n) {
        1:  print " (na ", (mie) x1, ")";
        2:  print " (", (pl_w) x1, " ", (mie) x1, ")";
        3:  print " (jako ", (name) x1, ")";
        4:  print "^Na ", (mie) x1;
            PL_ListaZCzasownikiem(child(x1),
              ENGLISH_BIT+RECURSE_BIT+PARTINV_BIT+TERSE_BIT+CONCEAL_BIT+ISARE_BIT);
            ".";
        5,6:
            new_line;
            if (x1 ~= location) {
                if (x1 has supporter) print "Na ";
                else { if (PL_We(x1, PRZYP_MIE)) print "We "; else print "W "; }
                print (mie) x1, " ";
                Kto(actor, "widzisz", "widzi");
            }
            else { Kto(actor, "Widzisz", "widzi"); print " tutaj"; }
            if (n == 5) print " także";
            print " ";
            ListaWPrzypadku(x1, PRZYP_BIE);
            PL_ListaZCzasownikiem(child(x1),
              ENGLISH_BIT+RECURSE_BIT+PARTINV_BIT+TERSE_BIT+CONCEAL_BIT+WORKFLAG_BIT);
            ListaWPrzypadku(0);
            ".";
        7:  Kto(actor, "Nie dostrzegasz", "nie dostrzega"); " niczego niezwykłego w tym kierunku.";
    }
  LookUnder: switch (n) {
        1:  "Przecież jest ciemno.";
        2:  Kto(actor, "Nie znajdujesz", "nie znajduje"); " niczego ciekawego.";
    }
  Mild:     "No właśnie.";
  Miscellany: switch (n) {
        1:  "(biorę pod uwagę tylko pierwszych szesnaście przedmiotów)^";
        2:  "Nie ma nic do zrobienia.";
        3:  print " "; Gracz("Zginąłeś", "Zginęłaś"); print " ";
        4:  print " "; Gracz("Zwyciężyłeś", "Zwyciężyłaś"); print " ";
        5:  print "^Czy chcesz zacząć od nowa (RESTART), wczytać zapisaną grę (WCZYTAJ)";
            #Ifdef DEATH_MENTION_UNDO;
            if (undo_flag ~= 0)
                print ", cofnąć ostatni ruch (COFNIJ)";
            #Endif;
            if (TASKS_PROVIDED == 0) print ", zobaczyć pełny wynik (PEŁNY)";
            if (deadflag == 2 && AMUSING_PROVIDED == 0)
                print ", zobaczyć kilka zabawnych pomysłów (ZABAWNE)";
            print " czy zakończyć (KONIEC)?";
        6:  "[Ten interpreter nie pozwala cofać ruchów. Przykro mi!]";
            #Ifdef TARGET_ZCODE;
        7:  "Nie udało się ~cofnąć~ ruchu. [Nie wszystkie interpretery to potrafią.]";
            #Ifnot; ! TARGET_GLULX
        7:  "[Nie można już niczego więcej ~cofnąć~.]";
            #Endif; ! TARGET_
        8:  "Wybierz, proszę, jedną z powyższych odpowiedzi.";
        9:  "^Zrobiło się zupełnie ciemno!";
        10: "Słucham?";
        11: "[Nie można ~cofnąć~ czegoś, co się jeszcze nie wydarzyło!]";
        12: "[Nie można ~cofnąć~ dwa razy z rzędu. Przykro mi!]";
        13: "[Poprzedni ruch został cofnięty.]";
        14: "Niestety, tego nie da się poprawić.";
        15: "Nie ma za co.";
        16: "~Ups~ poprawia tylko jedno słowo.";
        17: "Jest zupełnie ciemno i nic nie widzisz.";
        18: print "siebie";
        19: "Wyglądasz jak zwykle świetnie.";
        20: "Aby powtórzyć polecenie typu ~żabo, skacz~, napisz po prostu ~powtórz~,
             a nie ~żabo, powtórz~.";
        21: "Tego raczej nie da się powtórzyć.";
        22: "Polecenie nie może zaczynać się od przecinka.";
        23: "Wygląda na to, że chcesz z kimś porozmawiać, ale nie wiem z kim.";
        24: print "Nie możesz rozmawiać ";
            if (PL_Ze(x1, PRZYP_NAR)) print "ze "; else print "z ";
            print_ret (nar) x1, ".";
        25: "Aby z kimś porozmawiać, napisz na przykład ~kowalu, cześć~.";
        26: "(najpierw bierzesz ", (bie) x1, ")";
        27: "Nie zrozumiałem tego polecenia.";
        28: print "Zrozumiałem tylko tyle, że chcesz ";
        29: "Nie zrozumiałem tej liczby.";
        30: Kto(actor, "Nie widzisz", "nie widzi"); " niczego takiego.";
        31: "Chyba czegoś zabrakło w tym poleceniu.";
        32: "Nie trzymasz tego!";
        33: "Tego czasownika nie można użyć z kilkoma przedmiotami naraz.";
        34: "Kilku przedmiotów naraz można użyć tylko raz w poleceniu.";
        35: "Nie wiem, do czego odnosi się ~", (address) x1, "~.";
        36: "Wykluczasz coś, czego i tak nie było na liście.";
        37: "Można to zrobić tylko z kimś żywym.";
        38: "Nie znam takiego czasownika.";
        39: "W tej grze nie musisz się tym zajmować.";
        40: "Nie widzisz teraz ~", (address) x1, "~ (", (dop) x2, ").";
        41: "Nie zrozumiałem końcówki tego polecenia.";
        42: if (x1 == 0) "Żadna z tych rzeczy nie jest dostępna.";
            if (x1 == 1) "Jest tylko jedna sztuka.";
            print (nop) Liczebnik(x1, "", "Są", "Jest"), " tylko ", x1, " ";
            Liczebnik(x1, "sztuka", "sztuki", "sztuk"); ".";
        43: "Nie ma nic do zrobienia.";
        44: print "Nie ma tu nic, co można by ";
            if (~~LanguageVerb(x1)) print (address) x1;
            ".";
        ! 45, 46 i 57 otaczają listę obiektów w pytaniu parsera, które są
        ! drukowane w bierniku: "Co masz na myśli: mosiężną lampę czy ...?"
        45: short_name_case = PRZYP_BIE; print "Kogo masz na myśli: ";
        46: short_name_case = PRZYP_BIE; print "Co masz na myśli: ";
        47: "Niestety, tu można podać tylko jedną rzecz. Którą dokładnie?";
        48: PytanieOBrak(true, x1);
        49: PytanieOBrak(false, x1);
        50: print "Wynik właśnie ";
            if (x1 > 0) print "wzrósł"; else { x1 = -x1; print "zmalał"; }
            print " o ", (number) x1, " ";
            Liczebnik(x1, "punkt", "punkty", "punktów");
        51: "(Wydarzyło się coś ważnego, więc pozostałe polecenia zostały pominięte.)";
        52: "^Wpisz liczbę od 1 do ", x1, ", 0, aby ponownie wyświetlić menu, albo naciśnij ENTER.";
        53: "^[Naciśnij SPACJĘ.]";
        54: "[Komentarz zapisany.]";
        55: "[Komentarz NIE został zapisany.]";
        56: ".";
        57: short_name_case = PRZYP_MIAN; "?";
        58: print "(najpierw ";
            if (x2 has container) print "wyjmujesz "; else print "zdejmujesz ";
            "", (bie) x1, " ", (pl_z) x2, " ", (dop) x2, ")";
        59: "Musisz wyrazić się dokładniej.";
        60: print (CMian) x1, " zauważa, że ";
    }
  No,Yes:   "To było pytanie retoryczne.";
  NotifyOff:
            "Powiadamianie o wyniku wyłączone.";
  NotifyOn: "Powiadamianie o wyniku włączone.";
  Objects: switch (n) {
        1:  print (nop) Gracz("Miałeś", "Miałaś"), " w rękach";
            short_name_case = PRZYP_BIE;    ! lista w bierniku: "lampę sodową"
        2:  "Nie ", (nop) Gracz("miałeś", "miałaś"), " jeszcze niczego w rękach.";
        3:  print " (na sobie)";
        4:  print " (przy sobie)";
        5:  print " (oddane)";
        6:  print " (", (name) x1, ")";
        7:  print " (", (mian) x1, ")";
        8:  print " (", (pl_w) x1, " ", (mie) x1, ")";
        9:  print " (na ", (mie) x1, ")";
        10: print " (zgubione)";
    }
  Open: switch (n) {
        1:  "Tego nie da się otworzyć.";
        2:  print_ret (CMian) x1, " ", (JestSa) x1, " chyba ",
                (nop) PL_PrzymZ(x1, "zamknięt", "zamknięci"), " na klucz.";
        3:  print_ret (CMian) x1, " ", (JestSa) x1, " już ", (nop) PL_PrzymZ(x1, "otwart", "otwarci"), ".";
        4:  Kto(actor, "Otwierasz", "otwiera"); print " ", (bie) x1, ". W środku";
            if (PL_ListaZCzasownikiem(child(x1), ENGLISH_BIT+TERSE_BIT+CONCEAL_BIT+ISARE_BIT) == 0)
                " nic nie ma.";
            ".";
        5:  Kto(actor, "Otwierasz", "otwiera"); " ", (bie) x1, ".";
        6:  "(najpierw otwierasz ", (bie) x1, ")";
    }
  Order:    print_ret (CMian) x1, " ", (nop) PL(x1, "ma", "mają"), " ciekawsze rzeczy do roboty.";
  Places: switch (n) {
        1:  print "Odwiedzone miejsca";       ! biblioteka dopisze ":" tylko w układzie pionowym
            if (~~(places_style & INDENT_BIT)) print ":";
        2:  ".";
        3:  "Nie ", (nop) Gracz("odwiedziłeś", "odwiedziłaś"), " jeszcze żadnego miejsca.";
    }
  Pray:     "Modlitwa nie przynosi widocznego skutku.";
  Prompt:   print "^>";
  Pronouns: switch (n) {
        1:  print "W tej chwili ";
        2:  print "oznacza ";
            short_name_case = PRZYP_BIE;    ! "oznacza mosiężną lampę"
        3:  print "nic nie oznacza";
        4:  "gra nie zna żadnych zaimków.";
        5:  short_name_case = PRZYP_MIAN; ".";
    }
  Pull,Push,Turn: switch (n) {
        1:  "Karanie samego siebie w ten sposób niczego nie poprawi.";
        2:  print_ret (CMian) x1, " ", (JestSa) x1, " ", (nop) PL_PrzymZ(x1, "przymocowan", "przymocowani"), " na stałe.";
        3:  "Nie dasz rady tego zrobić.";
        4:  "Nic szczególnego się nie dzieje.";
        5:  "To byłoby niegrzeczne.";
        6:  DecideAgainst();
    }
! Push: see Pull
  PushDir: switch (n) {
        1:  "To nic by nie dało.";
        2:  "To nie jest kierunek.";
        3:  "Nie w tę stronę.";
    }
  PutOn: switch (n) {
        1:  "Musisz najpierw wziąć ", (bie) x1, ", żeby ", (GoJaJe) x1,
            " gdzieś położyć.";
        2:  "Nie można położyć czegoś na samym sobie.";
        3:  "Kładzenie czegokolwiek na ", (mie) x1, " nic by nie dało.";
        4:  "Brakuje ci zręczności.";
        5:  "(najpierw zdejmujesz ", (GoJaJe) x1, ")";
        6:  "Na ", (mie) x1, " nie ma już miejsca.";
        7:  "Zrobione.";
        8:  Kto(actor, "Kładziesz", "kładzie"); " ", (bie) x1, " na ", (mie) x2, ".";
    }
  Quit: switch (n) {
        1:  print "Odpowiedz ~tak~ lub ~nie~.";
        2:  print "Czy na pewno chcesz zakończyć grę? ";
    }
  Remove: switch (n) {
        1:  print_ret "Niestety, ", (mian) x1, " ", (JestSa) x1, " ", (nop) PL_PrzymZ(x1, "zamknięt", "zamknięci"), ".";
        2:  "Ale ", (JegoJejIch) x1, " tam teraz nie ma.";
        3:  "Wzięte.";
        4:  print_ret "Ale ", (mian) x1, " nie ", (nop) PL(x1, "znajduje", "znajdują"),
                " się ani w niczym, ani na niczym.";
    }
  Restart: switch (n) {
        1:  print "Czy na pewno chcesz zacząć od nowa? ";
        2:  "Nie udało się.";
    }
  Restore: switch (n) {
        1:  "Nie udało się wczytać gry.";
        2:  "Gotowe.";
    }
  Rub: switch (n) {
        1:  "W ten sposób niczego nie osiągniesz.";
        2:  DecideAgainst();
    }
  RunTimeError: print "** ";
    switch (n) {
    1:  print "Nie znaleziono przyimka (to nie powinno się zdarzyć)";
    2:  print "Wartość własności nie jest procedurą ani tekstem: ~", (property) x2,
            "~ obiektu ~", (name) x1, "~ (", x1, ")";
    3:  print "Element listy własności nie jest procedurą ani tekstem: ~",
            (property) x2, "~ obiektu ~", (name) x1, "~ (", x1, ")";
    4:  print "Zbyt wiele jednocześnie aktywnych zegarów/demonów.
            Limit to stała biblioteki MAX_TIMERS
            (obecnie ", MAX_TIMERS, "), którą należy zwiększyć";
    5:  print "Obiekt ~", (name) x1, "~ nie ma własności ~", (property) x2, "~";
    7:  print "Obiekt ~", (name) x1, "~ może być graczem tylko wtedy, gdy ma
            własność ~number~";
    8:  print "Próba wylosowania elementu z pustej tablicy";
    9:  print x1, " nie jest poprawnym numerem własności kierunku";
    10: print "Obiekt gracza jest poza drzewem obiektów";
    11: print "Pomieszczenie ~", (name) x1, "~ nie ma własności ~", (property) x2, "~";
    12: print "Próba ustawienia nieistniejącego zaimka w SetPronoun";
    13: print "Po tokenie 'topic' może występować tylko przyimek";
    14: print "Przepełnienie bufora o rozmiarze ", x1,
            " przy użyciu '@@64output_stream 3' ", (string) x2;
    15: print "LoopWithinObject przerwane, bo obiekt ",
            (name) x1, " został przeniesiony w trakcie pętli.";
    16: print "Niedozwolona wartość narrative_voice: ", x1, ".";
    default:
        print "(nieznany błąd)";
    }
    print " **";
  Save: switch (n) {
        1:  "Nie udało się zapisać gry.";
        2:  "Gotowe.";
    }
  Score: switch (n) {
        1:  if (deadflag) print "W tej grze "; else print "Do tej pory ";
            Gracz("zdobyłeś ", "zdobyłaś ");
            print score, " ";
            Liczebnik(score, "punkt", "punkty", "punktów");
            print " na ", MAX_SCORE, " możliw";
            Liczebnik(MAX_SCORE, "y", "e", "ych");
            print ", w ", turns, " ";
            Liczebnik(turns, "ruchu", "ruchach", "ruchach");
            return;
        2:  "W tej opowieści nie ma punktów.";
    }
  ScriptOff: switch (n) {
        1:  "Zapis przebiegu gry jest już wyłączony.";
        2:  "^Koniec zapisu przebiegu gry.";
        3:  "Nie udało się zakończyć zapisu przebiegu gry.";
    }
  ScriptOn: switch (n) {
        1:  "Zapis przebiegu gry jest już włączony.";
        2:  print "Początek zapisu przebiegu gry"; VersionSub();
        3:  "Nie udało się rozpocząć zapisu przebiegu gry.";
    }
  Search: switch (n) {
        1:  "Przecież jest ciemno.";
        2:  "Na ", (mie) x1, " nic nie ma.";
        3:  print "Na ", (mie) x1;
            PL_ListaZCzasownikiem(child(x1), ENGLISH_BIT+TERSE_BIT+CONCEAL_BIT+ISARE_BIT);
            ".";
        4:  Kto(actor, "Nie znajdujesz", "nie znajduje"); " niczego ciekawego.";
        5:  "Nie widać, co jest w środku, bo ", (mian) x1, " ", (JestSa) x1,
            " ", (nop) PL_PrzymZ(x1, "zamknięt", "zamknięci"), ".";
        6:  print_ret (CMian) x1, " ", (JestSa) x1, " ", (nop) PL_PrzymZ(x1, "pust", "puści"), ".";
        7:  print "W ", (mie) x1;
            PL_ListaZCzasownikiem(child(x1), ENGLISH_BIT+TERSE_BIT+CONCEAL_BIT+ISARE_BIT);
            ".";
    }
  Set:      "Tego nie da się ustawić.";
  SetTo:    "Tego nie da się na nic ustawić.";
  Show: switch (n) {
        1:  "Nie masz ", (dop) x1, ".";
        2:  "Na ", (mie) x1, " nie robi to wrażenia.";
    }
  Sing:     "Fałszujesz niemiłosiernie.";
  Sleep:    "Wcale nie chce ci się spać.";
  Smell: switch (n) {
        1:  Kto(actor, "Nie czujesz", "nie czuje"); " żadnego niezwykłego zapachu.";
        2:  DecideAgainst();
    }
  Sorry:    "Nie ma za co przepraszać.";
  Squeeze: switch (n) {
        1:  DecideAgainst();
        2:  "W ten sposób niczego nie osiągniesz.";
    }
  Strong:   "Prawdziwi poszukiwacze przygód nie używają takich słów.";
  Swim:     "Nie ma tu dość wody, żeby pływać.";
  Swing:    "Nie ma tu niczego, na czym można by się rozhuśtać.";
  SwitchOff: switch (n) {
        1:  "Tego nie da się wyłączyć.";
        2:  print_ret (CMian) x1, " ", (JestSa) x1, " już ", (nop) PL_PrzymZ(x1, "wyłączon", "wyłączeni"), ".";
        3:  Kto(actor, "Wyłączasz", "wyłącza"); " ", (bie) x1, ".";
    }
  SwitchOn: switch (n) {
        1:  "Tego nie da się włączyć.";
        2:  print_ret (CMian) x1, " ", (JestSa) x1, " już ", (nop) PL_PrzymZ(x1, "włączon", "włączeni"), ".";
        3:  Kto(actor, "Włączasz", "włącza"); " ", (bie) x1, ".";
    }

  Take: switch (n) {
        1:  "Wzięte.";
        2:  "Siebie masz zawsze przy sobie.";
        3:  print_ret (CMian) x1, " raczej by sobie tego nie ",
                (nop) Forma(x1, "życzył", "życzyła", "życzyło", "życzyły", "życzyli"), ".";
        4:  print "Najpierw musisz ";
            if (x1 has supporter) print "zejść "; else print "wyjść ";
            print_ret (pl_z) x1, " ", (dop) x1, ".";
        5:  "Już masz ", (bie) x1, ".";
        6:  print_ret (CMian) x2, " ", (nop) PL(x2, "należy", "należą"), " do ", (dop) x1, ".";
        7:  print_ret (CMian) x2, " ", (JestSa) x2, " częścią ", (dop) x1, ".";
        8:  print_ret (CMian) x1, " nie ", (JestSa) x1, " ", (nop) PL_PrzymZ(x1, "dostępn", "dostępni"), ".";
        9:  print_ret (CMian) x1, " nie ", (JestSa) x1, " ", (nop) PL_PrzymZ(x1, "otwart", "otwarci"), ".";
        10: "Tego nie da się przenieść.";
        11: print_ret (CMian) x1, " ", (JestSa) x1, " ", (nop) PL_PrzymZ(x1, "przymocowan", "przymocowani"), " na stałe.";
        12: "Masz już przy sobie za dużo rzeczy.";
        13: "(wkładasz ", (bie) x1, " do ", (dop) x2, ", żeby zrobić miejsce)";
    }
  Taste: switch (n) {
        1:  Kto(actor, "Nie czujesz", "nie czuje"); " niczego niezwykłego w smaku.";
        2:  DecideAgainst();
    }
  Tell: switch (n) {
        1:  "Przez chwilę mówisz sam", (nop) Gracz("", "a"), " do siebie.";
        2:  "Nie wywołuje to żadnej reakcji.";
    }
  Think:    "Świetny pomysł.";
  ThrowAt: switch (n) {
        1:  "To na nic.";
        2:  "W decydującej chwili brakuje ci odwagi.";
    }
  Tie: switch (n) {
        1:  "W ten sposób niczego nie osiągniesz.";
        2:  DecideAgainst();
    }
  Touch: switch (n) {
        1:  DecideAgainst();
        2:  Kto(actor, "Nie czujesz", "nie czuje"); " niczego niezwykłego.";
        3:  "To naprawdę nic by nie dało.";
    }
  Transfer: switch (n) {
        1:  "Nadal trzymasz ", (bie) x1, ".";
    }

! Turn: see Pull.
  Unlock:  switch (n) {
        1:  "Tego nie da się otworzyć kluczem.";
        2:  print_ret (CMian) x1, " nie ", (JestSa) x1, " teraz ", (nop) PL_PrzymZ(x1, "zamknięt", "zamknięci"), " na klucz.";
        3:  print_ret (CMian) x1, " nie ", (nop) PL(x1, "pasuje", "pasują"), " do zamka.";
        4:  Kto(actor, "Otwierasz", "otwiera"); print " zamek ", (dop) x1;
            if (second) print " ", (nar) second;
            ".";
        5:  print "(najpierw otwierasz zamek";      ! x2: klucz
            if (x2) print " ", (nar) x2; else print " ", (dop) x1;
            ")";
    }
  VagueGo:  "Musisz podać, w którą stronę chcesz iść.";
  Verify: switch (n) {
        1:  "Plik gry jest nienaruszony.";
        2:  "Plik gry nie przeszedł weryfikacji i może być uszkodzony.";
    }
  Wait:     "Mija trochę czasu.";
  Wake:     "Niestety, to nie jest sen.";
  WakeOther:"To chyba niepotrzebne.";
  Wave: switch (n) {
        1:  "Przecież ", (JegoJejIch) x1, " nie trzymasz.";
        2:  print "Wyglądasz śmiesznie, machając ", (nar) x1;
            if (x2) " w stronę ", (dop) x2, ".";
            ".";
        3:  DecideAgainst();
    }
  WaveHands:
        Kto(actor, "Machasz", "macha");
        switch (n) {
        1: ! nothing
        2: print " do ", (dop) x1;
        }
        " rękami, czując się trochę głupio.";
  Wear: switch (n) {
        1:  "Nie da się ", (JegoJejIch) x1, " założyć.";
        2:  "Nie trzymasz ", (JegoJejIch) x1, ".";
        3:  "Masz ", (GoJaJe) x1, " już na sobie.";
        4:  Kto(actor, "Zakładasz", "zakłada"); " ", (bie) x1, ".";
        5:  print "Najpierw musisz ";
            if (x2 has container) print "wyjąć "; else print "zdjąć ";
            print (bie) x1, " ", (pl_z) x2, " ", (dop) x2;
            ".";
    }
! Yes:  see No.
];

! ==============================================================================

Constant LIBRARY_ENGLISH;       ! for dependency checking.
#Endif;

! ==============================================================================
