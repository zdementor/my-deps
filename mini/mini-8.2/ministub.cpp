// (c) by Stefan Roettger

#include "minibase.h"

#include "mini.h"

#include "ministub.hpp"

// default constructors:

ministub::ministub(short int *image,
                   int *size,float *dim,float scale,float cellaspect,
                   float centerx,float centery,float centerz,
                   void (*beginfan)(),void (*fanvertex)(float i,float y,float j),
                   void (*notify)(int i,int j,int s),
                   short int (*getelevation)(int i,int j,int S,void *data),
                   void *objref,
                   unsigned char *fogimage,
                   int fogsize,float lambda,float displace,float attenuation,
                   void (*prismedge)(float x,float y,float yf,float z),
                   void **d2map2,int *size2,
                   float minres,float bsafety,
                   float maxd2,float gradinf,
                   int maxcull)
   {
   int i;

   FLOAT=FALSE;

   if (minres<1.0f)
      {
      minres=9.0f;
      bsafety=0.1f;
      maxd2=100.0f;
      gradinf=0.25f;
      maxcull=8;
      }

   mini::setparams(minres,maxd2,gradinf,bsafety,maxcull);

   MAP=mini::initmap(image,&D2MAP,
                     size,dim,scale,
                     cellaspect,
                     getelevation,
                     objref);

   FOGMAP=mini::initfogmap(fogimage,
                           fogsize,lambda,displace,
                           0.0f,attenuation);

   SIZE=*size;
   DIM=*dim;
   SCALE=scale;
   CELLASPECT=cellaspect;

   CENTERX=centerx;
   CENTERY=centery;
   CENTERZ=centerz;

   LAMBDA=lambda;
   DISPLACE=displace;

   RELSCALE=1.0f;

   BEGINFAN_CALLBACK=beginfan;
   FANVERTEX_CALLBACK=fanvertex;
   NOTIFY_CALLBACK=notify;
   PRISMEDGE_CALLBACK=prismedge;

   for (i=0; i<4; i++)
      if (d2map2==NULL) D2MAP2[i]=NULL;
      else
         {
         D2MAP2[i]=d2map2[i];
         SIZE2[i]=size2[i];
         }

   MINRES=minres;
   BSAFETY=bsafety;
   MAXD2=maxd2;
   GRADINF=gradinf;
   MAXCULL=maxcull;

   FOCUS=FALSE;

   MODIFIED=FALSE;
   }

ministub::ministub(float *image,
                   int *size,float *dim,float scale,float cellaspect,
                   float centerx,float centery,float centerz,
                   void (*beginfan)(),void (*fanvertex)(float i,float y,float j),
                   void (*notify)(int i,int j,int s),
                   float (*getelevation)(int i,int j,int S,void *data),
                   void *objref,
                   unsigned char *fogimage,
                   int fogsize,float lambda,float displace,float attenuation,
                   void (*prismedge)(float x,float y,float yf,float z),
                   void **d2map2,int *size2,
                   float minres,float bsafety,
                   float maxd2,float gradinf,
                   int maxcull)
   {
   int i;

   FLOAT=TRUE;

   if (minres<1.0f)
      {
      minres=9.0f;
      bsafety=0.1f;
      maxd2=100.0f;
      gradinf=0.25f;
      maxcull=8;
      }

   Mini::setparams(minres,maxd2,gradinf,bsafety,maxcull);

   MAP=Mini::initmap(image,&D2MAP,
                     size,dim,scale,
                     cellaspect,
                     getelevation,
                     objref);

   FOGMAP=Mini::initfogmap(fogimage,
                           fogsize,lambda,displace,
                           0.0f,attenuation);

   SIZE=*size;
   DIM=*dim;
   SCALE=scale;
   CELLASPECT=cellaspect;

   CENTERX=centerx;
   CENTERY=centery;
   CENTERZ=centerz;

   LAMBDA=lambda;
   DISPLACE=displace;

   RELSCALE=1.0f;

   BEGINFAN_CALLBACK=beginfan;
   FANVERTEX_CALLBACK=fanvertex;
   NOTIFY_CALLBACK=notify;
   PRISMEDGE_CALLBACK=prismedge;

   for (i=0; i<4; i++)
      if (d2map2==NULL) D2MAP2[i]=NULL;
      else
         {
         D2MAP2[i]=d2map2[i];
         SIZE2[i]=size2[i];
         }

   MINRES=minres;
   BSAFETY=bsafety;
   MAXD2=maxd2;
   GRADINF=gradinf;
   MAXCULL=maxcull;

   FOCUS=FALSE;

   MODIFIED=FALSE;
   }

// destructor
ministub::~ministub()
   {
   if (!FLOAT)
      {
      mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ,NULL,NULL,
                    FOGMAP);

      mini::deletemaps();
      }
   else
      {
      Mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ,NULL,NULL,
                    FOGMAP);

      Mini::deletemaps();
      }
   }

