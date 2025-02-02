initState = 0
playState = 1
overState = 2
GameState = initState
size = 6
raws = 20
cols = 12
x = 5
y = 2
r = 0
loopcnt = 0
dy = 0
wall = 7
back = 8
clist = {8, 9, 10, 11, 12, 13, 14, 6, 7}
stage = {
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7},
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7},
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7},
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7}, 
    {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7}
}
ptnno = (flr(rnd(7))) % 7
if ptnno == 0 then y = 3 end
dx = 0
px = {0, 0, 0, 0}
py = {0, 0, 0, 0}
ptnxy = {
    --■□■■
    {{-1, 0}, {1, 0}, {2, 0}},--1
    --■
    --■□■
    {{-1, -1}, {-1, 0}, {1, 0}},--2
    --  ■
    --■□■
    {{-1, 0}, {1, -1}, {1, 0}},--3
    --■□
    -- ■■
    {{-1, 0}, {0, 1}, {1, 1}},--4
    -- □■
    --■■
    {{1, 0}, {0, 1}, {-1, 1}},--5
    -- ■
    --■□■
    {{-1, 0}, {0, -1}, {1, 0}},--6
    --□■
    --■■
    {{1, 0}, {0, 1}, {1, 1}}--6
}

ff = 0

function setposition()
  px[1], py[1] = x, y
  r = r % 4
  _r = r
  for i = 1, 3 do--オフセット値を取り出すため３回ループ
      _x = ptnxy[ptnno+1][i][1]
      _y = ptnxy[ptnno+1][i][2]
      for j = 1, _r do
          _w = _x
          _x = _y
          _y = -_w
      end
      px[i + 1] = x + _x
      py[i + 1] = y + _y
  end
end

function setptn()
  setposition()
  for i = 1, 4 do
    _x = px[i]
    _y = py[i]
    stage[_y][_x] = ptnno
  end
end

function delptn()
  setposition()
  for i = 1, 4 do
    _x = px[i]
    _y = py[i]
    stage[_y][_x] = back
  end
end

function collision()
  setposition()
  for i = 1, 4 do
      if stage[py[i]][px[i]] ~= back then
          return true
      end
  end
  return false
end

function complete()
  for _y = 1, raws - 1 do
      local _flag2 = 1  -- 列がすべて埋まっているかどうかを示すフラグを初期化
      for _x = 2, cols - 2 do
          if stage[_y][_x] == back then
              _flag2 = 0  -- 空白があればフラグを 0 に設定
          end
      end
      if _flag2 == 1 then  -- 列がすべて埋まっている場合
          for upy = _y - 1, 1, -1 do  -- 上の行から順に
              for _x = 2, cols - 2 do  -- 各列について
                  stage[upy + 1][_x] = stage[upy][_x]  -- 上の行の値をコピー
              end
          end
      end
  end
end


function drawStage()
  for by = 1, raws do
      for bx = 1, cols do
        no = stage[by][bx]
        -- color(clist[no])
        -- spr8(1,size * bx, size * by)
        fillrect(size * bx, size * by, size, size, clist[no])
      end
  end
  
end

function init()
  -- if btnp(9) then
  cls(0)
    for by = 1, raws do
        stage[by][1] = wall
        stage[by][cols] = wall
        for bx = 2, cols - 1 do
            if by == raws then
                stage[by][bx] = wall
            else
                stage[by][bx] = back
            end
        end
    end
    x = 5
    y = 2
    r = 0
    ptnno = (flr(rnd(7))) % 7
    if ptnno == 0 then y = 3 end
    GameState = playState
  -- end
end


function over()
  drawStage()
  color(8)
  text("game over", 16, 42)
  color(7)
  text("restart[↑]", 80, 42)
  if btnp(3) then
      GameState = initState
  end
end

function play()
  delptn()
  loopcnt = loopcnt + 1
  dy, dx, dr = 0, 0, 0
  if loopcnt%10 == 0 then
    dy = 1
  end

  if btn(2) >= 1 then
    dx = 1
  end

  if btn(1) >= 1 then
    dx = -1
    -- sfx(6)
  end

  if btnp(8) then
      dr = 1
  end

  if btn(4) >= 1 then
      ff = 1
  end

  if ff == 1 then dy = 1 end

  y = y + dy
  x = x + dx
  r = r + dr

    if collision() == true then
    y = y - dy--床があったら戻す
    x = x - dx--壁にめり込んだら戻す
    r = r - dr--回転して当たったら戻す
    ff = 0
    if dy == 1 and dx == 0 and dr == 0 then--途中でとまらないように

      setptn()
      complete()
      y = 2
      x = 5
      r = 0
      ptnno = (flr(rnd(7)))%7
      if ptnno == 0 then y = 3 end
      if collision() == true then
        GameState = overState
      end

    end
  end
  setptn()
  drawStage()
end

function setup()
  fillrect(20,20,8,8,7)
end

function _init()--1回だけ
    setup()
end

function input()

end

function _update()--ループします
  input()
end

function _draw()--ループします
  -- fillrect(tp(0),tp(1),8,8,10)

  if GameState == initState then
    init()
  elseif GameState == playState then
    play()
  elseif GameState == overState then
    over()
  end
end