MODULE Labirint; (** portable *)
IMPORT
  Scr := GrApp, Gr := GrTiles, Rsrc, Ctrl := Control, Timer, Sound;

CONST
  MaxMonsters  = 128;
  FieldWidth   =  16;
  FieldHeight  =  16;
  FieldSize    = FieldWidth * FieldHeight;

  None         =  0;
  Grass        =  1;
  Stone        =  2;
  Almas        =  3;
  StopMan      =  4;
  Wall         =  5;
  Mina         =  6;
  Babo         =  7;
  LeftMan      =  8;
  LeftMan1     =  9;
  RightMan     = 10;
  RightMan1    = 11;
  UpMan        = 12;
  UpMan1       = 13;
  DownMan      = 14;
  DownMan1     = 15;
  Mina1        = 16;
  Babo1        = 17;
  LastSprite   = Babo1;
  ActiveStone1 = LastSprite + 1;
  ActiveStone2 = LastSprite + 2;
  ActiveAlmas1 = LastSprite + 3;
  ActiveAlmas2 = LastSprite + 4;

  UserNoKeys = {}; (*Ctrl.NoKeys*)
  UserUp     = Ctrl.Up;
  UserDown   = Ctrl.Down;
  UserLeft   = Ctrl.Left;
  UserRight  = Ctrl.Right;
  UserAbort  = Ctrl.Esc;

  SideLeft   = -2;
  SideRight  =  2;

TYPE
  NatInt = Rsrc.NATINT; (* Native integer *)
  Coords = NatInt;      (* Coordinates in labirinth *)
  Cell   = NatInt;      (* An object of labirinth   *)
  Level  = NatInt;      (* A number of level (room) *)
  Index  = INTEGER;     (* Must be enough for field *)
  Monstr = RECORD x, y, dx, dy: Coords END;

VAR
  field: ARRAY FieldSize OF Cell;
  monstr: ARRAY MaxMonsters OF Monstr;
  monstrsNum: NatInt;
  objX, objY: Coords; objIdx: Index; objCell: Cell;
  manX, manY, manDirX, manDirY: Coords;
  manPresent: BOOLEAN;
  lives, score: INTEGER;(*BYTE?*)

(* -------------------------------------------------------------------------------- *)
(* Base procedures for manipulating cell array, unpack levels and display labirinth *)
(* -------------------------------------------------------------------------------- *)
PROCEDURE GetIndex (x, y: Coords): Index; (*?*)
(* Converts coords x, y to field index *)
BEGIN
  RETURN ((y DIV 2) * FieldWidth) + x DIV 2
END GetIndex;

PROCEDURE SetCell (x, y: Coords; cell: Cell); (*?*)
BEGIN
  field [((y DIV 2) * FieldWidth) + x DIV 2] := cell
END SetCell;

PROCEDURE GetCell (x, y: Coords): Cell; (*?*)
BEGIN
  RETURN field [((y DIV 2) * FieldWidth) + x DIV 2]
END GetCell;

PROCEDURE Unpack (room: Level);
VAR
  level: Rsrc.Resource; idx: Index; cell: Cell;
  bitsCount, n: (* UNSIGNED *) SHORTINT; val: NatInt;
BEGIN
  level := Rsrc.OpenAt(room * Rsrc.LevelSize, Rsrc.Levels);
  idx := 0; val := Rsrc.ReadByte(level); bitsCount := 8;
  REPEAT
    (* Unpacking and normalizing 3 bits from package: *)
    cell := 0;
    FOR n := 2 TO 0 BY -1 DO
      IF bitsCount = 0 THEN val := Rsrc.ReadByte(level); bitsCount := 8 END;
      DEC(bitsCount); (* INC(cell, SHORT(SHORT(ASH(val MOD 2, n)))) *)
      cell := cell DIV 2 + 4*(val MOD 2); val := val DIV 2;
    END;
    field[idx] := cell; INC(idx);
  UNTIL idx = FieldSize;
  Rsrc.Close(level);
END Unpack;

PROCEDURE TwoStep (dy: Coords);
BEGIN
  Timer.Start(1);
  dy := dy * 6;
  IF dy >= 0 THEN
    Scr.ScrollUp(dy)
  ELSE
    Scr.ScrollDown(-dy)
  END;
  Timer.Until;
  Timer.Start(1);
  IF dy >= 0 THEN
    Scr.ScrollUp(dy)
  ELSE
    Scr.ScrollDown(-dy)
  END
