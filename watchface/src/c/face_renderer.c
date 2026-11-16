#include "face_renderer.h"

typedef struct {
  bool light;
  uint32_t background;
  uint32_t foreground;
  uint32_t accent;
  uint32_t soft;
} AtTheme;

typedef struct {
  GColor background;
  GColor foreground;
  GColor accent;
  GColor soft;
} AtColors;

static const AtTheme THEMES[AT_FACE_COUNT] = {
  {false, 0x09192B, 0xFFF4DC, 0x4AAFB1, 0x365573}, // Observatory
  {true,  0xF6F4EA, 0x17212B, 0x20A7B5, 0xB5C9C7}, // Scratchpad
  {false, 0x16191B, 0xFFF7E8, 0xFFAA22, 0x626768}, // Command
  {false, 0x0A1730, 0xEFFFF9, 0x46BBDD, 0x28567F}, // Branches
  {true,  0xFFFFFF, 0x101214, 0xA67542, 0xC9BCA6}, // Margin Notes
  {false, 0x405E48, 0xFFF9E6, 0xC7DBB0, 0x749375}, // Paper Garden
  {false, 0x063C47, 0xFFF6DA, 0xFFBB44, 0x5B9A9A}, // Beacon
  {false, 0x1552A5, 0xFFF6DE, 0xFFCA5B, 0x82B5D9}, // Open Sky
  {true,  0xF8EBCF, 0x17191B, 0xAE4936, 0xAA9779}, // Station House
  {true,  0xFBF2DD, 0x16212A, 0x567A9B, 0xB9AB93}, // Keepsake
  {true,  0xF1F2EB, 0x111A1E, 0x687F88, 0xC5D0CC}, // Ledger
  {true,  0xFFF0CE, 0x2A1C1B, 0xC2782C, 0xE7B66B}, // Gathering
  {false, 0x171B1D, 0xFFF5DF, 0xE3B567, 0x5E6465}, // Field Kit
  {false, 0x1B1724, 0xFFF7E9, 0xE66774, 0x6D4D8B}, // Color Study
  {false, 0x0642B5, 0xFFFDF5, 0xFF881F, 0x347BD1}, // Velocity
  {false, 0x081B37, 0xFFF9E8, 0xFFAA22, 0x4FBFD0}  // Festival
};

static GRect s_area;
static GBitmap *s_art_bitmap;
static int s_art_face = -1;

#if defined(PBL_COLOR)
static uint32_t art_id_for(int face) {
#if defined(PBL_PLATFORM_EMERY)
  static const uint32_t IDS[AT_FACE_COUNT] = {
    RESOURCE_ID_ART_00_EMERY, RESOURCE_ID_ART_01_EMERY, RESOURCE_ID_ART_02_EMERY,
    RESOURCE_ID_ART_03_EMERY, RESOURCE_ID_ART_04_EMERY, RESOURCE_ID_ART_05_EMERY,
    RESOURCE_ID_ART_06_EMERY, RESOURCE_ID_ART_07_EMERY, RESOURCE_ID_ART_08_EMERY,
    RESOURCE_ID_ART_09_EMERY, RESOURCE_ID_ART_10_EMERY, RESOURCE_ID_ART_11_EMERY,
    RESOURCE_ID_ART_12_EMERY, RESOURCE_ID_ART_13_EMERY, RESOURCE_ID_ART_14_EMERY,
    RESOURCE_ID_ART_15_EMERY
  };
#elif defined(PBL_PLATFORM_GABBRO)
  static const uint32_t IDS[AT_FACE_COUNT] = {
    RESOURCE_ID_ART_00_GABBRO, RESOURCE_ID_ART_01_GABBRO, RESOURCE_ID_ART_02_GABBRO,
    RESOURCE_ID_ART_03_GABBRO, RESOURCE_ID_ART_04_GABBRO, RESOURCE_ID_ART_05_GABBRO,
    RESOURCE_ID_ART_06_GABBRO, RESOURCE_ID_ART_07_GABBRO, RESOURCE_ID_ART_08_GABBRO,
    RESOURCE_ID_ART_09_GABBRO, RESOURCE_ID_ART_10_GABBRO, RESOURCE_ID_ART_11_GABBRO,
    RESOURCE_ID_ART_12_GABBRO, RESOURCE_ID_ART_13_GABBRO, RESOURCE_ID_ART_14_GABBRO,
    RESOURCE_ID_ART_15_GABBRO
  };
#else
  static const uint32_t IDS[AT_FACE_COUNT] = {
    RESOURCE_ID_ART_00_CHALK, RESOURCE_ID_ART_01_CHALK, RESOURCE_ID_ART_02_CHALK,
    RESOURCE_ID_ART_03_CHALK, RESOURCE_ID_ART_04_CHALK, RESOURCE_ID_ART_05_CHALK,
    RESOURCE_ID_ART_06_CHALK, RESOURCE_ID_ART_07_CHALK, RESOURCE_ID_ART_08_CHALK,
    RESOURCE_ID_ART_09_CHALK, RESOURCE_ID_ART_10_CHALK, RESOURCE_ID_ART_11_CHALK,
    RESOURCE_ID_ART_12_CHALK, RESOURCE_ID_ART_13_CHALK, RESOURCE_ID_ART_14_CHALK,
    RESOURCE_ID_ART_15_CHALK
  };
#endif
  return IDS[face];
}

