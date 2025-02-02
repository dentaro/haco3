moji = 999;


-------------------------------------------------------
firstF = true
function setup()
  cls(0)
    -- wini(32)
    local tableData = {
      "off", "r", "%", "ce", "c", 
      "7", "8", "9", "/", "mrc", 
      "4", "5", "6", "*", "m-", 
      "1", "2", "3", "-", "m+", 
      "0", ".", "+/-", "+", "="}
  
    for j = 0,4 do
      for i = 0,4 do
        -- addbtn(9+j*5+i,24*i,20+20*j,3)
        color(7)
        text(tableData[j*5+i+1],24*i+8,20+20*j+8)
      end
    end
end

function _init()--1回だけ
    setup()
end

function input()

  if btnp(1) then --1
    moji = 999
  end
  if btnp(2) then --1
    moji = 1
  end

  if btnp(3) then --1
    moji = 1
  end

  if btnp(4) then --1
    fillrect(0,0,128,8,3)
  end

  -- if btn(6) >= 2 then--3
  --   prey = prey - 1
  -- elseif btn(7) >= 2 then--4
  --   prey = prey + 1
  -- end
end

function _update()
  input()
end

function _draw()
  if moji ~= 999 then
    color(7)
    text(moji,0,0)
  end
end
