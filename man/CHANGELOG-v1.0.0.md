# Changelog v1.0.0

- `lcharmap` rewritten in C
- `--range` syntax converted to the more common `START-END` from `START,END`.
- `--range` now accepts numbers in binary, octal, hexadecimal.
- `--search` engine now uses `musl`'s buggy regex implementation, instead of
  the Rust crate.