static GBitmap *art_for(int face) {
  if (s_art_face != face) {
    if (s_art_bitmap) gbitmap_destroy(s_art_bitmap);
    s_art_bitmap = gbitmap_create_with_resource(art_id_for(face));
    s_art_face = face;
  }
  return s_art_bitmap;
}
#endif

void at_render_deinit(void) {
  if (s_art_bitmap) gbitmap_destroy(s_art_bitmap);
  s_art_bitmap = NULL;
  s_art_face = -1;
}

static int px(int unit) { return s_area.origin.x + unit * s_area.size.w / 100; }
static int py(int unit) { return s_area.origin.y + unit * s_area.size.h / 100; }
static int pw(int unit) { return unit * s_area.size.w / 100; }
static int ph(int unit) { return unit * s_area.size.h / 100; }
static int max1(int value) { return value > 0 ? value : 1; }

static void line(GContext *ctx, GColor color, int x1, int y1, int x2, int y2) {
  graphics_context_set_stroke_color(ctx, color);
  graphics_draw_line(ctx, GPoint(px(x1), py(y1)), GPoint(px(x2), py(y2)));
}

static void rect(GContext *ctx, GColor color, int x, int y, int w, int h) {
  graphics_context_set_fill_color(ctx, color);
  graphics_fill_rect(ctx, GRect(px(x), py(y), max1(pw(w)), max1(ph(h))), 0, GCornerNone);
}

static void outline(GContext *ctx, GColor color, int x, int y, int w, int h) {
  graphics_context_set_stroke_color(ctx, color);
  graphics_draw_rect(ctx, GRect(px(x), py(y), max1(pw(w)), max1(ph(h))));
}

static void circle(GContext *ctx, GColor color, int x, int y, int radius) {
  graphics_context_set_stroke_color(ctx, color);
  graphics_draw_circle(ctx, GPoint(px(x), py(y)), max1(pw(radius)));
}

static void dot(GContext *ctx, GColor color, int x, int y, int radius) {
  graphics_context_set_fill_color(ctx, color);
  graphics_fill_circle(ctx, GPoint(px(x), py(y)), max1(pw(radius)));
}

static void star(GContext *ctx, GColor color, int x, int y, int radius) {
  line(ctx, color, x - radius, y, x + radius, y);
  line(ctx, color, x, y - radius, x, y + radius);
}

static void thick_line(GContext *ctx, GColor color, int x1, int y1, int x2, int y2,
                       int thickness) {
  for (int i = 0; i < thickness; i++) line(ctx, color, x1, y1 + i, x2, y2 + i);
}

static void ellipse(GContext *ctx, GColor color, int cx, int cy, int rx, int ry) {
  static const int8_t X[17] = {100, 92, 71, 38, 0, -38, -71, -92, -100,
                              -92, -71, -38, 0, 38, 71, 92, 100};
  static const int8_t Y[17] = {0, 38, 71, 92, 100, 92, 71, 38, 0,
                              -38, -71, -92, -100, -92, -71, -38, 0};
  for (int i = 0; i < 16; i++) {
    line(ctx, color, cx + rx * X[i] / 100, cy + ry * Y[i] / 100,
         cx + rx * X[i + 1] / 100, cy + ry * Y[i + 1] / 100);
  }
}

static void flower(GContext *ctx, GColor stem, GColor petal, int x, int y) {
  dot(ctx, petal, x - 2, y, 2);
  dot(ctx, petal, x + 2, y, 2);
  dot(ctx, petal, x, y - 2, 2);
  dot(ctx, petal, x, y + 2, 2);
  dot(ctx, stem, x, y, 1);
}

