# openlr2-config.xml

English | [한국어](openlr2-config.ko.md) | [简体中文](openlr2-config.zh-Hans.md) | [日本語](openlr2-config.ja.md)

This file holds OpenLR2-only settings. It sits next to the original LR2 settings file:

`LR2files/Config/openlr2-config.xml`

`LR2files/Config/config.xml` is still used for the usual LR2 options. The options on this page are read only from `openlr2-config.xml`.

## Before you edit

- Close the game first. OpenLR2 writes this file again when it quits, and that can replace your edit.
- If this file is missing, the game still starts and uses the defaults below. A normal exit creates the file. If the file is there but the XML cannot be read, the game stops with "Failed to read OpenLR2 config".
- `0` means off. `1` means on, unless the option lists other numbers.
- If a line is missing, the game uses the default shown below.
- Leave the tag names as they are. Only change the value between the tags.
- The file uses the Japanese text encoding Shift_JIS. Paths with Korean, Chinese, or Japanese letters are safer to set inside the game, then quit so the game saves them.

## system

### resolution

Default size for skins that do not set their own resolution. A skin that has its own `#RESOLUTION` line uses that size on that screen. This value is the fallback.

| Value | Size | Name |
| --- | --- | --- |
| 0 | 640×480 | SD |
| 1 | 1280×720 | HD |
| 2 | 1920×1080 | Full HD |

A number other than 1 or 2 is shown as SD (640×480). The number in the file is not changed to 0, and quitting the game writes that same number back. The SD size is used only when the number is turned into pixels: `1` is 1280×720, `2` is 1920×1080, and every other value is 640×480.

### fullscreenfilter

How the picture is smoothed when it is scaled to fill the screen.

| Value | Look |
| --- | --- |
| 0 | Smooth (bilinear) |
| 1 | Sharp pixels (nearest) |
| 2 | CRT |

### fullscreenfitstretch

What to do when the picture and the monitor are not the same shape.

| Value | Result |
| --- | --- |
| 0 | Keep the picture's shape. Bars may appear on the sides or on the top and bottom. |
| 1 | Stretch the picture until it fills the screen. It may look wider or taller than it should. |

### screenmode

How the game window opens. The `screenmode` in `config.xml` is not used. OpenLR2 uses the `screenmode` in this file.

| Value | Result |
| --- | --- |
| 0 | Full screen |
| 1 | Window |
| 2 | Borderless full screen (a window with no frame, covering the monitor) |

While reading the file, a number less than 0 or greater than 2 is replaced with 0 in memory. A normal exit then saves `0`. `0` opens exclusive full screen at the monitor's current mode. `1` opens a window. `2` opens a borderless window that covers the monitor. `0` and `2` are both full screen; only the full-screen method differs. This replacement happens when the file is read, before the window mode is applied. The window-mode function itself would treat an out-of-range number as borderless, but a value from this file never arrives there still out of range.

## play

### gaugeautoshift

Gauge Auto Shift (GAS). You can also turn this on from the song select option pages. The game saves that choice here.

| Value | Result |
| --- | --- |
| 0 | Off. You stay on the gauge you picked. |
| 1 | On. If you would fail that gauge, the game moves you to an easier gauge that you are still clearing. |

G-ATTACK is left as it is.

### newrandomrestart

On the result screen, holding a white key and a blue key together restarts the song. This option chooses the note order for that restart.

| Value | Result |
| --- | --- |
| 0 | The note order stays the same. |
| 1 | Key 2 (the blue note) shuffles the notes again. The other blue keys keep the same order. |

This does not change the restart during play (hold Start and Select, then press Start for a new order or Select for the same order).

If this line is missing, the default is 0 (off).

### lifttype

Turns lift on or off. Lift moves the notes and the judge line up. It does not change the timing.

| Value | Result |
| --- | --- |
| 0 | Off. The `lift` number below does nothing. |
| 1 | On. |

Any number other than 0 is treated as 1. Both players start with the same setting.

### lift

How far the notes and the judge line move up, from 0 to 100. 0 is the normal position. 100 is the highest. This only changes how the chart looks.

Both players start with this number. In Battle, each side can be changed while you play, but this file only remembers player 1's number for the next launch.

## skin

### courseresult

Path to the skin used on the course result screen. Other skins (select, play, result, and so on) stay in `config.xml`. Original LR2 has no slot for this skin, so OpenLR2 keeps it here.

Leave it empty if you have not chosen one.

## network

### display_ir

Name of the internet ranking you want to see in the song list and on the result screen. It must match a folder name inside `LR2files/CustomIRs`.

Leave it empty to keep the normal LR2 ranking display. If the name does not match a folder, that ranking is not shown.

## Example

```xml
<?xml version="1.0" encoding="shift_jis"?>
<config>
	<system>
		<resolution>0</resolution>
		<fullscreenfilter>0</fullscreenfilter>
		<fullscreenfitstretch>0</fullscreenfitstretch>
		<screenmode>1</screenmode>
	</system>
	<play>
		<gaugeautoshift>0</gaugeautoshift>
		<newrandomrestart>0</newrandomrestart>
		<lifttype>0</lifttype>
		<lift>0</lift>
	</play>
	<skin>
		<courseresult></courseresult>
	</skin>
	<network>
		<display_ir></display_ir>
	</network>
</config>
```
