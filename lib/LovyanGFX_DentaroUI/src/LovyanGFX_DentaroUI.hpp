#pragma once //インクルードガード

#include "LovyanGFX_DentaroUI.hpp"
#include "TouchBtn.hpp"
#include <deque>

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#include <limits.h>
#include <stddef.h>
#include <vector>

#include "DentaroVector.hpp"
#include "DentaroPhisics.hpp"
#include "PhysicBtn.hpp"//物理ボタン用
using namespace std;

//開発者表記
#define CHAR_1_BYTE_5 5//=EN 5文字
#define CHAR_1_BYTE_9 9//=EN 9文字
#define CHAR_3_BYTE_5 15//=JP 5文字
#define CHAR_3_BYTE_9 27//=JP 9文字


//開発者表記
#define CHAR_3_BYTE 0//=JP
#define CHAR_1_BYTE 2//=EN
#define NUMERIC 4
//ユーザー表記(Arduino側でしか使わない)
#define JP 0
#define EN 2



#define SHOW_NAMED 0
#define SHOW_ALL 1

#define TOUCH_MODE 0
#define PHYSICAL_MODE 1
#define ANIME_MODE 2

#define TOGGLE_MODE true

#define PARENT_LCD 0
#define PARENT_SPRITE 1

#define ROW_MODE false
#define COL_MODE true

// #define SHIFT_NUM 5//シフトパネルの数
#define HENKAN_NUM 57

#define PRESET_BTN_NUM 20

#define VISIBLE true
#define INVISIBLE false

//--地図用
#define BUF_PNG_NUM 1



class MyTFT_eSPI : public LGFX {
  public:

    void drawObako(uint16_t* data){
    int _width = 128;
    int _height = 128;

    pushImage(0,0,128,128,data);
    }
};

class MyTFT_eSprite : public LGFX_Sprite {
  public:
    MyTFT_eSprite(MyTFT_eSPI* tft): LGFX_Sprite(tft){
      _mytft = tft;
    }

    void myDrawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color){
      drawFastHLine(x, y, w, color);
      drawFastHLine(x, y + h - 1, w, color);
      // Avoid drawing corner pixels twice
      drawFastVLine(x, y+1, h-2, color);
      drawFastVLine(x + w - 1, y+1, h-2, color);
    }

    void drawObako(){
      _mytft->drawObako(_img);
    }

  private:
    uint16_t* _img;
    MyTFT_eSPI* _mytft;
    MyTFT_eSprite * _mysprite;
  };

class RetClass {
public:
  int btnID = -1;
  int btnNo = -1;
  // bool toggleVal = true;
  lgfx::v1::touch_point_t sp;
  
  void reset(){
    btnID = -1;//-1;
    btnNo = -1;
    sp.x = 0;
    sp.y = 0;
  }

  int setBtnID_ret(int _btnID){
    btnID = _btnID;
    return btnID;
  }

  int setBtnNO_ret(int _btnNo){
    btnNo = _btnNo;
    return btnNo;
  }

  int ret0_return_btnID(int _btnID){
    btnID = _btnID;
    return btnID;
  }

  int ret1_return_btnNo(int _btnNo){
    btnNo = _btnNo;
    return btnNo;
  }

  void func01(int _btnID, int _x, int _y, int _eventState, int _runEventNo) {   // 関数プロトタイプ「void xxx(int,int,int,int);」
    btnID = _btnID;
    sp.x = _x;
    sp.y = _y;

    if(_eventState!=-1&&_eventState!=0&&_eventState!=5&&_eventState!=6&&_eventState!=7&&_eventState!=8){//触れていないときNONEと、待機中WAIT、移動中MOVEのイベントは表示しない
      Serial.print("BUTTON_NO_"+String(_btnID));
      
      if(_eventState == 3)Serial.println(String("TAP"));
      if(_eventState == 4)Serial.println(String("W_TAP"));
      if(_eventState == 10)Serial.println(String("RIGHT"));
      if(_eventState == 12)Serial.println(String("UP"));
      if(_eventState == 14)Serial.println(String("LEFT"));
      if(_eventState == 16)Serial.println(String("DOWN"));

      if(_eventState == 11)Serial.println(String("RIGHT_UP"));
      if(_eventState == 13)Serial.println(String("LEFT_UP"));
      if(_eventState == 15)Serial.println(String("LEFT_DOWN"));
      if(_eventState == 17)Serial.println(String("RIGHT_DOWN"));
      
      if(_eventState == 2)Serial.println(String("DRAG"));
      
      //FrameEvent
      if(_eventState == 6)Serial.println(String("TOUCH"));
      if(_eventState == 7)Serial.println(String("WAIT"));
      if(_eventState == 8)Serial.println(String("MOVE"));
      if(_eventState == 9)Serial.println(String("RELEASE"));
      
    }
  }
};

