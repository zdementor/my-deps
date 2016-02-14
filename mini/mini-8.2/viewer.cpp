// (c) by Stefan Roettger

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE // mute loud VC8 compiler
#pragma comment(lib,"pthreadVC2.lib")
#pragma comment(lib,"libcurl.lib")
#pragma comment(lib,"squish.lib")
#endif

// configuration section:

#define VIEWER_WINWIDTH 768
#define VIEWER_WINHEIGHT 512

#define VIEWER_WINTITLE "libMini Viewer"

#define VIEWER_SCALE 100.0f // 1:100
#define VIEWER_EXAGGERATION 1.0f // no exaggeration

#define VIEWER_LOAD 0.1f // relative to far plane
#define VIEWER_PRELOAD 1.25f // relative to far plane
#define VIEWER_RANGE 0.001f // relative to far plane
#define VIEWER_RADIUS 3.0f // relative to range
#define VIEWER_DROPOFF 1.0f // lod dropoff

#define VIEWER_LAZYNESS 1 // paging lazyness
#define VIEWER_UPDATE 25 // update period
#define VIEWER_EXPIRE 1000 // expiration time

#define VIEWER_UPLOAD 0.02f // upload for 20ms
#define VIEWER_KEEP 0.5f // keep for 30sec
#define VIEWER_CACHE 128.0f // 128MB

#define VIEWER_FPS 25.0f // frames per second
#define VIEWER_FPU 25 // frames per cache update

#define VIEWER_RES 1.0E6f // global resolution

#define VIEWER_FOVY 60.0f
#define VIEWER_NEARP 10.0f
#define VIEWER_FARP 10000.0f

#define VIEWER_SBASE 0.5f

#define VIEWER_MAXSPEED 1000.0f
#define VIEWER_GRAVITY 10.0f
#define VIEWER_HOOVER 0.1f
#define VIEWER_JUMP 0.5f

#define VIEWER_TILESETFILE "tileset.sav"
#define VIEWER_VTBINIFILE "tileset.ini"
#define VIEWER_STARTUPFILE "startup.sav"
#define VIEWER_SAVFILE "settings.sav"

#ifndef _WIN32
#define VIEWER_LOCALPATH "/var/tmp/"
#else
#define VIEWER_LOCALPATH "C:\\Windows\\Temp\\"
#endif

#define VIEWER_SIGNPOSTHEIGHT 100.0f
#define VIEWER_SIGNPOSTRANGE 0.1f

#define VIEWER_BRICKSIZE 100.0f
#define VIEWER_BRICKRAD 1000.0f

#undef VIEWER_GREYCSTORATION

// greycstoration support:

#ifdef VIEWER_GREYCSTORATION

#include <pthread.h>

#define cimg_display_type 0
#define cimg_plugin "plugins/greycstoration.h"
#include "CImg.h"
using namespace cimg_library;

#include "minitime.h"

#endif

void denoise(unsigned char *image,int width,int height)
   {
#ifdef VIEWER_GREYCSTORATION

   const float amplitude      = 40.0f;
   const unsigned int nb_iter = 3;
   const float sharpness      = 0.9f; // -p option of greycstoration
   const float anisotropy     = 0.1f; // -a option of greycstoration
   const float alpha          = 0.6f;
   const float sigma          = 1.1f;
   const bool fast_approx     = true;
   const float gauss_prec     = 2.0f;
   const float dl             = 0.8f;
   const float da             = 30.0f;
   const unsigned int interp  = 0;
   const unsigned int tile    = 0;
   const unsigned int btile   = 4;

   CImg<unsigned char> img(image,width,height,1,3,false);

   for (int iter=0; iter<nb_iter; iter++)
      {
      img.greycstoration_run(amplitude,sharpness,anisotropy,alpha,sigma,dl,da,gauss_prec,interp,fast_approx,tile,btile);
      while (img.greycstoration_is_running()) waitfor(0.001);
      }

   memcpy(image,img.ptr(),3*width*height);

#endif
   }

// async paging support:

#include <pthread.h>

#include "datacloud.hpp"

static const int numthreads=10;

pthread_t pthread[numthreads];
pthread_mutex_t mutex,iomutex;
pthread_attr_t attr;

void threadinit()
   {
   pthread_mutex_init(&mutex,NULL);
   pthread_mutex_init(&iomutex,NULL);

   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
   }

void threadexit()
   {
   pthread_mutex_destroy(&mutex);
   pthread_mutex_destroy(&iomutex);

   pthread_attr_destroy(&attr);
   }

void startthread(void *(*thread)(void *background),backarrayelem *background,void *data)
   {pthread_create(&pthread[background->background-1],&attr,thread,background);}

void jointhread(backarrayelem *background,void *data)
   {
   void *status;
   pthread_join(pthread[background->background-1],&status);
   }

void lock_cs(void *data)
   {pthread_mutex_lock(&mutex);}

void unlock_cs(void *data)
   {pthread_mutex_unlock(&mutex);}

void lock_io(void *data)
   {pthread_mutex_lock(&iomutex);}

void unlock_io(void *data)
   {pthread_mutex_unlock(&iomutex);}

// http paging support:

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

CURL *curl_handle[numthreads+2];

struct MemoryStruct
   {
   char *memory;
   size_t size;
   };

