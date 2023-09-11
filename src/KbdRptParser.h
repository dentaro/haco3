#include <Arduino.h>
#include <FS.h>
#include "SPIFFS.h"
#include "string.h"
#include <hidboot.h>
#include <usbhub.h>
#include <regex>

#ifndef KBDRPTPARSER_H
#define KBDRPTPARSER_H

extern int keyState;

class KbdRptParser : public KeyboardReportParser
{
  private:
      //エディタ、キーイベント用
    int keyDownMillis = -1;
    uint8_t gkeycode = 0;
    uint8_t gasciicode = 0;

    bool shiftF = false;
    

    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
    
  public:
    uint8_t getKeycode();
    bool getShiftF();
    uint8_t getasciicode();
    void setKeycode(uint8_t _keycode);
    int getKeyDownMillis();
    uint8_t keyremap(uint8_t mod, uint8_t _keycode);

};


#endif
