bgcNo = 1
size = 15
x=64
y=60

function setup()
  debug(1)--デバグ表示
  music(2,64,60,0,3)
  fillrect(20,20,80,80,7)
end

function _init()--1回だけ
    setup()
end

function _update()--ループします
  if tpf() == true then
    x = tp(0)
    y = tp(1)
  end
end

function _draw()--ループします
  cls(bgcNo)
  fillrect(x,y,size,size,8)
  spr8(1,x,y)
  print("hello world", x, y)
end








  