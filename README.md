# Wii Menu GCN Hack — Enhanced

**GameCube controller support for the Nintendo Wii System Menu via Priiloader**, with improved analog-stick handling, START → HOME, and simultaneous Wii Remote support.

> **Community modification of [dg4l/wiimenu-gcn-hack](https://github.com/dg4l/wiimenu-gcn-hack), not an official release by the original author.** Many thanks to **dg4l** for making the original hack and publishing its source code. Also thanks to **koopthekoopa** for the [wii-ipl decompilation project](https://github.com/koopthekoopa/wii-ipl), acknowledged by the original author.

[**Leia em português (Brasil)**](README.pt-BR.md)

## What's new in this version?

- **Analog-stick deadzone**: a small ±12-per-axis deadzone eliminates observed idle cursor drift without impairing navigation. This uses the GameCube controller in port 1.
- **GameCube START → Wii HOME**: press START to open the HOME menu **while in the Wii System Menu**.
- **Wii Remote + GameCube coexistence**: both can point and click independently on the Wii System Menu. The hack no longer forcibly disconnects Wii Remote P1 and instead assigns the virtual GameCube pointer to a free controller slot.
- **Dynamic slot allocation**: with only GameCube present it can appear as P1; when Wii Remote P1 connects, GameCube can move to P2; if Wii Remote disconnects, GameCube can return to P1. This behavior was observed on a real Wii.
- **Retains the original GameCube button mapping**, including A for selecting and opening channels on the Wii System Menu.

## Important: scope and limitations

**START does NOT act as HOME inside Wii games.** The Priiloader patch changes the **Wii System Menu only**. When a Wii game with native GameCube controller support launches, the GameCube controller is handled **normally as a GameCube controller** by that game; START regains its ordinary in-game purpose. The patch does not add Wii Remote features to game code, change game controller mappings, or provide a universal way to return to the Wii Menu from a game using START.

Also:

- The two independent pointers are a **Wii System Menu** feature, not a promise of simultaneous control in every application or game.
- USB Loader GX, Homebrew Channel and emulators may have their **own controller mapping**. In the hardware test, USB Loader GX identified both inputs as P1 in its own interface; this does not indicate a failure in the System Menu patch.
- In the tested Wii game *Tatsunoko vs. Capcom*, native controls worked, but the two devices were not observed functioning simultaneously. Behavior elsewhere may differ.
- Only **4.3U (v513)** was validated on physical hardware. **4.3E (v514)** and **4.3J (v512)** were compiled and statically checked, **not yet tested on real consoles**. **4.3K is unsupported**.
- This is a community **beta**. Back up your current setup and maintain a working recovery path (Priiloader via RESET; preferably BootMii with a NAND backup).

## Download / installation

Exactly two installable INI files are provided at the project root:

| File | Purpose |
| --- | --- |
| [`gcn.ini`](gcn.ini) | **The hack only**. Append its entire contents to your existing `hacks_hash.ini`. **Recommended**, because it preserves your other custom hacks. |
| [`hacks_hash.ini`](hacks_hash.ini) | A **ready-merged example** containing a specific existing hacks collection plus this GameCube patch. Use only if you intend to replace your own collection; it is not a universal replacement for all Priiloader versions. |

### Option A — append only the hack (recommended)

1. Back up `sd:/apps/priiloader/hacks_hash.ini`.
2. Open your existing `hacks_hash.ini` in a plain-text editor.
3. Make sure the file ends with a **line break**; add a blank line between the last existing patch and the new section.
4. Append the full contents of **`gcn.ini`**, without duplicating older `[GameCube Controller Support]` sections. It contains three region-specific sections (U/E/J), which must all remain intact.
5. Save as `sd:/apps/priiloader/hacks_hash.ini` (plain text; not `.ini.txt`).
6. Boot Priiloader, open **System Menu Hacks**, enable **GameCube Controller Support**, then **Save settings**.
7. Test a GameCube controller connected to **port 1**.

### Option B — use the premerged example

Back up your own `hacks_hash.ini`, then copy the provided **`hacks_hash.ini`** to `sd:/apps/priiloader/hacks_hash.ini`. Activate the hack in Priiloader as above. **This may replace other hacks you already use**, so Option A is preferable for most users.

**Troubleshooting**: if the patch does not appear in the Priiloader list, verify the file path, the Wii System Menu version and **line separation** between INI sections. A missing newline between the previous hack and `[GameCube Controller Support]` was observed to prevent discovery.

If booting the Wii System Menu fails after enabling the hack, use your established Priiloader recovery method (typically hold RESET at startup), disable the hack, and restore your backed-up file. Do not test system menu patches without a way to recover.

## Testing status

| Feature | Wii 4.3U hardware test |
| --- | --- |
| No idle analog cursor drift | ✅ Confirmed |
| GameCube A opens Wii Menu channels | ✅ Confirmed |
| GameCube START opens Wii Menu HOME | ✅ Confirmed |
| Independent Wii Remote and GCN pointers | ✅ Confirmed |
| Automatic P1 ↔ P2 reassignment | ✅ Confirmed |
| In-game START → HOME | ❌ **Not provided by this patch** |
| 4.3E / 4.3J consoles | ⏳ Pending external hardware testing |

More detailed scenarios and known gaps: [docs/TESTING.md](docs/TESTING.md).

## Build from source

The source and build scripts are in [`source/`](source/). Building is **not necessary to install** the supplied INI files.

Requirements: `clang` (PowerPC backend), `ld.lld`, `gcc`, `g++` with `std::format` support, and Python 3. Run:

```bash
cd source
bash build-with-clang.sh
python3 test_gcnhook.py
cmp gcn.ini ../gcn.ini
```

This rebuilds three ELF patches, generates `source/gcn.ini` with the existing builder, and validates their layout. The committed root `gcn.ini` is the hardware-tested candidate, not automatically replaced by the build step. **The included ELF files are build outputs, not installable Wii applications** (ELF outputs are ignored by Git).

## Credits and license

- **Original implementation:** [dg4l/wiimenu-gcn-hack](https://github.com/dg4l/wiimenu-gcn-hack). Full credit for the original hack, injection method and baseline GameCube input translation goes to its author.
- **Foundational reverse-engineering:** [koopthekoopa/wii-ipl](https://github.com/koopthekoopa/wii-ipl), as also acknowledged by the original project.
- **Community-enhanced modifications:** deadzone, START-to-HOME mapping and non-exclusive virtual input-slot management. Verified on real Wii hardware through community testing.

The original **MIT license and copyright notice are preserved verbatim** in [LICENSE](LICENSE). See [CREDITS.md](CREDITS.md). If publishing as a fork, retain the GitHub fork relationship so the project history and original authorship remain visible.

## Contributions

Hardware reports are welcome, especially for **4.3E/4.3J**, full-slot handling, plug/unplug while buttons are held and long-duration stability. Please include console/menu region, Priiloader version, reproduction steps, and expected vs actual behavior. See [docs/TESTING.md](docs/TESTING.md).
