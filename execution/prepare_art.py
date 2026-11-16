"""Slice the approved screen-only pixel-art atlas into Pebble bitmap resources."""

from pathlib import Path

from PIL import Image


PROJECT = Path(__file__).resolve().parents[1]
SOURCE = PROJECT / "watchface/resources/images/art_atlas.png"
OUTPUT = SOURCE.parent
TARGET_SIZES = {"emery": (200, 228), "gabbro": (260, 260), "chalk": (180, 180)}
ROW_EDGES = (0, 313, 627, 929, 1254)
LIGHT_TILES = {1, 4, 8, 9, 10, 11}
STATION_FACADE_END = 168
STATION_RED_BANDS = ((177, 181), (188, 201))
ANALYST_PALETTES = {
    0: ((0, 0, 0), (255, 255, 255), (170, 170, 170), (85, 85, 85),
        (0, 0, 85), (0, 85, 85), (0, 170, 255), (255, 255, 170)),
    1: ((255, 255, 255), (170, 255, 255), (0, 0, 0),
        (170, 170, 170), (0, 170, 170)),
}


def limit_palette(native: Image.Image, palette: tuple[tuple[int, int, int], ...]) -> None:
    """Snap generated fringe hues to the approved colors for a design."""
    pixels = native.load()
    nearest: dict[tuple[int, int, int], tuple[int, int, int]] = {}
    for y in range(native.height):
        for x in range(native.width):
            color = pixels[x, y]
            if color not in nearest:
                nearest[color] = min(
                    palette,
                    key=lambda target: sum((a - b) ** 2 for a, b in zip(color, target)),
                )
            pixels[x, y] = nearest[color]


def clean_station(tile: Image.Image) -> Image.Image:
    """Remove generated color specks while retaining the station silhouette."""
    clean = tile.copy()
    pixels = clean.load()
    for y in range(clean.height):
        if y < 20:
            for x in range(clean.width):
                pixels[x, y] = (255, 255, 255)
            continue
        if y >= STATION_FACADE_END:
            band_color = ((170, 0, 0) if any(start <= y < end
                                              for start, end in STATION_RED_BANDS)
                          else (255, 255, 255))
            for x in range(clean.width):
                pixels[x, y] = band_color
            continue
        for x in range(clean.width):
            pixels[x, y] = ((0, 0, 0) if sum(pixels[x, y]) < 384
                            else (255, 255, 255))
    return clean


def clean_pale_art(native: Image.Image, index: int) -> None:
    """Limit generated fringe colors on the three paper-based compositions."""
    pixels = native.load()
    for y in range(native.height):
        for x in range(native.width):
            r, g, b = pixels[x, y]
            if index in {9, 10} and (y < 2 or y >= native.height - 2):
                pixels[x, y] = (255, 255, 255)
            elif (r + g + b) >= 600:
                pixels[x, y] = (255, 255, 255)
            elif index == 9 and b >= 85 and b >= g and b > r + 20:
                pixels[x, y] = (0, 85, 170) if b > g else (0, 85, 85)
            else:
                pixels[x, y] = (0, 0, 0)


def clean_bold_art(native: Image.Image, index: int) -> None:
    """Remove generator texture from the flat color and diagonal faces."""
    pixels = native.load()
    for y in range(native.height):
        for x in range(native.width):
            r, g, b = pixels[x, y]
            if index == 14:
                pixels[x, y] = (255, 85, 0) if r > b and r > g else (0, 85, 255)
            elif b > r and b >= g:
                pixels[x, y] = (85, 170, 255)
            elif r >= 220 and r > g:
                pixels[x, y] = (255, 85, 85)
            elif r >= 85 and r > g and r >= b:
                pixels[x, y] = (170, 85, 85)
            else:
                pixels[x, y] = (0, 0, 0)


def main() -> None:
    with Image.open(SOURCE) as source:
        art = source.convert("RGB")
        width, height = art.size
        if art.size != (1254, 1254):
            raise ValueError(f"Expected the approved 1254x1254 atlas, got {art.size}")
        for row in range(4):
            for col in range(4):
                left = round(col * width / 4)
                top = ROW_EDGES[row]
                right = round((col + 1) * width / 4)
                bottom = ROW_EDGES[row + 1]
                tile = art.crop((left, top, right, bottom))
                index = row * 4 + col
                if index == 8:
                    tile = clean_station(tile)
                for platform, size in TARGET_SIZES.items():
                    native = tile.resize(size, Image.Resampling.NEAREST)
                    pixels = native.load()
                    for y in range(size[1]):
                        for x in range(size[0]):
                            r, g, b = pixels[x, y]
                            if index in LIGHT_TILES and r > 220 and g > 210 and b > 160 and r >= g:
                                pixels[x, y] = (255, 255, 255)
                            else:
                                pixels[x, y] = tuple(min(255, ((value + 42) // 85) * 85)
                                                     for value in (r, g, b))
                            if index == 1 and pixels[x, y] == (255, 170, 170):
                                pixels[x, y] = (170, 255, 255)
                            if index in {9, 10} and pixels[x, y] == (255, 170, 170):
                                pixels[x, y] = (170, 170, 170)
                    if index in {4, 9, 10}:
                        clean_pale_art(native, index)
                    if index == 4:
                        pixels = native.load()
                        for y in range(native.height):
                            for x in range(native.width):
                                if pixels[x, y] == (255, 255, 255) and x % 2 == 0 and y % 2 == 0:
                                    pixels[x, y] = (255, 255, 170)
                    if index in ANALYST_PALETTES:
                        limit_palette(native, ANALYST_PALETTES[index])
                    if index in {13, 14}:
                        clean_bold_art(native, index)
                    if index == 8 and platform != "emery":
                        facade_height = round(size[1] * 0.55)
                        facade = native.crop((0, 0, size[0], facade_height))
                        inset = round(size[0] * 0.06)
                        top_inset = round(size[1] * 0.04)
                        facade = facade.resize((size[0] - 2 * inset,
                                                facade_height - 2 * top_inset),
                                               Image.Resampling.NEAREST)
                        native.paste((255, 255, 255), (0, 0, size[0], facade_height))
                        native.paste(facade, (inset, top_inset))
                    native.save(OUTPUT / f"art_{index:02d}_{platform}.png",
                                optimize=True)


if __name__ == "__main__":
    main()
