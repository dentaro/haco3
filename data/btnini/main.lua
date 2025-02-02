bgcNo = 2
size = 15
x=64
y=60
selectno = 0

vallist = {0, 0, 0, 0, 0, 0, 0, 0}

function setup()
end

function _init()--1回だけ
  debug(1)--デバグ表示
  music(2,64,60,0,3)
  fps(30)
  setup()
end

function input()

  if btnf() == 113 then
    btnini()
  end

  -- if btnf() == 111 then
  --   selectno = selectno + 7
  --   selectno = selectno%8
  -- end

  if btnf() == 112 then
    svolv(vallist)
  end

  if btnf() == 111 then
    vallist[selectno+1] = gvolv()
  end

  if btnp(3) then
    selectno = selectno + 7
    selectno = selectno%8
  end

  if btnp(4) then
    selectno = selectno + 9
    selectno = selectno%8
  end

end

function _update()--ループします
  input()
end

function _draw()--ループします
  cls(bgcNo)
  utilMenu = {"↑", "→", "↑→", "↓", "↓→", "←", "←↑", "←↓"}

  color(7)
  for k, v in pairs(utilMenu) do
    text(         v, 20, 0 + 10 * (k - 1))
    text(vallist[k], 60, 0 + 10 * (k - 1))
  end


  -- text(gvolv(), 80, 10)
  

  oval(10, 5+selectno*10, 5, 5, 7)

  -- setbtnv(selectno)

end








  