class RetClass;

class UiContainer{
  public:
  int id = -1;
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int row = 0;
  int col = 0;

  int r0 = 0;
  int r1 = 0;
  int a = 0;
  int n = 0;

  //円形ボタン
  int c_d = 0;//中心から現在のタッチポイントまでの距離current_distance
  int c_a = 0;//中心から現在のタッチポイントがなす角度current_angle
  int p_a = 0;//前フレームのcurrent_angle= pre_angle
  int diff_a = 0;//c_a - p_a;

  int b_sNo = 0;
  int b_num = 0;
  int eventNo = -1; //MULTI_EVENT=-1
  int parentID = 0;
  String label = "";
  bool toggle_mode = false;
};

class RetClass;
class LovyanGFX_DentaroUI;

// // static 変数を初期化
// int totalButtonCount = 0;
// // 新しいパネルを追加した際の処理
// void addPanel(Panel* panel) {
//     // 新しいパネルのボタン数を totalButtonCount に加算
//     totalButtonCount += panel->row * panel->col;
//     // パネルをリストに追加する処理...
// }
// ボタンの属性を管理する構造体
struct TouchButton {
    int id;       // ボタンID
    int x;        // ボタンのx座標
    int y;        // ボタンのy座標
    int width;    // ボタンの幅
    int height;   // ボタンの高さ
    bool visibleF;
};

class Panel {
  public:
  std::vector<TouchBtn*> touchBtns;

  int id = -1;
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int row = 0;
  int col = 0;

  int r0 = 0;
  int r1 = 0;
  int a = 0;
  int n = 0;
  
  //円形ボタン
  int c_d = 0;//中心から現在のタッチポイントまでの距離current_distance
  int c_a = 0;//中心から現在のタッチポイントがなす角度current_angle
  int p_a = 0;//前フレームのcurrent_angle= pre_angle
  int diff_a = 0;//c_a - p_a;

  int b_sNo = 0;
  int b_num = 0;
  int eventNo = -1; //MULTI_EVENT=-1
  int parentID = 0;
  String label = "";
  String btn_name = "0";
  // String shift_btn_name = "ば";
  
  bool toggle_mode = false;

  int btn_mode = -1;
  int ui_no = -1;
  bool visibleF = true;
  bool availableF = true;
  bool selectBtnF = false;

  std::vector<TouchButton> buttons;

  Panel() {
    // デフォルトコンストラクタ
  }

  ~Panel() {
    clearTouchBtns();  // Panel オブジェクトが破棄されるときに TouchBtn オブジェクトも解放
  }

  // String getHenkanChar(int _henkanListNo, int _kanaShiftNo)
  // {
  //   return kanalist[_henkanListNo][_kanaShiftNo];
  // }

