MODULE Control;
IMPORT GrApp;

CONST
  (* ����������� � ������� �������� ��������� (��� ��������) *)
  Right* = 0;
  Left*  = 1;
  Down*  = 2;
  Up*    = 3;
  Fire*  = 4;
  Esc*   = 5;

TYPE
  Key* = CHAR;
  Keys* = SET;

PROCEDURE Get* (): Keys; BEGIN RETURN {}                END Get;
PROCEDURE GetCustom* (): Keys; BEGIN RETURN {}          END GetCustom;
PROCEDURE ReadKey* (): Key; BEGIN RETURN 0X             END ReadKey;
PROCEDURE PressedAnyKey* (): BOOLEAN; BEGIN RETURN FALSE END PressedAnyKey;
PROCEDURE Select* (methodN: (* {0..3} *) SHORTINT);      END Select;
PROCEDURE Close* ;                                       END Close;

END Control.
