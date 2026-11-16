# Archetypes

One configurable Pebble watchface with 16 visual designs. The four-letter labels organize the design catalog; they do not assess the wearer's personality or restrict which design can be selected.

The default is **ISTJ / Station House**. **INFJ / Margin Notes** includes “NOTICE THE GOOD.” The other 14 designs are Observatory, Scratchpad, Command, Branches, Paper Garden, Beacon, Open Sky, Keepsake, Ledger, Gathering, Field Kit, Color Study, Velocity, and Festival.

## Settings

- Design: any of the 16 named faces.
- Time: follow the watch, 12-hour, or 24-hour.
- Date: Off (default), YYYY-MM-DD, MM/DD/YYYY, or DD/MM/YYYY.
- Visual style: Illustrated (default), Warm sketch, or Cool sketch. Sketch styles use simpler line art.
- Detail: Full (default) or Minimal sketch. Diorite uses black-and-white line art.
- Station House arch: On or Off.
- Margin Notes phrase: “NOTICE THE GOOD,” Off, or a custom phrase up to 18 characters. Long custom phrases may be truncated on small displays.

The default Margin Notes phrase uses pixel lettering matched to the clock. Custom phrases use a system font to support arbitrary text.

Settings are stored on the watch and stay selected when the design changes. The battery indicator was removed from every design in version 0.1.1, including for existing installations. The clock redraws on minute ticks; there is no location, network, or motion requirement.

## Build and checks

Requires Pebble Tool and an installed SDK. From this directory:

```sh
python3 execution/build.py
python3 execution/verify.py
```

The illustrated faces use a generated pixel-art atlas in `watchface/resources/images/art_atlas.png`, derived from the concept sheet. The build script runs `prepare_art.py` to make native-size, 64-color bitmap resources for Emery, Gabbro, and Chalk, then clears fixture flags, runs a clean four-target build, and writes `dist/archetypes.pbw`. Station House's generated color flecks and the Analyst artwork's fringe hues are cleaned during art preparation. Art preparation and verification require Pillow 12.3.0. Verification checks the bundle, art sizes, palette limits, Clay controls, face mapping, date choices, and portable C formatting logic. Supported package targets are Emery, Gabbro, Chalk, and Diorite.

Native emulator captures live under `.tmp/` and `watchface/.tmp/native/`. Those captures establish emulator layout only. Physical-watch readability, store naming, and publication remain separate decisions.
