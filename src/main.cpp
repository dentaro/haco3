#include <Arduino.h>
#include <FS.h>
#include "SPIFFS.h"
#include "runLuaGame.h"
// #include "Editor.h"
#include "Channel.hpp"
#include <LovyanGFX_DentaroUI.hpp>
#include <map>
#include <TaskScheduler.h> // TaskSchedulerライブラリを使用
// #include "haco8/runHaco8Game.h"
// #define BSTARBUFNUM 1000
#define MAPWH 16//マップのpixelサイズ
#define SOUND_OFF_STATE 0
#define SOUND_ON_STATE  1
// #define TFT_WIFI_MODE 2
#define TFT_SOUNDEDIT_MODE 3

#define CH1_VIEW_MODE 1
#define CH8_VIEW_MODE 8
uint8_t ch_viewmode = CH8_VIEW_MODE;
uint8_t debugMode = 0;

Scheduler runner; // タスクスケジューラのインスタンス

int select_ui_id = TOUCH_BTN_MODE; // 0はプリセットパネル

TaskHandle_t taskHandle[4];
SemaphoreHandle_t syncSemaphore;
SemaphoreHandle_t nextFrameSemaphore;

//星のデータ用
std::vector<std::array<float, 2>> bsParamFloat;
std::vector<std::array<uint8_t, 1>> bsParamInt8t;
std::vector<std::array<uint16_t, 1>> bsParamInt16t;

uint8_t sprno;
uint8_t repeatnum;

uint8_t masterVol = 0;

bool sndbufferedF = false;
// int isEditMode;
// bool firstBootF = true;
// bool difffileF = false;//前と違うファイルを開こうとしたときに立つフラグ


bool sfxflag = false;
uint8_t sfxNo = 0;
uint8_t wavNo = 0;
uint8_t sfxChNo = 0;
uint8_t sfxVol = 0;
float sfxspeed = 100;
uint8_t loopStart = 0;
uint8_t loopEnd = 63;
uint8_t looplen = (loopEnd - loopStart)+1;
float bpm = 120;
int8_t soundState = SOUND_OFF_STATE;
uint8_t musicNo = 255;
// uint8_t musicSpeed = 64;
Channel* channels;
// // //音関連
uint8_t buffAreaNo = 0;
uint8_t gEfectNo = 0;
uint8_t effectVal = 0.0f;
uint8_t toneTickNo = 0;
uint8_t sfxTickNo = 0;
uint8_t instrument = 0;
uint8_t targetChannelNo = 0;//描画編集する効果音番号を設定（sfx(n)のnで効果音番号を指定することで作った効果音がなる）
uint8_t tickTime = 125;//125ms*8chはbpm60
uint8_t tickSpeed = 5;//連動してない
uint8_t patternNo = 0;//0~63

bool textMoveF = false;
bool shiftF = false;

bool enemyF = false;
uint8_t enemyX = 0;
uint8_t enemyY = 0;
uint8_t enemyTransCn = 0;
String enemyPath = "";

int gWx;
int gWy;
int gSpr8numX = 8;
int gSpr8numY = 8;
int gSprw = 8;
int gSprh = 8;

static int menu_x = 2;
static int menu_y = 20;
static int menu_w = 120;
static int menu_h = 30;
static int menu_padding = 36;

int HACO3_C0    = 0x0000;
int HACO3_C1    = 6474;//27,42,86 
int HACO3_C2    = 35018;
int HACO3_C3    = 1097;
int HACO3_C4    = 45669;
int HACO3_C5    = 25257;
int HACO3_C6    = 50712;
int HACO3_C7    = 65436;
int HACO3_C8    = 63496;//0xF802;
int HACO3_C9    = 64768;
int HACO3_C10   = 65376;
int HACO3_C11   = 1856;
int HACO3_C12   = 1407;
int HACO3_C13   = 33715;
int HACO3_C14   = 64341;
int HACO3_C15   = 65108;

uint8_t clist2[16][3] =
  {
  { 0,0,0},//0: 黒色
  { 27,42,86 },//1: 暗い青色
  { 137,24,84 },//2: 暗い紫色
  { 0,139,75 },//3: 暗い緑色
  { 183,76,45 },//4: 茶色
  { 97,87,78 },//5: 暗い灰色
  { 194,195,199 },//6: 明るい灰色
  { 255,241,231 },//7: 白色
  { 255,0,70 },//8: 赤色
  { 255,160,0 },//9: オレンジ
  { 255,238,0 },//10: 黄色
  { 0,234,0 },//11: 緑色
  { 0,173,255 },//12: 水色
  { 134,116,159 },//13: 藍色
  { 255,107,169 },//14: ピンク
  { 255,202,165}//15: 桃色
  };
  //2倍拡大表示用のパラメータ
float matrix[6] = {2.0,   // 横2倍
                     0.0,  // 横傾き
                     0.0,   // X座標
                     0.0,   // 縦傾き
                     2.0,   // 縦2倍
                     0.0    // Y座標
                    };

#define KEYBOARD_DATA 32
#define KEYBOARD_CLK  33

#define TFT_RUN_MODE 0
#define TFT_EDIT_MODE 1
#define TFT_WIFI_MODE 2

// PS2Keyboard keyboard;

// Speaker_Class speaker;

uint64_t frame = 0;

int isEditMode;
bool firstBootF = true;
bool difffileF = false;//前と違うファイルを開こうとしたときに立つフラグ

std::deque<int> buttonState;//ボタンの個数未定
// int buttonState[16];

enum struct FileType {
  LUA,
  JS,
  BMP,
  PNG,
  TXT,
  OTHER
};

//キーボード関連
// Editor editor;

char keychar;//キーボードから毎フレーム入ってくる文字

//esp-idfのライブラリを使う！
//https://qiita.com/norippy_i/items/0ed46e06427a1d574625
#include <driver/adc.h>//アナログボタンはこのヘッダファイルを忘れないように！！

using namespace std;

#define MAX_CHAR 1
#define FORMAT_SPIFFS_IF_FAILED true

#define BUF_PNG_NUM 0

uint8_t mainVol = 180;
uint8_t toolNo = 0;
int fps=60;//デフォルト


//outputmode最終描画の仕方
// int outputMode = FAST_MODE;//50FPS程度128*128 速いけど小さい画面　速度が必要なモード
int outputMode = WIDE_MODE;//20FPS程度240*240 遅いけれどタッチしやすい画面　パズルなど

uint8_t xpos, ypos = 0;
uint8_t colValR = 0;
uint8_t colValG = 0;
uint8_t colValB = 0;

uint8_t charSpritex = 0;
uint8_t charSpritey = 0;
int pressedBtnID = -1;//この値をタッチボタン、物理ボタンの両方から操作してbtnStateを間接的に操作している

// esp_now_peer_info_t slave;

int mapsprnos[16];
// int mapsprnos[16] = { 20, 11, 32, 44, 53, 49, 54, 32, 52, 41, 46, 42, 45, 50, 43, 38 };

//uiIDを変換する

int8_t convUiId[25] = {
-1,-1,-1,-1,-1,
 5, 3, 6,-1,10,
 1, 9, 2,-1,-1,
 7, 4, 8,-1,11,
11,-1,-1, 0,-1,
};

// int remapAnalogBtnID[8] = {7,4,8,2,6,3,5,1};//{1,2,3,4,5,6,7,8};
// int remapAnalogBtnID[8] = {11,1,10,3,9,0,8,2};//{1,2,3,4,5,6,7,8};
int remapAnalogBtnID[8] = {12,4,11,2,10,3,9,1};//{1,2,3,4,5,6,7,8};

// int remapPhysicBtnID[8] = {8,2,6,4,5,1,7,3};//{4,3,2,1,0,7,6,5};
// int remapPhysicBtnID[8] = {13,7,14,5,15,6,12,4};//{4,3,2,1,0,7,6,5};
int remapPhysicBtnID[8] = {14,8,15,6,16,7,13,5};//{4,3,2,1,0,7,6,5};

const uint8_t RGBValues[][3] PROGMEM = {//16bit用
  {0, 0, 0},     // 0: 黒色=なし
  {24, 40, 82},  // 1: 暗い青色
  {140, 24, 82}, // 2: 暗い紫色
  {0, 138, 74},  // 3: 暗い緑色
  {181, 77, 41}, // 4: 茶色 
  {99, 85, 74},  // 5: 暗い灰色
  {198, 195, 198}, // 6: 明るい灰色
  {255, 243, 231}, // 7: 白色
  {255, 0, 66},  // 8: 赤色
  {255, 162, 0}, // 9: オレンジ
  {255, 239, 0}, // 10: 黄色
  {0, 235, 0},   // 11: 緑色
  {0, 174, 255}, // 12: 水色
  {132, 117, 156}, // 13: 藍色
  {255, 105, 173}, // 14: ピンク
  {255, 203, 165}  // 15: 桃色
};
//2倍拡大表示用のパラメータ
float matrix_side[6] = {2.0,   // 横2倍
                     -0.0,  // 横傾き
                     258.0,   // X座標
                     0.0,   // 縦傾き
                     2.0,   // 縦2倍
                     0.0    // Y座標
                    };

LGFX screen;//LGFXを継承

LovyanGFX_DentaroUI ui(&screen);
LGFX_Sprite tft(&screen);
// LGFX_Sprite scaler(&screen);
static LGFX_Sprite sideSprite( &screen );//背景スプライトはディスプレイに出力
static LGFX_Sprite logoSprite( &screen );//背景スプライトはディスプレイに出力

// #include "MapDictionary.h"
// MapDictionary& dict = MapDictionary::getInstance();

LGFX_Sprite sprite88_roi = LGFX_Sprite(&tft);
LGFX_Sprite sprite11_roi = LGFX_Sprite(&tft);
LGFX_Sprite sprite64 = LGFX_Sprite();

std::vector<uint8_t> sprite64cnos_vector;

LGFX_Sprite buffSprite = LGFX_Sprite(&tft);

LGFX_Sprite sprite88_0 = LGFX_Sprite(&tft);

// LGFX_Sprite mapTileSprites[1];
// static LGFX_Sprite sliderSprite( &tft );//スライダ用

BaseGame* game;
// Tunes tunes;
String appfileName = "";//最初に実行されるアプリ名
String savedAppfileName = "";
// String txtName = "/init/txt/sample.txt";//実行されるファイル名

uint8_t mapsx = 0;
uint8_t mapsy = 0;
String mapFileName = "/init/map/0.png";
int readmapno = 0;
int divnum = 1;
bool readMapF = false;
//divnumが大きいほど少ない領域で展開できる(2の乗数)
// LGFX_Sprite spritebg[16];//16種類のスプライトを背景で使えるようにする
LGFX_Sprite spriteMap;//地図用スプライト

uint8_t mapArray[16][20];
bool mapready = false;

int8_t sprbits[128];//fgetでアクセスするスプライト属性を格納するための配列

char buf[MAX_CHAR];
// char str[100];//情報表示用
int mode = 0;//記号モード //0はrun 1はexit
int gameState = 0;
String appNameStr = "init";
int soundNo = -1;
float soundSpeed = 1.0;
bool musicflag = false;
bool toneflag = false;
bool firstLoopF = true;

float sliderval[2] = {0,0};
bool optionuiflag = false;

int addUiNum[4];
int allAddUiNum = 0;

int xtile = 0;
int ytile = 0;
float ztile = 0.0;

int xtileNo = 29100;
int ytileNo = 12909;

int hitvalue = 0;

uint8_t INPUT_START = 0;  /// ◄ START = ENTER
uint8_t INPUT_SELECT = 0; /// ► SELECT = BACKSPACE
uint8_t INPUT_SYSTEM = 0; /// ■ SYSTEM = ESCAPE

