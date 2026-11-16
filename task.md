# Task — Build Archetypes watchface — 2026-10-23

## Ledger and Margin Notes render fidelity — 2026-10-23

Match the supplied concept sheet's split Ledger clock and restrained ruled-paper Margin Notes composition at native Pebble resolution.

### Validation Contract

- Emery Ledger shows the first time digit in a narrow left column.
- Emery Ledger shows the remaining time characters to the right of a vertical divider.
- Emery Ledger's divider joins the calendar grid below the clock.
- Emery Ledger's lower grid remains visible.
- Emery Ledger's lower grid remains visible with ISO date enabled.
- Gabbro Ledger's lower grid remains visible with ISO date enabled.
- Emery Ledger shows a complete five-character 12-hour time.
- Diorite Ledger shows a complete five-character time without overlap.
- Emery Margin Notes shows its default phrase on one line between the lower rules.
- Emery Margin Notes preserves the left page rule.
- Emery Margin Notes preserves the top star.
- Emery Margin Notes time remains clear of the page rules.
- Gabbro Margin Notes shows the complete default phrase.
- Chalk Margin Notes shows the complete default phrase.
- `python3 execution/build.py` produces a four-platform PBW.
- `python3 execution/verify.py` passes.
- A fresh-context verifier checks the final native captures against the supplied reference.

### Progress

- [x] Compare the reference with current native Ledger and Margin Notes.
- [x] Recompose Ledger's clock header.
- [x] Refine Margin Notes' caption and paper layout.
- [x] Capture rectangular and round native targets.
- [x] Run independent visual verification.

## Analyst artwork cleanup — 2026-10-23

Remove stray generated hues from Observatory and Scratchpad while retaining their illustrated structures.

### Validation Contract

- Observatory's Emery bitmap contains no pink pixels.
- Observatory's Emery bitmap contains no green pixels.
- Observatory's Emery capture retains the central orbit.
- Scratchpad's Emery bitmap contains no yellow pixels.
- Scratchpad's Emery bitmap contains no green pixels.
- Scratchpad's Emery capture retains the black graph.
- Scratchpad's Emery capture retains the teal graph.
- Scratchpad's Emery capture retains its grid.
- Gabbro Scratchpad has no graph mark touching the time.
- Chalk Scratchpad has no graph mark touching the time.
- The Emery gallery contains 16 distinct compositions.
- `execution/build.py` regenerates bitmap resources before packaging.
- `python3 execution/build.py` produces a four-platform PBW.
- `python3 execution/verify.py` passes.

### Progress

- [x] Inspect latest native gallery against concept art.
- [x] Tighten generated artwork palettes.
- [x] Rebuild and inspect native captures.

## Margin Notes lettering — 2026-10-23

Replace the default phrase's rough pixel alphabet with clear, restrained lettering that fits the paper composition.

### Validation Contract

- Emery shows the complete default phrase on one line.
- Gabbro shows the complete default phrase.
- Chalk shows the complete default phrase.
- Diorite shows the complete default phrase.
- The Emery phrase clears the page rules.
- The Emery time remains legible.
- `python3 execution/build.py` produces a four-platform PBW.
- `python3 execution/verify.py` passes.

### Progress

- [x] Inspect current native phrase and concept reference.
- [x] Replace the default phrase lettering.
- [x] Inspect native captures on four targets.

## Composition polish — 2026-10-23

Refine the illustrated time areas where the current clearing rectangle creates stray line fragments. Preserve the watch-glance contrast and the established face motifs.

### Validation Contract

- Emery Ledger has a continuous rule above its time.
- Emery Ledger has a continuous grid rule below its time.
- Emery Ledger has no dangling grid dashes beside its time.
- Diorite Ledger has no dangling grid dashes beside its time.
- Emery Beacon's five-character time remains legible.
- Emery Beacon's lighthouse remains visible below the time.
- Emery Beacon's gold rings remain visible above the time.
- Emery Station House keeps at least three white rows below its lower red band.
- Emery Margin Notes keeps the full default phrase.
- The Emery gallery contains 16 distinct compositions.
- `python3 execution/build.py` produces a four-platform PBW.
- `python3 execution/verify.py` passes.
- A fresh-context verifier checks the revised native captures.

### Progress

- [x] Compare the latest native gallery with the concept sheet.
- [x] Refine Ledger and Beacon time areas.
- [x] Rebuild and inspect native captures.
- [x] Run independent visual verification.

## Station House digit clearance — 2026-10-23

The Emery Station House digits touch the lower red platform band. Restore a visible gap without crowding the optional date.

### Validation Contract

