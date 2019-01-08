#include "BemaniConLib.h"

#define LED_REPORT_ID 5
#define GAMEPAD_REPORT_ID 4
#define MOUSE_REPORT_ID 3

static const uint8_t _hidReportDescriptor[] PROGMEM = 
{
    // Mouse Device
    0x05, 0x01, /* USAGE_PAGE (Generic Desktop) */
    0x09, 0x02, /* USAGE (Mouse) */
    0xa1, 0x01, /* COLLECTION (Application) */
    0x09, 0x01, /*   USAGE (Pointer) */
    0xa1, 0x00, /*   COLLECTION (Physical) */
    0x85, MOUSE_REPORT_ID, /*     REPORT_ID */
    0x05, 0x09, /*     USAGE_PAGE (Button) */
    0x19, 0x01, /*     USAGE_MINIMUM (Button 1) */
    0x29, 0x03, /*     USAGE_MAXIMUM (Button 3) */
    0x15, 0x00, /*     LOGICAL_MINIMUM (0) */
    0x25, 0x01, /*     LOGICAL_MAXIMUM (1) */
    0x95, 0x03, /*     REPORT_COUNT (3) */
    0x75, 0x01, /*     REPORT_SIZE (1) */
    0x81, 0x02, /*     INPUT (Data,Var,Abs) */
    0x95, 0x01, /*     REPORT_COUNT (1) */
    0x75, 0x05, /*     REPORT_SIZE (5) */
    0x81, 0x03, /*     INPUT (Cnst,Var,Abs) */
    0x05, 0x01, /*     USAGE_PAGE (Generic Desktop) */
    0x09, 0x30, /*     USAGE (X) */
    0x09, 0x31, /*     USAGE (Y) */
    0x09, 0x38, /*     USAGE (Wheel) */
    0x15, 0x81, /*     LOGICAL_MINIMUM (-127) */
    0x25, 0x7f, /*     LOGICAL_MAXIMUM (127) */
    0x75, 0x08, /*     REPORT_SIZE (8) */
    0x95, 0x03, /*     REPORT_COUNT (3) */
    0x81, 0x06, /*     INPUT (Data,Var,Rel) */
    0xc0,       /*   END_COLLECTION */
    0xc0,       /* END_COLLECTION */
    
    // Gamepad Device
    0x05, 0x01, /* USAGE_PAGE (Generic Desktop) */ 
    0x09, 0x05, /* USAGE (Game Pad) */ 
    0xa1, 0x01, /* COLLECTION (Application) */ 
    0x85, GAMEPAD_REPORT_ID, /*   REPORT_ID */
    
    // Buttons
    0x05, 0x09, /*     USAGE_PAGE (Button) */ 
    0x19, 0x01, /*     USAGE_MINIMUM (Button 1) */ 
    0x29, 0x0b, /*     USAGE_MAXIMUM (Button 11)*/ 
    0x15, 0x00, /*     LOGICAL_MINIMUM (0) */ 
    0x25, 0x01, /*     LOGICAL_MAXIMUM (1) */ 
    0x95, 0x0b, /*     REPORT_COUNT (11) */ 
    0x75, 0x01, /*     REPORT_SIZE (1) */ 
    0x81, 0x02, /*     INPUT (Data,Var,Abs) */ 
    
    // Padding
    0x95, 0x01, /*     REPORT_COUNT (1) */ 
    0x75, 0x05, /*     REPORT_SIZE (5) */ 
    0x81, 0x03, /*     INPUT (Cnst,Var,Abs) */ 
    
    // Axes 
    0x05, 0x01, /*     USAGE_PAGE (Generic Desktop) */ 
    0x09, 0x30, /*     USAGE (X) */ 
    0x09, 0x31, /*     USAGE (Y) */ 
    0x15, 0x81, /*     LOGICAL_MINIMUM (0) */ 
    0x25, 0x7f, /*     LOGICAL_MAXIMUM (255) */ 
    0x95, 0x02, /*     REPORT_COUNT (2) */ 
    0x75, 0x08, /*     REPORT_SIZE (8) */ 
    0x81, 0x02, /*     INPUT (Data,Var,Abs) */ 

    // Lights
    0x85, LED_REPORT_ID, /*   REPORT_ID */ 
    0x15, 0x00, /*     LOGICAL_MINIMUM (0) */ 
    0x25, 0x01, /*     LOGICAL_MAXIMUM (1) */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x01, /*     USAGE (Instance 1) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x02, /*     USAGE (Instance 2) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x03, /*     USAGE (Instance 3) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x04, /*     USAGE (Instance 4) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x05, /*     USAGE (Instance 5) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x06, /*     USAGE (Instance 6) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x07, /*     USAGE (Instance 7) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x08, /*     USAGE (Instance 8) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x09, /*     USAGE (Instance 9) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x0a, /*     USAGE (Instance 10) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x0b, /*     USAGE (Instance 11) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    0x05, 0x0a, /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x0c, /*     USAGE (Instance 12) */ 
    0xa1, 0x02, /*     COLLECTION (Logical) */ 
    0x05, 0x08, /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b, /*       USAGE (Generic Indicator 1) */ 
    0x75, 0x01, /*       REPORT_SIZE (1) */ 
    0x95, 0x01, /*       REPORT_COUNT (1) */ 
    0x91, 0x02, /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,       /*     END_COLLECTION */ 
    
    // Padding
    0x95, 0x01, /*   REPORT_COUNT (1) */ 
    0x75, 0x04, /*   REPORT_SIZE (4) */ 
    0x91, 0x03, /*   OUTPUT (Cnst,Var,Abs) */ 
    
    // End of Gamepad Device
    0xc0        /* END_COLLECTION */ 
};

BemaniCon_::BemaniCon_(void) 
{
    mouseData[0] = 0;
    mouseData[3] = 0;
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    HID().AppendDescriptor(&node);
}

void BemaniCon_::offset(int8_t x, int8_t y)
{
    gamePadData[2] += x;
    mouseData[1] = x;
    gamePadData[3] += y;
    mouseData[2] = y;
}

void BemaniCon_::xOffset(int8_t offset)
{
    gamePadData[2] += offset;
    mouseData[1] = offset;
}

void BemaniCon_::yOffset(int8_t offset)
{
    gamePadData[3] += offset;
    mouseData[2] = offset;
}

void BemaniCon_::press(uint8_t button)
{
    if(button < 8)
    {
        gamePadData[0] |= 1 << button;
    }
    else
    {
        gamePadData[1] |= 1 << (button - 8);
    }
}

void BemaniCon_::release(uint8_t button)
{
    if(button < 8)
    {
        gamePadData[0] &= ~(1 << button);
    }
    else
    {
        gamePadData[1] &= ~(1 << (button - 8));
    }
}

void BemaniCon_::sendReports()
{
    HID().SendReport(GAMEPAD_REPORT_ID, gamePadData, GAMEPAD_REPORT_SIZE);
    HID().SendReport(MOUSE_REPORT_ID, mouseData, MOUSE_REPORT_SIZE);
}

BemaniCon_ BemaniCon;