void *myrealloc(void *ptr,size_t size)
   {
   if (ptr) return realloc(ptr,size);
   else return malloc(size);
   }

size_t WriteMemoryCallback(void *ptr,size_t size,size_t nmemb,void *data)
   {
   size_t realsize=size*nmemb;
   struct MemoryStruct *mem=(struct MemoryStruct *)data;

   mem->memory=(char *)myrealloc(mem->memory,mem->size+realsize+1);

   if (mem->memory)
      {
      memcpy(&(mem->memory[mem->size]),ptr,realsize);
      mem->size+=realsize;
      mem->memory[mem->size]=0;
      }

   return(realsize);
   }

void curlinit(char *proxyname=NULL,char *proxyport=NULL)
   {
   int i;

   /* init curl */
   curl_global_init(CURL_GLOBAL_ALL);

   for (i=0; i<numthreads+2; i++)
      {
      /* init the curl session */
      curl_handle[i]=curl_easy_init();

      /* send all data to this function */
      curl_easy_setopt(curl_handle[i],CURLOPT_WRITEFUNCTION,WriteMemoryCallback);

      /* optionally set the proxy server address */
      if (proxyname!=NULL)
         {
         curl_easy_setopt(curl_handle[i],CURLOPT_PROXY,proxyname);
         if (proxyport!=NULL) curl_easy_setopt(curl_handle[i],CURLOPT_PROXYPORT,proxyport);
         }

      /* some servers don't like requests that are made without a user-agent */
      curl_easy_setopt(curl_handle[i],CURLOPT_USERAGENT,"libMini-agent/1.0");

      if (i<numthreads+1)
         {
         /* request zlib compression */
         curl_easy_setopt(curl_handle[i],CURLOPT_ENCODING,"deflate");
         }
      else
         {
         /* request header only */
         curl_easy_setopt(curl_handle[i],CURLOPT_NOBODY,1);
         }
      }
   }

void curlexit()
   {
   int i;

   /* cleanup curl stuff */
   for (i=0; i<numthreads+2; i++) curl_easy_cleanup(curl_handle[i]);
   }

char *concat(char *str1,char *str2,char *str3)
   {
   char *str;

   str=(char *)malloc(strlen(str1)+strlen(str2)+strlen(str3)+1);

   memcpy(str,str1,strlen(str1));
   memcpy(str+strlen(str1),str2,strlen(str2));
   memcpy(str+strlen(str1)+strlen(str2),str3,strlen(str3)+1);

   return(str);
   }

void geturl(char *src_url,char *src_id,char *src_file,char *dst_file,int background)
   {
   char *url;

   struct MemoryStruct chunk;

   chunk.memory=NULL;
   chunk.size=0;

   url=concat(src_url,src_id,src_file);

   /* we pass our chunk struct to the callback function */
   curl_easy_setopt(curl_handle[background],CURLOPT_WRITEDATA,(void *)&chunk);

   /* specify URL to get */
   curl_easy_setopt(curl_handle[background],CURLOPT_URL,url);

   /* get it! */
   curl_easy_perform(curl_handle[background]);

   /* write to file */
   if (chunk.memory)
      {
      FILE *file;

      if ((file=fopen(dst_file,"wb"))==NULL) exit(1);
      fwrite(chunk.memory,1,chunk.size,file);
      fclose(file);

      free(chunk.memory);
      }

   free(url);
   }

int checkurl(char *src_url,char *src_id,char *src_file)
   {
   char *url;
   long response;

   struct MemoryStruct chunk;

   chunk.memory=NULL;
   chunk.size=0;

   url=concat(src_url,src_id,src_file);

   /* we pass our chunk struct to the callback function */
   curl_easy_setopt(curl_handle[numthreads+1],CURLOPT_WRITEDATA,(void *)&chunk);

   /* specify URL to get */
   curl_easy_setopt(curl_handle[numthreads+1],CURLOPT_URL,url);

   /* get it! */
   curl_easy_perform(curl_handle[numthreads+1]);

   /* query response code */
   curl_easy_getinfo(curl_handle[numthreads+1],CURLINFO_RESPONSE_CODE,&response);

   /* free memory chunk */
   if (chunk.memory) free(chunk.memory);

   free(url);

   return(response==200);
   }

// squish support:

#include <squish.h>

void autocompress(int isrgbadata,unsigned char *rawdata,unsigned int bytes,
                  unsigned char **s3tcdata,unsigned int *s3tcbytes,
                  databuf *obj,void *data)
   {
   int i;

   unsigned char *rgbadata;

   int mode;

   static const int modefast=squish::kDxt1 | squish::kColourRangeFit; // fast but produces artifacts
   static const int modeslow=squish::kDxt1 | squish::kColourClusterFit; // almost no artifacts but slow
   static const int modeslug=squish::kDxt1 | squish::kColourIterativeClusterFit; // no artifacts but very sluggish

   if (*((int *)data)==0) mode=modefast;
   else mode=modeslow;

   if (isrgbadata==0) denoise((unsigned char *)obj->data,obj->xsize,obj->ysize);

   if (isrgbadata==0)
      {
      rgbadata=(unsigned char *)malloc(4*obj->xsize*obj->ysize);
      if (rgbadata==NULL) ERRORMSG();

      for (i=0; i<obj->xsize*obj->ysize; i++)
         {
         rgbadata[4*i]=rawdata[3*i];
         rgbadata[4*i+1]=rawdata[3*i+1];
         rgbadata[4*i+2]=rawdata[3*i+2];
         rgbadata[4*i+3]=255;
         }

      rawdata=rgbadata;
      }

   *s3tcbytes=squish::GetStorageRequirements(obj->xsize,obj->ysize,mode);
   *s3tcdata=(unsigned char *)malloc(*s3tcbytes);
   if (*s3tcdata==NULL) ERRORMSG();

   squish::CompressImage(rawdata,obj->xsize,obj->ysize,*s3tcdata,mode);

   if (isrgbadata==0) free(rawdata);
   }

