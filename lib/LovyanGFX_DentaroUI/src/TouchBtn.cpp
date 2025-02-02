#include "TouchBtn.hpp"
#include "LovyanGFX_DentaroUI.hpp"
using namespace std;

TouchBtn::TouchBtn()
{
}

void TouchBtn::setuiSpritePos(lgfx::v1::touch_point_t _uiSpritePos)
{
  uiSpritePos = _uiSpritePos;
}

void TouchBtn::initBtn(int _btnID, String _label, int _s_x, int _s_y, int _s_w, int _s_h,
lgfx::v1::touch_point_t _uiSpritePos,
int _btn_mode)
{
  int _xy_mode = XY_NONE;
  int _eventNo = TOUCH;
  initSlider(_btnID, _label, _s_x, _s_y, _s_w, _s_h, _uiSpritePos, _btn_mode, _xy_mode, _eventNo);
}

void TouchBtn::initFlick(int _btnID, String _label, int _s_x, int _s_y, int _s_w, int _s_h,
lgfx::v1::touch_point_t _uiSpritePos,
int _btn_mode)
{
  int _xy_mode = XY_NONE;
  int _eventNo = TOUCH;
  initSlider(_btnID, _label, _s_x, _s_y, _s_w, _s_h, _uiSpritePos, _btn_mode, _xy_mode, _eventNo);
}

void TouchBtn::initSlider(int _btnID, String _label, int _s_x, int _s_y, int _s_w, int _s_h,
lgfx::v1::touch_point_t _uiSpritePos,
int _btn_mode,
int _xy_mode,
int _eventNo)
{
  btnID = _btnID;
  label = _label;
  uiSpritePos = _uiSpritePos;
  s_x = _s_x;
  s_y = _s_y;
  s_w = _s_w;
  s_h = _s_h;
  s_hw = s_w/2;
  s_hh = s_h/2;
  
  if(_btn_mode==TOUCH_PRE_MODE)btn_name = "PRE_"+_label;
  else  if(_btn_mode==TOUCH_BTN_MODE)btn_name = "BTN_"+_label;
  else if(_btn_mode==TOUCH_SLIDER_MODE)btn_name = "SLIDER_"+_label;
  else if(_btn_mode==TOUCH_FLICK_MODE)btn_name = "FLICK_"+_label;
  // btn_name = _label;
  btn_mode = _btn_mode;
  xy_mode = _xy_mode;
  eventNo = _eventNo;

  sliderPosx = _s_x + (_s_w/2);
  sliderPosy = _s_y + (_s_h/2);
}

void TouchBtn::setOBtnPos( int _s_x, int _s_y){
  s_x = _s_x;
  s_y = _s_y;
}

void TouchBtn::initOBtn(int _btnID, String _label, int _btnNo, int _btns_starAngle, int _s_x, int _s_y, int _s_r0, int _s_r1, int _s_n, String _btn_name,
lgfx::v1::touch_point_t _uiSpritePos,
int _btn_mode)
{
  btnID = _btnID;
  label = _label;
  btnNo = _btnNo;
  btns_starAngle = _btns_starAngle;
  uiSpritePos = _uiSpritePos;
  s_x = _s_x;
  s_y = _s_y;

  b_r0 = _s_r0;
  b_r1 = _s_r1;
  b_n = _s_n;

  b_a = 360/_s_n;

  b_a0 = (b_a * btnNo + btns_starAngle)%360;
  b_a1 = b_a0 + b_a;

  s_hw = s_w/2;
  s_hh = s_h/2;

  btn_name = _btn_name;
  btn_mode = _btn_mode;
  
}

// void TouchBtn::setBtnNameFalse(String _btnNameFalse)
// {
//   // btn_nameFalse = _btnNameFalse;
// }