  void panelbtnDraw(LovyanGFX&  _lcd, int _x, int _y, int _w, int _h, String _btn_name, size_t _btnID, size_t _kanaShiftNo)
  {
    _lcd.fillRoundRect( _x, _y, _w, _h, 5, TFT_BLACK );
    _lcd.drawRoundRect( _x, _y, _w, _h, 5, TFT_WHITE );

    if(btn_mode != TOUCH_PRE_MODE){
      //  if( visibleF == true ){
      _lcd.setTextColor( TFT_WHITE );
      _lcd.setTextSize(1);
      _lcd.setCursor(_x+4 , _y+10 );
      _lcd.setFont(&lgfxJapanGothicP_16);
      _lcd.setTextWrap(true);
      _lcd.setTextScroll(true);
      size_t touchBtnID = _btnID - 20 - PRESET_BTN_NUM;

      if(btn_mode == TOUCH_FLICK_MODE){
        if(_btnID >= 40 ){
          if(_kanaShiftNo==0){
            touchBtns[touchBtnID]->shift_btn_name  = _btn_name;
          }
          else
          {
            if (_btn_name != "ま" || _btn_name != "な")
              {
                // if (_kanaShiftNo == 1 || _kanaShiftNo == 2)
                // {
                //   int i = 0;
                //   while (_btn_name != kanalist[i][0] || i > HENKAN_NUM - 1)
                //   {
                //     i++;
                //     if (i > HENKAN_NUM - 1)
                //       break;
                //   }
                //   if (i < HENKAN_NUM && kanalist[i][_kanaShiftNo].c_str() != NULL)
                //     touchBtns[touchBtnID]->shift_btn_name = kanalist[i][_kanaShiftNo].c_str();
                //   else if (i < HENKAN_NUM && kanalist[i][_kanaShiftNo].c_str() != "＿")
                //     touchBtns[touchBtnID]->shift_btn_name = "無";
                //   else
                //     touchBtns[touchBtnID]->shift_btn_name = "虚";
                // }
              }
          }

          _lcd.print( touchBtns[_btnID - 20 - PRESET_BTN_NUM]->shift_btn_name );
        }else{
          String btn_name_headline = "";
          if(_btn_name == "#"){btn_name_headline = "#@|&";}
          else if(_btn_name == "A"){btn_name_headline = "ABC";}
          else if(_btn_name == "D"){btn_name_headline = "DEF";}
          else if(_btn_name == "G"){btn_name_headline = "GHI";}
          else if(_btn_name == "J"){btn_name_headline = "JKL";}
          else if(_btn_name == "M"){btn_name_headline = "MNO";}
          else if(_btn_name == "P"){btn_name_headline = "PQRS";}
          else if(_btn_name == "T"){btn_name_headline = "TUV";}
          else if(_btn_name == "W"){btn_name_headline = "WXYZ";}
          else if(_btn_name == "^"){btn_name_headline = "^[$]\\";}
          else if(_btn_name == "'"){btn_name_headline = "'<\">";}
          else if(_btn_name == "."){btn_name_headline = ".?!";}

          else if(_btn_name == "%"){btn_name_headline = "%#@&";}
          else if(_btn_name == "a"){btn_name_headline = "abc";}
          else if(_btn_name == "d"){btn_name_headline = "def";}
          else if(_btn_name == "g"){btn_name_headline = "ghi";}
          else if(_btn_name == "j"){btn_name_headline = "jkl";}
          else if(_btn_name == "m"){btn_name_headline = "mno";}
          else if(_btn_name == "p"){btn_name_headline = "pqrs";}
          else if(_btn_name == "t"){btn_name_headline = "tuv";}
          else if(_btn_name == "w"){btn_name_headline = "wxyz";}
          else if(_btn_name == "-"){btn_name_headline = "-(/)_";}
          else if(_btn_name == ":"){btn_name_headline = ":'\"";}
          else if(_btn_name == "."){btn_name_headline = ".?!";}
          else {btn_name_headline = _btn_name;}

          _lcd.print( btn_name_headline );
        }

      }else{
        _lcd.print( _btn_name );
      }

      // _lcd.print( _btn_name );
    }
  }


  void setBtnName(String _btn_name){
    btn_name = _btn_name;
  }

  void setSelectBtnF(bool _selectBtnF){
    selectBtnF = _selectBtnF;
  }

  lgfx::v1::touch_point_t getTouchPoint(int _x, int _y){
    lgfx::v1::touch_point_t tp;
    tp.x = _x;
    tp.y = _y;
    return tp;
  }

  void addTouchBtn(int _gBtnID, int _b_x, int _b_y,int _b_w, int _b_h, int _btn_mode, int _eventNo, int _xy_mode) {

    TouchBtn* touchBtn = new TouchBtn();  // 新しい TouchBtn オブジェクトを動的に確保
    touchBtns.push_back(touchBtn);  // vector に追加
    label = String(_gBtnID);

    if(_btn_mode==TOUCH_BTN_MODE||_btn_mode==0){
      btn_name = "BTN_"+String(_gBtnID);
      // touchBtn->initBtn(_gBtnID, label, _b_x, _b_y, _b_w, _b_h, getTouchPoint(x, y), _btn_mode);
      touchBtn->initSlider(_gBtnID, label, _b_x, _b_y, _b_w, _b_h, getTouchPoint(x, y), _btn_mode, _eventNo, _xy_mode);
    }
    else if(_btn_mode==TOUCH_SLIDER_MODE){
      btn_name = "SLIDER_"+String(_gBtnID);
      touchBtn->initSlider(_gBtnID, label, _b_x, _b_y, _b_w, _b_h, getTouchPoint(x, y), _btn_mode, _eventNo, _xy_mode);
    }
    else if(_btn_mode==TOUCH_FLICK_MODE){
      btn_name = "FLICK_"+String(_gBtnID);
      touchBtn->initFlick(_gBtnID, label, _b_x, _b_y, _b_w, _b_h, getTouchPoint(x, y), _btn_mode);
    }
    touchBtn->setVisibleF(true);
    touchBtn->setAvailableF(true);
  }

  void clearTouchBtns() {
    for (TouchBtn* touchBtn : touchBtns) {
      delete touchBtn;  // TouchBtn オブジェクトを解放
    }
    touchBtns.clear();  // vector をクリア
  }

