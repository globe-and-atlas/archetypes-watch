# Decisions

## Initial Decision

- Template profile: `workflow-python`
- Deploy target: `local-only`
- Runtime: `pebble-c`

## 2026-10-23 — One package, shared preferences

- Decision: ship 16 named compositions in one Pebble watchface. A single `Face` setting selects the design; clock, date, battery, palette, and detail settings remain global.
- Alternatives: four temperament engines or 16 separate PBWs. The requested direct mapping between type code and face is clearer in a single 16-option picker; the compiled package remains small enough for all four target platforms.
- Decision: use lightweight C primitives and system fonts, with a separate round-safe layout. This avoids an asset matrix for 16 designs across color and monochrome targets.
- Decision: keep diagnostic face-tour behavior behind compile-time flags. The production build always cleans and removes those flags.

## 2026-10-23 — Illustrated default supersedes the primitive-only visual decision

- Decision: default color faces use screen-only bitmap art from a generated 4-by-4 pixel-art atlas. Live time, date, battery, and phrase remain C-rendered. The earlier primitive-only decision remains as the Diorite and optional sketch fallback.
- Alternatives: continue elaborating every composition with primitives, or ship the render imagery as full screenshots with fixed time. Elaborated primitives remained visibly thin; fixed-time images cannot function as a watchface. Sixteen native-size bitmap resources per color platform fit the package resource limit.
- Decision: prepare separate Emery, Gabbro, and Chalk images. Pebble's bitmap draw call tiles a smaller image and clips a larger one; it does not resize on screen.

## 2026-10-23 — Shape-specific graphics refinement

- Decision: inset the Station House facade in round art assets while retaining full-width platform stripes. The unmodified facade lost lamp detail at the circular mask; shrinking the entire scene would also weaken the bands and clock balance.
- Decision: draw the default Margin Notes phrase with pixel glyphs, using a narrower alphabet on round screens. A single large rectangular alphabet collided with the page rule on Chalk and Gabbro. Arbitrary custom phrases keep the system font.
- Decision: retain Paper Garden's solid green background. A closer average sage tone made with ordered green/gray pixels produced visible full-screen noise in native captures, which distracted from the flowers and time.

## 2026-10-23 — Remove battery indicator

- Decision: remove battery rendering and the Clay battery control from all 16 faces. The user found the indicator visually out of place. This supersedes the initial shared-battery preference decision.
- Decision: leave the former battery persistence slot reserved. Existing face, date, palette, and phrase preferences retain their previous numeric storage keys after the update; the old battery value is ignored.

## 2026-10-23 — Clear the time against illustrated art

- Decision: measure each rendered time string and clear a padded field behind its exact pixel bounds. This accommodates four- and five-character times without a fixed-width cutout.
- Decision: on illustrated defaults, use the bitmap's dominant background color for the field. On alternate palettes and the monochrome fallback, use the configured background color.
- Exception: illustrated Station House keeps its existing white time area without a new field. A first native pass showed a field cutting through the continuous red platform band.
- Round Open Sky moves its time slightly right and uses a narrower left field edge so the crescent remains complete without touching the digits.
