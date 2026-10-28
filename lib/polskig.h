! ==============================================================================
!   POLSKIG:  Gramatyka (czasowniki) biblioteki standardowej dla Inform 6
!
!   Polska wersja biblioteki standardowej Inform 6.12.8
!   Copyright Bartosz Zajączkowski 2011-2026
!   Kontynuacja inform-pl 6/11/PL; pochodzenie poszczególnych części
!   i wykorzystanie AI opisuje POCHODZENIE.md.
!
!   Oparte na pliku grammar.h z biblioteki 6.12.8
!   Copyright Graham Nelson 1993-2004 and David Griffith 2012-2025
!   Licencja: tradycyjna licencja Inform (DM4) lub Artistic License 2.0.
!
!   Plik w kodowaniu UTF-8 (kompilować z opcją -Cu).
!
!   W grze dołącz pliki biblioteki w tej kolejności:
!       Include "Parser";
!       Include "VerbLib";
!       Include "PolskiG";
!
!   Uwagi:
!   - Nazwy obiektów są rozpoznawane w każdym przypadku (name oraz name_dop,
!     name_cel itd. z polski.h), więc gramatyka używa zwykłych tokenów noun,
!     held, multi... bez rozróżniania przypadków.
!   - Słowo "się" jest usuwane z polecenia przed analizą (zob. polski.h,
!     LanguageToInformese), dlatego nie występuje w liniach gramatyki.
!   - Jednoliterowe przyimki zapisuje się z //: 'w//', 'z//', 'o//'.
! ==============================================================================

System_file;

#Ifdef      LIBRARY_STAGE;
#Iffalse    LIBRARY_STAGE >= AFTER_GRAMMAR;  ! if not already included
#Iftrue     LIBRARY_STAGE == AFTER_VERBLIB;  ! if okay to include it

! ------------------------------------------------------------------------------
!  Polecenia dotyczące samej gry (meta)
! ------------------------------------------------------------------------------

Verb meta 'opisy'
    *                                           -> LModeNormal
    * 'normalne'/'domyślne'                     -> LModeNormal
    * 'pełne'/'długie'                          -> LMode2
    * 'skrócone'/'krótkie'                      -> LMode3;

Verb meta 'brief'
    *                                           -> LMode1;

Verb meta 'verbose' 'long'
    *                                           -> LMode2;

Verb meta 'superbrief' 'short'
    *                                           -> LMode3;

Verb meta 'normal'
    *                                           -> LModeNormal;

Verb meta 'powiadamiaj' 'powiadomienia' 'notify'
    *                                           -> NotifyOn
    * 'włącz'/'tak'/'on'                        -> NotifyOn
    * 'wyłącz'/'nie'/'off'                      -> NotifyOff;

Verb meta 'zaimki' 'pronouns' 'nouns'
    *                                           -> Pronouns;

Verb meta 'koniec' 'zakończ' 'skończ' 'quit' 'q//' 'die'
    *                                           -> Quit;

Verb meta 'nagrywaj' 'recording'
    *                                           -> CommandsOn
    * 'włącz'/'on'                              -> CommandsOn
    * 'wyłącz'/'off'                            -> CommandsOff;

Verb meta 'odtwórz' 'replay'
    *                                           -> CommandsRead;

Verb meta 'restart' 'zrestartuj'
    *                                           -> Restart;

Verb meta 'wczytaj' 'załaduj' 'odczytaj' 'restore'
    *                                           -> Restore;

Verb meta 'zapisz' 'save'
    *                                           -> Save;

Verb meta 'wynik' 'punkty' 'score'
    *                                           -> Score;

Verb meta 'pełny' 'fullscore' 'full'
    *                                           -> FullScore
    * 'wynik'/'score'                           -> FullScore;

Verb meta 'zapis' 'transkrypcja' 'script' 'transcript'
    *                                           -> ScriptOn
    * 'włącz'/'on'                              -> ScriptOn
    * 'wyłącz'/'off'                            -> ScriptOff;

Verb meta 'noscript' 'unscript'
    *                                           -> ScriptOff;

Verb meta 'weryfikuj' 'zweryfikuj' 'verify'
    *                                           -> Verify;

Verb meta 'wersja' 'version'
    *                                           -> Version;