// main section:

#include "minibase.h"

#include "miniOGL.h"
#include "miniload.hpp"
#include "minicache.hpp"
#include "minisky.hpp"
#include "minitext.hpp"
#include "minipoint.hpp"
#include "minitime.h"

#include "pnmbase.h"
#include "database.hpp"
#include "datacloud.hpp"
#include "datacache.hpp"

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

// maximum path length
#define MAXSTR 1000

// base url and id-path to the tiles and textures
static char baseurl[MAXSTR]="";
static char baseid[MAXSTR]="";

// sub-path to the tiles and textures
static char basepath1[MAXSTR]=""; // elevation
static char basepath2[MAXSTR]=""; // imagery

// number of columns and rows
static int cols=0,rows=0;

// maximum texture size
static int basesize=0;

// window size
static int winwidth,winheight,winid;

// reduced scale
static const float scale=VIEWER_SCALE;

// vertical exaggeration
static const float exaggeration=VIEWER_EXAGGERATION;

// fog and sky color
static const GLfloat fogcolor[]={0.65f,0.7f,0.7f,1.0f};

// horizontal offset
static float offsetx=0.0f,offsety=0.0f;

// geometric output parameters
static float outparams[5];

// approximate value of one arc-second in meters
static float arcsec[2];

// the terrain and its cache
static miniload *terrain;
static minicache *cache;

// the tile cache
static datacache *tilecache;

// the sky dome
static minisky skydome;

// the waypoints
static minipoint points;

// frames per second
static float fps=VIEWER_FPS;
static int fpu=VIEWER_FPU;
static int upd=fpu;

// global resolution
static float res=VIEWER_RES;

// texture range
static float range=VIEWER_RANGE;

// view frustum
static float fovy=VIEWER_FOVY,nearp=VIEWER_NEARP,farp=VIEWER_FARP;

// view point and direction
static float ex,ey,ez,ay,angle,turn,pitch,incline;

// stereo base
static const float sbase=VIEWER_SBASE;

// gliding parameters
static float maxspeed=VIEWER_MAXSPEED,speedinc=0.1f,accel=0.1f,gravity=0.0f,hoover=VIEWER_HOOVER,jump=VIEWER_JUMP,damping=200.0f;

// steering parameters
static const float oneturn=5.0f,oneincline=10.0f;

// gliding speed
static float speed,topspeed;

// wire frame flag
static int wireframe=0;

// consumed time per frame
static double accu_delta=0.0;
static double accu_idle=0.0;
static double avg_delta=1.0/fps;
static double avg_idle=0.0;
static int avg_count=0;

// command line switches
static int sw_stereo=0;
static int sw_anaglyph=0;
static int sw_full=0;
static int sw_hud=0;
static int sw_pnm=0;
static int sw_dropwp=0;
static int sw_s3tcslow=0;
static int sw_bricks=0;
static int sw_4pass=0;

// height and range of waypoint signposts
static const float signpostheight=VIEWER_SIGNPOSTHEIGHT;
static const float signpostrange=VIEWER_SIGNPOSTRANGE;

// waypoint brick size
static const float bricksize=VIEWER_BRICKSIZE;
static const float brickrad=VIEWER_BRICKRAD;

// initialize the point of view
void initview(float x,float y,float e,float a,float p,float h)
   {
   float te;

   ex=(x-offsetx)*arcsec[0];
   ez=(y-offsety)*arcsec[1];

   te=terrain->getheight(ex,-ez);

   if (e==0.0f && te!=-MAXFLOAT) ey=te+hoover+h;
   else ey=e;

   terrain->restrictroi(ex,-ez,VIEWER_LOAD*farp/scale);

   terrain->updateroi(res,
                      ex,ey+1000*farp/scale,-ez,
                      ex,-ez,farp/scale);

   ay=0.0f;

   angle=turn=a;
   pitch=incline=p;

   speed=topspeed=0.0f;

   upd=1;
   }

