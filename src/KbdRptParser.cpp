#include "KbdRptParser.h"

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;

  if (mod.bmLeftShift  == 1)shiftF = true;
  else if (mod.bmLeftShift  == 0)shiftF = false;

  Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
  Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
  Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

  Serial.print(" >");
  Serial.print(key);
  gkeycode = key;//ここでキーを確定している
  Serial.print(":");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("< ");
  
  Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
  Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
  Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
};

bool KbdRptParser::getShiftF(){
  return shiftF;
}

uint8_t KbdRptParser::getKeycode()
{
  
    return gkeycode;
}

uint8_t KbdRptParser::getasciicode()
{
  
    return gasciicode;
}

void KbdRptParser::setKeycode(uint8_t _keycode){
  gkeycode = _keycode;
  
}

  // #define BACKSPACE 76
  // #define ARROW_LEFT 80
  // #define ARROW_RIGHT 79
  // #define ARROW_UP 82
  // #define ARROW_DOWN 81
  // #define DEL_KEY 76
  // #define HOME_KEY 74
  // #define END_KEY 77
  // #define PAGE_UP 75
  // #define PAGE_DOWN 78
  // #define ENTER_KEY 19//13,//通常は13
  // #define ESCAPE_KEY 115

uint8_t KbdRptParser::keyremap(uint8_t mod, uint8_t _keycode){

  int asciicode;
  Serial.print("_keycode");
  Serial.println(_keycode);
  switch(_keycode){

  //制御コードをそのまま出力コードに
  case 19:return 19;break;
  // case 27:return 27;break;
  case 76:return 76;break;
  case 74:return 74;break;
  case 77:return 77;break;
  case 75:return 75;break;
  case 78:return 78;break;
  case 79:return 79;break;
  case 80:return 80;break;
  case 81:return 81;break;
  case 82:return 82;break;
  case 58:return 58;break;
  case 65:return 65;break;
  case 135:return 135;break;//_


  //制御コードとアスキーがかぶる場合の出力コードを振り分ける
  case 41: return 27;break;//ESC
  case 42: return 76;break;//L
  
  case 137:return 92;break;//￥
  case 44: return 32;break;//空白
  // case 43:return 9;break;//水平tab

  // いったんアスキーコードに変換
  default:
    asciicode = OemToAscii(mod, _keycode);
    break;
  }

  //アスキーコード変換後の出力コード(シフトなどで変化させられる)


  if(asciicode == 95){return 94;}//^
  else if(asciicode == 123){return 96;}//'
  else if(asciicode == 135){return 95;}//_

  else if(asciicode == 42){return 40;}//(
  else if(asciicode == 40){return 41;}//)

  else if(asciicode == 125){return 123;}//{
  else if(asciicode == 126){return 125;}//}

  else if(asciicode == 34){return 42;}//*
  else if(asciicode == 58){return 43;}//+

  else if(asciicode == 39){return 58;}//:

  else if(asciicode == 43){return 126;}//~
  else if(asciicode == 93){return 91;}//[
  else if(asciicode == 32){return 93;}//]

  else if(asciicode == 91){return 64;}//@
  else if(asciicode == 64){return 22;}//"
  else if(asciicode == 94){return 26;}//&
  else if(asciicode == 38){return 27;}//'
  

  else return asciicode;
}

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t _keycode)
{
  // if(key == DEL_KEY||key == BACKSPACE)
  
  // targetTime = 0;//waitを入れる
  keyDownMillis = millis();
  Serial.print("DN ");

  gkeycode = keyremap(mod, _keycode);
  

  PrintKey(mod, keyremap(mod, _keycode));
  
  uint8_t asciiedc = keyremap(mod, _keycode);
  if (asciiedc){
    OnKeyPressed(asciiedc);
    }
}

int KbdRptParser::getKeyDownMillis(){
  return keyDownMillis; 
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
    Serial.println("LeftCtrl changed");
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    Serial.println("LeftShift changed");
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
    Serial.println("LeftAlt changed");
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
    Serial.println("LeftGUI changed");
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
    Serial.println("RightCtrl changed");
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
    Serial.println("RightShift changed");
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
    Serial.println("RightAlt changed");
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
    Serial.println("RightGUI changed");
  }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t _keycode)
{
  Serial.print("UP ");
  // PrintKey(mod, keyremap(mod, _keycode));
  // gkeycode = keyremap(mod, _keycode);
   PrintKey(mod, _keycode);
  gkeycode = _keycode;
  // gkeycode = NULL;
  //初期化
  keyDownMillis = -1;
}

void KbdRptParser::OnKeyPressed(uint8_t _remapkeycode)
{
  Serial.print("ASCII: ");
  Serial.print((char)_remapkeycode);
  Serial.print("[");
  Serial.print(_remapkeycode);
  Serial.println("]");
  gkeycode = _remapkeycode;
  gasciicode = _remapkeycode;
}