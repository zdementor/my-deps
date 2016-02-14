// (c) by Stefan Roettger

#ifndef MINITILE_H
#define MINITILE_H

#include "database.hpp"

class minitile
   {
   public:

   //! default constructor
   //! constructs a regularly tiled terrain
   //! each tile is assumed to have a corner centric data representation
   minitile(unsigned char **hfields, // 2D array of filenames for height fields
            unsigned char **textures, // 2D array of filenames for texture maps
            int cols,int rows, // number of tile columns and rows
            float coldim,float rowdim, // dimension of a column or a row
            float scale, // vertical scaling of elevations
            float centerx=0.0f,float centery=0.0f,float centerz=0.0f, // map center
            unsigned char **fogmaps=0, // optional 2D array of filenames for ground fog maps
            float lambda=0.0f, // vertical scaling of fog elevations
            float displace=0.0f, // vertical fog displacement above ground
            float emission=0.0f, // fog appearance control
            float attenuation=1.0f, // relative importance of fog
            float fogR=1.0f,float fogG=1.0f,float fogB=1.0f, // fog color
            float minres=0.0f, // absolute minimum of global resolution
            float bsafety=0.0f, // absolute minimum base offset safety
            int loaded=0); // interprete 2D arrays as images

   //! destructor
   ~minitile();

   //! configure core parameters
   static void configure_minres(float minres=9.0f); // default minimum resolution
   static void configure_maxd2(float maxd2=100.0f); // default maximum d2-value
   static void configure_gradinf(float gradinf=0.25f); // default influence of gradient
   static void configure_bsafety(float bsafety=0.1f); // default base offset safety
   static void configure_maxcull(int maxcull=8); // default maximum culling depth

   //! configuring
   static void configure_mipmaps(int mipmaps=1); // turn mipmaps on or off
   static void configure_tilesetpath(char *tilesetpath); // define tile set path

   //! draw scene
   void draw(float res, // resolution
             float ex,float ey,float ez, // eye point
             float dx,float dy,float dz, // view direction
             float ux,float uy,float uz, // up vector
             float fovy,float aspect, // field of view and aspect
             float nearp,float farp, // near and far plane
             int update=0); // optional incremental update

   //! set focus of interest (equal to eye point by default)
   void setfocus(int on=0,float fx=0.0f,float fy=0.0f,float fz=0.0f);

   //! define relative scaling (0<=scale<=1)
   void setrelscale(float scale=1.0f);

   //! set rendering callbacks
   void setcallbacks(void (*beginfan)(), // mandatory for incremental update
                     void (*fanvertex)(float i,float y,float j), // mandatory for incremental update
                     void (*notify)(int i,int j,int s)=0, // optional
                     void (*texmap)(int m,int n,int S)=0, // optional
                     void (*prismedge)(float x,float y,float yf,float z)=0, // optional
                     void (*trigger)(int phase,float scale,float ex,float ey,float ez)=0); // mandatory for incremental update

   //! define resolution reduction of invisible tiles
   void setreduction(float reduction=2.0f,float ratio=3.0f);

   //! set rendering modes
   void settexmode(int texmode=0) {TEXMODE=texmode;} // 0=on 1=off
   void setfogmode(int fogmode=1) {FOGMODE=fogmode;} // 0=off 1=on 2=mip

   //! loader for tile sets
   static minitile *load(int cols,int rows,
                         char *basepath1=0,char *basepath2=0,char *basepath3=0,
                         float offsetlat=0.0f,float offsetlon=0.0f,
                         float exaggeration=1.0f,float scale=1.0f,
                         void (*shader)(float nx,float ny,float nz,float elev,float *rgb)=0,
                         float sealevel=0.0f,float snowline=3000.0f,
                         float lambda=1.0f,float attenuation=1.0f,
                         float minres=0.0f,float bsafety=0.0f,
                         float *outparams=0, // array of 5 output parameters
                         float *arcsec=0); // array of 2 output parameters

   //! get-functions for geometric properties
   float getheight(float x,float z,int approx=0);
   float getfogheight(float x,float z,int approx=0);
   void getnormal(float x,float z,float *nx,float *ny,float *nz);
   int getdim(float x,float z,float *dimx,float *dimz,float *radius2=0);

   // grid query functions:

   int getcols() {return(COLS);}
   int getrows() {return(ROWS);}

   float getcoldim() {return(COLDIM);}
   float getrowdim() {return(ROWDIM);}

   float getsizex() {return(COLS*COLDIM);}
   float getsizez() {return(ROWS*ROWDIM);}

   float getcenterx() {return(CENTERX);}
   float getcentery() {return(CENTERY);}
   float getcenterz() {return(CENTERZ);}

   float getcenterx(int col) {return(CENTERX+COLDIM*(col-(COLS-1)/2.0f));}
   float getcenterz(int row) {return(CENTERZ+ROWDIM*(row-(ROWS-1)/2.0f));}

   float getscale() {return(SCALE*RELSCALE0);}
   float getrelscale() {return(RELSCALE0);}
   float getlambda() {return(LAMBDA);}
   float getdisplace() {return(DISPLACE);}

   int getsize(int col,int row) {return(SIZE[col+row*COLS]);}

   int gettexid(int col,int row) {return(TEXID[col+row*COLS]);}
   int gettexw(int col,int row) {return(TEXWIDTH[col+row*COLS]);}
   int gettexh(int col,int row) {return(TEXHEIGHT[col+row*COLS]);}
   int gettexmm(int col,int row) {return(TEXMIPMAPS[col+row*COLS]);}

   int hasfog(int col,int row) {return(FOGMAP[col+row*COLS]!=0);}

   int usesfloat(int col,int row) {return(USEFLOAT[col+row*COLS]!=0);}

   float getcenteri() {return(CENTERI);}
   float getcenterj() {return(CENTERJ);}

   int getvisibleleft() {return(LEFT);}
   int getvisibleright() {return(RIGHT);}
   int getvisiblebottom() {return(BOTTOM);}
   int getvisibletop() {return(TOP);}

   int getpreloadedleft() {return(PLEFT);}
   int getpreloadedright() {return(PRIGHT);}
   int getpreloadedbottom() {return(PBOTTOM);}
   int getpreloadedtop() {return(PTOP);}

   // functions for grid reloading:

   //! check whether or not a tile has already been loaded
   int isloaded(int col,int row);

   //! check whether or not a tile is visible and has already been loaded
   int isvisible(int col,int row);

   //! check whether or not a tile has already been preloaded
   int ispreloaded(int col,int row);

   //! calculate unused LODs
   int getunusedlods(int col,int row,
                     float res,float fx,float fy,float fz);

   //! reload a specific tile
   void reload(int col,int row, // tile to reload
               unsigned char *hmap, // filename of height map
               unsigned char *tmap, // filename of texture map
               unsigned char *fmap=0, // optional filename of ground fog map
               int updatetex=1, // update actual texture map or leave it untouched
               int loaded=0); // interprete filenames as images

   //! reload a specific tile from data buffers
   void reload(int col,int row, // tile to reload
               databuf &hmap, // height map
               databuf &tmap, // texture map
               databuf &fmap, // optional ground fog map
               int updatetex=1); // update actual texture map or leave it untouched

   //! set callbacks for requesting and preloading tiles
   void setrequest(void (*request)(int col,int row,int needtex,void *data),void *obj,
                   void (*preload)(int col,int row,void *data)=0,
                   float pfarp=0.0f,int pupdate=0);

   //! set preloading
   void setpreload(float pfarp,int pupdate);

   //! modify the terrain at run time
   void setheight(float x,float z,float h);
   void setrealheight(float x,float z,float h);

   protected:

   int COLS,ROWS;
   float COLDIM,ROWDIM;

   void **MAP,**D2MAP,**FOGMAP;
   void **MAP2,**D2MAP2,**FOGMAP2;

   int *SIZE,*SIZE2;
   float *DIM,*DIM2,SCALE,CELLASPECT;

   float CENTERX,CENTERY,CENTERZ;

   int *TEXID;
   int *TEXWIDTH,*TEXHEIGHT;
   int *TEXMIPMAPS;
   int TEXMODE;

   float LAMBDA,DISPLACE;
   float EMISSION,ATTENUATION;
   float FR,FG,FB;
   int FOGMODE;

   int *USEFLOAT,*USEFLOAT2;

   int *RELOADED;
   int *UPDATED;
   int *MODIFIED;

   int MOD;

   float CENTERI,CENTERJ;

   int LEFT,RIGHT,BOTTOM,TOP;

   int COL,ROW;
   int PHASE;

   int SWIZZLE;

   float RELSCALE0,RELSCALE;

   int FOCUS;
   float FX,FY,FZ;

   float PFARP;

   int PLEFT,PRIGHT,PBOTTOM,PTOP;

   int PCOL,PROW;
   int PUPDATE;

   int PSWIZZLE;

   float REDUCTION;
   float RATIO;

   void (*BEGINFAN_CALLBACK)();
   void (*FANVERTEX_CALLBACK)(float i,float y,float j);
   void (*NOTIFY_CALLBACK)(int i,int j,int s);
   void (*TEXMAP_CALLBACK)(int m,int n,int S);
   void (*PRISMEDGE_CALLBACK)(float x,float y,float yf,float z);
   void (*TRIGGER_CALLBACK)(int phase,float scale,float ex,float ey,float ez);

   void (*REQUEST_CALLBACK)(int col,int row,int needtex,void *data);
   void (*PRELOAD_CALLBACK)(int col,int row,void *data);
   void *REQUEST_OBJ;

   private:

   static const int MAX_STR;

   static float CONFIGURE_MINRES;
   static float CONFIGURE_MAXD2;
   static float CONFIGURE_GRADINF;
   static float CONFIGURE_BSAFETY;
   static int CONFIGURE_MAXCULL;

   static int CONFIGURE_MIPMAPS;

   static char CONFIGURE_TILESETPATH[];

   void checktiles(float ex,float ez,
                   float farp,float fovy,float aspect);

   void updatetile(int col,int row);

   void drawtile(float res,
                 float ex,float ey,float ez,
                 float dx,float dy,float dz,
                 float ux,float uy,float uz,
                 float fovy,float aspect,
                 float nearp,float farp,
                 int col,int row,int phase,
                 int update);

   int setblock(float x,float z,int approx=0);

   void freetile(int col,int row,int freetex);
   void freetile2(int col,int row);

   int gcd(int a,int b);
   };

#endif