// load settings
void loadsettings()
   {
   FILE *file;

   char *savname=tilecache->getfile(VIEWER_SAVFILE);

   if (savname==NULL) savname=strdup(VIEWER_SAVFILE);

   if ((file=fopen(savname,"rb"))!=NULL)
      {
      if (fscanf(file,"ex=%f\n",&ex)!=1) ERRORMSG();
      if (fscanf(file,"ey=%f\n",&ey)!=1) ERRORMSG();
      if (fscanf(file,"ez=%f\n",&ez)!=1) ERRORMSG();

      if (fscanf(file,"angle=%f\n",&angle)!=1) ERRORMSG();
      if (fscanf(file,"pitch=%f\n",&pitch)!=1) ERRORMSG();

      if (fscanf(file,"farp=%f\n",&farp)!=1) ERRORMSG();
      if (fscanf(file,"res=%f\n",&res)!=1) ERRORMSG();
      if (fscanf(file,"range=%f\n",&range)!=1) ERRORMSG();

      fclose(file);

      initview(ex/arcsec[0]+offsetx,ez/arcsec[1]+offsety,ey,angle,pitch,farp/scale/10);

      tilecache->getcloud()->getterrain()->setpreload(VIEWER_PRELOAD*farp/scale,VIEWER_UPDATE);

      tilecache->getcloud()->getterrain()->setrange(range*farp/scale);
      tilecache->getcloud()->getterrain()->setradius(VIEWER_RADIUS*range*farp/scale,VIEWER_DROPOFF);
      }

   free(savname);
   }

// save settings
void savesettings()
   {
   FILE *file;

   char *savname=tilecache->getfile(VIEWER_SAVFILE);

   if (savname==NULL) savname=strdup(VIEWER_SAVFILE);

   if ((file=fopen(savname,"wb"))==NULL) ERRORMSG();
   free(savname);

   fprintf(file,"ex=%f\n",ex);
   fprintf(file,"ey=%f\n",ey);
   fprintf(file,"ez=%f\n",ez);

   fprintf(file,"angle=%f\n",angle);
   fprintf(file,"pitch=%f\n",pitch);

   fprintf(file,"farp=%f\n",farp);
   fprintf(file,"res=%f\n",res);
   fprintf(file,"range=%f\n",range);

   fclose(file);
   }

// load requested data
void request_callback(char *file,int istexture,databuf *buf,void *data)
   {
   if (sw_pnm==0) buf->loaddata(file);
   else buf->loadPNMdata(file);
   }

// http receiver
void receive_callback(char *src_url,char *src_id,char *src_file,char *dst_file,int background,void *data)
   {geturl(src_url,src_id,src_file,dst_file,background);}

// http checker
int check_callback(char *src_url,char *src_id,char *src_file,void *data)
   {return(checkurl(src_url,src_id,src_file));}

// render tile info
void renderinfo()
   {
   int i,j,t;

   const float sx=0.6f;
   const float sy=0.02f;

   const float xr=0.38f;
   const float yr=0.38f;

   const float size=0.3f;
   const float alpha=0.5f;

   minitile *mt=terrain->getminitile();

   float vcol=mt->getvisibleleft();
   float vrow=mt->getvisiblebottom();

   float vcols=mt->getvisibleright()-mt->getvisibleleft()+1;
   float vrows=mt->getvisibletop()-mt->getvisiblebottom()+1;

   float pcol=mt->getpreloadedleft();
   float prow=mt->getpreloadedbottom();

   float pcols=mt->getpreloadedright()-mt->getpreloadedleft()+1;
   float prows=mt->getpreloadedtop()-mt->getpreloadedbottom()+1;

   glLoadIdentity();
   glTranslatef(sx,sy+yr,0.0f);
   glScalef(xr/pcols,-yr/prows,0.0f);
   glTranslatef(0.5f,0.5f,0.0f);

   glColor4f(1.0f,1.0f,1.0f,0.25f);

   glBegin(GL_QUADS);
   glVertex2f(-size,-size);
   glVertex2f(pcols-1+size,-size);
   glVertex2f(pcols-1+size,prows-1+size);
   glVertex2f(-size,prows-1+size);
   glEnd();

   glColor4f(1.0f,1.0f,1.0f,0.75f);

   glBegin(GL_QUADS);
   glVertex2f(vcol-pcol-size,vrow-prow-size);
   glVertex2f(vcol-pcol+vcols-1+size,vrow-prow-size);
   glVertex2f(vcol-pcol+vcols-1+size,vrow-prow+vrows-1+size);
   glVertex2f(vcol-pcol-size,vrow-prow+vrows-1+size);
   glEnd();

   glBegin(GL_QUADS);

   for (i=0; i<pcols; i++)
      for (j=0; j<prows; j++)
         {
         t=mt->gettexw(pcol+i,prow+j);

         switch (t)
            {
            case 0:
            case 2:
            case 4:
            case 8:
            case 16:
            case 32: glColor4f(1.0f,1.0f,1.0f,alpha); break;
            case 64: glColor4f(0.0f,0.0f,1.0f,alpha); break;
            case 128: glColor4f(0.0f,0.5f,1.0f,alpha); break;
            case 256: glColor4f(0.0f,1.0f,0.0f,alpha); break;
            case 512: glColor4f(0.5f,1.0f,0.0f,alpha); break;
            case 1024: glColor4f(1.0f,1.0f,0.0f,alpha); break;
            case 2048: glColor4f(1.0f,0.5f,0.0f,alpha); break;
            default: glColor4f(1.0f,0.0f,0.0f,alpha); break;
            }

         glVertex2f(i-size,j-size);
         glVertex2f(i+size,j-size);
         glVertex2f(i+size,j+size);
         glVertex2f(i-size,j+size);
         }

   glEnd();
   }