  // ボタンIDを取得するメソッド
    int getButtonId(int rowIndex, int colIndex) {
        if (rowIndex >= 0 && rowIndex < row && colIndex >= 0 && colIndex < col) {
            int index = rowIndex * col + colIndex;
            if (index < buttons.size()) {
                return buttons[index].id; // 計算した位置からIDを取得
            }
        }
        return -1; // 無効なインデックスの場合
    }

    bool getButtonVisibility(int buttonId) {
    for (const TouchButton &button : buttons) {
        if (button.id == buttonId) {
            return button.visibleF; // 一致するIDのボタンの可視性を返す
        }
    }
    return false; // 一致するボタンが見つからない場合は false を返す
}

};



class Ui{//パネルを集めてUIを作るクラス
  public:
  std::vector<Panel *> panels;

  int id = -1;
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;

  Ui() {
    // デフォルトコンストラクタ
  }

  ~Ui() {
    clearUis();  // Panel オブジェクトが破棄されるときに TouchBtn オブジェクトも解放
  }

  void clearUis() {
    for (Panel* panel : panels) {
      if (panel != nullptr) {
        delete panel;  // TouchBtn オブジェクトを解放
      }
    }
    panels.clear();  // vector をクリア
  }
};


struct Vec2{
  double x;
  double y;

  Vec2() = default; // デフォルトコンストラクタ
  Vec2(double _x, double _y) // コンストラクタ
  : x(_x)
  , y(_y) {}
};

class FlickPanel
{
  // private:
  public:
  int b_No = -1;
  std::deque<String> text_list; //パネルテキスト配置用デック配列
  FlickPanel(){};

  void new_text_list(int _btnID, String _btnsString )
  {
    if(_btnID==0)text_list[_btnID] = _btnsString;
    else text_list.push_back(_btnsString);
  }
};

class MapTile
{
  // private:
  public:
  int  posId = -1;
  int  posNo = -1;
  bool readF = false;
  int  mapName = -1;
  int  mapNo = -1;
  int  preMapNo = 0;
  int objNo = -1;
  int MapNo = 0;
  int latPos, lonPos = 0;
  bool existF = false;
  // int xtile, ytile, ztile = 0;
  int xtilePos, ytilePos = 0;
  int xtileNo, ytileNo, ztileNo = 0;//IDで使う
  int preXtileNo, preYtileNo, preZtileNo = 0;//
  int addXTileNo, addYTileNo = 0;//中央からの相対的なタイルナンバー -1,1
  int preAddXTileNo, preAddYTileNo = 0;//中央からの相対的なタイルナンバー -1,1
  String m_url;
  LGFX_Sprite buff_sprite;
  LGFX_Sprite* buff_sprite_p;
  MapTile(){};

  void begin(int _objNo, int _xtile, int _ytile,int _ztile, String _m_url)
  {
    objNo = _objNo;

    m_url = _m_url;

    buff_sprite.setPsram(USE_PSRAM);
    buff_sprite.setColorDepth(TILE_COL_DEPTH);//子スプライトの色深度
    buff_sprite.createSprite(256, 256);//子スプライトメモリ確保
    buff_sprite.setPaletteGrayscale();
    buff_sprite.drawPngFile(SD, m_url,
                            0, 0,
                            256, 256,
                            0, 0, 1.0, 1.0,
                            datum_t::top_left);
    buff_sprite_p = &buff_sprite;//オブジェクトのアドレスをコピーして、ポインタを格納
  }

  LGFX_Sprite* getSpritePtr(){return buff_sprite_p;}
  int getObjNo(){return objNo;}

  void setPreXtileNo(int _preXtileNo){preXtileNo = _preXtileNo;}
  void setPreYtileNo(int _preYtileNo){preYtileNo = _preYtileNo;}
  void setXtileNo(int _xtileNo){xtileNo = _xtileNo;}
  void setYtileNo(int _ytileNo){ytileNo = _ytileNo;}

  void setXtilePos(int _xtilePos){xtilePos = _xtilePos;}
  void setYtilePos(int _ytilePos){ytilePos = _ytilePos;}

  int getXtileNo(){return xtileNo;}
  int getYtileNo(){return ytileNo;}

  int getPreXtileNo(){return preXtileNo;}
  int getPreYtileNo(){return preYtileNo;}

  int getXtilePos(){return xtilePos;}
  int getYtilePos(){return ytilePos;}

  void setAddX( int _addXTileNo){addXTileNo = _addXTileNo;}
  void setAddY(int _addYTileNo){addYTileNo = _addYTileNo;}
  int getAddX(){return addXTileNo;}
  int getAddY(){return addYTileNo;}