// use triangle fan callbacks to render terrain (and ground fog)
void ministub::draw(float res,
                    float ex,float ey,float ez,
                    float dx,float dy,float dz,
                    float ux,float uy,float uz,
                    float fovy,float aspect,
                    float nearp,float farp)
   {
   float fx,fy,fz;

   if (FOCUS)
      {
      fx=FX;
      fy=FY;
      fz=FZ;
      }
   else
      {
      fx=ex;
      fy=ey;
      fz=ez;
      }

   if (!FLOAT)
      {
      mini::setparams(MINRES,MAXD2,GRADINF,BSAFETY,MAXCULL);

      mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ,D2MAP2,SIZE2,
                    FOGMAP,LAMBDA,DISPLACE);

      if (MODIFIED)
         {
         mini::updatemaps();
         MODIFIED=FALSE;
         }

      mini::drawlandscape(res,
                          ex,ey,ez,
                          fx,fy,fz,
                          dx,dy,dz,
                          ux,uy,uz,
                          fovy,aspect,
                          nearp,farp,
                          BEGINFAN_CALLBACK,
                          FANVERTEX_CALLBACK,
                          NOTIFY_CALLBACK,
                          PRISMEDGE_CALLBACK);
      }
   else
      {
      Mini::setparams(MINRES,MAXD2,GRADINF,BSAFETY,MAXCULL);

      Mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ,D2MAP2,SIZE2,
                    FOGMAP,LAMBDA,DISPLACE);

      if (MODIFIED)
         {
         Mini::updatemaps();
         MODIFIED=FALSE;
         }

      Mini::drawlandscape(res,
                          ex,ey,ez,
                          fx,fy,fz,
                          dx,dy,dz,
                          ux,uy,uz,
                          fovy,aspect,
                          nearp,farp,
                          BEGINFAN_CALLBACK,
                          FANVERTEX_CALLBACK,
                          NOTIFY_CALLBACK,
                          PRISMEDGE_CALLBACK);
      }
   }

// set focus of interest
void ministub::setfocus(int on,
                        float fx,float fy,float fz)
   {
   FOCUS=on;

   FX=fx;
   FY=fy;
   FZ=fz;
   }

// define relative scaling (0<=scale<=1)
void ministub::setrelscale(float scale)
   {
   if (scale<0.0f || scale>1.0f) ERRORMSG();
   RELSCALE=scale;
   }

// return the elevation at grid position (i,j)
float ministub::getheight(int i,int j)
   {
   if (!FLOAT)
      {
      mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ);

      return(mini::getheight(i,j));
      }
   else
      {
      Mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ);

      return(Mini::getheight(i,j));
      }
   }

// calculate the elevation at position (x,z)
float ministub::getheight(float x,float z)
   {
   if (!FLOAT)
      {
      mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ);

      return(mini::getheight(x,z));
      }
   else
      {
      Mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ);

      return(Mini::getheight(x,z));
      }
   }

// calculate the height of the fog layer at position (x,z)
float ministub::getfogheight(float x,float z)
   {
   if (!FLOAT)
      {
      mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ,NULL,NULL,
                    FOGMAP,LAMBDA);

      return(mini::getfogheight(x,z));
      }
   else
      {
      Mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ,NULL,NULL,
                    FOGMAP,LAMBDA);

      return(Mini::getfogheight(x,z));
      }
   }

// calculate the normal vector at position (x,z)
void ministub::getnormal(float x,float z,float *nx,float *ny,float *nz)
   {
   if (!FLOAT)
      {
      mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ);

      mini::getnormal(x,z,nx,ny,nz);
      }
   else
      {
      Mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ);

      Mini::getnormal(x,z,nx,ny,nz);
      }
   }

// modify the terrain at run time:

void ministub::setheight(int i,int j,float h)
   {
   if (!FLOAT)
      {
      mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ);

      mini::setheight(i,j,h);
      }
   else
      {
      Mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ);

      Mini::setheight(i,j,h);
      }

   MODIFIED=TRUE;
   }

void ministub::setheight(float x,float z,float h)
   {
   if (!FLOAT)
      {
      mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ);

      mini::setheight(x,z,h);
      }
   else
      {
      Mini::setmaps(MAP,D2MAP,
                    SIZE,DIM,SCALE*RELSCALE,
                    0,0,0,0,CELLASPECT,CENTERX,CENTERY,CENTERZ);

      Mini::setheight(x,z,h);
      }

   MODIFIED=TRUE;
   }

void ministub::setrealheight(int i,int j,float h)
   {
   if (SCALE*RELSCALE==0.0f) ERRORMSG();
   setheight(i,j,(h-CENTERY)/(SCALE*RELSCALE));
   }

void ministub::setrealheight(float x,float z,float h)
   {
   if (SCALE*RELSCALE==0.0f) ERRORMSG();
   setheight(x,z,(h-CENTERY)/(SCALE*RELSCALE));
   }
