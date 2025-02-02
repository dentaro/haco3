camera = creobj(0)
light = creobj(1)
obj1 = creobj(2)
colangle = 0
--正２０面体生成
function createIcosahedron()
  local phi = (1 + math.sqrt(5)) / 2

  -- local vertices = {
  --   {-1, phi, 0}, {1, phi, 0}, {-1, -phi, 0}, {1, -phi, 0},
  --   {0, -1, phi}, {0, 1, phi}, {0, -1, -phi}, {0, 1, -phi},
  --   {phi, 0, -1}, {phi, 0, 1}, {-phi, 0, -1}, {-phi, 0, 1}
  -- }

  -- local faces = {
  --   {1, 12, 6}, {1, 6, 2}, {1, 2, 8}, {1, 8, 11}, {1, 11, 12},
  --   {2, 6, 10}, {6, 12, 5}, {12, 11, 3}, {11, 8, 7}, {8, 2, 9},
  --   {4, 10, 5}, {4, 5, 3}, {4, 3, 7}, {4, 7, 9}, {4, 9, 10},
  --   {5, 10, 6}, {3, 5, 12}, {7, 3, 11}, {9, 7, 8}, {10, 9, 2}
  -- }

  -- local poly_vertices = {}

  -- for _, face in ipairs(faces) do
  --   for _, index in ipairs(face) do
  --     table.insert(poly_vertices, vertices[index])
  --   end
  -- end
  -- return poly_vertices
end

-- local poly_vertices = createIcosahedron()

function setup()
  -- fillrect(20,20,80,80,7)
end

function _init()--1回だけ
    setup()
end

function _update()
  -- if camera.angle > 360 then 
  --   camera.angle = 0
  -- end
  -- -- カメラの移動と回転
  -- if btnp(1) then
  --   camera.x = camera.x + 1  -- 左に移動
  -- elseif btnp(2) then
  --   camera.x = camera.x - 1  -- 右に移動
  -- end

  -- if btnp(3) then
  --   camera.y = camera.y + 1  -- 上に移動
  -- elseif btnp(4) then
  --   camera.y = camera.y - 1  -- 下に移動
  -- end
  -- if btnp(6) then
  --   camera.zoom = camera.zoom - 0.05  -- 左に回転
  -- elseif btnp(8) then
  --   camera.zoom = camera.zoom + 0.05  -- 右に回転
  -- end
  -- colangle = colangle + 1
  -- if colangle > 360 then 
  --   colangle  = colangle - 360
  -- end
  -- obj1.angle = obj1.angle + 1
  -- if obj1.angle > 360 then 
  --   obj1.angle  = obj1.angle - 360
  -- end
end 

function _draw()--ループします
  cls(1)
  -- trans(obj1.x,obj1.y,obj1.z,obj1.angle,obj1.size,2,colangle)
  -- cam(camera.x,camera.y,camera.z,camera.angle,camera.zoom)
  -- rendr(poly_vertices)
end