  void setPreAddX( int _preAddXTileNo){preAddXTileNo = _preAddXTileNo;}
  void setPreAddY(int _preAddYTileNo){preAddYTileNo = _preAddYTileNo;}
  int getPreAddX(){return preAddXTileNo;}
  int getPreAddY(){return preAddYTileNo;}

  void setMapReadF( bool _readF){readF = _readF;}
  bool getMapReadF(){return readF;};
  // void setMapName( int _mapName){mapName = _mapName;}

  int getMapName(){return mapName;}
  void setMapNo(int _mapNo){mapNo = _mapNo;}
  int getMapNo(){return mapNo;}

  void setPreMapNo(int _preMapNo){preMapNo = _preMapNo;}
  int getPreMapNo(){return preMapNo;}

  void setPosNo(int _posNo){posNo = _posNo;}
  int getPosNo(){return posNo;}

  void setExistF(bool _existF){existF = _existF;}
  bool getExistF(){return existF;}
};

class LovyanGFX_DentaroUI {

  private:

  int x_btncenter;
  int y_btncenter;

  uint32_t eventBits = 0b00000000000000000000000000000000;
  uint8_t tapCount = 0;//タップカウンタ
  uint8_t lastTapCount = 0;
  bool jadgeF = false;

    lgfx::v1::touch_point_t tp;
    lgfx::v1::touch_point_t sp;
    lgfx::v1::touch_point_t sp2;
    lgfx::v1::touch_point_t layoutSpritePos; //レイアウト用
    lgfx::v1::touch_point_t lcdPos;//レイアウト用
    uint32_t btnState = B00000000;
    uint16_t touchState = B00000000;
    unsigned long touchStartTime = 0;
    unsigned long preTouchStartTime = 0;
    unsigned long lastTappedTime = 0;
    unsigned long firstTappedTime = 0;

    unsigned long sTime = 0;
    unsigned long tappedTime = 0;
    
    int eventState = -1;
    int flickState = -1;
    int tap_flick_thre = 82000;//タップとフリックの閾値
    int dist_thre = 40;
    int uiId = -1;
    int gPanelID = 0;
    int gBtnID = 0;
    uint constantBtnID = 9999;
    bool constantGetF = false;
    int selectBtnID = -1;
    int selectBtnBoxID = -1;
    int runEventNo =  -1;
    int parentID = 0;//初期値0 =　PARENT_LCD
    bool availableF = false;
    int uiMode = TOUCH_MODE;
    std::deque<TouchBtn*> touch_btn_list; //ボタン配置用デック配列
    std::deque<TouchBtn*> flick_touch_btn_list; //フリックボタン配置用デック配列
    std::deque<FlickPanel*> flickPanels;
    int showFlickPanelNo = 0;

    int timeCnt = 0;
    uint16_t clist[5] = {0,0,0,0,0};
    int preEventState = -1;
    int AngleCount = 0;
    int panels_num = 0;
    int uiAddBoxes_num = 0;

    UiContainer flickPanel;//9個の文字を入れる
    int layoutSprite_w = 0;
    int layoutSprite_h = 0;
    bool toggle_mode = false;

    String m_url = "";
    RetClass obj_ret;
    int shiftNum = 3;
    int charMode = CHAR_3_BYTE; //日本語
    //int charMode = CHAR_1_BYTE; //英語

    int charNumMode = CHAR_3_BYTE_5;//日本語5文字
    int sec, psec;
    int fps = 0;
    int frame_count = 0;
    bool use_flickUiSpriteF = false;
    int charNo=0;
    String flickString = "";
    String flickStr = "";
    String flickStrDel = "";
    String preFlickChar = "";
    String previewFlickChar = "";
    String finalChar ="";
    int fpNo = 0;
    // int kanaShiftNo = 0;
    bool selectModeF = false;
    int curbtnID;//現在の行番号
    int curKanaRowNo = 0;
    int curKanaColNo = 0;
    bool touchCalibrationF = false;
    int touchZoom = 1;
    int addBtnNum = 0;

    // PhysicBtn phbs;
    // uint16_t calData[8] = {326,3722,259,185,3776,3655,3776,243};//y
    uint16_t calData[8] = {3811,3853,345,3789,3711,403,1086,430};//daiso
    //uint16_t calData[8] = {558,3648,447,396,3599,3622,3625,324};
    // uint16_t calData[8];// = {0,1,0,273,319,1,319,264};//core2
    uint16_t calDataOK = 0;

    int LeftBtnUiID = 0;
    int RightBtnUiID = 0;
    int FlickUiID = 0;

    bool uiOpenF = true;

