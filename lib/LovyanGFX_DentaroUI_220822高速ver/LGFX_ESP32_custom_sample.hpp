#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>

// ESP32でLovyanGFXを独自設定で利用する場合の設定例

/*
このファイルを複製し、新しい名前を付けて、環境に合わせて設定内容を変更してください。
作成したファイルをユーザープログラムからincludeすることで利用可能になります。

複製したファイルはライブラリのlgfx_userフォルダに置いて利用しても構いませんが、
その場合はライブラリのアップデート時に削除される可能性があるのでご注意ください。

安全に運用したい場合はバックアップを作成しておくか、ユーザープロジェクトのフォルダに置いてください。
//*/


/// 独自の設定を行うクラスを、LGFX_Deviceから派生して作成します。
class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ILI9341 _panel_instance;
    lgfx::Bus_SPI       _bus_instance;
    lgfx::Light_PWM     _light_instance;
    lgfx::Touch_XPT2046 _touch_instance;
  public:
    LGFX(void)
    {
      {
        auto cfg = _bus_instance.config();

        // SPIバスの設定
        cfg.spi_host         = VSPI_HOST;
        cfg.spi_mode         = 0;
        cfg.freq_write       = 40000000;
        cfg.freq_read        = 16000000;
        cfg.spi_3wire        = true;
        cfg.use_lock         = true;
        cfg.dma_channel      = SPI_DMA_CH_AUTO;
        cfg.pin_sclk         = 14; // 変更
        cfg.pin_mosi         = 13; // 変更
        cfg.pin_miso         = 12; // 変更
        cfg.pin_dc           =  2; // 変更

        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
      }

     {
        auto cfg = _panel_instance.config();

        cfg.pin_cs           =    15; // 変更
        cfg.pin_rst          =    -1; // 変更
        cfg.pin_busy         =    -1; // 変更

        cfg.panel_width      =   240;
        cfg.panel_height     =   320;
        cfg.offset_x         =     0;
        cfg.offset_y         =     0;
        cfg.offset_rotation  =     0;
        cfg.dummy_read_pixel =     8;
        cfg.dummy_read_bits  =     1;
        cfg.readable         =  true;
        cfg.invert           = false;
        cfg.rgb_order        = false;
        cfg.dlen_16bit       = false;
        cfg.bus_shared       = false; // 変更

        _panel_instance.config(cfg);
      }
     {
        auto cfg = _light_instance.config();

        cfg.pin_bl = 21;              // 変更
        cfg.invert = false;
        cfg.freq   = 44100;
        cfg.pwm_channel = 7;

        _light_instance.config(cfg);
        _panel_instance.setLight(&_light_instance);
      }

      {
        auto cfg = _touch_instance.config();

        cfg.x_min      =  300;  // 変更
        cfg.x_max      = 3900;  // 変更
        cfg.y_min      = 3700;  // 変更
        cfg.y_max      =  200;  // 変更
        cfg.pin_int    = -1;    // 変更
        cfg.bus_shared = false; // 変更
        cfg.offset_rotation = 0;

        cfg.spi_host = HSPI_HOST; // 変更
        cfg.freq = 1000000;
        cfg.pin_sclk = 25;        // 変更
        cfg.pin_mosi = 32;        // 変更
        cfg.pin_miso = 39;        // 変更
        cfg.pin_cs   = 33;        // 変更

        _touch_instance.config(cfg);
        _panel_instance.setTouch(&_touch_instance);
      }
    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }
};
