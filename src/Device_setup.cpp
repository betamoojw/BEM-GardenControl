#include "Device_setup.h"
#include <stdint.h>
#include <Arduino.h>
#include "BEM_hardware.h"
#include "I2C_IOExpander.h"
#include "ReadBinary.h"
#include "Input_Impulse.h"
#include "ReadADC.h"
#include "GardenControlDevice.h"

uint8_t hw_ID = 0;
uint8_t hw_ID_Bot = 0;

void init_GPIOs()
{
}

uint8_t get_PROG_LED_PIN()
{
  switch (hw_ID)
  {
  case HW_1_0: // V1.x
    return 24;
    break;
  default:
    return 255;
    break;
  }
}

uint8_t get_PROG_BUTTON_PIN()
{
  switch (hw_ID)
  {
  case HW_1_0: // V1.x
    return 25;
    break;

  default:
    return 250;
    break;
  }
}

uint8_t get_SAVE_INTERRUPT_PIN()
{
  switch (hw_ID)
  {
  case HW_1_0: // V1.x
    return 23;
    break;

  default:
    return 252;
    break;
  }
}

uint8_t get_SSR_EN_PIN()
{
  switch (hw_ID)
  {
  case HW_1_0: // V1.x
    return GPIO_SSR_EN;
    break;
  default:
    return 255;
    break;
  }
}

uint8_t get_5V_EN_PIN()
{
  switch (hw_ID)
  {
  case HW_1_0: // V1.x
    return GPIO_5V_EN;
    break;
  default:
    return 255;
    break;
  }
}

uint8_t get_5V_status_PIN()
{
  switch (hw_ID)
  {
  case HW_1_0: // V1.x
    return GPIO_5V_status;
    break;
  default:
    return 255;
    break;
  }
}

uint8_t get_5V_fault_PIN()
{
  switch (hw_ID)
  {
  case HW_1_0: // V1.x
    return IO_5V_fault;
    break;
  default:
    return 255;
    break;
  }
}

void print_HW_ID_TOP(uint8_t id)
{
  SERIAL_PORT.print("  HW-ID-TOP: ");
  switch (id)
  {
  case HW_1_0:
    SERIAL_PORT.println("V1.x");
    break;

  default:
    SERIAL_PORT.println("Not Defined");
    break;
    break;
  }
}

void print_HW_ID_BOT(uint8_t id)
{
  SERIAL_PORT.print("  HW-ID-BOT: ");
  switch (id)
  {
  case HW_BOT_1_0:
    SERIAL_PORT.println("V1.x");
    break;

  default:
    SERIAL_PORT.print("Not Defined: ");
    SERIAL_PORT.println(id,BIN);
    break;
  }
}

void read_HW_ID_TOP()
{
  // Set Inputs
  pinMode(ID1, INPUT_PULLUP);
  pinMode(ID2, INPUT_PULLUP);
  pinMode(ID3, INPUT_PULLUP);
  // read Inputs
  bitWrite(hw_ID, 0, digitalRead(ID1));
  bitWrite(hw_ID, 1, digitalRead(ID2));
  bitWrite(hw_ID, 2, digitalRead(ID3));
}

uint8_t get_HW_ID_TOP()
{
  return hw_ID;
}

void read_HW_ID_BOT()
{
  // read Inputs
  bitWrite(hw_ID_Bot, 0, get_IOExpander_Input(IO_HW_ID1));
  bitWrite(hw_ID_Bot, 1, get_IOExpander_Input(IO_HW_ID2));
  bitWrite(hw_ID_Bot, 2, get_IOExpander_Input(IO_HW_ID3));
}

uint8_t get_HW_ID_BOT()
{
  return hw_ID_Bot;
}

void initHW()
{
  switch (hw_ID)
  {
  case HW_1_0:
    // RP2040 GPIO Init
    pinMode(get_PROG_LED_PIN(),OUTPUT);
    pinMode(get_SSR_EN_PIN(), OUTPUT);
    pinMode(get_5V_EN_PIN(), OUTPUT);
    pinMode(get_5V_status_PIN(), INPUT);
    pinMode(OptoIN_1, INPUT);
    pinMode(OptoIN_2, INPUT);
    pinMode(OptoIN_3, INPUT);
    pinMode(OptoIN_4, INPUT);

    digitalWrite(PROG_LED_PIN, LOW);
    digitalWrite(GPIO_SSR_EN, LOW);
    digitalWrite(GPIO_5V_EN, LOW);

    

    
    break;

  default:
    SERIAL_PORT.println("Wrong HW-ID  = STOPP");
    while (true)
    {
    };
    break;
  }
}

void initHW_Top()
{
  switch (hw_ID)
  {
  case HW_1_0:
    SERIAL_PORT.println("  I2C.begin");
    // I2C Init
    // Wire.setSDA(20);
    // Wire.setSCL(21);

    Wire1.setSDA(14);
    Wire1.setSCL(15);

    // Wire.begin();
    Wire1.begin();

    init_IOExpander_GPIOs_TOP();

    initADC_TOP(Resolution16Bit);
    break;
  
  default:
    SERIAL_PORT.println("Wrong HW-ID  = STOPP");
    while (true)
    {
    };
    break;
  }

}

void initHW_Bot()
{
  switch (hw_ID_Bot)
  {
  case HW_BOT_1_0:

    init_IOExpander_GPIOs_BOT();

#ifdef ADC_enable
    initADC_BOT(Resolution16Bit);
#endif
    break;

  default:
    break;
  }
}