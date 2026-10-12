# Changelog

## v2.0.0-beta.1 — 2026-10-12

Community-enhanced branch based on `dg4l/wiimenu-gcn-hack`.

### Added

- Per-axis ±12 deadzone for the GameCube analog stick, addressing neutral-position cursor drift.
- GameCube START mapped to Wii HOME **in the Wii System Menu only**.
- Wii Remote / GameCube input coexistence, using the first free virtual controller slot instead of disconnecting the Wii Remote.
- Automatic virtual pointer reassignment between P1 and P2 as physical Wii Remotes join or leave the Wii Menu.
- Bilingual documentation, installation alternatives, safety notes and build/testing scripts.

### Verified

- Nintendo Wii 4.3U: channel selection with GameCube A; START/HOME; drift-free cursor; Wii Remote and GameCube pointer independence; P1/P2 slot switching.
- 4.3U/E/J: compiled and statically checked; simulated input tests passed. Hardware validation pending for 4.3E and 4.3J.

### Known limitations

- No in-game START → HOME feature; the GameCube controller remains a regular GameCube controller in Wii titles that support it.
- Simultaneous gameplay and controller assignment within individual games/homebrew are controlled by those applications, not by this Wii Menu patch.
- 4.3K not supported.
