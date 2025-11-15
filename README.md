# Purpose Of This Branch
 for "LR2 beta3 ver100201" moding. improvement is not allowed, this is a evidence that started from LR2

# OpenLR2
Rewritten source code of bms player Lunatic Rave 2
Project started Jan, 2021. And now it's about to release.

progress is now on test to find different behaviors.

# installation
get original LR2 beta3 100201
put OpenLR2.exe into the folder and run (or you can replace original LR2body.exe )

# caution
It uses same score DB with origianl one.
To avoid data corruption, do not run this on where you played original LR2 (or use -ns option).
recommend use newly installed LR2.
sending score to LR2IR is disabled to avoid accidents.
(accidents can be occur, by saving score with OpenLR2 and sending score with original LR2. DO NOT MIX yet)

# known issues
 - score save issue on NONSTOP MIX
 - readme left click scroll is faster than original (not important)

# build
use visual studio 2008, with japanese locale (locale emulator works).

# libraries
*DxLib 3.01a -> 3.02
	orginal library is dxlib 3.01a, not available. we can get 3.01 or 3.02.

*FMODex 4.13.4(080401)
 
*SQLite 3.6.7

*tinyxml

*md5.c, md5.h
 from https://github.com/Zunawe/md5-c. not in original LR2 but to save time I used this.


