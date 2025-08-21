#include <M5Unified.h>

// メトロノーム音
#define MTONE 500

typedef struct MetronomeState {
  bool is_started;
  uint8_t bpm;
  uint32_t previous_beat;
  uint32_t millis_offset;
} metronome_state_t;

static metronome_state_t g_state = {
  .is_started = false,
  .bpm = 120
};

void update_metronome(metronome_state_t *state) {
  if (!state->is_started) return;

  uint32_t current_millis = millis() - state->millis_offset;

  uint64_t bpm_millis = 60000 / state->bpm;
  uint64_t bpm_millis_div4 = bpm_millis / 4;

  uint32_t current_beat = current_millis / bpm_millis;
  uint32_t beat_remainder = current_millis % bpm_millis;

  if (current_beat != state->previous_beat) {
    M5.Speaker.tone(MTONE, bpm_millis_div4, 0, true);
    M5.Power.setVibration(240);
    M5.Power.setLed(255);
  } else if (beat_remainder >= bpm_millis_div4) {
    M5.Power.setVibration(0);
    M5.Power.setLed(0);
  }

  state->previous_beat = current_beat;
}

void redraw_display_text(metronome_state_t *state) {
  M5.Display.clear(TFT_BLACK);
  M5.Display.setTextDatum(middle_center);
  /* TODO: deprecated function */
  M5.Display.setTextFont(&fonts::Orbitron_Light_24);
  M5.Display.setTextSize(1);
  M5.Display.drawString("TEMPO [ " + String(state->bpm) + " ]",
                        M5.Display.width() / 2,
                        M5.Display.height() / 2);
}

void setup(void) {
  M5.begin(M5.config());

  redraw_display_text(&g_state);
}

void loop(void) {
  M5.update();

  // メトロノーム処理
  update_metronome(&g_state);

  if (M5.BtnA.wasPressed()) {
    M5.Power.setVibration(0);
    M5.Power.setLed(0);

    M5.Speaker.tone(400, 100, 0, true);
    M5.Speaker.tone(1200, 100, 0, false);

    g_state.millis_offset = millis();
    g_state.is_started = !g_state.is_started;
  }

  if (M5.BtnB.wasPressed()) {
    M5.Speaker.tone(400, 100, 0, true);
    M5.Speaker.tone(1200, 100, 0, false);

    g_state.bpm--;
    redraw_display_text(&g_state);
  }
  if (M5.BtnC.wasPressed()) {
    M5.Speaker.tone(400, 100, 0, true);
    M5.Speaker.tone(1200, 100, 0, false);

    g_state.bpm++;
    redraw_display_text(&g_state);
  }
}
