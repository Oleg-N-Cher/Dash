MODULE Labirint; (* portable *)
IMPORT
  Scr := GrApp, Gr := GrTiles, Rsrc,
  Ctrl := Control, Timer, Sound;

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
  Coords  = (* UNSIGNED *) SHORTINT; (* Coordinates in labirinth *)
  Cell    = (* UNSIGNED *) SHORTINT; (* An object of labirinth   *)
  Level   = SHORTINT;   (* A number of level (room) *)
  Index   = INTEGER;
  Monster = RECORD
    coordX, coordY, dirX, dirY: Coords;
    idx: Index
  END;

VAR
  field: ARRAY FieldSize OF Cell;
  Monsters: ARRAY MaxMonsters OF Monster;
  monstrsNum: INTEGER;
  objX, objY: Coords; objIdx: Index; objCell: Cell;
  manX, manY, manDirX, manDirY: Coords; manIdx: Index;
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
  bitsCount, n, val: (* UNSIGNED *) SHORTINT;
BEGIN
  level := Rsrc.OpenAt(room * Rsrc.LevelSize, Rsrc.Levels);
  idx := 0; bitsCount := 0;
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

PROCEDURE TwoStep (dy: INTEGER);
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
          Monsters[monstrsNum].coordX := x;
          Monsters[monstrsNum].coordY := y;
          Monsters[monstrsNum].idx := idx;
          Monsters[monstrsNum].dirX := 0;
          Monsters[monstrsNum].dirY := 0;
          INC(monstrsNum)
      | StopMan :
          manPresent := TRUE;
          manX := x; manY := y; manIdx := idx
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
  x: Coords; idx: Index;
BEGIN
  x := objX + side; idx := objIdx + side DIV 2;
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
  Gr.DrawTile(plusX, plusY, Rsrc.GetTileByNum( GetCell(plusX, plusY) ) + 1); (*Бегущая фаза (следующая)*)
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
(* -------------------------------------------------------------------------------- *)
(* --------------- Live of monsters - flyes (babo) and mines (mina) --------------- *)
(* -------------------------------------------------------------------------------- *)
PROCEDURE GetMonsterSprite (sprite: Cell): Cell;
  (*Вычисление противофазы для бабочки или мины*)
BEGIN
  CASE sprite OF
    Mina  : RETURN Mina1
  | Mina1 : RETURN Mina
  | Babo  : RETURN Babo1
  | Babo1 : RETURN Babo
  ELSE END;
  RETURN None
END GetMonsterSprite;

PROCEDURE MonsterContinuesGoing (VAR monstr: Monster);
(*Продолжение движения бабочки или мины - аналогично как для камней или
человечка. Из промежуточного положения - в целую клетку*)
VAR
  newX, newY, oldX, oldY: Coords;
  sprite: SHORTINT; (*Grax.SPRITE*)
BEGIN
  newX := monstr.coordX + monstr.dirX; oldX := monstr.coordX - monstr.dirX;
  newY := monstr.coordY + monstr.dirY; oldY := monstr.coordY - monstr.dirY;
  sprite := GetMonsterSprite(GetCell(oldX, oldY));
  SetCell(oldX, oldY, None); SetCell(newX, newY, sprite);
  Gr.DrawTile(oldX, oldY, Rsrc.None);
  Gr.DrawTile(newX, newY, Rsrc.GetTileByNum( sprite ));
  monstr.coordX := newX; monstr.coordY := newY
END MonsterContinuesGoing;

PROCEDURE ExcludeMonster (i: INTEGER);
(*Исключение бабочки или мины, убитой камнем или алмазом, из списка
действующих объектов*)
VAR
  tX, tY: Coords;