    //--Map用
    MapTile* MapTiles[9];
    bool DownloadF = false;
    bool mataidaF =false;
    // double latPos = 35.667995;
    // double lonPos = 139.7532971887966;//139.642076;
    // int tileZoom = 15;//8; //座標計算用

    int xtile = 0;
    int ytile = 0;
    int ztile = 0;
    int xtileNo = 0;
    int ytileNo = 0;

    int dirID = 0;

    int preXtileNo = 0;
    int preYtileNo = 0;
    int preXtile = 0;
    int preYtile = 0;

    int preDirID = 0;

    float vx = 0;
    float vy = 0;
    String host = "";
    //dirID
    //|6|7|8|
    //|5|0|1|
    //|4|3|2|
    // int mapNolist[9][2];
    // int tilePositons[9][4];
    int mapNoArray[9]= {-1,-1,-1,-1,-1,-1,-1,-1,-1};

    float matrix[6];
    float matrix_list[9][6] = {
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0},
      {1.0, 0.0, 0,  0.0, 1.0, 0}
    };

    //        //dirID
    //        //|6|7|8|
    //        //|5|0|1|
    //        //|4|3|2|
    int addTPosList[9][2] = {
      { 0, 0},{ 1, 0},{ 0, 1},
      { -1,0},{ 0,-1},{ 1,-1},
      {1, 1},{-1,1},{ -1,-1}
    };

    int gPosId = -1;

    // int preReadXtileNo[9];
    // int preReadYtileNo[9];

    bool allpushF = false;
    int ecnt = 0;

    String ROI_m_url ="";

    //----Map用ここまで
    int phbtnState[4];//物理ボリューム
    int flickShiftNo = 0;
    // int preflickShiftNo = 0;
    int kanaShiftNo = 0;

