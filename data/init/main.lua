x = 16
y = 56

t = 0

flip = 0

function setup()
end

function draw()
  --if btn(4) == 1 then spr(x, y, 8, 8, 8, 0) end
end

function loop()
  t = t + 1

  walk = 0

  if btn(0) == 1 then x = x - 1  walk = 1 end
  if btn(1) == 1 then x = x + 1  walk = 1 end
  if btn(2) == 1 then y = y - 1  walk = 1 end
  if btn(3) == 1 then y = y + 1  walk = 1 end

  if
            x > 128  then x = -8
    elseif  x < -8  then  x = 128
    elseif  y > 120  then y = -8
    elseif  y < -8  then  y = 120
  end


  color(0,0,0)
  fillrect(x-1,y-1,10,10)

  if walk == 1 then

   if t%3 == 0 then
    flip = flip + 1
    flip = flip%2
   end

   if flip == 0 then spr(x, y, 8, 8, 16, 0) end
   if flip == 1 then spr(x, y, 8, 8, 24, 0) end

  else
   spr(x, y, 8, 8, 8, 0)
  end

end
