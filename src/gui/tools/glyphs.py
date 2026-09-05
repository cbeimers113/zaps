import argparse
import math
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

GLYPH_HEADER_START = "/* GLYPH GENERATION START */"
GLYPH_HEADER_END = "/* GLYPH GENERATION END */"


def load_font(font_path: str, size: int) -> ImageFont.FreeTypeFont:
    """Load a TrueType/OpenType font at the requested size."""
    return ImageFont.truetype(font_path, size)


def pack_rgba(r: int, g: int, b: int, a: int) -> int:
    """Pack 8-bit RGBA components the way main.c's set_pixel_rgba does."""
    return (r << 24) | (g << 16) | (b << 8) | a


def extract_glyph_pixels(
    sheet: Image.Image, characters: str, columns: int, cell_size: int
) -> dict[str, list[int]]:
    """Crop each glyph's cell out of the rendered sheet as packed RGBA pixels."""
    glyphs = {}

    for index, character in enumerate(characters):
        column = index % columns
        row = index // columns

        cell_x = column * cell_size
        cell_y = row * cell_size

        cell = sheet.crop((cell_x, cell_y, cell_x + cell_size, cell_y + cell_size))
        glyphs[character] = [pack_rgba(r, g, b, a) for r, g, b, a in cell.getdata()]

    return glyphs


def render_spritesheet(
    characters: str,
    font_path: str,
    cell_size: int,
    output_path: str,
    columns: int = 16,
    padding: int = 0,
) -> dict[str, list[int]]:
    """
    Render characters into a fixed-size glyph spritesheet.

    Each glyph gets exactly one cell_size x cell_size cell. Returns each
    character mapped to its packed RGBA pixels (row-major).
    """

    # Remove duplicate characters while preserving order.
    characters = "".join(dict.fromkeys(characters))

    if not characters:
        raise ValueError("No characters were provided.")

    font = load_font(font_path, cell_size)

    rows = math.ceil(len(characters) / columns)

    sheet_width = columns * cell_size
    sheet_height = rows * cell_size

    # RGBA gives us a transparent background.
    sheet = Image.new(
        "RGBA",
        (sheet_width, sheet_height),
        (0, 0, 0, 0),
    )

    draw = ImageDraw.Draw(sheet)

    for index, character in enumerate(characters):
        column = index % columns
        row = index // columns

        cell_x = column * cell_size
        cell_y = row * cell_size

        # Get the glyph's bounding box.
        bbox = draw.textbbox(
            (0, 0),
            character,
            font=font,
        )

        glyph_width = bbox[2] - bbox[0]
        glyph_height = bbox[3] - bbox[1]

        # Center the glyph in its fixed-size cell.
        x = cell_x + (cell_size - glyph_width) // 2 - bbox[0]
        y = cell_y + (cell_size - glyph_height) // 2 - bbox[1]

        draw.text(
            (x, y),
            character,
            font=font,
            fill=(255, 255, 255, 255),
        )

    sheet.save(output_path)

    print(
        f"Created {output_path}: "
        f"{len(characters)} glyphs, "
        f"{columns} columns × {rows} rows, "
        f"{sheet_width}×{sheet_height}px"
    )

    return extract_glyph_pixels(sheet, characters, columns, cell_size)


def glyph_array_name(resolution: int, character: str) -> str:
    return f"GLYPH_{resolution}_{ord(character)}"


def format_glyph_array(resolution: int, character: str, pixels: list[int]) -> str:
    # Glyph/Glyph32 are already sized array typedefs; adding a size here would
    # declare an array-of-Glyph instead of a single flat pixel array.
    type_name = "Glyph" if resolution == 16 else "Glyph32"
    name = glyph_array_name(resolution, character)

    lines = [f"static {type_name} {name} = {{"]
    lines += [f"    0x{pixel:08x}," for pixel in pixels]
    lines.append("};")

    return "\n".join(lines)