END TwoStep;

PROCEDURE Display;
VAR
  x, y: Coords;
BEGIN
  y := 0;
  WHILE y < FieldHeight * 2 DO
    TwoStep(+1);
    x := 0;
    WHILE x < FieldWidth * 2 DO
      Gr.DrawTile(x, (FieldHeight-1) * 2, Rsrc.GetTileByNum( GetCell(x, y)) );
      INC(x, 2);
    END;
    Scr.Redraw;
    INC(y, 2);
    Timer.Until;
  END;
END Display;

PROCEDURE DisplayInf;
VAR
  i: INTEGER;
BEGIN
  TwoStep(-1);
  (*strcpy (mmm, _M[LIVES_N]);
  strcat (mmm, dec_asciz (lives));
  strcat (mmm, _M[SCORE_N]);
  strcat (mmm, dec_asciz (score));
  center (0, mmm);*)

  Timer.Until;
  i := 1;
  WHILE (i < FieldHeight) DO
    TwoStep(-1);
    Timer.Until;
    INC(i)
  END;
  Timer.Start(18);
  Timer.Until;
END DisplayInf;

PROCEDURE LookForObjects;
VAR
  x, y: Coords;
  idx: Index;
BEGIN
  monstrsNum := 0; (*Выловим координаты человека, бабочек и мин*)
  manPresent := FALSE;
  idx := FieldSize;
  y := FieldHeight * 2;
  REPEAT
    DEC(y, 2); x := FieldWidth * 2;
    REPEAT
      DEC(x, 2); DEC(idx);
      CASE field[idx] OF
        Mina, Babo :
          monstr[monstrsNum].x := x;
          monstr[monstrsNum].y := y;
          monstr[monstrsNum].dx := 0;
          monstr[monstrsNum].dy := 0;
          INC(monstrsNum)
      | StopMan :
          manPresent := TRUE;
          manX := x; manY := y;
      ELSE END
    UNTIL x = 0
  UNTIL y = 0
END LookForObjects;

PROCEDURE SceneOfDeath (x, y: Coords);
(*Засыпание алмазами человечка, на которого наехала бабочка или мина,
и человечка, бабочки или мины, на которых упал камень или алмаз*)
VAR
  plusX, minusX, plusY, minusY, tX, tY: Coords; ctr, lim: INTEGER;
BEGIN
  (*unsigned xa, ya,*)
  IF x > 2 THEN minusX := x - 2 ELSE minusX := 0 END;
  IF y > 2 THEN minusY := y - 2 ELSE minusY := 0 END;
  IF x > (FieldWidth-2) * 2 THEN plusX := (FieldWidth-1) * 2 ELSE plusX := x + 2 END;
  IF y > (FieldHeight-2) * 2 THEN plusY := (FieldHeight-1) * 2 ELSE plusY := y + 2 END;

  tY := minusY;
  WHILE tY <= plusY DO
    tX := minusX;
    WHILE tX <= plusX DO
      Gr.DrawTile(tX, tY, Rsrc.Almas);
      SetCell(tX, tY, Almas);
      INC(tX, 2)
    END;
    INC(tY, 2)
  END;
(*
 unsigned ctr = y << 2, lim = ctr + 16;

 for (;ctr < lim; ctr ++)
  snd (((ctr >> 3) | (ctr << 5)) & 0xFF, 8); (*Terrible Sound Of Death*)
*)
(*
  ctr := y * 4; lim := ctr + 16;
  WHILE ctr < lim DO
    Sound.Beep(24858 DIV (((ctr DIV 8) + (ctr * 32)) MOD 256) * 8, 3); (*Terrible Sound Of Death*)
    (*Sound.Beep(((ctr >> 3) | (ctr << 5)) & 0xFF, 8); (*Terrible Sound Of Death*)*)
    INC(ctr)
  END
*)
END SceneOfDeath;

(* -------------------------------------------------------------------------------- *)
(* ---------------- Stone and almas functionality - drop if it can ---------------- *)
(* -------------------------------------------------------------------------------- *)
PROCEDURE Deactivate;
(*Для летящих в клетке камней и алмазов - прилет на опору*)
BEGIN
  IF objCell = ActiveStone2 THEN
    Gr.DrawTile(objX, objY, Rsrc.Stone);
    (*SetCell(objX, objY, Stone)*)
    field[objIdx] := Stone
  ELSE
    Gr.DrawTile(objX, objY, Rsrc.Almas);
    (*SetCell(objX, objY, Almas)*)
    field[objIdx] := Almas
  END;
