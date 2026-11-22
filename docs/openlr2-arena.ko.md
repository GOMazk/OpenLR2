# OpenLR2 Arena

Arena는 OpenLR2 멀티플레이입니다. 차트·랜덤 시드를 공유하고, 그래프/인게임 페이스메이커로 라이브 점수를 보며, IR가 지원하면 방 결과도 남길 수 있습니다.

세션마다 오버레이 **Connection**(`Insert`)에서 모드를 하나 고릅니다.

| 모드 | 하는 일 |
|---|---|
| **IR (CustomIR)** | Arena를 지원하는 CustomIR을 `display_ir`로 두고 로그인한 뒤, IR에서 받은 **room id**로 **Join room**. OpenLR2가 방을 만들지는 않습니다. |
| **LAN** | 한 PC가 **Start server**, 나머지는 username + 호스트 IP로 **Connect** (포트 **2222**). Username / host는 기억됩니다. |

방 인원 상한은 OpenLR2 고정값이 아니라 **IR 모듈·서버**(또는 LAN 호스트 정책)가 정합니다.

## 시작하기

### IR

1. Arena를 지원하는 CustomIR을 `LR2files/CustomIRs/`에 설치합니다.
2. `openlr2-config.xml`의 `display_ir`를 그 폴더 이름으로 설정합니다.
3. 평소처럼 IR에 로그인합니다.
4. IR 쪽에서 방을 만들거나 열고 **room id**를 복사합니다.
5. 게임에서 `Insert` → room id 입력 → **Join room**.

### LAN

1. 호스트 PC: `Insert` → **LAN** → **Start server**.
2. 전원: **Username**과 **Server IP / hostname** 입력 → **Connect**.
3. 같은 LAN/VPN이면 됩니다. 통신은 `ws://host:2222`.

플레이 중 연결이 끊기면 동기화는 멈추고 토스트가 뜨지만, **로컬 플레이는 계속**됩니다. 의도적으로 나갈 때는 **Leave**.

## 오버레이

이동 가능한 로비 / 채팅 / 점수 창. 위치는 `LR2files/Config/imgui.ini`, 설정·키는 `LR2files/Config/arena.ini`에 저장됩니다.

| 동작 | 기본 |
|---|---|
| 로비 열기/닫기 | `Insert` |
| 점수/그래프 창 | `Page Up` (또는 Settings) |
| 채팅 전송 | **Send** 또는 Enter |
| 아이템 모드 | Lobby Settings 또는 `F4` |
| 로비 연 동안 게임 입력 뮤트 | Settings — **기본 ON** |
| 키 변경 | Settings → Bindings |
| ojama 사용 | **Backspace** (아이템 모드일 때) |
| Give host / Kick (호스트) | 로비에서 플레이어 우클릭 |

- 뮤트 **ON**(기본): 로비가 열려 있으면 게임 키가 막힙니다.
- 뮤트 **OFF**: 로비를 연 채로 선곡·플레이 가능. 채팅/room/LAN 입력 중에는 키보드가 오버레이에 잡힙니다.
- 오버레이 입력란은 **IME 변환을 지원하지 않습니다**(라틴/붙여넣기/UTF-8 표시는 가능).
- 오버레이가 전혀 안 보이면, 일반 그래픽 경로가 아닌 환경(예: 소프트웨어 렌더링)일 수 있습니다. Arena는 평소처럼 하드웨어 렌더링이 필요합니다.

## 진행

1. **호스트**가 차트를 먼저 고르면 hash·랜덤 시드가 공유됩니다.
2. 전원 로컬에 같은 차트(MD5, `song.db`)가 있어야 합니다. 없으면 로비에 알림이 뜨고 ready할 수 없습니다.
3. 로딩이 끝나면 모두 ready할 때까지 기다린 뒤 같이 시작합니다.
4. 플레이 중 점수는 그래프/페이스메이커에 대략 초당 10회 갱신됩니다.
5. 곡이 끝나면:
   - **IR:** 솔로와 같은 규칙으로 IR 랭킹 제출(ojama여도 막지 않음). 방 기록은 IR가 지원할 때만.
   - **LAN:** 피어 final 점수만 (원격 방 아카이브 없음).

`LR2files/Config/sample_*` 스킨 프리뷰 차트는 방 선곡으로 보내지 않습니다.

## ojama

호스트가 Settings / `F4`로 켭니다. 콤보로 아이템을 모으고(2×/3×에서 강화), **Backspace**로 상대에게 던집니다(~8초 효과). **Item settings**에서 가중치·임계값을 바꿀 수 있고 다른 사람에게도 동기화됩니다.

## 페이스메이커

인게임 페이스메이커 목표는 방 안 **상대 최고 EX**입니다. 점수 창에는 전원이 나옵니다.

## 참고

- **IR:** 통신은 CustomIR을 통합니다(P2P 아님). OpenLR2를 통해 공인 IP가 상대에게 보이지 않습니다.
- **LAN:** LAN/VPN의 **2222** 포트. CustomIR과 별개입니다.
- 오버레이를 연 직후 첫 Join/Connect는 짧게 멈출 수 있습니다.
- 라이브 동기화는 점수/그래프/페이스메이커만입니다(상대 노트 레인 라이브 없음).