static uint8_t digit_row(char digit, int row) {
  static const uint8_t glyphs[10][7] = {
    {14, 17, 19, 21, 25, 17, 14}, {4, 12, 4, 4, 4, 4, 14},
    {14, 17, 1, 2, 4, 8, 31}, {30, 1, 1, 14, 1, 1, 30},
    {2, 6, 10, 18, 31, 2, 2}, {31, 16, 16, 30, 1, 1, 30},
    {14, 16, 16, 30, 17, 17, 14}, {31, 1, 2, 4, 8, 8, 8},
    {14, 17, 17, 14, 17, 17, 14}, {14, 17, 17, 15, 1, 1, 14}
  };
  if (digit >= '0' && digit <= '9') return glyphs[digit - '0'][row];
  return digit == ':' && (row == 2 || row == 5) ? 1 : 0;
}

typedef struct {
  int x;
  int top;
  int width;
  int height;
  int scale;
} TimeLayout;

static TimeLayout time_layout(const char *value, int y, int max_scale) {
  int columns = 0;
  for (const char *p = value; *p; p++) columns += (*p == ':' ? 1 : 5) + 1;
  if (columns) columns--;
  int scale = s_area.size.w / (columns + 5);
  if (scale < 3) scale = 3;
  if (max_scale > 0 && scale > max_scale) scale = max_scale;
  int x = s_area.origin.x + (s_area.size.w - columns * scale) / 2;
  return (TimeLayout){x, py(y), columns * scale, 7 * scale, scale};
}

static void pixel_time(GContext *ctx, GColor color, const char *value, TimeLayout layout) {
  int x = layout.x;
  int top = layout.top;
  int scale = layout.scale;
  graphics_context_set_fill_color(ctx, color);
  for (const char *p = value; *p; p++) {
    int width = *p == ':' ? 1 : 5;
    for (int row = 0; row < 7; row++) {
      uint8_t bits = digit_row(*p, row);
      for (int col = 0; col < width; col++) {
        if (bits & (1 << (width - col - 1))) {
          graphics_fill_rect(ctx, GRect(x + col * scale, top + row * scale, scale, scale),
                             0, GCornerNone);
        }
      }
    }
    x += (width + 1) * scale;
  }
}

#if !defined(PBL_ROUND)
static void pixel_ledger_time(GContext *ctx, GColor color, const char *value) {
  int scale = s_area.size.w / 28;
  if (scale < 3) scale = 3;
  if (scale > 7) scale = 7;
  int divider = px(26);
  int gap = max1(pw(5));
  int top = py(21);
  char first[] = {value[0], '\0'};
  TimeLayout left = {divider - gap - 5 * scale, top, 5 * scale, 7 * scale, scale};
  TimeLayout right = {divider + gap, top, 19 * scale, 7 * scale, scale};
  pixel_time(ctx, color, first, left);
  pixel_time(ctx, color, value + 1, right);
  line(ctx, color, 26, 14, 26, 95);
}
#endif

static void label(GContext *ctx, GColor color, const char *value, int x, int y, int w, int h,
                  const char *font_key, GTextAlignment align) {
  graphics_context_set_text_color(ctx, color);
  graphics_draw_text(ctx, value, fonts_get_system_font(font_key),
                     GRect(px(x), py(y), max1(pw(w)), max1(ph(h))),
                     GTextOverflowModeTrailingEllipsis, align, NULL);
}

#if defined(PBL_PLATFORM_EMERY)
static void tracked_margin_phrase(GContext *ctx, GColor color) {
  static const char phrase[] = "NOTICE THE GOOD";
  const int advance = 9;
  int x = s_area.origin.x + (s_area.size.w - 15 * advance) / 2;
  int top = py(87);
  graphics_context_set_text_color(ctx, color);
  for (int i = 0; phrase[i]; i++, x += advance) {
    if (phrase[i] == ' ') continue;
    char letter[] = {phrase[i], '\0'};
    graphics_draw_text(ctx, letter, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
                       GRect(x, top, advance, ph(11)), GTextOverflowModeFill,
                       GTextAlignmentCenter, NULL);
  }
}
#endif

#if defined(PBL_COLOR)
static GColor rgb(uint32_t value) {
  return GColorFromHEX(value);
}

static GColor art_ground_for(int face) {
  static const uint32_t GROUNDS[AT_FACE_COUNT] = {
    0x000000, 0xFFFFFF, 0x000000, 0x000055,
    0xFFFFFF, 0x55AA55, 0x005555, 0x0055FF,
    0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
    0x000000, 0x000000, 0x0055FF, 0x000055
  };
  return rgb(GROUNDS[face]);
}
#endif

