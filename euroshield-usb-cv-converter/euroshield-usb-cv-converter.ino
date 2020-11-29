#include <Audio.h>
#include <Wire.h>
#include <SPI.h>

AudioInputUSB            usbIn;
AudioInputI2S            i2sIn;
AudioOutputUSB           usbOut;
AudioOutputI2S           i2sOut;
AudioConnection          patchCord1(usbIn, 0, i2sOut, 0);
AudioConnection          patchCord2(usbIn, 1, i2sOut, 1);
AudioConnection          patchCord3(i2sIn, 0, usbOut, 0);
AudioConnection          patchCord4(i2sIn, 1, usbOut, 1);
AudioControlSGTL5000     audioController;

void setup()
{
  AudioMemory(12);

  audioController.enable();
  audioController.inputSelect(AUDIO_INPUT_LINEIN);
  audioController.volume(0.82);
  audioController.adcHighPassFilterDisable();
  audioController.lineInLevel(0,0);
  audioController.unmuteHeadphone();
}

void loop()
{
}
