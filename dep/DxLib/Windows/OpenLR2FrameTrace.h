#ifndef OPENLR2_FRAME_TRACE_H
#define OPENLR2_FRAME_TRACE_H
#ifdef _WIN32
#include <windows.h>
#include <evntprov.h>
#include <stdio.h>
#pragma comment(lib, "advapi32.lib")
namespace OpenLR2FrameTrace {
enum Kind { Draw, Movie, Flip, Texture, Convert, Lock, Unlock, Upload, Count };
enum { Capacity = 4096 };
struct Row { LONGLONG begin, end, ticks[Count], covered; double displayGap; DWORD tid; LONG song; };
enum { SongCapacity=128, MetricCount=12, HistogramBins=401 };
struct Metric {
 unsigned long long count, bins[HistogramBins], below3, below5, atLeast10, above2;
 double sum, minimum, maximum, excess2Sum, excess2Max;
 void Add(double ms) {
  if(!count || ms<minimum)minimum=ms;
  if(!count || ms>maximum)maximum=ms;
  ++count;sum+=ms;
  if(ms>2){++above2;excess2Sum+=ms-2;if(ms-2>excess2Max)excess2Max=ms-2;}
  if(ms<3)++below3;if(ms<5)++below5;if(ms>=10)++atLeast10;
  int bin=ms<=0?0:(ms>=20?400:(int)(ms*20));++bins[bin];
 }
 // Upper edge of a 0.05ms histogram bin; max is used for the >=20ms tail.
 double Percentile(unsigned percent) const {
  unsigned long long target=(count/100)*percent+((count%100)*percent+99)/100, n=0;
  for(int i=0;i<HistogramBins;i++) {
   n+=bins[i];if(n>=target)return i==400?maximum:(i+1)*0.05;
  }
  return maximum;
 }
};
struct SongSummary {
 Metric metrics[MetricCount];
 double displayAverage, over2Sums[MetricCount];
 unsigned long long over2Frames;
 LONGLONG begin,end;
};
struct State {
 volatile LONG enabled, song, dropped;
 CRITICAL_SECTION lock;
 REGHANDLE provider;
 LONGLONG frequency;
 unsigned long long total;
 DWORD mainThread;
 Row rows[Capacity];
 SongSummary summaries[SongCapacity];
 unsigned long long summaryOverflow;
};
inline State &Global() { static State value = {}; return value; }
struct Local {
 int depth[Count], anyDepth;
 LONGLONG start[Count], total[Count], previous[Count], anyStart, anyTotal, previousCovered, boundary;
 LONG song;
 bool valid;
};
inline Local &Thread() { static thread_local Local value = {}; return value; }
inline LONGLONG Now() { LARGE_INTEGER t; QueryPerformanceCounter(&t); return t.QuadPart; }
inline bool Enabled() { return InterlockedCompareExchange(&Global().enabled,0,0)!=0; }
class Scope {
 int kind;
 bool active;
public:
 explicit Scope(int k, bool gate=true):kind(k),active(gate&&Enabled()) {
  if(!active)return;
  Local &l=Thread(); LONGLONG t=Now();
  if(l.depth[k]++==0)l.start[k]=t;
  if(l.anyDepth++==0)l.anyStart=t;
 }
 ~Scope() {
  if(!active)return;
  Local &l=Thread();LONGLONG t=Now();
  if(--l.depth[kind]==0)l.total[kind]+=t-l.start[kind];
  if(--l.anyDepth==0)l.anyTotal+=t-l.anyStart;
 }
 Scope(const Scope&)=delete; Scope&operator=(const Scope&)=delete;
};
// Only measure graphics calls nested inside the movie update on this thread.
template<class F> inline auto MeasureMovie(int kind,F fn)->decltype(fn()) {
 Scope scope(kind,Thread().depth[Movie]>0);return fn();
}
inline void Reset() { if(Enabled())InterlockedIncrement(&Global().song); }
inline void Boundary(double displayGap, double displayAverage=0) {
 if(!Enabled())return;
 State &g=Global();Local &l=Thread();LONGLONG t=Now();
 LONG song=InterlockedCompareExchange(&g.song,0,0);
 Row row={};row.begin=l.boundary;row.end=t;row.displayGap=displayGap;row.song=song;row.tid=GetCurrentThreadId();
 for(int k=0;k<Count;k++) {
  LONGLONG value=l.total[k]+(l.depth[k]?t-l.start[k]:0);
  row.ticks[k]=value-l.previous[k];l.previous[k]=value;
 }
 LONGLONG covered=l.anyTotal+(l.anyDepth?t-l.anyStart:0);
 row.covered=covered-l.previousCovered;l.previousCovered=covered;
 bool valid=l.valid&&l.song==song;
 bool record=valid&&(displayGap>=10.0||t-l.boundary>=g.frequency/100);
 l.boundary=t;l.song=song;l.valid=true;
 if(!valid)return;
 if(TryEnterCriticalSection(&g.lock)) {
  if(Enabled()) {
   if(song>0 && song<=SongCapacity) {
    SongSummary &summary=g.summaries[song-1];
    double scale=1000.0/g.frequency;
    double values[MetricCount]={displayGap,(row.end-row.begin)*scale,
     row.ticks[Draw]*scale,row.ticks[Movie]*scale,row.ticks[Flip]*scale,
     row.covered*scale,(row.end-row.begin-row.covered)*scale,
     row.ticks[Texture]*scale,row.ticks[Convert]*scale,row.ticks[Lock]*scale,
     row.ticks[Unlock]*scale,row.ticks[Upload]*scale};
    if(!summary.metrics[0].count)summary.begin=row.begin;
    summary.end=row.end;summary.displayAverage=displayAverage;
    for(int i=0;i<MetricCount;i++)summary.metrics[i].Add(values[i]);
    if(row.end-row.begin>g.frequency/500) {
     ++summary.over2Frames;
     for(int i=0;i<MetricCount;i++)summary.over2Sums[i]+=values[i];
    }
   } else ++g.summaryOverflow;
   if(record)g.rows[g.total++%Capacity]=row;
  }
  LeaveCriticalSection(&g.lock);
 } else InterlockedIncrement(&g.dropped);
 if(!record)return;
 // Only slow gaps emit an ETW marker. QPC boundaries exclude marker cost;
 // its cost, if any, belongs to the following interval.
 if(g.provider&&EventProviderEnabled(g.provider,4,0)) {
  wchar_t text[640];
  _snwprintf_s(text,_countof(text),_TRUNCATE,
   L"OpenLR2SlowGap v=1 song=%ld tid=%lu begin=%lld end=%lld freq=%lld display_ms=%.6f draw_ticks=%lld movie_ticks=%lld flip_ticks=%lld covered_ticks=%lld",
   row.song,row.tid,row.begin,row.end,g.frequency,row.displayGap,row.ticks[Draw],row.ticks[Movie],row.ticks[Flip],row.covered);
  EventWriteString(g.provider,4,0,text);
 }
}
inline void Init() {
 State &g=Global();InitializeCriticalSection(&g.lock);
 LARGE_INTEGER f;QueryPerformanceFrequency(&f);g.frequency=f.QuadPart;g.mainThread=GetCurrentThreadId();
 // Reuse the GUID enabled by the existing MF-Diagnostics WPR profile.
 const GUID provider={0xc6f39439,0xf328,0x4778,{0xa8,0x4e,0xce,0x81,0x5a,0xc4,0x18,0x01}};
 EventRegister(&provider,NULL,NULL,&g.provider);
 InterlockedExchange(&g.enabled,1);
}
inline void Shutdown() {
 State &g=Global();if(!InterlockedExchange(&g.enabled,0))return;
 EnterCriticalSection(&g.lock);
 wchar_t path[MAX_PATH];DWORD n=GetModuleFileNameW(NULL,path,_countof(path));
 if(n&&n<_countof(path)) {
  wchar_t *slash=wcsrchr(path,L'\\');
  if(slash) {
   ULONGLONG stamp=GetTickCount64();
   size_t remain=_countof(path)-(slash+1-path);
   _snwprintf_s(slash+1,remain,_TRUNCATE,L"frame-gap-%lu-%llu.csv",GetCurrentProcessId(),stamp);
   FILE *f=NULL;
   if(_wfopen_s(&f,path,L"wb")==0&&f) {
    fprintf(f,"# version=1,variant=ffmpeg,qpc_frequency=%lld,main_tid=%lu,slow_total=%llu,capacity=%d,overwritten=%llu,contention_dropped=%ld\n",g.frequency,g.mainThread,g.total,Capacity,g.total>Capacity?g.total-Capacity:0,g.dropped);
    fprintf(f,"song,tid,begin_qpc,end_qpc,display_gap_ms,qpc_gap_ms,draw_ms,movie_ms,flip_ms,covered_ms,other_ms\n");
    unsigned long long first=g.total>Capacity?g.total-Capacity:0;
    double scale=1000.0/g.frequency;
    for(unsigned long long i=first;i<g.total;i++) {
     const Row&r=g.rows[i%Capacity];
     fprintf(f,"%ld,%lu,%lld,%lld,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",r.song,r.tid,r.begin,r.end,r.displayGap,(r.end-r.begin)*scale,r.ticks[Draw]*scale,r.ticks[Movie]*scale,r.ticks[Flip]*scale,r.covered*scale,(r.end-r.begin-r.covered)*scale);
    }
    fclose(f);
   }
   _snwprintf_s(slash+1,remain,_TRUNCATE,L"frame-summary-%lu-%llu.csv",GetCurrentProcessId(),stamp);
   f=NULL;
   if(_wfopen_s(&f,path,L"wb")==0&&f) {
    fprintf(f,"# version=3,variant=ffmpeg,qpc_frequency=%lld,main_tid=%lu,contention_dropped=%ld,summary_overflow=%llu,histogram_bin_ms=0.05,histogram_tail_ms=20,percentiles=upper_bound,first_boundary_per_song=excluded\n",g.frequency,g.mainThread,g.dropped,g.summaryOverflow);
    fprintf(f,"song,begin_qpc,end_qpc,display_avg_gap_ms,metric,frames,mean_ms,min_ms,max_ms,p50_upper_ms,p95_upper_ms,p99_upper_ms,below_3ms,below_5ms,at_least_10ms,above_2ms,excess_over_2ms_sum,excess_over_2ms_max,qpc_gap_over_2ms_frames,mean_when_qpc_gap_over_2ms\n");
    const char *names[MetricCount]={"display_gap","qpc_gap","draw","movie","flip","covered","other","movie_texture_total","movie_convert_copy","movie_lock","movie_unlock","movie_update_surface"};
    for(int i=0;i<SongCapacity;i++) {
     const SongSummary &summary=g.summaries[i];
     if(!summary.metrics[0].count)continue;
     for(int k=0;k<MetricCount;k++) {
      const Metric &m=summary.metrics[k];
      fprintf(f,"%d,%lld,%lld,%.6f,%s,%llu,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%llu,%llu,%llu,%llu,%.6f,%.6f,%llu,%.6f\n",
       i+1,summary.begin,summary.end,summary.displayAverage,names[k],m.count,m.sum/m.count,
       m.minimum,m.maximum,m.Percentile(50),m.Percentile(95),m.Percentile(99),m.below3,m.below5,m.atLeast10,m.above2,m.excess2Sum,m.excess2Max,summary.over2Frames,summary.over2Frames?summary.over2Sums[k]/summary.over2Frames:0);
     }
    }
    fclose(f);
   }
  }
 }
 LeaveCriticalSection(&g.lock);
 // Keep the lock alive for a late caller that observed enabled before shutdown.
 // Provider is unregistered after gameplay threads have been shut down by main.
 if(g.provider)EventUnregister(g.provider);
}
struct Session { Session(){Init();} ~Session(){Shutdown();} };
}
#else
namespace OpenLR2FrameTrace {
enum Kind { Draw, Movie, Flip };
struct Scope { explicit Scope(int){} };struct Session {};
inline void Reset(){} inline void Boundary(double,double=0){}
}
#endif
#endif
