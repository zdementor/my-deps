// (c) by Stefan Roettger

#include "minibase.h"

#include "pnmbase.h"

#include "miniOGL.h"
#include "minitext.hpp"

#include "minipoint.hpp"

// default constructor
minipoint::minipoint(minitile *tile)
   {
   TILE=tile;

   POINTS=NULL;
   PNUM=0;

   VPOINTS=NULL;
   MAXVNUM=0;
   VNUM=0;

   TAKEN=TRANS=0;

   OFFSETLAT=OFFSETLON=0.0f;
   SCALEX=SCALEY=SCALEELEV=1.0f;

   LODS=NULL;

   CONFIGURE_SRCDATUM=3; // WGS84
   CONFIGURE_DSTZONE=0; // LatLon
   CONFIGURE_DSTDATUM=3; // WGS84
   CONFIGURE_AUTOMAP=0;

   CONFIGURE_BRICKCEILING=0.0f;
   CONFIGURE_BRICKPASSES=1;
   CONFIGURE_BRICKALPHA=0.5f;
   CONFIGURE_BRICKLODS=16;
   CONFIGURE_BRICKSTAGGER=1.25f;
   CONFIGURE_BRICKSTRIPES=0.0f;
   }

// destructor
minipoint::~minipoint()
   {
   int i,j;

   if (POINTS!=NULL)
      {
      for (i=0; i<COLS*ROWS; i++)
         {
         for (j=0; j<NUM[i]; j++)
            {
            if (POINTS[i][j].desc!=NULL) free(POINTS[i][j].desc);
            if (POINTS[i][j].meta!=NULL) free(POINTS[i][j].meta);
            if (POINTS[i][j].system!=NULL) free(POINTS[i][j].system);
            if (POINTS[i][j].latitude!=NULL) free(POINTS[i][j].latitude);
            if (POINTS[i][j].longitude!=NULL) free(POINTS[i][j].longitude);
            if (POINTS[i][j].elevation!=NULL) free(POINTS[i][j].elevation);
            }

         free(POINTS[i]);
         }

      free(POINTS);

      free(MAXNUM);
      free(NUM);
      }

   if (VPOINTS!=NULL) free(VPOINTS);

   if (LODS!=NULL) delete LODS;
   }

// add waypoint
void minipoint::add(minipointdata *point)
   {
   int i;

   float posx,posy;
   int col,row;

   if (TILE==NULL) ERRORMSG();

   if (POINTS==NULL)
      {
      COLS=TILE->getcols();
      ROWS=TILE->getrows();

      if ((POINTS=(minipointdata **)malloc(COLS*ROWS*sizeof(minipointdata *)))==NULL) ERRORMSG();

      if ((NUM=(int *)malloc(COLS*ROWS*sizeof(int)))==NULL) ERRORMSG();
      if ((MAXNUM=(int *)malloc(COLS*ROWS*sizeof(int)))==NULL) ERRORMSG();

      for (i=0; i<COLS*ROWS; i++)
         {
         POINTS[i]=NULL;
         MAXNUM[i]=NUM[i]=0;
         }
      }

   posx=(point->x-TILE->getcenterx())/TILE->getcoldim()+COLS/2.0f;
   posy=(-point->y-TILE->getcenterz())/TILE->getrowdim()+ROWS/2.0f;

   if (posx<0.0f || posx>COLS || posy<0.0f || posy>ROWS) return;

   col=ftrc(posx);
   row=ftrc(posy);

   if (col==COLS) col--;
   if (row==ROWS) row--;

   if (NUM[col+row*COLS]>=MAXNUM[col+row*COLS])
      {
      MAXNUM[col+row*COLS]=2*MAXNUM[col+row*COLS]+1;

      if (POINTS[col+row*COLS]==NULL)
         {if ((POINTS[col+row*COLS]=(minipointdata *)malloc(MAXNUM[col+row*COLS]*sizeof(minipointdata)))==NULL) ERRORMSG();}
      else
         {if ((POINTS[col+row*COLS]=(minipointdata *)realloc(POINTS[col+row*COLS],MAXNUM[col+row*COLS]*sizeof(minipointdata)))==NULL) ERRORMSG();}
      }

   point->number=PNUM++;

   POINTS[col+row*COLS][NUM[col+row*COLS]++]=*point;

   point->desc=point->meta=point->system=point->latitude=point->longitude=point->elevation=NULL;
   }

