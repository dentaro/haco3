#include "LovyanGFX_DentaroUI.hpp"
using namespace std;

#define FLICK_DIST 4
#define HOLDING_SEC 160000
#define TAP_TO_TAP_SEC 220000

#define CALIBRATION_FILE "/init/param/caldata.txt"

#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true

// // 閾値の数
// const int numThresholds = 5;
// // 方向を示すビット列
// byte directionBits = 0;

// // 閾値のリスト
// float statebtn_vals[6] = {
//     1150, 1050, 560, 180, 20, 0};


void LovyanGFX_DentaroUI::updatePhVols()
{
//AD1の場合
  phbtnState[0] = adc1_get_raw(ADC1_CHANNEL_7);//34pin　ジョイスティックX
  phbtnState[1] = adc1_get_raw(ADC1_CHANNEL_6);//35pin　ジョイスティックY
  phbtnState[2] = adc1_get_raw(ADC1_CHANNEL_3);//39pin　4つのボタン
  //phbtnState[3] = adc1_get_raw(ADC1_CHANNEL_5);//33pin　ボリューム
  
  //取得したいチャンネルを指定したらOK！

  // Serial.print(" x[0]:");
  // Serial.print(phbtnState[0]);
  // Serial.print(" y[1]:");
  // Serial.print(phbtnState[1]);
  // Serial.print("  4btn[2]:");
  // Serial.print(phbtnState[2]);
  
  // Serial.println(" : ");
}


int LovyanGFX_DentaroUI::getPhVol(int n)
{
  return phbtnState[n];
}

LovyanGFX_DentaroUI::LovyanGFX_DentaroUI(LGFX *_lcd)
{
  lcd = _lcd;
}

// int LovyanGFX_DentaroUI::getPhVol(int n)
// {
//   return phbtnState[n];
// }

// int LovyanGFX_DentaroUI::getPhVolDir(int n)
// {
//   // 音声ボリュームの値
//   float volume = getPhVol(n);
//   // 閾値を元に方向を判断する
//   for (int i = 0; i < numThresholds; i++)
//   {
//     float threshold = (statebtn_vals[i] + statebtn_vals[i + 1]) / 2;
//     if (volume >= threshold)
//     {
//       directionBits = i; // 閾値を超えたら該当する方向を示すビット列をセット
//       break;             // 最初の該当する閾値を見つけたらループを終了
//     }
//   }
//   if (volume == 0)
//   {
//     directionBits = -1;
//   }
//   return directionBits;
// }

// int LovyanGFX_DentaroUI::getPhVolVec(int n1, int n2)
// {
//   // 音声ボリュームの値
//   float x = getPhVol(n1);
//   float y = getPhVol(n2);

//   // 基準値を引いて調整
//   x -= 1950;
//   y -= 1904;

//   // 値の制限
//   if (x > 1900)
//   {
//     x = 1900;
//   }
//   else if (x < -1900)
//   {
//     x = -1900;
//   }
//   if (y > 1900)
//   {
//     y = 1900;
//   }
//   else if (y < -1900)
//   {
//     y = -1900;
//   }
//   // 角度を計算（ラジアン）
//   float angle = atan2(y, x);

//   // 角度を0~360度に変換
//   if (angle < 0)
//   {
//     angle += 2 * M_PI;
//   }
//   float degrees = angle * 180.0 / M_PI;

//   // 方向を判定
//   int direction = -1; // デフォルトは方向がないという意味

//   if (abs(x) < 50 && abs(y) < 50)
//   {
//     direction = -1; // 方向がない
//   }
//   else if (degrees >= 22.5 && degrees < 67.5)
//   {
//     direction = 0; // 右上
//   }
//   else if (degrees >= 67.5 && degrees < 112.5)
//   {
//     direction = 1; // 上
//   }
//   else if (degrees >= 112.5 && degrees < 157.5)
//   {
//     direction = 2; // 左上
//   }
//   else if (degrees >= 157.5 && degrees < 202.5)
//   {
//     direction = 3; // 左
//   }
//   else if (degrees >= 202.5 && degrees < 247.5)
//   {
//     direction = 4; // 左下
//   }
//   else if (degrees >= 247.5 && degrees < 292.5)
//   {
//     direction = 5; // 下
//   }
//   else if (degrees >= 292.5 && degrees < 337.5)
//   {
//     direction = 6; // 右下
//   }
//   else if ((degrees >= 337.5 && degrees <= 360) || (degrees >= 0 && degrees < 22.5))
//   {
//     direction = 7; // 右
//   }

//   if (abs(x) < 50 && abs(y) < 50)
//   {
//     direction = -1; // 方向がない
//   }

//   return direction;
// }

void LovyanGFX_DentaroUI::showSavedCalData(LGFX &_lcd)
{

  // タッチが使用可能な場合のキャリブレーション値の可視化を行います。（省略可）
  if (touchCalibrationF == true)
  {
    if (!SPIFFS.begin())
    {
      _lcd.println("Formating file system");
      SPIFFS.format();
      SPIFFS.begin();
    }
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f)
    {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
    _lcd.fillRect(0, 0, 128, 128, TFT_GREEN);
    _lcd.setTextSize(1);
    _lcd.setTextColor(TFT_WHITE, TFT_BLUE);
    _lcd.setCursor(0, 0);
    _lcd.println(calData[0]);
    _lcd.println(calData[1]);
    _lcd.println(calData[2]);
    _lcd.println(calData[3]);
    _lcd.println(calData[4]);
    _lcd.println(calData[5]);
    _lcd.println(calData[6]);
    _lcd.println(calData[7]);
  }
  delay(1000);
  _lcd.setTouchCalibrate(calData);
}

void LovyanGFX_DentaroUI::touchCalibration(bool _calibF)
{
  touchCalibrationF = _calibF;
}

// bool LovyanGFX_DentaroUI::begin(LGFX &_lcd, int _colBit, int _rotateNo, bool _calibF)
// {

//   _lcd.init();
//   _lcd.begin();
//   _lcd.startWrite(); // CSアサート開始
//   _lcd.setColorDepth(_colBit);
//   touchCalibrationF = _calibF;
//   begin(_lcd);
//   _lcd.setRotation(_rotateNo);
//   showSavedCalData(_lcd); // タッチキャリブレーションの値を表示
  
//   return true;
// }

void LovyanGFX_DentaroUI::begin(LGFX &_lcd, int _colBit, int _rotateNo, bool _calibF)
{
  _lcd.init();
  _lcd.begin();
  _lcd.startWrite(); // CSアサート開始
  _lcd.setColorDepth(_colBit);
  touchCalibrationF = _calibF;
  begin(_lcd);
  _lcd.setRotation(_rotateNo);
  showSavedCalData(_lcd); // タッチキャリブレーションの値を表示
}

int LovyanGFX_DentaroUI::getTouchZoom()
{
  return touchZoom;
}

void LovyanGFX_DentaroUI::setTouchZoom(int _touchzoom)
{
  touchZoom = _touchzoom;
}

void LovyanGFX_DentaroUI::begin(LGFX &_lcd, String _host, int _shiftNum, int _colBit, int _rotateNo, bool _calibF)
{
  host = _host;
  // SD.end();
  // SDカードがマウントされているかの確認

  // if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 20000000))
  // {
  //   Serial.println("Card Mount Failed");
  //   while (1)
  //   {
  //   }
  // }

  // カードタイプの取得
  // uint8_t cardType = SD.cardType();
  // if (cardType == CARD_NONE)
  // {
  //   Serial.println("No SD card attached");
  //   // 画面に案内文章を描画します。
  //   _lcd.setTextDatum(textdatum_t::middle_center);
  //   _lcd.drawString("No SD card attached.", _lcd.width() >> 1, _lcd.height() >> 1);
  //   _lcd.setTextDatum(textdatum_t::top_left);
  //   while (1)
  //   {
  //   }
  // }

  begin(_lcd, _shiftNum, _colBit, _rotateNo, _calibF);
}

void LovyanGFX_DentaroUI::begin(LGFX &_lcd, int _shiftNum, int _colBit, int _rotateNo, bool _calibF)
{

  _lcd.init();
  _lcd.begin();
  // _lcd.startWrite();//CSアサート開始
  _lcd.setColorDepth(_colBit);
  touchCalibrationF = _calibF;
  shiftNum = _shiftNum;

  // シフト数分パネルを確保
  for (int i = 0; i < shiftNum; i++)
  {
    flickPanels[i] = new FlickPanel;
  }

  use_flickUiSpriteF = true;
  // flickUiSprite = _flickUiSprite;
  begin(_lcd);
  _lcd.setRotation(_rotateNo);
}

 
std::vector<std::string> split(std::string str, char del) {
    int first = 0;
    int last = str.find_first_of(del);
 
    std::vector<std::string> result;
 
    while (first < str.size()) {
        std::string subStr(str, first, last - first);
 
        result.push_back(subStr);
 
        first = last + 1;
        last = str.find_first_of(del, first);
 
        if (last == std::string::npos) {
            last = str.size();
        }
    }
 
    return result;
}

String LovyanGFX_DentaroUI::rCalData(String _wrfile){
  File fr = SPIFFS.open(_wrfile.c_str(), "r");// ⑩ファイルを読み込みモードで開く
  String _readStr = fr.readStringUntil('\n');// ⑪改行まで１行読み出し
  fr.close();	// ⑫	ファイルを閉じる
  return _readStr;
}
void LovyanGFX_DentaroUI::calibrationRun( LGFX& _lcd){
// タッチが使用可能な場合のキャリブレーションを行います。（省略可）
  
    if (_lcd.touch())
    {
      Serial.println("タッチされました");
      if (_lcd.width() < _lcd.height()) _lcd.setRotation(_lcd.getRotation() ^ 1);

      _lcd.fillScreen(TFT_BLACK);
      _lcd.setColorDepth(COL_DEPTH);

      _lcd.setTextSize(1);
      _lcd.setFont(&lgfxJapanGothicP_8);
      // _lcd.setTextColor(TFT_WHITE, TFT_BLACK);

      // 画面に案内文章を描画します。
      _lcd.setTextDatum(textdatum_t::middle_center);
      _lcd.drawString("矢印をタッチしてください。", _lcd.width()>>1, _lcd.height() >> 1);
      _lcd.setTextDatum(textdatum_t::top_left);

      // タッチを使用する場合、キャリブレーションを行います。画面の四隅に表示される矢印の先端を順にタッチしてください。
      uint16_t fg = TFT_WHITE;
      uint16_t bg = TFT_BLACK;

      if (_lcd.isEPD()) swap(fg, bg);

      // _lcd.calibrateTouch(nullptr, fg, bg, max(_lcd.width(), _lcd.height()) >> 3);

      String readStr = rCalData(CALIBRATION_FILE);
      char del = ',';
      int i=0;
      for (const auto subStr : split(readStr.c_str(), del)) {//csv形式のStringから数値文字を取り出し
        int calval = atoi(subStr.c_str());//c_strでconst char*に変換してからatoiで整数値に
        calData[i] = calval;
        i++;
      }
      _lcd.calibrateTouch(calData, fg, bg, max(_lcd.width(), _lcd.height()) >> 3);
      _lcd.setTouchCalibrate(calData);
     }
  }

void LovyanGFX_DentaroUI::begin(LGFX &_lcd)
{
  lcdPos.x = 0;
  lcdPos.y = 0;
  if(uiMode == TOUCH_MODE){
  }else if(uiMode == PHYSICAL_MODE){
    selectBtnID = 0;
  }

  if(touchCalibrationF == true){
    calibrationRun(_lcd);
  }else{
    // String readStr = rCalData(CALIBRATION_FILE);

    File fr = SPIFFS.open(CALIBRATION_FILE, "r"); // パスを適切に指定してください
    String line;
    int i = 0;

    while (fr.available()) {
      line = fr.readStringUntil(',');
      if (!line.isEmpty()) {
        calData[i] = line.toInt();
        Serial.print(calData[i]);
        Serial.print(",");
        i++;
      }
    }

    fr.close();

      // char del = ',';
      // int i=0;
      // for (const auto subStr : split(readStr.c_str(), del)) {//csv形式のStringから数値文字を取り出し
      //   int calval = atoi(subStr.c_str());//c_strでconst char*に変換してからatoiで整数値に
      //   calData[i] = calval;
      //   Serial.
      //   i++;
      // }
      _lcd.setTouchCalibrate(calData);

  }
  
  Serial.println("");
  Serial.println("[UI_ID information]");
}