String chatstr = "";
String c = "";
int preFlickBtnID = 20;

bool btnpF = false;
bool prebtnpF = false;
int btnptick = 0;
int prebtnptick = 0;
int btnpms = 0;

unsigned long startTime = millis();

int vol_value; //analog値を代入する変数を定義
int statebtn_value; //analog値を代入する変数を定義
int jsx_value; //analog値を代入する変数を定義
int jsy_value; //analog値を代入する変数を定義
// getSign関数をMapDictionaryクラス外に移動
Vector2<int> getSign(int dirno) {
    if (dirno == -1) {
        // 方向を持たない場合、(0.0, 0.0, 0.0)を返す
        return {0, 0};
    } else {
        float dx = (dirno == 0 || dirno == 1 || dirno == 7) ? 1.0 : ((dirno == 3 || dirno == 4 || dirno == 5) ? -1.0 : 0.0);
        float dy = (dirno == 1 || dirno == 2 || dirno == 3) ? 1.0 : ((dirno == 5 || dirno == 6 || dirno == 7) ? -1.0 : 0.0);
        return {int(dx), int(dy)};
    }
}

void createPanels(int _uiContainerID, int _read_ui_no,bool _visibleF)
{
  // 抽象UIを生成
  File fr = SPIFFS.open("/init/param/uiinfo.txt", "r");
  String line;

  while (fr.available())
  {
    line = fr.readStringUntil('\n');
    if (!line.isEmpty())
    {
      int commaIndex = line.indexOf(',');
      if (commaIndex != -1)
      {
        String val = line.substring(0, commaIndex);
        addUiNum[0] = val.toInt();
        if (addUiNum[0] == _read_ui_no)
        { // 指定されたUI以外は生成しない

          // if(addUiNum[0]<=-1){//-1位下の時は生成しない

          // }
          // else{//0の時はボタンを生成
          for (int i = 1; i < 8; i++)
          {
            int nextCommaIndex = line.indexOf(',', commaIndex + 1);
            if (nextCommaIndex != -1)
            {
              val = line.substring(commaIndex + 1, nextCommaIndex);
              addUiNum[i] = val.toInt();
              commaIndex = nextCommaIndex;
            }
          }

          ui.createPanel(
              addUiNum[0], addUiNum[1],
              addUiNum[2], addUiNum[3],
              addUiNum[4], addUiNum[5],
              addUiNum[6], addUiNum[7], ui.getTouchZoom(), MULTI_EVENT, XY_VAL, _uiContainerID, _visibleF);

          allAddUiNum++;

          // }
        }
      }
    }
  }
  fr.close();
}

uint16_t gethaco3Col(uint8_t haco3ColNo) {
    uint16_t result = ((static_cast<uint16_t>(clist2[haco3ColNo][0]) >> 3) << 11) |
                      ((static_cast<uint16_t>(clist2[haco3ColNo][1]) >> 2) << 5) |
                       (static_cast<uint16_t>(clist2[haco3ColNo][2]) >> 3);
    return result;
}
void reboot()
{
  ESP.restart();
}

void setOpenConfig(String fileName, uint8_t _isEditMode) {
  char numStr[64];//64文字まで
  sprintf(numStr, "%s,%d,", 
    fileName.c_str(), _isEditMode
  );

  String writeStr = numStr;  // 書き込み文字列を設定
  File fw = SPIFFS.open("/init/param/openconfig.txt", "w"); // ファイルを書き込みモードで開く
  fw.println(writeStr);  // ファイルに書き込み
  // savedAppfileName = fileName;
  delay(50);
  fw.close(); // ファイルを閉じる
}

void reboot(String _fileName, uint8_t _isEditMode)
{
  setOpenConfig(_fileName, _isEditMode);
  // editor.setCursorConfig();//カーソルの位置を保存
  delay(100);
  ESP.restart();
}

Vector2<int> getKey2Sign(String _currentKey, String _targetKey) {
    int slashPos = _currentKey.indexOf('/'); // '/'の位置を取得
    if (slashPos != -1) { // '/'が見つかった場合
        String numA_str = _currentKey.substring(0, slashPos); // '/'より前の部分を取得
        String numB_str = _currentKey.substring(slashPos + 1); // '/'より後の部分を取得
        int numA_current = numA_str.toInt(); // 数字に変換
        int numB_current = numB_str.toInt(); // 数字に変換
        
        slashPos = _targetKey.indexOf('/'); // '/'の位置を取得
        if (slashPos != -1) { // '/'が見つかった場合
            numA_str = _targetKey.substring(0, slashPos); // '/'より前の部分を取得
            numB_str = _targetKey.substring(slashPos + 1); // '/'より後の部分を取得
            int numA_target = numA_str.toInt(); // 数字に変換
            int numB_target = numB_str.toInt(); // 数字に変換
            
            int dx = numA_target - numA_current;
            int dy = numB_target - numB_current;
            
            return {dx, dy};
        }
    }
    
    return {0, 0}; // デフォルトの値
}

Vector3<float> currentgpos = {0,0,0};;
Vector3<float> prePos= {0.0, 0.0, 0.0};
Vector3<float> currentPos = {0,0,0};
Vector3<float> diffPos = {0.0,0.0,0.0};

int dirNos[9];
int shouldNo = 0;
int downloadLimitNum = 0;
String targetKey = "";
float tileZoom = 15.0;
float bairitu = 1.0;

std::vector<String> temporaryKeys;
std::vector<String> previousKeys;
std::vector<String> writableKeys;
std::vector<String> downloadKeys;
std::vector<String> predownloadKeys;
std::vector<String> allKeys;
std::vector<String> preallKeys;

void printDownloadKeys() {
  Serial.println("Download Keys:");
  for (const auto& key : downloadKeys) {
      Serial.print(key);
  }
  Serial.println("");
}
void drawUI() {
    // fillRoundRectの最適化
    uint16_t cornerRadius = 5;
    uint16_t color = TFT_DARKGRAY;
    screen.setTextColor(TFT_DARKGRAY, TFT_BLACK);
    screen.setTextSize(1); // サイズ

    int buttonId = 0; // ボタンIDの初期化

    // パネル情報を取得して描画
    for (auto &uiContainer : ui.uis) {
        if (uiContainer != nullptr) {
            for (auto &panel : uiContainer->panels) {
                if (panel != nullptr) {
                    // パネルのボタン幅と高さを計算
                    int b_w = panel->w / panel->row;
                    int b_h = panel->h / panel->col;

                    for (int j = 0; j < panel->col; j++) {
                        for (int i = 0; i < panel->row; i++) {
                            bool _visibleF = panel->getButtonVisibility(buttonId); // ボタンの可視性を取得

                            if (_visibleF) { // ボタンが可視の場合のみ描画
                                int btn_x = panel->x + i * b_w;
                                int btn_y = panel->y + j * b_h;

                                // ボタンを描画
                                screen.drawRoundRect(btn_x, btn_y, b_w, b_h, cornerRadius, color);

                                // ボタンIDを表示
                                screen.setCursor(btn_x + 8, btn_y + 8); // ボタンの中心にカーソルを設定
                                screen.print(buttonId); // ボタンIDを表示
                            }

                            buttonId++; // ボタンIDをインクリメント
                        }
                    }
                }
            }
        }
    }

    // スプライトの解放
    logoSprite.deleteSprite();
}

FileType detectFileType(String *appfileName)
{
  if(appfileName->endsWith(".js")){
    return FileType::JS;
  }else if(appfileName->endsWith(".lua")){
    return FileType::LUA;
  }else if(appfileName->endsWith(".bmp")){
    return FileType::BMP;
  }else if(appfileName->endsWith(".png")){
    return FileType::PNG;
  }else if(appfileName->endsWith(".txt")){
    return FileType::TXT;
  }
  return FileType::OTHER;
}

String *targetfileName;
BaseGame* nextGameObject(String* _appfileName, int _gameState, String _mn)
{

  switch(detectFileType(_appfileName)){
    case FileType::JS:  
      // game = new RunJsGame(); 
      break;
    case FileType::LUA: 
      
      game = new runLuaGame(_gameState, _mn);
      break;
    case FileType::TXT: 
      // game = new RunJsGame(); 
      // //ファイル名がもし/init/param/caldata.txtなら
      // if(*_appfileName == CALIBRATION_FILE)
      // {
      //   ui.calibrationRun(screen);//キャリブレーション実行してcaldata.txtファイルを更新して
      //   drawLogo();//サイドボタンを書き直して
      // }
      // appfileName = "/init/txt/main.js";//txtエディタで開く
      break; //txteditorを立ち上げてtxtを開く
    case FileType::BMP: // todo: error
      game = NULL;
      break;
    case FileType::PNG: // todo: error
      // game = new RunJsGame(); 
      // appfileName = "/init/png/main.js";//pngエディタで開く
      break;
    case FileType::OTHER: // todo: error
      game = NULL;
      break;
  }

  return game;

}

// char *A;

uint32_t preTime;
// void setFileName(String s){
//   appfileName = s;
// }

void runFileName(String s){
  
  ui.setConstantGetF(false);//初期化処理 タッチポイントの常時取得を切る
  
  appfileName = s;
  mode = 1;//exit to run

}


int getcno2tftc(uint8_t _cno){
  switch (_cno)
  {
  case 0:return HACO3_C0;break;
  case 1:return HACO3_C1;break;
  case 2:return HACO3_C2;break;
  case 3:return HACO3_C3;break;
  case 4:return HACO3_C4;break;
  case 5:return HACO3_C5;break;
  case 6:return HACO3_C6;break;
  case 7:return HACO3_C7;break;
  case 8:return HACO3_C8;break;
  case 9:return HACO3_C9;break;
  case 10:return HACO3_C10;break;
  case 11:return HACO3_C11;break;
  case 12:return HACO3_C12;break;
  case 13:return HACO3_C13;break;
  case 14:return HACO3_C14;break;
  case 15:return HACO3_C15;break;

  default:
  return HACO3_C0;
    break;
  }
}


// タイマー
hw_timer_t * timer = NULL;

void readFile(fs::FS &fs, const char * path) {
   File file = fs.open(path);
   while(file.available()) file.read();
  //  while(file.available()) Serial.print(file.read());
}

//ファイル書き込み
void writeFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        return;
    }
    file.print(message);
}

void deleteFile(fs::FS &fs, const char * path){
   Serial.print("Deleting file: ");
   Serial.println(path);
   if(fs.remove(path)) Serial.print("− file deleted\n\r");
   else { Serial.print("− delete failed\n\r"); }
}

void listDir(fs::FS &fs){
   File root = fs.open("/");
   File file = root.openNextFile();
   while(file){
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.print(file.size());
      file = root.openNextFile();
   }
}

String rFirstAppName(String _wrfile){
  File fr = SPIFFS.open(_wrfile.c_str(), "r");// ⑩ファイルを読み込みモードで開く
  String _readStr = fr.readStringUntil('\n');// ⑪改行まで１行読み出し
  fr.close();	// ⑫	ファイルを閉じる
  return _readStr;
}

void getOpenConfig(String _wrfile) {
  File fr = SPIFFS.open(_wrfile, "r");
  String line;
  while (fr.available()) {
    line = fr.readStringUntil('\n');
    if (!line.isEmpty()) {
      int commaIndex = line.indexOf(',');
        String val = line.substring(0, commaIndex);
        if(val != NULL)appfileName =  val;
        else appfileName = "/init/main.lua";//configファイルが壊れていても強制的に値を入れて立ち上げる
          int nextCommaIndex = line.indexOf(',', commaIndex + 1);//一つ先のカンマ区切りの値に進める
          if (nextCommaIndex != -1) {
            val = line.substring(commaIndex + 1, nextCommaIndex);
            if(val.toInt() != NULL){
              isEditMode = val.toInt();
              Serial.print("editmode[");Serial.print(isEditMode);Serial.println("]");
            }else{
              isEditMode = 0;//configファイルが壊れていても強制的に値を入れて立ち上げる
            }
          }
    }
  }
  fr.close();
}

