#include "model.h"

#include <assert.h>
#include <string.h>

int main(void) {
  char text[32];
  assert(strcmp(at_face_code(4), "INFJ") == 0);
  assert(strcmp(at_face_name(8), "Station House") == 0);
  assert(at_choice(-1, 16, 8) == 8);
  assert(at_choice(16, 16, 8) == 8);
  at_format_time(0, 8, 1, true, text, sizeof(text));
  assert(strcmp(text, "12:08") == 0);
  at_format_time(13, 8, 2, false, text, sizeof(text));
  assert(strcmp(text, "13:08") == 0);
  at_format_time(13, 8, 0, false, text, sizeof(text));
  assert(strcmp(text, "1:08") == 0);
  at_format_date(2026, 10, 23, 0, text, sizeof(text));
  assert(strcmp(text, "") == 0);
  at_format_date(2026, 10, 23, 1, text, sizeof(text));
  assert(strcmp(text, "2026-10-23") == 0);
  at_format_date(2026, 10, 23, 2, text, sizeof(text));
  assert(strcmp(text, "10/23/2026") == 0);
  at_format_date(2026, 10, 23, 3, text, sizeof(text));
  assert(strcmp(text, "23/10/2026") == 0);
  at_clean_phrase(" notice\nthe good ", text, sizeof(text));
  assert(strcmp(text, " NOTICETHE GOOD") == 0);
  at_clean_phrase("12345678901234567890", text, sizeof(text));
  assert(strlen(text) == AT_PHRASE_MAX);
  return 0;
}