void LovyanGFX_DentaroUI::update2( LGFX& _lcd )
{
  _lcd.getTouch(&tp);
  for(int i = 0; i < 4; i++)
  {
    clist[i+1] = clist[i];
  }
  if(eventState != NO_EVENT) preEventState = eventState;//前のイベント状態を保持

  //以下イベント取得処理
  eventState = NO_EVENT;//イベント初期化
  flickState = NO_EVENT;//-1で初期化
  obj_ret.reset();
  
  //1ビット左にシフト
  touchState = touchState<<1;

  clist[0] = TFT_WHITE;
  
  if ( _lcd.getTouch( &tp ) )
  {
    bitSet(touchState, 0);//右０ビット目を１（Set）に
  }else{
    bitClear(touchState, 0);//右０ビット目を0（Clear）に
  }
  //下位２ビットをみてイベントを振り分け
  touchState &= B00000011;//マスク
  if (touchState == B_TOUCH){
    sp = tp; touchStartTime =  micros();
    eventState = TOUCH;//7 
    setEventBit(TOUCH,1);
  }
  else if (touchState == B_MOVE) {
    clist[0] = TFT_GREEN;
    unsigned long holdingTime = micros() - touchStartTime;
    
    eventState = WAIT;
    setEventBit(WAIT,1);
    if(holdingTime > HOLDING_SEC){
      clist[0] = TFT_YELLOW;
      eventState = MOVE;//8
      setEventBit(MOVE,1);
    }
  }
  else if (touchState == B_RELEASE) 
  {
    clist[0] = TFT_BLUE;
    unsigned long holdingTime = micros() - touchStartTime;
    float dist = sqrt( pow((tp.x - sp.x),2) + pow((tp.y - sp.y), 2) );

    if(holdingTime > HOLDING_SEC - 10000 && holdingTime <= HOLDING_SEC){
      if(dist <= FLICK_DIST){
        eventState = WAIT;//5
        setEventBit(WAIT,1);
      }
    }
    else if(holdingTime > HOLDING_SEC)
    {
      eventState = DRAG;//2
      setEventBit(DRAG,1);
    }
    else if(dist <= FLICK_DIST)
    {
      float dist2 = sqrt( pow((tp.x - sp2.x),2) + pow((tp.y - sp2.y),2) );
      
      if(preTouchStartTime != 0)
      {
        unsigned long tappingTime  = touchStartTime - preTouchStartTime;
        if( tappingTime < TAP_TO_TAP_SEC ){//前とのタップ間の時間調整, 位置判定
          if(dist2 < FLICK_DIST) tapCount++;//短距離、短時間なら加算
        }
        else if( tappingTime >= TAP_TO_TAP_SEC )//時間が空いていたらリセット
        {

          lastTapCount = tapCount;
          if(lastTapCount>0){
            eventState = WTAPPED;
            setEventBit(WTAPPED, 1);
          }else if(lastTapCount==0){
            eventState = TAPPED;
            setEventBit(TAPPED, 1);
          }
          tapCount = 0;

        }
      }

      //tapCount++;//今回タップした分を足す
      lastTappedTime = micros();
      // Serial.print(tapCount);
      // Serial.print(":");
      // Serial.print(micros());
      // Serial.print(":");
      // Serial.println(micros() - firstTappedTime);

      
      // if(endTapCount)

      if(tapCount == 0){
        firstTappedTime = touchStartTime;//tap番号0のタップスタート時間を格納
        eventState = TAPPED;
        setEventBit(TAPPED, 1);
      }

      if(tapCount == 1){
        eventState = WTAPPED;//4
        setEventBit(WTAPPED, 1);
      }
      

      sp2 = tp;
      preTouchStartTime = touchStartTime;
    }
    else if(dist > FLICK_DIST)
    {
       if(holdingTime <= HOLDING_SEC){
        float angle = getAngle(sp, tp);
  //      Serial.println(angle);
          
          if(angle <= 22.5 || angle > 337.5){
            eventState = RIGHT_FLICK;//0
            setEventBit(RIGHT_FLICK, 1);
            flickState = RIGHT_FLICK;//0
          }else if(angle <= 67.5 && angle > 22.5){
            eventState = R_D_FLICK;//7
            setEventBit(R_D_FLICK, 1);
            flickState = R_D_FLICK;//7
          }else if(angle <= 112.5 && angle > 67.5){
            eventState = DOWN_FLICK;//6
            setEventBit(DOWN_FLICK, 1);
            flickState = DOWN_FLICK;//6
          }else if(angle <= 157.5 && angle > 112.5){
            eventState = D_L_FLICK;//5
            setEventBit(D_L_FLICK, 1);
            flickState = D_L_FLICK;//5
          }else if(angle <= 202.5 && angle > 157.5){
            eventState = LEFT_FLICK;//4
            setEventBit(LEFT_FLICK, 1);
            flickState = LEFT_FLICK;//4
          }else if(angle <= 247.5 && angle > 202.5){
            eventState = L_U_FLICK;//3
            setEventBit(L_U_FLICK, 1);
            flickState = L_U_FLICK;//3
          }else if(angle <= 292.5 && angle > 247.5){
            eventState = UP_FLICK;//2
            setEventBit(UP_FLICK, 1);
            flickState = UP_FLICK;//2
          }else if(angle <= 337.5 && angle > 292.5){
            eventState = U_R_FLICK;//1
            setEventBit(U_R_FLICK, 1);
            flickState = U_R_FLICK;//1
          }

       }
    }
    eventState = RELEASE;//9
    setEventBit(RELEASE,1);
  }
  else if (touchState == B_UNTOUCH)
  {
    clist[0] = TFT_DARKGREEN;

  // if(micros() - firstTappedTime < 200000 && jadgeF == false){
  //   jadgeF = true;
  // }

    // if(micros() - firstTappedTime > 200000 && jadgeF == true){
    //   eventState = TAPPED;//
    //   setEventBit(TAPPED, 1);
    //   jadgeF = false;
    // }
  }
else if (touchState == B_UNTOUCH)
  {
    clist[0] = TFT_DARKGREEN;
  }
  for (int k = 0; k < uis.size(); k++)
  {  // UIの数
    for (int i = 0; i < uis[k]->panels.size(); i++)
    { // パネルの数
      // Serial.print(panels[i]->id);
      for (int j = 0; j < uis[k]->panels[i]->b_num; j++)
      { // パネル内のボタンの数
        addHandler(k,i, j, setBtnID_ret_DG, uis[k]->panels[i]->eventNo, uis[k]->panels[i]->parentID, constantGetF);
        addHandler(k,i, j, setBtnNO_ret_DG, uis[k]->panels[i]->eventNo, uis[k]->panels[i]->parentID, constantGetF);
      }
    }
  }

  if (constantGetF)
  {
    if (getEvent() == NO_EVENT)
    {
      obj_ret.setBtnID_ret(-1); // ‐１（ボタンが離れている状態）にクリア
    }
    // if(getEvent() != TOUCH)obj_ret.setBtnID_ret(-1);//‐１（ボタンが離れている状態）にクリア
  }


  // constantBtnID = obj_ret.btnID;
}

// 交点を求める関数
lgfx::v1::touch_point_t findIntersection(double x, double y, double x2, double y2, int h) {
    // 直線の傾きを計算
    double m = (y2 - y) / (x2 - x);

    // 直線のy切片を計算
    double b = y - m * x;

    // 高さ128との水平線との交点を計算
    double intersection_x = (h - b) / m;
    double intersection_y = h;
    lgfx::v1::touch_point_t p;
    p.x = intersection_x;
    p.y = intersection_y;
    return p;
}


size_t LovyanGFX_DentaroUI::getVecNo()
{
  lgfx::v1::touch_point_t ip = findIntersection(sp.x, sp.y+76, tp.x, tp.y, sp.y-114);//最後の高さ0との交点
  size_t btn_w = 48;

  if(ip.x >= 0 && ip.x < btn_w){
    return 0;
  }else if(ip.x >= btn_w && ip.x < btn_w*2){
    return 1;
  }else if(ip.x >= btn_w*2 && ip.x < btn_w*3){
    return 2;
  }else if(ip.x >= btn_w*3 && ip.x < btn_w*4){
    return 3;
  }else if(ip.x >= btn_w*4 && ip.x < btn_w*5){
    return 4;
  }else{
    return 0;
  }

    // float angle = getAngle(sp, tp);
          
    // if(angle <= 22.5 || angle > 337.5){
    //   eventState = RIGHT_FLICK;//0
    //   setEventBit(RIGHT_FLICK, 1);
    //   flickState = RIGHT_FLICK;//0
    // }else if(angle <= 67.5 && angle > 22.5){
    //   eventState = R_D_FLICK;//7
    //   setEventBit(R_D_FLICK, 1);
    //   flickState = R_D_FLICK;//7
    // }else if(angle <= 112.5 && angle > 67.5){
    //   eventState = DOWN_FLICK;//6
    //   setEventBit(DOWN_FLICK, 1);
    //   flickState = DOWN_FLICK;//6
    // }else if(angle <= 157.5 && angle > 112.5){
    //   eventState = D_L_FLICK;//5
    //   setEventBit(D_L_FLICK, 1);
    //   flickState = D_L_FLICK;//5
    // }else if(angle <= 202.5 && angle > 157.5){
    //   eventState = LEFT_FLICK;//4
    //   setEventBit(LEFT_FLICK, 1);
    //   flickState = LEFT_FLICK;//4
    // }else if(angle <= 247.5 && angle > 202.5){
    //   eventState = L_U_FLICK;//3
    //   setEventBit(L_U_FLICK, 1);
    //   flickState = L_U_FLICK;//3
    // }else if(angle <= 292.5 && angle > 247.5){
    //   eventState = UP_FLICK;//2
    //   setEventBit(UP_FLICK, 1);
    //   flickState = UP_FLICK;//2
    // }else if(angle <= 337.5 && angle > 292.5){
    //   eventState = U_R_FLICK;//1
    //   setEventBit(U_R_FLICK, 1);
    //   flickState = U_R_FLICK;//1
    // }

    // return flickState-11;
}



void LovyanGFX_DentaroUI::update(LGFX &_lcd)
{
  // constantGetF = true;
  _lcd.getTouch(&tp);

  for (int i = 0; i < 4; i++)
  {
    clist[i + 1] = clist[i];
  }
  if (eventState != NO_EVENT)
    preEventState = eventState; // 前のイベント状態を保持

  // 以下イベント取得処理
  eventState = NO_EVENT; // イベント初期化
  flickState = NO_EVENT; //-1で初期化
  obj_ret.reset();

  // 1ビット左にシフト
  touchState = touchState << 1;

  clist[0] = TFT_WHITE;

  if (_lcd.getTouch(&tp))
  {
    bitSet(touchState, 0); // 右０ビット目を１（Set）に
  }
  else
  {
    bitClear(touchState, 0); // 右０ビット目を0（Clear）に
  }
  // 下位２ビットをみてイベントを振り分け
  touchState &= B00000011;
  if (touchState == B_TOUCH)
  {
    sp = tp;
    sTime = micros();
    eventState = TOUCH; // 6
  }
  else if (touchState == B_MOVE)
  {
    clist[0] = TFT_GREEN;
    unsigned long touchedTime = micros() - sTime;
    eventState = WAIT;
    if (touchedTime > 160000)
    {
      clist[0] = TFT_YELLOW;
      eventState = MOVE; // 8
    }
  }
  else if (touchState == B_RELEASE)
  {
    clist[0] = TFT_BLUE;
    unsigned long touchedTime = micros() - sTime;
    float dist = sqrt(pow((tp.x - sp.x), 2) + pow((tp.y - sp.y), 2));

    if (touchedTime > 120000 && touchedTime <= 160000)
    {
      if (dist <= FLICK_DIST)
      {
        eventState = WAIT; // 5
      }
    }
    else if (touchedTime > 160000)
    {
      eventState = DRAG; // 2
    }
    else if (dist <= FLICK_DIST)
    {
      eventState = TAPPED; // 4

      float dist2 = sqrt(pow((tp.x - sp2.x), 2) + pow((tp.y - sp2.y), 2));
      if (sTime - tappedTime < 200000 && dist2 < FLICK_DIST)
      {                       // ダブルタップの間の時間調整, 位置判定
        eventState = WTAPPED; // 5
      }
      tappedTime = micros();
      sp2 = tp;
    }
    else if (dist > FLICK_DIST)
    {
      if (touchedTime <= 160000)
      {
        float angle = getAngle(sp, tp);

        if (angle <= 22.5 || angle > 337.5)
        {
          eventState = RIGHT_FLICK; // 0
          flickState = RIGHT_FLICK; // 0
        }
        else if (angle <= 67.5 && angle > 22.5)
        {
          eventState = R_D_FLICK; // 7
          flickState = R_D_FLICK; // 7
        }
        else if (angle <= 112.5 && angle > 67.5)
        {
          eventState = DOWN_FLICK; // 6
          flickState = DOWN_FLICK; // 6
        }
        else if (angle <= 157.5 && angle > 112.5)
        {
          eventState = D_L_FLICK; // 5
          flickState = D_L_FLICK; // 5
        }
        else if (angle <= 202.5 && angle > 157.5)
        {
          eventState = LEFT_FLICK; // 4
          flickState = LEFT_FLICK; // 4
        }
        else if (angle <= 247.5 && angle > 202.5)
        {
          eventState = L_U_FLICK; // 3
          flickState = L_U_FLICK; // 3
        }
        else if (angle <= 292.5 && angle > 247.5)
        {
          eventState = UP_FLICK; // 2
          flickState = UP_FLICK; // 2
        }
        else if (angle <= 337.5 && angle > 292.5)
        {
          eventState = U_R_FLICK; // 1
          flickState = U_R_FLICK; // 1
        }
      }
    }
    eventState = RELEASE; // 9
  }
  else if (touchState == B_UNTOUCH)
  {
    clist[0] = TFT_DARKGREEN;
  }
  for (int k = 0; k < uis.size(); k++)
  {  // UIの数
    for (int i = 0; i < uis[k]->panels.size(); i++)
    { // パネルの数
      // Serial.print(panels[i]->id);
      for (int j = 0; j < uis[k]->panels[i]->b_num; j++)
      { // パネル内のボタンの数
        addHandler(k,i, j, setBtnID_ret_DG, uis[k]->panels[i]->eventNo, uis[k]->panels[i]->parentID, constantGetF);
        addHandler(k,i, j, setBtnNO_ret_DG, uis[k]->panels[i]->eventNo, uis[k]->panels[i]->parentID, constantGetF);
      }
    }
  }

  if (constantGetF)
  {
    if (getEvent() == NO_EVENT)
    {
      obj_ret.setBtnID_ret(-1); // ‐１（ボタンが離れている状態）にクリア
    }
    // if(getEvent() != TOUCH)obj_ret.setBtnID_ret(-1);//‐１（ボタンが離れている状態）にクリア
  }


  // constantBtnID = obj_ret.btnID;
}

