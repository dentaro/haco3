function _init()

end

function _draw()
  
end

function _update()
  cls(1)
  if tstat() > 0 then -- タッチされたら
    local mx = tp(0) -- マウス座標取得
    local my = tp(1) -- マウス座標取得
    local cx, cy = 64, 60 -- 画面中心座標
    line(cx, cy, mx, my, 7) -- 線を描画
  else
  end
end
-------------------------------------------------------
function setup()--init
  _init()
end
function loop()--update --draw
_update()
_draw()
end

--tstatのタッチ状態は以下の通り
--eventState
-- NO_EVENT -1
-- UNTOUCH 0
-- DRAG 2
-- TAPPED 3
-- WTAPPED 4
-- FLICK 5 //フリックの中心、始まりのイベント
-- FLICKED 6 //フリック終わりのイベント
-- TOUCH 7
-- WAIT 8
-- MOVE 9
-- RELEASE 10


