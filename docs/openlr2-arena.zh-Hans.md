# OpenLR2 Arena

Arena 是 OpenLR2 的多人模式：共享谱面与随机种子，用图表／游戏内节拍器查看实时分数；若 IR 支持，还可保存房间结果。

每个会话在覆盖层 **Connection**（`Insert`）中选择一种模式。

| 模式 | 做什么 |
|---|---|
| **IR (CustomIR)** | 将支持 Arena 的 CustomIR 设为 `display_ir` 并登录，用 IR 提供的 **room id** 点 **Join room**。OpenLR2 本身不创建房间。 |
| **LAN** | 一台 **Start server**，其他人输入用户名 + 主机 IP 后 **Connect**（端口 **2222**）。Username / host 会被记住。 |

房间人数上限不是 OpenLR2 的固定值，由 **IR 模块／服务器**（或 LAN 主机策略）决定。

## 开始使用

### IR

1. 将支持 Arena 的 CustomIR 放到 `LR2files/CustomIRs/`。
2. 在 `openlr2-config.xml` 把 `display_ir` 设为该文件夹名。
3. 按平时方式登录 IR。
4. 在 IR 侧创建或打开房间并复制 **room id**。
5. 游戏内：`Insert` → 输入 room id → **Join room**。

### LAN

1. 主机：`Insert` → **LAN** → **Start server**。
2. 所有人：填写 **Username** 与 **Server IP / hostname** → **Connect**。
3. 同一 LAN／VPN 即可；通信走 `ws://host:2222`。

曲中断线会停止同步并弹出提示，但 **本地游玩继续**。主动退出请用 **Leave**。

## 覆盖层

可移动的大厅／聊天／分数窗。位置保存在 `LR2files/Config/imgui.ini`，设置与按键在 `LR2files/Config/arena.ini`。

| 操作 | 默认 |
|---|---|
| 打开／关闭大厅 | `Insert` |
| 分数／图表窗 | `Page Up`（或 Settings） |
| 发送聊天 | **Send** 或 Enter |
| 道具模式 | Lobby Settings 或 `F4` |
| 大厅打开时屏蔽游戏输入 | Settings — **默认开启** |
| 改键 | Settings → Bindings |
| 使用 ojama | **Backspace**（道具模式开启时） |
| 转让房主／踢人（房主） | 在大厅右键玩家 |

- 屏蔽 **ON**（默认）：大厅打开时游戏按键无效。
- 屏蔽 **OFF**：可在大厅打开时选谱／游玩。在聊天／room／LAN 框输入时键盘仍会被覆盖层占用。
- 覆盖层输入框 **不支持 IME 组字**（拉丁／粘贴／UTF-8 显示可用）。
- 若覆盖层完全不出现，可能是非正常图形路径（例如软件渲染）。Arena 需要平时的硬件渲染。

## 流程

1. **房主**先选谱，共享 hash 与随机种子。
2. 全员本地需有同一谱面（MD5，`song.db`）。缺失会在大厅提示，且无法 ready。
3. 加载结束后等待全员 ready，然后同时开始。
4. 游玩中分数大约每秒更新 10 次（图表／节拍器）。
5. 曲终后：
   - **IR：** 按与单人相同的规则提交 IR 排名（ojama **不**阻止）。仅当 IR 支持时写入房间记录。
   - **LAN：** 仅同伴最终分数（无远程房间归档）。

`LR2files/Config/sample_*` 皮肤预览谱面不会作为房间选谱发送。

## ojama

房主用 Settings／`F4` 开启。连击积攒道具（2×／3× 加强），**Backspace** 扔给同伴（约 8 秒效果）。可在 **Item settings** 调整权重／阈值并同步给他人。

## 节拍器

游戏内节拍器目标为房间内 **对手最高 EX**。分数窗显示所有人。

## 说明

- **IR：** 通信经 CustomIR（非 P2P）。通过 OpenLR2 不会向同伴暴露公网 IP。
- **LAN：** 走 LAN／VPN 的 **2222** 端口，与 CustomIR 无关。
- 刚打开覆盖层后首次 Join／Connect 可能短暂卡顿。
- 实时同步仅为分数／图表／节拍器（无对手音符轨直播）。
