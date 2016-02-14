// (c) by Stefan Roettger

#ifndef MINIBRICK_H
#define MINIBRICK_H

#include "miniOGL.h"

#include "database.hpp"

#define MINIBRICK_ONE_RENDER_PHASE 0
#define MINIBRICK_FIRST_RENDER_PHASE 1
#define MINIBRICK_SECOND_RENDER_PHASE 2
#define MINIBRICK_THIRD_RENDER_PHASE 3
#define MINIBRICK_LAST_RENDER_PHASE 4

class mininorm
   {
   public:

   //! default constructor
   mininorm() {}

   //! copy constructor
   mininorm(const mininorm &v) {x=v.x; y=v.y; z=v.z;}

   //! constructors
   mininorm(const float vx,const float vy,const float vz) {x=vx; y=vy; z=vz;}
   mininorm(const unsigned char *ptr) {x=ptr[0]-127.5f; y=ptr[1]-127.5f; z=ptr[2]-127.5f;}

   //! destructor
   ~mininorm() {}

   float x,y,z;

   protected:

   private:
   };

inline mininorm operator + (const mininorm &a,const mininorm &b)
   {return(mininorm(a.x+b.x,a.y+b.y,a.z+b.z));}

inline mininorm operator * (const float a,const mininorm &b)
   {return(mininorm(a*b.x,a*b.y,a*b.z));}

class minivtxarray
   {
   public:

   //! default constructor
   minivtxarray();

   //! destructor
   ~minivtxarray();

   //! release memory
   void release();

   //! reset array
   void reset();

   //! add one vertex to the array (including normals)
   void addvtx(const float x,const float y,const float z,
               const float nx,const float ny,const float nz);

   //! get array size (number of vertices)
   int getsize() {return(SIZE);}

   //! get vertex array
   float *getarray() {return(ARRAY);}

   protected:

   int SIZE;
   float *ARRAY;

   private:

   int MAXSIZE;
   };

class minispect
   {
   public:

   //! default constructor
   minispect();

   //! destructor
   ~minispect();

   //! determine range of iso values
   void setrange(float minval,float maxval);

   //! add iso value
   void addiso(float iso,float R,float G,float B,float A);

   //! delete iso value
   void deliso(float iso);

   //! reset iso spectrum
   void reset();

   //! determine size of cullmap
   void setcullmapsize(int size=256);

   //! get actual size of cullmap
   int getcullmapsize() {return(CSIZE);}

   //! get cull map
   unsigned char *getcullmap();

   //! evaluate cull map
   int isvisible(float minval,float maxval);

   //! get number of iso values
   int getnum() {return(N);}

   //! get number of opaque iso surfaces
   int getopaqnum();

   //! get number of semi-transparent iso surfaces
   int gettransnum();

   //! get iso value
   float getval(int n) {return(ISO[n]);}

   //! get color and opacity
   float getR(int n) {return(RGBA[4*n]);}
   float getG(int n) {return(RGBA[4*n+1]);}
   float getB(int n) {return(RGBA[4*n+2]);}
   float getA(int n) {return(RGBA[4*n+3]);}

   //! swap vertex array for writing
   void swapwritebuf();

   //! swap vertex array for reading
   void swapreadbuf();

   //! release vertex arrays
   void releasebufs();

   //! get vertex array for writing
   minivtxarray *getwritebuf(int n);

   //! get vertex array for reading
   minivtxarray *getreadbuf(int n);

   protected:

   float MINVAL,MAXVAL;

   int N,NMAX;
   float *ISO;
   float *RGBA;

   private:

   int CSIZE;
   unsigned char *CULLMAP;

   int UPDATED;

   int WRITEBUF,READBUF;

   minivtxarray *VTX1;
   minivtxarray *VTX2;
   };

