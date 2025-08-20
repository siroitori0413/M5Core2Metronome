#include <M5Unified.h>

// メトロノーム音
# define MTONE 500

int bpm = 120;
float beat; // 1拍

boolean isStarted = false;

void calcBPM()
{
  beat = (60000 / bpm) / 4;
}

void playMetronome()
{
  M5.Speaker.tone( MTONE, beat, 0, true);
  M5.Power.setVibration(240);
  M5.Power.setLed(255);
  delay(beat);
  
  M5.Power.setVibration(0);
  M5.Power.setLed(0);
  delay(beat * 3);
}

void setup(void) {
  auto cfg = M5.config();
  M5.begin(cfg);

  redrawDisplayText();
  
  calcBPM();
}

void redrawDisplayText(){
  M5.Display.clear(TFT_BLACK);
  M5.Display.setTextDatum(middle_center);
  M5.Display.setTextFont(&fonts::Orbitron_Light_24);
  M5.Display.setTextSize(1);
  M5.Display.drawString("TEMPO [ " + String(bpm) + " ]", M5.Display.width() / 2, M5.Display.height() / 2);
}

void loop(void) {
  M5.update();

  if (isStarted){
    // メトロノーム処理
    playMetronome();
  }
  if (M5.BtnA.wasPressed()){
	  M5.Speaker.tone( 400, 100, 0, true);
	  M5.Speaker.tone(1200, 100, 0, false);
    calcBPM();
    isStarted = !isStarted;
  }
  if (M5.BtnB.wasPressed()){
	  M5.Speaker.tone( 400, 100, 0, true);
	  M5.Speaker.tone(1200, 100, 0, false);
    bpm--;
    redrawDisplayText();
  }
  if (M5.BtnC.wasPressed()){
	  M5.Speaker.tone( 400, 100, 0, true);
	  M5.Speaker.tone(1200, 100, 0, false);
    bpm++;
    redrawDisplayText();
  }
}