// add character to string
char *minipoint::addch(char *str,char ch)
   {
   const int space=4;

   if (str==NULL)
      {
      if ((str=(char *)malloc(space))==NULL) ERRORMSG();
      str[0]='\0';
      }
   else if (strlen(str)%space>=space-1)
      {
      char *str2;
      if ((str2=(char *)malloc(strlen(str)+1+space))==NULL) ERRORMSG();
      strcpy(str2,str);
      free(str);
      str=str2;
      }

   str[strlen(str)+1]='\0';
   str[strlen(str)]=ch;

   return(str);
   }

// load waypoints
void minipoint::load(char *filename,
                     float offsetlat,float offsetlon,
                     float scalex,float scaley,float scaleelev,
                     minitile *tile,
                     char delimiter)
   {
   int i;

   FILE *file;

   int ch;

   minipointdata point;

   int zone;

   if (tile!=NULL)
      if (TILE==NULL) TILE=tile;
      else if (tile!=TILE) ERRORMSG();

   if (TILE==NULL) ERRORMSG();

   if ((file=fopen(filename,"rb"))==NULL) return;
   ch=fgetc(file);

   while (ch=='\n' || ch=='\r') ch=fgetc(file);

   while (ch!=EOF)
      {
      point.x=point.y=0.0f;
      point.elev=point.height=0.0f;

      point.desc=point.meta=point.system=point.latitude=point.longitude=point.elevation=NULL;

      while (ch!='\n' && ch!='\r')
         {
         point.meta=addch(point.meta,ch);
         ch=fgetc(file);
         }

      while (ch=='\n' || ch=='\r') ch=fgetc(file);

      if (point.meta==NULL) ERRORMSG();

      for (i=0; i<strlen(point.meta); i++)
         {
         if (point.meta[i]==delimiter) break;
         point.desc=addch(point.desc,point.meta[i]);
         }

      if (point.desc==NULL) point.desc=addch(point.desc,'\0');

      while (ch!='\n' && ch!='\r')
         {
         point.system=addch(point.system,ch);
         ch=fgetc(file);
         }

      while (ch=='\n' || ch=='\r') ch=fgetc(file);

      if (point.system==NULL) ERRORMSG();

      if (strcmp(point.system,"LL")==0)
         {
         while (ch!='\n' && ch!='\r')
            {
            point.latitude=addch(point.latitude,ch);
            ch=fgetc(file);
            }

         while (ch=='\n' || ch=='\r') ch=fgetc(file);

         if (point.latitude==NULL) ERRORMSG();

         while (ch!='\n' && ch!='\r')
            {
            point.longitude=addch(point.longitude,ch);
            ch=fgetc(file);
            }

         while (ch=='\n' || ch=='\r') ch=fgetc(file);

         if (point.longitude==NULL) ERRORMSG();

         if (sscanf(point.latitude,"%g",&point.y)!=1) ERRORMSG();
         if (sscanf(point.longitude,"%g",&point.x)!=1) ERRORMSG();

         point.x*=60*60;
         point.y*=60*60;

         point.x=LONSUBLL(point.x);
         if (point.y<-90*60*60 || point.y>90*60*60) ERRORMSG();

         if (CONFIGURE_AUTOMAP!=0 && TAKEN==0)
            {
            CONFIGURE_DSTZONE=0;
            TAKEN=1;
            }
         }
      else if (sscanf(point.system,"UTM %d",&zone)==1)
         {
         if (zone==0 || zone<-60 || zone>60) ERRORMSG();

         while (ch!='\n' && ch!='\r')
            {
            point.longitude=addch(point.longitude,ch);
            ch=fgetc(file);
            }

         while (ch=='\n' || ch=='\r') ch=fgetc(file);

         if (point.longitude==NULL) ERRORMSG();

         while (ch!='\n' && ch!='\r')
            {
            point.latitude=addch(point.latitude,ch);
            ch=fgetc(file);
            }

         while (ch=='\n' || ch=='\r') ch=fgetc(file);

         if (point.latitude==NULL) ERRORMSG();

         if (sscanf(point.longitude,"%g",&point.x)!=1) ERRORMSG();
         if (sscanf(point.latitude,"%g",&point.y)!=1) ERRORMSG();

         UTM2LL(point.x,point.y,zone,CONFIGURE_SRCDATUM,&point.y,&point.x);

         if (CONFIGURE_AUTOMAP!=0 && TAKEN==0)
            {
            CONFIGURE_DSTZONE=zone;
            CONFIGURE_DSTDATUM=CONFIGURE_SRCDATUM;

            TAKEN=1;
            }
         }
      else ERRORMSG();

      while (ch!='\n' && ch!='\r')
         {
         point.elevation=addch(point.elevation,ch);
         ch=fgetc(file);
         }

      while (ch=='\n' || ch=='\r') ch=fgetc(file);

      if (point.elevation==NULL) ERRORMSG();

      if (sscanf(point.elevation,"%g",&point.elev)!=1) ERRORMSG();

      if (CONFIGURE_DSTZONE==0)
         {
         point.x=LONSUBLL(point.x,-offsetlon);
         point.y+=offsetlat;

         if (point.y<-90*60*60 || point.y>90*60*60) ERRORMSG();
         }
      else
         {
         LL2UTM(point.y,point.x,CONFIGURE_DSTZONE,CONFIGURE_DSTDATUM,&point.x,&point.y);

         point.x+=offsetlon;
         point.y+=offsetlat;
         }

      point.x*=scalex;
      point.y*=scaley;

      point.elev*=scaleelev;
      point.height=-MAXFLOAT;

      add(&point);

      if (point.desc!=NULL) free(point.desc);
      if (point.meta!=NULL) free(point.meta);
      if (point.system!=NULL) free(point.system);
      if (point.latitude!=NULL) free(point.latitude);
      if (point.longitude!=NULL) free(point.longitude);
      if (point.elevation!=NULL) free(point.elevation);
      }

   fclose(file);

   if (TRANS==0)
      {
      OFFSETLAT=offsetlat;
      OFFSETLON=offsetlon;

      SCALEX=scalex;
      SCALEY=scaley;
      SCALEELEV=scaleelev;

      TRANS=1;
      }
   }