class minisurf
   {
   public:

   //! default constructor
   minisurf();

   //! destructor
   ~minisurf();

   //! get applied iso spectrum
   minispect *getspectrum() {return(&SPECTRUM);}

   //! reset surface
   void reset();

   //! set stripe pattern
   void setstripewidth(float stripewidth) {STRIPEWIDTH=stripewidth;}
   void setstripedir(float dx,float dy,float dz) {STRIPEDX=dx; STRIPEDY=dy; STRIPEDZ=dz;}
   void setstripeoffset(float stripeoffset) {STRIPEOFFSET=stripeoffset;}

   //! render surfaces
   void render(int phase=MINIBRICK_ONE_RENDER_PHASE,
               int passes=4,
               float ambient=0.1f,
               float bordercontrol=1.0f,float centercontrol=1.0f,float colorcontrol=1.0f,
               float bordercontrol2=1.0f,float centercontrol2=1.0f,float colorcontrol2=1.0f);

   //! cache one data block
   void cacheblock(const unsigned int i,const unsigned int j,const unsigned int k,const unsigned int s,
                   const float vc1,const float vc2,const float vc3,const float vc4,const float vc5,const float vc6,const float vc7,const float vc8,
                   const mininorm &nc1,const mininorm &nc2,const mininorm &nc3,const mininorm &nc4,const mininorm &nc5,const mininorm &nc6,const mininorm &nc7,const mininorm &nc8);

   protected:

   minispect SPECTRUM;

   float STRIPEWIDTH;
   float STRIPEDX,STRIPEDY,STRIPEDZ;
   float STRIPEOFFSET;

   private:

   static int INSTANCES;

   static int VTXPROGID;
   static int FRAGPROGID;

   static int VTXPROGID2;
   static int FRAGPROGID2;

   void extractiso(const float isoval,
                   const int x1,const int y1,const int z1,const float c1,const mininorm &n1,
                   const int x2,const int y2,const int z2,const float c2,const mininorm &n2,
                   const int x3,const int y3,const int z3,const float c3,const mininorm &n3,
                   const int x4,const int y4,const int z4,const float c4,const mininorm &n4,
                   minivtxarray *vtx);

   inline void extractiso1A(const int x1,const int y1,const int z1,const mininorm &n1,
                            const int x2,const int y2,const int z2,const mininorm &n2,
                            const int x3,const int y3,const int z3,const mininorm &n3,
                            const int x4,const int y4,const int z4,const mininorm &n4,
                            minivtxarray *vtx);

   inline void extractiso1B(const int x1,const int y1,const int z1,const mininorm &n1,const float d1,
                            const int x2,const int y2,const int z2,const mininorm &n2,const float d2,
                            const int x3,const int y3,const int z3,const mininorm &n3,
                            const int x4,const int y4,const int z4,const mininorm &n4,
                            minivtxarray *vtx);

   inline void extractiso1C(const int x1,const int y1,const int z1,const mininorm &n1,const float d1,
                            const int x2,const int y2,const int z2,const mininorm &n2,const float d2,
                            const int x3,const int y3,const int z3,const mininorm &n3,const float d3,
                            const int x4,const int y4,const int z4,const mininorm &n4,
                            minivtxarray *vtx);

   inline void extractiso1D(const int x1,const int y1,const int z1,const mininorm &n1,const float d1,
                            const int x2,const int y2,const int z2,const mininorm &n2,const float d2,
                            const int x3,const int y3,const int z3,const mininorm &n3,const float d3,
                            const int x4,const int y4,const int z4,const mininorm &n4,const float d4,
                            minivtxarray *vtx);

   inline void extractiso2(const int x1,const int y1,const int z1,const mininorm &n1,const float d1,
                           const int x2,const int y2,const int z2,const mininorm &n2,const float d2,
                           const int x3,const int y3,const int z3,const mininorm &n3,const float d3,
                           const int x4,const int y4,const int z4,const mininorm &n4,const float d4,
                           minivtxarray *vtx);

   void renderarrays(int phase);

   void enabletorch(int phase,
                    float ambient,
                    float bordercontrol,float centercontrol,float colorcontrol);

   void disabletorch(int phase);

   void enablepattern(float ambient,
                      float bordercontrol,float centercontrol,float colorcontrol);

   void disablepattern();

   // OpenGL extensions:

   int GLSETUP;

   int GLEXT_VP;
   int GLEXT_FP;

   void initglexts();

   // Windows OpenGL extensions:

   int WGLSETUP;

#ifndef NOOGL

#ifdef _WIN32

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

class minibrick
   {
   public:

   //! default constructor
   minibrick(int cols=1,int rows=1);

   //! destructor
   ~minibrick();

   //! set paging callbacks
   void setloader(int (*isavailable)(int col,int row,int lod,void *data),void *data,
                  void (*loadvolume)(int col,int row,int lod,databuf *volume,void *data),
                  float offsetlat=0.0f,float offsetlon=0.0f,
                  float scalex=1.0f,float scaley=1.0f,float scaleelev=1.0f,
                  int paging=0,
                  float safety=0.0f,
                  int expire=0);

   //! set the brick orientation
   void setorientation(float dx1,float dy1,float dz1,
                       float dx2,float dy2,float dz2,
                       float dx3,float dy3,float dz3);

   //! reset the coordinate system
   void resetcoords(float offsetlat=0.0f,float offsetlon=0.0f,
                    float scalex=1.0f,float scaley=1.0f,float scaleelev=1.0f);

   //! reset the brick position
   void resetpos(float midx,float midy,float basez,
                 float dx,float dy,float dz,
                 int col=0,int row=0);

   //! set optional working thread
   void setthread(void (*start)(void *(*thread)(void *brick),void *brick,void *data),void *data,
                  void (*join)(void *data),
                  void (*lock_cs1)(void *data),
                  void (*unlock_cs1)(void *data),
                  void (*lock_cs2)(void *data),
                  void (*unlock_cs2)(void *data));

   //! stop working thread
   void stopthread();

   //! get number of columns
   int getcols() {return(COLS);}

   //! get number of rows
   int getrows() {return(ROWS);}

   //! get one brick tile
   databuf *getvolume(int col,int row);

   //! add iso value
   void addiso(float iso,float R,float G,float B,float A);

   //! delete iso value
   void deliso(float iso);

   //! reset iso spectrum
   void resetiso();

   //! determine size of cullmap
   void setcullmapsize(int size=256);

   //! add clipping plane
   void addclip(int num,float ox,float oy,float oz,float nx,float ny,float nz);

   //! delete clipping plane
   void delclip(int num);

   //! reset clipping planes
   void resetclip();

   //! extract triangle mesh
   void extract(float ex,float ey,float ez,
                float rad,float farp,
                float fovy,float aspect,
                float t=0.0f);

   //! render the brick
   void render(float ex,float ey,float ez,
               float rad,float farp,
               float fovy,float aspect,
               float t=0.0f,
               int phase=MINIBRICK_ONE_RENDER_PHASE);

   //! adjust distance for C-LOD colculation
   void setoffset(float off) {OFF=off;}

   //! override distance for S-LOD colculation
   void setdistance(float dist) {DIST=dist;}

   //! freeze volume
   void freeze() {FREEZE=1;}

   //! defrost volume
   void defrost() {FREEZE=0;}

   // release all buffers
   void release();

   //! configuring
   void configure_reverse(int reverse=1) {CONFIGURE_REVERSE=reverse;} // reverse normals
   void configure_semiopen(int semiopen=0) {CONFIGURE_SEMIOPEN=semiopen;} // use semi-open time [interval[
   void configure_lomem(int lomem=1) {CONFIGURE_LOMEM=lomem;} // use low memory configuration

   //! configure shading
   void configure_renderpasses(int passes=4) {CONFIGURE_RENDERPASSES=passes;}
   void configure_shadeparams(float ambient=0.1f,float brdrctrl=1.0f,float cntrctrl=1.0f,float colrctrl=1.0f) {CONFIGURE_AMBIENT=ambient; CONFIGURE_BRDRCTRL=brdrctrl; CONFIGURE_CNTRCTRL=cntrctrl; CONFIGURE_COLRCTRL=colrctrl;}
   void configure_shadeparams2(float brdrctrl2=1.0f,float cntrctrl2=1.0f,float colrctrl2=1.0f) {CONFIGURE_BRDRCTRL2=brdrctrl2; CONFIGURE_CNTRCTRL2=cntrctrl2; CONFIGURE_COLRCTRL2=colrctrl2;}

   //! configure pattern
   void configure_stripewidth(float stripewidth=1.0f) {CONFIGURE_STRIPEWIDTH=stripewidth;}
   void configure_stripedir(float dx=0.0f,float dy=0.0f,float dz=1.0f) {CONFIGURE_STRIPEDX=dx; CONFIGURE_STRIPEDY=dy; CONFIGURE_STRIPEDZ=dz;}
   void configure_stripeoffset(float stripeoffset=0.0f) {CONFIGURE_STRIPEOFFSET=stripeoffset;}

   protected:

   int COLS,ROWS;
   databuf *BRICKS,*MINMAX,*NORMAL;
   float *MINVAL,*MAXVAL;
   int *MAXLOD,*LOD;

   minisurf **SURFACE;

   int *LRU;
   int FRAME;

   private:

   int (*AVAILABLE_CALLBACK)(int col,int row,int lod,void *data);
   void (*LOAD_CALLBACK)(int col,int row,int lod,databuf *volume,void *data);
   void *LOAD_DATA;

   float OFFSETLAT,OFFSETLON;
   float SCALEX,SCALEY,SCALEELEV;

   float DX1,DY1,DZ1;
   float DX2,DY2,DZ2;
   float DX3,DY3,DZ3;

   int PAGING;
   float SAFETY;
   int EXPIRE;

   void (*START_CALLBACK)(void *(*thread)(void *brick),void *brick,void *data);
   void (*JOIN_CALLBACK)(void *data);
   void (*LOCK1_CALLBACK)(void *data);
   void (*UNLOCK1_CALLBACK)(void *data);
   void (*LOCK2_CALLBACK)(void *data);
   void (*UNLOCK2_CALLBACK)(void *data);
   void *START_DATA;

   int ISRUNNING;
   int ISREADY;

   float CYCLE_EX;
   float CYCLE_EY;
   float CYCLE_EZ;
   float CYCLE_RAD;
   float CYCLE_OFF;
   float CYCLE_DIST;
   float CYCLE_FARP;
   float CYCLE_FOVY;
   float CYCLE_ASPECT;
   float CYCLE_T;

   float LAST_EX;
   float LAST_EY;
   float LAST_EZ;
   float LAST_RAD;
   float LAST_OFF;
   float LAST_DIST;
   float LAST_FARP;
   float LAST_FOVY;
   float LAST_ASPECT;
   float LAST_T;

   int UPDATED_SPECT;
   int UPDATED_COORD;
   int UPDATED_LOD;

   databuf *BRICK;

   minisurf *SURF;

   int LOMEM;

   float *DATA;

   unsigned short int *MM;
   unsigned char *MM2;

   unsigned char *NRML;

   int XSIZE,YSIZE,ZSIZE;
   int MINSIZE;

   float MINV,MAXV;
   float MMRANGE;

   float RES,SLOD;
   float EX,EY,EZ;
   float CX,CY,CZ;
   float DOFF;

   float THETA,OMTHETA;

   float CLIP_OX[6];
   float CLIP_OY[6];
   float CLIP_OZ[6];

   float CLIP_NX[6];
   float CLIP_NY[6];
   float CLIP_NZ[6];

   int CLIP_ON[6];

   float OFF;
   float DIST;

   int FREEZE;

   int CONFIGURE_REVERSE;
   int CONFIGURE_SEMIOPEN;
   int CONFIGURE_LOMEM;

   int CONFIGURE_RENDERPASSES;
   float CONFIGURE_AMBIENT;
   float CONFIGURE_BRDRCTRL;
   float CONFIGURE_CNTRCTRL;
   float CONFIGURE_COLRCTRL;
   float CONFIGURE_BRDRCTRL2;
   float CONFIGURE_CNTRCTRL2;
   float CONFIGURE_COLRCTRL2;

   float CONFIGURE_STRIPEWIDTH;
   float CONFIGURE_STRIPEDX;
   float CONFIGURE_STRIPEDY;
   float CONFIGURE_STRIPEDZ;
   float CONFIGURE_STRIPEOFFSET;

   int isvisible(databuf *brick,float ex,float ey,float ez,float rad,float off,float dist,float farp,float fovy,float aspect);

   int getlod0(int col,int row);
   int getlod(databuf *brick,int lod,int maxlod,float ex,float ey,float ez,float rad,float off,float dist);

   void pagedata(float ex,float ey,float ez,
                 float rad,float off,float dist,float farp,
                 float fovy,float aspect,
                 float t);

   void checkdata(databuf *brick,databuf *minmax,databuf *normal,
                  float *minval,float *maxval,
                  minisurf *surface);

   void initdata(databuf *brick,databuf *minmax,databuf *normal,
                 float *minval,float *maxval,
                 minisurf *surface);

   void initblock(const unsigned int i,const unsigned int j,const unsigned int k,const unsigned int s,const unsigned int t,
                  const float vc1,const float vc2,const float vc3,const float vc4,const float vc5,const float vc6,const float vc7,const float vc8,
                  float *minval,float *maxval);

   static void *startcycle(void *brick);
   void cycle();

   int check4update(float ex,float ey,float ez,
                    float rad,float off,float dist,float farp,
                    float fovy,float aspect,
                    float t);

   void update(float ex,float ey,float ez,
               float rad,float off,float dist,float farp,
               float fovy,float aspect,
               float t);

   void updatedata(int lod,float ex,float ey,float ez,float rad,float off,float dist,float t,
                   databuf *brick,databuf *minmax,databuf *normal,
                   float *minval,float *maxval,
                   minisurf *surface);

   inline float bf(const unsigned int i,const unsigned int j,const unsigned int k,const unsigned int s2);

   void calcblock(const unsigned int i,const unsigned int j,const unsigned int k,const unsigned int s,
                  const float vc1,const float vc2,const float vc3,const float vc4,const float vc5,const float vc6,const float vc7,const float vc8,
                  const mininorm &nc1,const mininorm &nc2,const mininorm &nc3,const mininorm &nc4,const mininorm &nc5,const mininorm &nc6,const mininorm &nc7,const mininorm &nc8);

   void drawdata(databuf *brick,minisurf *surface,int phase);
   };

#endif
