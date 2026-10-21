# openlr2-config.xml

[English](openlr2-config.md) | [한국어](openlr2-config.ko.md) | 简体中文 | [日本語](openlr2-config.ja.md)

这个文件只放 OpenLR2 自己的设置。它和原来的 LR2 设置文件放在一起：

`LR2files/Config/openlr2-config.xml`

`LR2files/Config/config.xml` 仍然负责原来的 LR2 选项。本页这些选项只从 `openlr2-config.xml` 读取。

## 修改之前

- 先关掉游戏再改。游戏退出时会重新保存这个文件，手改的内容可能被盖掉。
- 没有这个文件时，游戏仍会用下面的默认值启动。正常退出后会创建这个文件。文件存在但 XML 读不出来时，游戏会显示 `Failed to read OpenLR2 config` 并停止启动。
- 没有特别说明时，`0` 是关，`1` 是开。
- 某一行不存在时，使用下面写的默认值。
- 不要改标签名字。只改标签中间的值。
- 文件使用日文编码 Shift_JIS。路径里有中文、韩文或日文时，最好在游戏里选好，再退出让游戏保存。

## system

### resolution

皮肤没有自己指定大小时使用的画面尺寸。皮肤文件写了 `#RESOLUTION` 时，那个画面用皮肤里的尺寸，这个值是那时的默认大小。

| 值 | 尺寸 | 名称 |
| --- | --- | --- |
| 0 | 640×480 | SD |
| 1 | 1280×720 | HD |
| 2 | 1920×1080 | Full HD |

不是 1 或 2 的数字会以 640×480 显示。文件里的数字不会改成 0，退出游戏时也会把原来的数字写回去。SD 尺寸只在把数字换成像素时使用：`1` 是 1280×720，`2` 是 1920×1080，其他值都是 640×480。

### fullscreenfilter

画面被放大铺满屏幕时，画面怎么平滑。

| 值 | 效果 |
| --- | --- |
| 0 | 平滑（bilinear） |
| 1 | 像素清晰（nearest） |
| 2 | CRT |

### fullscreenfitstretch

画面比例和显示器比例不一样时怎么处理。

| 值 | 结果 |
| --- | --- |
| 0 | 保持画面比例。左右或上下可能出现黑边。 |
| 1 | 把画面拉到铺满屏幕。看起来可能被拉宽或拉高。 |

### screenmode

游戏以什么窗口方式打开。不使用 `config.xml` 里的 `screenmode`。OpenLR2 使用这个文件里的 `screenmode`。

| 值 | 结果 |
| --- | --- |
| 0 | 全屏 |
| 1 | 窗口 |
| 2 | 无边框全屏（没有边框、盖住显示器的窗口） |

读取文件时，小于 0 或大于 2 的数字会在内存里换成 0。正常退出后，文件里也会保存 `0`。`0` 是按显示器当前模式打开的全屏。`1` 是窗口。`2` 是没有边框、盖住显示器的窗口。`0` 和 `2` 都是全屏，只是全屏方式不同。这个替换发生在读取文件时，早于套用窗口模式。套用窗口模式的函数会把超出范围的数字当成无边框全屏，但从这个文件读到的值在到达那里之前已经不再超出范围。

## play

### gaugeautoshift

血槽自动切换（GAS）。也可以在选曲界面的选项里打开，那个选择会保存在这里。

| 值 | 结果 |
| --- | --- |
| 0 | 关。一直使用你选的血槽。 |
| 1 | 开。如果这个血槽会失败，游戏会换成你仍在通过的更简单血槽。 |

G-ATTACK 不会被更换。

### newrandomrestart

在结算画面同时按住白键和蓝键会重开这首歌。这个选项决定重开时的音符顺序。

| 值 | 结果 |
| --- | --- |
| 0 | 音符顺序保持不变。 |
| 1 | 2号键（蓝键）会重新打乱音符。其他蓝键保持原来的顺序。 |

这和游玩中的重开无关（同时按住 Start 和 Select，然后按 Start 是新顺序，按 Select 是原顺序）。

这一行不存在时，默认值是 0（关）。

### lifttype

打开或关闭 lift。lift 会把音符和判定线往上移。判定时间不会变。

| 值 | 结果 |
| --- | --- |
| 0 | 关。下面的 `lift` 数字不会生效。 |
| 1 | 开。 |

不是 0 的数字都会当成 1。1P 和 2P 开始时使用同一设置。

### lift

音符和判定线向上移动多少，范围是 0 到 100。0 是原来的位置，100 是最高。只改变看起来的位置。

1P 和 2P 都从这个数字开始。对战中可以在游玩时分别调整两边，但下次启动时，这个文件只记住 1P 的数字。

## skin

### courseresult

段位（课程）结算画面使用的皮肤文件路径。选曲、游玩、普通结算等其他皮肤仍在 `config.xml` 里。原来的 LR2 没有这个皮肤栏，所以 OpenLR2 把它放在这里。

还没选择时留空。

## network

### display_ir

要在选曲画面和结算画面显示的网络排名名称。它必须和 `LR2files/CustomIRs` 里的文件夹名一致。

留空则继续使用原来的 LR2 排名显示。名称对不上文件夹时，那个排名不会显示。

## 示例

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
