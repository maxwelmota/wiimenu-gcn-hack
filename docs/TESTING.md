# Hardware test notes and suggested test matrix

## Confirmed observations (physical Nintendo Wii, System Menu 4.3U)

- GameCube analog pointer does not drift noticeably while untouched.
- GameCube A can highlight, select, and open channels on the original Wii System Menu, including with only the GC pad connected.
- GC START opens the Wii System Menu HOME interface; GC A can confirm within it.
- Wii Remote and GameCube controller each have their **own independent cursor** and both can interact with the System Menu.
- With GameCube alone, its virtual pointer behaves as P1. When Wii Remote P1 connects, GameCube shifts to P2. On Wii Remote disconnect, GameCube can return to P1.
- USB Loader GX treated both as P1 within its own UI in the reported test.
- *Tatsunoko vs. Capcom*: native GameCube controls were functional; no simultaneous play through the patch was demonstrated. The patch does not affect game-local controller reading.

## Additional tests requested before stable release

- [ ] Connect/disconnect Wii Remote while holding GC A and START; check for lost/stuck/duplicate events.
- [ ] Connect GC while Wii Remote A is held; verify button events and slot ownership.
- [ ] Return repeatedly to Wii System Menu from different channels and games.
- [ ] Three and four connected Wii Remotes: verify slot exhaustion does not displace physical inputs.
- [ ] Leave the Wii System Menu running 30–60 minutes, retest pointers, buttons and reconnections.
- [ ] Verify input mapping on an actual 4.3E console.
- [ ] Verify input mapping on an actual 4.3J console.
- [ ] Validate Priiloader version and alternate SD/USB storage load paths.

## Reporting a bug

Provide the Wii System Menu version/region (e.g. 4.3U / v513), Priiloader version, GameCube port and controller, Wii Remote pairing details, steps to reproduce, expected/observed behavior, and whether the issue occurs with only the GameCube controller connected. Do not use unverified patches without an accessible recovery path.