// render head up display
void renderhud()
   {
   float te;

   char str[MAXSTR];

   datacloud *cloud=tilecache->getcloud();

   minitext::configure_zfight(1.0f);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);

   if (sw_hud)
      {
      glTranslatef(0.0f,0.7f,0.0f);

      glColor4f(1.0f,1.0f,1.0f,0.5f);
      glBegin(GL_QUADS);
      glVertex2f(0.0f,0.0f);
      glVertex2f(1.0f,0.0f);
      glVertex2f(1.0f,0.275f);
      glVertex2f(0.0f,0.275f);
      glEnd();

      glTranslatef(0.033f,0.0f,0.0f);

      te=terrain->getheight(ex,-ez);
      if (te==-MAXFLOAT) te=0.0f;

      snprintf(str,MAXSTR,"Position:                \n\n x= %11.1fm\n y= %11.1fm (%.1fm)\n z= %11.1fm\n\n dir= %.1f\n yon= %.1f\n\nSettings:\n\n farp= %.1fm (f/F)\n\n res=   %.1f (t/T)\n range= %.1fm (r/R)\n\n gravity= %.1f (g)\n",
               ex*scale+offsetx*arcsec[0]*scale,ey*scale,te*scale/exaggeration,ez*scale+offsety*arcsec[1]*scale,turn,incline, // position
               farp,res,range*farp,gravity); // adjustable parameters

      minitext::drawstring(0.3f,240.0f,1.0f,0.25f,1.0f,str);

      glTranslatef(0.3f,0.0f,0.0f);

      snprintf(str,MAXSTR,"Tile Set:                \n\n area= [%d-%d]x[%d-%d]\n fps=  %.1fHz (%.1f%%)\n\n mem=    %.1fMB\n tex= %.1fMB (%.1fMB)\n\nStreaming:\n\n pending= %d\n cache=   %.1fMB\n\nGeometry:\n\n fans=     %d\n vertices= %d\n",
               terrain->getminitile()->getvisibleleft(),
               terrain->getminitile()->getvisibleright(),
               terrain->getminitile()->getvisiblebottom(),
               terrain->getminitile()->getvisibletop(),
               1.0/(avg_delta+avg_idle),100*(1.0-avg_idle*fps), // frame rate
               terrain->getmem(),terrain->gettexmem()/6,terrain->gettexmem(), // memory
               cloud->getpending(), // number of total pending tiles
               cloud->getmem(), // total memory foot print
               cache->getfancnt(), // triangles fans
               cache->getvtxcnt()); // vertices

      minitext::drawstring(0.3f,240.0f,1.0f,0.25f,1.0f,str);

      glTranslatef(0.3f,0.0f,0.0f);

      snprintf(str,MAXSTR,"Controls:                \n\n left=  a\n right= d\n accelerate= w\n decelerate= s\n\n stop= space\n jump= j\n look down= <\n look up=   >\n\n hud=       h\n waypoints= p\n wireframe= l\n\n quit= q esc\n");

      minitext::drawstring(0.3f,240.0f,1.0f,0.25f,1.0f,str);

      if (incline>89.9f)
         {
         glLoadIdentity();
         glTranslatef(0.5f,0.5f,0.0f);
         glScalef((float)winheight/winwidth,1.0f,1.0f);
         glColor3f(0.0f,0.0f,0.0f);
         glBegin(GL_LINES);
         glVertex2f(-0.025,-0.025f);
         glVertex2f(0.025,0.025f);
         glVertex2f(-0.025,0.025f);
         glVertex2f(0.025,-0.025f);
         glEnd();
         }

      renderinfo();
      }
   else
      {
      glTranslatef(0.033f,0.95f,0.0f);
      minitext::drawstring(0.3f,240.0f,1.0f,0.25f,1.0f,"[Press h for HUD]",1.0f,0.25f);
      }

   glDisable(GL_BLEND);
   }

// draw waypoints
void drawpoints()
   {
   if (sw_dropwp==0)
      if (sw_bricks==0)
         points.drawsignposts(ex,ey,ez,signpostheight/scale,signpostrange*farp/scale,turn,-incline);
      else
         points.drawbricks(ex,ey,ez,brickrad/scale,farp/scale,fovy,(float)winwidth/winheight,bricksize/scale);
   }

