"""Check package coverage, settings wiring, and portable formatting logic."""

from __future__ import annotations

import json
import struct
import subprocess
import tempfile
import zipfile
from pathlib import Path

from PIL import Image


ROOT = Path(__file__).resolve().parents[1]
WATCHFACE = ROOT / "watchface"
PBW = ROOT / "dist" / "archetypes.pbw"
PLATFORMS = ("emery", "gabbro", "chalk", "diorite")
CODES = (
    "INTJ", "INTP", "ENTJ", "ENTP", "INFJ", "INFP", "ENFJ", "ENFP",
    "ISTJ", "ISFJ", "ESTJ", "ESFJ", "ISTP", "ISFP", "ESTP", "ESFP",
)
ART_SIZES = {"emery": (200, 228), "gabbro": (260, 260), "chalk": (180, 180)}
ANALYST_PALETTES = {
    0: {(0, 0, 0), (255, 255, 255), (170, 170, 170), (85, 85, 85),
        (0, 0, 85), (0, 85, 85), (0, 170, 255), (255, 255, 170)},
    1: {(255, 255, 255), (170, 255, 255), (0, 0, 0),
        (170, 170, 170), (0, 170, 170)},
}


def sections(items: list[dict]) -> list[dict]:
    return [item for group in items for item in (group.get("items", []) if group.get("type") == "section" else [])]


def main() -> None:
    package = json.loads((WATCHFACE / "package.json").read_text(encoding="utf-8"))["pebble"]
    assert package["watchapp"]["watchface"] is True
    assert sorted(package["targetPlatforms"]) == sorted(PLATFORMS)
    media = package["resources"]["media"]
    assert len(media) == 16 * len(ART_SIZES)
    for platform, size in ART_SIZES.items():
        for index in range(16):
            name = f"ART_{index:02d}_{platform.upper()}"
            item = next((entry for entry in media if entry["name"] == name), None)
            assert item is not None
            assert item["targetPlatforms"] == [platform]
            image = WATCHFACE / "resources" / item["file"]
            header = image.read_bytes()[:24]
            assert header[:8] == b"\x89PNG\r\n\x1a\n"
            assert struct.unpack(">II", header[16:24]) == size
            if index in ANALYST_PALETTES:
                with Image.open(image) as analyst_art:
                    colors = set(analyst_art.convert("RGB").get_flattened_data())
                assert colors <= ANALYST_PALETTES[index], (platform, index, colors)
            if index == 8:
                with Image.open(image) as station:
                    colors = set(station.convert("RGB").get_flattened_data())
                assert colors <= {(0, 0, 0), (255, 255, 255), (170, 0, 0)}, (platform, colors)
            if index in {4, 9, 10}:
                with Image.open(image) as pale_art:
                    colors = set(pale_art.convert("RGB").get_flattened_data())
                expected = ({(0, 0, 0), (255, 255, 255), (255, 255, 170)}
                            if index == 4 else {(0, 0, 0), (255, 255, 255)})
                if index == 9:
                    expected |= {(0, 85, 85), (0, 85, 170)}
                assert colors <= expected, (platform, index, colors)
            if index in {13, 14}:
                with Image.open(image) as bold_art:
                    colors = set(bold_art.convert("RGB").get_flattened_data())
                expected = ({(0, 0, 0), (170, 85, 85), (85, 170, 255), (255, 85, 85)}
                            if index == 13 else {(0, 85, 255), (255, 85, 0)})
                assert colors <= expected, (platform, index, colors)
    config = json.loads((WATCHFACE / "src/pkjs/config.json").read_text(encoding="utf-8"))
    controls = {item.get("messageKey"): item for item in sections(config) if "messageKey" in item}
    assert set(package["messageKeys"]) == set(controls)
    face = controls["Face"]
    assert face["defaultValue"] == "8"
    assert [option["label"].split(" / ")[0] for option in face["options"]] == list(CODES)
    assert [option["value"] for option in face["options"]] == [str(i) for i in range(16)]
    assert face["options"][4]["label"] == "INFJ / Margin Notes"
    assert face["options"][8]["label"] == "ISTJ / Station House"
    date = controls["DateFormat"]
    assert date["defaultValue"] == "0"
    assert [option["label"] for option in date["options"]] == [
        "Off", "YYYY-MM-DD", "MM/DD/YYYY", "DD/MM/YYYY"
    ]
    assert "BatteryMode" not in package["messageKeys"]
    assert "BatteryMode" not in controls
    assert PBW.is_file()
    with zipfile.ZipFile(PBW) as bundle:
        names = set(bundle.namelist())
        for platform in PLATFORMS:
            assert f"{platform}/pebble-app.bin" in names
            assert f"{platform}/manifest.json" in names
        assert "pebble-js-app.js" in names
        appinfo = json.loads(bundle.read("appinfo.json"))
        assert appinfo["uuid"] == package["uuid"]
    with tempfile.TemporaryDirectory(dir=ROOT / ".tmp") as temporary:
        executable = Path(temporary) / "model_test"
        subprocess.run(
            ["cc", "-std=c99", "-Wall", "-Wextra", "-Werror", "-I", str(WATCHFACE / "src/c"),
             str(ROOT / "tests/model_test.c"), str(WATCHFACE / "src/c/model.c"), "-o", str(executable)],
            check=True,
        )
        subprocess.run([str(executable)], check=True)
    print("Package, settings, and model checks passed")


if __name__ == "__main__":
    main()
