# LunaticRave2_rewrite
Rewritten source code of bms player Lunatic Rave 2
Project started Jan, 2021. And now it's about to release.

progress is now on test for different behaviors.

# installation
get original LR2 beta3 100201
put lr2body.exe and fmod.dll into the folder and replace original LR2body.exe (or you can use other filename)

# caution
It uses same score DB with origianl one.
To avoid data corruption, do not run this on where you played original LR2.
please use newly installed LR2.
sending score to LR2IR is disabled to avoid accidents.
(accidents can be occur, by saving score with LR2_re and sending score with original LR2. DO NOT MIX)

# known issues
 - behavior mismatch on NONSTOP MIX

# libraries
*DxLib (3.02 -> 3.12a)
	suspected orginal library is dxlib 3.01 ~ dxlib3.02
	but, to avoid LoadJpegImage() crash error I used dxlib3.12a (2014/4/19)
 
*SQLite 3.6.7
	version doesn't matter

*FMODex 4.13.4(080401)
	is replaced with FMOD 20207(220518)
	because I can't find the library of correspondng version
	Unfortunately, due to this problem the audio FX is not same with original version.
 
