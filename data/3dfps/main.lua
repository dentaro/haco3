x=12
y=12
angle = 0
lefttop=0
righttop=0
leftbottom=0
rightbottom=0

s=1 -- スプライト番号
d=1 -- 方向を示す                                                                                  
ipf=8 -- アニメーション1フレームについての時間(1ipf = 1/30秒)
nf=2 -- アニメーションするフレーム数(足踏みは2フレーム)
t=0

wallheight = 32

closestDistance = 999
centerangle = 32
raylength = 60
divnum = 32
angleStep = 64/divnum
centerno = divnum/2

bw = 16
bx = 16
by = 16

bx2 = 32
by2 = 32

walls = {
  {bx+0, by+0, bx+bw, by+0, 240},
  {bx+bw, by+0, bx+bw, by+bw, 60}, 
  {bx+bw, by+bw, bx+0, by+bw, 120}, 
  {bx+0, by+bw, bx+0, by+0, 180},

  {bx2+bw, by2+0, bx2+bw, by2+bw, 60}, 
  {bx2+bw, by2+bw, bx2+0, by2+bw, 120}, 
  {bx2+0, by2+bw, bx2+0, by+0, 180} 
}


function input()
  --wifiserve("ap")
  -- if tpf() == true then--タッチした時だけ
  if tstat() >= 2 then
    x = tp(0)
    y = tp(1)
  end

  -- カメラの移動と回転
  if btnp(1) then
    x = x - 1  -- 左に移動
  elseif btnp(2) then
    x = x + 1  -- 右に移動
  end

  if btnp(3) then
    y = y - 1  -- 上に移動
  elseif btnp(4) then
    y = y + 1  -- 下に移動
  end

  if btnp(8) then
    angle = angle - 1  -- 左に回転
  end
  if btnp(7) then
    angle = angle + 1    -- 右に回転
  end

  if angle > 360 then 
    angle = 1
  end

  if angle == 0 then 
    angle  = 360
  end

 end

function _init()--1回だけ
  music(1,255,100,0,63)--曲番号、音量、BPM、スタート番号、エンド番号
  -- vol(128)
  debug(1)--デバッグ表示
  wini(32)
end

function _update()--ループします
  input()
end

function _draw()
  cls(1)

  -- map(0,0,0,0,8,8,"/init/map/0.png")
  -- spr8(9,48,48)
  -- spr8(s+64,x, y)

  -- print(gstat(),68,64+2,7)--ゲーム状態を表示
  -- print(get_map_sprn(tp(0), tp(1)), 68,64+14,7)
  
  angle = (angle+360)%360--プレイヤーの角度

  for _, wall in ipairs(walls) do
    local x1, y1, x2, y2, color = table.unpack(wall)
      line(x1, y1, x2, y2, color)--壁の線を描画
      wset(angle, x, y, x1, y1, x2, y2, color)--壁を1枚設置
  end

  wdraw(angle)--壁を３D描画
  drawrect(96,0, 64,64, 7)

  spr8(s+64,x, y)
  -- text(x, 10, 60)
  -- text(y, 10, 70)
end