void LovyanGFX_DentaroUI::setConstantGetF(bool _constantGetF)
{
  constantGetF = _constantGetF;
}


    size_t getCharSize(char c) {
        // 単純な実装例（UTF-8の場合）
        if ((c & 0b10000000) == 0) {
            // 1バイト文字
            return 1;
        } else if ((c & 0b11100000) == 0b11000000) {
            // 2バイト文字
            return 2;
        } else if ((c & 0b11110000) == 0b11100000) {
            // 3バイト文字
            return 3;
        } else {
            // その他の場合、1バイトとして処理
            return 1;
        }
    }

String LovyanGFX_DentaroUI::delChar(const String& _flickStr) {
        // 文字列が空の場合はそのまま返す
        if (_flickStr.isEmpty()) {
            return _flickStr;
        }

        // Stringをstd::stringに変換
        std::string updatedStr = _flickStr.c_str();

        // 最後の文字を取り除く
        size_t lastCharSize = getCharSize(updatedStr.back());
        updatedStr.erase(updatedStr.length() - lastCharSize);

        // std::stringをStringに変換して返す
        return String(updatedStr.c_str());
    }

// String LovyanGFX_DentaroUI::delChar(String _flickStr) {
//   // 文字列が空の場合はそのまま返す
//   if (_flickStr.isEmpty()) {
//     return _flickStr;
//   }

//   // 最後の文字を取り除く
//   _flickStr.remove(_flickStr.length() - 1);

//   // 更新後の文字列を返す
//   return _flickStr;
// }


// 次のマルチバイト文字へのポインタを取得
const char *LovyanGFX_DentaroUI::next_c_mb(const char *c)
{
  int L = mblen(c, 10);
  return c + L;
}

// マルチバイト文字を一文字取り出す
void LovyanGFX_DentaroUI::ngetc(char *const dst, const char *src)
{
  int L = mblen(src, 10);
  memcpy(dst, src, L);
  dst[L] = '\0';
}

// マルチバイト文字を比較する
bool LovyanGFX_DentaroUI::nchr_cmp(const char *c1, const char *c2)
{
  int K = mblen(c1, 10);
  int L = mblen(c2, 10);

  if (K != L)
    return false;

  bool issame = (strncmp(c1, c2, K) == 0);
  return issame;
}

std::vector<std::string> LovyanGFX_DentaroUI::split_mb(const char *src, const char *del)
{

  char tmp[10];

  std::vector<std::string> result;

  std::string tmps;
  while (*src)
  {

    // デリミタを飛ばす
    //  const char* p = src;
    while (nchr_cmp(src, del) == true && *src != '\0')
      src = next_c_mb(src);

    // デリミタに遭遇するまで文字を追加し続ける
    while (nchr_cmp(src, del) != true && *src != '\0')
    {
      ngetc(tmp, src); // 一文字取り出す
      tmps += tmp;
      src = next_c_mb(src);
    }
    if (tmps.size())
    {
      result.push_back(tmps);
    }
    tmps.clear();
  }

  return result;
}

void LovyanGFX_DentaroUI::updateSelectBtnID(int _selectBtnID)
{
  selectBtnID = _selectBtnID;
}

void LovyanGFX_DentaroUI::showTouchEventInfo(LovyanGFX &_lcd, int _x, int _y)
{
  for (int i = 0; i < 5; i++)
  {
    _lcd.fillRect(_x + 1 + 10 * i, _y, 9, 9, clist[0]);
  }
}

void LovyanGFX_DentaroUI::createToggles(int _x, int _y, int _w, int _h, int _row, int _col, int _eventNo)
{
  toggle_mode = true;
  // createBtns(_x, _y, _w, _h, _row, _col, _eventNo);
  toggle_mode = false;
}

void LovyanGFX_DentaroUI::createFlick9Btns(LGFX_Sprite &_flickUiSprite)
{
  flickPanel.b_sNo = 0;
  flickPanel.id = 0;
  flickPanel.x = 0;
  flickPanel.y = 0;
  //  flickPanel.w   = 144;
  //  flickPanel.h   = 144;
  flickPanel.row = 3;
  flickPanel.col = 3;
  // flickPanel.eventNo = _eventNo;

  //  _lcd.setPsram( USE_PSRAM );
  // _lcd.setPsram(false);//UNUSE_PSRAM
  // _lcd.setColorDepth( COL_DEPTH );
  // _lcd.createSprite( 144, 144 );
  //  _lcd.createSprite( flickPanel.w, flickPanel.h );

  // btnNum = _row * _col ;
  int b_w = 48;
  int b_h = 48;

  for (int j = 0; j < 3; j++)
  {
    for (int i = 0; i < 3; i++)
    {
      // if( i*flickPanel.col + j > 0 ){
      //    flick_touch_btn_list.push_back( new TouchBtn() );
      // }
      // else
      // {
      //   flick_touch_btn_list[ i*flickPanel.col + j ] = NULL;
      //   flick_touch_btn_list[ 0 ] = new TouchBtn();
      // }

      flick_touch_btn_list[i * flickPanel.col + j] = NULL;
      flick_touch_btn_list[i * flickPanel.col + j] = new TouchBtn();

      flick_touch_btn_list[i * flickPanel.col + j]->initBtn(i * flickPanel.col + j, "a",
                                                            i * b_w,
                                                            j * b_h,
                                                            b_w,
                                                            b_h,
                                                            getTouchPoint(0, 0),
                                                            TOUCH_FLICK_MODE);

      // ベータ版は基本true
      flick_touch_btn_list[i * flickPanel.col + j]->setVisibleF(true);
      flick_touch_btn_list[i * flickPanel.col + j]->setAvailableF(true);
    }
  }
  flickPanel.b_num = 9; // UIのボタン数をセット
}

void LovyanGFX_DentaroUI::setFlickPanel(int _flickPanelID, int _btnID, String _btnsString)
{
  flickPanels[_flickPanelID]->new_text_list(_btnID, _btnsString);
}

String LovyanGFX_DentaroUI::getFlickStr()
{
  if (getTouchBtnID() != NO_EVENT)
  {
    // 押されたボタンのモードがFLICKだったら
    if (touch_btn_list[getTouchBtnID()]->getBtnMode() == TOUCH_FLICK_MODE)
    {

      std::string gettingText = flickPanels[showFlickPanelNo]->text_list[getTouchBtnID() - getUiFirstNo(FlickUiID)].c_str();
      std::string trimText = "";
      flickString = "";
      trimText = gettingText;

      float angle = getAngle(sp, tp);
      //      Serial.println(angle);
      float dist = getDist(sp, tp);
      if (charMode == CHAR_3_BYTE)
      { // 日本語3ビットの場合
        // ５文字の場合
        if (dist > 24)
        {

          if (gettingText.size() == 15) // 5文字以下
          {
            charNumMode = CHAR_3_BYTE_5; // 5文字
            if (angle <= 22.5 || angle > 337.5)
            {
              flickString = trimText.substr(3 * 3, 3).c_str(); // え
              curKanaColNo = 3;
            }
            else if (angle <= 112.5 && angle > 67.5)
            {
              flickString = trimText.substr(4 * 3, 3).c_str(); // お
              curKanaColNo = 4;
            }
            else if (angle <= 202.5 && angle > 157.5)
            {
              flickString = trimText.substr(1 * 3, 3).c_str(); // い
              curKanaColNo = 1;
            }
            else if (angle <= 292.5 && angle > 247.5)
            {
              flickString = trimText.substr(2 * 3, 3).c_str(); // う
              curKanaColNo = 2;
            }
          }
          else if (gettingText.size() == 27) // 9文字
          {
            charNumMode = CHAR_3_BYTE_9; // 9文字
            if (angle <= 22.5 || angle > 337.5)
            {
              flickString = trimText.substr(5 * 3, 3).c_str(); // か
              curKanaColNo = 5;
            }
            else if (angle <= 112.5 && angle > 67.5)
            {
              flickString = trimText.substr(7 * 3, 3).c_str(); // く
              curKanaColNo = 7;
            }
            else if (angle <= 202.5 && angle > 157.5)
            {
              flickString = trimText.substr(1 * 3, 3).c_str(); // い
              curKanaColNo = 1;
            }
            else if (angle <= 292.5 && angle > 247.5)
            {
              flickString = trimText.substr(3 * 3, 3).c_str(); // え
              curKanaColNo = 3;
            }
            else if (angle <= 67.5 && angle > 22.5)
            {
              flickString = trimText.substr(6 * 3, 3).c_str(); // き
              curKanaColNo = 6;
            }
            else if (angle <= 157.5 && angle > 112.5)
            {
              flickString = trimText.substr(8 * 3, 3).c_str(); // け
              curKanaColNo = 8;
            }
            else if (angle <= 247.5 && angle > 202.5)
            {
              flickString = trimText.substr(2 * 3, 3).c_str(); // う
              curKanaColNo = 2;
            }
            else if (angle <= 337.5 && angle > 292.5)
            {
              flickString = trimText.substr(4 * 3, 3).c_str(); // お
              curKanaColNo = 4;
            }
          }
        }
        else if (dist <= 24)
        {
          flickString = trimText.substr(0, 3).c_str(); // あ
          curKanaColNo = 0;
        }
        return flickString;
      }
      else if (charMode == CHAR_1_BYTE)
      { // 英数１ビットの場合
        if (dist > 24)
        {

          if (gettingText.size() <= 5) // 1バイト文字5文字（英語）以下の場合
          {
            charNumMode = CHAR_1_BYTE_5; // 5文字
            flickString = "";
            if (angle <= 22.5 || angle > 337.5)
            {
              if (trimText.length() >= 4)
                flickString = trimText.substr(3, 1).c_str(); // D
            }

            if (angle <= 112.5 && angle > 67.5 && trimText.length() >= 5)
            {
              flickString = trimText.substr(4, 1).c_str(); // E
            }

            if (angle <= 202.5 && angle > 157.5 && trimText.length() >= 2)
            {
              flickString = trimText.substr(1, 1).c_str(); // B
            }

            if (angle <= 292.5 && angle > 247.5 && trimText.length() >= 3)
            {
              flickString = trimText.substr(2, 1).c_str(); // C
            }
          }

          if (gettingText.size() == 9) // 1バイト文字5文字（英語）以下の場合
          {
            charNumMode = CHAR_1_BYTE_9; // 9文字
            flickString = "";

            if (angle <= 22.5 || angle > 337.5)
            {
              flickString = trimText.substr(5, 1).c_str(); // かF
              // curKanaColNo = 5;
            }
            else if (angle <= 112.5 && angle > 67.5)
            {
              flickString = trimText.substr(7, 1).c_str(); // くH
              // curKanaColNo = 7;
            }
            else if (angle <= 202.5 && angle > 157.5)
            {
              flickString = trimText.substr(1, 1).c_str(); // いB
              // curKanaColNo = 1;
            }
            else if (angle <= 292.5 && angle > 247.5)
            {
              flickString = trimText.substr(3, 1).c_str(); // えD
              // curKanaColNo = 3;
            }
            else if (angle <= 67.5 && angle > 22.5)
            {
              flickString = trimText.substr(6, 1).c_str(); // きG
              // curKanaColNo = 6;
            }
            else if (angle <= 157.5 && angle > 112.5)
            {
              flickString = trimText.substr(8, 1).c_str(); // けI
              // curKanaColNo = 8;
            }
            else if (angle <= 247.5 && angle > 202.5)
            {
              flickString = trimText.substr(2, 1).c_str(); // うC
              // curKanaColNo = 2;
            }
            else if (angle <= 337.5 && angle > 292.5)
            {
              flickString = trimText.substr(4, 1).c_str(); // おE
              // curKanaColNo = 4;
            }
          }
        }
        else if (dist <= 24)
        {
          flickString = trimText.substr(0, 1).c_str(); // A
        }
        return flickString;
      }
    }
  }
  return "";
}