void setOpenConfig(String fileName, int _isEditMode) {
  char numStr[64];//64文字まで
  sprintf(numStr, "%s,%d,", 
    fileName.c_str(), _isEditMode
  );

  Serial.println(fileName.c_str());
  Serial.println(_isEditMode);

  String writeStr = numStr;  // 書き込み文字列を設定
  File fw = SPIFFS.open("/init/param/openconfig.txt", "w"); // ファイルを書き込みモードで開く
  fw.println(writeStr);  // ファイルに書き込み
  // savedAppfileName = fileName;
  delay(50);
  fw.close(); // ファイルを閉じる
}


int readMap()
{
  mapready = false;

  for(int n = 0; n<divnum; n++)
  {
    spriteMap.drawPngFile( SPIFFS, mapFileName, 0, (int32_t)(-MAPWH*n/divnum) );
    for(int j = 0; j<MAPWH/divnum; j++){
              // Serial.println("");
      for(int i = 0; i<MAPWH; i++){

        int k = j+(MAPWH/divnum)*(n);//マップ下部
        colValR = spriteMap.readPixelRGB(i,j).R8();
        colValG = spriteMap.readPixelRGB(i,j).G8();
        colValB = spriteMap.readPixelRGB(i,j).B8();

        // Serial.print(colValR);
        // Serial.print(":");
        // Serial.print(colValG);
        // Serial.print(":");
        // Serial.print(colValB);
        // Serial.println(":");

  //16ビットRGB（24ビットRGB）
        if(colValR==0&&colValG==0&&colValB==0){//0: 黒色=なし
          mapArray[i][k] = mapsprnos[0];//20;
        }else if(colValR==24&&colValG==40&&colValB==82){//{ 27,42,86 },//1: 暗い青色
          mapArray[i][k] = mapsprnos[1];//11;//5*8+5;
        }else if(colValR==140&&colValG==24&&colValB==82){//{ 137,24,84 },//2: 暗い紫色
          mapArray[i][k] = mapsprnos[2];//32;//5*8+5;
        }else if(colValR==0&&colValG==138&&colValB==74){//{ 0,139,75 },//3: 暗い緑色
          mapArray[i][k] = mapsprnos[3];//44;//5*8+5;
        }else if(colValR==181&&colValG==77&&colValB==41){//{ 183,76,45 },//4: 茶色 
          mapArray[i][k] = mapsprnos[4];//53;//5*8+5;
        }else if(colValR==99&&colValG==85&&colValB==74){//{ 97,87,78 },//5: 暗い灰色
          mapArray[i][k] = mapsprnos[5];//49;
        }else if(colValR==198&&colValG==195&&colValB==198){//{ 194,195,199 },//6: 明るい灰色
          mapArray[i][k] = mapsprnos[6];//54;//5*8+5;
        }else if(colValR==255&&colValG==243&&colValB==231){//{ 255,241,231 },//7: 白色
          mapArray[i][k] = mapsprnos[7];//32;
        }else if(colValR==255&&colValG==0&&colValB==66){//{ 255,0,70 },//8: 赤色
          mapArray[i][k] = mapsprnos[8];//52;
        }else if(colValR==255&&colValG==162&&colValB==0){//{ 255,160,0 },//9: オレンジ
          mapArray[i][k] = mapsprnos[9];//41;//5*8+5;
        }else if(colValR==255&&colValG==239&&colValB==0){//{ 255,238,0 },//10: 黄色
          mapArray[i][k] = mapsprnos[10];//46;
        }else if(colValR==0&&colValG==235&&colValB==0){//{ 0,234,0 },//11: 緑色
          mapArray[i][k] = mapsprnos[11];//42;
        }else if(colValR==0&&colValG==174&&colValB==255){//{ 0,173,255 },//12: 水色
          mapArray[i][k] = mapsprnos[12];//45;//5*8+5;
        }else if(colValR==132&&colValG==117&&colValB==156){//{ 134,116,159 },//13: 藍色
          mapArray[i][k] = mapsprnos[13];//50;
        }else if(colValR==255&&colValG==105&&colValB==173){//{ 255,107,169 },//14: ピンク
          mapArray[i][k] = mapsprnos[14];//43;//5*8+5;
        }else if(colValR==255&&colValG==203&&colValB==165){//{ 255,202,165}//15: 桃色
          mapArray[i][k] = mapsprnos[15];//38;//5*8+5;
        }

    // Serial.print(mapArray[i][k]);
    //       Serial.print(":");

        if(i==MAPWH-1 && k==MAPWH-1){mapready = true;return 1;}//読み込み終わったら1をリターン
      }
    }
  }
  // spriteMap.deleteSprite();//メモリに格納したら解放する
  return 1;
}

using namespace std;
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <cmath>

using namespace std;