// GLUT display function
void displayfunc()
   {
   double time,delta,idle;

   float sina,cosa;
   float sinp,cosp;

   float elev,coef;
   float rx,rz;

   // remember start of actual frame
   time=minitime();

   // update point of view:

   sina=sin(2.0f*PI/360.0f*turn);
   cosa=cos(2.0f*PI/360.0f*turn);

   sinp=sin(2.0f*PI/360.0f*incline);
   cosp=cos(2.0f*PI/360.0f*incline);

   ex+=sina*speed/fps;
   ez+=cosa*speed/fps;

   speed+=accel*(topspeed-speed);

   elev=terrain->getheight(ex,-ez);

   coef=(ey-elev)/hoover-1.0f;
   if (coef>1.0f) coef=1.0f;
   else if (coef<-1.0f) coef=-1.0f;

   ay-=coef*gravity/fps;
   ay*=pow(1.0f/damping,1.0f/fps);
   ey+=ay/fps;

   if (ey<elev+hoover)
      {
      ay=(elev+hoover-ey)*fps;
      ey=elev+hoover;
      }

   turn+=accel*(angle-turn);

   incline+=accel*(pitch-incline);

   // setup OpenGL state:

   glClearColor(fogcolor[0],fogcolor[1],fogcolor[2],1.0f);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   glClearColor(fogcolor[0],fogcolor[1],fogcolor[2],fogcolor[3]);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   glFogfv(GL_FOG_COLOR,fogcolor);

   glFogi(GL_FOG_MODE,GL_LINEAR);
   glFogf(GL_FOG_START,0.5f*farp/scale);
   glFogf(GL_FOG_END,farp/scale);

   glEnable(GL_FOG);

   // render scene:

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(fovy,(float)winwidth/winheight,nearp/scale,farp/scale);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(ex,ey,-ez,ex+sina*cosp,ey-sinp,-ez-cosa*cosp,sina*sinp,cosp,-cosa*sinp);

   // update vertex arrays
   terrain->draw(res,
                 ex,ey,-ez,
                 sina*cosp,-sinp,-cosa*cosp,
                 sina*sinp,cosp,-cosa*sinp,
                 fovy,(float)winwidth/winheight,
                 nearp/scale,farp/scale,
                 upd);

   upd=fpu;

   skydome.setpos(ex,0.0f,-ez,
                  2.0f*farp/scale,0.5f);

   if (sw_stereo==0)
      {
      // draw sky dome
      skydome.drawskydome();

      // render vertex arrays
      cache->rendercache();

      // draw waypoints
      drawpoints();
      }
   else
      {
      rx=cosa*sbase;
      rz=-sina*sbase;

      // left channel:

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(ex-rx,ey,-ez+rz,ex+sina*cosp-rx,ey-sinp,-ez-cosa*cosp+rz,sina*sinp,cosp,-cosa*sinp);

      if (sw_anaglyph==0) glDrawBuffer(GL_BACK_LEFT);
      else glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_FALSE);

      skydome.drawskydome();

      cache->rendercache();

      drawpoints();

      // right channel:

      glClear(GL_DEPTH_BUFFER_BIT);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(ex+rx,ey,-ez-rz,ex+sina*cosp+rx,ey-sinp,-ez-cosa*cosp-rz,sina*sinp,cosp,-cosa*sinp);

      if (sw_anaglyph==0) glDrawBuffer(GL_BACK_RIGHT);
      else glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_FALSE);

      skydome.drawskydome();

      cache->rendercache();

      drawpoints();

      if (sw_anaglyph==0) glDrawBuffer(GL_BACK);
      else glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_FALSE);
      }

   renderhud();

   // swap buffers and wait for next frame:

   glutSwapBuffers();

   delta=minitime()-time;

   points.configure_brickstripes(time-(int)time);

   idle=1.0f/fps-delta;
   if (idle>0.0) waitfor(idle);

   accu_delta+=delta;
   if (idle>0.0) accu_idle+=idle;

   if (++avg_count>fps)
      {
      avg_delta=accu_delta/avg_count;
      avg_idle=accu_idle/avg_count;
      accu_delta=0.0;
      accu_idle=0.0f;
      avg_count=0;
      }
   }

// GLUT reshape function
void reshapefunc(int width,int height)
   {
   if (sw_full==0)
      {
      winwidth=width;
      winheight=height;
      }
   else
      {
      winwidth=glutGameModeGet(GLUT_GAME_MODE_WIDTH);
      winheight=glutGameModeGet(GLUT_GAME_MODE_HEIGHT);

      fps=glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE);

      if (winwidth<=0 || winheight<=0 || fps<=0.0f)
         {
         winwidth=width;
         winheight=height;

         fps=VIEWER_FPS;
         }
      }

   if (winwidth<1) winwidth=1;
   if (winheight<1) winheight=1;

   glViewport(0,0,width,height);
   }