void LovyanGFX_DentaroUI::drawFlickBtns(LovyanGFX &_lcd, int _btnID, int _btnNo, bool _visible, bool _available)
{

  if (_btnID != NO_EVENT)
  {

    // 押されたボタンのモードがFLICKだったら
    if (touch_btn_list[getTouchBtnID()]->getBtnMode() == TOUCH_FLICK_MODE)
    {
      // if(_visible){
      //  _lcd.setPivot( 0, 0 );//setPivot()で回転する場合の原点を指定します。初期値は左上の(0, 0)だと思います

      // _lcd.fillScreen(TFT_RED);

      std::string panelText = "０１２３４５６７８"; // デフォルトの文字セット
      // std::string prePanelText ="";
      std::string gettingText = flickPanels[showFlickPanelNo]->text_list[_btnNo].c_str(); // ArduinoのString型をc_str()で、std::string型に直してから、渡す
      std::string trimText = "";

      if (gettingText.size() <= 5) // 1バイト文字5文字（英語）以下の場合
      {
        panelText = "";
        for (int i = 0; i < 9; i++)
        {
          int swapArray[9] =
              {5, 2, 6,
               1, 0, 3,
               8, 4, 7};
          if (swapArray[i] < gettingText.size())
          {
            trimText = gettingText;
            panelText += trimText.substr(swapArray[i], 1);
            panelText += " ";
            panelText += " ";
          }
          else
          {
            panelText += "　";
          }
        }
      }
      else if (gettingText.size() == 9) // 1バイト文字9文字（英語）以下の場合
      {
        panelText = "";
        for (int i = 0; i < 9; i++)
        {
          int swapArray[9] =
              {2, 3, 4,
               1, 0, 5,
               8, 7, 6};
          if (swapArray[i] < gettingText.size())
          {
            trimText = gettingText;
            panelText += trimText.substr(swapArray[i], 1);
            panelText += " ";
            panelText += " ";
          }
          else
          {
            panelText += "　";
          }
        }
      }
      else if (gettingText.size() == 15) // ２バイト文字（日本語）5文字の場合
      {
        panelText = "";

        panelText += "　"; //"５";//gettingText.substr(5*3,3).c_str();か
        trimText = gettingText;
        panelText += trimText.substr(2 * 3, 3); // う
        panelText += "　";                      //"８";//gettingText.substr(8*3,3).c_str();け

        trimText = gettingText;
        panelText += trimText.substr(1 * 3, 3); // い
        trimText = gettingText;
        panelText += trimText.substr(0, 3); // あ
        trimText = gettingText;
        panelText += trimText.substr(3 * 3, 3); // え

        panelText += "　"; //"６";//gettingText.substr(6*3,3).c_str();き
        trimText = gettingText;
        panelText += trimText.substr(4 * 3, 3); // お
        panelText += "　";                      //"７";//gettingText.substr(7*3,3).c_str();く
      }
      else if (gettingText.size() == 27) // 9文字の場合
      {
        panelText = "";

        trimText = gettingText;
        panelText += trimText.substr(2 * 3, 3); // う
        trimText = gettingText;
        panelText += trimText.substr(3 * 3, 3); // え
        trimText = gettingText;
        panelText += trimText.substr(4 * 3, 3); // お

        trimText = gettingText;
        panelText += trimText.substr(1 * 3, 3); // い
        trimText = gettingText;
        panelText += trimText.substr(0, 3); // あ
        trimText = gettingText;
        panelText += gettingText.substr(5 * 3, 3); // か

        trimText = gettingText;
        panelText += gettingText.substr(8 * 3, 3); // け
        trimText = gettingText;
        panelText += gettingText.substr(7 * 3, 3); // く
        trimText = gettingText;
        panelText += gettingText.substr(6 * 3, 3); // き
      }

      // panelText.replace( 0, gettingText.size(), gettingText );//　"あああ"　がきたら　"あああ３４５６７８"　のように置き換える

      // 以下なぜか動かず、、、
      //  if(panelText.size()<27){//3バイト*9文字以下なら
      //    panelText.replace(0,gettingText.size(), gettingText);//　"あああ"　がきたら　"あああ３４５６７８"　のように置き換える
      //  }else{
      //    panelText.substr(0,27);//3バイト*9文字のみ切り出す
      //  }

      // フリックパネルの文字を描画
      for (int j = 0; j < 3; j++)
      {
        for (int i = 0; i < 3; i++)
        {
          _lcd.setTextSize(1);

          flick_touch_btn_list[j * 3 + i]->setBtnName(panelText.substr((j * 3 + i) * 3, 3).c_str()); // std::string型の状態で一文字切り出して、Stringに渡す
          // flick_touch_btn_list[ j*3+i ]->setBtnName(panelText.substr(3,3).c_str());//std::string型の状態で一文字切り出して、Stringに渡す

          // String btn_name = flickPanels[showFlickPanelNo]->btn_name.c_str();
          String btn_name = flick_touch_btn_list[j * 3 + i]->getBtnName();

          // Serial.print(panelText.substr((j*3+i)*3,3).c_str());
          // Serial.println(btn_name);

          _lcd.fillRoundRect(
              // flick_touch_btn_list[i]->getBtnPos().x,
              // flick_touch_btn_list[i]->getBtnPos().y,
              48 * i,
              48 * j,
              48, // flick_touch_btn_list[i]->getBtnSize().w,
              48, // flick_touch_btn_list[i]->getBtnSize().h,
              10, TFT_WHITE);

          if (btn_name == NULL)
          {
            btn_name = String(j * 3 + i);
          }

          _lcd.setTextColor(TFT_BLACK);
          _lcd.setFont(&lgfxJapanGothicP_20);
          int b_hw = 48 / 2;
          int b_hh = 48 / 2;
          float s_str_hw = _lcd.textWidth(btn_name) / 2;
          // _lcd.drawString( flick_touch_btn_list[ j*3+i ]->getBtnName(), 48*i + b_hw - s_str_hw , 48*j + b_hh - 4 );

          // String btn_name_headline = "";
          
          // if(btn_name == "A")btn_name_headline = "ABC";
          // else if(btn_name == "a")btn_name_headline = "abc";

          // _lcd.drawString( btn_name_headline, 48 * i + b_hw - s_str_hw, 48 * j + b_hh - 4);
          _lcd.drawString(btn_name, 48 * i + b_hw - s_str_hw, 48 * j + b_hh - 4);
        }
      }
      // _lcd.fillRoundRect(0, 0, 50, 50, 10, TFT_WHITE);
      // _lcd.pushSprite(&_lcd, _x, _y);
      // }
      // else if(!_visible){
      //_lcd.fillRoundRect(flick_touch_btn_list[i]->, 0, 48, 48, 10, TFT_WHITE);
      // _lcd.pushSprite(&_lcd, getBtnPos(_btnID).x, getBtnPos(_btnID).y);
      // _lcd.pushSprite(&_lcd, getBtnPos(_btnID).x, getBtnPos(_btnID).y);
      // }
    }
  }
}

void LovyanGFX_DentaroUI::createOBtns(
    int _r0, int _r1, int _a, int _n, int _eventNo)
{
  int _x = 125; // 初期値を適当に入れておく
  int _y = 245; // 初期値を適当に入れておく
  createOBtns(_x, _y, _r0, _r1, _a, _n, _eventNo);
}

void LovyanGFX_DentaroUI::createOBtns(
    int _x, int _y, int _r0, int _r1, int _a, int _n, int _eventNo)
{ // 円形に並ぶ//中心位置XY、外半径r0、内半径r1、スタート角、分割数

  // int _x, int _y,
  // int _w,int _h,
  // int _row, int _col,
  // LGFX_Sprite& _uiSprite, int _eventNo)
  // panels.push_back(*new Panel);

  // panels_num++;
  // uiId++;
  // uis[_uiId]->panels[0]->label = "BTN_" + String(uiId);
  // Serial.println("BTN_" + String(uiId) + "=[" + String(uiId) + "]" + String(gBtnID) + "~");

  // int _startId = gBtnID; // スタート時のボタンIDをセット
  // uis[_uiId]->panels[0]->b_sNo = gBtnID;
  // uis[_uiId]->panels[0]->id = uiId;
  // uis[_uiId]->panels[0]->x = _x; // - _r1;
  // uis[_uiId]->panels[0]->y = _y; // - _r1;

  // // uis[_uiId]->panels[0]->w   = _w;
  // // uis[_uiId]->panels[0]->h   = _h;
  // // uis[_uiId]->panels[0]->row = _row;
  // // uis[_uiId]->panels[0]->col = _col;

  // uis[_uiId]->panels[0]->r0 = _r0;
  // uis[_uiId]->panels[0]->r1 = _r1;
  // uis[_uiId]->panels[0]->a = _a;
  // uis[_uiId]->panels[0]->n = _n;

  // uis[_uiId]->panels[0]->eventNo = _eventNo;
  // uis[_uiId]->panels[0]->toggle_mode = toggle_mode;

  // // _uiSprite.setPsram(false);//UNUSE_PSRAM
  // // _uiSprite.setPsram(USE_PSRAM);
  // // _uiSprite.setColorDepth( COL_DEPTH );
  // // _uiSprite.createSprite( 2*uis[_uiId]->panels[0]->r1, 2*uis[_uiId]->panels[0]->r1 );
  // // _uiSprite.createSprite( 150, 150 );

  // int b_w = int(uis[_uiId]->panels[0]->r0 - uis[_uiId]->panels[0]->r1); // ボタン幅を計算

  // for (int i = 0; i < uis[_uiId]->panels[0]->n; i++)
  // {

  //   int p_btnID = _startId + i; // 事前に計算

  //   touch_btn_list[p_btnID] = NULL;
  //   touch_btn_list[p_btnID] = new TouchBtn();

  //   touch_btn_list[p_btnID]->initOBtn(
  //       p_btnID,
  //       String(p_btnID),
  //       i,
  //       uis[_uiId]->panels[0]->a, // 描画のスタート角（右から時計回り）
  //       uis[_uiId]->panels[0]->x,
  //       uis[_uiId]->panels[0]->y,
  //       uis[_uiId]->panels[0]->r1,
  //       uis[_uiId]->panels[0]->r0,
  //       uis[_uiId]->panels[0]->n, // 分割数
  //       String(p_btnID),
  //       // uiSpritePos,
  //       getTouchPoint(_x, _y),
  //       TOUCH_OBTN_MODE);

  //   touch_btn_list[p_btnID]->setVisibleF(true);
  //   touch_btn_list[p_btnID]->setAvailableF(true);

  //   gBtnID++; // ボタンを更新
  // }
  // uis[_uiId]->panels[0]->b_num = gBtnID - uis[_uiId]->panels[0]->b_sNo; // UIのボタン数をセット
}

void LovyanGFX_DentaroUI::createUIs(int uiContainernum){
// Ui オブジェクトを事前に生成して uis ベクターに追加
        for (int i = 0; i < uiContainernum; ++i) {
            Ui* ui = new Ui();
            uis.push_back(ui);
        }
}

void LovyanGFX_DentaroUI::createPanel(
    int _ui_no, int _btn_mode,
    int _x, int _y,
    int _w, int _h,
    int _row, int _col,
    int _touchZoom, int _eventNo, int _xy_mode, int _uiContainerID, bool _visibleF)
{
    touchZoom = _touchZoom;
    addBtnNum += _row * _col;

    Panel *panel = new Panel(); // 新しい Panel オブジェクトを動的に確保

    panel->btn_mode = _btn_mode;
    panel->ui_no = _ui_no;

    panel->b_sNo = gBtnID;
    panel->id = gPanelID;
    panel->x = _x * _touchZoom;
    panel->y = _y * _touchZoom;
    panel->w = _w * _touchZoom;
    panel->h = _h * _touchZoom;
    panel->row = _row;
    panel->col = _col;
    panel->eventNo = _eventNo;

    int b_w = int(panel->w / _row);
    int b_h = int(panel->h / _col);

    for (int j = 0; j < _col; j++) {
        for (int i = 0; i < _row; i++) {
            int buttonID = gBtnID++; // ボタンIDを生成
            // ボタンの属性を持つ構造体を生成して、リストに追加
            TouchButton button = {
                .id = buttonID,
                .x = panel->x + i * b_w,
                .y = panel->y + j * b_h,
                .width = b_w,
                .height = b_h,
                .visibleF = _visibleF
            };
            panel->buttons.push_back(button);

            // TouchBtn オブジェクトを Panel に追加
            panel->addTouchBtn(button.id, button.x, button.y, button.width, button.height, _btn_mode, _eventNo, _xy_mode);
            panel->b_num++;
        }
    }

    if (panel != nullptr) {
        uis[_uiContainerID]->panels.push_back(panel);
    }

    Serial.println("e");
    gPanelID++;
}


void LovyanGFX_DentaroUI::deletePanels(int _uiContainerID, size_t _PRESET_BTN_NUM) {
  if(_uiContainerID==1){
    if (_uiContainerID < uis.size()) {
        for (auto panel : uis[_uiContainerID]->panels) {
            delete panel;
        }
        uis[_uiContainerID]->panels.clear();
        // panelsベクターも削除する
        uis[_uiContainerID]->panels.shrink_to_fit();
    }
    gBtnID = _PRESET_BTN_NUM;//再設定のため、ボタンIDを初期化
    gPanelID = 1;//再設定のため、UIIDを初期化
  }
}

void LovyanGFX_DentaroUI::setAvailableF(int _uiId, int _btnID, bool _available)
{
  int _startId = uis[_uiId]->panels[0]->b_sNo; // スタート時のボタンIDをセット
  touch_btn_list[_startId + _btnID]->setAvailableF(_available);
}

void LovyanGFX_DentaroUI::setAllBtnAvailableF(int _uiId, bool _available)
{
  int _startId = uis[_uiId]->panels[0]->b_sNo; // スタート時のボタンIDをセット
  for (int i = 0; i < uis[_uiId]->panels[0]->row; i++)
  {
    for (int j = 0; j < uis[_uiId]->panels[0]->col; j++)
    {
      touch_btn_list[_startId + j * uis[_uiId]->panels[0]->row + i]->setAvailableF(_available);
    }
  }
}

void LovyanGFX_DentaroUI::setVisibleF(int _uiId, int _btnID, bool _visible)
{
  int _startId = uis[_uiId]->panels[0]->b_sNo; // スタート時のボタンIDをセット
  touch_btn_list[_startId + _btnID]->setVisibleF(_visible);
}

void LovyanGFX_DentaroUI::setAllBtnVisibleF(int _uiId, bool _visible)
{
  int _startId = uis[_uiId]->panels[0]->b_sNo; // スタート時のボタンIDをセット
  for (int i = 0; i < uis[_uiId]->panels[0]->row; i++)
  {
    for (int j = 0; j < uis[_uiId]->panels[0]->col; j++)
    {
      touch_btn_list[_startId + j * uis[_uiId]->panels[0]->row + i]->setVisibleF(_visible);
    }
  }
}

