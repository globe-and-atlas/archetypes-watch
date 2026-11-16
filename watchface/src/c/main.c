#include <pebble.h>
#include <string.h>

#include "face_renderer.h"
#include "model.h"

enum {
  STORE_FACE = 1,
  STORE_TIME_FORMAT = 2,
  STORE_DATE_FORMAT = 3,
  STORE_RETIRED_BATTERY_MODE = 4, // Keep later persistence keys stable across upgrades.
  STORE_PALETTE = 5,
  STORE_DECORATION = 6,
  STORE_PHRASE_MODE = 7,
  STORE_CUSTOM_PHRASE = 8,
  STORE_STATION_ARCH = 9
};

static Window *s_window;
static Layer *s_canvas;
static AtSettings s_settings;

static int stored_choice(int key, int count, int fallback) {
  return persist_exists(key) ? at_choice(persist_read_int(key), count, fallback) : fallback;
}

static void load_settings(void) {
  s_settings.face = stored_choice(STORE_FACE, AT_FACE_COUNT, AT_ISTJ_STATION_HOUSE);
  s_settings.time_format = stored_choice(STORE_TIME_FORMAT, 3, 0);
  s_settings.date_format = stored_choice(STORE_DATE_FORMAT, 4, 0);
  s_settings.palette = stored_choice(STORE_PALETTE, 3, 0);
  s_settings.decoration = stored_choice(STORE_DECORATION, 2, 0);
  s_settings.phrase_mode = stored_choice(STORE_PHRASE_MODE, 3, 0);
  s_settings.station_arch = stored_choice(STORE_STATION_ARCH, 2, 1) == 1;
  s_settings.custom_phrase[0] = '\0';
  if (persist_exists(STORE_CUSTOM_PHRASE)) {
    char stored[AT_PHRASE_MAX + 1] = {0};
    persist_read_string(STORE_CUSTOM_PHRASE, stored, sizeof(stored));
    at_clean_phrase(stored, s_settings.custom_phrase, sizeof(s_settings.custom_phrase));
  }
#if defined(AT_TEST_FACE)
  s_settings.face = AT_TEST_FACE;
#endif
#if defined(AT_TEST_TOUR)
  s_settings.face = 0;
#endif
}

static void update_canvas(Layer *layer, GContext *ctx) {
  time_t current = time(NULL);
  struct tm value = *localtime(&current);
#if defined(AT_TEST_FACE) || defined(AT_TEST_TOUR)
  value.tm_year = 126;
  value.tm_mon = 9;
  value.tm_mday = 23;
  value.tm_wday = 5;
  value.tm_hour = 10;
  value.tm_min = 8;
#endif
  at_render(layer, ctx, &s_settings, &value);
}

static void redraw(void) {
  if (s_canvas) layer_mark_dirty(s_canvas);
}

static void on_tick(struct tm *tick_time, TimeUnits changed) {
  (void)tick_time;
  (void)changed;
  redraw();
}

static void set_choice(int *field, int value, int count, int storage_key) {
  if (value < 0 || value >= count) return;
  if (*field == value) return;
  *field = value;
  persist_write_int(storage_key, value);
  redraw();
}

static void on_message(DictionaryIterator *iter, void *context) {
  (void)context;
  for (Tuple *item = dict_read_first(iter); item; item = dict_read_next(iter)) {
    if (item->key == MESSAGE_KEY_CustomPhrase) {
      if (item->type != TUPLE_CSTRING) continue;
      char cleaned[AT_PHRASE_MAX + 1];
      at_clean_phrase(item->value->cstring, cleaned, sizeof(cleaned));
      if (strcmp(cleaned, s_settings.custom_phrase) != 0) {
        strcpy(s_settings.custom_phrase, cleaned);
        persist_write_string(STORE_CUSTOM_PHRASE, cleaned);
        redraw();
      }
      continue;
    }
    if (item->type != TUPLE_INT && item->type != TUPLE_UINT) continue;
    int value = item->value->int32;
    if (item->key == MESSAGE_KEY_Face) {
#if !defined(AT_TEST_FACE) && !defined(AT_TEST_TOUR)
      set_choice(&s_settings.face, value, AT_FACE_COUNT, STORE_FACE);
#endif
    } else if (item->key == MESSAGE_KEY_TimeFormat) {
      set_choice(&s_settings.time_format, value, 3, STORE_TIME_FORMAT);
    } else if (item->key == MESSAGE_KEY_DateFormat) {
      set_choice(&s_settings.date_format, value, 4, STORE_DATE_FORMAT);
    } else if (item->key == MESSAGE_KEY_Palette) {
      set_choice(&s_settings.palette, value, 3, STORE_PALETTE);
    } else if (item->key == MESSAGE_KEY_Decoration) {
      set_choice(&s_settings.decoration, value, 2, STORE_DECORATION);
    } else if (item->key == MESSAGE_KEY_PhraseMode) {
      set_choice(&s_settings.phrase_mode, value, 3, STORE_PHRASE_MODE);
    } else if (item->key == MESSAGE_KEY_StationArch && (value == 0 || value == 1)) {
      s_settings.station_arch = value == 1;
      persist_write_int(STORE_STATION_ARCH, value);
      redraw();
    }
  }
}

#if defined(AT_TEST_TOUR)
static void on_tap(AccelAxisType axis, int32_t direction) {
  (void)axis;
  (void)direction;
  s_settings.face = (s_settings.face + 1) % AT_FACE_COUNT;
  APP_LOG(APP_LOG_LEVEL_INFO, "Archetypes fixture face %d %s", s_settings.face,
          at_face_code(s_settings.face));
  redraw();
}
#endif

static void init(void) {
  load_settings();
  s_window = window_create();
  Layer *root = window_get_root_layer(s_window);
  s_canvas = layer_create(layer_get_bounds(root));
  layer_set_update_proc(s_canvas, update_canvas);
  layer_add_child(root, s_canvas);
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, on_tick);
  app_message_register_inbox_received(on_message);
  app_message_open(512, 64);
#if defined(AT_TEST_TOUR)
  accel_tap_service_subscribe(on_tap);
#endif
}

static void deinit(void) {
#if defined(AT_TEST_TOUR)
  accel_tap_service_unsubscribe();
#endif
  app_message_deregister_callbacks();
  tick_timer_service_unsubscribe();
  at_render_deinit();
  layer_destroy(s_canvas);
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