- The Emery 24-hour digits have at least three white pixel rows below the lower red band.
- The Emery 12-hour digits have at least three white pixel rows below the lower red band.
- The Emery ISO date does not overlap the 24-hour digits.
- The Emery ISO date does not overlap the 12-hour digits.
- The Emery station facade remains intact.
- The Gabbro Station House time remains below the platform bands.
- The Chalk Station House time remains below the platform bands.
- `python3 execution/build.py` produces a four-platform PBW.
- `python3 execution/verify.py` passes.

### Progress

- [x] Inspect the final native Station House capture.
- [x] Adjust rectangular time and date placement.
- [x] Capture 12-hour and 24-hour layouts with ISO date.
- [x] Check round Station House captures.

## Time legibility against illustrations — 2026-10-23

Give the live numerals clear negative space on the illustrated defaults while retaining the distinctive face artwork and all time-format choices.

### Validation Contract

- Observatory numerals have a solid dark field behind them on Emery.
- Scratchpad numerals have a solid light field behind them on Emery.
- Beacon numerals have a solid dark field behind them on Emery.
- Open Sky numerals have a solid blue field behind them on Emery.
- Ledger numerals have a solid light field behind them on Emery.
- Velocity numerals have a solid blue field behind them on Emery.
- The time field encloses a five-character time value on Emery.
- Gabbro Branches shows a solid field behind the time.
- Gabbro Beacon shows a solid field behind the time.
- Gabbro Open Sky shows a solid field behind the time.
- Gabbro Ledger shows a solid field behind the time.
- Gabbro Velocity shows a solid field behind the time.
- Chalk Branches shows a solid field behind the time.
- Chalk Beacon shows a solid field behind the time.
- Chalk Open Sky shows a solid field behind the time.
- Chalk Ledger shows a solid field behind the time.
- Chalk Velocity shows a solid field behind the time.
- Gabbro Open Sky retains the full crescent.
- Chalk Open Sky retains the full crescent.
- Station House retains the station facade on Emery.
- Station House retains continuous red platform bands on Emery.
- Margin Notes retains its default phrase on Emery.
- `python3 execution/build.py` produces a four-platform PBW.
- `python3 execution/verify.py` passes.
- A fresh-context verifier reviews the native captures for legibility and artwork loss.

### Progress

- [x] Compare current native captures with the concept sheet.
- [x] Add measured time fields to the renderer.
- [x] Rebuild and recapture the native faces.
- [x] Run independent verification.

## Native artwork cleanup — 2026-10-23

Compare the battery-free native gallery with the concept sheet. Remove generated color noise from the pale graphic faces while preserving their main motifs and live time.

### Validation Contract

- Margin Notes bitmap uses only black and white pixels on Emery.
- Keepsake bitmap uses only black, white, and blue-family pixels on Emery.
- Ledger bitmap uses only black and white pixels on Emery.
- Color Study bitmap uses only its four intended flat colors on Emery.
- Velocity bitmap uses only blue and orange pixels on Emery.
- The Emery native gallery shows 16 distinct face compositions.
- The Emery native Margin Notes capture shows the complete default phrase.
- The Emery native Keepsake capture shows the ornamental frame.
- The Emery native Ledger capture shows the calendar grid.
- `python3 execution/build.py` produces a four-platform PBW.
- `python3 execution/verify.py` passes.
- A fresh-context verifier compares the final native captures with the concept sheet.

### Progress

- [x] Capture and inspect the battery-free Emery gallery.
- [x] Clean the pale and bold face bitmap palettes.
- [x] Rebuild and inspect final native captures.
- [x] Run independent visual verification.

## Remove battery indicator — 2026-10-23

The battery icon crowds the face artwork. Remove it from the watchface and its settings; retain the storage slot so older installations keep their other preferences.

### Validation Contract

- The Emery Station House native capture has no battery indicator.
- The Gabbro Station House native capture has no battery indicator.
- The Chalk Station House native capture has no battery indicator.
- The Emery Margin Notes native capture has no battery indicator.
- The Clay configuration has no Battery control.
- The package declares no BatteryMode message key.
- An existing stored face selection uses the same storage key after the update.
- An existing stored date preference uses the same storage key after the update.
- `python3 execution/build.py` produces a four-platform PBW.
- `python3 execution/verify.py` passes.
- A fresh-context verifier checks the final package and native captures.

### Progress

- [x] Trace the battery setting through rendering, persistence, and Clay.
- [x] Remove rendering and configuration paths.
- [x] Build and inspect native captures.
- [x] Run independent verification.

## Station House artifact cleanup — 2026-10-23

The generated station source contains pink, yellow, and orange specks around the facade and platform. Keep the station silhouette and red platform bands while producing a clean native palette.

### Validation Contract

- The Emery Station House bitmap contains only black, white, and red pixels.
- The Gabbro Station House bitmap contains only black, white, and red pixels.
- The Chalk Station House bitmap contains only black, white, and red pixels.
- The Emery native capture shows the arched station facade.
- The Gabbro native capture shows the station lamps inside the circular display.
- The Chalk native capture shows the station lamps inside the circular display.
- `python3 execution/build.py` succeeds after cleanup.
- `python3 execution/verify.py` passes after cleanup.

