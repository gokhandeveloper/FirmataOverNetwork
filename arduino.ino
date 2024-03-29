/*
 * arduino.ino generated by FirmataBuilder
 * Tue Apr 04 2019 21:29:34 GMT-0400 (EDT)
 */

#include <ConfigurableFirmata.h>

#include <Bridge.h>
#include <YunClient.h>
#include <EthernetClientStream.h>

YunClient client;

const byte mac[] = {0xDE, 0xAA, 0xBB, 0xCC, 0xDD, 0x01};
int remotePort = 3030;
IPAddress remoteIp(192, 168, 0, 225);
EthernetClientStream stream(client, IPAddress(0, 0, 0, 0), remoteIp, NULL, remotePort);

#include <DigitalInputFirmata.h>
DigitalInputFirmata digitalInput;

#include <DigitalOutputFirmata.h>
DigitalOutputFirmata digitalOutput;

#include <AnalogInputFirmata.h>
AnalogInputFirmata analogInput;

#include <AnalogOutputFirmata.h>
AnalogOutputFirmata analogOutput;

#include <OneWireFirmata.h>
OneWireFirmata oneWire;

#include <StepperFirmata.h>
StepperFirmata stepper;

#include <FirmataExt.h>
FirmataExt firmataExt;

#include <AnalogWrite.h>

#include <FirmataReporting.h>
FirmataReporting reporting;

void systemResetCallback()
{
  for (byte i = 0; i < TOTAL_PINS; i++) {
    if (IS_PIN_ANALOG(i)) {
      Firmata.setPinMode(i, ANALOG);
    } else if (IS_PIN_DIGITAL(i)) {
      Firmata.setPinMode(i, OUTPUT);
    }
  }
  firmataExt.reset();
}

void initTransport()
{
  Bridge.begin();

  Firmata.begin(stream);
}

void initFirmata()
{
  Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);

  firmataExt.addFeature(digitalInput);
  firmataExt.addFeature(digitalOutput);
  firmataExt.addFeature(analogInput);
  firmataExt.addFeature(analogOutput);
  firmataExt.addFeature(oneWire);
  firmataExt.addFeature(stepper);
  firmataExt.addFeature(reporting);

  Firmata.attach(SYSTEM_RESET, systemResetCallback);
}

void setup()
{
  initFirmata();

  initTransport();

  systemResetCallback();
}

void loop()
{
  digitalInput.report();

  while(Firmata.available()) {
    Firmata.processInput();
  }

  if (reporting.elapsed()) {
    analogInput.report();
  }

  stepper.update();
}
