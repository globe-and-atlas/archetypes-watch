#include "model.h"

#include <ctype.h>
#include <stdio.h>

static const char *CODES[AT_FACE_COUNT] = {
  "INTJ", "INTP", "ENTJ", "ENTP", "INFJ", "INFP", "ENFJ", "ENFP",
  "ISTJ", "ISFJ", "ESTJ", "ESFJ", "ISTP", "ISFP", "ESTP", "ESFP"
};

static const char *NAMES[AT_FACE_COUNT] = {
  "Observatory", "Scratchpad", "Command", "Branches",
  "Margin Notes", "Paper Garden", "Beacon", "Open Sky",
  "Station House", "Keepsake", "Ledger", "Gathering",
  "Field Kit", "Color Study", "Velocity", "Festival"
};

int at_choice(int value, int count, int fallback) {
  return value >= 0 && value < count ? value : fallback;
}

const char *at_face_code(int face) {
  return CODES[at_choice(face, AT_FACE_COUNT, AT_ISTJ_STATION_HOUSE)];
}

const char *at_face_name(int face) {
  return NAMES[at_choice(face, AT_FACE_COUNT, AT_ISTJ_STATION_HOUSE)];
}

void at_format_time(int hour, int minute, int format, bool watch_24h, char *out, size_t capacity) {
  if (!out || !capacity) return;
  bool use_24h = format == 2 || (format == 0 && watch_24h);
  hour = at_choice(hour, 24, 0);
  minute = at_choice(minute, 60, 0);
  if (use_24h) {
    snprintf(out, capacity, "%02d:%02d", hour, minute);
  } else {
    int display_hour = hour % 12;
    if (!display_hour) display_hour = 12;
    snprintf(out, capacity, "%d:%02d", display_hour, minute);
  }
}

void at_format_date(int year, int month, int day, int format, char *out, size_t capacity) {
  if (!out || !capacity) return;
  out[0] = '\0';
  if (format == 0) return;
  if (format == 1) snprintf(out, capacity, "%04d-%02d-%02d", year, month, day);
  if (format == 2) snprintf(out, capacity, "%02d/%02d/%04d", month, day, year);
  if (format == 3) snprintf(out, capacity, "%02d/%02d/%04d", day, month, year);
}

void at_clean_phrase(const char *source, char *out, size_t capacity) {
  if (!out || !capacity) return;
  size_t end = 0;
  if (source) {
    for (size_t i = 0; source[i] && end + 1 < capacity && end < AT_PHRASE_MAX; i++) {
      unsigned char c = (unsigned char)source[i];
      if (c >= 32 && c <= 126) out[end++] = (char)toupper(c);
    }
  }
  while (end && out[end - 1] == ' ') end--;
  out[end] = '\0';
}