### Progress

- [x] Trace the artifacts from source art to native capture.
- [x] Clean the station bitmap palette.
- [x] Rebuild and recapture Station House.

## Graphics quality pass — 2026-10-23

Refine the native illustrated defaults after comparing them with the concept contact sheet at wrist size. Keep the 16-face catalog and settings behavior intact.

### Validation Contract

- Paper Garden's native Emery background stays visually calm at screen size.
- Station House's facade fits inside the Gabbro circular screen.
- Station House's facade fits inside the Chalk circular screen.
- Margin Notes shows a bold pixel-lettered default phrase on Emery.
- Margin Notes shows the full default phrase on Chalk.
- Margin Notes shows the full default phrase on Gabbro.
- The custom phrase setting still displays custom text.
- The Off phrase setting removes the phrase.
- `python3 execution/build.py` succeeds after the changes.
- `python3 execution/verify.py` passes after the changes.
- A fresh-context verifier reviews the revised native captures.

### Progress

- [x] Inspect concept and native captures at screen size.
- [x] Refine Station House and Margin Notes graphics.
- [x] Reject Paper Garden's noisy color treatment after native review.
- [x] Capture native Emery, Gabbro, and Chalk results.
- [x] Run independent visual review.

## Visual correction — 2026-10-23

The first native contact sheet does not match the concept contact sheet's visual density. Replace the generic system-font-and-lines presentation with compositions recognizably based on the concept reference at `/Users/danielbally/Git/directives/assets/archetypes_contact_sheet.jpg`.

### Validation Contract

- Emery renders time with block pixel numerals on each of the 16 faces.
- Station House shows a recognizable arched station facade above the time.
- Margin Notes shows the ruled-page composition above its phrase.
- Observatory shows an elliptical orbit scene.
- Scratchpad shows graph paper with plotted lines.
- Command shows filled instrument bars.
- Branches shows branching circuit paths.
- Paper Garden shows botanical stems with blooms.
- Beacon shows a lighthouse above water.
- Open Sky shows a layered landscape below stars.
- Keepsake shows an ornamental frame.
- Ledger shows a calendar-like grid.
- Gathering shows thick geometric arches.
- Field Kit shows ruler ticks and a compass rose.
- Color Study shows broad angular color shapes.
- Velocity shows a bold diagonal slash.
- Festival shows colorful square confetti.
- An Emery native contact sheet shows the revised 16-face set.
- `python3 execution/build.py` succeeds after the revision.
- `python3 execution/verify.py` passes after the revision.
- A fresh-context verifier compares the native contact sheet with the concept sheet.

### Progress

- [x] Compare the original concept sheet with native captures.
- [x] Replace the shared typography and redraw each face.
- [x] Build and inspect revised native captures.
- [x] Run independent visual review.

## Contract

- Target directive: `directives/build_archetypes.md`.
- Intended execution scripts: `execution/build.py` and `execution/verify.py`.
- Expected artifact: `dist/archetypes.pbw`.
- Expected evidence: native emulator captures under `watchface/.tmp/native/`.
- Safety: leave `.env`, tokens, and `.tmp/` out of Git.
- Safety: do not upload, publish, or claim physical-watch acceptance.
- Scope decision: start with Emery, then build and check Gabbro, Chalk, and Diorite in the same PBW if the aggregate design fits.
- Review boundary: independent pre-build QC has not been completed; the owner explicitly authorized development on 2026-10-23.

## Validation Contract

- `watchface/package.json` declares one watchface UUID.
- The face setting offers the 16 codes in `directives/build_archetypes.md`.
- INFJ selects Margin Notes.
- ISTJ selects Station House.
- A face change preserves the time-format setting.
- A face change preserves the date-format setting.
- Date format offers Off.
- Date format offers YYYY-MM-DD.
- Date format offers MM/DD/YYYY.
- Date format offers DD/MM/YYYY.
- The default time remains legible in an Emery native capture.
- `python3 execution/build.py` produces `dist/archetypes.pbw`.
- `python3 execution/verify.py` passes.
- The PBW contains an Emery app binary.
- The PBW contains a Gabbro app binary.
- The PBW contains a Chalk app binary.
- The PBW contains a Diorite app binary.
- A fresh-context verifier reviews the final implementation against this contract.

The initial battery-display criteria were superseded by the removal contract above.

## Progress

- [x] Inspect Pebble skill, SDK, template, and existing local examples.
- [x] Create the project from `project-template`.
- [x] Implement shared settings and 16 designs.
- [x] Build the four target platforms.
- [x] Inspect native emulator captures.
- [x] Run independent review.
- [x] Record final evidence and handoff.
