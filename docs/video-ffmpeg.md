# FFmpeg playback experiment

The `dxlib_video_ffmpeg` branch builds on `dxlib_video_mf` and replaces the video
reader with a CPU FFmpeg backend. The bounded decode queue, main-thread graphics
uploads, SSE2 expansion, and frame-gap instrumentation are shared with the
[Media Foundation experiment](video-playback.md).

This is an experimental **Windows x86 Release** build. The current setup uses
`OpenLR2.sln`; CMake, Debug, and x64 are not configured for this backend.

## Additional commits

1. **Build dependencies.** Add an isolated, pinned vcpkg manifest for FFmpeg
   8.1.2 (`avcodec`, `avformat`, and `swscale`), a static x86 Release triplet, an
   installer, and a shared MSBuild property sheet. Paths are relative to the
   repository and can be overridden with `OpenLR2FFmpegRoot`.
2. **Video backend.** Add `OpenLR2FFReader`, replace sample reads and seeks in
   the existing worker, and mark CSV metadata with `variant=ffmpeg`.
3. **Documentation.** Record the supported build, playback scope, validation,
   and observed comparison.

## Build

Install the project's normal dependencies as described in the main README.
From the repository root, install the separate FFmpeg dependency set:

```powershell
.\tools\video\ffmpeg\Install-FFmpeg.ps1 -VcpkgExe 'C:\path\to\vcpkg.exe'
```

The manifest pins the vcpkg baseline used by this experiment. Dependencies are
installed under `vcpkg_installed/ffmpeg`; that directory is ignored by Git.
An uncached FFmpeg build can take several minutes.

Open `OpenLR2.sln` in Visual Studio 2022 and select **Release / x86**, or run from
a Visual Studio developer shell:

```powershell
MSBuild.exe OpenLR2.sln /t:OpenLR2 /p:Configuration=Release /p:Platform=x86 /m:2
```

To reuse an existing installation of the same x86 static Release profile:

```powershell
MSBuild.exe OpenLR2.sln /t:OpenLR2 /p:Configuration=Release /p:Platform=x86 /m:2 /p:OpenLR2FFmpegRoot='C:\path\to\installed\x86-ffmpeg'
```

The output is `out/x86/Release/OpenLR2_x86.exe`. It needs the normal LR2 runtime
files. The four FFmpeg libraries are statically linked; FFmpeg DLLs are not
required by this profile. Dependency license text is available in
`vcpkg_installed/ffmpeg/x86-ffmpeg/share/ffmpeg/copyright`.

## Playback scope

`OpenLR2FFReader` uses libavformat for demuxing and libavcodec with two decoder
threads. It drains delayed frames at EOF, flushes decoder state after seeking,
and converts timestamps to the existing movie timeline. Limited-range YUV420P
frames are copied directly into Y and interleaved UV planes; other supported
pixel formats use libswscale to produce NV12.

The existing worker owns decode activity after startup. The owner joins the
worker before destroying the reader. Texture creation, conversion, locking, and
upload stay on the rendering thread. FFmpeg does not remove those costs.

The branch retains DxLib's dedicated Ogg Theora path and the existing audio
handling. Some initialization and audio code still uses Media Foundation. It
does not provide a DirectShow fallback for failed FFmpeg video loads, and it
does not retain the old reader's archive-to-temporary-file extraction path.

The intended input is local, finite-duration video with valid frame-rate
metadata and even dimensions up to 8192 pixels per axis. Unsupported metadata,
resolution changes, and PQ/HLG HDR frames are rejected. Broad variable-frame-rate
coverage and color fidelity across formats remain unverified.

The backend writes `ffmpeg-backend-<pid>.log` in the process working directory.
It records startup/ready status and errors, including the media path. Frame
summary and slow-gap CSVs are written beside the executable on normal exit.
Follow the [shared comparison procedure and metric definitions](video-playback.md#build-and-compare).

## Recorded comparison

These are the existing local runs for the same approximately 150.486-second
song and 1080p H.264, 30 fps BGA. All values below come from `qpc_gap` summaries.
The original and optimized MF cases each have one run; FFmpeg has two. BGA OFF
was recorded earlier and is included only as context.

| Variant | Frames | Mean (ms) | >2 ms (%) | Excess above 2 ms (ms) | P95 upper (ms) | P99 upper (ms) | Max (ms) | >=10 ms frames |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Original MF, BGA ON | 63,601 | 2.366096 | 51.9394 | 27,479.6399 | 6.80 | 9.15 | 37.4378 | 215 |
| Optimized MF, BGA ON | 72,285 | 2.081856 | 51.7092 | 10,411.3537 | 3.15 | 4.35 | 16.4129 | 1 |
| FFmpeg, BGA ON, run 1 | 72,604 | 2.072702 | 51.0702 | 9,942.9379 | 3.15 | 4.10 | 6.1031 | 0 |
| FFmpeg, BGA ON, run 2 | 72,712 | 2.069621 | 51.1167 | 9,684.1325 | 3.05 | 4.05 | 9.3039 | 0 |
| BGA OFF (earlier run) | 75,154 | 2.002378 | 50.6440 | 3,558.1386 | 2.20 | 2.30 | 7.8366 | 0 |

Relative to the recorded optimized MF run, FFmpeg's mean gap was about 0.4-0.6%
lower and its accumulated excess above 2 ms was about 4.5-7.0% lower. The small
sample does not establish a repeatable backend advantage. Approximately half
of all intervals still exceeded 2 ms, including the BGA OFF run. Switching
decoders has not solved the strict 2 ms target.

Additional source capture identifiers (raw recordings are not committed):

- Run 1: `frame-summary-320672-7677888078.csv`
- Run 2: `frame-summary-300456-7678545421.csv`

## Validation

The x86 experiment was checked with a 90-frame H.264 fixture: complete decoder
drain, six forward/backward/EOF seeks with identical pixel hashes, twelve
open/close cycles, a Unicode filename, and missing-file cleanup. A separate
DxLib/D3D9 check exercised three playback, seek, texture draw, and teardown
cycles with advancing frames. Gameplay captures supplied the measurements
above. These checks do not cover every codec or input format.
