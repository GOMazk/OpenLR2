# OpenLR2 Arena

Arena is OpenLR2 multiplayer: share a chart and random seed, watch live scores (graph / in-game pacemaker), and optionally keep room results when your IR supports it.

Pick one connection mode per session in the overlay **Connection** panel (`Insert`).

| Mode | What you do |
|---|---|
| **IR (CustomIR)** | Use an Arena-capable CustomIR as `display_ir`, log in, get a **room id** from your IR, then **Join room**. OpenLR2 does not create rooms itself. |
| **LAN** | One PC **Start server**, others enter username + host IP and **Connect** (port **2222**). Username / host are remembered. |

Room size (max players) is decided by your **IR module / server** (or LAN host policy), not by a fixed OpenLR2 cap.

## Getting started

### IR

1. Install an Arena-capable CustomIR under `LR2files/CustomIRs/`.
2. Set `display_ir` in `openlr2-config.xml` to that folder name.
3. Log in with the IR’s usual credentials.
4. Create or open a room on the IR side and copy the **room id**.
5. In-game: `Insert` → enter room id → **Join room**.

### LAN

1. On the host PC: `Insert` → **LAN** → **Start server**.
2. On every PC: enter **Username** and **Server IP / hostname** → **Connect**.
3. Same LAN or VPN is enough; traffic goes to `ws://host:2222`.

If the connection drops mid-song, sync stops and a toast appears, but **you keep playing locally**. Use **Leave** to exit the lobby on purpose.

## Overlay

Movable lobby / chat / score windows. Positions save to `LR2files/Config/imgui.ini`; settings and key binds to `LR2files/Config/arena.ini`.

| Action | Default |
|---|---|
| Open / close lobby | `Insert` |
| Score / graph window | `Page Up` (or Settings) |
| Send chat | **Send** or Enter |
| Item mode | Lobby Settings or `F4` |
| Mute game input while lobby open | Settings — **ON** by default |
| Rebind keys | Settings → Bindings |
| Use ojama item | **Backspace** (while item mode is on) |
| Give host / Kick (host) | Right-click a player in the lobby |

- Mute **ON** (default): game keys are blocked while the lobby is open.
- Mute **OFF**: you can select charts / play with the lobby open. Typing in chat / room / LAN fields still captures the keyboard.
- Overlay text fields do **not** support IME composition (Latin / paste / UTF-8 display still work).
- If the overlay never appears, OpenLR2 is likely running without a normal graphics device (for example software rendering). Arena needs the usual hardware rendering path.

## Play flow

1. The **host** picks the chart first; everyone gets the hash and random seed.
2. Everyone must have that chart locally (same MD5 in `song.db`). Missing chart → notify in lobby; that player cannot ready.
3. After loading, the game waits until everyone is ready, then starts together.
4. During play, peer scores update on the graph / pacemaker (about 10 times per second).
5. After the song:
   - **IR:** normal IR ranking submit (same rules as solo; ojama does **not** block it). Room history is stored only if your IR supports it.
   - **LAN:** peers see final scores only (no remote room archive).

Skin-preview charts under `LR2files/Config/sample_*` are not sent as room charts.

## Ojama (item) mode

Host turns it on (Settings / `F4`) before / with chart select. Combo builds an item (stronger at 2× / 3× threshold); **Backspace** throws it at peers (~8s effects). Host can tune weights / threshold in **Item settings** (synced to others).

## Pacemaker

The in-game pacemaker targets the **highest opponent EX** in the room. The score window lists everyone.

## Notes

- **IR:** live traffic goes through your CustomIR (not peer-to-peer). Other players do not see your public IP via OpenLR2.
- **LAN:** uses your LAN/VPN to port **2222**; separate from CustomIR.
- First Join / Connect right after opening the overlay may hitch briefly.
- Live sync is scores / graph / pacemaker only (no live opponent note lanes).
