# Media Foundation playback experiment

The `dxlib_video_mf` branch contains the measured Media Foundation playback
optimizations and frame-gap instrumentation for **Windows x86 Release**.
The aim is to reduce main-thread stalls during BGA playback. The strict target is
to eliminate frame gaps above 2 ms; the measurements below do not meet that target.

## Changes and commit boundaries

The branch starts at `047ae03c` on `dxlib_video`. Its changes are split into:

1. **Frame-gap instrumentation.** Record per-song summaries of every measured
   gameplay interval, plus a bounded ring of slow intervals. Measure draw, movie
   update, flip, and the movie texture conversion, lock, unlock, and upload paths.
2. **Decoder thread limit.** Request two Media Foundation decoder worker threads
   through `CODECAPI_AVDecNumWorkerThreads`. A decoder that does not support the
   setting keeps its own defaults.
3. **Asynchronous decoding.** Run sample reads and CPU frame preparation on a
   dedicated worker with three queued frames. Keep graphics operations on the
   main thread. Use a ready frame when available and retain the previous image
   when decoding has not caught up. Serialize seeks, discard stale generations,
   and join the worker before releasing its reader and buffers.
4. **SSE2 channel expansion.** Accelerate the Y and UV byte expansion used by
   the texture path, retaining scalar tails and the existing fallback.
5. **Documentation.** Explain the experiment, measurement rules, and limitations.

The separate `dxlib_video_ffmpeg` branch inherits these commits and replaces the
video reader with an experimental FFmpeg backend. Comparing the two branches
therefore isolates the backend changes from the shared queue and rendering work.

## Build and compare

Use Visual Studio 2022 and the existing vcpkg dependencies. Build `OpenLR2.sln`
with **Release / x86**, or run this from a Visual Studio developer shell:

```powershell
MSBuild.exe OpenLR2.sln /t:OpenLR2 /p:Configuration=Release /p:Platform=x86 /m:2
```

The solution maps x86 to the projects' Win32 configuration. The executable is
written to `out/x86/Release`. Keep builds in separate LR2 runtime directories, or
give the executables distinct names, so each run's backend is known.

For each variant, use the same chart, BGA, skin, frame limiter, display settings,
and background applications. Play the same section and exit normally. Tracing
starts automatically and writes `frame-summary-<pid>-<stamp>.csv` and
`frame-gap-<pid>-<stamp>.csv` beside the executable at shutdown. Crashes and forced
termination can prevent those files from being written. No WPR capture is needed
for these CSV summaries; an ETW capture is useful for investigating a particular
stall's cause.

Repeat each variant several times and alternate their order. Compare equivalent
song rows and recorded durations, rather than the on-screen average alone.

## Reading the metrics

- Use the **`qpc_gap` row** for interval statistics. `above_2ms` counts strictly
  greater than 2 ms. Divide it by `frames` for the percentage.
- `excess_over_2ms_sum` is the sum of `max(gap_ms - 2, 0)`. It distinguishes many
  tiny overruns from fewer large stalls. Compare it over matching durations.
- `p95_upper_ms` and `p99_upper_ms` are upper bounds from 0.05 ms histogram bins,
  not exact sorted percentiles. The >=20 ms tail uses the observed maximum.
- The summary covers every successfully recorded interval for up to 128 songs.
  The first boundary in each song is excluded. Check `contention_dropped` and
  `summary_overflow` in the metadata before treating a summary as complete.
- The separate gap file retains at most 4,096 intervals whose displayed GAP or
  QPC gap is at least 10 ms. It does **not** enumerate all >2 ms intervals. Check
  `overwritten` before interpreting its event count.
- `movie` is nested inside `draw`; texture stages are nested within movie work.
  Do not add overlapping measurements. `covered` is their union and `other` is
  the remainder of the interval. These scopes do not measure the decode worker's
  CPU time. Per-metric maxima may come from different frames.

## Recorded comparison

These local x86 Release runs used the same approximately 150.486-second song
with a 1080p H.264, 30 fps BGA. The original playback run included the same
summary instrumentation. BGA OFF was captured earlier and is contextual, not a
simultaneous controlled baseline. Values below are from the `qpc_gap` rows.

| Variant | Frames | Mean (ms) | >2 ms (%) | Excess above 2 ms (ms) | P95 upper (ms) | P99 upper (ms) | Max (ms) | >=10 ms frames |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Original MF, BGA ON | 63,601 | 2.366096 | 51.9394 | 27,479.6399 | 6.80 | 9.15 | 37.4378 | 215 |
| Optimized MF, BGA ON | 72,285 | 2.081856 | 51.7092 | 10,411.3537 | 3.15 | 4.35 | 16.4129 | 1 |
| BGA OFF (earlier run) | 75,154 | 2.002378 | 50.6440 | 3,558.1386 | 2.20 | 2.30 | 7.8366 | 0 |

The optimized MF run reduced the accumulated excess above 2 ms by approximately
62%, but approximately half of the intervals still exceeded 2 ms. This is a
reduction in stall severity, not evidence that the target has been achieved.
One run per MF variant is insufficient to establish a repeatable performance gain.

Source capture identifiers (raw recordings are not included in the repository):

- Original: `frame-summary-326548-7675602343.csv`
- Optimized: `frame-summary-325292-7675777875.csv`
- BGA OFF: `frame-summary-301088-7593379687.csv`

## Validation and remaining scope

The optimization work was exercised in local x86 Release gameplay. Standalone
SSE2 checks covered 1,344 output comparisons and 130 guard-page boundary cases.
The instrumentation is retained on these experimental branches and has its own
overhead. Results do not establish correctness or performance across other
codecs, resolutions, hardware, or build configurations. Queue starvation can
repeat a displayed video frame even when the game thread remains responsive.
