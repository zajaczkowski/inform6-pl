! ==============================================================================
!   SZYFR:  zamki szyfrowe dla polskiej biblioteki inform6-pl
!
!   Dodatek, nie część biblioteki standardowej.  Copyright Bartosz Zajączkowski
!   2026.  Licencja: tradycyjna licencja Inform (DM4) lub Artistic License 2.0.
!   Instrukcja: extensions/szyfr_instrukcja.md.
!
!   Dołącza się po PolskiG:
!
!       Include "PolskiG";
!       Include "szyfr";
!
!   Obiekt zamykany na szyfr ma atrybuty lockable (i zwykle locked) oraz
!   właściwość with_code (odpowiednik with_key) - napis albo liczbę:
!
!       Object -> sejf "stalow/y sejf/"
!         with with_code "15432"
!         has  static container openable lockable locked;
!
!   Osobny obiekt zamka (klawiatura, tarcza) wskazuje zamykany obiekt
!   właściwością lock_for.  Polecenia: "otwórz sejf kodem 15432", "otwórz sejf
!   15432", "wpisz 15432", "wpisz kod 15432 na zamku", "zamknij sejf na szyfr",
!   "zamknij sejf kodem 15432".
! ==============================================================================

System_file;

#Ifndef with_code;
Property individual with_code;
#Endif;
#Ifndef lock_for;
Property individual lock_for;
#Endif;

! Cyfry wpisanego kodu.  Biblioteczny token number kończy się na 10000 i gubi
! zera z przodu, więc kod czytany jest jako tekst: "15432", "0042", "15-43-2",
! "15 43 2".  Mieści do 16 cyfr.
Array szyfr_cyfry -> 16;
Global szyfr_dl;
Array szyfr_wzor --> 20;

! Token gramatyki: jedno lub kilka słów złożonych z cyfr i myślników.
[ SzyfrKod  adr len i c n start;
    start = wn;
    while (wn <= NumberWords()) {
        adr = WordAddress(wn); len = WordLength(wn);
        for (i=0 : i<len : i++) {
            c = adr->i;
            if (~~((c >= '0' && c <= '9') || c == '-')) break;
        }
        if (i < len) break;
        for (i=0 : i<len : i++) {
            c = adr->i;
            if (c >= '0' && c <= '9' && n < 16) szyfr_cyfry->(n++) = c;
        }
        wn++;
    }
    if (n == 0) { wn = start; return GPR_FAIL; }
    szyfr_dl = n; parsed_number = n;    ! second > 0: kod padł w tym poleceniu
    return GPR_NUMBER;
];

! Reguła wydruku: wpisany kod ("15432").
[ SzyfrWpisany x  i; x = 0; for (i=0 : i<szyfr_dl : i++) print (char) szyfr_cyfry->i; ];

[ SzyfrWzor x; if (metaclass(x) == String) print (string) x; else print x; ];

! Czy wpisany kod jest równy x (napis albo liczba)?  Liczą się tylko cyfry.
! Kod podany jako liczba (42) nie zwraca uwagi na zera z przodu ("0042"),
! podany jako napis ("0042") - tak.  Gra może tego używać w swoich regułach:
! if (SzyfrWpisano("1111")) ...
[ SzyfrWpisano x  n i j c p;
    n = PL_Renderuj(szyfr_wzor, 20, SzyfrWzor, x);
    if (metaclass(x) ~= String)
        while (p < szyfr_dl - 1 && szyfr_cyfry->p == '0') p++;
    for (i=0 : i<n : i++) {
        c = szyfr_wzor-->i;
        if (c >= '0' && c <= '9') {
            if (p+j >= szyfr_dl || szyfr_cyfry->(p+j) ~= c) rfalse;
            j++;
        }
    }
    return p+j == szyfr_dl;
];

! Czy wpisany kod pasuje do kodu obiektu o?
[ SzyfrPasuje o; return SzyfrWpisano(o.with_code); ];

! Ile cyfr wpisał gracz (do użycia w regułach gry, np. before).
[ SzyfrDlugosc; return szyfr_dl; ];

! Obiekt zamykany przez zamek o: sam o albo o.lock_for.
[ SzyfrZamykany o; if (o provides lock_for) return o.lock_for; return o; ];

! ------------------------------------------------------------------------------
!   Akcje
! ------------------------------------------------------------------------------

