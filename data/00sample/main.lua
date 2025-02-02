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
end

function _draw()--ループします
  cls(bgcNo)
  fillrect(x,y,size,size,8)
  spr8(1,x,y)
end








  