void LovyanGFX_DentaroUI::setQWERTY(int _uiId, String _btnsString, LGFX_Sprite &_sprite)
{
  int charsNum = _btnsString.length();
  const char *btnChars = _btnsString.c_str();
  int _id = uis[_uiId]->panels[0]->b_sNo;

  for (int i = 0; i < charsNum; i++)
  {
    touch_btn_list[_id + i]->setVisibleF(true);
    // touch_btn_list[_id + i]->setAvailableF(true);
    touch_btn_list[_id + i]->setBtnName(String(btnChars[i]));
    // touch_btn_list[_id + i]->setColor(_bgColIndex);
  }
}

void LovyanGFX_DentaroUI::setBtnName(int _btnNo, String _btnName, String _btnNameFalse)
{
  int _btnId = _btnNo;
  touch_btn_list[_btnId]->setBtnName(_btnName);
  // touch_btn_list[_btnId]->setBtnNameFalse(_btnNameFalse); // トグルのfalse状態の時のラベルを設定
}

void LovyanGFX_DentaroUI::setBtnName(int _uiId, int _panelId, int _btnNo, String _btnName)
{
  int _btnId = _btnNo;
  // panels[_btnId]->setAvailableF(true);
  // panels[_btnId]->setVisibleF(true);
  // panels[_btnId]->setAvailableF(true);
  uis[_uiId]->panels[_panelId]->setBtnName(_btnName);
  // panels[_btnId]->setColor(_bgColIndex);
}

void LovyanGFX_DentaroUI::setupFlickData(int _uiId) {
  uis[_uiId]->panels[1]->touchBtns[0]->namelabel = "Nxt";
  uis[_uiId]->panels[1]->touchBtns[1]->namelabel = "a/A";
  uis[_uiId]->panels[1]->touchBtns[2]->namelabel = "SFT";
  uis[_uiId]->panels[1]->touchBtns[3]->namelabel = "゛小";

  uis[_uiId]->panels[2]->touchBtns[0]->namelabel = "del";
  uis[_uiId]->panels[2]->touchBtns[1]->namelabel = "_";
  uis[_uiId]->panels[2]->touchBtns[2]->namelabel = "Clr";
  uis[_uiId]->panels[2]->touchBtns[3]->namelabel = "Ret";
}

bool LovyanGFX_DentaroUI::readFlickData(int _uiId, int startLine) {

    File fr = SPIFFS.open("/init/param/flickdata.csv", "r");
    if (!fr) {
        Serial.println("Failed to open file");
        return false;
    }

    String line;
    int j = 0;
    int currentLine = 0;

    // 読み込み
    while (fr.available() && j < 12) {
        char c = fr.read();
        if (c == '\n') {
            if (currentLine >= startLine) {
                // 13行目から処理を開始
                line.trim(); // 文字列の前後のスペースを削除
                int commaIndex = line.indexOf(',');
                if (commaIndex != -1) {
                    std::string stdstr = line.substring(commaIndex + 1).c_str();
                    String str = line.substring(0, commaIndex).c_str(); // 先頭文字をラベルにする
                    uis[_uiId]->panels[0]->touchBtns[j]->namelabel = str;
                    j++;
                }
            }
            currentLine++;
            line = ""; // 行をリセット
        } else {
            line += c;
        }
    }
    fr.close();
    return true;
}

String LovyanGFX_DentaroUI::getFlickChar(int _selectbtnID){
  String c = uis[1]->panels[3]->touchBtns[_selectbtnID]->shift_btn_name;
  return c;
}

String LovyanGFX_DentaroUI::getFlickChar(int _PRESET_BTN_NUM, int _uiId, int _btnID){
  // String c = uis[_uiId]->panels[3]->touchBtns[_btnID - 20 - _PRESET_BTN_NUM]->namelabel;
  String c = uis[_uiId]->panels[3]->touchBtns[_btnID - 20 - _PRESET_BTN_NUM]->shift_btn_name;

  return c;
}

String LovyanGFX_DentaroUI::getFlickline(int flickDatalineNo) {
    File fr = SPIFFS.open("/init/param/flickdata.csv", "r");
    if (!fr) {
        Serial.println("Failed to open file");
        return "";
    }

    String line;
    int currentLine = 0;

    // 読み込み
    while (fr.available() && currentLine <= flickDatalineNo) {
        char c = fr.read();
        if (c == '\n') {
            if (currentLine == flickDatalineNo) {
                // flickDatalineNo 番目の行を取得
                line.trim(); // 文字列の前後のスペースを削除
                fr.close();
                return line;
            }
            currentLine++;
            line = ""; // 行をリセット
        } else {
            line += c;
        }
    }
    fr.close();
    return "";
}


void LovyanGFX_DentaroUI::readFlickDataline(int _PRESET_BTN_NUM, int _uiId, int _btnID) {
  int flickDatalineNo = flickShiftNo * 12 + _btnID - _PRESET_BTN_NUM;//プリセットボタンの数を引いて0~12の値にする
  String linedata = getFlickline(flickDatalineNo);
  // スプリットして panels[3]->touchBtns[i]->namelabel に格納
  for (int i = 0; i < 5; i++) {
      int commaIndex = linedata.indexOf(',');
      if (commaIndex != -1) {
          uis[_uiId]->panels[3]->touchBtns[i]->namelabel = linedata.substring(0, commaIndex).c_str();
          linedata = linedata.substring(commaIndex + 1);
      } else {
          // カンマが見つからない場合
          uis[_uiId]->panels[3]->touchBtns[i]->namelabel = linedata;
          break;
      }
  }
}

void LovyanGFX_DentaroUI::flickShift(){
  
  flickShiftNo++;
  flickShiftNo%=5;
  // preflickShiftNo = flickShiftNo;
  kanaShiftNo=0;
}

void LovyanGFX_DentaroUI::kanaShift(size_t _kanaShiftNo){
    kanaShiftNo = _kanaShiftNo;
}
void LovyanGFX_DentaroUI::kanaShift(){
  // if(flickShiftNo != preflickShiftNo)
  // {
  //   kanaShiftNo=0;
  // }else{
    kanaShiftNo++;
    kanaShiftNo %=3;
  // }
}

void LovyanGFX_DentaroUI::drawFlicks(int _uiId, LovyanGFX &_lcd){
  // ファイルからデータを読み込む
  // if(_btn_mode == TOUCH_FLICK_MODE){
    readFlickData(_uiId, flickShiftNo * 12);

    // ボタンの描画などの処理
    // int k=1;//カスタムUIコンテナを指定
      for (int i = 0; i < uis[_uiId]->panels.size(); i++) {
          for (int j = 0; j < uis[_uiId]->panels[i]->b_num; j++) {
              int x = uis[_uiId]->panels[i]->touchBtns[j]->s_x;
              int y = uis[_uiId]->panels[i]->touchBtns[j]->s_y;
              int w = uis[_uiId]->panels[i]->touchBtns[j]->s_w;
              int h = uis[_uiId]->panels[i]->touchBtns[j]->s_h;
              String namelabel = uis[_uiId]->panels[i]->touchBtns[j]->namelabel;
              // uis[_uiId]->panels[i]->touchBtns[j]->btnDraw(_lcd, x, y, w, h, namelabel, uis[_uiId]->panels[i]->touchBtns[j]->btnID, 0, kanaShiftNo);
              uis[_uiId]->panels[i]->panelbtnDraw(_lcd, x, y, w, h, namelabel, uis[_uiId]->panels[i]->touchBtns[j]->btnID, kanaShiftNo);
          }
      }
  // }
}

void LovyanGFX_DentaroUI::drawBtns(int _uiId, LovyanGFX &_lcd)
{
  if (getEvent() != NO_EVENT)
  {
    if (getEvent() == uis[_uiId]->panels[0]->eventNo || uis[_uiId]->panels[0]->eventNo == MULTI_EVENT)
    {
      for (int i = 0; i < uis[_uiId]->panels.size(); i++) {
        for (int j = 0; j < uis[_uiId]->panels[i]->b_num; j++) {
          int x = uis[_uiId]->panels[i]->touchBtns[j]->s_x;
          int y = uis[_uiId]->panels[i]->touchBtns[j]->s_y;
          int w = uis[_uiId]->panels[i]->touchBtns[j]->s_w;
          int h = uis[_uiId]->panels[i]->touchBtns[j]->s_h;

          String namelabel = uis[_uiId]->panels[i]->touchBtns[j]->label;
          uis[_uiId]->panels[i]->touchBtns[j]->btnDraw(_lcd);
          // uis[_uiId]->panels[i]->btnDraw(_lcd, x, y, w, h, namelabel);
          
          
        }
      }
    }
  }
}

void LovyanGFX_DentaroUI::drawSliders(int _uiId, LovyanGFX &_lcd)
{
  if (getEvent() != NO_EVENT)
  {
    if (getEvent() == uis[_uiId]->panels[0]->eventNo || uis[_uiId]->panels[0]->eventNo == MULTI_EVENT)
    {
      for (int i = 0; i < uis[_uiId]->panels.size(); i++) {
        for (int j = 0; j < uis[_uiId]->panels[i]->b_num; j++) {
          int x = uis[_uiId]->panels[i]->touchBtns[j]->s_x;
          int y = uis[_uiId]->panels[i]->touchBtns[j]->s_y;
          int w = uis[_uiId]->panels[i]->touchBtns[j]->s_w;
          int h = uis[_uiId]->panels[i]->touchBtns[j]->s_h;

          String label = uis[_uiId]->panels[i]->touchBtns[j]->label;
          uis[_uiId]->panels[i]->touchBtns[j]->sliderDraw(_lcd);
        }
      }
    }
  }
}

void LovyanGFX_DentaroUI::drawToggles(int _uiId, LovyanGFX &_lcd)
{
  if (getEvent() == TOUCH)
  {
    if (_uiId >= 0)
    { // NULLを除外
      switchToggleVal();
      drawToggles(_uiId, _lcd, uis[_uiId]->panels[_uiId]->x, uis[_uiId]->panels[_uiId]->y);
    }
    else
    {
    }
  }
}

void LovyanGFX_DentaroUI::drawOBtns(int _uiId, LovyanGFX &_lcd, int _x, int _y)
{
  if (getEvent() != NO_EVENT)
  {
    if (getEvent() == uis[_uiId]->panels[0]->eventNo || uis[_uiId]->panels[0]->eventNo == MULTI_EVENT)
    {
      int _id = uis[_uiId]->panels[0]->b_sNo;
      int _btnNum = uis[_uiId]->panels[0]->b_num;
      // _uiSprite.setPivot( 0, 0 );//setPivot()で回転する場合の原点を指定します。初期値は左上の(0, 0)だと思います
      

      for (int i = _id; i < _id + _btnNum; i++)
      {
        uis[_uiId]->panels[i]->setSelectBtnF(false);
        if (selectBtnID == i)
          uis[_uiId]->panels[i]->setSelectBtnF(true);
        else
          uis[_uiId]->panels[i]->setSelectBtnF(false);
          // uis[_uiId]->panels[i]->panelbtnDraw(_lcd, 0, 0, 38, 48,"", gBtnID,0); // ボタンエリアの半径
          // uis[_uiId]->panels[i]->panelbtnDraw(_lcd, 0, 0, 38, 48, "", gBtnID, 0, kanaShiftNo);
          // uis[_uiId]->panels[i]->btnDraw(_uiSprite, uis[_uiId]->panels[0]->x, uis[_uiId]->panels[0]->y); //スプライト経由で描画する
      }
    }

    int _id = uis[_uiId]->panels[0]->b_sNo;

    for (int i = 0; i < uis[_uiId]->panels[0]->n; i++)
    {
      int p_btnID = _id + i; // 事前に計算
      // タッチポイントをUI表示位置にずらす

      // panels[p_btnID]->setOBtnPos(
      //     _x + 180 / 2,
      //     _y + 180 / 2);
    }
  }
}

void LovyanGFX_DentaroUI::drawToggles(int _uiId, LovyanGFX &_lcd, int _x, int _y)
{

  // if(panels[_uiId]->toggle_mode  == true){
  //   drawBtns(_uiId, _lcd, _uiSprite, _x, _y);
  // }
  toggle_mode = true;
  // drawBtns(_uiId, _lcd, _x, _y);
  toggle_mode = false;
}

// void LovyanGFX_DentaroUI::createFlicks(int _x, int _y, int _w, int _h, int _row, int _col, int _eventNo)
// { // 縦方向に並ぶ
//   // panels.push_back(*new Panel);
//   panels_num++;
//   uiId++;
//   uis[uiId]->panels[0]->label = "FLICK_" + String(uiId);
//   Serial.println("FLICK_" + String(uiId) + "=[" + String(uiId) + "]" + String(gBtnID) + "~");

//   int _startId = gBtnID; // スタート時のボタンIDをセット
//   uis[uiId]->panels[0]->b_sNo = gBtnID;
//   uis[uiId]->panels[0]->id = uiId;
//   uis[uiId]->panels[0]->x = _x;
//   uis[uiId]->panels[0]->y = _y;
//   uis[uiId]->panels[0]->w = _w;
//   uis[uiId]->panels[0]->h = _h;
//   uis[uiId]->panels[0]->row = _row;
//   uis[uiId]->panels[0]->col = _col;
//   uis[uiId]->panels[0]->eventNo = _eventNo;
//   uis[uiId]->panels[0]->toggle_mode = false;

//   int b_w = int(uis[uiId]->panels[0]->w / uis[uiId]->panels[0]->row);
//   int b_h = int(uis[uiId]->panels[0]->h / uis[uiId]->panels[0]->col);

