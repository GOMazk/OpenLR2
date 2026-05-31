# OpenLR2
Rewritten source code of bms player Lunatic Rave 2

Project started Jan, 2021. And now it's about to release (aged about one and half year sry).

current progress is 'doing whatever'

original codes remain at LR2Beta3-v100201 branch

## is it really open source?
~I didn't decide yet, I mailed to stakeholder(IR manager) but there was no reply~

LR2IR is closed, so YES

# For users
## installation
get original LR2 beta3 100201

put OpenLR2.exe into the folder and run (or you can replace original LR2body.exe )

## caution
It uses same score DB with origianl one.

If you want to avoid potential data corruption, do not run this on where you played original LR2 (or use -ns option), 

or backup your 'LR2files\Database\Score\'

~sending score to LR2IR is disabled to avoid accidents.~

~(accidents can be occur, by saving score with OpenLR2 and sending score with original LR2. DO NOT MIX yet)~

## New features

- Proper fullscreen support - The game doesn't crash on Alt+Tab or Fullscreen->Window change in the game menu
  
- Gauge Auto Shift (GAS) - Changes the gauge to the easier one automatically if you failed on the selected one. Turn it on in MANIAC OPTIONS (F2 -> 3rd page)
  
- Fast/Slow support
  
- Quick restart - Lets you restart the chart without going to result screen. Press both Start and Select, then quickly release and press either Start for a new random, or Select for the same random you had before.
  
- MainBPM Hi-Speed Fix - Hi-Speed anchor is set at the most prevalent BPM in the chart.
  
- UTF-8 support - Essentially means you don't need to change the locale in Windows settings to play.
  
- Base62 support - support more keysounds on some newer charts that require them.

## I want use HD skins
<img width="441" height="64" alt="image" src="https://github.com/user-attachments/assets/30f21a53-aa14-430b-ae74-9a11c62f3c49" />

add #RESOLUTION under #INFORMATION

## known issues
 - score save issue on NONSTOP MIX
 - readme left click scroll is faster than original (not important)

# For developers
## build
use visual studio 2022, right click on OpenLR2_vs22, build. I'm working on x86 release build only

## libraries (you don't need getting these)
*DxLib 3.01a => 3.24f(custom old dxa)
	currently using custom dxlib to read dxa files.

*FMODex 4.13.4(080401) => FMOD 2.3.10
 
*SQLite 3.6.7

*tinyxml

*md5.c, md5.h
 from https://github.com/Zunawe/md5-c. not in original LR2 but to save time I used this.

# Thanks to
.red who wrote lr2skin specification

and all contributters