public:


  std::vector<Ui*> uis;

  LovyanGFX_DentaroUI( LGFX* _lcd );
  LGFX* lcd;//タッチポイントとれる

  LGFX_Sprite layoutSprite_list[BUF_PNG_NUM];

  PhysicBtn phbs;

  //LovyanGFX* lgfx;//グラフィックのみ
  // LGFX_Sprite flickUiSprite;//フリック展開パネル用
  void update( LGFX& _lcd );
  void update2( LGFX& _lcd );

  void begin( LGFX& _lcd, int _colBit, int _rotateNo);
  void begin( LGFX& _lcd, int _colBit, int _rotateNo, bool _calibF);
  // bool begin( LGFX& _lcd, int _colBit, int _rotateNo, bool _calibF);
  void begin( LGFX& _lcd, String _host, int _shiftNum, int _colBit, int _rotateNo, bool _calibF );
  void begin( LGFX& _lcd, int _shiftNum, int _colBit, int _rotateNo, bool _calibF );
  void begin(LGFX& _lcd);

  int getTouchZoom();
  void setTouchZoom(int _touchzoom);
  void showSavedCalData(LGFX& _lcd);

  void touchCalibration (bool _calibUseF);
  bool isAvailable(int _btnID);

  void calibrationRun( LGFX& _lcd);
  String rCalData(String _wrfile);
  int getCalData(int _calNo);

  void addHandler(int _uiId, int _panelID, int _btnNo, DelegateBase2* _func, uint16_t _runEventNo, int parentID = 0, bool _constantGetF = false);
  void circle(LovyanGFX* _lcd, uint16_t c, int fillF);
  void rect(LovyanGFX* _lcd, uint16_t c, int fillF);
  float getAngle(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b );
  float getDist(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b );
  size_t getVecNo();

  int getPreEvent();

  void updateSelectBtnID(int _selectBtnID);
  lgfx::v1::touch_point_t getStartPos();//タッチされたスタート地点を取得
  lgfx::v1::touch_point_t getPos();//タッチしている座標を取得
  void setPos(int _x, int _y);
  void setStartPos(int _x, int _y);

  DelegateBase2 *setBtnID_ret_DG = Delegate2<RetClass>::createDelegator2( &obj_ret, &RetClass::setBtnID_ret );//型式が違うプロトタイプ関数
  DelegateBase2 *setBtnNO_ret_DG = Delegate2<RetClass>::createDelegator2( &obj_ret, &RetClass::setBtnNO_ret );//型式が違うプロトタイプ関数
  
  void setBtnName(int _uiId, int _panelId, int _btnNo, String _btnName);
  void setBtnName(int _btnNo, String _btnName, String _btnNameFalse);//toggle用
  void setQWERTY(int _uiId, String _btnsString, LGFX_Sprite& _sprite);

  void createOBtns( int _x, int _y, int _r0,int _r1, int _a, int _n, int _eventNo);//円形に並ぶ
  void createOBtns( int _r0,int _r1, int _a, int _n, int _eventNo);//円形に並ぶ
  
  void createToggles( int _uiSprite_x, int _uiSprite_y, int _w,int _h, int _row, int _col, int _eventNo);//縦方向に並ぶ

  // void createFlicks( int _uiSprite_x, int _uiSprite_y, int _w,int _h, int _row, int _col, int _eventNo);//フリック生成
  void createFlick9Btns(LGFX_Sprite& _flickUiSprite);
  void drawFlickBtns( LovyanGFX& _lcd, int _btnID, int _btnNo, bool _visible, bool _available );
  // void setFlickPanel(int _flickPanelID, int _btnID, String _btnsString, int _btn_mode);
  void setFlickPanel( int _flickPanelID, int _btnID, String _btnsString );
  bool readFlickData(int _uiId,int startLine);
  // void readFlickDataline(int _uiId, int _btnID);
  void readFlickDataline(int _PRESET_BTN_NUM, int _uiId, int _btnID);

  String getFlickline(int flickDatalineNo);
  String getFlickChar(int _selectbtnID);//0~4
  String getFlickChar(int _PRESET_BTN_NUM, int _uiId, int _btnID);
  void setupFlickData(int _uiId);
  void flickShift();
  void kanaShift();
  void kanaShift(size_t _kanaShiftNo);
  void drawFlicks(int _uiId, LovyanGFX& _lcd);
  void drawBtns(int _uiId, LovyanGFX& _lcd);

  void drawBtns(LovyanGFX& _lcd);
  void drawBtns(int _uiId, LovyanGFX& _lcd, int _uiSprite_x, int _uiSprite_y);

  // void drawOBtns(int _uiId, LovyanGFX& _lcd, int _x, int _y);
  // void drawOBtns(int _uiId, LovyanGFX& _lcd, LGFX_Sprite& _uiSprite　);
  void drawOBtns(int _uiId, LovyanGFX& _lcd, int _uiSprite_x, int _uiSprite_y);

  void drawToggles(int _uiId, LovyanGFX& _lcd, int _uiSprite_x, int _uiSprite_y);
  void drawToggles(int _uiId, LovyanGFX& _lcd );

  void drawSliders(int _uiId, LovyanGFX& _lcd );
  // void drawTile(int _uiId, LovyanGFX* _lcd, LGFX_Sprite& _layoutSprite, uint8_t _bgColIndex, int _spriteNo);

  void drawSelectBtn(int _id);
  // void setPngTile(fs::FS &fs, String _m_url, int _spriteNo);
  // void setTilePos(int _id, int _x, int _y);
  // LGFX_Sprite getTileSprite(int _btnID);
  int getTouchBtnNo();//タッチされたボタンオブジェクトの番号を取得
  int getTouchBtnID();//タッチされたボタンオブジェクトのIDを取得
  int getEvent();
  int getFlickEvent();
  void showTouchEventInfo(LovyanGFX& _lcd, int _x, int _y);
  lgfx::v1::touch_point_t getTouchPoint(int _x, int _y);
  float getSliderVal( int _uiId, int _panel_no, int _btnNo );
  float getSliderVal(int _uiId, int _panel_no, int _btnNo, int _xy);
  Vec2 getSliderVec2( int _uiId, int _btnNo );

  void setSliderVal(int _uiId, int _panel_no, int _btnNo, float _x, float _y);
  bool getToggleVal(int _uiId, int _btnNo);
  // bool getToggleVal(int _btnID);
  // bool getToggleVal2();

  void setAllBtnAvailableF(int _uiId, bool _available);
  void setAvailableF(int _uiId, int _btnID, bool _available);

  void setAllBtnVisibleF(int _uiId, bool _visible);
  void setVisibleF(int _uiId, int _btnID, bool _visible);

  void setCharMode(int _charMode);
  int getCharMode();

  String getFlickStr();

  int getflickPanelBtnNo(int uiID);

  int getUiFirstNo(int _uiId);
  int getUiBtnNum(int _uiId);
  int getUiBoxNum();
  int getAllBtnNum();
  int getParentID();
  int getUiID( const char* _uiLabel );
    void setBtnID(int _uiId, int _panelId, int _btnID);

  // void setShowFlickPanelNo(int _showFlickPanelNo);
  void setUiLabels(int _uiId, int _showFlickPanelNo);
  lgfx::v1::touch_point_t getBtnPos(int _btnID);

  void showInfo( LovyanGFX& _lcd, int _infox, int _infoy);

  // String getHenkanChar(int _henkanListNo, int _kanaShiftNo);
  void setFlick(int _uiId, int _panelId, int _FlickUiID, int _LeftBtnUiID, int _RightBtnUiID);//キーボード用プリセッ；
  void setFlick(int _uiId, int _panelId, int _charMode, int _FlickUiID, int _LeftBtnUiID, int _RightBtnUiID );
  String getInvisibleFlickStrings();
  String getFlickString();
  String getFlickString(bool _visibleMode);
  String getFlickline();
  // String getKana(int _panelID, int _rowID, int _colID, int _kanaShiftNo);

  const char* next_c_mb(const char* c);
  void ngetc(char* const dst,const char* src);
  bool nchr_cmp(const char* c1, const char* c2);
  std::vector<std::string> split_mb(const char* src, const char* del);

  String delEndChar(String _str, int _ByteNum);

  // void delChar();
  // String delChar(String _flickStr);
  String delChar(const String& _flickStr);
  void switchToggleVal();

  //---Map用関数

  void setDrawFinishF(int _objId, bool _drawFinishF);
  bool getDrawFinishF(int _objId);

  int get_gPosId();
  void set_gPosId(int _gPosId);


  void setAddX(int _objId,  int _xtileNo);
  void setAddY(int _objId,  int _ytileNo);
  void setPreAddX(int _objId,  int _xtileNo);
  void setPreAddY(int _objId,  int _ytileNo);

  int getPositionNo(int _addXTileNo, int _addYTileNo);

  void setXtileNo(int _objId,  int _xtileNo);
  void setYtileNo(int _objId,  int _ytileNo);
  void setTileNo(int _objId, int _xtileNo, int _ytileNo);

  void setPreYtileNo(int _objId,  int _preYtileNo);
  void setPreXtileNo(int _objId,  int _preXtileNo);

  void setXtilePos(int _objId,  int _xtilePos);
  void setYtilePos(int _objId,  int _ytilePos);

  int getAddX(int _spriteNo);
  int getAddY(int _spriteNo);
  int getPreAddX(int _spriteNo);
  int getPreAddY(int _spriteNo);

  String getPngUrl(int addNo);
  int getXtile();
  int getYtile();
  int getZtile();
  int getXtileNo();
  int getYtileNo();

  int getXtileNo(int _objNo);
  int getYtileNo(int _objNo);

  int getPreXtileNo(int _objNo);
  int getPreYtileNo(int _objNo);

  int getPreXtileNo();
  int getPreYtileNo();
  int getPreXtile();
  int getPreYtile();

  int getVx();
  int getVy();

  void setExistF(int _objId, bool _existF);
  bool getExistF(int _objId);

  int getDirID();
  int getPreDirID();
  void setPreDirID(int _dirID);

  void nowLoc(LovyanGFX& _lcd);

  void drawMaps(LGFX& _lcd, double _walkLatPos, double _walkLonPos, int _tileZoom);

  void setPngTile(fs::FS &fs, String _m_url, int _spriteNo);
  void setDownloadF(bool _b);
  bool getDownloadF();
  void task2_setPngTile(int _posId);

  void getTilePos(double lat, double lon, int zoom_level);

  void updateOBtnSlider(int uiID, int _x, int _y);

  int getCurrentAngle(int uiID);
  int getOBtnDiffAngle(int uiID);

  //イベントビットの操作
  bool getEventBit(int bitNo);
  void setEventBit(int bitNo, bool inputBit);
  void resetEventBits();

  //物理ボタン
  // void updatePhVols();
  // int getPhVol(int n);
  // int getPhVolDir(int n);
  // int getPhVolVec(int n1, int n2); 

  void setBtnVals(int n, float val);
  bool loadParametersFromFile();
  bool saveParametersToFile(int newX, int newY);
  bool loadParametersFromFile2();
  bool saveParametersToFile2();

  void setConstantGetF(bool _constantGetF);

  void createPanel( 
    int _ui_no, int _btn_mode, 
    int _uiSprite_x, int _uiSprite_y, 
    int _w,int _h, int _row, int _col, 
    int _touchZoom, int _eventNo, int _xy_mode, int _uiContainerID, bool _visibleF);

  void createUIs(int uiContainernum);
  void deletePanels(int _uiContainerID, size_t _PRESET_BTN_NUM);
  void clearAddBtns();

  //物理ボタン
    void updatePhVols();
    int getPhVol(int n);
    int getPhVolDir(int n);
    int getPhVolVec(int n1, int n2); 
    void updatePhBtns();

    void setupPhBtns(int pfbNo0, int pfbNo1, int pfbNo2 );
    // const bits_btn_t*  getStack();
    std::uint32_t getHitValue();

  // void showSavedCalData(LGFX& _lcd);

};





