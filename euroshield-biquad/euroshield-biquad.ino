#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioInputI2S i2sIn;
AudioFilterBiquad lpf;
AudioFilterBiquad hpf;
AudioAnalyzePeak cutoffPeak;
AudioOutputI2S i2sOut;
AudioConnection patchCord1(i2sIn, 0, lpf, 0);
AudioConnection patchCord2(i2sIn, 0, hpf, 0);
AudioConnection patchCord3(i2sIn, 1, cutoffPeak, 0);
AudioConnection patchCord4(lpf, 0, i2sOut, 0);
AudioConnection patchCord5(hpf, 0, i2sOut, 1);
AudioControlSGTL5000 codec;

int upperPotInput = 20;
int lowerPotInput = 21;
const int kMaxPotInput = 1024;
const float kMinFilterCutoff = 40;
const float kMaxFilterCutoff = 22050;
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
  float cutoffFreq = normalizedCutoffPotValue * kMaxFilterCutoff * cutoffPeak.read();
  cutoffFreq = max(cutoffFreq, kMinFilterCutoff);
  
  int resonancePotValue = analogRead(lowerPotInput);
  float q = calcResonance(resonancePotValue);

  lpf.setLowpass(0, cutoffFreq, q);
  lpf.setLowpass(1, cutoffFreq, q);

  hpf.setHighpass(0, cutoffFreq, q);
  hpf.setHighpass(1, cutoffFreq, q);

  delay(5);
}
