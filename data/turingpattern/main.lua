a={}
w=60
h=60
dx = 0
dy = 0
function turing() 
  for i=0,w do
    a[i]={}
    for j=0,h do
      a[i][j]=rnd(175)/17
    end
  end

  for n=0,100 do
    for i=0,w do
      for j=0,h do
        dx=a[(i+1)%(w+1)][j]-a[i][j]
        dy=a[i][(j+1)%(h+1)]-a[i][j]
        a[i][j]=a[i][j]+0.05*(dx+dy)
        a[i][j]=a[i][j]-0.02*a[i][j]
        a[i][j]=max(min(a[i][j], 1.0), 0.0)
        pset(i, j, flr(a[i][j]*255))
      end
    end
  end
end

function _draw()
  
end
-------------------------------------------------------
flag = true
function loop()--update --draw
  if flag then
    -- _init()
    -- turing()
    flag = false
  end
  turing()
-- _update()
_draw()
end