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