//   for (int i = 0; i < uis[uiId]->panels[0]->row; i++)
//   {
//     for (int j = 0; j < uis[uiId]->panels[0]->col; j++)
//     {
//       int p_btnID = _startId + j * uis[uiId]->panels[0]->row + i;

//       touch_btn_list[p_btnID] = NULL;
//       touch_btn_list[p_btnID] = new TouchBtn();
//       touch_btn_list[p_btnID]->initBtn(p_btnID, String(p_btnID),
//                                        i * b_w,
//                                        j * b_h,
//                                        b_w,
//                                        b_h,
//                                        // "あ",//str, //先頭文字をラベルにする
//                                        // // uiSpritePos,
//                                        getTouchPoint(_x, _y),
//                                        TOUCH_FLICK_MODE);

//       // ベータ版は基本true
//       touch_btn_list[p_btnID]->setVisibleF(true);
//       touch_btn_list[p_btnID]->setAvailableF(true);

//       gBtnID++; // ボタンを更新
//     }
//   }
//   uis[uiId]->panels[0]->b_num = gBtnID - uis[uiId]->panels[0]->b_sNo; // UIのボタン数をセット
// }

bool LovyanGFX_DentaroUI::isAvailable(int _btnID)
{
  return touch_btn_list[_btnID]->getAvailableF();
}

void LovyanGFX_DentaroUI::addHandler(int _uiId, int _panelNo, int _btnNo, DelegateBase2 *_func, uint16_t _runEventNo, int _parentID, bool _constantGetF)
{
  runEventNo = _runEventNo;
  _parentID = 0; // ベータ版ではとりあえず強制的にLCDのみのイベントをとる

  int _btnID = uis[_uiId]->panels[_panelNo]->touchBtns[_btnNo]->getBtnID();

  uis[_uiId]->panels[_panelNo]->touchBtns[_btnNo]->addHandler(_func);

  if (!_constantGetF)
  {
    uis[_uiId]->panels[_panelNo]->touchBtns[_btnNo]->run2(_btnID, _btnNo, sp, tp, eventState, runEventNo);
    //      if( parentID == PARENT_LCD )    panels[_panelNo]->touchBtns[_btnNo]->run2( _btnID, _btnNo, sp, tp, eventState, runEventNo);//ボタン内のイベントかチェック
    // else if( parentID == PARENT_SPRITE ) panels[_panelNo]->touchBtns[_btnNo]->run2( _btnID, _btnNo, sp, tp, eventState, runEventNo);//ボタン内のイベントかチェック
  }
  else if (_constantGetF)
  { // spにtpを入れて、常時取得モードにする
    uis[_uiId]->panels[_panelNo]->touchBtns[_btnNo]->run2(_btnID, _btnNo, tp, tp, eventState, runEventNo);
    //      if( parentID == PARENT_LCD )    panels[_panelNo]->touchBtns[_btnNo]->run2( _btnID, _btnNo, tp, tp, eventState, runEventNo);//ボタン内のイベントかチェック
    // else if( parentID == PARENT_SPRITE ) panels[_panelNo]->touchBtns[_btnNo]->run2( _btnID, _btnNo, tp, tp, eventState, runEventNo);//ボタン内のイベントかチェック
  }

  uis[_uiId]->panels[_panelNo]->touchBtns[_btnNo]->delHandlers2();
}

float LovyanGFX_DentaroUI::getAngle(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b)
{

  float r = atan2(b.y - a.y, b.x - a.x);
  if (r < 0)
  {
    r = r + 2 * M_PI;
  }
  return r * 360 / (2 * M_PI);
}

float LovyanGFX_DentaroUI::getDist(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b)
{
  float d = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
  return d;
}

int LovyanGFX_DentaroUI::getPreEvent()
{
  return preEventState;
}

void LovyanGFX_DentaroUI::circle(LovyanGFX *_lcd, uint16_t c, int fillF)
{
  if (fillF)
  {
    _lcd->fillCircle(tp.x, tp.y, 10, c);
    _lcd->fillCircle(tp.x, tp.y, 10, c);
  }
  else
  {
    _lcd->drawCircle(tp.x, tp.y, 10, c);
    _lcd->drawCircle(tp.x, tp.y, 10, c);
  }
}

void LovyanGFX_DentaroUI::rect(LovyanGFX *_lcd, uint16_t c, int fillF)
{
  if (fillF)
  {
    _lcd->fillRect(tp.x, tp.y, 10, 10, c);
    _lcd->fillRect(tp.x, tp.y, 10, 10, c);
  }
  else
  {
    _lcd->drawRect(tp.x, tp.y, 10, 10, c);
    _lcd->drawRect(tp.x, tp.y, 10, 10, c);
  }
}

lgfx::v1::touch_point_t LovyanGFX_DentaroUI::getPos()
{
  return tp;
}

lgfx::v1::touch_point_t LovyanGFX_DentaroUI::getStartPos()
{
  return sp;
}

void LovyanGFX_DentaroUI::setPos(int _x, int _y)
{
  tp.x = _x;
  tp.y = _y;
}

void LovyanGFX_DentaroUI::setStartPos(int _x, int _y)
{
  sp.x = _x;
  sp.y = _y;
}

int LovyanGFX_DentaroUI::getTouchBtnID()
{
  return obj_ret.btnID;
}

int LovyanGFX_DentaroUI::getTouchBtnNo()
{
  return getTouchBtnID() - getUiFirstNo(FlickUiID);
}

int LovyanGFX_DentaroUI::getEvent()
{
  return eventState;
}

int LovyanGFX_DentaroUI::getFlickEvent()
{
  return flickState;
}

lgfx::v1::touch_point_t LovyanGFX_DentaroUI::getTouchPoint(int _x, int _y)
{
  lgfx::v1::touch_point_t tp;
  tp.x = _x;
  tp.y = _y;
  return tp;
}

float LovyanGFX_DentaroUI::getSliderVal(int _uiId, int _panel_no, int _btnNo)
{
  float retVal = 1.0;
  int _btnID = uis[_uiId]->panels[_panel_no]->b_sNo + _btnNo;
  if (touch_btn_list[_btnID]->get_xy_mode() == X_VAL)
  {
    retVal = touch_btn_list[_btnID]->getSliderValx();
  }
  else if (touch_btn_list[_btnID]->get_xy_mode() == Y_VAL)
  {
    retVal = touch_btn_list[_btnID]->getSliderValy();
  }
  return retVal;
}

float LovyanGFX_DentaroUI::getSliderVal(int _uiId, int _panel_no, int _btnNo, int _xy)
{
  float retVal = 1.0;
  int _btnID = uis[_uiId]->panels[_panel_no]->b_sNo + _btnNo;
  if (_xy == X_VAL)
  {
    retVal = uis[_uiId]->panels[_panel_no]->touchBtns[_btnNo]->getSliderValx();
  }
  else if (_xy == Y_VAL)
  {
    retVal = uis[_uiId]->panels[_panel_no]->touchBtns[_btnNo]->getSliderValy();
  }
  return retVal;
}

Vec2 LovyanGFX_DentaroUI::getSliderVec2(int _uiId, int _btnNo)
{ // XYを一気に取得
  Vec2 retPos;
  int _btnID = uis[_uiId]->panels[0]->b_sNo + _btnNo;
  if (uis[_uiId]->panels[0]->touchBtns[_btnNo]->get_xy_mode() == XY_VAL)
  {
    retPos.x = uis[_uiId]->panels[0]->touchBtns[_btnNo]->getSliderValx();
    retPos.y = uis[_uiId]->panels[0]->touchBtns[_btnNo]->getSliderValy();
  }
  return retPos;
}

void LovyanGFX_DentaroUI::setSliderVal(int _uiId,int _panel_no, int _btnNo, float _x, float _y)
{
  int _btnID = uis[_uiId]->panels[0]->b_sNo + _btnNo;
  touch_btn_list[_btnID]->setSliderVal(_x, _y);
}

void LovyanGFX_DentaroUI::setCharMode(int _charMode)
{
  charMode = _charMode;
}

int LovyanGFX_DentaroUI::getCharMode()
{
  return charMode;
}

bool LovyanGFX_DentaroUI::getToggleVal(int _uiId, int _btnNo)
{
  int _btnID = uis[_uiId]->panels[0]->b_sNo + _btnNo;
  // touch_btn_list[_btnID]->switchToggleVal();//値をスイッチしておいて
  return touch_btn_list[_btnID]->getToggleVal(); // スイッチした値をリターンする
}

int LovyanGFX_DentaroUI::getUiFirstNo(int _uiId)
{
  return uis[_uiId]->panels[0]->b_sNo;
}

int LovyanGFX_DentaroUI::getflickPanelBtnNo(int _uiId)
{
  return flickPanels[_uiId]->b_No;
}

int LovyanGFX_DentaroUI::getUiBtnNum(int _uiId)
{
  return uis[_uiId]->panels[0]->b_num;
}

int LovyanGFX_DentaroUI::getUiBoxNum()
{
  //  return panels.size();
  return panels_num;
}

int LovyanGFX_DentaroUI::getAllBtnNum()
{
  return gBtnID;
}

int LovyanGFX_DentaroUI::getParentID()
{
  return parentID;
}

int LovyanGFX_DentaroUI::getUiID(const char *_uiLabel)
{
  for (int k = 0; k < uis.size(); k++){
    for (int i = 0; i < uis[k]->panels.size(); i++)
    { // ボタンの数分繰り返す
      if (uis[k]->panels[i]->btn_name != String(_uiLabel))
      {
        return i;
      }
    }
  }
  return -1;
}

void LovyanGFX_DentaroUI::setBtnID(int _uiId, int _panelId, int _btnID)
{
  obj_ret.setBtnID_ret(_btnID);
}

lgfx::v1::touch_point_t LovyanGFX_DentaroUI::getBtnPos(int _btnID)
{
  return getTouchPoint(touch_btn_list[_btnID]->getBtnPos().x, touch_btn_list[_btnID]->getBtnPos().y);
}

void LovyanGFX_DentaroUI::setUiLabels(int _uiId, int _showFlickPanelNo)
{
  showFlickPanelNo = _showFlickPanelNo;

  for (int i = 0; i < uis[_uiId]->panels[0]->row; i++)
  {
    for (int j = 0; j < uis[_uiId]->panels[0]->col; j++)
    {
      int p_btnID = uis[_uiId]->panels[0]->b_sNo + j * uis[_uiId]->panels[0]->row + i;
      //     std::string stdstr = flickPanels[showFlickPanelNo]->text_list[j * uis[_uiId]->panels[0]->row + i].c_str();
      //     String str = stdstr.substr(0, 3).c_str(); // 先頭文字をラベルにする
      //     panels[p_btnID]->setBtnName(str);
      // panels[p_btnID]->setBtnName("め");
    }
  }
}

void LovyanGFX_DentaroUI::showInfo(LovyanGFX &_lcd, int _infox, int _infoy)
{
  // フレームレート情報などを表示します。
  _lcd.setTextSize(1);
  _lcd.setFont(&lgfxJapanGothicP_8);
  _lcd.fillRect(0, 0, 128, 10, TFT_BLACK);
  _lcd.setTextColor(TFT_WHITE);
  _lcd.setCursor(1, 1);
  _lcd.print(fps);
  _lcd.print(":");
  _lcd.print(String(getEvent()));
  _lcd.print("[");
  _lcd.print(String(getPreEvent()));
  _lcd.print("]:BTN");
  _lcd.print("[");
  _lcd.print(String(getFlickEvent()));
  _lcd.print("]:BTN");
  _lcd.print(String(getTouchBtnID()));
  _lcd.print(":");
  // _lcd.println( String( getToggleVal(1,0) ) );
  ++frame_count;
  sec = millis() / 1000;
  if (psec != sec)
  {
    psec = sec;
    fps = frame_count;
    frame_count = 0;
    _lcd.setAddrWindow(0, 0, _lcd.width(), _lcd.height());
  }
}

// String LovyanGFX_DentaroUI::getHenkanChar(int _henkanListNo, int _kanaShiftNo)
// {
//   return kanalist[_henkanListNo][_kanaShiftNo];
// }

void LovyanGFX_DentaroUI::setFlick(int _uiId, int _panelId, int _FlickUiID, int _LeftBtnUiID, int _RightBtnUiID)
{
  setFlick(_uiId, _panelId, CHAR_3_BYTE, _FlickUiID, _LeftBtnUiID, _RightBtnUiID);
}

void LovyanGFX_DentaroUI::setFlick(int _uiId, int _panelId, int _charMode, int _FlickUiID, int _LeftBtnUiID, int _RightBtnUiID)
{

  FlickUiID = _FlickUiID;
  LeftBtnUiID = _LeftBtnUiID;
  RightBtnUiID = _RightBtnUiID;

  setBtnName(_uiId,_panelId, uis[_uiId]->panels[LeftBtnUiID]->b_sNo, "Nxt");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[LeftBtnUiID]->b_sNo + 1, "a/A");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[LeftBtnUiID]->b_sNo + 2, "SFT");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[LeftBtnUiID]->b_sNo + 3, "゛小");

  setBtnName(_uiId,_panelId, uis[_uiId]->panels[RightBtnUiID]->b_sNo, "del");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[RightBtnUiID]->b_sNo + 1, "_");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[RightBtnUiID]->b_sNo + 2, "Clr");
  setBtnName(_uiId,_panelId, uis[_uiId]->panels[RightBtnUiID]->b_sNo + 3, "Ret");

  if (_charMode == CHAR_1_BYTE)
  {
    fpNo = 2; // 英語モードに切り替える
  }
  else if (_charMode == NUMERIC)
  {
    fpNo = 4; // 数値モードに切り替える
  }
  else
  {
    setCharMode(_charMode); // 指定のパネルモードに切り替える
  }
  setUiLabels(FlickUiID, fpNo);
}

