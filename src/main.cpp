#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include "SD.h"
#include "SPIFFS.h"
#include <LovyanGFX_DentaroUI.hpp>
#include "Tunes.h"
#include "runLuaGame.h"
#include "wifiGame.h"


uint8_t xpos, ypos = 0;

#define LOG_FILE_NAME "/config.csv"
static File s_myFile;

int pressedBtnID = -1;

//2倍拡大表示用のパラメータ
float matrix_side[6] = {2.0,   // 横2倍
                     -0.0,  // 横傾き
                     258.0,   // X座標
                     0.0,   // 縦傾き
                     2.0,   // 縦2倍
                     0.0    // Y座標
                    };
                    
//2倍拡大表示用のパラメータ
float matrix_bg[6] = {2.0,   // 横2倍
                     -0.0,  // 横傾き
                     0.0,   // X座標
                     0.0,   // 縦傾き
                     2.0,   // 縦2倍
                     0.0    // Y座標
                    };

LGFX screen;//LGFXを継承

LovyanGFX_DentaroUI ui(&screen);

static LGFX_Sprite sideSprite( &screen );//背景スプライトはディスプレイに出力
LGFX_Sprite layoutSprite(&screen);//LGFX_Spriteを継承
LGFX_Sprite tft = LGFX_Sprite(&layoutSprite);//LGFX_Spriteを継承

// RunLuaGame*　game;
BaseGame* game;
String fileName = "/init/main.lua";
WifiGame* wifiGame = NULL;
Tunes tunes;

hw_timer_t * timerA = NULL;//スピーカー用
// extern void onTimerA();
volatile static boolean timer_flag = false;
//高速
void IRAM_ATTR onTimerA() {
  timer_flag != timer_flag;
}
// void onTimerA() {
//   timer_flag != timer_flag;
// }


void startWifiDebug(bool isSelf){
  tunes.pause();
  wifiGame = new WifiGame();
  wifiGame->init(isSelf);
  tunes.resume();
}
char buf[50];
char *A;
bool flip = false;

uint32_t preTime;
void setFileName(String s){
  fileName = s;
}
bool isWifiDebug(){
  return wifiGame != NULL;
}
void reboot(){
  wifiGame->pause();
  ESP.restart();
}



// タイマー
hw_timer_t * timer = NULL;

// 画面描画タスクハンドル
TaskHandle_t taskHandle;

// 画面描画タスク
void dispTask(void *pvParameters) {
  while (1) {
  // tft.fillScreen(TFT_RED);

  if(flip){
    tft.pushSprite(&layoutSprite,0,0);
    ui.showTouchEventInfo( layoutSprite, 120, 0 );//タッチイベントを視覚化する
    ui.showInfo( layoutSprite, 0, 0 );//ボタン情報、フレームレート情報などを表示します。
    layoutSprite.pushAffine(matrix_bg);
  }else{
  }
  
  flip = !flip;//描画は2フレにつき１回行う
  }
}

// タイマー割り込み
void IRAM_ATTR onTimer() {
  xTaskNotifyFromISR(taskHandle, 0, eIncrement, NULL);
}