#Ifndef NO_PLACES;
Verb meta 'przedmioty' 'objects'
    *                                           -> Objects
    * 'pionowo'/'tall'                          -> ObjectsTall
    * 'poziomo'/'wide'                          -> ObjectsWide;
Verb meta 'miejsca' 'places'
    *                                           -> Places
    * 'pionowo'/'tall'                          -> PlacesTall
    * 'poziomo'/'wide'                          -> PlacesWide;
#Endif; ! NO_PLACES

! ------------------------------------------------------------------------------
!  Debugging grammar (bez tłumaczenia, jak w bibliotece standardowej)
! ------------------------------------------------------------------------------

#Ifdef DEBUG;
Verb meta 'actions'
    *                                           -> ActionsOn
    * 'on'                                      -> ActionsOn
    * 'off'                                     -> ActionsOff;

Verb meta 'changes'
    *                                           -> ChangesOn
    * 'on'                                      -> ChangesOn
    * 'off'                                     -> ChangesOff;

Verb meta 'gonear'
    * anynumber                                 -> GoNear
    * noun                                      -> GoNear;

Verb meta 'goto'
    * anynumber                                 -> Goto;

Verb meta 'random'
    *                                           -> Predictable;

Verb meta 'routines' 'messages'
    *                                           -> RoutinesOn
    * 'on'                                      -> RoutinesOn
    * 'verbose'                                 -> RoutinesVerbose
    * 'off'                                     -> RoutinesOff;

Verb meta 'scope'
    *                                           -> Scope
    * anynumber                                 -> Scope
    * noun                                      -> Scope;

Verb meta 'showdict' 'dict'
    *                                           -> ShowDict
    * topic                                     -> ShowDict;

Verb meta 'showobj'
    *                                           -> ShowObj
    * anynumber                                 -> ShowObj
    * multi                                     -> ShowObj;

Verb meta 'showverb'
    * special                                   -> ShowVerb;

Verb meta 'timers' 'daemons'
    *                                           -> TimersOn
    * 'on'                                      -> TimersOn
    * 'off'                                     -> TimersOff;

Verb meta 'trace'
    *                                           -> TraceOn
    * number                                    -> TraceLevel
    * 'on'                                      -> TraceOn
    * 'off'                                     -> TraceOff;

Verb meta 'abstract'
    * anynumber 'to' anynumber                  -> XAbstract
    * noun 'to' noun                            -> XAbstract;

Verb meta 'purloin'
    * anynumber                                 -> XPurloin
    * multi                                     -> XPurloin;

Verb meta 'tree'
    *                                           -> XTree
    * anynumber                                 -> XTree
    * noun                                      -> XTree;

#Ifdef TARGET_GLULX;
Verb meta 'glklist'
    *                                           -> Glklist;
#Endif; ! TARGET_

#Endif; ! DEBUG

! ------------------------------------------------------------------------------
!  Tokeny z wersji 6/11/PL, zachowane dla zgodności ze starszymi grami.
!  Wszystkie przypadki są teraz rozpoznawane przez zwykłe tokeny.
! ------------------------------------------------------------------------------

[ dopelniacznoun;     return ParseToken(ELEMENTARY_TT, NOUN_TOKEN); ];
[ celowniknoun;       return ParseToken(ELEMENTARY_TT, NOUN_TOKEN); ];
[ celownikcreature;   return ParseToken(ELEMENTARY_TT, CREATURE_TOKEN); ];
[ bierniknoun;        return ParseToken(ELEMENTARY_TT, NOUN_TOKEN); ];
[ biernikheld;        return ParseToken(ELEMENTARY_TT, HELD_TOKEN); ];
[ biernikcreature;    return ParseToken(ELEMENTARY_TT, CREATURE_TOKEN); ];
[ biernikmulti;       return ParseToken(ELEMENTARY_TT, MULTI_TOKEN); ];
[ biernikmultiheld;   return ParseToken(ELEMENTARY_TT, MULTIHELD_TOKEN); ];
[ biernikmultiexcept; return ParseToken(ELEMENTARY_TT, MULTIEXCEPT_TOKEN); ];
[ biernikmultiinside; return ParseToken(ELEMENTARY_TT, MULTIINSIDE_TOKEN); ];
[ narzedniknoun;      return ParseToken(ELEMENTARY_TT, NOUN_TOKEN); ];
[ narzednikheld;      return ParseToken(ELEMENTARY_TT, HELD_TOKEN); ];
[ miejscowniknoun;    return ParseToken(ELEMENTARY_TT, NOUN_TOKEN); ];

