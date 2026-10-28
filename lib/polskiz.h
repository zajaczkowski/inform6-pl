! ==============================================================================
!   POLSKIZ:  Polskie znaki dla Inform 6
!
!   Polska wersja biblioteki standardowej Inform 6.12.8
!   Copyright Bartosz Zajączkowski 2011-2026
!   Licencja: tradycyjna licencja Inform (DM4) lub Artistic License 2.0.
!
!   Ten plik trzeba dołączyć jako PIERWSZY, przed jakimkolwiek tekstem w grze
!   (także przed Constant Story), bo zmienia alfabet maszyny Z, a teksty
!   skompilowane wcześniej zostałyby zapisane starym alfabetem:
!
!       Include "PolskiZ";
!       Constant Story "...";
!       Include "Parser";
! ==============================================================================

System_file;

Constant POLSKI_ZNAKI;

#Ifdef TARGET_ZCODE;

! Polskie litery spoza ZSCII (ó jest już w domyślnej tablicy ISO 8859-1).
Zcharacter table + '@{105}' '@{107}' '@{119}' '@{142}' '@{144}' '@{15B}' '@{17A}' '@{17C}'
                   '@{104}' '@{106}' '@{118}' '@{141}' '@{143}' '@{15A}' '@{179}' '@{17B}';

! Słownik maszyny Z mieści 9 z-znaków na słowo. Litera spoza alfabetu zajmuje
! 4 z-znaki, więc końcówki polskich słów by się gubiły ('ścianę' = 'ściany').
! Polskie małe litery w alfabecie A1 zajmują 2 z-znaki. Z wielkich liter
! zostają te najczęstsze na początku polskich zdań, pozostałe zajmują 4.
Zcharacter "abcdefghijklmnopqrstuvwxyz"
           "ąćęłńóśźżABCDEIJKMNOPRSTWZ"
           "0123456789.,!?_#'/@{5C}-:()";

#Ifnot; ! TARGET_GLULX

#Iftrue (DICT_CHAR_SIZE ~= 4);
Message fatalerror "Polskie litery w słowniku Glulx wymagają opcji kompilatora $DICT_CHAR_SIZE=4";
#Endif;

#Endif; ! TARGET_
