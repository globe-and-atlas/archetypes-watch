#ifndef ARCHETYPES_FACE_RENDERER_H
#define ARCHETYPES_FACE_RENDERER_H

#include <pebble.h>
#include "model.h"

typedef struct {
  int face;
  int time_format;
  int date_format;
  int palette;
  int decoration;
  int phrase_mode;
  bool station_arch;
  char custom_phrase[AT_PHRASE_MAX + 1];
} AtSettings;

void at_render(Layer *layer, GContext *ctx, const AtSettings *settings, const struct tm *now);
void at_render_deinit(void);

#endif