String LovyanGFX_DentaroUI::getInvisibleFlickStrings()
{
  String invisibleStr = "";
  // 最後の文字のバイト数を判定する
  setlocale(LC_ALL, "");
  std::vector<std::string> ret = split_mb(flickStrDel.c_str(), "\n");

  if (ret.size() >= 1)
  {

    for (size_t i = 0; i < ret.size() - 1; i++)
    {
      invisibleStr += "*";
    }

    invisibleStr += String(ret[ret.size() - 1].c_str());
  }

  return invisibleStr;
}

String LovyanGFX_DentaroUI::getFlickString(bool _visibleMode)
{
  if (_visibleMode == INVISIBLE)
    return getInvisibleFlickStrings();
  else
    return flickStr;
}

String LovyanGFX_DentaroUI::getFlickString()
{
  return flickStr;
}

String LovyanGFX_DentaroUI::getFlickline()
{

  String RetChar = "";
  if (!selectModeF)
  {
    RetChar = preFlickChar;
  }
  else if (selectModeF)
  {
    RetChar = previewFlickChar;
  }
  return RetChar;
}

String LovyanGFX_DentaroUI::delEndChar(String _str, int _ByteNum)
{
  std::string stdFlickStr = _str.c_str();
  for (int i = 0; i < _ByteNum; i++)
  {
    stdFlickStr.erase(--stdFlickStr.end());
  }
  return stdFlickStr.c_str();
}

void LovyanGFX_DentaroUI::switchToggleVal()
{
  if (getTouchBtnID() >= 0)
  { // NULLを除外
    touch_btn_list[getTouchBtnID()]->switchToggleVal();
  }
  else
  {
  }
}

//---MAP用関数

void LovyanGFX_DentaroUI::nowLoc(LovyanGFX &_lcd)
{
  _lcd.fillTriangle(110, 106, 130, 106, 120, 120, TFT_RED);
  _lcd.fillCircle(120, 120 - 18, 10, TFT_RED);
  _lcd.fillCircle(120, 120 - 18, 6, TFT_WHITE);
}

int LovyanGFX_DentaroUI::getPositionNo(int _addXTileNo, int _addYTileNo) // スプライトに高速描画
{
  int _posNo = 0;
  if (_addXTileNo == 0 && _addYTileNo == 0)
  {
    _posNo = 0;
  }
  else if (_addXTileNo == 1 && _addYTileNo == 0)
  {
    _posNo = 1;
  }
  else if (_addXTileNo == 0 && _addYTileNo == 1)
  {
    _posNo = 2;
  }
  else if (_addXTileNo == -1 && _addYTileNo == 0)
  {
    _posNo = 3;
  }
  else if (_addXTileNo == 0 && _addYTileNo == -1)
  {
    _posNo = 4;
  }
  else if (_addXTileNo == 1 && _addYTileNo == -1)
  {
    _posNo = 5;
  }
  else if (_addXTileNo == 1 && _addYTileNo == 1)
  {
    _posNo = 6;
  }
  else if (_addXTileNo == -1 && _addYTileNo == 1)
  {
    _posNo = 7;
  }
  else if (_addXTileNo == -1 && _addYTileNo == -1)
  {
    _posNo = 8;
  }
  return _posNo;
}

int LovyanGFX_DentaroUI::getAddX(int _spriteNo)
{
  int addX = MapTiles[_spriteNo]->getAddX();
  return addX;
}

int LovyanGFX_DentaroUI::getAddY(int _spriteNo)
{
  int addY = MapTiles[_spriteNo]->getAddY();
  return addY;
}

void LovyanGFX_DentaroUI::setAddX(int _objId, int _xtileNo)
{
  MapTiles[_objId]->setAddX(_xtileNo);
}

void LovyanGFX_DentaroUI::setAddY(int _objId, int _ytileNo)
{
  MapTiles[_objId]->setAddY(_ytileNo);
}

//---------

int LovyanGFX_DentaroUI::getPreAddX(int _spriteNo)
{
  int addX = MapTiles[_spriteNo]->getPreAddX();
  return addX;
}

int LovyanGFX_DentaroUI::getPreAddY(int _spriteNo)
{
  int addY = MapTiles[_spriteNo]->getPreAddY();
  return addY;
}

void LovyanGFX_DentaroUI::setPreAddX(int _objId, int _xtileNo)
{
  MapTiles[_objId]->setPreAddX(_xtileNo);
}

void LovyanGFX_DentaroUI::setPreAddY(int _objId, int _ytileNo)
{
  MapTiles[_objId]->setPreAddY(_ytileNo);
}

//---------

int LovyanGFX_DentaroUI::get_gPosId()
{
  return gPosId;
}

void LovyanGFX_DentaroUI::set_gPosId(int _gPosId)
{
  gPosId = _gPosId;
}

//---------

void LovyanGFX_DentaroUI::setDrawFinishF(int _objId, bool _drawFinishF)
{
  touch_btn_list[_objId]->setDrawFinishF(_drawFinishF);
}

bool LovyanGFX_DentaroUI::getDrawFinishF(int _objId)
{
  return touch_btn_list[_objId]->getDrawFinishF();
}

//---------

void LovyanGFX_DentaroUI::setXtileNo(int _objId, int _xtileNo)
{
  MapTiles[_objId]->setXtileNo(_xtileNo);
}

void LovyanGFX_DentaroUI::setPreXtileNo(int _objId, int _preXtileNo)
{
  MapTiles[_objId]->setPreXtileNo(_preXtileNo);
}

void LovyanGFX_DentaroUI::setYtileNo(int _objId, int _ytileNo)
{
  MapTiles[_objId]->setYtileNo(_ytileNo);
}

void LovyanGFX_DentaroUI::setPreYtileNo(int _objId, int _preYtileNo)
{
  MapTiles[_objId]->setPreYtileNo(_preYtileNo);
}

void LovyanGFX_DentaroUI::setTileNo(int _objId, int _xtileNo, int _ytileNo)
{
  MapTiles[_objId]->setXtileNo(_xtileNo);
  MapTiles[_objId]->setYtileNo(_ytileNo);
}

void LovyanGFX_DentaroUI::setXtilePos(int _objId, int _xtilePos)
{
  MapTiles[_objId]->setXtilePos(_xtilePos);
}

void LovyanGFX_DentaroUI::setYtilePos(int _objId, int _ytilePos)
{
  MapTiles[_objId]->setYtilePos(_ytilePos);
}

int LovyanGFX_DentaroUI::getXtile()
{
  return xtile;
}

int LovyanGFX_DentaroUI::getYtile()
{
  return ytile;
}

int LovyanGFX_DentaroUI::getZtile()
{
  return ztile;
}

int LovyanGFX_DentaroUI::getXtileNo()
{
  return xtileNo;
}

int LovyanGFX_DentaroUI::getYtileNo()
{
  return ytileNo;
}

int LovyanGFX_DentaroUI::getXtileNo(int _objNo)
{
  return MapTiles[_objNo]->getXtileNo();
}

int LovyanGFX_DentaroUI::getYtileNo(int _objNo)
{
  return MapTiles[_objNo]->getYtileNo();
}

int LovyanGFX_DentaroUI::getPreXtileNo(int _objNo)
{
  return MapTiles[_objNo]->getPreXtileNo();
}

int LovyanGFX_DentaroUI::getPreYtileNo(int _objNo)
{
  return MapTiles[_objNo]->getPreYtileNo();
}

int LovyanGFX_DentaroUI::getPreXtileNo()
{
  return preXtileNo;
}

int LovyanGFX_DentaroUI::getPreYtileNo()
{
  return preYtileNo;
}

int LovyanGFX_DentaroUI::getPreXtile()
{
  return preXtile;
}

int LovyanGFX_DentaroUI::getPreYtile()
{
  return preYtile;
}

int LovyanGFX_DentaroUI::getPreDirID()
{
  return preDirID;
}

int LovyanGFX_DentaroUI::getVx()
{
  return vx;
}

int LovyanGFX_DentaroUI::getVy()
{
  return vy;
}

void LovyanGFX_DentaroUI::setExistF(int _objId, bool _existF)
{
  MapTiles[_objId]->setExistF(_existF);
}

bool LovyanGFX_DentaroUI::getExistF(int _objId)
{
  return MapTiles[_objId]->getExistF();
}

void LovyanGFX_DentaroUI::setPreDirID(int _dirID)
{
  preDirID = _dirID;
}

int LovyanGFX_DentaroUI::getDirID()
{

  vx = getXtile() - getPreXtile();
  vy = getYtile() - getPreYtile();

  float r = atan2(vy, vx);
  if (r < 0)
  {
    r = r + 2 * M_PI;
  }
  float vecAngle = r * 360 / (2 * M_PI);

  // preDirID = dirID;

  if (vecAngle < 15 || vecAngle >= 345)
  {
    dirID = 1;
  }
  else if (vecAngle >= 15 && vecAngle < 75)
  {
    dirID = 2;
  }
  else if (vecAngle >= 75 && vecAngle < 105)
  {
    dirID = 3;
  }
  else if (vecAngle >= 105 && vecAngle < 165)
  {
    dirID = 4;
  }
  else if (vecAngle >= 165 && vecAngle < 195)
  {
    dirID = 5;
  }
  else if (vecAngle >= 195 && vecAngle < 255)
  {
    dirID = 6;
  }
  else if (vecAngle >= 255 && vecAngle < 285)
  {
    dirID = 7;
  }
  else if (vecAngle >= 285 && vecAngle < 345)
  {
    dirID = 8;
  }

  return dirID;
}

bool LovyanGFX_DentaroUI::getDownloadF()
{
  return DownloadF;
}

void LovyanGFX_DentaroUI::setDownloadF(bool _b)
{
  DownloadF = _b;
}

void LovyanGFX_DentaroUI::setPngTile(fs::FS &fs, String _m_url, int _spriteNo)
{
  MapTiles[_spriteNo]->getSpritePtr()->drawPngFile(fs, _m_url,
                                                   0, 0,
                                                   256, 256,
                                                   0, 0, 1.0, 1.0,
                                                   datum_t::top_left);
}

void LovyanGFX_DentaroUI::task2_setPngTile(int _posId)
{
  ROI_m_url = "/tokyo/" + String(getZtile()) + "/" + String(getXtileNo(_posId)) + "/" + String(getYtileNo(_posId)) + ".png";
  setPngTile(SD, ROI_m_url, _posId); // SDからの地図の読み込み
}

// void LovyanGFX_DentaroUI::drawMaps(LGFX& _lcd, double _walkLatPos, double _walkLonPos, int _tileZoom){

//   for(int objId = 0; objId < BUF_PNG_NUM; objId++)
//     {
//       setPreXtileNo(objId, getXtileNo(objId));//過去の名前（位置）を保存
//       setPreYtileNo(objId, getYtileNo(objId));//過去の名前（位置）を保存
//     }

//   getTilePos(_walkLatPos, _walkLonPos, _tileZoom);//経緯度からタイル座標を計算

//    //9枚のマップ座標取得
//    for(int objId = 0; objId < BUF_PNG_NUM; objId++){
//      setAddX(objId, addTPosList[objId][0]);//相対位置情報を登録
//      setAddY(objId, addTPosList[objId][1]);//相対位置情報を登録
//      setXtileNo(objId, getXtileNo() + getAddX(objId));//名前（位置）を登録
//      setYtileNo(objId, getYtileNo() + getAddY(objId));//名前（位置）を登録

//      matrix_list[objId][2] = 120 + (getAddX(objId)*255) - getXtile()%255;
//      matrix_list[objId][5] = 120 + (getAddY(objId)*255) - getYtile()%255;
//      setXtilePos(objId,  matrix_list[objId][2]);
//      setYtilePos(objId,  matrix_list[objId][5]);
//    }

//    if(getXtileNo() != getPreXtileNo()||getYtileNo() != getPreYtileNo()){
//      mataidaF = true;
//      //Serial.println("mataida!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//    }else{
//      mataidaF = false;
//    }

//    for(int objId = 0; objId < BUF_PNG_NUM; objId++)
//    {
//      //ディスプレイ内にはいっていて
//      //まだ読み込んでいない場合
//      setExistF(objId, false);
//        for(int i=0; i < 9; i++){
//          if(getXtileNo(objId) == preReadXtileNo[i] && getYtileNo(objId) == preReadYtileNo[i])
//          {

//            setExistF(objId, true);
//            break;
//          }else{

//            setExistF(objId, false);
//          }
//        }

//        if(getExistF(objId)==false)
//        {

//          preReadXtileNo[objId] = -1;
//          preReadYtileNo[objId] = -1;

//        }
//      }

// //---------------------------------------

//    if(!mataidaF){
//      //地図をまたいでいない時の処理
//      //描画
//      for(int objId = 0; objId < BUF_PNG_NUM; objId++)
//      {
//         for(int j = 0; j<6; j++ )matrix[j] = matrix_list[objId][j];//位置差分番号からスプライトを置く位置番号を特定し、実際の位置を登録
//         layoutSprite_list[objId].pushAffine( &_lcd, matrix );//0~9までのスプライトを差分位置に描画
//      }
//    }

// //---------------------------------------

//    for(int objId = 0; objId < BUF_PNG_NUM; objId++)
//    {
//          if(mataidaF){
//            //読み込む
//            set_gPosId(objId);

//            //上書き可能リストに、データ読み込み
//            //マップID取得

//            setDownloadF(true);//trueにするだけでタスク２が１回作動する

