#include "BemaniConLib.h"
#include "Keyboard.h"

// Configuration
// Quadrature inversion
#define INVERT_QUAD_1 0
#define INVERT_QUAD_2 0

// Debounce delay.
#define DEBOUNCE_DELAY 4

char keyPadCodes[] =
{
    KEYPAD_1, KEYPAD_4, KEYPAD_7, KEYPAD_0,    // 1 4 7 0
    KEYPAD_2, KEYPAD_5, KEYPAD_8, KEYPAD_ADD,  // 2 5 8 F
    KEYPAD_3, KEYPAD_6, KEYPAD_9, KEYPAD_DOT,  // 3 6 9 E
    KEY_F1, KEY_F2, KEY_F3, KEY_F4             // A B C D
};

// End of configuration
// Pin assignments
#define OPTO_A1 3   // PD0
#define OPTO_A2 2   // PD1
#define OPTO_B1 0   // PD2
#define OPTO_B2 1   // PD3

#define W1_SENSE PD4
#define W2_SENSE PC6
#define W3_SENSE PD7
#define W4_SENSE PE6
#define B1_SENSE PB4
#define B2_SENSE PB5
#define B3_SENSE PB6
#define ST_SENSE PB7
#define SE_SENSE PD6
#define B0_SENSE PB0
#define D5_SENSE PD5
#define ROW1     PC7
#define ROW2     PF7
#define ROW3     PF6
#define ROW4     PF5

#define DS       PF4
#define ST_CP    PF1
#define SH_CP    PF0

// Convenience macros
#define BITREAD(X, I) (((unsigned)(X) >> (I)) & 1)
#define POSBIT(I) (1 << I)
#define NEGBIT(I) (~(1 << I))

// Modes for modifying the shift register
#define KP_ONLY 0
#define LIGHTS_ONLY 1
#define ALL 2

