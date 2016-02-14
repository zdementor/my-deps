// (c) by Stefan Roettger

#ifndef MINICACHE_H
#define MINICACHE_H

#include "miniOGL.h"

#include "minitile.hpp"

class minicache
   {
   public:

   //! default constructor
   minicache();

   //! destructor
   ~minicache();

   //! set callbacks for scene double buffering
   void setcallbacks(minitile *terrain,
                     int cols,int rows,
                     float xdim,float zdim,
                     float centerx,float centery,float centerz,
                     void (*notify)(int i,int j,int s)=0,
                     void (*texmap)(int m,int n,int S,int texid,int texw,int texh,int texmm)=0,
                     void (*prismedge)(float x,float y,float yf,float z)=0,
                     void (*prismcache)(int phase,float scale,float ex,float ey,float ez)=0,
                     int (*prismrender)(float *cache,int cnt,float lambda)=0,
                     int (*trigger)(int phase)=0);

   //! render back buffer of the cache
   int rendercache();

   //! define triangle mesh opacity
   void setopacity(float alpha=1.0f);

   //! define optional fog rendering mode
   void setprismmode(int prismmode=1,float prismbase=0.5f,
                     float prismR=1.0f,float prismG=1.0f,float prismB=1.0f,float prismA=0.9f);

   //! define optional vertex shader
   void setvtxshader(char *vtxprog=0);
   void setvtxshaderparams(float p1=0.0f,float p2=0.0f,float p3=0.0f,float p4=0.0f,int n=0);
   void usevtxshader(int on=1) {USEVTXSHADER=on;}

   //! define optional pixel shader
   void setpixshader(char *fragprog=0);
   void setpixshaderparams(float p1=0.0f,float p2=0.0f,float p3=0.0f,float p4=0.0f,int n=0);
   void setpixshadertexRGB(unsigned char *image,int width,int height);
   void usepixshader(int on=1) {USEPIXSHADER=on;}

   //! define optional shader callback
   void setshadercb(void (*initstatecb)()=0,void (*exitstatecb)()=0);

   //! get number of rendered primitives
   int getfancnt();
   int getvtxcnt();

   //! configuring
   void configure_overlap(float overlap=0.02f); // overlap of tile borders (prevents pixel flickering)
   void configure_minsize(int minsize=33); // disable overlap for mesh size less than minsize
   void configure_zfight(float zscale=0.95f); // shift scene towards the viewer to avoid Z-fighting

   protected:

   static minicache *CACHE;

   static minitile *TERRAIN;

   static void cache_beginfan();
   static void cache_fanvertex(float i,float y,float j);
   static void cache_notify(int i,int j,int s);
   static void cache_texmap(int m,int n,int S);
   static void cache_prismedge(float x,float y,float yf,float z);
   static void cache_trigger(int phase,float scale,float ex,float ey,float ez);

   void cache(int op,float a=0.0f,float b=0.0f,float c=0.0f);
   void cacheprismedge(float x,float y,float yf,float z);
   void cachetrigger(int phase,float scale,float ex,float ey,float ez);

   void rendertexmap(int m,int n,int S);
   int rendertrigger(int phase,float scale);

   int COLS,ROWS;
   float XDIM,ZDIM;
   float CENTERX,CENTERY,CENTERZ;
   float SCALE;

   int CACHE_NUM;

   unsigned char *CACHE1_OP,*CACHE2_OP;
   float *CACHE1_ARG,*CACHE2_ARG;

   int CACHE_SIZE1,CACHE_SIZE2;
   int CACHE_MAXSIZE;

   int FANCNT1,FANCNT2;
   int VTXCNT1,VTXCNT2;

   int LAST_BEGINFAN;

   float LAMBDA;

   float *PRISM_CACHE1,*PRISM_CACHE2;

   int PRISM_SIZE1,PRISM_SIZE2;
   int PRISM_MAXSIZE;

   char *VTXPROG;
   int VTXDIRTY;

   int VTXPROGID;
   int USEVTXSHADER;

   float VTXSHADERPAR1[8];
   float VTXSHADERPAR2[8];
   float VTXSHADERPAR3[8];
   float VTXSHADERPAR4[8];

   char *FRAGPROG;
   int FRAGDIRTY;

   int FRAGPROGID;
   int USEPIXSHADER;

   float PIXSHADERPAR1[8];
   float PIXSHADERPAR2[8];
   float PIXSHADERPAR3[8];
   float PIXSHADERPAR4[8];

   int PIXSHADERTEXID;
   int PIXSHADERTEXWIDTH;
   int PIXSHADERTEXHEIGHT;

   void (*SHADERINITSTATE_CB)();
   void (*SHADEREXITSTATE_CB)();

   void (*NOTIFY_CALLBACK)(int i,int j,int s);
   void (*TEXMAP_CALLBACK)(int m,int n,int S,int texid,int texw,int texh,int texmm);
   void (*PRISMEDGE_CALLBACK)(float x,float y,float yf,float z);
   void (*PRISMCACHE_CALLBACK)(int phase,float scale,float ex,float ey,float ez);
   int (*PRISMRENDER_CALLBACK)(float *cache,int cnt,float lambda);
   int (*TRIGGER_CALLBACK)(int phase);

   private:

   float OPACITY;

   int PRISM_MODE;
   float PRISM_BASE;
   float PRISM_R,PRISM_G,PRISM_B,PRISM_A;

   float CONFIGURE_OVERLAP;
   int CONFIGURE_MINSIZE;
   float CONFIGURE_ZSCALE;

   int renderprisms(float *cache,int cnt,float lambda,
                    int mode=1,float base=0.5f,float pr=1.0f,float pg=1.0f,float pb=1.0f,float pa=0.9f);

   void enablevtxshader(float s1,float s2,float o1,float o2,float scale);
   void disablevtxshader();

   void enablepixshader();
   void disablepixshader();

   // OpenGL extensions:

   int GLSETUP;

   int GLEXT_MT;
   int GLEXT_VP;
   int GLEXT_FP;

   void initglexts();

   // Windows OpenGL extensions:

   int WGLSETUP;

#ifndef NOOGL

#ifdef _WIN32

#ifdef GL_ARB_multitexture
   PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
#endif

#if defined(GL_ARB_vertex_program) && defined(GL_ARB_fragment_program)
   PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
   PFNGLBINDPROGRAMARBPROC glBindProgramARB;
   PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
   PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB;
   PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
#endif

#endif

#endif

   void initwglprocs();
   };

#endif