BEGIN
  (*union object * a = object_array + n,*)
  (* * hb = object_array + num_objects; *)
  tX := Monsters[i].coordX; tY := Monsters[i].coordY;
  IF ODD(tX) OR ODD(tY) THEN
    INC(tX, Monsters[i].dirX); INC(tY, Monsters[i].dirY);
    SetCell(tX, tY, None); Gr.DrawTile(tX, tY, Rsrc.None)
  END;
  (*Удаление из промежуточной позиции*)
  DEC(monstrsNum); (*уменьшение числа объектов*)
  WHILE i < (*=*) monstrsNum DO
    Monsters[i].coordX := Monsters[i+1].coordX;
    Monsters[i].coordY := Monsters[i+1].coordY;
    Monsters[i].idx  := Monsters[i+1].idx;
    Monsters[i].dirX := Monsters[i+1].dirX;
    Monsters[i].dirY := Monsters[i+1].dirY;
    INC(i) (*сдвижка массива*)
  END
END ExcludeMonster;

PROCEDURE TryMoveMonster (VAR monstr: Monster);
  (*Попытка сдвига объекта из клетки*)
VAR
  solutionDirX, solutionDirY, negDirX, negDirY,
  delta, ddX, ddY, max, mark, x, y, tX, tY, vh: Coords;
  sprite: SHORTINT; (*Grax.SPRITE*)
  DirX, DirY: ARRAY 4 OF Coords;
  movesNum, i: INTEGER;
PROCEDURE Sign (x: Coords): Coords;
  (*Знак аргумента. Результат -1 для отрицательного, 1 для положительного
  и для нуля*)
  BEGIN
    IF x >= 0 THEN RETURN 1 END;
    RETURN -1
  END Sign;