//            while(getDownloadF() == true)
//            {
//            //読み込み完了を待つだけ
//              delay(1);
//            }

//           //  drawTileAuto(
//           //    objId + BUF_PNG_NUM,
//           //    _lcd,
//           //    layoutSprite_list[objId],
//           //    TFT_ORANGE,
//           //    objId);

//         //  Serial.print(objId);
//         Serial.print("[");
//         Serial.print(getXtileNo());
//         Serial.print(":");
//         Serial.print(getYtileNo());
//         Serial.print("]");
//         Serial.println("");

//          preReadXtileNo[objId] = getXtileNo(objId);
//          preReadYtileNo[objId] = getYtileNo(objId);
//          setExistF(objId, true);
//          }
//   }

// }

// 経緯度からタイル座標を求める
void LovyanGFX_DentaroUI::getTilePos(double _lat, double _lon, int zoom_level)
{
  // setLatLonPos(lat, lon);
  //  _lat = lat;
  //  _lon = lon;

  // //座標を含むタイル番号を計算
  // double lat_rad = _lat * (M_PI/180);
  // double n = _lcdpow(2, zoom_level);
  // xtileNo = int((_lon + 180.0) / 360.0 * n);
  // ytileNo = int((1.0 - log(tan(lat_rad) + (1 / cos(lat_rad))) / M_PI) / 2.0 * n);
  // ztile = zoom_level;

  // 緯度経度からタイル座標に変換
  double x = (_lon / 180 + 1) * pow(2, zoom_level) / 2;
  xtile = int(x * 255);
  double y = ((-log(tan((45 + _lat / 2) * M_PI / 180)) + M_PI) * pow(2, zoom_level) / (2 * M_PI));
  ytile = int(y * 255);
  ztile = zoom_level;

  // //座標を含むタイル番号を計算
  // x = (_lon / 180 + 1) * pow(2, zoom_level-1) / 2;
  // xtileNo = int(x);
  // y = ((-log(tan((45 + _lat / 2) * M_PI / 180)) + M_PI) * pow(2, zoom_level-1) / (2 * M_PI));
  // ytileNo = int(y);

  // //現在地を含むタイル番号を計算
  double lat_rad = _lat * (M_PI / 180);
  double n = pow(2, zoom_level);
  xtileNo = int((_lon + 180.0) / 360.0 * n);
  ytileNo = int((1.0 - log(tan(lat_rad) + (1 / cos(lat_rad))) / M_PI) / 2.0 * n);
}

void LovyanGFX_DentaroUI::updateOBtnSlider(int _uiId, int _x, int _y)
{
  uis[_uiId]->panels[0]->c_d = sqrt((pow(tp.x - (_x + uis[_uiId]->panels[0]->r1), 2.0) + pow(tp.y - (_y + uis[_uiId]->panels[0]->r1), 2.0)));
  if (uis[_uiId]->panels[0]->c_d > uis[_uiId]->panels[0]->r0 && uis[_uiId]->panels[0]->c_d < uis[_uiId]->panels[0]->r1)
  {
    if (getEvent() == MOVE)
    {

      uis[_uiId]->panels[0]->c_a = int((atan2(tp.y - (_y + uis[_uiId]->panels[0]->r1),
                                     tp.x - (_x + uis[_uiId]->panels[0]->r1)) *
                               180 / M_PI)); // 現在のボタン中心の角度を登録

      if (uis[_uiId]->panels[0]->c_a < 0)
        uis[_uiId]->panels[0]->c_a = 180 + (180 + uis[_uiId]->panels[0]->c_a); // 0~360の値に収める

      // if( abs(uis[_uiId]->panels[0]->c_a - uis[_uiId]->panels[0]->p_a) > 0 ){

      uis[_uiId]->panels[0]->diff_a = uis[_uiId]->panels[0]->c_a - uis[_uiId]->panels[0]->p_a; // 差分を計算する

      // Serial.println(uis[_uiId]->panels[0]->diff_a);

      // if( getEvent() == TOUCH ){
      // uis[_uiId]->panels[0]->p_a = uis[_uiId]->panels[0]->c_a;//タッチした時のボタン中心の角度を登録しておく
      // }

      if (uis[_uiId]->panels[0]->diff_a > 180)
      {
        uis[_uiId]->panels[0]->diff_a -= 360;
      }
      else if (uis[_uiId]->panels[0]->diff_a < -180)
      {
        uis[_uiId]->panels[0]->diff_a += 360;
      }

      // Serial.println(uis[_uiId]->panels[0]->diff_a);

      uis[_uiId]->panels[0]->p_a = uis[_uiId]->panels[0]->c_a;
      // Serial.print(uis[_uiId]->panels[0]->p_a);
      // Serial.print(":");
    }
    else if (getEvent() == RELEASE)
    {
      uis[_uiId]->panels[0]->diff_a = 0;
    }
  }
}

int LovyanGFX_DentaroUI::getCurrentAngle(int _uiId)
{
  return uis[_uiId]->panels[0]->c_a;
}

int LovyanGFX_DentaroUI::getOBtnDiffAngle(int _uiId)
{
  return uis[_uiId]->panels[0]->diff_a;
}

void LovyanGFX_DentaroUI::setEventBit(int bitNo, bool inputBit)
{

  if (inputBit == 1)
  {
    bitSet(eventBits, bitNo - 1); // 右０ビット目を１（Set）に
  }
  else if (inputBit == 0)
  {
    bitClear(eventBits, bitNo); // 右０ビット目を0（Clear）に
  }
}

bool LovyanGFX_DentaroUI::getEventBit(int bitNo)
{
  uint32_t b = eventBits; // コピー
  // uint32_t mb = 0b1;//マスク用
  bool rb = 0b1 & (b >> bitNo - 1); // ビットシフトして、マスクで１ビット目を抽出してboolにキャスト
  return rb;
}

void LovyanGFX_DentaroUI::resetEventBits()
{
  eventBits = 0b00000000000000000000000000000000;
}

void LovyanGFX_DentaroUI::clearAddBtns()
{
  for (int k = 0; k < uis.size(); k++){
  int count = uis[k]->panels.size();
  }
  int count2 = touch_btn_list.size();
}


// 閾値の数
const int numThresholds = 8;

// 方向を示すビット列
int directionBits = -1;
//int predirectionBits = -1;

// 閾値のリスト
float statebtn_vals[9] = {
  //4070, 3910, 3760, 3530, 3290, 3030, 2940, 2650, 0
  3230, 2600, 2200, 1740, 1400, 1135, 1050, 810, 0
  // 0,0,0,0,0,0,0,0,0
};

void LovyanGFX_DentaroUI::setBtnVals(int n, float val){
  statebtn_vals[n] = val;
}
bool LovyanGFX_DentaroUI::saveParametersToFile(int newX, int newY) {
    File fr = SPIFFS.open("/init/param/phbtninfo.txt", "w");
    if (!fr) {
      Serial.println("Failed to open file for writing:/init/param/phbtninfo.txt");
      return false;
    }

    fr.print(newX);
    fr.print('\n');
    fr.print(newY);
    fr.close();
    return true;
  }


bool LovyanGFX_DentaroUI::loadParametersFromFile() {
    File fr = SPIFFS.open("/init/param/phbtninfo.txt", "r");
    if (!fr) {
      Serial.println("Failed to open file:/init/param/phbtninfo.txt");
      return false;
    }

    // 1行目: x_btncenter
    if (fr.available()) {
      String line = fr.readStringUntil('\n');
      x_btncenter = line.toInt();
    } else {
      Serial.println("Failed to read x_btncenter");
      fr.close();
      return false;
    }

    // 2行目: y_btncenter
    if (fr.available()) {
      String line = fr.readStringUntil('\n');
      y_btncenter = line.toInt();
    } else {
      Serial.println("Failed to read y_btncenter");
      fr.close();
      return false;
    }

    fr.close();
    return true;
  }


bool LovyanGFX_DentaroUI::saveParametersToFile2(){
    File fr = SPIFFS.open("/init/param/phbtnthre.txt", "w");
    if (!fr) {
      Serial.println("Failed to open file for writing:/init/param/phbtnthre.txt");
      return false;
    }
    fr.print(statebtn_vals[0]);
    fr.print('\n');
    fr.print(statebtn_vals[1]);
    fr.print('\n');
    fr.print(statebtn_vals[2]);
    fr.print('\n');
    fr.print(statebtn_vals[3]);
    fr.print('\n');
    fr.print(statebtn_vals[4]);
    fr.print('\n');
    fr.print(statebtn_vals[5]);
    fr.print('\n');
    fr.print(statebtn_vals[6]);
    fr.print('\n');
    fr.print(statebtn_vals[7]);
    fr.print('\n');
    fr.print(statebtn_vals[8]);
    fr.close();
    return true;
}

bool LovyanGFX_DentaroUI::loadParametersFromFile2() {
    File fr = SPIFFS.open("/init/param/phbtnthre.txt", "r");
    if (!fr) {
      Serial.println("Failed to open file:/init/param/phbtnthre.txt");
      return false;
    }

    // 9行まで読み込む
    String line;
    int i = 0;
    while (fr.available() && i < 9) {
      line = fr.readStringUntil('\n');
      if (!line.isEmpty()) {
        statebtn_vals[i] = line.toInt();
        i++;
      }
    }

    fr.close();
    return true;
  }


int LovyanGFX_DentaroUI::getPhVolDir(int n)
{
  // 4スイッチの値
  float volume = getPhVol(n);

  directionBits = -1;  // デフォルト値として無効な値を設定
  //    Serial.println(volume);

  //閾値を元に方向を判断する
  for (int i = 0; i < 8; i++) {  // statebtn_valsの範囲内でループ
    float threshold = (statebtn_vals[i] + statebtn_vals[i + 1]) / 2;
    if (volume >= threshold) {
      directionBits = i;  // 該当する方向を示すビット列をセット
      break;  // 最初の該当する閾値を見つけたらループを終了
    }
  }

  if (volume > 4090 && volume <= 4095) {
    directionBits = -1;
  }

  //+key

  // 6|5|7
  //|0   3|
  // 2|1|4

  //十字キーの出力値を揃える
  if (directionBits == 0) {
    directionBits = 7;
  }
  else if (directionBits == 2) {
    directionBits = 0;
  }
  else if (directionBits == 4) {
    directionBits = 2;
  }
  else if (directionBits == 7) {
    directionBits = 4;
  }

  // 6|5|4
  //|7   3|
  // 0|1|2

  //    if(predirectionBits==directionBits)//２回連続した時だけ前回の値を出力
  //    {
  //      predirectionBits = directionBits;//１回だけの時は前の値を出力
  //      return directionBits;//前の値を出力
  //    }
  //    else
  //    {
  //      predirectionBits = directionBits;
  //      return predirectionBits;
  //    }
  return directionBits;//前の値を出力
}

int LovyanGFX_DentaroUI::getPhVolVec(int n1, int n2) {
  // 音声ボリュームの値
  float x = getPhVol(n1);
  float y = getPhVol(n2);

  // Serial.print(x);
  // Serial.print(":");
  // Serial.print(y);
  // Serial.println(":");

  // 基準値を引いて調整
  // x -= 1950;
  // y -= 1904;

  // x -= 2170;
  // y -= 2220;

  x -= x_btncenter;
  y -= y_btncenter;

  // 値の制限
  if (x > 1900) {
    x = 1900;
  } else if (x < -1900) {
    x = -1900;
  }
  if (y > 1900) {
    y = 1900;
  } else if (y < -1900) {
    y = -1900;
  }
// 角度を計算（ラジアン）
  float angle = atan2(y, x);

  // 角度を0~360度に変換
  if (angle < 0) {
    angle += 2 * M_PI;
  }
  float degrees = angle * 180.0 / M_PI;

  // 方向を判定
  int direction = -1; // デフォルトは方向がないという意味

  

  if (degrees >= 22.5 && degrees < 67.5) {
    direction = 0; // 右上
  } else if (degrees >= 67.5 && degrees < 112.5) {
    direction = 1; // 上
  } else if (degrees >= 112.5 && degrees < 157.5) {
    direction = 2; // 左上
  } else if (degrees >= 157.5 && degrees < 202.5) {
    direction = 3; // 左
  } else if (degrees >= 202.5 && degrees < 247.5) {
    direction = 4; // 左下
  } else if (degrees >= 247.5 && degrees < 292.5) {
    direction = 5; // 下
  } else if (degrees >= 292.5 && degrees < 337.5) {
    direction = 6; // 右下
  } else if ((degrees >= 337.5 && degrees <= 360) || (degrees >= 0 && degrees < 22.5)) {
    direction = 7; // 右
  }

  if (abs(x) < 300 && abs(y) < 300) {
    direction = -1; // 方向がない
  }

  return direction;
  // return y;
}


void LovyanGFX_DentaroUI::setupPhBtns(int pfbNo0, int pfbNo1, int pfbNo2 ){
  phbs.setup(pfbNo0, pfbNo1, pfbNo2);
}

std::uint32_t LovyanGFX_DentaroUI::getHitValue(){
  // コマンド判定結果を取得
  return  phbs.getValue();
}

void LovyanGFX_DentaroUI::updatePhBtns()
{
  delay(1);
  // 入力状態の更新（変化がなければ終了）
  if (!phbs.loop()) return;

  // コマンド判定結果を取得
  static std::uint32_t hitvalue = ~0U;
  std::uint32_t prev = hitvalue;
  hitvalue = phbs.getValue();
  // Serial.println(hitvalue);

  // 以前の値と同じ場合は除外
  if (prev == hitvalue) return;

}