// GLUT keyboard function
void keyboardfunc(unsigned char key,int x,int y)
   {
   if (key==' ')
      if (topspeed==0.0f) topspeed=speedinc*maxspeed/scale;
      else topspeed=0.0f;
   else if (key=='w' || key=='W')
      {
      topspeed+=speedinc*maxspeed/scale;
      if (topspeed>maxspeed/scale) topspeed=maxspeed/scale;
      }
   else if (key=='a' || key=='A')
      {
      angle-=oneturn;

      if (angle<0.0f)
         {
         angle+=360.0f;
         turn+=360.0f;
         }
      }
   else if (key=='d' || key=='D')
      {
      angle+=oneturn;

      if (angle>360.0f)
         {
         angle-=360.0f;
         turn-=360.0f;
         }
      }
   else if (key=='s' || key=='S')
      {
      topspeed-=speedinc*maxspeed/scale;
      if (topspeed<-maxspeed/scale) topspeed=-maxspeed/scale;
      }
   else if (key=='<')
      {
      pitch+=oneincline;
      if (pitch>90.0f) pitch=90.0f;
      }
   else if (key=='>')
      {
      pitch-=oneincline;
      if (pitch<-90.0f) pitch=-90.0f;
      }
   else if (key=='j' || key=='J') ay=jump*fps;
   else if (key=='t')
      {
      res/=1.1f;
      if (res<1.0f) res=1.0f;
      }
   else if (key=='T')
      {
      res*=1.1f;
      if (res>1.0E10f) res=1.0E10f;
      }
   else if (key=='r')
      {
      range/=1.1f;
      if (range<1.0E-5f) range=1.0E-5f;

      tilecache->getcloud()->getterrain()->setrange(range*farp/scale);
      tilecache->getcloud()->getterrain()->setradius(VIEWER_RADIUS*range*farp/scale,VIEWER_DROPOFF);
      }
   else if (key=='R')
      {
      range*=1.1f;
      if (range>1.0f) range=1.0f;

      tilecache->getcloud()->getterrain()->setrange(range*farp/scale);
      tilecache->getcloud()->getterrain()->setradius(VIEWER_RADIUS*range*farp/scale,VIEWER_DROPOFF);
      }
   else if (key=='f')
      {
      farp/=1.1f;
      if (farp<2*nearp) farp=2*nearp;

      tilecache->getcloud()->getterrain()->setpreload(VIEWER_PRELOAD*farp/scale,VIEWER_UPDATE);

      tilecache->getcloud()->getterrain()->setrange(range*farp/scale);
      tilecache->getcloud()->getterrain()->setradius(VIEWER_RADIUS*range*farp/scale,VIEWER_DROPOFF);
      }
   else if (key=='F')
      {
      farp*=1.1f;
      if (farp>1.0E10f*nearp) farp=1.0E10f*nearp;

      tilecache->getcloud()->getterrain()->setpreload(VIEWER_PRELOAD*farp/scale,VIEWER_UPDATE);

      tilecache->getcloud()->getterrain()->setrange(range*farp/scale);
      tilecache->getcloud()->getterrain()->setradius(VIEWER_RADIUS*range*farp/scale,VIEWER_DROPOFF);
      }
   else if (key=='g' || key=='G')
      if (gravity==0.0f) gravity=VIEWER_GRAVITY;
      else gravity=0.0f;
   else if (key=='h' || key=='H')
      if (sw_hud==0) sw_hud=1;
      else sw_hud=0;
   else if (key=='l')
      {
      wireframe=1-wireframe;
      if (wireframe!=0) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      }
   else if (key=='p') sw_dropwp=1-sw_dropwp;
   else if (key=='o') savesettings();
   else if (key=='O') loadsettings();
   else if (key=='q' || key==27)
      {
      delete tilecache;
      delete terrain;

      threadexit();
      curlexit();

      if (sw_full==0) glutDestroyWindow(winid);
      else glutLeaveGameMode();

      exit(0);
      }
   }

