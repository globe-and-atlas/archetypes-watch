# Errors

Record deterministic errors, root causes, and fixes here.

## 2026-10-23 — Message-key switch did not compile

- Error: C `case MESSAGE_KEY_Face` and the other generated message-key cases failed with “case label does not reduce to an integer constant.”
- Cause: this SDK emits message keys as runtime constants.
- Fix: dispatch with typed key comparisons (`if` / `else if`) and reject non-integer tuples for numeric settings.
- Graduated to: `watchface/src/c/main.c`; use comparisons for generated Pebble message keys in this project.

## 2026-10-23 — Gabbro artwork tiled in the emulator

- Error: after a successful four-target build, Gabbro showed repeated station facades above and below the clock.
- Cause: the prepared Gabbro bitmap was 144×168, while its round native screen is 260×260. Pebble tiles a bitmap when its draw rectangle is larger; `graphics_draw_bitmap_in_rect` does not scale it.
- Fix: prepare a 260×260 Gabbro bitmap, assert each platform asset dimension in `execution/verify.py`, and inspect a native screenshot.
- Graduated to: `execution/prepare_art.py` and `directives/build_archetypes.md`.

## 2026-10-23 — Station House palette artifacts

- Error: native Station House showed scattered yellow, orange, and pink pixels in its windows and platform.
- Cause: generated source art contains small warm-color flecks; nearest-neighbor resize and 64-color quantization preserve and sharpen them.
- Fix: normalize the station tile to black/white facade pixels and fixed red platform bands before resizing. Verify each generated color asset has only black, white, and red pixels.
- Graduated to: `execution/prepare_art.py` and `execution/verify.py`.

## 2026-10-23 — Station House time touched the platform band

- Error: on Emery, a five-character time touched the lower red platform band even after the general time-legibility pass.
- Cause: Station House intentionally skips the generic time field to preserve its red bands, while its earlier vertical placement began on the band. Four-character time also grew taller because the pixel font scaled with available width.
- Fix: move rectangular Station House time below the band, cap its glyph scale at six pixels, and move its optional date lower. Native 12-hour and 24-hour captures with ISO date show three white rows below the band and no time/date overlap.
- Graduated to: `watchface/src/c/face_renderer.c` and `directives/build_archetypes.md`.

## 2026-10-23 — Ledger time mask left grid dashes

- Error: Ledger showed short isolated rule segments next to its time on Emery and Diorite.
- Cause: a measured clearing rectangle erased only the part of a horizontal rule beneath the pixel numerals.
- Fix: draw a full-width time header on rectangular Ledger targets. Native Emery and Diorite captures show clean time and an intact lower calendar grid.
- Graduated to: `watchface/src/c/face_renderer.c` and `directives/build_archetypes.md`.

## 2026-10-23 — Margin Notes phrase collided with page details

- Error: the first native-font pass drew the Emery phrase across the upper page rule; Chalk truncated the phrase and its enabled date covered the label.
- Cause: the text rectangles were sized from the larger targets, while the date kept a shared round-screen position near the bottom.
- Fix: move the Emery label into the ruled band, use a smaller full-width Chalk label, and place the round date above the phrase.
- Graduated to: `watchface/src/c/face_renderer.c` and `directives/build_archetypes.md`.

## 2026-10-23 — Build packaged stale artwork

- Error: a successful PBW build still contained Observatory and Scratchpad bitmaps with old fringe colors; the new palette assertion failed.
- Cause: `execution/build.py` cleaned and built the Pebble project without first rerunning `execution/prepare_art.py`.
- Fix: run art preparation as the first build step, then build and verify the package. Native captures were repeated after regeneration.
- Graduated to: `execution/build.py` and `directives/build_archetypes.md`.

## 2026-10-23 — Ledger date erased calendar rows

- Error: enabling the date on Ledger covered part of its lower calendar on Emery and Gabbro.
- Cause: the shared date renderer painted a full-width background rectangle at 88% height, across the calendar grid.
- Fix: place Ledger's optional date in the open upper header without a clearing rectangle. Native Emery and Gabbro ISO-date captures show the full grid.
- Graduated to: `watchface/src/c/face_renderer.c` and `directives/build_archetypes.md`.

## 2026-10-23 — Stale emulator state stalled installs

- Error: an Emery install stalled after emulator restarts; Chalk later stalled in the same way.
- Cause: the saved emulator state continued launching a stale or unhealthy QEMU instance. Repeating `pebble install` while the first command was pending started overlapping instances.
- Fix: wait for the active install to finish or terminate it, stop the target emulator, back up that target's saved SDK state under `.tmp/`, then start a fresh install. Reset only the affected target. Emery and Chalk installs succeeded after this sequence.
- Graduated to: the `_PEBBLE` workflow for target-specific emulator recovery.