vector<string> split(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

void setTFTedit(int _iseditmode){
  if(_iseditmode ==TFT_RUN_MODE){
    tft.setPsram( false );//DMA利用のためPSRAMは切る
    tft.createSprite( TFT_WIDTH, TFT_HEIGHT );//PSRAMを使わないギリギリ
    tft.startWrite();//CSアサート開始
  }else if(_iseditmode == TFT_EDIT_MODE){
    tft.setPsram( false );//DMA利用のためPSRAMは切る
    tft.createSprite( TFT_WIDTH, TFT_HEIGHT );
    tft.startWrite();//CSアサート開始
  }
  else if(_iseditmode == TFT_WIFI_MODE){
    tft.setPsram( false );//DMA利用のためPSRAMは切る
    tft.createSprite( TFT_WIDTH, TFT_HEIGHT );
    tft.startWrite();//CSアサート開始
  }
}

// int btn(int btnno)
// {
//   return buttonState[btnno];
// }

void reboot(String _fileName, int _isEditMode)
{
  setOpenConfig(_fileName, _isEditMode);
  // editor.setCursorConfig();//カーソルの位置を保存
  delay(100);
  ESP.restart();
}

void restart(String _fileName, int _isEditMode)
{
  setOpenConfig(_fileName, _isEditMode);
  
  // editor.setCursorConfig();//カーソルの位置を保存
  // delay(100);

  firstBootF = false;
  setup();

  // tunes.pause();
  game->pause();
  free(game);
  firstLoopF = true;
  toneflag = false;
  sfxflag = false;
  musicflag = false;
  // txtName = _fileName;
  game = nextGameObject(&_fileName, gameState, mapFileName);//ファイルの種類を判別して適したゲームオブジェクトを生成
  game->init();//resume()（再開処理）を呼び出し、ゲームで利用する関数などを準備
  // tunes.resume();
}

uint8_t readpixel(int i, int j)
{
        // int k = j+(MAPWH/divnum)*(n);//マップ下部
        colValR = sprite64.readPixelRGB(i,j).R8();
        colValG = sprite64.readPixelRGB(i,j).G8();
        colValB = sprite64.readPixelRGB(i,j).B8();

  //16ビットRGB（24ビットRGB）
        if(colValR==0&&colValG==0&&colValB==0){//0: 黒色=なし
          return 0;//20;
        }else if(colValR==24&&colValG==40&&colValB==82){//{ 27,42,86 },//1: 暗い青色
          return 1;//11;//5*8+5;
        }else if(colValR==140&&colValG==24&&colValB==82){//{ 137,24,84 },//2: 暗い紫色
          return 2;//32;//5*8+5;
        }else if(colValR==0&&colValG==138&&colValB==74){//{ 0,139,75 },//3: 暗い緑色
          return 3;//44;//5*8+5;
        }else if(colValR==181&&colValG==77&&colValB==41){//{ 183,76,45 },//4: 茶色 
          return 4;//53;//5*8+5;
        }else if(colValR==99&&colValG==85&&colValB==74){//{ 97,87,78 },//5: 暗い灰色
          return 5;//49;
        }else if(colValR==198&&colValG==195&&colValB==198){//{ 194,195,199 },//6: 明るい灰色
          return 6;//54;//5*8+5;
        }else if(colValR==255&&colValG==243&&colValB==231){//{ 255,241,231 },//7: 白色
          return 7;//32;
        }else if(colValR==255&&colValG==0&&colValB==66){//{ 255,0,70 },//8: 赤色
          return 8;//52;
        }else if(colValR==255&&colValG==162&&colValB==0){//{ 255,160,0 },//9: オレンジ
          return 9;//41;//5*8+5;
        }else if(colValR==255&&colValG==239&&colValB==0){//{ 255,238,0 },//10: 黄色
          return 10;//46;
        }else if(colValR==0&&colValG==235&&colValB==0){//{ 0,234,0 },//11: 緑色
          return 11;//42;
        }else if(colValR==0&&colValG==174&&colValB==255){//{ 0,173,255 },//12: 水色
          return 12;//45;//5*8+5;
        }else if(colValR==132&&colValG==117&&colValB==156){//{ 134,116,159 },//13: 藍色
          return 13;//50;
        }else if(colValR==255&&colValG==105&&colValB==173){//{ 255,107,169 },//14: ピンク
          return 14;//43;//5*8+5;
        }else if(colValR==255&&colValG==203&&colValB==165){//{ 255,202,165}//15: 桃色
          return 15;//38;//5*8+5;
        }
}

size_t cursor_index = 0;


void safeReboot(){
  // editor.setCursorConfig(0,0,0);//カーソルの位置を強制リセット保存
      delay(50);

      ui.setConstantGetF(false);//初期化処理 タッチポイントの常時取得を切る
      appfileName = "/init/main.lua";
      
      firstLoopF = true;
      toneflag = false;
      sfxflag = false;
      musicflag = false;

      // editor.editorSave(SPIFFS);//SPIFFSに保存
      delay(100);//ちょっと待つ
      reboot(appfileName, TFT_RUN_MODE);//現状rebootしないと初期化が完全にできない
}

void debugInfo()
{
  tft.setFont(&fonts::Font0);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setCursor(90,111);
  if(patternNo<10)tft.print("0");
  tft.print(patternNo);

  tft.setCursor(0, 101);
  tft.print(ui.getPhVol(0)); 
  tft.print(":"); 
  tft.print(ui.getPhVol(1));
  tft.print("[");
  tft.print(ui.getPhVol(2)); 
  tft.print("]");

  tft.setCursor(110, 111);
  tft.print(masterVol);
  int vw=2;
  int vh=1;
  for(int vx=0; vx<16; vx++){
    if(masterVol > 16*vx){
      tft.fillRect(120+vx*vw, 113+(7-vx/2), vw, vx/2, gethaco3Col(8));
    }
  }

  tft.setCursor(0,111);
  tft.print(pressedBtnID);

  tft.print("[");
  tft.print(ui.getPos().x/2);
  tft.print(":");
  tft.print(ui.getPos().y/2);
  tft.print("]");
}

void showMusicInfo()
{
  tft.setTextSize(1);
  screen.setTextColor(TFT_DARKGRAY, TFT_BLACK);
tft.setCursor(120,1);
    if(patternNo<10)tft.print("0");
    tft.print(patternNo);

    tft.setCursor(148,1);
    // tft.print("SP");
    if(tickSpeed<10)tft.print("0");
    tft.print(tickSpeed);
    
tft.drawLine(120, 9, 160, 9, gethaco3Col(6));

    // tft.fillRect(120, 10, 32, 20, gethaco3Col(13));
    tft.setCursor(120,11);
    for(int n = 0;n<4;n++){tft.print(channels->patterns[patternNo][n]);}
    tft.setCursor(120,21);
    for(int n = 0;n<4;n++){tft.print(channels->patterns[patternNo][4+n]);}

tft.drawLine(120,29, 160,29, gethaco3Col(6));

    // tft.fillRect(138, 30, 32, 40, gethaco3Col(2));
    tft.setCursor(138,31);
    tft.print("IN");
    tft.print(instrument);
    tft.fillRect(120, 30, 10, 9, gethaco3Col(instrument+4));

tft.drawLine(120, 39, 160, 39, gethaco3Col(6));

    tft.setCursor(138,41);
    tft.print(" ");
    // tft.print(instrument);
    // tft.fillRect(120, 40, 10, 9, gethaco3Col(instrument+8));

tft.drawLine(120, 49, 160, 49, gethaco3Col(6));

    tft.setCursor(138,51);
    tft.print("CH");
    tft.print(targetChannelNo);
    tft.fillRect(120, 50, 10, 9, gethaco3Col(targetChannelNo));

tft.drawLine(120, 59, 160,59, gethaco3Col(6));

    tft.setCursor(138,61);
    tft.print("EF");
    tft.print(gEfectNo);
    tft.fillRect(120, 60, 10, 9, gethaco3Col(gEfectNo));

tft.drawLine(120, 69, 160,69, gethaco3Col(6));
    int vw=2;
    int vh=1;
    for(int vx=0; vx<16; vx++){
      if(masterVol > 16*vx){
        tft.fillRect(120+vx*vw, 71+(7-vx/2), vw, vx/2, gethaco3Col(8));
      }
    }
    tft.setCursor(120, 71);
    tft.print(masterVol);
tft.drawLine(120, 79, 160,79, gethaco3Col(6));

    tft.setCursor(120, 81);
    tft.print(hitvalue);
tft.drawLine(120, 89, 160,89, gethaco3Col(6));

    tft.setCursor(120, 91);
    tft.print(pressedBtnID);
tft.drawLine(120, 99, 160,99, gethaco3Col(6));

    tft.setCursor(0, 91);
    tft.print(ui.getPhVol(0)); 
    tft.print(":"); 
    tft.print(ui.getPhVol(1));
tft.drawLine(0, 99, 0,99, gethaco3Col(6));
    tft.setCursor(0, 101);
    tft.print(ui.getPhVol(2)); 
tft.drawLine(0, 109, 0,109, gethaco3Col(6));

}

void controlMusicVisual(){
      for(int i = 0; i<32; i++)//32音書き換える
      {
        uint8_t efn = channels->notedata[targetChannelNo][i].effectNo;
        uint8_t ins = channels->notedata[targetChannelNo][i].instrument;
        uint8_t vol = channels->notedata[targetChannelNo][i].volume;
        uint8_t oct = channels->notedata[targetChannelNo][i].octave;
        uint8_t pit = channels->notedata[targetChannelNo][i].pitch;

        // if (ui.getEvent() != NO_EVENT)
        // {
        //   if((ui.getPos().x-40)>>2==i){//押した1音だけの音色を変える
        //     if(96>ui.getPos().y && 0<=ui.getPos().y){ //上の領域で
        //       //音程を変える
        //       pit = (96 - ui.getPos().y)>>2 % 12;
        //       oct = 4;//floor((96 - ui.getPos().y)>>2 / 12)+3;
        //       //音色を変える
        //       ins = instrument;
        //       //エフェクトを変える
        //       efn = gEfectNo;
        //     }
            
        //     if(128>ui.getPos().y && 96<=ui.getPos().y){//下の領域で
        //       //音量を変える
        //       vol = (124 - ui.getPos().y)>>2 % 8;
        //     }
        //     buffAreaNo = patternNo%2;
        //     channels->resetTones(i, targetChannelNo, pit, oct, vol, ins, efn, buffAreaNo);//ピッチ,オクターブなどトーンを置き換える
        //   }
        // }

        if(toneTickNo == i){

          tft.fillRect(i*4, 92-(pit + (oct-4)*12)*4, 4,4, gethaco3Col(10));
          tft.fillRect(i*4, 124-(vol)*4, 4,4, gethaco3Col(10));

        }else{
          if(efn!=0)tft.drawLine(i*4, 92-(pit + (oct-4)*12)*4+3, i*4+3, 92-(pit + (oct-4)*12)*4+3, gethaco3Col(efn+8));

          tft.fillRect(i*4, 92-(pit + (oct-4)*12)*4, 4,3, gethaco3Col(ins+4));
          tft.fillRect(i*4, 124-(vol)*4, 4,4, gethaco3Col(3));
        }

      }
}

void showMusicVisual(){//操作できないタイプ
    for(int i = 0; i<32; i++)//32音書き換える
    {
      uint8_t bufNo = patternNo%2;
      uint8_t efn = channels->notedata[targetChannelNo][i+bufNo*32].effectNo;
      uint8_t ins = channels->notedata[targetChannelNo][i+bufNo*32].instrument;
      uint8_t vol = channels->notedata[targetChannelNo][i+bufNo*32].volume;
      uint8_t oct = channels->notedata[targetChannelNo][i+bufNo*32].octave;
      uint8_t pit = channels->notedata[targetChannelNo][i+bufNo*32].pitch;

      if(toneTickNo == i){
        tft.fillRect(i*4, 92-(pit + (oct-4)*12)*4, 4,4, gethaco3Col(10));
        tft.fillRect(i*4, 124-(vol)*4, 4,4, gethaco3Col(10));
      }else{
        if(efn!=0)tft.drawLine(i*4, 92-(pit + (oct-4)*12)*4+3, i*4+3, 92-(pit + (oct-4)*12)*4+3, gethaco3Col(efn+8));

        tft.fillRect(i*4, 92-(pit + (oct-4)*12)*4, 4,3, gethaco3Col(ins+4));
        tft.fillRect(i*4, 124-(vol)*4, 4,4, gethaco3Col(3));
      }
    }
}


void showMusicVisual8ch(){//操作できないタイプ

for(uint8_t chx = 0; chx<2; chx++)
{
  for(uint8_t chy = 0; chy<4; chy++)
  {
    if(chx*4+chy == targetChannelNo){
      tft.drawRect(chx*64, chy*32, 64,32, gethaco3Col(8));
    }

    //バッファ表示
    for(int i = 0; i<32; i++)//32音書き換える
    {
      uint8_t bufNo = (patternNo+1)%2;
      uint8_t efn = channels->notedata[chx*4+chy][i+bufNo*32].effectNo;
      uint8_t ins = channels->notedata[chx*4+chy][i+bufNo*32].instrument;
      uint8_t vol = channels->notedata[chx*4+chy][i+bufNo*32].volume;
      uint8_t oct = channels->notedata[chx*4+chy][i+bufNo*32].octave;
      uint8_t pit = channels->notedata[chx*4+chy][i+bufNo*32].pitch;

      if(efn!=0)
      tft.drawPixel(i*2+(64*chx)+1,(23-(pit + (oct-4)*12))+(32*chy), gethaco3Col(5));

      tft.drawPixel(i*2+(64*chx),  (23-(pit + (oct-4)*12))+(32*chy), gethaco3Col(5));
      tft.drawPixel(i*2+(64*chx),  (31-(vol))+(32*chy), gethaco3Col(5));
      
    }

    //現在の音データ表示
    for(int i = 0; i<32; i++)//32音書き換える
    {
      uint8_t bufNo = patternNo%2;
      uint8_t efn = channels->notedata[chx*4+chy][i+bufNo*32].effectNo;
      uint8_t ins = channels->notedata[chx*4+chy][i+bufNo*32].instrument;
      uint8_t vol = channels->notedata[chx*4+chy][i+bufNo*32].volume;
      uint8_t oct = channels->notedata[chx*4+chy][i+bufNo*32].octave;
      uint8_t pit = channels->notedata[chx*4+chy][i+bufNo*32].pitch;

      if(toneTickNo == i){
        tft.drawPixel(i*2+(64*chx),  (23-(pit + (oct-4)*12))+(32*chy), gethaco3Col(10));
        tft.drawPixel(i*2+(64*chx),  (31-(vol))+(32*chy), gethaco3Col(10));
      }else{
        if(efn!=0)
        tft.drawPixel(i*2+(64*chx)+1,(23-(pit + (oct-4)*12))+(32*chy), gethaco3Col(efn+8));

        tft.drawPixel(i*2+(64*chx),  (23-(pit + (oct-4)*12))+(32*chy), gethaco3Col(ins+4));
        tft.drawPixel(i*2+(64*chx),  (31-(vol))+(32*chy), gethaco3Col(3));
      }
    }
      
    }
  }
}

bool createChannels() {
    uint8_t addTones[8];
    String line;
    int j = 0;

    String filePath = "/init/sound/patterns/" + String(musicNo) + ".csv";

    // ファイルが存在するかを確認する
    if (!SPIFFS.exists(filePath)) {
        // ファイルが存在しない場合の処理
        Serial.println("File does not exist");
        musicNo = 255;
        return false; // ファイルが存在しない場合は false を返す
    }

    // ファイルが存在する場合は開く
    File fr = SPIFFS.open(filePath, "r");

    if (!fr) { // ファイルのオープンに失敗した場合の処理
        Serial.println("Failed to open patterns.csv");
        return false; // ファイルのオープンに失敗した場合は false を返す
    }

    // 64行まで読み込む
    while (fr.available() && j < 64) {
        line = fr.readStringUntil('\n'); // 1行読み込む
        line.trim(); // 空白を削除

        if (!line.isEmpty()) {
            int commaIndex = line.indexOf(',');
            if (commaIndex != -1) {
                String val = line.substring(0, commaIndex);
                addTones[0] = val.toInt();

                for (int i = 1; i < 8; i++) {
                    int nextCommaIndex = line.indexOf(',', commaIndex + 1);
                    if (nextCommaIndex != -1) {
                        val = line.substring(commaIndex + 1, nextCommaIndex);
                        addTones[i] = val.toInt();
                        commaIndex = nextCommaIndex;
                    } else {
                        // 最後の値の場合
                        val = line.substring(commaIndex + 1);
                        addTones[i] = val.toInt();
                        break; // 行の終わりに達したらループを抜ける
                    }
                }

                // パターンを設定
                for (uint8_t n = 0; n < CHANNEL_NUM; n++) {
                    channels->setPatterns(j, n, addTones[n]);
                }

                j++; // 行数をカウント
            }
        }
    }

    fr.close();

    // 読み込んだ行数を出力（デバッグ用）
    // Serial.print("Read ");
    // Serial.print(j);
    // Serial.println(" lines from patterns.csv");

    // すべてが終わったら true を返す
    return true;
}

uint8_t sfxlistNo = 0;
uint8_t sfxnos[8] ={0,1,2,3,4,5,6,7};
void readsfxlist() {
  // 読み込む効果音を外部ファイルを使い指定する
  File fr = SPIFFS.open("/init/sfxes/sfxlist"+String(sfxlistNo)+".txt", "r");
  String line;
  int index = 0; // インデックスを追加
  while (fr.available()) {
    line = fr.readStringUntil('\n');
    if (!line.isEmpty()) {
      int commaIndex = line.indexOf(',');
      String val = line.substring(0, commaIndex);
      if (val.toInt() != 0) { // 0 でないことを確認
        sfxnos[index] = val.toInt(); // インデックスを使用して sfxnos 配列に値を代入
        index++; // インデックスをインクリメント
        if (index >= 8) break; // sfxnos 配列がオーバーフローしないようにする
      }
    }
  }
  fr.close();
}


bool readsfx()//行32列9のデータ
{
  //読み込む効果音を外部ファイルを使い指定する
  readsfxlist();
  // トーンファイルを読み込む
  File fr;
  for (int sfxno = 0; sfxno < SFX_NUM; sfxno++)
    {
    uint8_t addTones[32];
    String line;
    int j = 0;

    fr = SPIFFS.open("/init/sfxes/"+String(sfxnos[sfxno])+".csv", "r");
    if (!fr)
    {
      Serial.println("Failed to open tones.csv");
      return true;//とりあえず進む
    }
    while (fr.available())
    {
      line = fr.readStringUntil('\n');
      // line.trim(); // 空白を削除
      if (!line.isEmpty())
      {
        int commaIndex = line.indexOf(',');
        if (commaIndex != -1)
        {
          String val = line.substring(0, commaIndex);//一個目はここで読み込む

          addTones[0] = val.toInt();
          if(j==0)channels->sfxdata[sfxno][0].onoffF = addTones[0];
          else if(j==1)channels->sfxdata[sfxno][0].instrument = addTones[0];//pitch
          else if(j==2)channels->sfxdata[sfxno][0].pitch = addTones[0];
          else if(j==3){
            channels->sfxdata[sfxno][0].octave = addTones[0];
            channels->sfxdata[sfxno][0].hz = channels->calculateFrequency(
              channels->sfxdata[sfxno][0].pitch, 
              channels->sfxdata[sfxno][0].octave);
          }
          else if(j==4)channels->sfxdata[sfxno][0].sfxno = addTones[0];
          else if(j==5)channels->sfxdata[sfxno][0].volume = addTones[0];

          for (int i = 1; i < 32; i++)
          {

            int nextCommaIndex = line.indexOf(',', commaIndex + 1);
            if (nextCommaIndex != -1)
            {
              val = line.substring(commaIndex + 1, nextCommaIndex);
              addTones[i] = val.toInt();
              commaIndex = nextCommaIndex;

              if(j==0)channels->sfxdata[sfxno][i].onoffF = addTones[i];
              else if(j==1)channels->sfxdata[sfxno][i].instrument = addTones[i];//pitch
              else if(j==2)channels->sfxdata[sfxno][i].pitch = addTones[i];
              else if(j==3){
                channels->sfxdata[sfxno][i].octave = addTones[i];
                channels->sfxdata[sfxno][i].hz = channels->calculateFrequency(
                  channels->sfxdata[sfxno][i].pitch, 
                  channels->sfxdata[sfxno][i].octave);
              }
              else if(j==4)channels->sfxdata[sfxno][i].sfxno = addTones[i];
              else if(j==5)channels->sfxdata[sfxno][i].volume = addTones[i];
            }
          }
          j++;
        }
      }
    }
  }
  fr.close();

  //すべてが終わったらtrueを返す
  return true;
}

void fillNotedataWithDummyValues(ChannelData notedata[CHANNEL_NUM][TONE_NUM * 2]) {
    for (int chno = 0; chno < CHANNEL_NUM; chno++) {
        for (int i = 0; i < TONE_NUM * 2; i++) {
            notedata[chno][i].onoffF = true; // ノートを有効にする
            notedata[chno][i].hz = 440 + (i * 20.0f);  // A4の周波数 (440 Hz)
            notedata[chno][i].pitch = 9;    // A (ラ)
            notedata[chno][i].octave = 4;   // 4オクターブ
            notedata[chno][i].sfxno = 0;    // サウンドエフェクト番号
            notedata[chno][i].instrument = 0; // 楽器番号
            notedata[chno][i].volume = 127; // 音量 (0-127)
            notedata[chno][i].effectNo = 0; // エフェクト番号
            notedata[chno][i].tickNo = i;   // ティック番号
        }
    }
    
}

#include <cstring> // strtok用

// 定数定義
#define CHANNEL_NUM 8
#define TONE_NUM 32
bool readTones(uint8_t _patternNo) {
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to initialize SPIFFS.");
        return false;
    }

    // トーンファイルを読み込む
    for (int chno = 0; chno < CHANNEL_NUM; chno++) {
        uint8_t patternID = channels->getPatternID(_patternNo, chno);
        String filePath = "/init/tones/" + String(patternID) + ".csv";

        // ファイルが存在するかを確認する
        if (!SPIFFS.exists(filePath)) {
            Serial.println("File does not exist: " + filePath);
            return false;
        }

        // ファイルを開く
        File fr = SPIFFS.open(filePath, "r");
        if (!fr) {
            Serial.println("Failed to open file: " + filePath);
            return false;
        }

        // ファイルを1行ずつ読み込む
        char line[128]; // 適切なサイズを指定
        int j = 0;
        while (fr.available()) {
            fr.readBytesUntil('\n', line, sizeof(line));
            line[strcspn(line, "\r\n")] = 0; // 改行文字を削除

            if (strlen(line) > 0) {
                char *token = strtok(line, ",");
                int i = 0;
                while (token != NULL) {
                    uint8_t value = atoi(token); // 文字列を整数に変換

                    uint8_t bufNo = (_patternNo + 1) % 2;

                    // データをnotedataに格納
                    if (j == 0) channels->notedata[chno][i + bufNo * TONE_NUM].onoffF = value;
                    else if (j == 1) channels->notedata[chno][i + bufNo * TONE_NUM].instrument = value;
                    else if (j == 2) channels->notedata[chno][i + bufNo * TONE_NUM].pitch = value;
                    else if (j == 3) {
                        channels->notedata[chno][i + bufNo * TONE_NUM].octave = value;
                        channels->notedata[chno][i + bufNo * TONE_NUM].hz = channels->calculateFrequency(
                            channels->notedata[chno][i + bufNo * TONE_NUM].pitch,
                            channels->notedata[chno][i + bufNo * TONE_NUM].octave
                        );
                    } else if (j == 4) channels->notedata[chno][i + bufNo * TONE_NUM].sfxno = value;
                    else if (j == 5) channels->notedata[chno][i + bufNo * TONE_NUM].volume = value;

                    token = strtok(NULL, ",");
                    i++;
                }
                j++;
            }
        }
        fr.close();
        // Serial.println("Finished reading tones for channel: " + String(chno));
    }

    // すべての読み込みが成功した場合に true を返す
    // Serial.println("Successfully read tones for all channels.");
    return true;
}