// main function
int main(int argc,char *argv[])
   {
   int i;

   // check arguments
   if (argc<5)
      {
      printf("usage: %s <url> <tileset.path> <elevation.subpath> <imagery.subpath> [<columns> <rows> [<max.texture.size>]] {-s | -a | -p}\n",argv[0]);
      exit(1);
      }

   // path setup for elevation and imagery tile set
   if (sscanf(argv[1],"%s",baseurl)!=1) exit(1);
   if (sscanf(argv[2],"%s",baseid)!=1) exit(1);
   if (sscanf(argv[3],"%s",basepath1)!=1) exit(1);
   if (sscanf(argv[4],"%s",basepath2)!=1) exit(1);

   // read columns and rows of the tile set
   if (argc>6)
      {
      if (sscanf(argv[5],"%d",&cols)!=1) exit(1);
      if (sscanf(argv[6],"%d",&rows)!=1) exit(1);
      }

   // optionally read maximum texture size
   if (argc>7) if (sscanf(argv[7],"%d",&basesize)!=1) basesize=0;

   // process command line options
   for (i=5; i<argc; i++)
      if (strcmp(argv[i],"-s")==0) sw_stereo=1;
      else if (strcmp(argv[i],"-a")==0) sw_anaglyph=1;
      else if (strcmp(argv[i],"-f")==0) sw_full=1;
      else if (strcmp(argv[i],"-h")==0) sw_hud=1;
      else if (strcmp(argv[i],"-p")==0) sw_pnm=1;
      else if (strcmp(argv[i],"-d")==0) sw_dropwp=1;
      else if (strcmp(argv[i],"-c")==0) sw_s3tcslow=1;
      else if (strcmp(argv[i],"-b")==0) sw_bricks=1;
      else if (strcmp(argv[i],"-B")==0) sw_bricks=sw_4pass=1;

   // open window with GLUT:

   winwidth=VIEWER_WINWIDTH;
   winheight=VIEWER_WINHEIGHT;

   glutInit(&argc,argv);
   glutInitWindowSize(winwidth,winheight);
   if (sw_stereo!=0 && sw_anaglyph==0) glutInitDisplayMode(GLUT_RGB|GLUT_ALPHA|GLUT_DEPTH|GLUT_DOUBLE|GLUT_STEREO);
   else glutInitDisplayMode(GLUT_RGB|GLUT_ALPHA|GLUT_DEPTH|GLUT_DOUBLE);
   if (sw_full==0) winid=glutCreateWindow(VIEWER_WINTITLE);
   else glutEnterGameMode();

   glutDisplayFunc(displayfunc);
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(NULL);
   glutMotionFunc(NULL);
   glutKeyboardFunc(keyboardfunc);
   glutSpecialFunc(NULL);
   glutIdleFunc(displayfunc);

   // setup libMini:

   // turn off on-the-fly texture compression
   miniOGL::configure_compression(0);

   // register autocompression hook
   databuf::setautocompress(autocompress,&sw_s3tcslow);

   // create terrain and its cache
   terrain=new miniload;
   cache=new minicache;

   // attach tile cache
   tilecache=new datacache(terrain);
   tilecache->settilesetfile(VIEWER_TILESETFILE);
   tilecache->setvtbinifile(VIEWER_VTBINIFILE);
   tilecache->setstartupfile(VIEWER_STARTUPFILE);
   tilecache->setloader(request_callback,NULL,1,VIEWER_PRELOAD*farp/scale,range*farp/scale,basesize,VIEWER_LAZYNESS,VIEWER_UPDATE,VIEWER_EXPIRE);
   tilecache->getcloud()->getterrain()->setradius(VIEWER_RADIUS*range*farp/scale,VIEWER_DROPOFF);
   tilecache->getcloud()->setschedule(VIEWER_UPLOAD,VIEWER_KEEP);
   tilecache->getcloud()->setmaxsize(VIEWER_CACHE);
   tilecache->getcloud()->setthread(startthread,NULL,jointhread,lock_cs,unlock_cs,lock_io,unlock_io);
   tilecache->configure_netthreads(numthreads);
   tilecache->setremoteid(baseid);
   tilecache->setremoteurl(baseurl);
   tilecache->setlocalpath(VIEWER_LOCALPATH);
   tilecache->setreceiver(receive_callback,NULL,check_callback);
   threadinit();
   curlinit();

   // load persistent startup file
   tilecache->load();

   // check tileset info
   if (tilecache->hasinfo())
      {
      cols=tilecache->getinfo_tilesx();
      rows=tilecache->getinfo_tilesy();

      offsetx=tilecache->getinfo_centerx();
      offsety=tilecache->getinfo_centery();

      basesize=tilecache->getinfo_maxtexsize();

      tilecache->getcloud()->getterrain()->setbasesize(basesize);

      sw_pnm=1;
      }
   else if (tilecache->hasini())
      {
      cols=tilecache->getini_tilesx();
      rows=tilecache->getini_tilesy();

      offsetx=tilecache->getini_centerx();
      offsety=tilecache->getini_centery();

      basesize=tilecache->getini_maxtexsize();

      tilecache->getcloud()->getterrain()->setbasesize(basesize);

      sw_pnm=0;
      }

   // use .db file numbering starting with zero for compatibility with vtp
   if (sw_pnm==0) terrain->configure_usezeronumbering(1);

   // turn off mip-mapping
   terrain->configure_mipmaps(0);

   // load tiles
   terrain->load(cols,rows, // number of columns and rows
                 basepath1,basepath2,NULL, // directories for tiles and textures (and no fogmaps)
                 -offsety,-offsetx, // horizontal offset
                 exaggeration,scale, // vertical exaggeration and global scale
                 0.0f,0.0f, // no fog parameters required
                 0.0f, // choose default minimum resolution
                 0.0f, // disable base offset safety
                 outparams, // geometric output parameters
                 (sw_pnm==0)?NULL:arcsec);

   // check if tileset is defined in meters or arc-seconds
   if (sw_pnm==0) arcsec[0]=arcsec[1]=1.0f/scale;

   // define resolution reduction of invisible tiles
   terrain->getminitile()->setreduction(2.0f,3.0f);

   // use tile caching with vertex arrays
   cache->setcallbacks(terrain->getminitile(), // the minitile object to be cached
                       cols,rows, // number of tile columns and rows
                       outparams[0],outparams[1], // tile extents
                       outparams[2],0.0f,-outparams[3]); // origin with negative Z

   // load sky dome:

   char *skyname=tilecache->getfile("SkyDome.ppm");

   if (skyname!=NULL)
      {
      skydome.loadskydome(skyname,
                          outparams[2],0.0f,-outparams[3],
                          cols*outparams[0],rows*outparams[1]/(cols*outparams[0]));

      free(skyname);
      }

   // load waypoints:

   char *wpname=tilecache->getfile("Waypoints.txt");

   minipointdata *first=NULL;

   if (wpname!=NULL)
      {
      if (sw_pnm==0) points.configure_automap(1);

      points.load(wpname,-offsety,-offsetx,arcsec[0],arcsec[1],exaggeration/scale,terrain->getminitile());
      free(wpname);

      first=points.getfirst();

      points.configure_brickceiling(3.0f*first->elev*scale);
      if (sw_4pass==0) points.configure_brickpasses(1);
      else points.configure_brickpasses(4);
      }

   // initialize view point
   if (first==NULL) initview(outparams[2]/arcsec[0]+offsetx,outparams[3]/arcsec[1]+offsety,0.0f,0.0f,fovy/2,farp/scale/10);
   else initview(first->x/arcsec[0]+offsetx,first->y/arcsec[1]+offsety,0.0f,0.0f,fovy/2,farp/scale/10);

   // load settings
   loadsettings();

   // enter event loop
   glutMainLoop();

   // never reached
   return(0);
   }