static AtColors colors_for(const AtSettings *settings) {
  AtTheme theme = THEMES[at_choice(settings->face, AT_FACE_COUNT, AT_ISTJ_STATION_HOUSE)];
#if defined(PBL_COLOR)
  if (settings->palette == 1) {
    theme.background = theme.light ? 0xFFF2DB : 0x241B19;
    theme.foreground = theme.light ? 0x281C18 : 0xFFF3D8;
    theme.accent = theme.light ? 0xAA573C : 0xFFB264;
    theme.soft = theme.light ? 0xCBB69A : 0x7D5741;
  } else if (settings->palette == 2) {
    theme.background = theme.light ? 0xE9F3F1 : 0x0B2036;
    theme.foreground = theme.light ? 0x102B38 : 0xF6FAF4;
    theme.accent = theme.light ? 0x287D92 : 0x55C4CD;
    theme.soft = theme.light ? 0xAAC8CA : 0x31516A;
  }
  return (AtColors){rgb(theme.background), rgb(theme.foreground), rgb(theme.accent), rgb(theme.soft)};
#else
  GColor bg = theme.light ? GColorWhite : GColorBlack;
  GColor fg = theme.light ? GColorBlack : GColorWhite;
  return (AtColors){bg, fg, fg, fg};
#endif
}