int prepatternNo = 63;

void createChTask(void *pvParameters) {
    while (true) {
      if(patternNo!=prepatternNo){
        while (!createChannels()) {
            delay(10);
        }
        sndbufferedF = readTones(patternNo);
        prepatternNo = patternNo;
      }
      xSemaphoreGive(syncSemaphore);
      delay(10);
    }
}

void sfxTask(void *pvParameters) {
  while (true) {
if(sfxflag==true){
      channels->begin();
      channels->setVolume(masterVol); // 0-255

      for(int n=0;n < 32;n++){
        sfxTickNo = n;
        channels->sfx(sfxChNo, sfxNo, wavNo, sfxVol, sfxspeed);
      }
      channels->stop();

      if(sfxTickNo == 31){sfxflag=false;sfxTickNo=0;}
    }
        delay(1);
  }
}

void musicTask(void *pvParameters) {
  while (true) {
    
    // if(musicflag){
    // 何らかの条件が満たされるまで待機
    if(sndbufferedF){//次の音がバッファされたら
      if (xSemaphoreTake(syncSemaphore, portMAX_DELAY)) {
      // 同期が取れたらここに入る
      channels->begin();
      channels->setVolume(masterVol); // 0-255
      channels->note(0, toneTickNo, patternNo+loopStart);
      channels->note(1, toneTickNo, patternNo+loopStart);
      channels->note(2, toneTickNo, patternNo+loopStart);
      channels->note(3, toneTickNo, patternNo+loopStart);       
      channels->note(4, toneTickNo, patternNo+loopStart);
      channels->note(5, toneTickNo, patternNo+loopStart);
      
      // channels->note(6, toneTickNo, patternNo+loopStart);
      // channels->note(7, toneTickNo, patternNo+loopStart);   

      channels->stop();
      
      xSemaphoreGive(syncSemaphore);
      }
    }
    // }

    //効果音は別にならす(ひとまず同発2ch)
    

    toneTickNo++;
    toneTickNo %= TONE_NUM;

    if (toneTickNo == 0) {
        patternNo++;

        // if (patternNo >= PATTERN_NUM) {
        //     patternNo = 0;
        // }
        if (patternNo <= loopStart) {//現在の進行パタンがスタート位置より手前なら
            patternNo = loopStart;
        }else if (patternNo > loopEnd) {//現在の進行パタンがエンド位置より後ろなら
            patternNo = loopStart;
        }
    }
    delay(1);
  }
  // 他の処理や適切な待機時間をここに追加
  // delay(10);

}
void sendFileToSerial(const char *path) { 
    File file = SPIFFS.open(path, FILE_READ);
    if (!file) {
        Serial.println("ERROR: Failed to open file");
        return;
    }

    String line;  // 1行ずつ格納するための変数

    while (file.available()) {
        // ファイルから1行ずつ読み取る
        line = file.readStringUntil('\n');

        // バイナリデータとして送信
        uint8_t checksum = 0;
        uint8_t* buffer = (uint8_t*)line.c_str();  // lineをバイト配列に変換

        // チェックサム計算
        for (size_t i = 0; i < line.length(); i++) {
            checksum ^= buffer[i];
        }

        bool success = false;
        int retryCount = 0;
        const int MAX_RETRIES = 3;  // 最大3回まで再送する

        while (!success && retryCount < MAX_RETRIES) {
            // バイナリデータ送信
            Serial.write(buffer, line.length());
            Serial.print('\n'); // 最後が1バイト文字になるように追加
            Serial.print('|');
            Serial.printf("%02X", checksum);
            Serial.print('^');
            Serial.flush();  // バッファを完全に送信

            // PC側からのレスポンス待ち
            String response = "";
            unsigned long start = millis();
            while (millis() - start < 3000) {  // 3秒以内にレスポンスが来ることを期待
                while (Serial.available()) {
                    char c = Serial.read();
                    if (c == '\n') break;
                    response += c;
                }
                response.trim();
                if (response == "OK") {
                    success = true;
                    break;
                }
                if (response == "ERROR") {
                    retryCount++;
                    Serial.println("ERROR: Checksum mismatch, retrying...");
                    break;
                }
                delay(10);
            }
        }

        if (!success) {
            Serial.println("ERROR: Failed to send data after retries.");
            break;  // 3回失敗したら送信を中断
        }
    }

    // 終了トリガーとして---EOF---を送信
    Serial.write("EOF|00^");  // バイナリデータとして送信(パース用の形式を守る)
    Serial.flush();  // バッファを完全に送信
    file.close();
}


// writeコマンド処理（ファイルのバックアップを作成してから書き換える）
void handleWriteCommand(const char *path, const String &newContent) {
  // バックアップファイルの作成
  String backupPath = String(path) + ".bak";
  File backupFile = SPIFFS.open(backupPath.c_str(), FILE_WRITE);
  if (!backupFile) {
    Serial.println("バックアップファイルを作成できませんでした");
    return;
  }

  // 元のファイルを読み込みながらバックアップ作成
  File originalFile = SPIFFS.open(path, FILE_READ);
  if (!originalFile) {
    Serial.println("元のファイルを開けませんでした");
    backupFile.close();
    return;
  }

  while (originalFile.available()) {
    backupFile.write(originalFile.read());
  }

  originalFile.close();
  backupFile.close();
  Serial.println("バックアップが完了しました");

  // 新しいコンテンツでファイルを開く（書き換え）
  File file = SPIFFS.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("ファイルを開けませんでした");
    return;
  }

  // 新しいコンテンツを書き込む
  file.print(newContent);
  file.close();
  Serial.println("ファイルの書き換えが完了しました");
}


// シリアルから1行ずつ受け取り、ファイルに追記する関数
// void appendToFile(String line) {
//     if (line.length() == 0) {
//         return;
//     }

