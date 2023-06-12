--empty project
-- camera = creobj(0)
-- light = creobj(1)
-- obj1 = creobj(2)

function _update()

  if btn(1) >= 2 then
    -- camera.x = camera.x + 1  -- 左に移動
  elseif btn(2) >= 2 then
    -- camera.x = camera.x - 1  -- 右に移動
  end
end 

function _draw()
  cls()
  -- trans(obj1.x,obj1.y,obj1.z,obj1.angle,obj1.size,2,colangle)
  -- cam(camera.x,camera.y,camera.z,camera.angle,camera.zoom)
  -- rendr(poly_vertices)
end
-------------------------------------------------------
function setup()--init
  _init()
end
function loop()--update --draw
_update()
_draw()
end