// get waypoint
minipointdata *minipoint::getpoint(int p)
   {
   int i,j,k;

   if (TILE==NULL) return(NULL);

   for (i=0; i<COLS; i++)
      for (j=0; j<ROWS; j++)
         for (k=0; k<NUM[i+j*COLS]; k++)
            if (p==POINTS[i+j*COLS][k].number) return(&POINTS[i+j*COLS][k]);

   return(NULL);
   }

// calculate visible waypoints
void minipoint::calcvdata()
   {
   int i,j,k;

   if (TILE==NULL) return;

   VNUM=0;

   for (i=TILE->getvisibleleft(); i<=TILE->getvisibleright(); i++)
      for (j=TILE->getvisiblebottom(); j<=TILE->getvisibletop(); j++)
         for (k=0; k<NUM[i+j*COLS]; k++)
            {
            if (VNUM>=MAXVNUM)
               {
               MAXVNUM=2*MAXVNUM+1;

               if (VPOINTS==NULL)
                  {if ((VPOINTS=(minipointdata **)malloc(MAXVNUM*sizeof(minipointdata *)))==NULL) ERRORMSG();}
               else
                  {if ((VPOINTS=(minipointdata **)realloc(VPOINTS,MAXVNUM*sizeof(minipointdata *)))==NULL) ERRORMSG();}
               }

            VPOINTS[VNUM]=&POINTS[i+j*COLS][k];

            VPOINTS[VNUM]->height=TILE->getheight(VPOINTS[VNUM]->x,-VPOINTS[VNUM]->y);

            VNUM++;
            }
   }

// sort visible waypoints along viewing axis
void minipoint::sortvdata(float x,float y,float elev,
                          float dx,float dy,float de)
   {
   if (VPOINTS!=NULL)
      shellsort(VPOINTS,VNUM,x,y,elev,dx,dy,de);
   }

