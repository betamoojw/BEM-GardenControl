#include <Arduino.h>
#include <OpenKNX.h>
//#include <Logikmodul.h>
#include "GardenControl.h"
#include "Device_setup.h"
#include "BEM_hardware.h"
#include "I2C_IOExpander.h"
#include "GardenControlDevice.h"

uint32_t LED_Delay = 0;

bool TestLEDstate = false;

void appSetup();
void appLoop();

void setup()
{
  SERIAL_DEBUG.begin(115200);
  pinMode(PROG_LED_PIN, OUTPUT);
  digitalWrite(PROG_LED_PIN, HIGH);
  delay(6000);
  digitalWrite(PROG_LED_PIN, LOW);
  SERIAL_DEBUG.println("Start Appication");
  ArduinoPlatform::SerialDebug = &SERIAL_DEBUG;

#ifdef ARDUINO_ARCH_RP2040
  Serial1.setRX(17); // UART0 KNX
  Serial1.setTX(16); // UART0
#endif

#ifdef INFO_LED_PIN
  pinMode(INFO_LED_PIN, OUTPUT);
  // ledInfo(true);
#endif

  // all MAIN_* parameters are generated by OpenKNXproducer for correct version checking by ETS
  // If you want just a bugfix firmware update without ETS-Application dependency, just increase firmwareRevision.
  // As soon, as you want again a sync between ETS-Application and firmware, set firmwareRevision to 0.
  const uint8_t firmwareRevision = 0;
  OpenKNX::knxRead(MAIN_OpenKnxId, MAIN_ApplicationNumber, MAIN_ApplicationVersion, firmwareRevision);

  // pin or GPIO the programming led is connected to. Default is LED_BUILDIN
  knx.ledPin(PROG_LED_PIN);
  // is the led active on HIGH or low? Default is LOW
  knx.ledPinActiveOn(PROG_LED_PIN_ACTIVE_ON);
  // pin or GPIO programming button is connected to. Default is 0
  knx.buttonPin(PROG_BUTTON_PIN);
  // Is the interrup created in RISING or FALLING signal? Default is RISING
  knx.buttonPinInterruptOn(PROG_BUTTON_PIN_INTERRUPT_ON);

  // print values of parameters if device is already configured
  if (knx.configured())
    appSetup();

  // start the framework.
  knx.start();
  // ledInfo(false);
}

void loop()
{
  // don't delay here to much. Otherwise you might lose packages or mess up the timing with ETS
  knx.loop();

  // only run the application code if the device was configured with ETS
  if (knx.configured())
    appLoop();

  if (delayCheck(LED_Delay, 200))
  {
    TestLEDstate = !TestLEDstate;
    digitalWrite(get_PROG_LED_PIN(), TestLEDstate);
    LED_Delay = millis();
  }
}