(*
  Sound.Beep(24858 DIV (objY + 100) * 4, 10) (*, snd (200 + (y << 1), 10),*)
*)
  (*деактивация и звук падения. Камень или алмаз теперь лежит*)
END Deactivate;

PROCEDURE CanFlyThru (): BOOLEAN;
(*Для летящих в клетке камней и алмазов - разрешение пролета вниз дальше*)
BEGIN
  IF objY = (FieldHeight-1) * 2 THEN
    RETURN FALSE (*Нельзя - упали на границу лабиринта*)
  END;
  CASE (*GetCell(objX, objY + 2)*)field[objIdx + FieldWidth] OF
    Wall, Grass, Stone, Almas, ActiveStone1, ActiveAlmas1, ActiveStone2, ActiveAlmas2 :
      RETURN FALSE (*Нельзя - упали на траву, стену, камень или алмаз*)
  ELSE END;
  RETURN TRUE (*Можно - впереди пусто или убиваемый объект*)
END CanFlyThru;

PROCEDURE NextFall;
(*Для летящих в клетке камней - переход в положение между двумя клетками*)
(* WARNING: This procedure may change global vars objX, objIdx *)
BEGIN
  CASE (*GetCell(objX, objY + 2)*)field[objIdx + FieldWidth] OF
    None :
      Gr.DrawTile(objX, objY, Rsrc.None);
      IF objCell = ActiveStone2 THEN
        Gr.DrawTile(objX, objY + 1, Rsrc.Stone);
        (*SetCell(objX, objY, ActiveStone1);*)
        field[objIdx] := ActiveStone1;
        (*SetCell(objX, objY + 2, ActiveStone1)*)
        field[objIdx + FieldWidth] := ActiveStone1
      ELSE
        Gr.DrawTile(objX, objY + 1, Rsrc.Almas);
        (*SetCell(objX, objY, ActiveAlmas1);*)
        field[objIdx] := ActiveAlmas1;
        (*SetCell(objX, objY + 2, ActiveAlmas1)*)
        field[objIdx + FieldWidth] := ActiveAlmas1
      END
  | Babo, Mina, Babo1, Mina1,
    StopMan,
    UpMan, UpMan1, DownMan, DownMan1,
    RightMan, RightMan1, LeftMan, LeftMan1 :
      SceneOfDeath(objX, objY + 2);
      INC(objX, 2); INC(objIdx)
  ELSE END
END NextFall;

PROCEDURE CanStartFall (): BOOLEAN;
(*Разрешение падения прямо вниз из неподвижного положения*)
BEGIN
  IF objY < (FieldHeight-1) * 2 THEN
    IF (*GetCell(objX, objY + 2)*)field[objIdx + FieldWidth] = None THEN
      RETURN TRUE
    END
  END;
  RETURN FALSE
END CanStartFall;

PROCEDURE CanRollFrom (side: Coords): BOOLEAN;
(*Разрешение скатывания в сторону (side = SIDE_LEFT или SIDE_RIGHT)*)
VAR
  x: Coords;
BEGIN
  x := objX + side;
  IF (x >= 0) & (x <= (FieldWidth-1) * 2) & (objY >= 0) & (objY < (FieldHeight-1) * 2) THEN
    IF (GetCell(x, objY) = None)
     & (GetCell(x, objY + 2) = None) THEN
      RETURN TRUE
    END
  END;
  RETURN FALSE
END CanRollFrom;

PROCEDURE RollFrom (side: Coords);
(*Скатывание в сторону side на 1 клетку*)
(* WARNING: This procedure may change global vars objX, objIdx *)
BEGIN
  (*SetCell(objX, objY, None);*)
  field[objIdx] := None;
  Gr.DrawTile(objX, objY, Rsrc.None);
  (*SetCell(objX + side, objY, objCell);*)
  field[objIdx + side DIV 2] := objCell;
  Gr.DrawTile(objX + side, objY, Rsrc.GetTileByNum( objCell ));
(*Специфично для скатывания вправо - чтобы этот камень или алмаз мог только
скатиться и не был повторно обработан в следующей клетке - мы его новую
клетку пропускаем*)
  IF side = SideRight THEN INC(objX, SideRight); INC(objIdx) END