def format_glyphs_table(resolution: int, glyphs: dict[str, list[int]]) -> str:
    # A Glyph/Glyph32 array decays to uint32_t* (pointer to its first pixel),
    # not to Glyph*/Glyph32* (pointer to a whole other array), so the table
    # holding those decayed pointers must be typed uint32_t* too.
    table_name = f"GLYPHS_{resolution}"

    # Sparse array indexable by ASCII value, e.g. GLYPHS_16[ord('A')].
    size = max(128, max((ord(c) for c in glyphs), default=0) + 1)

    lines = [f"static uint32_t *{table_name}[{size}] = {{"]
    for character in sorted(glyphs, key=ord):
        lines.append(f"    [{ord(character)}] = {glyph_array_name(resolution, character)},")
    lines.append("};")

    return "\n".join(lines)


def build_glyph_generation_block(
    glyphs_16: dict[str, list[int]], glyphs_32: dict[str, list[int]]
) -> str:
    parts = []

    for character in sorted(glyphs_16, key=ord):
        parts.append(format_glyph_array(16, character, glyphs_16[character]))

    for character in sorted(glyphs_32, key=ord):
        parts.append(format_glyph_array(32, character, glyphs_32[character]))

    parts.append(f"static int NUM_GLYPHS = {len(glyphs_16)};")
    parts.append(format_glyphs_table(16, glyphs_16))
    parts.append(format_glyphs_table(32, glyphs_32))

    return "\n\n".join(parts)


def write_glyph_header(
    header_path: Path, glyphs_16: dict[str, list[int]], glyphs_32: dict[str, list[int]]
) -> None:
    """Overwrite the GLYPH GENERATION region of glyph.h with freshly rendered glyphs."""
    text = header_path.read_text(encoding="utf-8")

    start = text.index(GLYPH_HEADER_START) + len(GLYPH_HEADER_START)
    end = text.index(GLYPH_HEADER_END)

    body = build_glyph_generation_block(glyphs_16, glyphs_32)
    new_text = f"{text[:start]}\n\n{body}\n\n{text[end:]}"

    header_path.write_text(new_text, encoding="utf-8")

    print(f"Updated {header_path}: {len(glyphs_16)} glyphs (16px + 32px)")


def read_input_text(value: str) -> str:
    """
    Treat the argument as a filename if it exists,
    otherwise treat it as literal text.
    """
    path = Path(value)

    if path.is_file():
        return path.read_text(encoding="utf-8")

    return value


def main():
    parser = argparse.ArgumentParser(
        description="Generate 16x16 and 32x32 glyph spritesheets."
    )

    parser.add_argument(
        "text",
        help="Text to include, or path to a UTF-8 text file.",
    )

    parser.add_argument(
        "--font",
        required=True,
        help="Path to a .ttf or .otf font.",
    )

    parser.add_argument(
        "--columns",
        type=int,
        default=16,
        help="Number of glyph columns per spritesheet (default: 16).",
    )

    parser.add_argument(
        "--output-dir",
        default=".",
        help="Directory for generated spritesheets.",
    )

    parser.add_argument(
        "--header",
        help="Path to glyph.h; if given, overwrites its GLYPH GENERATION region.",
    )

    args = parser.parse_args()

    text = read_input_text(args.text)

    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    # Generate 16x16 sheet.
    glyphs_16 = render_spritesheet(
        characters=text,
        font_path=args.font,
        cell_size=16,
        output_path=str(output_dir / "glyphs_16.png"),
        columns=args.columns,
    )

    # Generate 32x32 sheet.
    glyphs_32 = render_spritesheet(
        characters=text,
        font_path=args.font,
        cell_size=32,
        output_path=str(output_dir / "glyphs_32.png"),
        columns=args.columns,
    )

    if args.header:
        write_glyph_header(Path(args.header), glyphs_16, glyphs_32)


if __name__ == "__main__":
    main()