! "otwórz sejf kodem 15432", "wpisz 15432 na zamku".  Dobry kod otwiera zamek
! i od razu sam obiekt (po cichu, z jego regułami dla Open); reguła after dla
! UnlockWithCode może potem zastąpić cały komunikat.
[ UnlockWithCodeSub  t ks otwarte;
    t = SzyfrZamykany(noun);
    if (~~(t provides with_code)) return L__M(##UnlockWithCode, 1, noun);
    if (t hasnt locked) {
        if (t has openable) <<Open t, actor>>;
        return L__M(##Unlock, 2, t);        ! "Kłódka nie jest zamknięta."
    }
    if (~~SzyfrPasuje(t)) return L__M(##UnlockWithCode, 2, t);
    give t ~locked;
    if (t has openable && t hasnt open) {
        ks = keep_silent; keep_silent = 1;
        <Open t, actor>;
        keep_silent = ks;
        otwarte = (t has open);
    }
    if (AfterRoutines()) return;
    if (otwarte) {
        L__M(##UnlockWithCode, 3, t);
        if (t has container) return L__M(##Open, 4, t);
        return L__M(##Open, 5, t);
    }
    L__M(##UnlockWithCode, 4, t);
];

! "zamknij sejf kodem 15432" (kod musi się zgadzać), "zamknij sejf na szyfr"
! (bez kodu; second == 0, bo w tym poleceniu nie było tokenu kodu)
[ LockWithCodeSub  t;
    t = SzyfrZamykany(noun);
    if (~~(t provides with_code)) return L__M(##UnlockWithCode, 1, noun);
    if (t has locked) return L__M(##Lock, 2, t);
    if (second && ~~SzyfrPasuje(t)) return L__M(##UnlockWithCode, 2, t);
    if (t has open && ImplicitClose(t)) return L__M(##Lock, 3, t);
    give t locked;
    if (AfterRoutines()) return;
    L__M(##LockWithCode, 1, t);
];

! "wpisz 15432" bez wskazania zamka: jedyny zamek szyfrowy w zasięgu.
[ EnterCodeSub  o t z n;
    objectloop (o provides with_code || o provides lock_for) {
        if (~~TestScope(o)) continue;
        t = SzyfrZamykany(o);
        if (n == 0) { z = t; n = 1; }
        else if (t ~= z) n = 2;
    }
    if (n ~= 1) return L__M(##UnlockWithCode, 5);
    <<UnlockWithCode z second, actor>>;
];

! ------------------------------------------------------------------------------
!   Komunikaty i bezokoliczniki (przez rozszerzenia biblioteki 6.12, bez zmian
!   w polski.h).  Gra może je zastąpić w LibraryMessages, jak każde inne.
! ------------------------------------------------------------------------------

Object szyfr_rozszerzenie "(szyfr)" LibraryExtensions
  with ext_messages [;
           UnlockWithCode:
               switch (lm_n) {
                 1: "Tego nie otwiera się kodem.";
                 2: Kto(actor, "Wprowadzasz", "wprowadza");
                    " kod ", (SzyfrWpisany) 0, ", ale nic się nie dzieje.";
                 3: "(najpierw wprowadzasz kod ", (SzyfrWpisany) 0, ")";
                 4: Kto(actor, "Wprowadzasz", "wprowadza");
                    " kod ", (SzyfrWpisany) 0, " i zamek ustępuje.";
                 5: "Gdzie chcesz wprowadzić ten kod?";
               }
           LockWithCode:
               Kto(actor, "Zamykasz", "zamyka"); print " ", (bie) lm_o;
               ". Zamek zatrzaskuje się.";
           ! Przy zamkach szyfrowych bez "na klucz": "Sejf jest zamknięty. Bez
           ! kodu nie da się go otworzyć."
           Open, Lock, Unlock:
               if (~~(lm_o ofclass Object && lm_o provides with_code)) rfalse;
               if (action == ##Open && lm_n == 2) {
                   print (CMian) lm_o, " ", (JestSa) lm_o, " ";
                   PL_PrzymZ(lm_o, "zamknięt", "zamknięci");
                   ". Bez kodu nie da się ", (JegoJejIch) lm_o, " otworzyć.";
               }
               if (action == ##Lock && lm_n == 2) {
                   print (CMian) lm_o, " ", (JestSa) lm_o, " już ";
                   PL_PrzymZ(lm_o, "zamknięt", "zamknięci");
                   ".";
               }
               if (action == ##Unlock && lm_n == 2) {
                   print (CMian) lm_o, " nie ", (JestSa) lm_o, " ";
                   PL_PrzymZ(lm_o, "zamknięt", "zamknięci");
                   ".";
               }
               if (action == ##Lock && lm_n == 6)
                   "Ten zamek otwiera się kodem, a nie kluczem.";
           ],
       ext_printverb [ v;
           switch (v) {
             'wpisz':    print "wpisać";
             'wprowadź': print "wprowadzić";
             'wystukaj': print "wystukać";
             default:    rfalse;
           }
           rtrue;
       ];

! ------------------------------------------------------------------------------
!   Gramatyka
! ------------------------------------------------------------------------------

Extend 'otwórz' first
    * noun 'kodem'/'szyfrem'/'kombinacją' SzyfrKod -> UnlockWithCode
    * noun SzyfrKod                               -> UnlockWithCode;

Extend 'zamknij'
    * noun 'kodem'/'szyfrem'/'kombinacją' SzyfrKod -> LockWithCode
    * noun 'na' 'kod'/'szyfr'                     -> LockWithCode;

Verb 'wpisz' 'wprowadź' 'wystukaj'
    * SzyfrKod                                    -> EnterCode
    * 'kod'/'szyfr'/'kombinację' SzyfrKod         -> EnterCode
    * SzyfrKod 'na'/'w//'/'do' noun               -> UnlockWithCode reverse
    * 'kod'/'szyfr'/'kombinację' SzyfrKod 'na'/'w//'/'do' noun -> UnlockWithCode reverse;
