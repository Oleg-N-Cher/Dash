MODULE Strings;

IMPORT SYSTEM;

PROCEDURE Length* (str: ARRAY OF CHAR): SHORTINT;
VAR
  len: SHORTINT;
BEGIN
  len := 0;
  WHILE (len < SHORT(LEN(str))) & (str[len] # 0X) DO INC(len) END;
  RETURN len
END Length;

PROCEDURE -_init* "/*-noinit*/";

END Strings.
