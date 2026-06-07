# OpenLR2
Rewritten source code of BMS player Lunatic Rave 2. Project started in January 2021.
Original code accessible at [LR2Beta3-v100201](https://github.com/GOMazk/OpenLR2/tree/LR2Beta3-v100201)

## Open source?

After the closure of LR2IR, the project has been made public.

## Installation

- Download unmodified Lunatic Rave 2 beta3 100201 [archive.org Download](
https://web.archive.org/web/20190802100906/http://www.dream-pro.info/~lavalse/LR2IR/search.cgi?mode=download)

- Place thew OpenLR2 executable in the root folder of your freshly downloaded LR2. You can also optionally replace the original LR2body.exe.

> [!CAUTION]
> OpenLR2 uses the same score DB as the original LR2.
> 
> To avoid data corruption, it's generally recommended to have a fresh LR2 directory, or use the "-ns" flag.
> 
> Backing up your "LR2files\Database\Score\" folder is highly recommended.

> [!NOTE]
> Existing song.db will be refreshed on first OpenLR2 launch.
> That is normal.
> This is needed because LR2 didn't account for leap year days in database timestamps.

## New Features

- Proper fullscreen support – The game doesn't crash on Alt+Tab or when toggling window mode with F4 in-game.
  
- Gauge Auto Shift (GAS) – Changes the gauge to the easier one automatically if you failed on the selected one. You can access GAS options in MANIAC OPTIONS by pressing F2 in-game and going to the 3rd page using the arrow keys.
  
- Native fast/slow support
  
- Quick restart – Lets you restart the chart without going to result screen. Press both Start and Select, then quickly release and press either Start for a new random, or Select for the same random you had before.
  
- MainBPM Hi-Speed Fix – Hi-Speed anchor is set at the most prevalent BPM in the chart.
  
- UTF-8 support – Essentially means you don't need to change the locale in Windows settings to play.
  
- Base62 support – support more keysounds on some newer charts that require them.

## How do I use HD skins with OpenLR2?

<img width="441" height="64" alt="image" src="https://github.com/user-attachments/assets/30f21a53-aa14-430b-ae74-9a11c62f3c49" />

- Add the corresponding #RESOLUTION under #INFORMATION in your .lr2skin files for each of the game scenes (music select, decide, play, result, course result...) Refer to the image above as an example.

## Known issues

 - score save issue on NONSTOP MIX
 - readme left click scroll is faster than original (not important)

# For developers

## Build

Use visual studio 2022, right click on OpenLR2_vs22, build. I'm working on x86 release build only

## Libraries (you don't need to get these)

*DxLib 3.01a => 3.24f (custom old dxa) – currently using custom dxlib to read dxa files.

*FMODex 4.13.4(080401) => FMOD 2.3.10
 
*SQLite 3.6.7

*tinyxml

*md5.c, md5.h
 from https://github.com/Zunawe/md5-c. not in original LR2 but to save time I used this.

# Thanks to
- .red who wrote lr2skin specification
- All contributors