BEGIN
  movesNum := 0;            solutionDirX := 0; solutionDirY := 0;
  x := monstr.coordX; y := monstr.coordY;
  (*signed delta, vh;*)
  (*union direction * p = cross, * solution;*)
  sprite := GetMonsterSprite(GetCell(x, y));
  (*Просмотр по 4 сторонам света c севера против часовой стрелки*)
  delta := -1;
  REPEAT (* delta := [-1, 1] *)
  (*for (delta = -1; delta <= 1; delta += 2) ( *Проследить, куда может*)
    vh := 0;
    REPEAT
    (*for (vh = 0; vh <= 1; vh ++) { ( *двинуться объект с этого места*)
      tX := x; tY := y;
      IF vh # 0 THEN
        INC(tX, delta + delta); (* tx += delta << 1; *)
      ELSE
        INC(tY, delta + delta); (* ty += delta << 1; *)
      END;
      IF (tX >= 0) & (tX <= (FieldWidth-1) * 2) & (tY >= 0) & (tY <= (FieldHeight-1) * 2) THEN
        CASE GetCell(tX, tY) OF
          StopMan, None : (*Двинуться можно*)
            IF vh # 0 THEN
              DirX[movesNum] := delta; DirY[movesNum] := 0
            ELSE
              DirX[movesNum] := 0; DirY[movesNum] := delta
            END;
            INC(movesNum) (*Вариант включен в список*)
        ELSE END
      END;
    INC(vh) UNTIL vh > 1;
  INC(delta, 2) UNTIL delta > 1;
(*Проверяется все множество вариантов*)
  IF movesNum = 0 THEN (*Объект зажат со всех сторон - нет выбора*)
    SetCell(x, y, sprite); Gr.DrawTile(x, y, Rsrc.GetTileByNum( sprite ));
  ELSE
    IF movesNum = 1 THEN
      solutionDirX := DirX[0]; (*Объект отражается от тупика коридора*)
      solutionDirY := DirY[0]
    ELSE (*Много вариантов*)
      (*union direction * i, neg;*)
      negDirX := -monstr.dirX; negDirY := -monstr.dirY;
      i := 0;
      LOOP (*for (i = cross; i < p; i ++)  ( *Поискать худший*)
        IF (DirX[i] = negDirX) & (DirY[i] = negDirY) THEN (*(обратное движение)*)
          DEC(movesNum); (*Уменьшить количество вариантов*)
          WHILE i < movesNum DO
            DirX[i] := DirX[i+1];
            DirY[i] := DirY[i+1];
            INC(i) (*Вычеркнуть - сжать массив*)
          END;
          EXIT
        END;
        INC(i);
        IF i >= movesNum THEN EXIT END
      END;
      IF movesNum = 1 THEN (*Если остался только один вариант, это обеспечит*)
        solutionDirX := DirX[0]; (*продвижение объекта по коридору*)
        solutionDirY := DirY[0]
      ELSE (*Только здесь оцениваем направление к человечку*)
        ddX := Sign(manX - x); ddY := Sign(manY - y);
        max := 0;
        i := 0;
        REPEAT (* for (i = cross; i < p; i ++) *)
          mark := ABS(DirX[i] + ddX) + ABS(DirY[i] + ddY);
          IF mark > max THEN (*!*)
            max := mark;
            solutionDirX := DirX[i];
            solutionDirY := DirY[i]
          END;
          INC(i)
        UNTIL i >= movesNum
      END
    END;
    monstr.dirX := solutionDirX; INC(monstr.coordX, solutionDirX);
    monstr.dirY := solutionDirY; INC(monstr.coordY, solutionDirY);
    Gr.DrawTile(x, y, Rsrc.None);
    Gr.DrawTile(monstr.coordX, monstr.coordY, Rsrc.GetTileByNum( sprite ));
    SetCell(monstr.coordX + monstr.dirX, monstr.coordY + monstr.dirY, sprite)
  END
END TryMoveMonster;

(* -------------------------------------------------------------------------------- *)
(* ---------------------------- Main cycle of the game ---------------------------- *)
(* -------------------------------------------------------------------------------- *)
PROCEDURE Play* (room: Level);
(* Main game - select labirinth, play game, and returns score *)
VAR
  x, y, side: Coords;
  number: INTEGER; keys: Ctrl.Keys;
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
      (* Timer.Start(2); *)
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
     (* Timer.Until; *)
     ManContinuesGoing;
     EXIT
   END;
(*#endif  /*STONES*/*)
(*---------------Д-В-И-Ж-Е-Н-И-Е---Б-А-Б-О-Ч-Е-К---И---М-И-Н---------------*)
      (*monstr*)number := 0;
      WHILE (*monstr*)number < monstrsNum DO (* for (ctr = 0; ctr < num_objects;) { *)
        (*optr = object_array + ctr;*)
        x := Monsters[(*monstr*)number].coordX; y := Monsters[(*monstr*)number].coordY;
        IF ODD(x) THEN DEC(x, Monsters[(*monstr*)number].dirX) END; (*Отход назад*)
        IF ODD(y) THEN DEC(y, Monsters[(*monstr*)number].dirY) END;
        CASE GetCell(x, y) OF
          Mina, Babo, Mina1, Babo1 :
            IF ODD(Monsters[(*monstr*)number].coordX) OR ODD(Monsters[(*monstr*)number].coordY) THEN
              MonsterContinuesGoing(Monsters[(*monstr*)number])
            ELSE
              TryMoveMonster(Monsters[(*monstr*)number])
            END;
            INC((*monstr*)number)
        ELSE (*Уничтожить объект - он был разбит*)
          ExcludeMonster((*monstr*)number) (*Число объектов уменьшается*)
        END
      END;
      manPresent := CheckMan();
      IF ~manPresent THEN SceneOfDeath(manX, manY) END;
(*#endif  /*OBJECTS*/*)

      (* Timer.Until; *) (* (2); - 1/9 second per phase*) (*End of phase*)

      (*} while (almasNum != 0 && man_present); /*Цикл игры в 1 лабиринте*)
      IF ~manPresent (*OR Grax.terminate*) THEN EXIT END (* *)
    END;
  (*UNTIL FALSE*)
  DisplayInf
END Play;

END Labirint.