// shellsort as proposed by Robert Sedgewick in "Algorithms"
void minipoint::shellsort(minipointdata **a,const int n,
                          float x,float y,float elev,
                          float dx,float dy,float de)
   {
   int i,j,h;

   minipointdata *v;

   for (h=1; h<=(n-1)/9; h=3*h+1);

   while (h>0)
      {
      for (i=h; i<n; i++)
         {
         j=i;
         v=a[i];
         while (j>=h && compare(v,a[j-h],x,y,elev,dx,dy,de))
            {
            a[j]=a[j-h];
            j-=h;
            }
         a[j]=v;
         }
      h/=3;
      }
   }

// comparison operator for shellsort
inline int minipoint::compare(minipointdata *a,minipointdata *b,
                              float x,float y,float elev,
                              float dx,float dy,float de)
   {
   return(dx*(a->x-x)+dy*(a->y-y)+de*(a->height-elev)>
          dx*(b->x-x)+dy*(b->y-y)+de*(b->height-elev));
   }

// get nearest waypoint
minipointdata *minipoint::getnearest(float x,float y,float elev)
   {
   int i;

   minipointdata **vpoint=VPOINTS,*nearest=NULL;

   if (vpoint==NULL) return(NULL);

   for (i=0; i<VNUM; i++,vpoint++)
      if (nearest==NULL) nearest=*vpoint;
      else if (getdistance2(x,y,elev,*vpoint)<getdistance2(x,y,elev,nearest)) nearest=*vpoint;

   return(nearest);
   }

// get squared distance to waypoint
float minipoint::getdistance2(float x,float y,float elev,minipointdata *point)
   {return(fsqr(point->x-x)+fsqr(point->y-y)+fsqr(point->height-elev));}

// render waypoints with signposts
void minipoint::drawsignposts(float ex,float ey,float ez,
                              float height,float range,
                              float turn,float yon)
   {
   int i;

   float sint,cost;
   float siny,cosy;

   float dx,dy,dz;

   const int maxinfo=1000;
   static char info[maxinfo];

   minipointdata **vpoint;
   minipointdata *nearest;

   calcvdata();
   vpoint=getvdata();

   if (vpoint==NULL) return;

   nearest=getnearest(ex,ez,ey);

   sint=sin(2.0f*PI/360.0f*turn);
   cost=cos(2.0f*PI/360.0f*turn);

   siny=sin(2.0f*PI/360.0f*yon);
   cosy=cos(2.0f*PI/360.0f*yon);

   dx=cosy*sint;
   dz=cosy*cost;
   dy=siny;

   sortvdata(ex,ez,ey,dx,dz,dy);

   initstate();
   disableculling();
   enableblending();

   linewidth(2);
   enablelinesmooth();

   // mark all waypoints with a post
   for (i=0; i<getvnum(); i++,vpoint++)
      {
      if (*vpoint==nearest) color(1.0f,0.0f,0.0f);
      else color(0.0f,0.0f,1.0f);

      drawline((*vpoint)->x,(*vpoint)->height,-(*vpoint)->y,(*vpoint)->x,(*vpoint)->height+height,-(*vpoint)->y);
      }

   linewidth(1);

   minitext::configure_zfight(0.95f);

   // compile label information of nearest waypoint
   snprintf(info,maxinfo,"\n %s \n\n %s Lat=%s Lon=%s \n Elev=%s \n",
            nearest->desc,
            nearest->system,nearest->latitude,nearest->longitude,
            nearest->elevation);

   // label nearest waypoint
   mtxpush();
   mtxtranslate(0.0f,height,0.0f);
   mtxtranslate(nearest->x,nearest->height,-nearest->y);
   mtxrotate(-turn,0.0f,1.0f,0.0f);
   mtxscale(2.0f*height,2.0f*height,2.0f*height);
   mtxrotate(yon,1.0f,0.0f,0.0f);
   mtxtranslate(-0.5f,0.0f,0.0f);
   minitext::drawstring(1.0f,240.0f,0.5f,0.5f,1.0f,info,1.0f,1.0f);
   mtxpop();

   disableZwriting();

   // process waypoints from back to front
   vpoint=getvdata();
   for (i=0; i<getvnum(); i++,vpoint++)
      if (*vpoint!=nearest)
         if (getdistance2(ex,ez,ey,*vpoint)<fsqr(range))
            {
            // compile label information of waypoint within range
            snprintf(info,maxinfo,"\n %s \n",(*vpoint)->desc);

            // label waypoint within range
            mtxpush();
            mtxtranslate(0.0f,height,0.0f);
            mtxtranslate((*vpoint)->x,(*vpoint)->height,-(*vpoint)->y);
            mtxrotate(-turn,0.0f,1.0f,0.0f);
            mtxrotate(yon,1.0f,0.0f,0.0f);
            mtxscale(2.0f*height,2.0f*height,2.0f*height);
            mtxtranslate(-0.5f,0.0f,0.0f);
            minitext::drawstring(1.0f,240.0f,0.5f,0.5f,1.0f,info,1.0f,0.75f);
            mtxpop();
            }

   enableZwriting();

   disablelinesmooth();

   disableblending();
   enableBFculling();
   exitstate();
   }

