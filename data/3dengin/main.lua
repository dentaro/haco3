
ypos = 1

px=12
py=12
angle = 0
lefttop=0
righttop=0
leftbottom=0
rightbottom=0

sn=1 -- スプライト番号
wallheight = 32

closestDistance = 999
centerangle = 32

p = { x = 10, 
      y = 10,
      angle = 0,      -- 視点の角度（度数法）
      height = 25 
    }

firstF = true

function input()
  -- p.height = btn(3,4,1, p.height)
  -- p.angle  = btn(7,8,3, p.angle )

  if btnp(3) then
    p.height = p.height + 1  -- 上に移動
  elseif btnp(4) then
    p.height = p.height - 1  -- 下に移動
  end

  if btnp(1) then
    p.angle = p.angle - 3  -- 左に回転
  elseif btnp(2) then
    p.angle = p.angle + 3    -- 右に回転
  end

  -- p.x = tp(0)
  -- p.y = tp(1)

  if p.angle > 360 then 
    p.angle = 1
  end

  if p.angle == 0 then 
    p.angle  = 360
  end
 end

function fv(v)
 return 0.25/v
end


bgcNo = 1
size = 15
x=64
y=60
function setup()
  fillrect(20,20,80,80,7)
end

function _init()--1回だけ
    setup()
end

function _update()--ループします
  input()
end

function _draw()--ループします
  cls(bgcNo)
  -- map(0,0, 0,56, 16,8,"/init/map/0.png")
  ypos=ypos+1
  spmap(0,60, 128,60, 0,0, 16,8, p.x, p.y, p.angle, p.height, "/init/map/0.png")
  -- spmap(0,60, 128,60, 0,0, 16,8, 10,10, 0,25, "/init/map/0.png")
    if ypos==16 then ypos = 1 end
  spr8(sn+64, p.x, p.y)
end


