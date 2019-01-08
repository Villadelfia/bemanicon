#ifndef BemaniConLib_h
#define BemaniConLib_h

#include "HID.h"

#define GAMEPAD_REPORT_SIZE 4
#define MOUSE_REPORT_SIZE 4

class BemaniCon_
{
public:
    BemaniCon_();
    void press(uint8_t button);
    void release(uint8_t button);
    void xOffset(int8_t offset);
    void yOffset(int8_t offset);
    void offset(int8_t x, int8_t y);
    void sendReports();

private:
    uint8_t gamePadData[GAMEPAD_REPORT_SIZE];
    uint8_t mouseData[MOUSE_REPORT_SIZE];
    uint16_t buttonTmp;
};

extern BemaniCon_ BemaniCon;

#endif
