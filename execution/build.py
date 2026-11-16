"""Build a production Archetypes PBW without fixture-only compiler flags."""

from __future__ import annotations

import os
import shutil
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
WATCHFACE = ROOT / "watchface"
OUTPUT = ROOT / "dist" / "archetypes.pbw"
RUNLOG = ROOT / ".tmp" / "runlog.md"


def run(command: list[str], env: dict[str, str]) -> None:
    result = subprocess.run(command, cwd=WATCHFACE, env=env, text=True, capture_output=True)
    RUNLOG.parent.mkdir(parents=True, exist_ok=True)
    with RUNLOG.open("a", encoding="utf-8") as log:
        log.write(f"\n### {' '.join(command)}\n\n```text\n{result.stdout}{result.stderr}\n```\n")
    if result.returncode:
        raise RuntimeError(f"{' '.join(command)} failed; see {RUNLOG}")


def main() -> None:
    env = os.environ.copy()
    env.pop("AT_TEST_FACE", None)
    env.pop("AT_TEST_TOUR", None)
    run(["python3", str(ROOT / "execution" / "prepare_art.py")], env)
    run(["pebble", "clean"], env)
    run(["pebble", "build"], env)
    source = WATCHFACE / "build" / "watchface.pbw"
    if not source.is_file():
        raise FileNotFoundError(source)
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, OUTPUT)
    print(OUTPUT)


if __name__ == "__main__":
    main()
