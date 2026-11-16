---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-10-23T13:49:08-05:00"
title: "Build Archetypes watchface"
verb_noun: build_archetypes
layer: "1 - Directive"
---

# Build Archetypes Watchface

## Goal

Deliver one Pebble C watchface whose phone settings select one of 16 visually distinct, type-named designs. Users can choose any design; the codes are catalog labels rather than a personality assessment. Keep large time legible. Preserve shared settings across face changes.

## Inputs

- [Pre-build QC directive](/Users/danielbally/Git/directives/review_archetypes.md) and its concept sheet. The sheet is an art direction reference, not a pixel specification.
- `task.md` for binary completion checks.
- [Pebble hardware guide](https://developer.repebble.com/guides/tools-and-resources/hardware-information/) for current platform dimensions and capabilities.

## Face map

| Code | Design | Code | Design |
|---|---|---|---|
| INTJ | Observatory | INTP | Scratchpad |
| ENTJ | Command | ENTP | Branches |
| INFJ | Margin Notes | INFP | Paper Garden |
| ENFJ | Beacon | ENFP | Open Sky |
| ISTJ | Station House | ISFJ | Keepsake |
| ESTJ | Ledger | ESFJ | Gathering |
| ISTP | Field Kit | ISFP | Color Study |
| ESTP | Velocity | ESFP | Festival |

## Implementation route

1. Build a shared clock/settings renderer with a 16-entry face table. Draw distinctive motifs with C primitives; use resource bitmaps only when their measured cost is justified.
2. Keep production updates on minute ticks. Make any face-tour tap handler a compile-time test fixture only.
3. Use Clay for face, time format, date format, palette, decoration, Margin Notes phrase, and Station House arch settings. Persist global preferences across face changes.
4. Build Emery first. Then adapt round layout for Gabbro and Chalk and monochrome layout for Diorite. Report a target as supported only after its binary builds and a native capture is inspected.
5. Use `execution/build.py` for clean production builds and `execution/verify.py` for deterministic checks. Keep fixture builds and screenshots in `.tmp/`; never copy a fixture PBW to `dist/`.
6. Have a fresh-context verifier compare implementation and native evidence against `task.md`.

## Boundaries

- No personality quiz, `-A/-T` psychological behavior, fake live telemetry, continuous animation, external network feed, or location requirement.
- No store submission or physical-watch acceptance claim from emulator evidence.
- The contact sheet is concept art; native screen captures decide layout and contrast.

## Outputs

- `dist/archetypes.pbw` production package.
- `.tmp/` native capture and verification evidence.
- A concise README with settings, build commands, supported targets, and evidence limits.

## Learnings — 2026-10-23 visual correction

- The initial primitive-only native sheet did not preserve the concept sheet's visual density. The user rejected that look. Keep the concept's illustrated composition as the default color style; primitives remain as a monochrome and optional sketch fallback.
- The 4-by-4 screen-art atlas is sliced by `execution/prepare_art.py` into three native color target sizes: Emery 200×228, Gabbro 260×260, and Chalk 180×180. `graphics_draw_bitmap_in_rect` clips or tiles images; it does not scale them. A mismatched Gabbro asset visibly tiled in QEMU.
- Each bitmap is snapped to Pebble's 64-color channel levels before packaging, so the SDK does not introduce unexpected texture into pale backgrounds.
- The live pixel numerals, date, Margin Notes phrase, and settings continue to render in C over the bitmap art.

## Learnings — 2026-10-23 graphics quality pass

- A round bitmap needs its detailed facade inset before packaging. Full-width Station House art clipped its outer lamps at the Gabbro and Chalk circular edge, even though the rectangular asset looked correct.
- The default Margin Notes phrase now uses compact pixel lettering. Round screens use narrower glyphs so the phrase clears the left page rule; custom phrases still use the system font.
- Paper Garden's solid 64-color green is brighter than the concept's sage. A green/gray ordered mix lowered saturation but added visible checkerboard noise; native and independent visual review rejected it. Keep the calm solid field until a better native color treatment is found.

## Learnings — 2026-10-23 station cleanup

- Generated station art carried yellow, orange, and pink flecks around the facade and platform. Quantization made them conspicuous on the watch. Normalize the station tile before resizing, leaving black/white architecture and deliberate red bands; `execution/verify.py` now rejects any other station bitmap color.

## Learnings — 2026-10-23 battery removal

- The battery meter conflicted with the art direction, so it was removed from the renderer and Clay settings. Reserve its former persistence slot in C; deleting that enum position would shift later stored settings on existing installs.

## Learnings — 2026-10-23 native artwork cleanup

- A fresh battery-free 16-face Emery gallery exposed generated fringe colors on Margin Notes, Keepsake, and Ledger. The first two paper faces now use black/white artwork, while Keepsake retains a controlled blue floral accent. The same palette limits apply to Gabbro and Chalk resources.
- Color Study and Velocity read more cleanly with flat intended colors. The atlas texture had left dark flecks inside Color Study's shapes and blue specks along Velocity's orange slash. `execution/prepare_art.py` maps each to a constrained palette, and `execution/verify.py` checks it.
- Inspect the default Margin Notes phrase at native resolution before judging its legibility from a downscaled gallery. The full phrase is present; contact-sheet interpolation can make its narrow pixel letters look broken.

## Learnings — 2026-10-23 time legibility

- Draw a solid, face-matched field under the live time using the actual pixel-glyph bounds. Fixed-width masks leave stray artwork inside shorter or longer time strings; a bitmap background color also avoids an off-palette rectangle.
- Station House already gives time clear white space. Its red platform bands cross the top edge of a generic padded field, so skip that field for the illustrated Station House and inspect the stripe in the native capture.
- Capture both rectangular and round targets with a five-character time after changing the renderer. Emery, Gabbro, and Chalk can expose different overlaps because their pixel scale and circular clipping differ.
- On round Open Sky, a centered time field clipped the crescent into a straight edge. Shift the five-character time slightly right and keep a narrow left margin; verify the moon and digits together in native Gabbro and Chalk captures.

## Learnings — 2026-10-23 Station House digit clearance

- Keeping the illustrated Station House free of a generic time field preserved the red bands, but a five-character time still touched the lower band. On rectangular Emery, start the time lower and cap the pixel scale for shorter 12-hour strings so the digits retain a fixed height. Move the optional date lower with it. Verify the band/time gap and time/date gap in native captures for both time formats.

## Learnings — 2026-10-23 composition polish

- Ledger's measured time mask left fragments of the grid rule beside the digits. A full-width header keeps the upper rules continuous and the calendar grid below the clock. Apply it to Diorite as well as color targets; its monochrome fallback showed the same stray dash.
- Beacon's dark time field reads as an intentional signal band when edged by thin gold rules. Keep the upper arcs and lower lighthouse visible in the native capture.

## Learnings — 2026-10-23 Margin Notes lettering

- The hand-built 7-row phrase alphabet looked crude at wrist scale. Use the native bold uppercase label for the default phrase; reserve the page rules as clear space around it.
- Chalk needs a smaller label and the full width of its round layout to fit the complete phrase. Place the round date above the phrase so it does not erase the lettering.

## Learnings — 2026-10-23 Analyst artwork cleanup

- Observatory and Scratchpad inherited small off-palette hues from the generated atlas. Quantize them to each design's intended Pebble colors during art preparation; inspect the native orbit, graphs, and grid after the conversion.
- Scratchpad's graph approaches the time on Gabbro. Give its live digits extra white clearance above and beside the glyph bounds on that target. The same larger field clips decorative chart marks on Chalk, so keep Chalk's original field.
- `execution/build.py` must run `execution/prepare_art.py` before the clean PBW build. Rebuilding alone previously packaged stale bitmaps after a preparation-script change.

## Learnings — 2026-10-23 Ledger and Margin Notes candidate fidelity

- The Ledger concept separates its first time digit with a vertical rule that continues into the lower calendar. On rectangular targets, draw the two time runs on opposite sides of that rule and preserve a full-width white header. Pad a one-digit 12-hour value to five characters so this composition holds.
- Ledger's optional date belongs in the open upper header. A full-width date clearing rectangle at the bottom erased rows of the calendar on both rectangular and round targets. Native ISO-date captures must show the date and the complete grid together.
- Margin Notes needs a quiet paper ground and a small tracked caption between the lower rules. A solid cream conversion looked too yellow in native output; a sparse warm halftone retained a restrained paper tone. Keep the top star and left page rule clear of the live time.