void TouchBtn::btnDraw(LovyanGFX&  _lcd)
{
  _lcd.fillRoundRect(this->s_x, this->s_y, this->s_w, this->s_h, 10, TFT_BLACK);
  _lcd.drawRoundRect(this->s_x, this->s_y, this->s_w, this->s_h, 10, TFT_WHITE);
  _lcd.setTextSize(1);
  _lcd.setFont(&lgfxJapanGothicP_8);
  _lcd.setTextColor(TFT_RED);
  int8_t s_str_hw = floor(_lcd.textWidth( btn_name ) / 2);
  // if( btn_mode == TOUCH_FLICK_MODE )
  // {
  // String btn_name_headline = "";
  // if(btn_name == "A")btn_name_headline = "ABC";
  // else if(btn_name == "a")btn_name_headline = "abc";
  // _lcd.drawString( btn_name_headline, s_x + s_hw - s_str_hw , s_y + s_hh - 4 );
  // }else{
  //   _lcd.drawString( btn_name, s_x + s_hw - s_str_hw , s_y + s_hh - 4 );
  // }
  _lcd.drawString( btn_name, s_x + s_hw - s_str_hw , s_y + s_hh - 4 );
  
}

void TouchBtn::sliderDraw(LovyanGFX& _lcd)
{
  if( btn_mode == TOUCH_SLIDER_MODE )
  {
    if(xy_mode != XY_NONE)
    {
      // _lcd.fillRect(this->s_x, this->s_y, this->s_w, this->s_h, TFT_BLACK);
      _lcd.setTextSize(1);
      _lcd.setTextColor(TFT_WHITE);
      this->sliderPosx = this->s_x + sliderValx * this->s_w;
      this->sliderPosy = this->s_y + sliderValy * this->s_h;

      _lcd.fillCircle(this->sliderPosx,
                      this->sliderPosy, 5, TFT_WHITE);
      _lcd.drawRoundRect(this->s_x, this->s_y, this->s_w, this->s_h, 10, TFT_WHITE);
      
      // if( xy_mode == Y_VAL || xy_mode==XY_VAL ){
      //   //_lcd.drawString(String(sliderValy), this->s_x + 4 , this->s_y + 16);
      //   _lcd.drawLine(this->s_x,
      //                 this->sliderPosy,
      //                 this->s_x + this->s_w,
      //                 this->sliderPosy, TFT_WHITE);
      // }

      // if( xy_mode == X_VAL || xy_mode==XY_VAL ){
      //   //_lcd.drawString(String(sliderValx), this->s_x + 4 , this->s_y + 4);
      //   _lcd.drawLine(sliderPosx,
      //                 this->s_y,
      //                 sliderPosx,
      //                 this->s_y + this->s_h , TFT_WHITE);
    }
  }
   //_lcd.drawLine(this->s_x, this->s_x, this->s_x + this->s_w, this->s_x, TFT_WHITE);

  // if( btn_mode == TOUCH_BTN_MODE ){
  //   s_str_hw = _lcd.textWidth(btn_name)/2;
  //   _lcd.drawString(btn_name, this->s_x + this->s_hw - s_str_hw , this->s_y + this->s_hh -4);
  // }
  // }
}

// void TouchBtn::setSelectBtnF(bool _selectBtnF){
//   selectBtnF = _selectBtnF;
// }

void TouchBtn::tileDraw(LovyanGFX& _lgfx, LGFX_Sprite& _layoutSprite,
lgfx::v1::touch_point_t _uiSpritePos, 
lgfx::v1::touch_point_t _sp,
uint8_t _bgColIndex, LGFX_Sprite& _g_basic_sprite)
{
  // _g_basic_sprite.setPivot(_g_basic_sprite.width()/2.0, _g_basic_sprite.height()/2.0);//setPivot()で回転する場合の原点を指定します。初期値は左上の(0, 0)だと思います

  // _g_basic_sprite.pushSprite(&divSprite0, tilePos.x, tilePos.y);
  // divSprite0.pushSprite(&_layoutSprite, 0, 0);//親を指定して出力

  // _g_basic_sprite.pushSprite(&divSprite0, tilePos.x-s_qw, tilePos.y);
  // divSprite0.pushSprite(&_layoutSprite, s_qw, 0);//親を指定して出力

  // _g_basic_sprite.pushSprite(&divSprite0, tilePos.x-s_qw*2, tilePos.y);
  // divSprite0.pushSprite(&_layoutSprite, s_qw*2, 0);//親を指定して出力

  // _g_basic_sprite.pushSprite(&divSprite0, tilePos.x-s_qw*3, tilePos.y);
  // divSprite0.pushSprite(&_layoutSprite, s_qw*3, 0);//親を指定して出力
}

void TouchBtn::setAvailableF(bool _availableF){
  availableF = _availableF;
}

bool TouchBtn::getAvailableF(){
  return availableF;
}