//     // ファイルを開き、1行を追記
//     File file = SPIFFS.open(fileName.c_str(), FILE_APPEND);
//     if (file) {
//         file.println(line);
//         file.close();
//         Serial.println("Line written to file.");
//     } else {
//         Serial.println("Failed to open file for appending.");
//     }
// }

// ファイル書き込みが終了したら、再び待機状態に戻る
// void endFileWrite() {
//     writingToFile = false;
//     Serial.println("File write completed.");
// }

String wfileName = "";
bool writingToFile = false;
// シリアルデータ受信タスク
void serialReceiveTask(void *parameter) {
  while (true) {
    if (Serial.available()) {
      // シリアルデータの受信処理
      String command = Serial.readStringUntil('\n');
      command.trim();
      // Serial.println(command);

      // 改行コードを統一（Windowsや古いMacの改行コードを処理）
      command.replace("\r\n", "\n");  // Windowsの改行コードを \n に変換
      command.replace("\r", "\n");    // 古いMacの改行コードを \n に変換

      // コマンドが "read <ファイル名>" の場合
      if (command.startsWith("read ")) {
        String filePath = command.substring(5); // "read "を除去してファイル名を取得
        sendFileToSerial(filePath.c_str()); // ファイルを読み取ってシリアルに送信
      } // コマンドが "write <ファイル名> <新しい内容>" の場合
      else if (command.startsWith("write ")) {
        // "write " を除去し、ファイル名を取得
        wfileName = command.substring(6);
        // writeToFile(wfileName);
        // // command = command.substring(6); // "write " を除去
        // String fileName = command.substring(6);// "write " を除去

        // // **強制的に固定の Lua スクリプトを使用**
        // String fileContent = 
        //     "bgcNo = 9\n"
        //     "size = 15\n"
        //     "x=64\n"
        //     "y=60\n"
        //     "function setup()\n"
        //     "  fillrect(20,20,80,80,7)\n"
        //     "end\n"
        //     "\n"
        //     "function _init()--1回だけ\n"
        //     "    setup()\n"
        //     "end\n"
        //     "\n"
        //     "function _update()--ループします\n"
        //     "end\n"
        //     "\n"
        //     "function _draw()--ループします\n"
        //     "  cls(bgcNo)\n"
        //     "  fillrect(x,y,size,size,8)\n"
        //     "  spr8(1,x,y)\n"
        //     "end\n";

        // // Serial.printf("Writing to file: %s\n", fileName.c_str());

        // // File file = SPIFFS.open((fileName).c_str(), FILE_WRITE);
        // SPIFFS.remove(fileName.c_str());// 既存ファイルを削除
        // File file = SPIFFS.open(fileName.c_str(), FILE_WRITE);
        // // File file = SPIFFS.open(fileName.c_str(), "w"); // ファイルを書き込みモードで開く

        // file.println(fileContent);  // ファイルに書き込み
        // file.close();
 
        // Serial.println("File write completed.");
        // reboot(fileName, TFT_RUN_MODE);
    
      // }else if (writingToFile) {
      //       // ファイル書き込みモードならシリアルから受け取ったデータをファイルに追加
      //       appendToFile(command);
      } 
      // 他のコマンドに対応する処理（例: "status" コマンド）
      else if (command.startsWith("list")) {
        String filePath = command.substring(5); // "list "を除去してファイル名を取得
        File root = SPIFFS.open("/");
        File file = root.openNextFile();
        Serial.print("[LIST]");  // 識別用プレフィックス
        while (file) {
            Serial.print(file.name());
            Serial.print("\n");
            file = root.openNextFile();
        }
        // 終了トリガーとして---EOF---を送信
        Serial.write("EOF|00^");  // バイナリデータとして送信(パース用の形式を守る)
        Serial.flush();  // バッファを完全に送信
      }
      // 他のコマンドに対応する処理（例: "status" コマンド）
      else if (command == "status") {
        Serial.println("システム状態: OK"); // システムのステータス情報を送信
      }
      // 無効なコマンドの処理
      else {
        Serial.println("無効なコマンドです。'read <ファイル名>'または'status'を入力してください。");
      }
    }
    delay(10);  // 小さな遅延を入れることでCPUの過剰な消費を避ける
  }
}

