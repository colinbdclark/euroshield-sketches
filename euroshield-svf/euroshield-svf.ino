#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioInputI2S i2sIn;
AudioFilterStateVariable filter;
AudioOutputI2S i2sOut;
AudioConnection patchCord1(i2sIn, 0, filter, 0);
AudioConnection patchCord2(i2sIn, 1, filter, 1);
AudioConnection patchCord3(filter, 0, i2sOut, 0);
AudioConnection patchCord4(filter, 2, i2sOut, 1);
AudioControlSGTL5000 codec;

int upperPotInput = 20;
int lowerPotInput = 21;
const int kMaxPotInput = 1024;
const float kMinFilterQ = 0.7;
const float kMaxFilterQ = 4.3;

void setup() {
  AudioMemory(10);

  codec.enable();
  codec.inputSelect(AUDIO_INPUT_LINEIN);
  codec.volume(0.82);
  codec.adcHighPassFilterDisable();
  codec.lineInLevel(0, 0);
  codec.unmuteHeadphone();

  filter.octaveControl(4);
}

float normalizePotValue(int potValue) {
  return float(potValue) / float(kMaxPotInput);
}

float calcResonance(int resonancePotValue) {
  float normalizedResonancePotValue = normalizePotValue(resonancePotValue);
  float q = (normalizedResonancePotValue * kMaxFilterQ) + kMinFilterQ;

  return q;
}

void loop() {
  int cutoffPotValue = analogRead(upperPotInput);
  float normalizedCutoffPotValue = normalizePotValue(cutoffPotValue);
  float cutoffFreq = normalizedCutoffPotValue * 1000;

  int resonancePotValue = analogRead(lowerPotInput);
  float q = calcResonance(resonancePotValue);

  filter.frequency(cutoffFreq);
  filter.resonance(q);

  delay(5);
}
