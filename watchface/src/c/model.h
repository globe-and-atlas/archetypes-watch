#ifndef ARCHETYPES_MODEL_H
#define ARCHETYPES_MODEL_H

#include <stdbool.h>
#include <stddef.h>

#define AT_FACE_COUNT 16
#define AT_PHRASE_MAX 18

typedef enum {
  AT_INTJ_OBSERVATORY,
  AT_INTP_SCRATCHPAD,
  AT_ENTJ_COMMAND,
  AT_ENTP_BRANCHES,
  AT_INFJ_MARGIN_NOTES,
  AT_INFP_PAPER_GARDEN,
  AT_ENFJ_BEACON,
  AT_ENFP_OPEN_SKY,
  AT_ISTJ_STATION_HOUSE,
  AT_ISFJ_KEEPSAKE,
  AT_ESTJ_LEDGER,
  AT_ESFJ_GATHERING,
  AT_ISTP_FIELD_KIT,
  AT_ISFP_COLOR_STUDY,
  AT_ESTP_VELOCITY,
  AT_ESFP_FESTIVAL
} AtFace;

int at_choice(int value, int count, int fallback);
const char *at_face_code(int face);
const char *at_face_name(int face);
void at_format_time(int hour, int minute, int format, bool watch_24h, char *out, size_t capacity);
void at_format_date(int year, int month, int day, int format, char *out, size_t capacity);
void at_clean_phrase(const char *source, char *out, size_t capacity);

#endif