void setup()
{
  Serial.begin(115200);
  delay(50);

  // タイマー作成(33.333ms)
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

  // 描画用タスク作成
  xTaskCreateUniversal(
    dispTask,
    "dispTask",
    8192,
    NULL,
    1,
    &taskHandle,
    APP_CPU_NUM
  );


  timerA = timerBegin(0, 80, true);//カウント時間は1マイクロ秒//hw_timer_t*オブジェクト(タイマーハンドラ）がかえってくる
  timerAttachInterrupt(timerA, &onTimerA, true);//タイマー割り込みが発生したときに実行する関数を登録する。timerA =フレームタイマー
  timerAlarmWrite(timerA, 1000000, true);//タイマーの設定値(割り込みのタイミング)を設定する。1/30秒　＝ 33333uSec　1/15秒　＝ 66666uSec
  timerAlarmEnable(timerA);
  delay(10);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  ui.begin( screen, 16, 1, true);//screenスクリーンを用意

    //sprite（bg1)のボタン配置の時
  ui.createBtns( 130,  9,  30, 44,  3, 4, TOUCH, 2);//コントローラー
  ui.createBtns( 130, 53,  30, 66,  2, 6, TOUCH, 2);//メインメニュー
  // ui.createBtns( 3,   66,  66, 11,  6, 1, TOUCH, 2);//最後の引数でタッチエリアを2倍している
  // ui.createBtns( 97,  66,  32, 11,  4, 1, TOUCH, 2);//最後の引数でタッチエリアを2倍している

  // ui.createBtns( 1, 77,    128, 32, 16, 4, TOUCH, 2);//最後の引数でタッチエリアを2倍している
  // ui.createBtns( 1, 1,     64, 64,  8, 8, TOUCH, 2);//最後の引数でタッチエリアを2倍している

  // ui.createBtns( 69, 6,    40, 40,  4, 4, TOUCH, 2);//最後の引数でタッチエリアを2倍している
  // ui.createBtns( 109, 3,   20, 46,  2, 5, TOUCH, 2);//最後の引数でタッチエリアを2倍している
  // ui.createBtns( 66, 48,   64, 18,  8, 1, TOUCH, 2);//最後の引数でタッチエリアを2倍している
  delay(100);

  layoutSprite.setPsram( false );//DMA利用のためPSRAMは切る
  layoutSprite.createSprite( 128, 128 );
  tft.setPsram( false );//DMA利用のためPSRAMは切る
  tft.createSprite( 128, 128 );
  // tft2.setPsram( false );//DMA利用のためPSRAMは切る
  // tft2.createSprite( 128, 128 );
  // screen.startWrite();//CSアサート開始
  layoutSprite.startWrite();//CSアサート開始
  // tft.startWrite();//CSアサート開始
  // tft2.startWrite();//CSアサート開始
  
  // pinMode(39, INPUT_PULLUP);//ボタン設定は後で
  // pinMode(23, INPUT_PULLUP);
  // pinMode(34, INPUT_PULLUP);
  // pinMode(35, INPUT_PULLUP);

  // pinMode(17, INPUT_PULLUP);
  // pinMode(16, INPUT_PULLUP);
  // pinMode(4, INPUT_PULLUP);

  // pinMode(36, INPUT_PULLUP);
  // pinMode(25, INPUT_PULLUP);
  // pinMode(22, INPUT_PULLUP);

  // game =  new RunLuaGame();//元はこっち
  game =  new BaseGame();//これしか通らない
  game->pause();
  free(game);
  game = new RunLuaGame();//リセットしてからだと通る
  game->init();
  tunes.init();
  delay(1000);
  
  sideSprite.setPsram(true );
  sideSprite.setColorDepth(16);//子スプライトの色深度
  sideSprite.createSprite(31, 120);//ゲーム画面用スプライトメモリ確保
  sideSprite.fillScreen(TFT_RED);
// //  //SDからの読み込みは４Gb以下じゃないとうまく動作しないかも
// //PNGをバッファに書いて2倍出力    
  sideSprite.drawPngFile(SPIFFS, "/haco3/util/side.png", 0, 0);
  sideSprite.pushAffine(matrix_side);

}

void loop() {
  ui.update(screen);//タッチイベントを取るので、LGFXが基底クラスでないといけない

  
  if( ui.getEvent() != NO_EVENT ){//何かイベントがあれば
    
    if( ui.getEvent() == TOUCH ){//TOUCHの時だけ

    }
    
    if(ui.getEvent() == MOVE){
      pressedBtnID = ui.getTouchBtnID();
    }

    if( ui.getEvent() == RELEASE ){//RELEASEの時だけ
      // modeChange(ui.getTouchBtnID());
      pressedBtnID = ui.getTouchBtnID();
    }
  } 

  // //ゲーム
  // //----------
  uint32_t now = millis();
  uint32_t remainTime = (now - preTime);
  preTime = now;

  if(wifiGame){ // debug mode
    int r = wifiGame->run(remainTime);
    if(r != 0){
      tunes.pause();
      game->pause();
      free(game);
      game = new RunLuaGame();
      game->init();
      
      tunes.resume();
      
    }
  }
  tunes.run();

  int mode = game->run(remainTime);

  if(mode != 0){
    tunes.pause();
    game->pause();
    free(game);
    game = new RunLuaGame();
    game->init();
    tunes.resume();
  }

  // if(flip){
  //   tft.pushSprite(&layoutSprite,0,0);
  //   ui.showTouchEventInfo( layoutSprite, 120, 0 );//タッチイベントを視覚化する
  //   ui.showInfo( layoutSprite, 0, 0 );//ボタン情報、フレームレート情報などを表示します。
  // }else{
  // }
  // layoutSprite.pushAffine(matrix_bg);
  // flip = !flip;//描画は2フレにつき１回行う
}