static void motif(GContext *ctx, const AtSettings *s, AtColors c) {
  const bool quiet = s->decoration == 1;
  switch (s->face) {
    case AT_INTJ_OBSERVATORY:
      for (int x = 10; x <= 90; x += 10) rect(ctx, c.soft, x, 7, 1, x % 20 ? 1 : 3);
      for (int y = 18; y <= 91; y += 9) {
        rect(ctx, c.soft, 6, y, y % 18 ? 1 : 3, 1);
        rect(ctx, c.soft, 93, y, y % 18 ? 1 : 3, 1);
      }
      ellipse(ctx, c.foreground, 50, 65, 39, 20);
      ellipse(ctx, c.soft, 50, 65, 40, 21);
      dot(ctx, c.foreground, 49, 65, 5);
      dot(ctx, c.accent, 78, 51, 2);
      if (!quiet) {
        star(ctx, c.foreground, 17, 21, 2); star(ctx, c.accent, 82, 28, 2);
        star(ctx, c.foreground, 22, 86, 2); star(ctx, c.accent, 79, 89, 2);
        dot(ctx, c.foreground, 61, 25, 1); dot(ctx, c.foreground, 36, 41, 1);
      }
      break;
    case AT_INTP_SCRATCHPAD:
      for (int x = 0; x <= 100; x += 8) line(ctx, c.soft, x, 0, x, 100);
      for (int y = 0; y <= 100; y += 8) line(ctx, c.soft, 0, y, 100, y);
      line(ctx, c.foreground, 0, 76, 12, 67); line(ctx, c.foreground, 12, 67, 25, 70);
      line(ctx, c.foreground, 25, 70, 39, 59); line(ctx, c.foreground, 39, 59, 53, 51);
      line(ctx, c.foreground, 53, 51, 66, 62); line(ctx, c.foreground, 66, 62, 80, 73);
      line(ctx, c.foreground, 80, 73, 100, 64);
      line(ctx, c.accent, 0, 91, 14, 89); line(ctx, c.accent, 14, 89, 28, 83);
      line(ctx, c.accent, 28, 83, 43, 78); line(ctx, c.accent, 43, 78, 58, 84);
      line(ctx, c.accent, 58, 84, 74, 69); line(ctx, c.accent, 74, 69, 91, 73);
      if (!quiet) {
        for (int x = 12; x <= 91; x += 16) rect(ctx, c.foreground, x, 76 + (x % 4), 2, 2);
        star(ctx, c.accent, 83, 42, 3); star(ctx, c.foreground, 28, 44, 2);
      }
      break;
    case AT_ENTJ_COMMAND:
      line(ctx, c.foreground, 9, 46, 91, 46);
      for (int i = 0; i < 4; i++) {
        int y = 52 + i * 10;
        rect(ctx, c.soft, 12, y, 73, 7);
        rect(ctx, i % 2 ? c.foreground : c.accent, 12, y, 24 + (i * 13) % 49, 7);
        rect(ctx, c.foreground, 8, y, 2, 7);
        if (!quiet) rect(ctx, c.accent, 88, y + 1, 2, 5);
      }
      if (!quiet) for (int i = 0; i < 12; i++) rect(ctx, i == 6 ? c.accent : c.soft, 10 + i * 7, 94, 1, i % 3 ? 2 : 4);
      break;
    case AT_ENTP_BRANCHES:
      for (int i = 0; i < 3; i++) {
        int y = 8 + i * 35;
        line(ctx, c.accent, 0, y, 12, y + 8); line(ctx, c.accent, 12, y + 8, 12, y + 17);
        line(ctx, c.accent, 12, y + 17, 30, y + 28); line(ctx, c.accent, 12, y + 8, 30, y - 1);
        line(ctx, c.soft, 100, y + 5, 84, y + 17); line(ctx, c.soft, 84, y + 17, 84, y + 25);
        line(ctx, c.soft, 84, y + 25, 68, y + 34); line(ctx, c.soft, 84, y + 17, 69, y + 8);
        outline(ctx, c.accent, 29, y + 26, 3, 3); outline(ctx, c.soft, 65, y + 31, 3, 3);
      }
      if (!quiet) for (int i = 0; i < 12; i++) rect(ctx, i % 2 ? c.accent : c.foreground,
                                                 10 + (i * 23) % 79, 6 + (i * 29) % 89, 2, 2);
      break;
    case AT_INFJ_MARGIN_NOTES:
#if defined(PBL_ROUND)
      line(ctx, c.foreground, 6, 15, 94, 15);
      line(ctx, c.foreground, 6, 76, 94, 76);
      star(ctx, c.foreground, 20, 11, 3);
#else
      line(ctx, c.foreground, 8, 20, 92, 20);
      line(ctx, c.foreground, 8, 77, 92, 77);
      line(ctx, c.foreground, 8, 96, 92, 96);
      line(ctx, c.foreground, 14, 9, 14, 97);
      star(ctx, c.foreground, 20, 14, 4);
      for (int i = 0; i < 8; i++) line(ctx, c.soft, 20 + i * 8, 76, 20 + i * 8, 77);
#endif
      break;
    case AT_INFP_PAPER_GARDEN:
      for (int i = 0; i < (quiet ? 3 : 5); i++) {
        int x = 8 + i * 22;
        int top = 56 + (i % 3) * 10;
        line(ctx, c.foreground, x, 100, x + (i % 2 ? 8 : -3), top);
        line(ctx, c.foreground, x + 1, 84, x - 5, 79);
        line(ctx, c.foreground, x + 2, 74, x + 8, 69);
        line(ctx, c.soft, x - 5, 79, x - 1, 78);
        line(ctx, c.soft, x + 8, 69, x + 3, 69);
        if (i % 2 == 0) flower(ctx, c.accent, c.foreground, x + (i % 2 ? 8 : -3), top);
      }
      for (int x = 0; x <= 100; x += 4) line(ctx, c.soft, x, 100, x + 2, 95 - x % 4);
      break;
    case AT_ENFJ_BEACON:
      for (int r = 14; r <= (quiet ? 26 : 40); r += 12) circle(ctx, c.accent, 50, 72, r);
      thick_line(ctx, c.soft, 0, 90, 22, 88, 2);
      thick_line(ctx, c.soft, 22, 88, 49, 94, 2);
      thick_line(ctx, c.soft, 49, 94, 72, 88, 2);
      thick_line(ctx, c.soft, 72, 88, 100, 91, 2);
      rect(ctx, c.foreground, 47, 69, 6, 24);
      rect(ctx, c.accent, 46, 67, 8, 3);
      rect(ctx, c.foreground, 48, 61, 4, 6);
      dot(ctx, c.accent, 50, 60, 2);
      if (!quiet) for (int i = 0; i < 8; i++) line(ctx, c.accent, i * 15, 95 + i % 3, i * 15 + 7, 95 + i % 3);
      break;
    case AT_ENFP_OPEN_SKY:
      star(ctx, c.accent, 20, 15, 3); star(ctx, c.foreground, 73, 12, 2);
      star(ctx, c.accent, 82, 54, 2); star(ctx, c.foreground, 32, 61, 2);
      if (!quiet) for (int i = 0; i < 15; i++) dot(ctx, i % 3 ? c.foreground : c.accent,
                                                  (i * 37 + 9) % 96, (i * 23 + 6) % 65, 1);
      dot(ctx, c.accent, 14, 48, 5); dot(ctx, c.background, 17, 46, 5);
      for (int x = 0; x < 100; x += 3) {
        int top = 75 + ((x / 11) % 3) * 3 - (x % 11) / 4;
        rect(ctx, c.soft, x, top, 3, 100 - top);
        rect(ctx, c.accent, x, top + 9, 3, 100 - top - 9);
        if (x % 9 == 0) rect(ctx, c.foreground, x, top + 5, 3, 1);
      }
      break;
    case AT_ISTJ_STATION_HOUSE:
#if defined(PBL_ROUND)
      if (s->station_arch) {
        rect(ctx, c.foreground, 18, 27, 64, 2);
        outline(ctx, c.foreground, 28, 13, 44, 14);
        for (int x = 32; x <= 68; x += 7) line(ctx, c.foreground, x, 18, x, 27);
        circle(ctx, c.foreground, 50, 13, 3);
      }
      rect(ctx, c.accent, 9, 31, 82, 3);
#else
      if (s->station_arch) {
        rect(ctx, c.foreground, 5, 47, 90, 3);
        rect(ctx, c.foreground, 10, 31, 18, 16);
        rect(ctx, c.foreground, 72, 31, 18, 16);
        rect(ctx, c.foreground, 23, 26, 54, 21);
        ellipse(ctx, c.foreground, 50, 28, 27, 16);
        ellipse(ctx, c.foreground, 50, 27, 28, 17);
        rect(ctx, c.background, 33, 32, 34, 15);
        for (int x = 35; x <= 65; x += 5) line(ctx, c.foreground, x, 32, x, 46);
        for (int y = 36; y <= 44; y += 4) line(ctx, c.foreground, 33, y, 67, y);
        rect(ctx, c.foreground, 18, 27, 9, 3);
        rect(ctx, c.foreground, 73, 27, 9, 3);
        for (int x = 13; x <= 85; x += 72) {
          rect(ctx, c.background, x, 35, 5, 10);
          line(ctx, c.foreground, x + 2, 35, x + 2, 45);
        }
        rect(ctx, c.foreground, 7, 23, 2, 23); rect(ctx, c.foreground, 91, 23, 2, 23);
        dot(ctx, c.foreground, 8, 22, 2); dot(ctx, c.foreground, 92, 22, 2);
        rect(ctx, c.foreground, 47, 11, 6, 5);
        circle(ctx, c.background, 50, 21, 4);
        line(ctx, c.foreground, 50, 21, 50, 18);
        line(ctx, c.foreground, 50, 21, 52, 22);
        for (int i = 0; i < 5; i++) line(ctx, c.soft, 30 + i * 10, 51, 35 + i * 10, 51);
      }
      rect(ctx, c.accent, 6, s->station_arch ? 52 : 30, 88, 3);
      if (!quiet) line(ctx, c.foreground, 6, 95, 94, 95);
#endif
      break;
    case AT_ISFJ_KEEPSAKE:
      outline(ctx, c.foreground, 7, 7, 86, 86);
      outline(ctx, c.soft, 10, 10, 80, 80);
      for (int y = 17; y < 84; y += 6) {
        rect(ctx, c.foreground, 8, y, 1, 2); rect(ctx, c.foreground, 91, y, 1, 2);
      }
      for (int side = 0; side < 2; side++) {
        int y = side ? 90 : 10;
        line(ctx, c.accent, 50, y, 42, y + (side ? -5 : 5));
        line(ctx, c.accent, 50, y, 58, y + (side ? -5 : 5));
        flower(ctx, c.foreground, c.accent, 50, y);
        for (int x = 28; x <= 72; x += 44) {
          line(ctx, c.foreground, x, y, x + (x < 50 ? 11 : -11), y + (side ? -3 : 3));
          dot(ctx, c.accent, x, y, 1);
        }
      }
      break;
    case AT_ESTJ_LEDGER:
      line(ctx, c.foreground, 5, 11, 95, 11);
      line(ctx, c.foreground, 5, 14, 95, 14);
      line(ctx, c.foreground, 5, 45, 95, 45);
      line(ctx, c.foreground, 5, 49, 95, 49);
      line(ctx, c.foreground, 5, 57, 95, 57);
      for (int y = 66; y <= 94; y += 9) line(ctx, c.soft, 5, y, 95, y);
      for (int x = 23; x <= 95; x += 12) line(ctx, c.soft, x, 49, x, 95);
      line(ctx, c.foreground, 22, 12, 22, 95);
      for (int y = 53; y < 92; y += 9) rect(ctx, c.foreground, 7, y, 5, 3);
      if (!quiet) for (int i = 0; i < 15; i++) rect(ctx, c.foreground,
                                                  29 + (i % 5) * 12, 61 + (i / 5) * 9, 2, 2);
      break;
    case AT_ESFJ_GATHERING:
      for (int i = 0; i < (quiet ? 3 : 6); i++) {
        GColor band = i % 2 ? c.accent : c.soft;
        int o = i * 5;
        thick_line(ctx, band, 0, 8 + o, 25 + o / 2, 8 + o, 3);
        thick_line(ctx, band, 25 + o / 2, 8 + o, 39 + o / 3, 0, 3);
        thick_line(ctx, band, 100, 92 - o, 75 - o / 2, 92 - o, 3);
        thick_line(ctx, band, 75 - o / 2, 92 - o, 61 - o / 3, 100, 3);
      }
      break;
    case AT_ISTP_FIELD_KIT:
      line(ctx, c.foreground, 7, 8, 7, 94);
      for (int i = 0; i < 20; i++) line(ctx, c.accent, 7, 9 + i * 4,
                                               i % 5 ? 14 : 21, 9 + i * 4);
      circle(ctx, c.foreground, 72, 74, 17);
      circle(ctx, c.soft, 72, 74, 16);
      line(ctx, c.foreground, 72, 56, 72, 92); line(ctx, c.foreground, 54, 74, 90, 74);
      thick_line(ctx, c.accent, 72, 61, 72, 75, 2);
      line(ctx, c.accent, 66, 80, 72, 74); line(ctx, c.accent, 72, 74, 78, 68);
      dot(ctx, c.foreground, 72, 74, 2);
      label(ctx, c.foreground, "N", 69, 52, 8, 9, FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
      if (!quiet) {
        label(ctx, c.foreground, "W", 48, 68, 8, 9, FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
        label(ctx, c.foreground, "E", 91, 68, 8, 9, FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
      }
      break;
    case AT_ISFP_COLOR_STUDY:
      for (int i = 0; i < 23; i++) {
        rect(ctx, c.soft, 0, i, 32 - i / 2, 1);
        rect(ctx, c.accent, 72 + i / 3, 6 + i, 28 - i / 3, 1);
        rect(ctx, c.soft, 0, 76 + i, 25 + i / 4, 1);
        rect(ctx, c.accent, 75 - i / 4, 77 + i, 25 + i / 4, 1);
      }
      if (!quiet) {
        rect(ctx, c.foreground, 45, 91, 7, 3);
        for (int i = 0; i < 5; i++) rect(ctx, c.accent, 32 + i * 8, 7 + (i * 17) % 17, 2, 2);
      }
      break;
    case AT_ESTP_VELOCITY:
      for (int i = 0; i < 20; i++) {
        thick_line(ctx, c.accent, 0, 97 - i * 3, 92, 0 - i * 3, 2);
      }
      for (int i = 0; i < 4; i++) {
        thick_line(ctx, c.background, 2, 92 - i * 4, 92, -2 - i * 4, 2);
      }
      if (!quiet) {
        thick_line(ctx, c.foreground, 17, 92, 95, 11, 1);
        thick_line(ctx, c.foreground, 24, 98, 100, 19, 1);
      }
      break;
    case AT_ESFP_FESTIVAL:
      for (int i = 0; i < (quiet ? 28 : 55); i++) {
        int x = (i * 37 + 11) % 96;
        int y = (i * 29 + 7) % 94;
        if (x > 16 && x < 84 && y > 33 && y < 72) continue;
        GColor square = i % 4 == 0 ? c.foreground : i % 3 ? c.accent : c.soft;
        rect(ctx, square, x, y, 2 + i % 2, 2 + i % 2);
      }
      break;
  }
}

void at_render(Layer *layer, GContext *ctx, const AtSettings *settings, const struct tm *now) {
  GRect bounds = layer_get_bounds(layer);
#if defined(PBL_ROUND)
  s_area = GRect(bounds.size.w * 14 / 100, bounds.size.h * 16 / 100,
                 bounds.size.w * 72 / 100, bounds.size.h * 68 / 100);
#else
  s_area = GRect(bounds.size.w * 4 / 100, bounds.size.h * 3 / 100,
                 bounds.size.w * 92 / 100, bounds.size.h * 94 / 100);
#endif
  AtColors c = colors_for(settings);
  graphics_context_set_fill_color(ctx, c.background);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  int face = at_choice(settings->face, AT_FACE_COUNT, AT_ISTJ_STATION_HOUSE);
  bool used_art = false;
#if defined(PBL_COLOR)
  if (settings->palette == 0 && settings->decoration == 0 &&
      (face != AT_ISTJ_STATION_HOUSE || settings->station_arch)) {
    GBitmap *art = art_for(face);
    if (art) {
      graphics_draw_bitmap_in_rect(ctx, art, bounds);
      used_art = true;
    }
  }
#endif
  if (!used_art) motif(ctx, settings, c);
  if (face == AT_INFJ_MARGIN_NOTES && settings->phrase_mode != 1) {
    bool custom = settings->phrase_mode == 2 && settings->custom_phrase[0];
    if (!custom) {
#if defined(PBL_PLATFORM_CHALK)
      label(ctx, c.foreground, "NOTICE THE GOOD", 0, 79, 100, 13,
            FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
#elif defined(PBL_ROUND)
      label(ctx, c.foreground, "NOTICE THE GOOD", 8, 79, 84, 13,
            FONT_KEY_GOTHIC_18_BOLD, GTextAlignmentCenter);
#elif defined(PBL_COLOR)
      tracked_margin_phrase(ctx, c.foreground);
#else
      label(ctx, c.foreground, "NOTICE THE GOOD", 10, 82, 88, 13,
            FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
#endif
    } else {
#if defined(PBL_ROUND)
      label(ctx, c.foreground, settings->custom_phrase, 0, 77, 100, 14,
            FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
#else
      label(ctx, c.foreground, settings->custom_phrase, 19, 86, 78, 10,
            FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentLeft);
#endif
    }
  }

  char time_text[8];
  at_format_time(now->tm_hour, now->tm_min, settings->time_format,
                 clock_is_24h_style(), time_text, sizeof(time_text));
#if !defined(PBL_ROUND)
  if (face == AT_ESTJ_LEDGER && time_text[1] == ':') {
    for (int i = 5; i > 0; i--) time_text[i] = time_text[i - 1];
    time_text[0] = '0';
  }
#endif
  static const int TIME_TOP[AT_FACE_COUNT] = {
    9, 12, 12, 39, 36, 27, 18, 29,
    70, 38, 21, 39, 17, 39, 39, 39
  };
  int y = TIME_TOP[face];
  if (face == AT_ISTJ_STATION_HOUSE && !settings->station_arch) y = 40;
#if defined(PBL_ROUND)
  y = face == AT_ISTJ_STATION_HOUSE ? 78 :
      face == AT_INFJ_MARGIN_NOTES ? 36 :
      face == AT_INTJ_OBSERVATORY ? 14 : 38;
#endif
  int max_time_scale = 0;
#if !defined(PBL_ROUND)
  if (face == AT_ISTJ_STATION_HOUSE) max_time_scale = 6;
#endif
  TimeLayout layout = time_layout(time_text, y, max_time_scale);
#if defined(PBL_ROUND)
  if (used_art && face == AT_ENFP_OPEN_SKY) layout.x += pw(8);
#endif
  GColor time_ground = c.background;
#if defined(PBL_COLOR)
  if (used_art) time_ground = art_ground_for(face);
#endif
  bool time_field_drawn = false;
#if !defined(PBL_ROUND)
  if (face == AT_ESTJ_LEDGER) {
    rect(ctx, time_ground, 0, 19, 100, 29);
    time_field_drawn = true;
  } else if (used_art && face == AT_ENFJ_BEACON) {
    rect(ctx, time_ground, 0, 15, 100, 26);
    line(ctx, c.accent, 0, 15, 100, 15);
    line(ctx, c.accent, 0, 41, 100, 41);
    time_field_drawn = true;
  }
#endif
  if (!time_field_drawn && !(used_art && (face == AT_ISTJ_STATION_HOUSE ||
                                        face == AT_INFJ_MARGIN_NOTES))) {
    int left_padding = 5;
    int top_padding = 4;
    int right_padding = 5;
    int bottom_padding = 4;
#if defined(PBL_ROUND)
    if (used_art && face == AT_ENFP_OPEN_SKY) left_padding = 1;
#endif
#if defined(PBL_PLATFORM_GABBRO)
    if (used_art && face == AT_INTP_SCRATCHPAD) {
      left_padding = 10;
      right_padding = 10;
      top_padding = 18;
      bottom_padding = 8;
    }
#endif
    graphics_context_set_fill_color(ctx, time_ground);
    graphics_fill_rect(ctx, GRect(layout.x - left_padding, layout.top - top_padding,
                                  layout.width + left_padding + right_padding,
                                  layout.height + top_padding + bottom_padding),
                       0, GCornerNone);
  }
#if !defined(PBL_ROUND)
  if (face == AT_ESTJ_LEDGER) {
    pixel_ledger_time(ctx, c.foreground, time_text);
  } else
#endif
  {
    pixel_time(ctx, c.foreground, time_text, layout);
  }

  char date_text[16];
  at_format_date(now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
                 settings->date_format, date_text, sizeof(date_text));
  if (date_text[0]) {
    if (face == AT_ESTJ_LEDGER) {
#if defined(PBL_ROUND)
      label(ctx, c.foreground, date_text, 31, 25, 62, 11,
            FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentRight);
#else
      label(ctx, c.foreground, date_text, 31, 11, 62, 9,
            FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentRight);
#endif
      return;
    }
    int date_y = face == AT_INFJ_MARGIN_NOTES ? 64 :
                 face == AT_ISTJ_STATION_HOUSE ? 91 : 88;
#if defined(PBL_ROUND)
    date_y = face == AT_ISTJ_STATION_HOUSE ? 53 :
             face == AT_INFJ_MARGIN_NOTES ? 67 : 88;
#endif
    if (!(used_art && face == AT_INFJ_MARGIN_NOTES)) {
      rect(ctx, c.background, 0, date_y, 100, 12);
    }
    label(ctx, c.foreground, date_text, 4, date_y, 92, 12,
          FONT_KEY_GOTHIC_14_BOLD, GTextAlignmentCenter);
  }
}