void TouchBtn::setVisibleF(bool _visibleF){
  visibleF = _visibleF;
}

bool TouchBtn::getVisibleF(){
  return visibleF;
}

int TouchBtn::getBtnMode(){
  return btn_mode;
}

void TouchBtn::addHandler(DelegateBase2* _func){
  lim2.push_back(_func);
}

void TouchBtn::delHandlers2(){
      std::list<DelegateBase2*>::iterator ite2 =  lim2.begin();
      while( ite2 != lim2.end() )
      {
        // DelegateBase2 *ptr = (*ite2);
        lim2.pop_front();//
      }
}

void TouchBtn::run2(int _btnID, int _btnNo, lgfx::v1::touch_point_t _sp, lgfx::v1::touch_point_t _tp, int _eventState, int _runEventNo)
{
  run2(_btnID, _btnNo, _sp.x, _sp.y, _tp.x, _tp.y, _eventState, _runEventNo);
}

void TouchBtn::run2(int _btnID, int _btnNo, int _sx, int _sy, int _tx, int _ty, int _eventState, int _runEventNo)
{
      sp.x = _sx;
      sp.y = _sy;
      tp.x = _tx;
      tp.y = _ty;

      eventState = _eventState;
      eventNo = _runEventNo;

      //初期化処理
      // this->sliderPosx = 0;
      // this->sliderPosy = 0;

      // this->sliderValx = 0;
      // this->sliderValy = 0;

      std::list<DelegateBase2*>::iterator ite2 =  lim2.begin();
      while( ite2 != lim2.end() )
      {

        DelegateBase2 *ptr = (*ite2);
        if(btn_mode == TOUCH_BTN_MODE){
          if(sp.x > this->s_x
          && sp.x < this->s_x + this->s_w
          && sp.y > this->s_y
          && sp.y < this->s_y + this->s_h)
          {//ボタンの領域内に入っていれば
            (*ptr)( _btnID );// 関数を実行！
          }
        }
        else if(btn_mode == TOUCH_FLICK_MODE){
          if(sp.x > this->s_x
          && sp.x < this->s_x + this->s_w
          && sp.y > this->s_y
          && sp.y < this->s_y + this->s_h)
          {//ボタンの領域内に入っていれば
            (*ptr)( _btnID );// 関数を実行！
          }
        }
        else if(btn_mode == TOUCH_TOGGLE_MODE)
        {//普通のボタン用の判定
          if(sp.x > s_x     + uiSpritePos.x + this->uiSpritePos.x
          && sp.x < s_x+s_w + uiSpritePos.x + this->uiSpritePos.x
          && sp.y > s_y     + uiSpritePos.y + this->uiSpritePos.y
          && sp.y < s_y+s_h + uiSpritePos.y + this->uiSpritePos.y)
          {//ボタンの領域内に入っていれば
          if(availableF == true){
            // this->switchToggleVal();
            (*ptr)( _btnID );    // 関数を実行！
          }
            // if(eventState == runEventNo)this->switchToggleVal();
            // if(eventState == runEventNo && eventState != NO_EVENT)this->switchToggleVal();
          }
        }
        else if(btn_mode == TOUCH_OBTN_MODE)
        {//円形ボタンの領域判定
          // if(sp.x > s_x     + uiSpritePos.x + this->uiSpritePos.x
          // && sp.x < s_x+s_w + uiSpritePos.x + this->uiSpritePos.x
          // && sp.y > s_y     + uiSpritePos.y + this->uiSpritePos.y
          // && sp.y < s_y+s_h + uiSpritePos.y + this->uiSpritePos.y)

          int d = sqrt( ( pow( sp.x- this->s_x, 2.0 ) + pow( sp.y- this->s_y, 2.0 ) ));
          int a = int((atan2(sp.y- this->s_y, sp.x- this->s_x)*180/M_PI ));
          if(a<0) a += 360;//マイナス値をなくす
          int div_a = (this->b_a * this->btnNo);

          if(d > this->b_r1 && d < this->b_r0)
          {//円形ボタンの領域内に入っていれば
            if((360 + a -this->btns_starAngle)%360 > div_a && (360 + a -this->btns_starAngle)%360 < div_a + this->b_a)
            {//分割角と一致する角度であれば
            if(availableF == true){
              (*ptr)( _btnID );    // 関数を実行！
            }
          }
          }
        }
        // else if(btn_mode == TOUCH_FLICK_MODE)//FLICK_MODEもこちらで処理
        // {//普通のボタン用の判定
        //   // if(sp.x > s_x     + uiSpritePos.x + this->uiSpritePos.x
        //   // && sp.x < s_x+s_w + uiSpritePos.x + this->uiSpritePos.x
        //   // && sp.y > s_y     + uiSpritePos.y + this->uiSpritePos.y
        //   // && sp.y < s_y+s_h + uiSpritePos.y + this->uiSpritePos.y)
        //   // {//ボタンの領域内に入っていれば
        //   if(sp.x > this->s_x
        //   && sp.x < this->s_x + this->s_w
        //   && sp.y > this->s_y
        //   && sp.y < this->s_y + this->s_h)
        //   {
        //   // if(availableF == true){
        //     (*ptr)( _btnID );    // 関数を実行！
        //   // }
        //     // if(eventState == runEventNo)this->switchToggleVal();
        //     // if(eventState == runEventNo && eventState != NO_EVENT)this->switchToggleVal();
        //   }
        // }
        else if(btn_mode == TOUCH_SLIDER_MODE)
        {//スライダボタン用の判定
          if(sp.x > this->s_x
          && sp.x < this->s_x + this->s_w
          && sp.y > this->s_y
          && sp.y < this->s_y + this->s_h)
          {//ボタンの領域内に入っていれば
            (*ptr)( _btnID );// 関数を実行！
            if( eventState == eventNo||eventNo == MULTI_EVENT ){
                this->sliderPosx = tp.x;
                this->sliderPosy = tp.y;

                this->sliderValx = float( tp.x - s_x)/s_w;
                this->sliderValy = float( tp.y - s_y)/s_h;
            }
            if( (tp.x) >= s_x && (tp.x) <= s_x + s_w ){
              if( (tp.y) >= s_y && (tp.y) <= s_y + s_h ){
                this->sliderPosx = tp.x;
                this->sliderPosy = tp.y;

                this->sliderValx = float(tp.x - s_x)/s_w;
                this->sliderValy = float(tp.y - s_y)/s_h;
              }
            }
          
          }else{
            // this->sliderPosx = 0;
            // this->sliderPosy = 0;

            // this->sliderValx = 0.5;
            // this->sliderValy = 0.5;
          }
        }
        ite2++;
        lim2.pop_front();//メソッドを実行したら削除
      }

}
int TouchBtn::getBtnID(){
    return btnID;
}
void TouchBtn::setBtnNamelabel(String _namelabel){
    namelabel = _namelabel;
}

