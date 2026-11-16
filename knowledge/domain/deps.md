---
generated_by: "OpenAI Codex (GPT-6; exact variant not exposed)"
timestamp: "2026-10-23T14:03:00-05:00"
---

# Dependencies

- Pebble Tool 5.0.40 with SDK 4.33.1: compiles Emery, Gabbro, Chalk, and Diorite targets and runs native emulators. The color builds now bundle per-platform bitmap resources; Diorite uses C-drawn line art.
- `@rebble/clay` 1.1.0: renders the phone configuration page. Pinned in `watchface/package.json` and `watchface/package-lock.json` so the controls and message-key serialization remain reproducible. The Clay page was opened in the emulator configuration tool and its 16-option selector and global controls were visible.
- Pillow 12.3.0: pinned in `requirements.txt` for deterministic atlas slicing, nearest-neighbor resizing, and Pebble-palette preparation in `execution/prepare_art.py`. Builds use prepared PNGs and do not require Pillow unless the source atlas changes.
