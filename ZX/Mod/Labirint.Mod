MODULE Labirint; (** portable *)
IMPORT
  Scr := GrApp, Gr := GrTiles, R := Rsrc, Ctrl := Control, Timer, Sound;

CONST
  MaxMonsters = 128;
  FieldWidth  = R.FieldWidth;
  FieldHeight = R.FieldHeight;
  FieldSize   = R.FieldSize;

  None    =  0;
  Grass   =  1;
  Stone   =  2;
  Almas   =  3;
  StopMan =  4;
  Wall    =  5;
  Mina    =  6;
  Babo    =  7;
  LeftMan =  8; LeftMan1 = 9; RightMan = 10; RightMan1 = 11; (* Don't change *)
  UpMan   = 12; UpMan1  = 13; DownMan  = 14; DownMan1  = 15; (* the order!!! *)
  Mina1   = 16;
  Babo1   = 17;
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
  NatInt = R.NATINT; (* Native integer *)
  Coords = NatInt;      (* Coordinates in labirinth *)
  Cell   = NatInt;      (* An object of labirinth   *)
  Level  = NatInt;      (* A number of level (room) *)
  Index  = SHORTINT;    (* Must be enough for field *)
  Monstr = R.Monstr;

VAR
  monstr: ARRAY MaxMonsters OF Monstr;
  monstrsNum: NatInt;
  objX, objY: Coords; objCell: Cell;
  manX, manY, manDirX, manDirY: Coords;
  manPresent: BOOLEAN;
  lives, score: SHORTINT;(*BYTE?*)

(* -------------------------------------------------------------------------------- *)
(* Base procedures for manipulating cell array, unpack levels and display labirinth *)
(* -------------------------------------------------------------------------------- *)

PROCEDURE Unpack (room: Level);
VAR
  level: R.Resource; idx: Index; cell: Cell;
  bitsCount, n: (* UNSIGNED *) BYTE; val: NatInt;
BEGIN
  level := R.OpenAt(room * R.LevelSize, R.Levels);
  idx := 0; val := R.ReadByte(level); bitsCount := 8;
  REPEAT
    (* Unpacking and normalizing 3 bits from package: *)
    cell := 0;
    FOR n := 2 TO 0 BY -1 DO
      IF bitsCount = 0 THEN val := R.ReadByte(level); bitsCount := 8 END;
      DEC(bitsCount); (* INC(cell, SHORT(SHORT(ASH(val MOD 2, n)))) *)
      cell := cell DIV 2 + 4*(val MOD 2); val := val DIV 2;
    END;
    R.field[idx] := cell; INC(idx);
  UNTIL idx = FieldSize;
  R.Close(level);
END Unpack;

PROCEDURE TwoStep (dy: Coords);
BEGIN
  Timer.Start(ENTIER(1*Timer.Mul/Timer.Div + 0.5));
  dy := dy * 6;
  IF dy >= 0 THEN
    Scr.ScrollUp(dy)
  ELSE
    Scr.ScrollDown(-dy)
  END;
  Timer.Until;
  Timer.Start(ENTIER(1*Timer.Mul/Timer.Div + 0.5));
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
      Gr.DrawTile(x, (FieldHeight-1) * 2, R.GetTileByNum( R.GetCell(x, y)) );
      INC(x, 2);
    END;
    Scr.Redraw;
    INC(y, 2);
    Timer.Until;
  END;
END Display;

PROCEDURE DisplayInf;
VAR
  i: SHORTINT;
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
  Timer.Start(ENTIER(18*Timer.Mul/Timer.Div + 0.5));
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
      CASE R.field[idx] OF
      | Mina, Babo :
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
  plusX, minusX, plusY, minusY, tX, tY: Coords; ctr, lim: SHORTINT;
BEGIN
  (*unsigned xa, ya,*)
  IF CHR(x) > 2X THEN minusX := x - 2 ELSE minusX := 0 END;
  IF CHR(y) > 2X THEN minusY := y - 2 ELSE minusY := 0 END;
  IF CHR(x) > CHR( (FieldWidth-2) * 2 ) THEN plusX := (FieldWidth-1) * 2;
  ELSE plusX := x + 2;
  END;
  IF CHR(y) > CHR( (FieldHeight-2) * 2 ) THEN plusY := (FieldHeight-1) * 2;
  ELSE plusY := y + 2;
  END;

  tY := minusY;
  WHILE tY <= plusY DO
    tX := minusX;
    WHILE tX <= plusX DO
      Gr.DrawTile(tX, tY, R.Almas);
      R.SetCell(tX, tY, Almas);
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
    Gr.DrawTile(objX, objY, R.Stone);
    R.SetCell(objX, objY, Stone);
  ELSE
    Gr.DrawTile(objX, objY, R.Almas);
    R.SetCell(objX, objY, Almas);
  END;
(*
  Sound.Beep(24858 DIV (objY + 100) * 4, 10) (*, snd (200 + (y << 1), 10),*)
*)
  (*деактивация и звук падения. Камень или алмаз теперь лежит*)
END Deactivate;

(* ========================= CanFlyThru (): BOOLEAN ========================= *)

PROCEDURE CanFlyThru (): BOOLEAN;
  (* Для летящих в клетке камней и алмазов - разрешение пролёта вниз дальше. *)
BEGIN
  IF objY = (FieldHeight-1) * 2 THEN
    RETURN FALSE (* Нельзя - упали на границу лабиринта. *)
  END;
  CASE R.GetCell(objX, objY + 2) OF
    Wall, Grass, Stone, Almas,
    ActiveStone1, ActiveAlmas1,
    ActiveStone2, ActiveAlmas2 :
      RETURN FALSE (* Нельзя - упали на траву, стену, камень или алмаз. *)
  ELSE END;
  RETURN TRUE (* Можно - впереди пусто или убиваемый объект. *)
END CanFlyThru;

(* ================================== NextFall ============================== *)

PROCEDURE NextFall; (* Может изменить objX *)
  (* Для летящих в клетке камней - переход в положение между двумя клетками *)
BEGIN
  CASE R.GetCell(objX, objY + 2) OF
  | None :
      Gr.DrawTile(objX, objY, R.None);
      IF objCell = ActiveStone2 THEN
        Gr.DrawTile(objX, objY + 1, R.Stone);
        R.SetCell(objX, objY, ActiveStone1);
        R.SetCell(objX, objY + 2, ActiveStone1);
      ELSE
        Gr.DrawTile(objX, objY + 1, R.Almas);
        R.SetCell(objX, objY, ActiveAlmas1);
        R.SetCell(objX, objY + 2, ActiveAlmas1);
      END;
  | Grass, Stone, Almas, Wall :
  ELSE (* Babo, Mina, Babo1, Mina1, StopMan, UpMan, UpMan1,
          DownMan, DownMan1, RightMan, RightMan1, LeftMan, LeftMan1 *)
    SceneOfDeath(objX, objY + 2);
    INC(objX, 2);
  END;
END NextFall;

(* ========================== CanStartFall (): BOOLEAN ====================== *)

PROCEDURE CanStartFall (): BOOLEAN;
  (* Разрешение падения прямо вниз из неподвижного положения. *)
BEGIN
  RETURN
    ( CHR(objY) < CHR( (FieldHeight-1) * 2) )
    & ( R.GetCell(objX, objY + 2) = None )
END CanStartFall;

(* ===================== CanRollTo (x: Coords): BOOLEAN ===================== *)

PROCEDURE CanRollTo (x: Coords): BOOLEAN;
  (* Разрешение скатывания в сторону: side = -2 (SideLeft) | 2 (SideRight). *)
BEGIN
  RETURN
    (* x, objY неявно проверяются здесь и на >= 0 (приведены к беззнаковым): *)
    ( CHR( x ) <= CHR( (FieldWidth-1) * 2) )
    & ( CHR( objY ) < CHR( (FieldHeight-1) * 2) )
    & ( R.GetCell(x, objY) = None )
    & ( R.GetCell(x, objY + 2) = None )
END CanRollTo;

(* ========================= RollTo (side: Coords) ========================== *)

PROCEDURE RollTo (side: Coords); (* Может изменить objX *)
  (* Скатывание в сторону side на 1 клетку *)
BEGIN
  R.SetCell(objX, objY, None);
  R.SetCell(objX + side, objY, objCell);
  Gr.DrawTile(objX, objY, R.None);
  Gr.DrawTile(objX + side, objY, R.GetTileByNum( objCell ));
  (* Специфично для скатывания вправо - чтобы этот камень или алмаз мог
     только скатиться и не был повторно обработан в следующей клетке -
     мы его новую клетку пропускаем: *)
  IF side = SideRight THEN INC(objX, SideRight) END;
END RollTo;

(* ================================== Activate ============================== *)

PROCEDURE Activate; (* Перевод неподвижного камня или алмаза
  в активное состояние и в положение между 2 клетками. *)
BEGIN
  Gr.DrawTile(objX, objY, R.None);
  IF objCell = Stone THEN (* Занимает 2 клетки. *)
    Gr.DrawTile(objX, objY + 1, R.Stone);
    R.SetCell(objX, objY, ActiveStone1);
    R.SetCell(objX, objY + 2, ActiveStone1);
  ELSE
    Gr.DrawTile(objX, objY + 1, R.Almas);
    R.SetCell(objX, objY, ActiveAlmas1);
    R.SetCell(objX, objY + 2, ActiveAlmas1);
  END;
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
    CASE R.GetCell(x, y) OF
      Wall, ActiveStone1, ActiveAlmas1 :
        RETURN FALSE (*Пройти нельзя*)
    | Stone, ActiveStone2 :
        (*unsigned tx, ty, z;*) (*Попробуем протолкнуть камни*)
        tX := x; tY := y;
        LOOP
          IF (tX >= 0) & (tX < FieldWidth * 2) & (tY >= 0) & (tY < FieldHeight * 2) THEN
            cell := R.GetCell(tX, tY);
            IF (cell # Stone) & (cell # ActiveStone2) THEN
              IF cell = None THEN EXIT END;
              RETURN FALSE (*Протолкнуть камни нельзя*)
            END;
            INC(tX, manDirX + manDirX); INC(tY, manDirY + manDirY) (*?*)
          ELSE
            RETURN FALSE (*Протолкнуть камни нельзя*)
          END
        END;
        R.SetCell(tX, tY, Stone); R.SetCell(x, y, None);
        Gr.DrawTile(tX, tY, R.Stone);
        Gr.DrawTile(x, y, R.None)
    ELSE END;
    RETURN TRUE (*Иначе - пройти в таком направлении можно*)
  END;
  RETURN FALSE
END ManCanGo;

PROCEDURE ManGoing (x, y: Coords): BOOLEAN;
(*Перевод человечка из текущей клетки в положение между двумя клетками*)
(*Разрешение дальнейшего существования человечка*)
BEGIN
  CASE R.GetCell(x, y) OF
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
      R.SetCell(x, y, manSprite); (*Отметить следующую позицию*)
      Gr.DrawTile(manX, manY, R.None); (*Стереть предыдущий спрайт*)
      INC(manX, manDirX);
      INC(manY, manDirY) (*Изменить координаты*)
    ELSE
      manSprite := StopMan (*Остановить человечка*)
    END;
  Gr.DrawTile(manX, manY, R.GetTileByNum( manSprite )); (*Вывести новый спрайт*)
END TryMoveMan;

PROCEDURE ManContinuesGoing;
  (*Перевод человечка из состояния между двумя клетками в одну клетку*)
  (*(продолжение предыдущего движения)*)
VAR
  plusX, minusX, plusY, minusY: Coords;
BEGIN
  plusX := manX + manDirX; minusX := manX - manDirX;
  plusY := manY + manDirY; minusY := manY - manDirY;
  Gr.DrawTile(minusX, minusY, R.None);
  IF R.GetCell(minusX, minusY) = StopMan THEN (*Откат на полшага назад*)
    R.SetCell(minusX, minusY, None) (*Затирка человечка*)
  END;
  Gr.DrawTile(plusX, plusY, R.GetTileByNum( R.GetCell(plusX, plusY) + 1) ); (*Бегущая фаза (следующая)*)
  R.SetCell(plusX, plusY, StopMan);
  manX := plusX; manY := plusY (*Следующие координаты*)
END ManContinuesGoing;

PROCEDURE CheckMan (): BOOLEAN;
(* Признак присутствия человечка в клетке, соответствующей его координатам *)
VAR
  cell: Cell;
BEGIN
  cell := R.GetCell(manX, manY);
  RETURN
    (cell = StopMan) OR
    (CHR( cell ) >= CHR( LeftMan )) & (CHR( cell ) <= CHR( DownMan1 ))
END CheckMan;

(* -------------------------------------------------------------------------- *)
(* ------------ Live of monsters - flyes (babo) and mines (mina) ------------ *)
(* -------------------------------------------------------------------------- *)
PROCEDURE NextMonstrPhase (cell: Cell): Cell;
  (* Вычисление противофазы для бабочки или мины *)
BEGIN
  CASE cell OF
  | Babo  : RETURN Babo1
  | Babo1 : RETURN Babo
  | Mina  : RETURN Mina1
  ELSE END;
  RETURN Mina
END NextMonstrPhase;

PROCEDURE MonstrContinuesGoing (VAR monstr: Monstr);
  (* Продолжение движения бабочки или мины - аналогично как для камней
     или человечка. Из промежуточного положения - в целую клетку *)
VAR
  newX, newY, oldX, oldY: Coords; sprite: Cell;
BEGIN
  newX := monstr.x + monstr.dx; oldX := monstr.x - monstr.dx;
  newY := monstr.y + monstr.dy; oldY := monstr.y - monstr.dy;
  sprite := NextMonstrPhase(R.GetCell(oldX, oldY));
  R.SetCell(oldX, oldY, None); R.SetCell(newX, newY, sprite);
  Gr.DrawTile(oldX, oldY, R.None);
  Gr.DrawTile(newX, newY, R.GetTileByNum( sprite ));
  monstr.x := newX; monstr.y := newY;
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
    R.SetCell(x, y, None); Gr.DrawTile(x, y, R.None);
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
  x := monstr[n].x; y := monstr[n].y; cell := NextMonstrPhase(R.GetCell(x, y));

  (* Просмотр по 4-м сторонам света c севера против часовой стрелки: *)
  moves := -1;
  i := 3; REPEAT (* i := 3 TO 0 BY -1 {север, запад, юг, восток} *)
    (* Проследить, куда может двинуться монстр с этого места: *)
    tx := x + 2*R.CrossX[i]; ty := y + 2*R.CrossY[i];
    IF ( CHR( tx ) <= CHR( (FieldWidth-1) * 2) ) &
       ( CHR( ty ) <= CHR( (FieldHeight-1) * 2) )
       (* tx,ty неявно проверяются здесь и на >= 0 (приведены к беззнаковым) *)
    THEN
      CASE R.GetCell(tx, ty) OF None, StopMan: (* Двинуться можно. *)
        INC(moves); canMove[moves] := i; (* Вариант включен в список. *)
      ELSE END;
    END;
  DEC(i) UNTIL i < 0;
  (* Сейчас moves = количество возможных ходов монстра - 1 (оптимизация) *)

  (* ================ Проверяется всё множество вариантов: ================= *)
  IF moves < 0 THEN (* Монстр зажат со всех сторон - нет выбора. *)
    R.SetCell(x, y, cell); Gr.DrawTile(x, y, R.GetTileByNum(cell));
    (* monstr[n].dx и monstr[n].dy в 0 не устанавливаем - смещения неважны,
       ибо продолжение движения возможно только для непарных x и y. *)
  ELSE
    IF moves = 0 THEN (* Монстр отражается от тупика коридора. *)
      dx := R.CrossX[ canMove[0] ]; dy := R.CrossY[ canMove[0] ];
    ELSE (* Много вариантов: *)

      (* Поискать худший (обратное движение): *)
      dx := -monstr[n].dx; dy := -monstr[n].dy;
      i := 0; REPEAT (* i := 0 TO moves *)
        IF (R.CrossX[canMove[i]] = dx) & (R.CrossY[canMove[i]] = dy) THEN
          WHILE CHR( i ) < CHR( moves ) DO (* Уменьшить кол-во вариантов: *)
            canMove[i] := canMove[i + 1]; INC(i); (* Сжать массив. *)
          END;
          DEC(moves); (* Вычеркнуть из списка. *)
          (* Обратное движение найдено и вычеркнуто, выход из поиска. *)
        END;
      INC(i) UNTIL CHR( i ) > CHR( moves );

      IF moves = 0 THEN (* Если остался только один вариант, *)
        (* это обеспечит продвижение объекта по коридору: *)
        dx := R.CrossX[ canMove[0] ]; dy := R.CrossY[ canMove[0] ];
      ELSE (* Только здесь оцениваем направление к человечку: *)
        tx := Sign(manX - x); ty := Sign(manY - y); max := 0;
        i := 0; REPEAT (* i := 0 TO moves *)
        (* Можно бы пустить цикл от moves до 0, но нарушится порядок поиска. *)
          mark := ABS(R.CrossX[ canMove[i] ] + tx) +
                  ABS(R.CrossY[ canMove[i] ] + ty);
          IF CHR( mark ) > CHR( max ) THEN
            max := mark;
            dx := R.CrossX[ canMove[i] ]; dy := R.CrossY[ canMove[i] ]
          END;
        INC(i) UNTIL CHR( i ) > CHR( moves );
      END;
    END;

    (* Направление движения монстра определено [dx, dy]. *)
    Gr.DrawTile(x, y, R.None); INC(x, dx); INC(y, dy);
    Gr.DrawTile(x, y, R.GetTileByNum(cell));
    R.SetCell(x + dx, y + dy, cell);
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
      Timer.Start(ENTIER(2*Timer.Mul/Timer.Div + 0.5)); (* 1/9 second per phase *)
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
   objY := FieldHeight * 2;
   WHILE objY > 0 DO (*Просмотр снизу вверх по строкам*)
     DEC(objY, 2);
     objX := 0;
     WHILE objX < FieldWidth * 2 DO (*Просмотр строки слева направо*)
       objCell := R.GetCell(objX, objY);
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
             IF CanRollTo(objX + side) THEN (*Попытаться скатиться*)
               RollTo(side);
             ELSIF CanRollTo(objX - side) THEN (*В другую сторону*)
               RollTo(-side);
             END;
           END;
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
           Gr.DrawTile(objX, objY - 2, R.None);
           Gr.DrawTile(objX, objY, R.Stone);
           R.SetCell(objX, objY, ActiveStone2);
           R.SetCell(objX, objY - 2, None);
       | ActiveAlmas1 : (* Almas, пролетающий между клетками*)
           Gr.DrawTile(objX, objY - 2, R.None);
           Gr.DrawTile(objX, objY, R.Almas);
           R.SetCell(objX, objY, ActiveAlmas2);
           R.SetCell(objX, objY - 2, None);
           (*continue;*) (*Он пролетел в следующую клетку*)
       ELSE END;
       INC(objX, 2);
     END;
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
        CASE R.GetCell(x, y) OF
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