END RollFrom;

PROCEDURE Activate;
(*Перевод неподвижного камня или алмаза в активное состояние и*)
(*в положение между 2 клетками*)
BEGIN
  IF objCell = Stone THEN (*Занимает 2 клетки*)
    Gr.DrawTile(objX, objY, Rsrc.None); Gr.DrawTile(objX, objY + 1, Rsrc.Stone);
    (*SetCell(objX, objY, ActiveStone1);*)
    field[objIdx] := ActiveStone1;
    (*SetCell(objX, objY + 2, ActiveStone1)*)
    field[objIdx + FieldWidth] := ActiveStone1
  ELSE
    Gr.DrawTile(objX, objY, Rsrc.None); Gr.DrawTile(objX, objY + 1, Rsrc.Almas);
    (*SetCell(objX, objY, ActiveAlmas1);*)
    field[objIdx] := ActiveAlmas1;
    (*SetCell(objX, objY + 2, ActiveAlmas1)*)
    field[objIdx + FieldWidth] := ActiveAlmas1
  END
END Activate;

(* -------------------------------------------------------------------------------- *)
(* ---------------- Man functionality - orders of live man's world ---------------- *)
(* -------------------------------------------------------------------------------- *)
PROCEDURE GetManSprite (): Cell;
(*Номер спрайта первой фазы человечка для заданного направления движения*)
BEGIN
  IF manDirX = 0 THEN (* Движение по вертикали *)
    CASE manDirY OF
      1  : RETURN DownMan (*вниз*)
    | -1 : RETURN UpMan   (*вверх*)
    ELSE RETURN StopMan  (*на месте (не бывает)*)
    END
  END;
  IF manDirY = 0 THEN (*Движение по горизонтали*)
    CASE manDirX OF
      1  : RETURN RightMan (*вправо*)
    | -1 : RETURN LeftMan  (*влево*)
    ELSE RETURN StopMan   (*на месте (не бывает)*)
    END
  END;
  RETURN None (*Неверная ситуация (не бывает)*)
END GetManSprite;

PROCEDURE ManCanGo (): BOOLEAN;
(*Разрешение прохода в данном направлении*)
VAR
  x, y, tX, tY: Coords; cell: Cell;
BEGIN
  x := manX + manDirX + manDirX;
  y := manY + manDirY + manDirY;
  IF (x >= 0) & (x < FieldWidth * 2) & (y >= 0) & (y < FieldHeight * 2) THEN
    CASE GetCell(x, y) OF
      Wall, ActiveStone1, ActiveAlmas1 :
        RETURN FALSE (*Пройти нельзя*)
    | Stone, ActiveStone2 :
        (*unsigned tx, ty, z;*) (*Попробуем протолкнуть камни*)
        tX := x; tY := y;
        LOOP
          IF (tX >= 0) & (tX < FieldWidth * 2) & (tY >= 0) & (tY < FieldHeight * 2) THEN
            cell := GetCell(tX, tY);
            IF (cell # Stone) & (cell # ActiveStone2) THEN
              IF cell = None THEN EXIT END;
              RETURN FALSE (*Протолкнуть камни нельзя*)
            END;
            INC(tX, manDirX + manDirX); INC(tY, manDirY + manDirY) (*?*)
          ELSE
            RETURN FALSE (*Протолкнуть камни нельзя*)
          END
        END;
        SetCell(tX, tY, Stone); SetCell(x, y, None);
        Gr.DrawTile(tX, tY, Rsrc.Stone);
        Gr.DrawTile(x, y, Rsrc.None)
    ELSE END;
    RETURN TRUE (*Иначе - пройти в таком направлении можно*)
  END;
  RETURN FALSE
END ManCanGo;

PROCEDURE ManGoing (x, y: Coords): BOOLEAN;
(*Перевод человечка из текущей клетки в положение между двумя клетками*)
(*Разрешение дальнейшего существования человечка*)
BEGIN
  CASE GetCell(x, y) OF
    Mina, Babo, (*StopMan,*) Mina1, Babo1 :
      SceneOfDeath(x, y);
      RETURN FALSE
  | Almas, ActiveAlmas2 :
      INC(score);
      (* snd (60 + ((xn + yn) << 1), 8);*)
(*
      Sound.Beep(24858 DIV (x+y+30) * 4, 8);
*)
  ELSE END;
  RETURN TRUE
END ManGoing;

PROCEDURE TryMoveMan ();
VAR
  x, y: Coords; manSprite: Cell;
BEGIN
  IF ManCanGo() THEN
    x := manX + manDirX + manDirX;
    y := manY + manDirY + manDirY;
    manPresent := ManGoing(x, y);
    IF ~manPresent THEN RETURN (*СМЕРТЬ - ВЫХОД*) END;
      manSprite := GetManSprite();
      SetCell(x, y, manSprite); (*Отметить следующую позицию*)
      Gr.DrawTile(manX, manY, Rsrc.None); (*Стереть предыдущий спрайт*)
      INC(manX, manDirX);
      INC(manY, manDirY) (*Изменить координаты*)
    ELSE
      manSprite := StopMan (*Остановить человечка*)
    END;
  Gr.DrawTile(manX, manY, Rsrc.GetTileByNum( manSprite )); (*Вывести новый спрайт*)
END TryMoveMan;

PROCEDURE ManContinuesGoing;
  (*Перевод человечка из состояния между двумя клетками в одну клетку*)
  (*(продолжение предыдущего движения)*)
VAR
  plusX, minusX, plusY, minusY: Coords;
BEGIN
  plusX := manX + manDirX; minusX := manX - manDirX;
  plusY := manY + manDirY; minusY := manY - manDirY;
  Gr.DrawTile(minusX, minusY, Rsrc.None);
  IF GetCell(minusX, minusY) = StopMan THEN (*Откат на полшага назад*)
    SetCell(minusX, minusY, None) (*Затирка человечка*)
  END;
  Gr.DrawTile(plusX, plusY, Rsrc.GetTileByNum( GetCell(plusX, plusY) + 1) ); (*Бегущая фаза (следующая)*)
  SetCell(plusX, plusY, StopMan);
  manX := plusX; manY := plusY (*Следующие координаты*)
END ManContinuesGoing;

PROCEDURE CheckMan (): BOOLEAN;
(*Признак присутствия человечка в клетке, соответствующей его координатам*)
BEGIN
  CASE GetCell(manX, manY)(*Field[manIdx]*) OF
    StopMan, UpMan, DownMan, LeftMan, RightMan,
    UpMan1, DownMan1, LeftMan1, RightMan1 :
      RETURN TRUE
  ELSE END;
  RETURN FALSE (*If the man became a victim*)
END CheckMan;
(*
PROCEDURE GetUserControl (): Ctrl.Keys;
BEGIN
  IF Ctrl.Get() = {Ctrl.Esc} THEN
    RETURN UserAbort END;
  IF Ctrl.Get() = {UserUp} THEN
    RETURN UserUp END;
  IF Ctrl.Get() = {UserDown} THEN
    RETURN UserDown END;
  IF Ctrl.Get() = {UserLeft} THEN
    RETURN UserLeft END;
  IF Ctrl.Get() = {UserRight} THEN
    RETURN UserRight END;
  RETURN UserNoKeys
END GetUserControl;
*)
(* -------------------------------------------------------------------------- *)
(* ------------ Live of monsters - flyes (babo) and mines (mina) ------------ *)
(* -------------------------------------------------------------------------- *)
PROCEDURE NextMonstrPhase (cell: Cell): Cell;
  (* Вычисление противофазы для бабочки или мины *)
BEGIN
  CASE cell OF
  | Mina  : RETURN Mina1
  | Mina1 : RETURN Mina
  | Babo  : RETURN Babo1
  | Babo1 : RETURN Babo
  ELSE END;
  RETURN None
END NextMonstrPhase;

PROCEDURE MonstrContinuesGoing (VAR monstr: Monstr);
(*Продолжение движения бабочки или мины - аналогично как для камней или
человечка. Из промежуточного положения - в целую клетку*)
VAR
  newX, newY, oldX, oldY: Coords; sprite: Cell;
BEGIN
  newX := monstr.x + monstr.dx; oldX := monstr.x - monstr.dx;
  newY := monstr.y + monstr.dy; oldY := monstr.y - monstr.dy;
  sprite := NextMonstrPhase(GetCell(oldX, oldY));
  SetCell(oldX, oldY, None); SetCell(newX, newY, sprite);
  Gr.DrawTile(oldX, oldY, Rsrc.None);
  Gr.DrawTile(newX, newY, Rsrc.GetTileByNum( sprite ));
  monstr.x := newX; monstr.y := newY
END MonstrContinuesGoing;

(* ======================== ExcludeMonstr (n: Index) ======================== *)
(*  Вход:      n - номер монстра (бабочки или мины)
    Обновляет: monstr[], monstrsNum
*)
PROCEDURE ExcludeMonstr (n: Index); (* Исключение бабочки или мины,
  убитой камнем или алмазом, из списка действующих монстров. *)
VAR
  x, y: Coords;
BEGIN
  x := monstr[n].x; y := monstr[n].y;
  IF ODD(x) OR ODD(y) THEN
    INC(x, monstr[n].dx); INC(y, monstr[n].dy);
    SetCell(x, y, None); Gr.DrawTile(x, y, Rsrc.None);
  END;
  (* Удаление из промежуточной позиции: *)
  DEC(monstrsNum); (* Уменьшение числа монстров. *)
  WHILE CHR( n ) < CHR( monstrsNum ) DO (* Сдвижка массива: *)
    monstr[n].x := monstr[n + 1].x; monstr[n].y := monstr[n + 1].y;
    monstr[n].dx := monstr[n + 1].dx; monstr[n].dy := monstr[n + 1].dy;
    INC(n);
  END;
END ExcludeMonstr;

(* ======================== TryMoveMonstr (n: Index) ======================== *)
(*  Вход:      n - номер монстра (бабочки или мины)
    Обновляет: monstr[n]
*)
PROCEDURE Sign (x: Coords): Coords;
  (* Знак аргумента.
     Результат: -1 для отрицательного, 1 для положительного и нуля. *)
BEGIN
  IF x < 0 THEN RETURN -1 END;
  RETURN 1
END Sign;

PROCEDURE TryMoveMonstr (n: Index); (* Попытка сдвига монстра N из клетки. *)
VAR
  x, y, dx, dy, tx, ty, max, mark: Coords; cell: Cell; i, moves: NatInt;
  canMove: ARRAY 4 OF NatInt; (* Направления движения монстра. *)
BEGIN
  x := monstr[n].x; y := monstr[n].y; cell := NextMonstrPhase(GetCell(x, y));

  (* Просмотр по 4-м сторонам света c севера против часовой стрелки: *)
  moves := -1;
  i := 3; REPEAT (* i := 3 TO 0 BY -1 {север, запад, юг, восток} *)
    (* Проследить, куда может двинуться монстр с этого места: *)
    tx := x + 2*Rsrc.CrossX[i]; ty := y + 2*Rsrc.CrossY[i];
    IF ( CHR( tx ) <= CHR( (FieldWidth-1) * 2) ) &
       ( CHR( ty ) <= CHR( (FieldHeight-1) * 2) )
       (* tx,ty неявно проверяются здесь и на >= 0 (приведены к беззнаковым) *)
    THEN
      CASE GetCell(tx, ty) OF None, StopMan: (* Двинуться можно. *)
        INC(moves); canMove[moves] := i; (* Вариант включен в список. *)
      ELSE END;
    END;
  DEC(i) UNTIL i < 0;
  (* Сейчас moves = количество возможных ходов монстра - 1 (оптимизация) *)

  (* ================ Проверяется всё множество вариантов: ================= *)
  IF moves < 0 THEN (* Монстр зажат со всех сторон - нет выбора. *)
    SetCell(x, y, cell); Gr.DrawTile(x, y, Rsrc.GetTileByNum(cell));
    (* monstr[n].dx и monstr[n].dy в 0 не устанавливаем - смещения неважны,
       ибо продолжение движения возможно только для непарных x и y. *)
  ELSE
    IF moves = 0 THEN (* Монстр отражается от тупика коридора. *)
      dx := Rsrc.CrossX[ canMove[0] ]; dy := Rsrc.CrossY[ canMove[0] ];
    ELSE (* Много вариантов: *)

      (* Поискать худший (обратное движение): *)
      dx := -monstr[n].dx; dy := -monstr[n].dy;
      i := 0; REPEAT (* i := 0 TO moves *)
        IF (Rsrc.CrossX[canMove[i]] = dx) & (Rsrc.CrossY[canMove[i]] = dy) THEN
          WHILE CHR( i ) < CHR( moves ) DO (* Уменьшить кол-во вариантов: *)
            canMove[i] := canMove[i + 1]; INC(i); (* Сжать массив. *)
          END;
          DEC(moves); (* Вычеркнуть из списка. *)
          (* Обратное движение найдено и вычеркнуто, выход из поиска. *)
        END;
      INC(i) UNTIL CHR( i ) > CHR( moves );

      IF moves = 0 THEN (* Если остался только один вариант, *)
        (* это обеспечит продвижение объекта по коридору: *)
        dx := Rsrc.CrossX[ canMove[0] ]; dy := Rsrc.CrossY[ canMove[0] ];
      ELSE (* Только здесь оцениваем направление к человечку: *)
        tx := Sign(manX - x); ty := Sign(manY - y); max := 0;
        i := 0; REPEAT (* i := 0 TO moves *)
        (* Можно бы пустить цикл от moves до 0, но нарушится порядок поиска. *)
          mark := ABS(Rsrc.CrossX[ canMove[i] ] + tx) +
                  ABS(Rsrc.CrossY[ canMove[i] ] + ty);
          IF CHR( mark ) > CHR( max ) THEN
            max := mark;
            dx := Rsrc.CrossX[ canMove[i] ]; dy := Rsrc.CrossY[ canMove[i] ]
          END;
        INC(i) UNTIL CHR( i ) > CHR( moves );
      END;
    END;

    (* Направление движения монстра определено [dx, dy]. *)
    Gr.DrawTile(x, y, Rsrc.None); INC(x, dx); INC(y, dy);
    Gr.DrawTile(x, y, Rsrc.GetTileByNum(cell));
    SetCell(x + dx, y + dy, cell);
    monstr[n].x := x; monstr[n].y := y;
    monstr[n].dx := dx; monstr[n].dy := dy;
  END;
END TryMoveMonstr;

(* ========================================================================== *)

PROCEDURE Play* (room: Level);
(* Main game - select labirinth, play game, and returns score *)
VAR
  x, y, side: Coords; number: NatInt; keys: Ctrl.Keys;
BEGIN
(*------------Н-А-Ч-А-Л-Ь-Н-Ы-Е---Д-Е-Й-С-Т-В-И-Я-------------*)
  (*select_lab ();*)
  (*lives := 10; score := 0;*)
  Scr.Cls;
  (*REPEAT*) (*---В---П-О-С-Л-Е-Д-О-В-А-Т-Е-Л-Ь-Н-Ы-Х---Л-А-Б-И-Р-И-Н-Т-А-Х---*)
    Unpack(room); Display; LookForObjects;
    (*Grax.SetCursorPos(0,0); WriteInt (monstrsNum);*)
    IF (~manPresent) OR (monstrsNum = 0) THEN RETURN (*0*) END;
      (* Если не с кем играть с самого начала - это нечестно *)
    side := SideRight;
    LOOP (*---В---О-Д-Н-О-М---Л-А-Б-И-Р-И-Н-Т-Е---*)
      Timer.Start(SHORT(ENTIER(2*Timer.Mul/Timer.Div + 0.5))); (* 1/9 second per phase *)
(*#ifdef MAN_*)
      (*---Д-Е-Й-С-Т-В-И-Я---Ч-Е-Л-О-В-Е-Ч-К-А---*)
      IF ODD(manX) OR ODD(manY) THEN (*Если нечетные координаты*)
        ManContinuesGoing (*Продолжать начатый ход*)
      ELSE (*Управлять человечком*)
        keys := Ctrl.GetCustom(); (*Проверить и зафиксировать управление*)
        IF keys = {UserUp} THEN manDirX := 0; manDirY :=-1; TryMoveMan
        ELSIF keys = {UserDown} THEN manDirX := 0; manDirY := 1; TryMoveMan
        ELSIF keys = {UserLeft} THEN manDirX :=-1; manDirY := 0; TryMoveMan
        ELSIF keys = {UserRight} THEN manDirX := 1; manDirY := 0; TryMoveMan
        ELSIF keys = {UserAbort} THEN manPresent := FALSE; EXIT (*exit loop*) (*?*)
        ELSE
          manDirX := 0; manDirY := 0
        END;
        IF ~manPresent THEN EXIT END
      END; (*ход из целой позиции*)
(*#endif /*MAN_*/*)

(*#ifdef STONES*)

(*------------Д-В-И-Ж-Е-Н-И-Е---К-А-М-Н-Е-Й---И---А-Л-М-А-З-О-В------------*)
   (*almas*)number := 0;
   objIdx := FieldSize - FieldWidth;
   objY := FieldHeight * 2;
   WHILE objY > 0 DO (*Просмотр снизу вверх по строкам*)
     DEC(objY, 2);
     objX := 0;
     WHILE objX < FieldWidth * 2 DO (*Просмотр строки слева направо*)
       objCell := field[objIdx];
       CASE objCell OF (*Отдельно - подсчет алмазов*)
         Almas, ActiveAlmas1, ActiveAlmas2 :
           INC((*almas*)number)
       ELSE END;
       CASE objCell OF
         Almas, Stone : (*Неактивный камень или алмаз*)
           IF CanStartFall() THEN
             Activate (*Начать падение*)
           ELSE
             side := -side;
             IF CanRollFrom(side) THEN (*Попытаться скатиться*)
               RollFrom(side)
             ELSIF CanRollFrom(-side) THEN (*В другую сторону*)
               RollFrom(-side)
             END
           END
           (*continue;*) (*Иначе остаться лежать*)
       | ActiveAlmas2, ActiveStone2 :
           (*Камень или алмаз, пролетающий через клетку*)
           IF CanFlyThru() THEN
             NextFall (*либо летит дальше*)
             (*continue;*)
           ELSE
             Deactivate (*либо падает на опору*)
           END
           (*Далее он сразу обрабатывается как неактивный*)
       | ActiveStone1 : (* Stone, пролетающий между клетками*)
           Gr.DrawTile(objX, objY - 2, Rsrc.None);
           Gr.DrawTile(objX, objY, Rsrc.Stone);
           (*SetCell(objX, objY, ActiveStone2);*)
           field[objIdx] := ActiveStone2;
           (*SetCell(objX, objY - 2, None)*)
           field[objIdx - FieldWidth] := None
       | ActiveAlmas1 : (* Almas, пролетающий между клетками*)
           Gr.DrawTile(objX, objY - 2, Rsrc.None);
           Gr.DrawTile(objX, objY, Rsrc.Almas);
           (*SetCell(objX, objY, ActiveAlmas2)*)
           field[objIdx] := ActiveAlmas2;
           (*SetCell(objX, objY - 2, None);*)
           field[objIdx - FieldWidth] := None
           (*continue;*) (*Он пролетел в следующую клетку*)
       ELSE END;
       INC(objX, 2); INC(objIdx)
     END;
     DEC(objIdx, 2 * FieldWidth)
   END;
   manPresent := CheckMan();
   IF (~manPresent) THEN EXIT END; (*Check for game end*)
   IF ((*almas*)number = 0) THEN
     Timer.Until;
     ManContinuesGoing;
     EXIT
   END;
(*#endif  /*STONES*/*)
(*---------------Д-В-И-Ж-Е-Н-И-Е---Б-А-Б-О-Ч-Е-К---И---М-И-Н---------------*)
      number := 0;
      WHILE number < monstrsNum DO (* for (ctr = 0; ctr < num_objects;) { *)
        (*optr = object_array + ctr;*)
        x := monstr[number].x; y := monstr[number].y;
        IF ODD(x) THEN DEC(x, monstr[number].dx) END; (*Отход назад*)
        IF ODD(y) THEN DEC(y, monstr[number].dy) END;
        CASE GetCell(x, y) OF
          Mina, Babo, Mina1, Babo1 :
            IF ODD(monstr[number].x) OR ODD(monstr[number].y) THEN
              MonstrContinuesGoing(monstr[number])
            ELSE
              TryMoveMonstr(number)
            END;
            INC(number)
        ELSE (*Уничтожить объект - он был разбит*)
          ExcludeMonstr(number) (*Число объектов уменьшается*)
        END
      END;
      manPresent := CheckMan();
      IF ~manPresent THEN SceneOfDeath(manX, manY) END;
(*#endif  /*OBJECTS*/*)

      Scr.Redraw;
      Timer.Until; (*End of phase*)

      (*} while (almasNum != 0 && man_present); /*Цикл игры в 1 лабиринте*)
      IF ~manPresent (*OR Grax.terminate*) THEN EXIT END (* *)
    END;
  (*UNTIL FALSE*)
  DisplayInf
END Play;

END Labirint.