// Quadrature Management
int8_t encoderLut[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
uint8_t quad1val = 0;
uint8_t quad2val = 0;
volatile long quad1pos = 0;
volatile long quad2pos = 0;
long prevQuad1pos = 0;
long prevQuad2pos = 0;

// Shift Register Data
byte kpNybble = 0;
byte lightsUpperNybble = 0;
byte lowerByte = 0;

// Button State
byte prevStates[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned long debounces[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int prevPressed = 16;

// Globals
unsigned long currTime;
unsigned long debounceTime;
byte B;
byte C;
byte D;
byte E;
byte F;
long state = 0;
byte states[11];
int i;
char pressed;
uint16_t reactiveLights;
long x;
long y;
extern volatile uint16_t led;
extern volatile long lastUpdate;

void lightUpdate()
{
    if(debounceTime - lastUpdate > 1000)
    {
        shiftIntoShiftRegister(reactiveLights, LIGHTS_ONLY);
    }
    else
    {
        shiftIntoShiftRegister(led, LIGHTS_ONLY);
    }
}

void quad1Int()
{
    quad1val <<= 2;
    quad1val |= (PIND & 0x03);
    quad1val &= 0x0F;
#if INVERT_QUAD_1 == 0
    quad1pos += encoderLut[quad1val & 0x0F];
#else
    quad1pos -= encoderLut[quad1val & 0x0F];
#endif
}

void quad2Int()
{
    quad2val <<= 2;
    quad2val |= ((PIND >> 2) & 0x03);
    quad2val &= 0x0F;
#if INVERT_QUAD_2 == 0
    quad2pos += encoderLut[quad2val & 0x0F];
#else
    quad2pos -= encoderLut[quad2val & 0x0F];
#endif
}

char readKeypad()
{
    pressed = 16;

    // Column 1
    shiftIntoShiftRegister(0xE, KP_ONLY);
    C = PINC;
    F = PINF;
    if(!BITREAD(C, 7)) pressed = 0;
    if(!BITREAD(F, 7)) pressed = 1;
    if(!BITREAD(F, 6)) pressed = 2;
    if(!BITREAD(F, 5)) pressed = 3;

    // Column 2
    shiftIntoShiftRegister(0xD, KP_ONLY);
    C = PINC;
    F = PINF;
    if(!BITREAD(C, 7)) pressed = 4;
    if(!BITREAD(F, 7)) pressed = 5;
    if(!BITREAD(F, 6)) pressed = 6;
    if(!BITREAD(F, 5)) pressed = 7;

    // Column 3
    shiftIntoShiftRegister(0xB, KP_ONLY);
    C = PINC;
    F = PINF;
    if(!BITREAD(C, 7)) pressed = 8;
    if(!BITREAD(F, 7)) pressed = 9;
    if(!BITREAD(F, 6)) pressed = 10;
    if(!BITREAD(F, 5)) pressed = 11;

    // Column 4
    shiftIntoShiftRegister(0x7, KP_ONLY);
    C = PINC;
    F = PINF;
    if(!BITREAD(C, 7)) pressed = 12;
    if(!BITREAD(F, 7)) pressed = 13;
    if(!BITREAD(F, 6)) pressed = 14;
    if(!BITREAD(F, 5)) pressed = 15;
}

void shiftIntoShiftRegister(unsigned int value, byte mode)
{
    if(mode == ALL)
    {
        lowerByte = value & 0xFF;
        lightsUpperNybble = (value >> 8) & 0x0F;
        kpNybble = (value >> 8) & 0x0F;
    }
    else if(mode == KP_ONLY)
    {
        kpNybble = value & 0x0F;
    }
    else
    {
        lowerByte = value & 0xFF;
        lightsUpperNybble = (value >> 8) & 0x0F;
    }

    // Pull SH_CP, ST_CP, and DS low.
    PORTF &= NEGBIT(4) & NEGBIT(1) & NEGBIT(0);

    // Upper Nybble (KP).
    for(i = 3; i >= 0; --i)
    {
        if(kpNybble & (1<<i))
        {
            // SH_CP low and DS high.
            PORTF &= NEGBIT(0);
            PORTF |= POSBIT(4);
        }
        else
        {
            // SH_CP low and DS low.
            PORTF &= NEGBIT(0) & NEGBIT(4);
        }

        // Pull SH_CP high and then DS low.
        PORTF |= POSBIT(0);
        PORTF &= NEGBIT(4);
    }

    // Upper lights nybble.
    for(i = 3; i >= 0; --i)
    {
        if(lightsUpperNybble & (1<<i))
        {
            // SH_CP low and DS high.
            PORTF &= NEGBIT(0);
            PORTF |= POSBIT(4);
        }
        else
        {
            // SH_CP low and DS low.
            PORTF &= NEGBIT(0) & NEGBIT(4);
        }

        // Pull SH_CP high and then DS low.
        PORTF |= POSBIT(0);
        PORTF &= NEGBIT(4);
    }

    // Lower byte.
    for(i = 7; i >= 0; --i)
    {
        if(lowerByte & (1<<i))
        {
            // SH_CP low and DS high.
            PORTF &= NEGBIT(0);
            PORTF |= POSBIT(4);
        }
        else
        {
            // SH_CP low and DS low.
            PORTF &= NEGBIT(0) & NEGBIT(4);
        }

        // Pull SH_CP high and then DS low.
        PORTF |= POSBIT(0);
        PORTF &= NEGBIT(4);
    }

    // ST_CP high.
    PORTF |= POSBIT(1);
}

void setup()
{
    // 1 -> output
    // 0 -> input
    // Set up IO pins.
    // All the bit defined in the following lines are inputs with pull-up
    // enabled. All the others are set to output.
    DDRB   = NEGBIT(7) & NEGBIT(6) & NEGBIT(5) & NEGBIT(4) & NEGBIT(0);
    PORTB |= POSBIT(7) | POSBIT(6) | POSBIT(5) | POSBIT(4) | POSBIT(0);
    DDRC   = NEGBIT(7) & NEGBIT(6);
    PORTC |= POSBIT(7) | POSBIT(6);
    DDRD   = NEGBIT(7) & NEGBIT(6) & NEGBIT(5) & NEGBIT(4) & NEGBIT(3) & NEGBIT(2) & NEGBIT(1) & NEGBIT(0);
    PORTD |= POSBIT(7) | POSBIT(6) | POSBIT(5) | POSBIT(4) | POSBIT(3) | POSBIT(2) | POSBIT(1) | POSBIT(0);
    DDRE   = NEGBIT(6);
    PORTE |= POSBIT(6);
    DDRF   = NEGBIT(7) & NEGBIT(6) & NEGBIT(5);
    PORTF |= POSBIT(7) | POSBIT(6) | POSBIT(5);

    // Initial state for shift register.
    shiftIntoShiftRegister(0xF000, ALL);

    // Set up interrupts for quadratures.
    attachInterrupt(digitalPinToInterrupt(OPTO_A1), quad1Int, CHANGE);
    attachInterrupt(digitalPinToInterrupt(OPTO_A2), quad1Int, CHANGE);
    attachInterrupt(digitalPinToInterrupt(OPTO_B1), quad2Int, CHANGE);
    attachInterrupt(digitalPinToInterrupt(OPTO_B2), quad2Int, CHANGE);

    Keyboard.begin();
}

void loop()
{
    // Snapshot current time.
    currTime = micros();
    x = 0;
    y = 0;

    // Get the current state of the first quadrature.
    state = quad1pos;
    x = prevQuad1pos - state;
    prevQuad1pos = state;

    // Get the current state of the second quadrature.
    state = quad2pos;
    y = prevQuad2pos - state;
    prevQuad2pos = state;
    
    BemaniCon.offset(x, y);
    
    // Read all the buttons.
    debounceTime = millis();
    B = PINB;
    C = PINC;
    D = PIND;
    E = PINE;
    F = PINF;
    states[0]  = BITREAD(D, 4);
    states[1]  = BITREAD(C, 6);
    states[2]  = BITREAD(D, 7);
    states[3]  = BITREAD(E, 6);
    states[4]  = BITREAD(B, 4);
    states[5]  = BITREAD(B, 5);
    states[6]  = BITREAD(B, 6);
    states[7]  = BITREAD(B, 7);
    states[8]  = BITREAD(D, 6);
    states[9]  = BITREAD(B, 0);
    states[10] = BITREAD(D, 5);

    reactiveLights = 0;
    for(i = 0; i < 11; i++)
    {
        if(!(states[i])) reactiveLights |= POSBIT(i);
        
        if(prevStates[i] != states[i] && debounceTime - debounces[i] >= DEBOUNCE_DELAY)
        {
            prevStates[i] = states[i];
            debounces[i] = debounceTime;
            if(!(states[i])) BemaniCon.press(i);
            else             BemaniCon.release(i);
        }
    }
    lightUpdate();
    BemaniCon.sendReports();
    
    readKeypad();
    if(pressed != prevPressed)
    {
        if(prevPressed != 16) Keyboard.release(keyPadCodes[prevPressed]);
        if(pressed != 16)     Keyboard.press(keyPadCodes[pressed]);
        prevPressed = pressed;
    }
}