// render waypoints with bricks
void minipoint::drawbricks(float ex,float ey,float ez,
                           float brad,float farp,
                           float fovy,float aspect,
                           float size)
   {
   int i;

   minipointdata **vpoint;

   float midx,midy,basez;
   float color,r,g,b;

   // calculate visible points
   calcvdata();
   vpoint=getvdata();

   // check if any points are available
   if (vpoint==NULL) return;

   // initialize renderer
   if (LODS==NULL)
      {
      LODS=new minilod(OFFSETLAT,OFFSETLON,SCALEX,SCALEY,SCALEELEV);
      LODS->configure_brickpasses(CONFIGURE_BRICKPASSES);
      LODS->addbrick("Cone.db",brad,CONFIGURE_BRICKLODS,CONFIGURE_BRICKSTAGGER);
      }

   // set stripe pattern
   LODS->configure_brickoffset(CONFIGURE_BRICKSTRIPES);

   // clear all volumes
   LODS->clearvolumes();

   // update visible points
   for (i=0; i<getvnum(); i++,vpoint++)
      {
      // calculate position
      midx=(*vpoint)->x/SCALEX-OFFSETLON;
      midy=(*vpoint)->y/SCALEY-OFFSETLAT;
      basez=((*vpoint)->elev-0.25f*size)/SCALEELEV;

      // check elevation
      if (CONFIGURE_BRICKCEILING==0.0f) color=0.0f;
      else color=basez/CONFIGURE_BRICKCEILING;

      // calculate color
      if (color<0.5f)
         {
         r=0.0f;
         g=2.0f*color;
         b=1.0f-2.0f*color;
         }
      else
         {
         r=2.0f*color-1.0f;
         g=2.0f-2.0f*color;
         b=0.0f;
         }

      // set position and color
      LODS->addvolume(0,
                      midx,midy,basez,
                      size/SCALEX,size/SCALEY,size/SCALEELEV,
                      r,g,b,CONFIGURE_BRICKALPHA);
      }

   // render visible points
   LODS->render(ex,ey,ez,farp,fovy,aspect);
   }

// configuring:

void minipoint::configure_srcdatum(int datum)
   {CONFIGURE_SRCDATUM=datum;}

void minipoint::configure_dstzone(int zone)
   {CONFIGURE_DSTZONE=zone;}

void minipoint::configure_dstdatum(int datum)
   {CONFIGURE_DSTDATUM=datum;}

void minipoint::configure_automap(int automap)
   {CONFIGURE_AUTOMAP=automap;}

void minipoint::configure_brickceiling(float brickceiling)
   {CONFIGURE_BRICKCEILING=brickceiling;}

void minipoint::configure_brickpasses(int brickpasses)
   {CONFIGURE_BRICKPASSES=brickpasses;}

void minipoint::configure_brickalpha(float brickalpha)
   {CONFIGURE_BRICKALPHA=brickalpha;}

void minipoint::configure_bricklods(int bricklods)
   {CONFIGURE_BRICKLODS=bricklods;}

void minipoint::configure_brickstagger(float brickstagger)
   {CONFIGURE_BRICKSTAGGER=brickstagger;}

void minipoint::configure_brickstripes(float brickstripes)
   {CONFIGURE_BRICKSTRIPES=brickstripes;}