void setup()
{
  channels = new Channel();//Hzリストを生成

  
  pinMode(OUTPIN_0, OUTPUT);
  pinMode(INPIN_0, INPUT);

  Serial.begin(115200);
  // editor.getCursorConfig("/init/param/editor.txt");//エディタカーソルの位置をよみこむ

  delay(50);
  if(firstBootF == true){
    difffileF = false;

    #if !defined(__MIPSEL__)
      while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
      #endif
      Serial.println("Keyboard Start");

    if (!SPIFFS.begin(true))
    {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  }

  // readsfx();

  // 同期用セマフォの作成
  syncSemaphore = xSemaphoreCreateBinary();
  nextFrameSemaphore = xSemaphoreCreateBinary();

  // createChannelsTask タスクの作成（コア1で実行）
  xTaskCreatePinnedToCore(
    createChTask,
    "createChTask",
    8192,//~3000だと非力だけど動く//2048,4096なら動く//1024だと動かない
    NULL,
    2,
    &taskHandle[0],  // タスクハンドルを取得
    1//
  );

  // musicTask タスクの作成（コア1で実行）
  xTaskCreatePinnedToCore(
    musicTask,
    "musicTask",
    4096,////1024だと動く//1500だと非力だけど動く//2048だと動く
    NULL,
    3,
    &taskHandle[1],//NULL,// タスクハンドルを取得
    1// タスクを実行するコア（0または1）
  );

  // sfxTask タスクの作成（コア1で実行）
  xTaskCreatePinnedToCore(
    sfxTask,
    "sfxTask",
    4096,////1024だと動く//1500だと非力だけど動く//2048だと動く
    NULL,
    4,
    &taskHandle[2],//NULL,// タスクハンドルを取得
    0 // タスクを実行するコア（0または1）
  );

  // シリアルデータ受信タスクの作成（コア1で実行）
  xTaskCreatePinnedToCore(
    serialReceiveTask,
    "serialReceiveTask",
    2048,  // サイズ
    NULL,
    5,     // 優先度
    &taskHandle[3],
    1      // コア1で実行
  );

  getOpenConfig("/init/param/openconfig.txt");//最初に立ち上げるゲームのパスとモードをSPIFFSのファイルopenconfig.txtから読み込む

  if(isEditMode == TFT_RUN_MODE){

    if(firstBootF == false){
      tft.deleteSprite();
      delay(100);
    }
    setTFTedit(TFT_RUN_MODE);

    // ui.begin( screen, 16, 0, 1);
    ui.begin( screen, 16, 1, 0);
    // ui.begin( screen, 16, 1, 1);
    screen.fillScreen(TFT_BLACK);

    ui.loadParametersFromFile();//ボタンの基準値を取得
    ui.loadParametersFromFile2();//ボタンの基準値を取得

    ui.createUIs(2); // 最大2つのUIパネルを使用
    createPanels(0, 0, 0); // UIコンテナ0にID０（プリセットUI）を生成
    createPanels(1, select_ui_id, 1); // UIコンテナ1（カスタムUI）にID１～を生成

    drawUI();//UIを表示

    // ui.setSliderVal(UI_NO_CUSTOM, 1, SLIDER_NO_0, 0.3,0.5);

    sprite88_0.setPsram(false );
    sprite88_0.setColorDepth(16);//子スプライトの色深度
    sprite88_0.createSprite(8, 8);//ゲーム画面用スプライトメモリ確保

    //sprite88_0.drawPngFile(SPIFFS, "/init/sprite.png", -8*1, -8*0);

    sprite64.setPsram(false );
    sprite64.setColorDepth(16);//子スプライトの色深度
    sprite64.createSprite(PNG_SPRITE_WIDTH, PNG_SPRITE_HEIGHT);//ゲーム画面用スプライトメモリ確保//wroomだと64*128だとメモリオーバーしちゃう問題を色番号配列にして回避した

    sprite64.drawPngFile(SPIFFS, "/init/initspr.png", 0, 0);//一時展開する

    sprite64cnos_vector.clear();//初期化処理

    for(int y = 0; y < PNG_SPRITE_HEIGHT; y++) {
        for(int x = 0; x < PNG_SPRITE_WIDTH; x++) {
          if(x%2 == 0){
            uint8_t pixel_data = (readpixel(x, y) << 4) | (readpixel(x + 1, y) & 0b00001111);
            sprite64cnos_vector.push_back(pixel_data);
          }
        }
    }

    //破棄
    sprite64.deleteSprite();//3Denginのマップ描画に使っているが、、、星の数が増えると、メモリが足りなくなる、、、

    sprite88_roi.setPsram(false );
    sprite88_roi.setColorDepth(16);//子スプライトの色深度
    sprite88_roi.createSprite(8, 8);//ゲーム画面用スプライトメモリ確保

    sprite11_roi.setPsram(false );
    sprite11_roi.setColorDepth(16);//子スプライトの色深度
    sprite11_roi.createSprite(1, 1);//ゲーム画面用スプライトメモリ確保

    spriteMap.setPsram(false );
    spriteMap.setColorDepth(16);//子スプライトの色深度
    spriteMap.createSprite(MAPWH, MAPWH/divnum);//マップ展開用スプライトメモリ確保

    if(firstBootF == true)
    {
      // createAbsUI();
      mapFileName = "/init/map/0.png";
      readMap();
      delay(50);

      game = nextGameObject(&appfileName, gameState, mapFileName);//ホームゲームを立ち上げる（オブジェクト生成している）
      game->init();//（オブジェクト生成している）
      // tunes.init();//（オブジェクト生成している）
    }

    frame=0;
    }
    else if(isEditMode == TFT_EDIT_MODE)//エディットモードの時
    {
      if(firstBootF == false){
        tft.deleteSprite();
        delay(10);
      }
      setTFTedit(TFT_EDIT_MODE);
      
      ui.begin( screen, 16, 1, 1);

      if(firstBootF == true)
      {

        if (SPIFFS.exists(appfileName)) {
          File file = SPIFFS.open(appfileName, FILE_READ);
          if (!file) {
            Serial.println("ファイルを開けませんでした");
            return;
          }
          // ファイルからデータを読み込み、シリアルモニターに出力
          while (file.available()) {
            Serial.write(file.read());
          }
          // ファイルを閉じる
          file.close();
        }

      // createAbsUI();
      game = nextGameObject(&appfileName, gameState, mapFileName);//ホームゲームを立ち上げる（オブジェクト生成している）
      game->init();//（オブジェクト生成している）
      // tunes.init();//（オブジェクト生成している）

      frame=0;

      // editor.initEditor(tft, EDITOR_ROWS, EDITOR_COLS);
      // editor.initEditor(tft);
      // editor.readFile(SPIFFS, appfileName.c_str());
      // editor.editorOpen(SPIFFS, appfileName.c_str());
      // editor.editorSetStatusMessage("Press ESCAPE to save file");
    }
  }
  savedAppfileName = appfileName;//起動したゲームのパスを取得しておく
  firstBootF = false;
  ui.setupPhBtns(22,25,36);

  // Serial.println("Free heap: " + String(esp_get_free_heap_size()));
  // Serial.println("Minimum free heap: " + String(heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT)));

  // タスクスケジューラの開始
    runner.startNow();


}

void loop()
{

  // luaSendWaitloop();// read /00sample/main.luaファイルを読み取ってシリアルに送信

  runner.execute(); // タスクスケジューラの実行
  // Serial.println("Minimum free heap: " + String(heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT)));
  
  // 現在の時間を取得する
  uint32_t currentTime = millis();
  // 前フレーム処理後からの経過時間を計算する
  uint32_t elapsedTime = currentTime - startTime;
  // 前フレームからの経過時間を計算する
  uint32_t remainTime = (currentTime - preTime);
  preTime = currentTime;
  
  // ボリュームの更新（アナログボタン）
  ui.updatePhVols();

  // ボタンの更新（物理ボタン3つ）
  ui.updatePhBtns();
  
  // ボタンの入力状態を取得
  hitvalue = ui.getHitValue();

  // 各ボタンの状態を判定
  if (hitvalue == 101 || hitvalue == 111) {
    INPUT_START = 1;
  } else {
    INPUT_START = 0;
  }

  if (hitvalue == 102 || hitvalue == 112) {
    INPUT_SELECT = 1;
  } else {
    INPUT_SELECT = 0;
  }

  if (hitvalue == 103 || hitvalue == 113) {
    INPUT_SYSTEM = 1;
  } else {
    INPUT_SYSTEM = 0;
  }

  // 状態をシリアルモニタに出力
  // Serial.print(hitvalue);
  // Serial.print(":");
  // Serial.print(INPUT_START);
  // Serial.print(":");
  // Serial.print(INPUT_SELECT);
  // Serial.print(":");
  // Serial.print(INPUT_SYSTEM);
  // Serial.print(":");

  
  // Serial.print(ui.getPhVolVec(0,1));
  // Serial.print(":");
  // Serial.print(ui.getPhVolDir(2));
  
  // Serial.println(":");

  

  if(ui.getPhVolVec(0,1)!=-1){
    pressedBtnID = remapAnalogBtnID[ui.getPhVolVec(0,1)];
  }else{
    pressedBtnID = -1;
  }

  if(pressedBtnID == -1 && ui.getPhVolDir(2)!=-1){
    pressedBtnID = remapPhysicBtnID[ui.getPhVolDir(2)];
  }


  // ui.setConstantGetF(true);//trueだとタッチポイントのボタンIDを連続取得するモード
  ui.update(screen);//タッチイベントを取るので、LGFXが基底クラスでないといけない

  // if( ui.getEvent() != NO_EVENT ){//何かイベントがあれば

  //   if( ui.getEvent() == TOUCH ){//TOUCHの時だけ
      
  //   }
  //   if(ui.getEvent() == MOVE){
     
  //   }
  //   if( ui.getEvent() == RELEASE ){//RELEASEの時だけ
  //     touchF = false
  //   }
  // }

  
  // if( ui.getEvent() != NO_EVENT ){//何かイベントがあれば

  //   if( ui.getEvent() == TOUCH ){//TOUCHの時だけ
      
  //   }
  //   if(ui.getEvent() == MOVE){
  //     if(ui.getTouchBtnID() == -1){
  //       pressedBtnID = -1;
  //     }else{
  //       pressedBtnID = convUiId[ui.getTouchBtnID()];//convUiIdを使って実際押されたボタンとゲームに送るボタン番号をマップ変換する
  //     }
      
  //   }
  //   if( ui.getEvent() == RELEASE ){//RELEASEの時だけ
  //     // ui.setBtnID(-1);//タッチボタンIDをリセット
  //     // pressedBtnID = ui.getTouchBtnID()+12;//12個分の物理ボタンをタッチボタンIDに足す
  //     pressedBtnID = -1;//リセット
  //   }
  // }

  // ui.update2(screen); // タッチイベントを取るので、LGFXが基底クラスでないといけない
  // // pressedBtnID = 3;

  // if (ui.getEvent() != NO_EVENT)
  // {
  //   uint16_t clist2[3] = {TFT_RED, TFT_GREEN, TFT_BLUE};
  //   uint8_t cNo = 0;
  //   uint8_t addx = 50;
  //   uint8_t addy = 16;

  //   // Serial.println(select_ui_id);

  //   // ui.panels[0]がカスタムパネル　ui.panels[1]がカラーパネル
  //   // if(select_ui_id == 0){//ボタンIDの取得
  //   if (ui.getEvent() != NO_EVENT)
  //   { // 何かイベントがあれば
  //     if (ui.getEvent() == TOUCH)
  //     { // TOUCHの時だけ
  //       if (ui.getTouchBtnID() == 39)//ret
  //       {
  //         pressedBtnID = 11;
  //       }

  //       if (select_ui_id == TOUCH_FLICK_MODE)
  //       {
  //         if (ui.getTouchBtnID() == 34){ui.flickShift();}
  //         else if (ui.getTouchBtnID() == 35){ui.kanaShift();}
  //         else if (ui.getTouchBtnID() == 36){
  //           // chatstr = ui.delChar(chatstr);
  //           String stdStr = ui.delChar(chatstr);
  //           chatstr = stdStr;
  //           // Serial.println(chatstr);        
  //         }
  //         else if(ui.getTouchBtnID() >= 20 && ui.getTouchBtnID() < 32)
  //         {
  //           if(preFlickBtnID != ui.getTouchBtnID()){
  //             ui.kanaShift(0);//ボタンを切り替えたら濁点にならないように０リセット
  //             // Serial.print(ui.getTouchBtnID());
  //             // Serial.println(preFlickBtnID);
  //           }
  //         }

  //         if (ui.getTouchBtnID() >= PRESET_BTN_NUM && ui.getTouchBtnID() < PRESET_BTN_NUM+12)
  //         {
  //           preFlickBtnID = ui.getTouchBtnID();//フリックボタンの値を保存しておく
  //           ui.readFlickDataline(PRESET_BTN_NUM, UI_NO_CUSTOM, preFlickBtnID);
  //         }
  //         else if (ui.getTouchBtnID() >= PRESET_BTN_NUM+12 && ui.getTouchBtnID() < PRESET_BTN_NUM+20)
  //         {
  //           ui.readFlickDataline(PRESET_BTN_NUM, UI_NO_CUSTOM, preFlickBtnID);//シフトの時は直前に押したフリックボタンの値を使う
  //         }
  //         ui.drawFlicks(UI_NO_CUSTOM, screen);
  //       } 
  //     }
  //     if (ui.getEvent() == MOVE)
  //     {
  //       if (ui.getTouchBtnID() == -1)
  //       {
  //         pressedBtnID = -1;
  //       }
  //       else
  //       {
  //         if (select_ui_id == TOUCH_SLIDER_MODE)
  //         {
  //           // pressedBtnID = ui.getTouchBtnID(); // ボタンIDを取得
  //           pressedBtnID = convUiId[ui.getTouchBtnID()];//convUiIdを使って実際押されたボタンとゲームに送るボタン番号をマップ変換する
  //         }

  //         if (select_ui_id == TOUCH_FLICK_MODE)
  //         {

  //           //show touchEvent
  //           // tft.fillRect(120-20, addy+40-20, 60, 60, TFT_BLACK);//塗りなおし
  //           // tft.fillRect(120, addy+40, 20, 20, TFT_GREEN);//中心

  //           // screen.drawLine(
  //           //   ui.getStartPos().x,
  //           //   ui.getStartPos().y,
  //           //   ui.getPos().x,
  //           //   ui.getPos().y,
  //           //   TFT_RED);

  //           screen.drawLine(0, 158, 240, 158, TFT_BLACK);

  //           screen.drawLine((ui.getVecNo())%5 * 48, 158, (ui.getVecNo())%5 * 48+48, 158, TFT_ORANGE);

  //           Serial.println(ui.getVecNo());
  //         }

  //       }
  //     }

  //     // スライダ値の取得
  //     if (select_ui_id == TOUCH_SLIDER_MODE)
  //     {
        
  //       ui.drawSliders(UI_NO_CUSTOM, screen); // drawしながら取得している
  //       if(ui.getSliderVal(UI_NO_CUSTOM, 1, SLIDER_NO_0, X_VAL)>=0&&ui.getSliderVal(UI_NO_CUSTOM, 1, SLIDER_NO_0, X_VAL)<=1){

  //         // Serial.println("A:"+String(ui.getSliderVal(UI_NO_CUSTOM, 1, SLIDER_NO_0, X_VAL)));
  //         // Serial.println("B:"+String(ui.getSliderVal(UI_NO_CUSTOM, 1, SLIDER_NO_1, X_VAL)));

  //         tickSpeed = floor(ui.getSliderVal(UI_NO_CUSTOM, 1, SLIDER_NO_0, X_VAL)*16);
  //         tickTime = tickSpeed*25 + 10;
  //         effectVal = ui.getSliderVal(UI_NO_CUSTOM, 1, SLIDER_NO_1, X_VAL)*16;

  //         // if (ui.getSliderVal(UI_NO_CUSTOM, 0, SLIDER_NO_0, X_VAL) > 0.6)
  //         // {
  //         //   pressedBtnID = 2;
  //         // }
  //         // if (ui.getSliderVal(UI_NO_CUSTOM, 0, SLIDER_NO_0, X_VAL) < 0.4)
  //         // {
  //         //   pressedBtnID = 1;
  //         // }
  //         // if (ui.getSliderVal(UI_NO_CUSTOM, 0, SLIDER_NO_0, Y_VAL) > 0.6)
  //         // {
  //         //   pressedBtnID = 4;
  //         // }
  //         // if (ui.getSliderVal(UI_NO_CUSTOM, 0, SLIDER_NO_0, Y_VAL) < 0.4)
  //         // {
  //         //   pressedBtnID = 3;
  //         // }
  //       }
  //     }
      
  //     if (select_ui_id == TOUCH_FLICK_MODE)
  //     {

  //     }
      
  //     if (select_ui_id == TOUCH_BTN_MODE)
  //     {
  //       // ui.drawSliders(UI_NO_CUSTOM, screen); // drawしながら取得している
        
  //       if (ui.getTouchBtnID() >= PRESET_BTN_NUM && ui.getTouchBtnID() < PRESET_BTN_NUM+25)
  //       {
  //         pressedBtnID = convUiId[ui.getTouchBtnID() - PRESET_BTN_NUM];
  //       }
        
  //     }

  //     if (ui.getEvent() == RELEASE)
  //     { // RELEASEの時だけ
  //       // ui.setBtnID(-1);//タッチボタンIDをリセット
  //       // pressedBtnID = ui.getTouchBtnID()+12;//12個分の物理ボタンをタッチボタンIDに足す

  //       // 17~19でUIパネルを切り替える
  //       if (ui.getTouchBtnID() >= 17 && ui.getTouchBtnID() < PRESET_BTN_NUM)
  //       {
  //         select_ui_id = ui.getTouchBtnID() - 16;
  //         // select_ui_id += 1;//プリセットUI＝０を更新しないように＋１しておく
  //         // プリセットパネル
  //         //  ui.deletePanels(0);//UIコンテナ0内のパネを削除して
  //         //  createPanels(0, select_ui_id);//UIコンテナ0に指定したUIを生成しなおす
  //         // カスタムパネル
  //         ui.deletePanels(UI_NO_CUSTOM, PRESET_BTN_NUM);            // UIコンテナ1内のパネを削除して,ボタン番号をプリセットの数でリセット
  //         createPanels(UI_NO_CUSTOM, select_ui_id,1); // UIコンテナ１に指定したUIを生成しなおす
  //       }
  //       if(select_ui_id == TOUCH_BTN_MODE||select_ui_id == TOUCH_SLIDER_MODE)
  //       {
  //         ui.drawBtns(UI_NO_CUSTOM, screen);//ボタンとボタン番号塗り替える処理
  //       }
  //       else if (select_ui_id == TOUCH_FLICK_MODE)
  //       {
  //         if (ui.getTouchBtnID()==38){//clr
  //           chatstr = "";
  //           Serial.println("");
  //         }
          
  //         // if (ui.getTouchBtnID() >= PRESET_BTN_NUM && ui.getTouchBtnID() < PRESET_BTN_NUM+12)
  //         // {
  //         //   preFlickBtnID = ui.getTouchBtnID();//フリックボタンの値を保存しておく
  //         //   ui.readFlickDataline(PRESET_BTN_NUM, UI_NO_CUSTOM, preFlickBtnID);
  //         // }
  //         // else if (ui.getTouchBtnID() >= PRESET_BTN_NUM+12 && ui.getTouchBtnID() < PRESET_BTN_NUM+20)
  //         // {
  //         //   ui.readFlickDataline(PRESET_BTN_NUM, UI_NO_CUSTOM, preFlickBtnID);//シフトの時は直前に押したフリックボタンの値を使う
  //         // }
  //         // else

  //         if (ui.getTouchBtnID() >= PRESET_BTN_NUM && ui.getTouchBtnID() < PRESET_BTN_NUM+12)
  //         {
  //           // c = ui.getFlickChar(0);
  //           c = ui.getFlickChar(ui.getVecNo());

  //           // if( ui.getEventBit(RIGHT_FLICK))c = ui.getFlickChar(4);
  //           // if( ui.getEventBit(U_R_FLICK) ) c = ui.getFlickChar(3);
  //           // if( ui.getEventBit(UP_FLICK) )  c = ui.getFlickChar(2);
  //           // if( ui.getEventBit(L_U_FLICK) ) c = ui.getFlickChar(1);
  //           // if( ui.getEventBit(LEFT_FLICK) )c = ui.getFlickChar(0);
  //           // if( ui.getEventBit(D_L_FLICK) );
  //           // if( ui.getEventBit(DOWN_FLICK) );
  //           // if( ui.getEventBit(R_D_FLICK) );
  //           chatstr += c;
  //           Serial.print(c);    
  //         }

  //         if (ui.getTouchBtnID() >= PRESET_BTN_NUM+20 && ui.getTouchBtnID() <= PRESET_BTN_NUM+24)
  //         {
  //           c = ui.getFlickChar(1);
  //           // String c = ui.getFlickChar(PRESET_BTN_NUM, UI_NO_CUSTOM, ui.getTouchBtnID());
  //           chatstr += c;
  //           Serial.print(c);            
  //         } 

  //         //入力画面を表示
  //         tft.fillScreen(TFT_BLACK);
  //         tft.setTextSize(1);
  //         tft.setFont(&lgfxJapanGothicP_16);//日本語可
  //         tft.setCursor(0,20);
  //         tft.print(chatstr);

  //         // ui.drawFlicks(UI_NO_CUSTOM, screen);
  //       }

  //       pressedBtnID = -1; // リセット
  //       for(int i = 0; i < ui.getAllBtnNum(); i ++){//ボタンの強制リセット
  //         buttonState[i] = 0;
  //       }
  //     }
  //   }
  // }



    // if(ui.getTouchBtnID() == RELEASE){//リリースされたら
    //   pressedBtnID = -1;
    // }
  
  // uint32_t now = millis();
  // uint32_t remainTime = (now >= preTime) ? (now - preTime) : (UINT32_MAX - preTime + now);
  // preTime = now;

  

    // 経過時間が1/30秒以上経過した場合
if (elapsedTime >= 1000/fps||fps==-1) {

  if(isEditMode == TFT_SOUNDEDIT_MODE){
    if (btnpF) {
      if(pressedBtnID == 3){
        targetChannelNo -=1;
      }else if(pressedBtnID == 4){
        targetChannelNo +=1;
      }
      targetChannelNo%=8;
      
      if(pressedBtnID == 1){
        masterVol -=8;
      }else if(pressedBtnID == 2){
        masterVol +=8;
      }
      masterVol%=256;

      if (pressedBtnID == 9) {

        if(ch_viewmode == CH1_VIEW_MODE){
          ch_viewmode = CH8_VIEW_MODE;
          return;
        }else if(ch_viewmode == CH8_VIEW_MODE){
          ch_viewmode = CH1_VIEW_MODE;
          return;
        }
      }
      // if (keychar == PS2_ENTER) {
      // }
    }

    tft.fillScreen(0);
    if(ch_viewmode == CH1_VIEW_MODE){
      controlMusicVisual();
    }else if(ch_viewmode == CH8_VIEW_MODE){
      showMusicVisual8ch();
    }

    if(debugMode==1){
      debugInfo();
    }else if(debugMode==2){
      showMusicInfo();
    }

    //最終出力
    tft.setPivot(0, 0);
    tft.pushRotateZoom(&screen, 40, 3, 0, 1, 1);

    if (pressedBtnID == 0)
    {
      reboot("/init/main.lua", TFT_RUN_MODE);
    }

  }else if( isEditMode == TFT_RUN_MODE ){


    //ゲーム内のprint時の文字設定をしておく
    tft.setTextSize(1);//サイズ
    tft.setFont(&lgfxJapanGothicP_8);//日本語可
    tft.setCursor(0, 0);//位置
    tft.setTextWrap(true);

    // == tune task ==
    // tunes.run();

    // == game task ==
    mode = game->run(remainTime);//exitは1が返ってくる　mode=１ 次のゲームを起動

    // if(pressedBtnID == 9){//(|)
    
    // // appfileName = 
    //   // editor.editorSave(SPIFFS);//SPIFFSに保存
    //   delay(100);//ちょっと待つ
    //   reboot(appfileName, TFT_EDIT_MODE);//現状rebootしないと初期化が完全にできない
    //   // restart(appfileName, 0);//初期化がうまくできない（スプライトなど）
    //   // broadchat();//ファイルの中身をブロードキャスト送信する（ファイルは消えない）
    // }

    //ESCボタンで強制終了
    if(hitvalue ==203){

      appfileName = "/init/main.lua";

      patternNo = 0;//音楽開始位置を0にリセット

      firstLoopF = true;
      toneflag = false;
      sfxflag = false;
      musicflag = false;
      debugMode = 0;

      mode = 1;//exit
      // 星用のベクター配列使用後は要素数を0にする
      bsParamFloat.resize(0);
      bsParamInt8t.resize(0);
      bsParamInt16t.resize(0);
    }

    if (pressedBtnID == 9999)
    { // reload
      mode = 1;//exit
      pressedBtnID = -1;
    }

    if(mode != 0){ // exit request//次のゲームを立ち上げるフラグ値、「modeが１＝次のゲームを起動」であれば
      // tunes.pause();
      game->pause();
      // ui.clearAddBtns();//個別のゲーム内で追加したタッチボタンを消去する
      free(game);
      firstLoopF = true;
      toneflag = false;
      sfxflag = false;
      musicflag = false;
      fps = 60;
      // txtName = appfileName;
      game = nextGameObject(&appfileName, gameState, mapFileName);//ファイルの種類を判別して適したゲームオブジェクトを生成
      game->init();//resume()（再開処理）を呼び出し、ゲームで利用する関数などを準備
      // tunes.resume();
    }
    
    // ui.showTouchEventInfo( tft, 0, 100 );//タッチイベントを視覚化する

    // if(patternNo<10)screen.fillRect(200,120,16,8,TFT_BLACK);
    // ui.showInfo( screen, 200, 112 );//ボタン情報、フレームレート情報などを表示します。
    // screen.setCursor(200,120);
    // screen.println(patternNo);
    
    

    if(enemyF){

      sprite64.setPsram(false);
      sprite64.setColorDepth(16);    // 子スプライトの色深度
      sprite64.createSprite(48, 48); // ゲーム画面用スプライトメモリ確保

      // sprite64.drawPngFile(SPIFFS, enemyPath, enemyX, enemyY);//sprite64に展開する
      sprite64.pushRotateZoom(&tft, enemyX, enemyY, 0, 1, 1, gethaco3Col(enemyTransCn));

      sprite64.deleteSprite();//消す

      // tft.drawPngFile(SPIFFS, enemyPath, enemyX, enemyY);//直接展開する
    }

      if(toolNo != 0){
        if(toolNo==1){//カラーパレット
          for(int j = 0; j<8; j++){
            for(int i = 0; i<2; i++){
              screen.fillRect(i*16,j*16,16,16,gethaco3Col(j*2+i));
            }
          }
        }

        toolNo = 0;
      }


    // tft.fillScreen(0);
    // if(ch_viewmode == CH1_VIEW_MODE){
    //   controlMusicVisual();
    // }else if(ch_viewmode == CH8_VIEW_MODE){
    //   showMusicVisual8ch();
    // }
    
    if(debugMode==1){
      debugInfo();
    }else if(debugMode==2){
      showMusicInfo();
    }

    // ui.showTouchEventInfo( tft, 0, 100 );//タッチイベントを視覚化する
    // ui.showInfo( screen, 200, 112 );//ボタン情報、フレームレート情報などを表示します。

    if(outputMode == WIDE_MODE){
      tft.pushAffine(matrix);//ゲーム画面を最終描画する
    }
    else if(outputMode == FAST_MODE){
      
      tft.pushSprite(&screen,TFT_OFFSET_X,TFT_OFFSET_Y);//ゲーム画面を小さく高速描画する
    }
    

    if(pressedBtnID == 5){//PAGEUP(:)//キーボードからエディタ再起動
      // restart(appfileName, 1);//appmodeでリスタートかけるので、いらないかも
    }
  }
  else if(isEditMode == TFT_EDIT_MODE)
  {
    
    // // editor.editorRefreshScreen(tft);

    // float codeunit = 128.0/float(editor.getNumRows());
    // float codelen = codeunit * 14;//14は表示行数
    // // int codelen = int(codelen_f + 0.5); // 四捨五入して整数に変換する
    
    // float curpos = codeunit*editor.getCy();
    // float codepos = codeunit * (editor.getCy() - editor.getScreenRow());
    // // int codepos = int(codepos_f + 0.5); // 四捨五入して整数に変換する
    
    // if(!textMoveF)
    // tft.fillRect(156,0, 4,128, HACO3_C5);//コードの全体の長さを表示
    // else
    // tft.fillRect(156,0, 4,128, HACO3_C9);//コードの全体の長さを表示

    // if(!shiftF)
    // tft.fillRect(156,int(codepos), 4,codelen, HACO3_C6);//コードの位置と範囲を表示
    // else
    // tft.fillRect(156,int(codepos), 4,codelen, HACO3_C12);//コードの位置と範囲を表示

    // if(curpos>=int(codepos)+codelen)//すこしはみ出たら表示コード内に入れる
    // {
    //   if(codeunit>=1)curpos = int(codepos)+codelen - codeunit;
    //   else curpos = int(codepos)+codelen - 1;
    // }

    // if(codeunit>=1){tft.fillRect(155, int(curpos), 4, codeunit, HACO3_C8);}//コードの位置と範囲を表示
    // else{tft.fillRect(155, int(curpos), 4, 1, HACO3_C8);}//１ピクセル未満の時は見えなくなるので１に
    
    //  //最終出力
    // tft.setPivot(0, 0);
    // tft.pushRotateZoom(&screen, 40, 3, 0, 1, 1);
    
    // if(pressedBtnID == 0)//(|)メニュー画面へ
    // {
    //   editor.setCursorConfig(0,0,0);//カーソルの位置を保存
    //   delay(50);
    //   restart("/init/main.lua", TFT_RUN_MODE);
    // }

    // if(pressedBtnID == 9){//(|)
    //   editor.editorSave(SPIFFS);//SPIFFSに保存
    //   delay(100);//ちょっと待つ
    //   reboot(appfileName, TFT_RUN_MODE);//現状rebootしないと初期化が完全にできない
    //   // restart(appfileName, 0);//初期化がうまくできない（スプライトなど）
    //   // broadchat();//ファイルの中身をブロードキャスト送信する（ファイルは消えない）
    // }

  }

  startTime = currentTime;
}

  frame++;
  if(frame > 18446744073709551615)frame = 0;
  delay(1);
  
}