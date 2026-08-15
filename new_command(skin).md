# New Command(.lr2skin)
Legacy LR2 command doccumentation.

[English](https://lr2skinhelp.vercel.app/) [日本語](https://right-stick.sub.jp/lr2skinhelp.html)
## `#INFORMATION`
`#INFORMATION` now can declare it's own size.

```
//before
#INFORMATION,{type},{title},{maker},{thumbnail},
//after
#INFORMATION,{type},{title},{maker},{thumbnail},{width},{height}
```

## `#RESOLUTION` (From Beatoraja)
* 251126

```csv
#RESOLUTION,0 //SD 480p
#RESOLUTION,1 //HD 720p
#RESOLUTION,2 //FULLHD 1080p
#RESOLUTION,3 //ULTRAHD 2160p
```

## `#SRC_IMAGE`
### gr:`106`
Used in SkinSelect

## dst_option
### `626`

Imported score displayed

### `270` (OpenLR2 lift)
* 260807

Also active while Start/Select is held and lift is on (same timing as Sudden+ value display). Shows the lift value HUD when lift is enabled.

### `274`
* 260817

is 1P lanecover enabled

### `275`
* 260817

is 2P lanecover enabled

## bargraph_type
### `48` (from LR2 with Fast/Slow)
* 260624

1P Slow count %
### `49` (from LR2 with Fast/Slow)
* 260624

1P Fast count %
### `58` (from LR2 with Fast/Slow)
* 260624

2P Slow count %
### `59` (from LR2 with Fast/Slow)
* 260624

2P Fast count %

## num
### `201` (from LR2 with Fast/Slow)
1P hit offset (ms)

### `210` (from LR2 with Fast/Slow)
1P FAST/SLOW

### `211` (from LR2 with Fast/Slow)
2P FAST/SLOW

### `212` (from LR2 with Fast/Slow)
Fast count

### `213` (from LR2 with Fast/Slow)
2P hit offset (ms)

### `214` (from LR2 with Fast/Slow)
Slow count

### `216` (from LR2 with Fast/Slow)
Combo Breaks

### `217` (from LR2 with Fast/Slow)
Remaining notes

### `218` (from LR2 with Fast/Slow)
Running notes

### `295` (from LR2OOL)
* 251128

1P random pattern
### `418` (from LR2OOL)
* 251128

2P random pattern

### `421` (OpenLR2 lift)
* 260807

1P lift value (0–100). Drawn only when lift is on.

### `422` (OpenLR2 lift)
* 260807

2P lift value (0–100). Drawn only when lift is on.

## slider_type
### `27` (OpenLR2 lift)
* 260807

1P lift cover / range. `op2` is lane height used for note offset. Active only when lift is on.

### `28` (OpenLR2 lift)
* 260807

2P lift cover / range. `op2` is lane height used for note offset. Active only when lift is on.

## button_type
### `55` (div:`5`)
* 251128
Main BPM

### `400` (div:`3`)
* 260710
scaling filter

### `401` (div:`2`)
* 260710
stretch

### `331` (div:`2`)
* 260807

Lift ON/OFF (song select). Same 0/1 pattern as Sudden+ shutter (`46`).


## text_st
### type:`74`, div:`5`
* 251128
Main BPM

### type:`300`, div:`3`
* 260710
scaling filter

### type:`301`, div:`2`
* 260710
stretch

### type:`302`
* 260807

Lift ON/OFF label text (song select).