void TouchBtn::setColor(uint16_t _color){
  color = _color;
}
void TouchBtn::setBgColorIndex(uint16_t _bgColorIndex){
  bgColorIndex = _bgColorIndex;
}

lgfx::v1::touch_point_t TouchBtn::getTouchPoint(int _x, int _y){
  lgfx::v1::touch_point_t tp;
  tp.x = _x;
  tp.y = _y;
  return tp;
}

void TouchBtn::switchToggleVal(){
  if(this->toggleVal == true)this->toggleVal = false;
  else if(this->toggleVal == false)this->toggleVal = true;
  else ;
}

float TouchBtn::getSliderValx(){
  return this->sliderValx;
}

float TouchBtn::getSliderValy(){
  return this->sliderValy;
}

void TouchBtn::setSliderVal(float _x, float _y){
  this->sliderValx = _x;
  this->sliderValy = _y;
}

bool TouchBtn::getToggleVal(){
  return this->toggleVal;
}

// lgfx::v1::touch_point_t TouchBtn::getTilePos(){
//   return tilePos;
// }

void TouchBtn::setTilePos(lgfx::v1::touch_point_t _pos){
  tilePos = _pos;
}

int TouchBtn::get_xy_mode(){
  return xy_mode;
}

lgfx::v1::touch_point_t TouchBtn::getBtnPos(){
  return getTouchPoint(this->s_x, this->s_y);
}


void TouchBtn::setBtnName(String _btn_name){
  btn_name = _btn_name;
}

String TouchBtn::getBtnName()
{
  return btn_name;
}


void TouchBtn::setDrawFinishF(bool _drawFinishF){
  drawFinishF = _drawFinishF;
}

bool TouchBtn::getDrawFinishF(){
  return drawFinishF;
}