! ------------------------------------------------------------------------------
!  Czasowniki w grze (alfabetycznie według pierwszego słowa)
! ------------------------------------------------------------------------------

[ ADirection; if (noun in Compass) rtrue; rfalse; ];
[ Przelaczalny; return noun has switchable; ];
[ RozmowaSub;
    print "Żeby porozmawiać, zapytaj o coś (~zapytaj ", (bie) noun,
          " o drogę~) albo powiedz coś (~powiedz ", (cel) noun, " cześć~).^";
];
! Jak "powiedz X cześć": reguła life postaci dostaje Answer ze słowem 'cześć'
! w noun (nie przez <Answer ...>, bo słowo nie jest obiektem).
[ PozdrowSub o;
    o = noun; noun = 'cześć'; second = o;
    if (RunLife(o, ##Answer) == 0) L__M(##Answer, 1, noun);
    noun = o; second = 0;
];
[ NakarmSub; "Kogo chcesz nakarmić i czym? Na przykład: ~nakarm niedźwiedzia jedzeniem~."; ];
[ KrzykSub; "Krzyczysz na cały głos, ale nic z tego nie wynika."; ];
[ PomocSub;
    "To gra tekstowa. Wpisujesz polecenia, a gra opisuje, co się dzieje.
     Polecenia pisz w trybie rozkazującym, na przykład: ZBADAJ LAMPĘ, WEŹ KLUCZ,
     OTWÓRZ DRZWI KLUCZEM, IDŹ NA PÓŁNOC (albo krócej PN), ZAPYTAJ KAPŁANA
     O RUINY.^^
     Najczęstsze polecenia: ROZEJRZYJ SIĘ (L), ZBADAJ coś (X), EKWIPUNEK (I),
     WEŹ, UPUŚĆ, WŁÓŻ coś DO czegoś, POŁÓŻ coś NA czymś, OTWÓRZ, ZAMKNIJ,
     WŁĄCZ, CZEKAJ (C). Kierunki: PN, PD, W (wschód), Z (zachód), PNW, PNZ, PDW,
     PDZ, G (góra), D (dół).^^
     Gra: ZAPISZ, WCZYTAJ, COFNIJ, POWTÓRZ (P), WYNIK, RESTART, KONIEC.";
];
[ ZejdzSub; if (parent(player) ~= location) <<Exit>>; <<Go d_obj>>; ];
[ NotPlayer; if (noun ~= player) rtrue; rfalse; ];
[ IsPlayer; if (noun == player) rtrue; rfalse; ];

Verb 'ciągnij' 'pociągnij' 'szarpnij' 'wyrwij'
    * noun                                      -> Pull
    * 'za' noun                                 -> Pull;

Verb 'czekaj' 'poczekaj' 'zaczekaj' 'c//'
    *                                           -> Wait;

Verb 'czytaj' 'przeczytaj'
    * noun                                      -> Examine
    * 'o//' topic 'w//'/'we' noun               -> Consult
    * 'w//'/'we' noun 'o//' topic               -> Consult;

Verb 'daj' 'oddaj' 'podaj' 'wręcz' 'podaruj' 'zaoferuj'
    * creature held                             -> Give reverse
    * held creature                             -> Give;

! "nakarm niedźwiedzia jedzeniem"
Verb 'nakarm' 'karm'
    *                                           -> Nakarm
    * creature held                             -> Give reverse;

Verb 'dmuchnij' 'dmuchaj' 'dmij' 'zadmij' 'zagraj'
    * held                                      -> Blow
    * 'w//'/'we'/'na' held                      -> Blow;

Verb 'dotknij' 'dotykaj' 'pomacaj' 'pogłaszcz' 'poczuj'
    * noun                                      -> Touch;

Verb 'i//' 'inv' 'inwentarz' 'ekwipunek'
    *                                           -> Inv
    * 'pionowo'/'lista'                         -> InvTall
    * 'poziomo'/'opis'                          -> InvWide;

Verb 'idź' 'pójdź' 'biegnij' 'pobiegnij' 'ruszaj' 'udaj' 'przejdź' 'chodź'
    *                                           -> VagueGo
    * noun=ADirection                           -> Go
    * 'na'/'w//'/'do' noun=ADirection           -> Go
    * 'do'/'w//'/'we' noun                      -> Enter
    * 'na'/'przez' noun                         -> Enter
    * 'po' noun                                 -> Climb;

Verb 'kop' 'wykop' 'rozkop' 'odkop'
    *                                           -> Dig
    * noun                                      -> Dig
    * 'w//'/'we' noun                           -> Dig
    * noun held                                 -> Dig
    * 'w//'/'we' noun held                      -> Dig;

Verb 'kup' 'zakup' 'kupuj'
    * noun                                      -> Buy;

Verb 'kurczę' 'kurcze' 'rany' 'jejku' 'ojej' 'psiakość'
    *                                           -> Mild
    * topic                                     -> Mild;

Verb 'kurwa' 'cholera' 'kurde' 'psiakrew' 'chuj' 'gówno' 'shit' 'fuck' 'damn'
    *                                           -> Strong
    * topic                                     -> Strong;

Verb 'napełnij' 'nalej' 'wypełnij'
    * noun                                      -> Fill
    * noun noun                                 -> Fill
    * noun 'z//'/'ze' noun                      -> Fill
    * 'do' noun                                 -> Fill;

Verb 'napij' 'wypij' 'pij' 'łyknij'
    * noun                                      -> Drink
    * 'z//'/'ze' noun                           -> Drink;

Verb 'nie' 'n//'
    *                                           -> No;

Verb 'obejrzyj' 'zbadaj' 'opisz' 'ob' 'x//' 'przyjrzyj' 'zobacz' 'oglądaj'
    * noun                                      -> Examine;

Verb 'obudź' 'zbudź' 'ocknij' 'przebudź'
    *                                           -> Wake
    * creature                                  -> WakeOther;

Verb 'odpowiedz'
    * creature topic                            -> Answer reverse
    * topic 'do' creature                       -> Answer;

Verb 'opróżnij' 'wysyp' 'wylej' 'wypróżnij'
    * noun                                      -> Empty
    * noun 'do'/'na'/'w//'/'we' noun            -> EmptyT;

Verb 'otwórz' 'odsłoń' 'rozwiń' 'odblokuj' 'odklucz'
    * noun                                      -> Open
    * noun held                                 -> Unlock
    * noun 'za' 'pomocą' held                   -> Unlock;

Verb 'pchnij' 'popchnij' 'przesuń' 'naciśnij' 'wciśnij' 'przyciśnij'
    * noun                                      -> Push
    * noun noun=ADirection                      -> PushDir
    * noun 'na'/'w//'/'do' noun=ADirection      -> PushDir;

Verb 'płyń' 'popłyń' 'pływaj' 'zanurkuj' 'nurkuj'
    *                                           -> Swim;

Verb 'pocałuj' 'całuj' 'przytul' 'uściskaj' 'obejmij'
    * creature                                  -> Kiss;

Verb 'podpal' 'spal' 'rozpal'
    * noun                                      -> Burn
    * noun held                                 -> Burn;

Verb 'podskocz' 'skocz' 'skacz' 'podskakuj'
    *                                           -> Jump
    * 'przez'/'nad' noun                        -> JumpOver
    * 'do'/'w//'/'we' noun                      -> JumpIn
    * 'na' noun                                 -> JumpOn;

Verb 'pokaż' 'zaprezentuj'
    * creature held                             -> Show reverse
    * held creature                             -> Show;

Verb 'połóż' 'umieść' 'odłóż' 'zostaw'
    * multiheld                                 -> Drop
    * multiexcept 'na' noun                     -> PutOn
    * multiexcept 'do'/'w//'/'we' noun          -> Insert
    * 'na'/'w//'/'we' noun                      -> Enter;

Verb 'pomachaj' 'machaj' 'zamachaj'
    *                                           -> WaveHands
    * 'ręką'/'rękami'/'rękoma'                  -> WaveHands
    * 'ręką'/'rękami'/'rękoma' 'do' noun        -> WaveHands
    * 'do' noun                                 -> WaveHands
    * held                                      -> Wave
    * held 'do'/'na' noun                       -> Wave;

Verb 'pomódl' 'módl'
    *                                           -> Pray;

Verb 'pomyśl' 'myśl' 'zastanów' 'rozmyślaj'
    *                                           -> Think;

Verb 'poproś' 'proś'
    * creature 'o//' noun                       -> AskFor
    * creature 'żeby'/'aby' topic               -> AskTo;

Verb 'porozmawiaj' 'rozmawiaj' 'pogadaj' 'pomów'
    * 'z//'/'ze' creature                       -> Rozmowa
    * 'z//'/'ze' creature 'o//' topic           -> Tell;

! "co mam", "co mam przy sobie": ekwipunek
Verb 'co'
    * 'mam'                                     -> Inv
    * 'mam' 'przy' 'sobie'                      -> Inv
    * 'mam' 'ze' 'sobą'                         -> Inv
    * 'niosę'                                   -> Inv;

Verb 'pozdrów' 'przywitaj' 'powitaj'
    * creature                                  -> Pozdrow
    * 'z//'/'ze' creature                       -> Pozdrow;

Verb 'krzycz' 'krzyknij' 'wrzaśnij' 'zawołaj' 'wołaj' 'wrzeszcz'
    *                                           -> Krzyk
    * topic                                     -> Krzyk;

! Krótka pomoc; gra może podać własną: Extend 'pomoc' replace * -> Help;
Verb meta 'pomoc' 'help' 'instrukcja' 'podpowiedź' 'pomocy'
    *                                           -> Pomoc;

Verb 'postaw' 'ustaw' 'wstaw'
    * multiheld                                 -> Drop
    * multiexcept 'na' noun                     -> PutOn
    * multiexcept 'do'/'w//'/'we' noun          -> Insert;

Verb 'potrzyj' 'przetrzyj' 'wytrzyj' 'zetrzyj' 'wyczyść' 'wypoleruj' 'umyj'
    * noun                                      -> Rub;

Verb 'powąchaj' 'wąchaj' 'obwąchaj'
    *                                           -> Smell
    * noun                                      -> Smell;

Verb 'powiedz'
    * creature 'o//' topic                      -> Tell
    * creature topic                            -> Answer reverse
    * topic 'do' creature                       -> Answer;

Verb 'przekręć' 'obróć' 'zakręć' 'odkręć' 'dokręć' 'kręć'
    * noun                                      -> Turn;

Verb 'przenieś' 'przełóż' 'przesyp'
    * noun 'do'/'na'/'w//'/'we' noun            -> Transfer;

Verb 'przepraszam' 'przeproś' 'wybacz' 'sorry'
    *                                           -> Sorry;

Verb 'przeskocz'
    * noun                                      -> JumpOver
    * 'przez'/'nad' noun                        -> JumpOver;

Verb 'przeszukaj' 'przetrząśnij' 'obszukaj'
    * noun                                      -> Search;

Verb 'przetnij' 'tnij' 'potnij' 'rozetnij' 'przekrój' 'krój' 'pokrój'
    * noun                                      -> Cut
    * noun held                                 -> Cut;

Verb 'przywiąż' 'zwiąż' 'przymocuj' 'połącz' 'przyczep'
    * noun                                      -> Tie
    * noun 'do'/'z//'/'ze' noun                 -> Tie;

Verb 'rozejrzyj' 'l//' 'rozglądaj' 'opis'
    *                                           -> Look
    * 'dookoła'/'wokół'/'naokoło'               -> Look;

Verb 'rozhuśtaj' 'huśtaj' 'pobujaj' 'bujaj'
    * noun                                      -> Swing
    * 'na' noun                                 -> Swing;

Verb 'rzuć' 'ciśnij' 'wyrzuć' 'upuść' 'porzuć'
    * multiheld                                 -> Drop
    * held 'w//'/'we'/'przez' noun              -> ThrowAt
    * held creature                             -> ThrowAt
    * held 'do' creature                        -> ThrowAt
    * multiexcept 'do' noun                     -> Insert
    * multiexcept 'na' noun                     -> PutOn;

Verb 'schowaj' 'włóż' 'wsadź' 'wrzuć' 'wetknij'
    * held                                      -> Wear
    * multiexcept 'do'/'w//'/'we' noun          -> Insert
    * multiexcept 'pod'/'na' noun               -> PutOn;

Verb 'słuchaj' 'posłuchaj' 'nasłuchuj'
    *                                           -> Listen
    * noun                                      -> Listen;

Verb 'śpij' 'zaśnij' 'prześpij' 'zdrzemnij'
    *                                           -> Sleep;

Verb 'spójrz' 'popatrz' 'patrz' 'sp'
    *                                           -> Look
    * noun=ADirection                           -> Examine
    * 'na'/'w//' noun=ADirection                -> Examine
    * 'na' noun                                 -> Examine
    * 'w//'/'we'/'do'/'przez' noun              -> Search
    * 'pod'/'za' noun                           -> LookUnder;

Verb 'sprawdź' 'skonsultuj'
    * noun                                      -> Examine
    * topic 'w//'/'we' noun                     -> Consult;

Verb 'spróbuj' 'skosztuj' 'posmakuj' 'poliż'
    * noun                                      -> Taste;

Verb 'stań' 'usiądź' 'siądź' 'kładź'
    * 'na'/'w//'/'we' noun                      -> Enter;

Verb 'szukaj' 'poszukaj' 'odszukaj' 'znajdź'
    * topic 'w//'/'we' noun                     -> Consult
    * 'w//'/'we'/'na' noun                      -> Search
    * 'pod'/'za' noun                           -> LookUnder;

Verb 'ściśnij' 'zgnieć' 'ugnieć' 'zmiażdż'
    * noun                                      -> Squeeze;

Verb 'śpiewaj' 'zaśpiewaj' 'zanuć' 'nuć'
    *                                           -> Sing;

Verb 'tak' 't//'
    *                                           -> Yes;

Verb 'uderz' 'walnij' 'kopnij' 'zaatakuj' 'atakuj' 'zabij' 'zniszcz' 'rozbij' 'połam' 'bij'
    * noun                                      -> Attack
    * 'w//'/'we' noun                           -> Attack
    * noun held                                 -> Attack;

Verb 'wdrap' 'wespnij' 'wspinaj' 'wspnij'
    * 'na'/'po' noun                            -> Climb    ! pierwsza: "Na co chcesz się wspiąć?"
    * noun                                      -> Climb;

Verb 'weź' 'zabierz' 'podnieś' 'chwyć' 'złap' 'trzymaj'
    * multi                                     -> Take
    * multiinside 'z//'/'ze'/'spod' noun        -> Remove;

Verb 'wejdź' 'wsiądź' 'wskocz'
    *                                           -> GoIn
    * noun=ADirection                           -> Go
    * 'do'/'na'/'w//' noun=ADirection           -> Go
    * 'do'/'w//'/'we'/'na'/'przez' noun         -> Enter
    * 'po' noun                                 -> Climb;

Verb 'włącz' 'uruchom'
    * noun                                      -> SwitchOn;

! "zapal lampę" włącza ją, "zapal zapałkę/świecę" podpala.
Verb 'zapal' 'odpal'
    * noun=Przelaczalny                         -> SwitchOn
    * noun                                      -> Burn
    * noun held                                 -> Burn;

Verb 'wstań' 'wysiądź' 'wyjdź' 'zsiądź'
    *                                           -> Exit
    * noun=ADirection                           -> Go
    * 'na'/'w//'/'do' noun=ADirection           -> Go
    * 'z//'/'ze' noun                           -> Exit;

! "zejdź" bez dopełnienia: zejście z czegoś, a jeśli gracz na niczym nie
! stoi - zejście w dół ("zejdź" na szczycie schodów).
Verb 'zejdź' 'zleź'
    *                                           -> Zejdz
    * noun=ADirection                           -> Go
    * 'na'/'w//'/'do' noun=ADirection           -> Go
    * 'z//'/'ze' noun                           -> Exit
    * 'po'/'do'/'w//'/'we' noun                 -> Enter;

Verb 'wyjmij' 'wyciągnij' 'wydobądź' 'zdejmij' 'ściągnij'
    * held                                      -> Disrobe
    * multi                                     -> Take
    * multiinside 'z//'/'ze'/'spod' noun        -> Remove;

Verb 'wyłącz' 'zgaś'
    * noun                                      -> SwitchOff;

Verb 'zablokuj' 'zamknij' 'zasłoń' 'zakryj'
    * noun                                      -> Close
    * noun held                                 -> Lock
    * noun 'na' held                            -> Lock
    * noun 'za' 'pomocą' held                   -> Lock;

Verb 'zajrzyj'
    * 'do'/'w//'/'we'/'przez' noun              -> Search
    * 'pod'/'za' noun                           -> LookUnder;

Verb 'załóż' 'ubierz' 'nałóż' 'wdziej'
    * held                                      -> Wear;

Verb 'zapytaj' 'pytaj' 'spytaj'
    * creature 'o//' topic                      -> Ask;

Verb 'zjedz' 'jedz' 'połknij' 'ugryź' 'przełknij'
    * held                                      -> Eat;

Verb 'nastaw' 'wyreguluj' 'dostosuj'
    * noun                                      -> Set
    * noun 'na' special                         -> SetTo;

! ------------------------------------------------------------------------------
!  This routine is no longer used here, but provided to help existing games
!  which use it as a general parsing routine:

[ ConTopic w;
    consult_from = wn;
    do w = NextWordStopped();
    until (w == -1 || (w == 'do' && action_to_be == ##Answer));
    wn--;
    consult_words = wn - consult_from;
    if (consult_words == 0) return -1;
    if (action_to_be == ##Answer or ##Ask or ##Tell) {
        w = wn; wn = consult_from; parsed_number = NextWord();
        if (parsed_number == 'the' && consult_words > 1) parsed_number = NextWord();
        wn = w;
        return 1;
    }
    return 0;
];

! ------------------------------------------------------------------------------
!  Final task: provide trivial routines if the user hasn't already:
! ------------------------------------------------------------------------------

Default Story          0;
Default Headline       0;
Default d_obj          NULL;
Default u_obj          NULL;

Stub AfterLife         0;
Stub AfterPrompt       0;
Stub Amusing           0;
Stub BeforeParsing     0;
Stub ChooseObjects     2;
Stub DarkToDark        0;
Stub DeathMessage      0;
Stub Epilogue          0;
Stub GamePostRoutine   0;
Stub GamePreRoutine    0;
Stub InScope           1;
Stub LookRoutine       0;
Stub NewRoom           0;
Stub ObjectDoesNotFit  2;
Stub ParseNumber       2;
Stub ParserError       1;
Stub PrintTaskName     1;
Stub PrintVerb         1;
Stub TimePasses        0;
Stub UnknownVerb       1;
Stub AfterSave         1;
Stub AfterRestore      1;

#Ifdef TARGET_GLULX;
Stub HandleGlkEvent    2;
Stub IdentifyGlkObject 4;
Stub InitGlkWindow     1;
#Endif; ! TARGET_GLULX

#Ifndef PrintRank;
[ PrintRank; "."; ];
#Endif;

#Ifndef ParseNoun;
[ ParseNoun obj; obj = obj; return -1; ];
#Endif;

#Ifdef INFIX;
Include "infix";
#Endif;

Object  pl_rozszerzenie LibraryExtensions
  with  ext_beforeparsing [; return PL_NieznaneSlowa(); ];

! Gdy jedno słowo pasuje do kilku obiektów ("wschód": wschód, północny wschód,
! południowy wschód), parser wybiera ten, którego nazwa jest dokładnie tym
! słowem.  Biblioteczne MatchWord porównuje znaki dosłownie; polska wersja
! pomija ogonki i wielkość liter, żeby "wschod" też wybierało wschód.
[ MatchWord p_obj p_word_no  len_name len_input start_input i;
    p_word_no--;
    PrintToBuffer(StorageForShortName, SHORTNAMEBUF_LEN, p_obj);
    len_name = StorageForShortName-->0;
    if (len_name > 9) {
        for (i=9 : i<len_name : i++)
            if (StorageForShortName->(WORDSIZE+i) == ' ') rfalse;
        len_name = 9;
    }
    #Ifdef TARGET_ZCODE;
    start_input = (parse+2)->(4*p_word_no+3);
    len_input = (parse+2)->(4*p_word_no+2);
    #Ifnot; ! TARGET_GLULX
    start_input = parse-->((p_word_no+1)*3) - 2 + 2;
    len_input = parse-->((p_word_no+1)*3 - 1);
    #Endif; ! TARGET_
    if (len_input > 9) len_input = 9;
    if (len_name ~= len_input) rfalse;
    for (i=0 : i<len_name : i++)
        if (BezOgonka(LowerCase(StorageForShortName->(WORDSIZE+i))) ~=
            BezOgonka(LowerCase(buffer->(start_input+i))))
            rfalse;
    rtrue;
];

[ Identical o1 o2;                                     ! zob. odmiana.h
    if (~~PL_Identical(o1, o2)) rfalse;
    return PL_IdenticalOryg(o1, o2);
];

! Locale z biblioteki 6.12.8 z polskim tekstem zamiast wpisanego na sztywno
! "On the ..." / "In the ..." (rzeczy na dekoracjach-podstawkach i w nich).
! Przy "Replace X Y" wygrywa ostatnia definicja, stąd ta kopia tutaj.
[ Locale descin text_without_ALSO text_with_ALSO
    o p num_objs must_print_ALSO x flag;

    objectloop (o in descin) {
        if (o has scenery && o ~= parent(player) && (o has supporter || (o has container && o has open or transparent)) && children(o) > 0) {
	    flag = 0;
	    objectloop (x in o) {
		if (x hasnt scenery && x hasnt concealed)
		    flag++;
	    }
	    if (flag > 0) {
		if (num_objs == 0)
		    print "^";
		else
		    print " ";

		if (o has supporter)
		    print "Na ", (mie) o;
		else
		    print (string) pl_W_Duze(o), (mie) o;

		PL_ListaZCzasownikiem(child(o), ENGLISH_BIT+TERSE_BIT+CONCEAL_BIT+ISARE_BIT);
		print ".";
		num_objs++;
	    }
	}
    }

    if (num_objs > 0)
	print "^";

    objectloop (o in descin)
	give o ~workflag;
    num_objs = 0;
    objectloop (o in descin) {
	if (o hasnt concealed && NotSupportingThePlayer(o)) {
#Ifndef MANUAL_PRONOUNS;
	    PronounNotice(o);
#Endif;
	    if (o hasnt scenery) {
		give o workflag;
		num_objs++;
		p = initial;
		if ((o has door or container) && o has open && o provides when_open) {
		    p = when_open;
		    jump Prop_Chosen;
		}
		if ((o has door or container) && o hasnt open && o provides when_closed) {
		    p = when_closed;
		    jump Prop_Chosen;
		}
		if (o has switchable && o has on && o provides when_on) {
		    p = when_on;
		    jump Prop_Chosen;
		}
		if (o has switchable && o hasnt on && o provides when_off)
		    p = when_off;
.Prop_Chosen;
		if (o.&describe && RunRoutines(o, describe)) {
		    must_print_ALSO = true;
		    give o ~workflag;
		    num_objs--;
		    continue;
		}
		if (o.p && (o hasnt moved || p ~= initial)) {
		    new_line;
		    PrintOrRun(o, p);
		    must_print_ALSO = true;
		    give o ~workflag;
		    num_objs--;
		    if (o has supporter && child(o))
			SayWhatsOn(o);
		}
	    }
	}
    }

    if (num_objs == 0)
	return 0;
    if (actor ~= player)
	give actor concealed;
    if (text_without_ALSO) {
	new_line;
	if (must_print_ALSO)
	    print (string) text_with_ALSO, " ";
	else
	    print (string) text_without_ALSO, " ";

	WriteListFrom(child(descin), ENGLISH_BIT+RECURSE_BIT+PARTINV_BIT+TERSE_BIT+CONCEAL_BIT+WORKFLAG_BIT);
    } else {
	if (must_print_ALSO)
	    L__M(##Look, 5, descin);
	else
	    L__M(##Look, 6, descin);
    }
    if (actor ~= player)
	give actor ~concealed;
    return num_objs;
];


[ pl_W_Duze o; if (o has w_article) return "We "; return "W "; ];

! ==============================================================================

Undef LIBRARY_STAGE; Constant LIBRARY_STAGE = AFTER_GRAMMAR;

#Ifnot;     ! LIBRARY_STAGE < AFTER_GRAMMAR but ~= AFTER_VERBLIB
Message "Błąd: przed PolskiG trzeba poprawnie dołączyć VerbLib. To spowoduje wiele błędów!";
#Endif;

#Ifnot;     ! LIBRARY_STAGE >= AFTER_GRAMMAR : already included
Message "Ostrzeżenie: PolskiG dołączono dwukrotnie; drugie dołączenie jest pomijane.";
#Endif;

#Ifnot;     ! LIBRARY_STAGE is not defined
Message "Błąd: przed PolskiG trzeba poprawnie dołączyć Parser, a potem VerbLib. To spowoduje wiele błędów!";
#Endif;

! ==============================================================================
