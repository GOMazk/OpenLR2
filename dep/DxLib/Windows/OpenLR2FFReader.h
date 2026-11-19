#pragma once
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <new>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/pixdesc.h>
}
// Experimental CPU video backend. Owned by the movie; accessed only by its
// decode worker after startup. The owner joins that worker before destruction.
class OpenLR2FFReader {
 AVFormatContext *format = nullptr;
 AVCodecContext *codec = nullptr;
 AVPacket *packet = nullptr;
 AVFrame *frame = nullptr;
 SwsContext *scale = nullptr;
 int stream = -1;
 bool draining = false;
 int64_t origin = 0, nextTime = 0;
 std::string path;
public:
 int width=0,height=0;
 AVRational rate={0,1},aspect={1,1};
 int64_t duration=0;
 void Log(const char *event,int error=0) const {
  char name[96],message[AV_ERROR_MAX_STRING_SIZE]={};
  sprintf_s(name,"ffmpeg-backend-%lu.log",GetCurrentProcessId());
  av_strerror(error,message,sizeof(message));
  FILE *f=nullptr; fopen_s(&f,name,"ab");
  if(f) { fprintf(f,"%s error=%d (%s) size=%dx%d fps=%d/%d threads=2 decoder=%s ffmpeg=%s file=%s\n",event,error,message,width,height,rate.num,rate.den,codec && codec->codec ? codec->codec->name : "none",av_version_info(),path.c_str()); fclose(f); }
 }
 ~OpenLR2FFReader() {
  sws_freeContext(scale); av_frame_free(&frame); av_packet_free(&packet);
  avcodec_free_context(&codec); avformat_close_input(&format);
 }
 int Open(const wchar_t *name) {
  int n=WideCharToMultiByte(CP_UTF8,0,name,-1,nullptr,0,nullptr,nullptr);
  if(n<=1) return -1;
  path.resize(n); WideCharToMultiByte(CP_UTF8,0,name,-1,&path[0],n,nullptr,nullptr); path.resize(n-1);
  int r=avformat_open_input(&format,path.c_str(),nullptr,nullptr);
  if(r<0) { Log("open_failed",r); return r; }
  if((r=avformat_find_stream_info(format,nullptr))<0) { Log("probe_failed",r); return r; }
  const AVCodec *decoder=nullptr;
  stream=av_find_best_stream(format,AVMEDIA_TYPE_VIDEO,-1,-1,&decoder,0);
  if(stream<0) return stream;
  AVStream *st=format->streams[stream];
  codec=avcodec_alloc_context3(decoder); if(!codec) return AVERROR(ENOMEM);
  if((r=avcodec_parameters_to_context(codec,st->codecpar))<0) return r;
  codec->thread_count=2;
  if((r=avcodec_open2(codec,decoder,nullptr))<0) { Log("decoder_failed",r); return r; }
  width=codec->width; height=codec->height;
  rate=av_guess_frame_rate(format,st,nullptr);
  aspect=st->sample_aspect_ratio.num ? st->sample_aspect_ratio : AVRational{1,1};
  origin=st->start_time==AV_NOPTS_VALUE ? 0 : st->start_time;
  if(st->duration!=AV_NOPTS_VALUE) duration=av_rescale_q(st->duration,st->time_base,AVRational{1,10000000});
  else if(format->duration!=AV_NOPTS_VALUE) duration=av_rescale_q(format->duration,AV_TIME_BASE_Q,AVRational{1,10000000});
  // Existing DxLib UV textures assume even dimensions and finite-duration video.
  if(width<=0 || height<=0 || width>8192 || height>8192 || (width&1) || (height&1) || rate.num<=0 || rate.den<=0 || duration<=0) { Log("unsupported_metadata",-1); return -1; }
  packet=av_packet_alloc(); frame=av_frame_alloc();
  if(!packet || !frame) return AVERROR(ENOMEM);
  Log("opened"); return 0;
 }
 int Seek(int64_t time) {
  int64_t target=origin+av_rescale_q(time,AVRational{1,10000000},format->streams[stream]->time_base);
  int r=av_seek_frame(format,stream,target,AVSEEK_FLAG_BACKWARD);
  if(r<0) { Log("seek_failed",r); return r; }
  avcodec_flush_buffers(codec); av_packet_unref(packet); av_frame_unref(frame);
  draining=false; nextTime=time; return 0;
 }
 int Read(int64_t &time) {
  av_frame_unref(frame);
  for(;;) {
   int r=avcodec_receive_frame(codec,frame);
   if(r==0) {
    if(frame->width!=width || frame->height!=height || frame->color_trc==AVCOL_TRC_SMPTE2084 || frame->color_trc==AVCOL_TRC_ARIB_STD_B67) { Log("unsupported_frame",-1); return -1; }
    int64_t ts=frame->best_effort_timestamp;
    time=ts==AV_NOPTS_VALUE ? nextTime : av_rescale_q(ts-origin,format->streams[stream]->time_base,AVRational{1,10000000});
    nextTime=time+av_rescale_q(1,av_inv_q(rate),AVRational{1,10000000}); return 0;
   }
   if(r!=AVERROR(EAGAIN)) { if(r!=AVERROR_EOF) Log("receive_failed",r); return r; }
   if(draining) return AVERROR_EOF;
   do {
    av_packet_unref(packet); r=av_read_frame(format,packet);
   } while(r>=0 && packet->stream_index!=stream);
   if(r<0 && r!=AVERROR_EOF) { Log("demux_failed",r); return r; }
   if(r==AVERROR_EOF) { draining=true; r=avcodec_send_packet(codec,nullptr); }
   else r=avcodec_send_packet(codec,packet);
   // A receive EAGAIN immediately before send guarantees send can accept input.
   av_packet_unref(packet);
   if(r<0 && r!=AVERROR_EOF) { Log("send_failed",r); return r; }
  }
 }
 int Copy(void *y,int yp,void *uv,int uvp) {
  if(frame->format==AV_PIX_FMT_YUV420P && frame->color_range!=AVCOL_RANGE_JPEG) {
   for(int row=0;row<height;++row) memcpy((uint8_t*)y+row*yp,frame->data[0]+row*frame->linesize[0],width);
   for(int row=0;row<height/2;++row) {
    uint8_t *d=(uint8_t*)uv+row*uvp;
    const uint8_t *u=frame->data[1]+row*frame->linesize[1],*v=frame->data[2]+row*frame->linesize[2];
    for(int x=0;x<width/2;++x) { d[2*x]=u[x]; d[2*x+1]=v[x]; }
   }
   return 0;
  }
  scale=sws_getCachedContext(scale,width,height,(AVPixelFormat)frame->format,width,height,AV_PIX_FMT_NV12,SWS_BILINEAR,nullptr,nullptr,nullptr);
  if(!scale) return -1;
  const int *coeff=sws_getCoefficients(frame->colorspace==AVCOL_SPC_BT709 ? SWS_CS_ITU709 : SWS_CS_ITU601);
  sws_setColorspaceDetails(scale,coeff,frame->color_range==AVCOL_RANGE_JPEG,coeff,0,0,1<<16,1<<16);
  uint8_t *dst[4]={(uint8_t*)y,(uint8_t*)uv,nullptr,nullptr}; int pitch[4]={yp,uvp,0,0};
  return sws_scale(scale,frame->data,frame->linesize,0,height,dst,pitch)==height ? 0 : -1;
 }
};
