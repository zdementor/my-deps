// (c) by Stefan Roettger

#include "minibase.h"

#include "pnmbase.h"

namespace pnm {

// PNMcomment implementation:

const int PNMcomment::MAX_STR=1024;
const int PNMcomment::MAX_COM=4096;

PNMcomment::PNMcomment()
   {
   maxc=MAX_COM;
   if ((com=(char *)malloc(maxc))==NULL) ERRORMSG();
   reset();
   }

PNMcomment::~PNMcomment()
   {free(com);}

void PNMcomment::reset()
   {
   c=0;
   com[c]='\0';
   }

char PNMcomment::addchar(char ch)
   {
   if (ch==' ' && c>0)
      if (com[c-1]==' ') return(ch);

   if (c>=maxc-1)
      {
      maxc*=2;
      if ((com=(char *)realloc(com,maxc))==NULL) ERRORMSG();
      }

   com[c++]=ch;
   com[c]='\0';

   return(ch);
   }

void PNMcomment::addstring(char *str)
   {while (*str!='\0') addchar(*str++);}

void PNMcomment::addfloat(float v,int leading)
   {
   char str[MAX_STR];
   if (leading==0) snprintf(str,MAX_STR,"%g",v);
   else snprintf(str,MAX_STR,"%12f",v);
   addstring(str);
   }

void PNMcomment::addunits(int units)
   {
   if (units==0) addstring("radians");
   else if (units==1) addstring("feet");
   else if (units==2) addstring("meters");
   else if (units==3) addstring("decimeters");
   else if (units==4) addstring("arc-seconds");
   }

// check a file
int checkfile(const char *filename)
   {
   FILE *file;

   if ((file=fopen(filename,"rb"))==NULL) return(0);
   fclose(file);

   return(1);
   }

// write a raw file
void writefile(const char *filename,unsigned char *data,int bytes)
   {
   FILE *file;

   if (bytes<1) ERRORMSG();

   if ((file=fopen(filename,"wb"))==NULL) ERRORMSG();
   if (fwrite(data,1,bytes,file)!=bytes) ERRORMSG();

   fclose(file);
   }

// read a raw file
unsigned char *readfile(const char *filename,int *bytes)
   {
   const int BLOCKSIZE=1<<20;

   FILE *file;

   unsigned char *data;
   int cnt,blkcnt;

   if ((file=fopen(filename,"rb"))==NULL) return(NULL);

   data=NULL;
   cnt=0;

   do
      {
      if (data==NULL)
         {if ((data=(unsigned char *)malloc(BLOCKSIZE))==NULL) ERRORMSG();}
      else
         if ((data=(unsigned char *)realloc(data,cnt+BLOCKSIZE))==NULL) ERRORMSG();

      blkcnt=fread(&data[cnt],1,BLOCKSIZE,file);
      cnt+=blkcnt;
      }
   while (blkcnt==BLOCKSIZE);

   if (cnt==0)
      {
      free(data);
      return(NULL);
      }

   if ((data=(unsigned char *)realloc(data,cnt))==NULL) ERRORMSG();

   fclose(file);

   *bytes=cnt;

   return(data);
   }

// write a PNM image
void writePNMimage(const char *pnmfilename,
                   unsigned char *image,
                   int width,int height,int components,
                   PNMcomment *comment)
   {
   FILE *file;

   char *com;

   if (width<1 || height<1) ERRORMSG();

   if ((file=fopen(pnmfilename,"wb"))==NULL) ERRORMSG();

   if (components==1 || components==2) fprintf(file,"P5");
   else if (components==3) fprintf(file,"P6");
   else ERRORMSG();

   if (comment!=NULL)
      {
      com=comment->str();
      fprintf(file,"\n#");
      while (*com!='\0')
         if (*com!='\n') fputc(*com++,file);
         else if (*++com!='\0') fprintf(file,"\n#");
      }

   fprintf(file,"\n%d %d\n",width,height);

   if (components==1 || components==3) fprintf(file,"255\n");
   else fprintf(file,"32767\n");

   if (fwrite(image,width*height*components,1,file)!=1) ERRORMSG();
   fclose(file);
   }

// read a PNM image
unsigned char *readPNMimage(unsigned char *pnmimage,
                            int *width,int *height,int *components,
                            int loaded,unsigned char **ptr,
                            PNMcomment *comment)
   {
   FILE *file;

   int pnmtype,maxval;
   unsigned char *image;

   char ch;
   int val;

   PNMcomment com;

   if (loaded==0)
      {
      if ((file=fopen((char *)pnmimage,"rb"))==NULL) return(NULL);

      if (fscanf(file,"P%1d\n",&pnmtype)!=1)
         {
         fclose(file);
         return(NULL);
         }

      while ((ch=fgetc(file))=='#')
         while (com.addchar(fgetc(file))!='\n');
      ungetc(ch,file);

      if (fscanf(file,"%d %d\n",width,height)!=2) ERRORMSG();
      if (fscanf(file,"%d",&maxval)!=1) ERRORMSG();
      if (fgetc(file)!='\n') ERRORMSG();

      if (*width<1 || *height<1) ERRORMSG();

      if (pnmtype==5 && maxval==255) *components=1;
      else if (pnmtype==5 && (maxval==32767 || maxval==65535)) *components=2;
      else if (pnmtype==6 && maxval==255) *components=3;
      else ERRORMSG();

      if ((image=(unsigned char *)malloc((*width)*(*height)*(*components)))==NULL) ERRORMSG();

      if (fread(image,(*width)*(*height)*(*components),1,file)!=1) ERRORMSG();
      fclose(file);
      }
   else
      {
      image=pnmimage;

      ch=*image++;

      if (ch!='P') return(NULL);

      ch=*image++;

      pnmtype=ch-'0';

      ch=*image++;

      if (ch!='\n') return(NULL);

      ch=*image++;

      while (ch=='#')
         {
         ch=*image++;
         while (com.addchar(ch)!='\n') ch=*image++;
         ch=*image++;
         }

      val=0;

      while (ch>='0' && ch<='9')
         {
         val=10*val+ch-'0';
         ch=*image++;
         }

      *width=val;

      if (ch!=' ') ERRORMSG();

      ch=*image++;

      val=0;

      while (ch>='0' && ch<='9')
         {
         val=10*val+ch-'0';
         ch=*image++;
         }

      *height=val;

      if (ch!='\n') ERRORMSG();

      ch=*image++;

      maxval=0;

      while (ch>='0' && ch<='9')
         {
         maxval=10*maxval+ch-'0';
         ch=*image++;
         }

      if (ch!='\n') ERRORMSG();

      if (pnmtype==5 && maxval==255) *components=1;
      else if (pnmtype==5 && (maxval==32767 || maxval==65535)) *components=2;
      else if (pnmtype==6 && maxval==255) *components=3;
      else ERRORMSG();

      if (ptr!=NULL) *ptr=image+(*width)*(*height)*(*components);
      }

   if (comment!=NULL)
      {
      comment->reset();
      comment->addstring(com.str());
      }

   return(image);
   }

// read a PNM file
unsigned char *readPNMfile(const char *pnmfilename,
                           int *width,int *height,int *components,
                           PNMcomment *comment)
   {
   return(readPNMimage((unsigned char *)pnmfilename,
                       width,height,components,
                       0,NULL,comment));
   }

// write a PVM volume
void writePVMvolume(const char *filename,unsigned char *volume,
                    int width,int height,int depth,int components)
   {
   FILE *file;

   if (width<1 || height<1 || depth<1 || components<1) ERRORMSG();

   if ((file=fopen(filename,"wb"))==NULL) ERRORMSG();

   fprintf(file,"PVM\n%d %d %d\n%d\n",width,height,depth,components);

   if (fwrite(volume,width*height*depth*components,1,file)!=1) ERRORMSG();
   fclose(file);
   }

// read a PVM volume
unsigned char *readPVMvolume(const char *filename,
                             int *width,int *height,int *depth,int *components)
   {
   FILE *file;

   unsigned char header[5],*volume;

   if ((file=fopen(filename,"rb"))==NULL) return(NULL);

   if (fread(header,1,4,file)!=4) return(NULL);

   header[4]='\0';

   if (strcmp((char *)header,"PVM\n")!=0)
      {
      fclose(file);
      return(NULL);
      }

   if (fscanf(file,"%d %d %d\n",width,height,depth)!=3) ERRORMSG();
   if (fscanf(file,"%d",components)!=1) ERRORMSG();
   if (fgetc(file)!='\n') ERRORMSG();

   if (*width<1 || *height<1 || *depth<1 || *components<1) ERRORMSG();

   if ((volume=(unsigned char *)malloc((*width)*(*height)*(*depth)*(*components)))==NULL) ERRORMSG();

   if (fread(volume,(*width)*(*height)*(*depth)*(*components),1,file)!=1) ERRORMSG();
   fclose(file);

   return(volume);
   }

// put a geographic grid descriptor into a PNM comment
// default values correspond to USGS 1:250,000 DEM quads
void putPNMparams(PNMcomment *comment,
                  char *pnm_description,
                  int coord_sys,
                  int coord_zone,
                  int coord_datum,
                  int coord_units,
                  float coord_SW_x,float coord_SW_y,
                  float coord_NW_x,float coord_NW_y,
                  float coord_NE_x,float coord_NE_y,
                  float coord_SE_x,float coord_SE_y,
                  float cell_size_x,float cell_size_y,
                  int scaling_units,
                  float vertical_scaling,
                  int missing_value)
   {
   if (coord_sys<0 || coord_sys>1) ERRORMSG();
   if (coord_zone<-60 || coord_zone>60) ERRORMSG();
   if (coord_units<0 || coord_units>4) ERRORMSG();
   if (cell_size_x<=0.0f || cell_size_y<=0.0f) ERRORMSG();
   if (scaling_units<1 || scaling_units>3) ERRORMSG();

   if (coord_sys==0 && coord_units==3)
      {
      coord_SW_x=LONSUBLL(coord_SW_x);
      if (fabs(coord_SW_y)>90*60*60) ERRORMSG();

      coord_NW_x=LONSUBLL(coord_NW_x);
      if (fabs(coord_NW_y)>90*60*60) ERRORMSG();

      coord_NE_x=LONSUBLL(coord_NE_x);
      if (fabs(coord_NE_y)>90*60*60) ERRORMSG();

      coord_SE_x=LONSUBLL(coord_SE_x);
      if (fabs(coord_SE_y)>90*60*60) ERRORMSG();

      if (coord_SW_y>=coord_NW_y || coord_SE_y>=coord_NE_y) ERRORMSG();
      }

   comment->reset();

   // magic descriptor
   comment->addstring(" BOX\n");

   // data description
   comment->addstring(" description=");
   if (pnm_description!=NULL) comment->addstring(pnm_description);
   comment->addchar('\n');

   // reference coordinate system
   comment->addstring(" coordinate system=");
   switch (coord_sys)
      {
      case 0: comment->addstring("LL\n"); break; // LL=Lat/Lon=Latitude/Longitude=geographic
      case 1: comment->addstring("UTM\n"); break; // UTM=Universal Transverse Mercator
      }
   comment->addstring(" coordinate zone=");
   comment->addfloat(coord_zone);
   comment->addchar('\n');
   comment->addstring(" coordinate datum=");
   comment->addfloat(coord_datum);
   comment->addchar('\n');

   // coordinates of corners
   comment->addstring(" SW corner=");
   comment->addfloat(coord_SW_x,1);
   comment->addchar('/');
   comment->addfloat(coord_SW_y,1);
   comment->addchar(' ');
   comment->addunits(coord_units);
   comment->addchar('\n');
   comment->addstring(" NW corner=");
   comment->addfloat(coord_NW_x,1);
   comment->addchar('/');
   comment->addfloat(coord_NW_y,1);
   comment->addchar(' ');
   comment->addunits(coord_units);
   comment->addchar('\n');
   comment->addstring(" NE corner=");
   comment->addfloat(coord_NE_x,1);
   comment->addchar('/');
   comment->addfloat(coord_NE_y,1);
   comment->addchar(' ');
   comment->addunits(coord_units);
   comment->addchar('\n');
   comment->addstring(" SE corner=");
   comment->addfloat(coord_SE_x,1);
   comment->addchar('/');
   comment->addfloat(coord_SE_y,1);
   comment->addchar(' ');
   comment->addunits(coord_units);
   comment->addchar('\n');

   // spatial resolution
   comment->addstring(" cell size=");
   comment->addfloat(cell_size_x);
   comment->addchar('/');
   comment->addfloat(cell_size_y);
   comment->addchar(' ');
   comment->addunits(coord_units);
   comment->addchar('\n');
   comment->addstring(" vertical scaling=");
   comment->addfloat(vertical_scaling);
   comment->addchar(' ');
   comment->addunits(scaling_units);
   comment->addchar('\n');

   // missing value
   comment->addstring(" missing value=");
   comment->addfloat(missing_value);
   comment->addchar('\n');
   }

// UTM conversion functions
void initUTM(float r_maj,float r_min,float scale_fact,int zone);
void calcLL2UTM(float lat,float lon,float *x,float *y);
void calcUTM2LL(float x,float y,float *lat,float *lon);

// UTM zone selector
void chooseUTMzone(int zone,int datum)
   {
   if (datum<=0 || datum>12) datum=3;

   // 1=Clarke1866/NAD27 (North American Datum of 1927)
   if (datum==1) initUTM(6378206.4,6356583.8,0.9996,zone);
   // 2=WGS72 (World Geodetic System of 1972)
   else if (datum==2) initUTM(6378135.0,6356750.519915,0.9996,zone);
   // 3=WGS84 (World Geodetic System of 1984)
   else if (datum==3) initUTM(6378137.0,6356752.314245,0.9996,zone);
   // 4=GRS80/NAD83 (North American Datum of 1983)
   else if (datum==4) initUTM(6378137.0,6356752.31414,0.9996,zone);
   // 5=Sphere (with radius 6370997 meters)
   else if (datum==5) initUTM(6370997.0,6370997.0,0.9996,zone);
   // 6=Clarke1880 (Clarke spheroid of 1880)
   else if (datum==6) initUTM(6378249.145,6356514.86955,0.9996,zone);
   // 7=International1909 (GRS, Geodetic Reference System of 1909)
   else if (datum==7) initUTM(6378388.0,6356911.94613,0.9996,zone);
   // 8=International1967 (GRS, Geodetic Reference System of 1967)
   else if (datum==8) initUTM(6378157.5,6356772.2,0.9996,zone);
   // 9=WGS60 (World Geodetic System of 1960)
   else if (datum==9) initUTM(6378165.0,6356783.287,0.9996,zone);
   // 10=WGS66 (World Geodetic System of 1966)
   else if (datum==10) initUTM(6378145.0,6356759.769356,0.9996,zone);
   // 11=Bessel1841 (German ellipsoid of 1841)
   else if (datum==11) initUTM(6377397.155,6356078.96284,0.9996,zone);
   // 12=Krassovsky (East-European ellipsoid)
   else if (datum==12) initUTM(6378245.0,6356863.0188,0.9996,zone);
   else ERRORMSG();
   }

// calculate the grid parameters in geographic coordinates
// returns 1 if PNM comment contains a valid descriptor, 0 otherwise
int getPNMparamsLL(PNMcomment *comment,
                   float *coord,
                   float *cell_size,
                   float *vertical_scaling,
                   int *missing_value,
                   int *utm_zone,
                   int *utm_datum)
   {
   int line;
   char *ptr;

   PNMcomment com;
   char str[PNMcomment::MAX_STR];

   BOOLINT texflag;
   int coord_sys,coord_zone,coord_datum;
   float coord_SW_x,coord_SW_y;
   float coord_NW_x,coord_NW_y;
   float coord_NE_x,coord_NE_y;
   float coord_SE_x,coord_SE_y;
   int coord_units;
   float cell_size_x,cell_size_y;
   float extent_x,extent_y;
   int scaling_units;

   float dxxSW,dxySW,dyxSW,dyySW;
   float dxxNW,dxyNW,dyxNW,dyyNW;
   float dxxNE,dxyNE,dyxNE,dyyNE;
   float dxxSE,dxySE,dyxSE,dyySE;
   float length;

   BOOLINT check;

   ptr=comment->str();

   for (check=TRUE,line=1; *ptr!='\0' && check; line++)
      {
      com.reset();

      while (*ptr==' ') ptr++;
      while (com.addchar(*ptr++)!='\n');

      switch (line)
         {
         case 1: // magic descriptor
            if (sscanf(com.str(),"%3s",
                       str)!=1) {check=FALSE; break;}

            check=FALSE;

            if (strcmp(str,"BOX")==0 || strcmp(str,"DEM")==0)
               {
               texflag=FALSE;
               check=TRUE;
               }

            if (strcmp(str,"TEX")==0)
               {
               texflag=TRUE;
               check=TRUE;
               }
            break;
         case 2: // data description
            if (sscanf(com.str(),"%11s=",
                       str)!=1) {check=FALSE; break;}

            if (strcmp(str,"description")!=0) check=FALSE;
            break;
         case 3: // coordinate system
            if (sscanf(com.str(),"coordinate system=%s",
                       str)!=1) {check=FALSE; break;}

            if (strcmp(str,"LL")==0) coord_sys=0;
            else if (strcmp(str,"UTM")==0) coord_sys=1;
            else check=FALSE;
            break;
         case 4: // coordinate zone
            if (sscanf(com.str(),"coordinate zone=%d",
                       &coord_zone)!=1) {check=FALSE; break;}

            if (coord_sys==0 && coord_zone!=0) check=FALSE;
            if (coord_sys==1 && coord_zone==0) check=FALSE;
            if (coord_zone<-60 || coord_zone>60) check=FALSE;
            break;
         case 5: // coordinate datum
            if (sscanf(com.str(),"coordinate datum=%d",
                       &coord_datum)!=1) {check=FALSE; break;}

            if (coord_sys==0 && coord_datum!=0) check=FALSE;
            break;
         case 6: // SW corner
            if (sscanf(com.str(),"SW corner=%g/%g %s",
                       &coord_SW_x,&coord_SW_y,str)!=3) {check=FALSE; break;}

            if (strcmp(str,"radians")==0) coord_units=0;
            else if (strcmp(str,"feet")==0) coord_units=1;
            else if (strcmp(str,"meters")==0) coord_units=2;
            else if (strcmp(str,"decimeters")==0) coord_units=3;
            else if (strcmp(str,"arc-seconds")==0) coord_units=4;
            else {check=FALSE; break;}

            if (coord_units==0)
               {
               coord_SW_x*=360*60*60/(2*PI);
               coord_SW_y*=360*60*60/(2*PI);
               coord_units=4;
               }

            if (coord_units==1)
               {
               coord_SW_x*=0.3048f;
               coord_SW_y*=0.3048f;
               coord_units=2;
               }

            if (coord_units==3)
               {
               coord_SW_x*=0.1f;
               coord_SW_y*=0.1f;
               coord_units=2;
               }

            if (coord_sys==0 && coord_units!=4) check=FALSE;
            if (coord_sys==1 && coord_units!=2) check=FALSE;
            break;
         case 7: // NW corner
            if (sscanf(com.str(),"NW corner=%g/%g %s",
                       &coord_NW_x,&coord_NW_y,str)!=3) {check=FALSE; break;}

            if (strcmp(str,"radians")==0) coord_units=0;
            else if (strcmp(str,"feet")==0) coord_units=1;
            else if (strcmp(str,"meters")==0) coord_units=2;
            else if (strcmp(str,"decimeters")==0) coord_units=3;
            else if (strcmp(str,"arc-seconds")==0) coord_units=4;
            else {check=FALSE; break;}

            if (coord_units==0)
               {
               coord_NW_x*=360*60*60/(2*PI);
               coord_NW_y*=360*60*60/(2*PI);
               coord_units=4;
               }

            if (coord_units==1)
               {
               coord_NW_x*=0.3048f;
               coord_NW_y*=0.3048f;
               coord_units=2;
               }

            if (coord_units==3)
               {
               coord_NW_x*=0.1f;
               coord_NW_y*=0.1f;
               coord_units=2;
               }

            if (coord_sys==0 && coord_units!=4) check=FALSE;
            if (coord_sys==1 && coord_units!=2) check=FALSE;
            break;
         case 8: // NE corner
            if (sscanf(com.str(),"NE corner=%g/%g %s",
                       &coord_NE_x,&coord_NE_y,str)!=3) {check=FALSE; break;}

            if (strcmp(str,"radians")==0) coord_units=0;
            else if (strcmp(str,"feet")==0) coord_units=1;
            else if (strcmp(str,"meters")==0) coord_units=2;
            else if (strcmp(str,"decimeters")==0) coord_units=3;
            else if (strcmp(str,"arc-seconds")==0) coord_units=4;
            else {check=FALSE; break;}

            if (coord_units==0)
               {
               coord_NE_x*=360*60*60/(2*PI);
               coord_NE_y*=360*60*60/(2*PI);
               coord_units=4;
               }

            if (coord_units==1)
               {
               coord_NE_x*=0.3048f;
               coord_NE_y*=0.3048f;
               coord_units=2;
               }

            if (coord_units==3)
               {
               coord_NE_x*=0.1f;
               coord_NE_y*=0.1f;
               coord_units=2;
               }

            if (coord_sys==0 && coord_units!=4) check=FALSE;
            if (coord_sys==1 && coord_units!=2) check=FALSE;
            break;
         case 9: // SE corner
            if (sscanf(com.str(),"SE corner=%g/%g %s",
                       &coord_SE_x,&coord_SE_y,str)!=3) {check=FALSE; break;}

            if (strcmp(str,"radians")==0) coord_units=0;
            else if (strcmp(str,"feet")==0) coord_units=1;
            else if (strcmp(str,"meters")==0) coord_units=2;
            else if (strcmp(str,"decimeters")==0) coord_units=3;
            else if (strcmp(str,"arc-seconds")==0) coord_units=4;
            else {check=FALSE; break;}

            if (coord_units==0)
               {
               coord_SE_x*=360*60*60/(2*PI);
               coord_SE_y*=360*60*60/(2*PI);
               coord_units=4;
               }

            if (coord_units==1)
               {
               coord_SE_x*=0.3048f;
               coord_SE_y*=0.3048f;
               coord_units=2;
               }

            if (coord_units==3)
               {
               coord_SE_x*=0.1f;
               coord_SE_y*=0.1f;
               coord_units=2;
               }

            if (coord_sys==0 && coord_units!=4) check=FALSE;
            if (coord_sys==1 && coord_units!=2) check=FALSE;
            break;
         case 10: // cell size
            if (sscanf(com.str(),"cell size=%g/%g %s",
                       &cell_size_x,&cell_size_y,str)!=3) {check=FALSE; break;}

            if (strcmp(str,"radians")==0) coord_units=0;
            else if (strcmp(str,"feet")==0) coord_units=1;
            else if (strcmp(str,"meters")==0) coord_units=2;
            else if (strcmp(str,"decimeters")==0) coord_units=3;
            else if (strcmp(str,"arc-seconds")==0) coord_units=4;
            else {check=FALSE; break;}

            if (coord_units==0)
               {
               cell_size_x*=360*60*60/(2*PI);
               cell_size_y*=360*60*60/(2*PI);
               coord_units=4;
               }

            if (coord_units==1)
               {
               cell_size_x*=0.3048f;
               cell_size_y*=0.3048f;
               coord_units=2;
               }

            if (coord_units==3)
               {
               cell_size_x*=0.1f;
               cell_size_y*=0.1f;
               coord_units=2;
               }

            if (coord_sys==0 && coord_units!=4) check=FALSE;
            if (coord_sys==1 && coord_units!=2) check=FALSE;
            break;
         case 11: // vertical scaling
            if (sscanf(com.str(),"vertical scaling=%g %s",
                       vertical_scaling,str)!=2) {check=FALSE; break;}

            if (strcmp(str,"feet")==0) scaling_units=1;
            else if (strcmp(str,"meters")==0) scaling_units=2;
            else if (strcmp(str,"decimeters")==0) scaling_units=3;
            else {check=FALSE; break;}

            if (scaling_units==1)
               {
               *vertical_scaling*=0.3048f;
               scaling_units=2;
               }

            if (scaling_units==3)
               {
               *vertical_scaling*=0.1f;
               scaling_units=2;
               }
            break;
         case 12: // missing value
            if (sscanf(com.str(),"missing value=%d",
                       missing_value)!=1) check=FALSE;
            break;
         }
      }

   if (line<13 || !check) return(0);

   // x coordinates extend from -180*60*60 arc-seconds (-180 degrees) to 180*60*60 arc-seconds (180 degrees)
   // y coordinates extend from -90*60*60 arc-seconds (-90 degrees) to 90*60*60 arc-seconds (90 degrees)
   if (coord_sys==0)
      {
      coord_SW_x=LONSUBLL(coord_SW_x);
      if (fabs(coord_SW_y)>90*60*60) ERRORMSG();

      coord_NW_x=LONSUBLL(coord_NW_x);
      if (fabs(coord_NW_y)>90*60*60) ERRORMSG();

      coord_NE_x=LONSUBLL(coord_NE_x);
      if (fabs(coord_NE_y)>90*60*60) ERRORMSG();

      coord_SE_x=LONSUBLL(coord_SE_x);
      if (fabs(coord_SE_y)>90*60*60) ERRORMSG();

      if (coord_SW_y>=coord_NW_y || coord_SE_y>=coord_NE_y) ERRORMSG();

      if (utm_zone!=NULL && utm_datum!=NULL)
         {
         *utm_zone=0;
         *utm_datum=0;
         }
      }
   else if (coord_sys==1)
      {
      if (coord_SW_x>=coord_SE_x || coord_NW_x>=coord_NE_x) ERRORMSG();
      if (coord_SW_y>=coord_NW_y || coord_SE_y>=coord_NE_y) ERRORMSG();

      if (utm_zone!=NULL && utm_datum!=NULL)
         {
         *utm_zone=coord_zone;
         *utm_datum=coord_datum;
         }
      else
         {
         chooseUTMzone(coord_zone,coord_datum);

         // calculate original extent:

         extent_x=fsqrt((coord_SE_x-coord_SW_x)*(coord_SE_x-coord_SW_x)+
                        (coord_SE_y-coord_SW_y)*(coord_SE_y-coord_SW_y))+
                  fsqrt((coord_NE_x-coord_NW_x)*(coord_NE_x-coord_NW_x)+
                        (coord_NE_y-coord_NW_y)*(coord_NE_y-coord_NW_y));

         extent_y=fsqrt((coord_NW_x-coord_SW_x)*(coord_NW_x-coord_SW_x)+
                        (coord_NW_y-coord_SW_y)*(coord_NW_y-coord_SW_y))+
                  fsqrt((coord_NE_x-coord_SE_x)*(coord_NE_x-coord_SE_x)+
                        (coord_NE_y-coord_SE_y)*(coord_NE_y-coord_SE_y));

         // transform corners
         calcUTM2LL(coord_SW_x,coord_SW_y,&coord_SW_y,&coord_SW_x);
         calcUTM2LL(coord_NW_x,coord_NW_y,&coord_NW_y,&coord_NW_x);
         calcUTM2LL(coord_NE_x,coord_NE_y,&coord_NE_y,&coord_NE_x);
         calcUTM2LL(coord_SE_x,coord_SE_y,&coord_SE_y,&coord_SE_x);

         // cell size changes approximately by the same factor as the extent changes:

         cell_size_x*=(fsqrt(fsqr(LONADDLL(coord_SE_x,-coord_SW_x))+
                             (coord_SE_y-coord_SW_y)*(coord_SE_y-coord_SW_y))+
                       fsqrt(fsqr(LONADDLL(coord_NE_x,-coord_NW_x))+
                             (coord_NE_y-coord_NW_y)*(coord_NE_y-coord_NW_y)))/extent_x;

         cell_size_y*=(fsqrt(fsqr(LONSUBLL(coord_NW_x,coord_SW_x))+
                             (coord_NW_y-coord_SW_y)*(coord_NW_y-coord_SW_y))+
                       fsqrt(fsqr(LONSUBLL(coord_NE_x,coord_SE_x))+
                             (coord_NE_y-coord_SE_y)*(coord_NE_y-coord_SE_y)))/extent_y;

         coord_zone=coord_datum=0;
         coord_units=4;
         coord_sys=0;
         }
      }

   // textures are usually provided using a cell centric data representation
   // so we need to shrink them by 0.5 texels on each side
   // to convert from a cell centric to a corner centric data representation
   if (texflag)
      if (coord_sys==1)
         {
         // SW corner:

         dxxSW=coord_SE_x-coord_SW_x;
         dxySW=coord_SE_y-coord_SW_y;

         if ((length=fsqrt(dxxSW*dxxSW+dxySW*dxySW))==0.0f) ERRORMSG();
         dxxSW/=length;
         dxySW/=length;

         dyxSW=coord_NW_x-coord_SW_x;
         dyySW=coord_NW_y-coord_SW_y;

         if ((length=fsqrt(dyxSW*dyxSW+dyySW*dyySW))==0.0f) ERRORMSG();
         dyxSW/=length;
         dyySW/=length;

         // NW corner:

         dxxNW=coord_NE_x-coord_NW_x;
         dxyNW=coord_NE_y-coord_NW_y;

         if ((length=fsqrt(dxxNW*dxxNW+dxyNW*dxyNW))==0.0f) ERRORMSG();
         dxxNW/=length;
         dxyNW/=length;

         dyxNW=coord_SW_x-coord_NW_x;
         dyyNW=coord_SW_y-coord_NW_y;

         if ((length=fsqrt(dyxNW*dyxNW+dyyNW*dyyNW))==0.0f) ERRORMSG();
         dyxNW/=length;
         dyyNW/=length;

         // NE corner:

         dxxNE=coord_NW_x-coord_NE_x;
         dxyNE=coord_NW_y-coord_NE_y;

         if ((length=fsqrt(dxxNE*dxxNE+dxyNE*dxyNE))==0.0f) ERRORMSG();
         dxxNE/=length;
         dxyNE/=length;

         dyxNE=coord_SE_x-coord_NE_x;
         dyyNE=coord_SE_y-coord_NE_y;

         if ((length=fsqrt(dyxNE*dyxNE+dyyNE*dyyNE))==0.0f) ERRORMSG();
         dyxNE/=length;
         dyyNE/=length;

         // SE corner:

         dxxSE=coord_SW_x-coord_SE_x;
         dxySE=coord_SW_y-coord_SE_y;

         if ((length=fsqrt(dxxSE*dxxSE+dxySE*dxySE))==0.0f) ERRORMSG();
         dxxSE/=length;
         dxySE/=length;

         dyxSE=coord_NE_x-coord_SE_x;
         dyySE=coord_NE_y-coord_SE_y;

         if ((length=fsqrt(dyxSE*dyxSE+dyySE*dyySE))==0.0f) ERRORMSG();
         dyxSE/=length;
         dyySE/=length;

         // shrink by 0.5 texel:

         coord_SW_x+=0.5f*(cell_size_x*dxxSW+cell_size_y*dyxSW);
         coord_SW_y+=0.5f*(cell_size_x*dxySW+cell_size_y*dyySW);

         coord_NW_x+=0.5f*(cell_size_x*dxxNW+cell_size_y*dyxNW);
         coord_NW_y+=0.5f*(cell_size_x*dxyNW+cell_size_y*dyyNW);

         coord_NE_x+=0.5f*(cell_size_x*dxxNE+cell_size_y*dyxNE);
         coord_NE_y+=0.5f*(cell_size_x*dxyNE+cell_size_y*dyyNE);

         coord_SE_x+=0.5f*(cell_size_x*dxxSE+cell_size_y*dyxSE);
         coord_SE_y+=0.5f*(cell_size_x*dxySE+cell_size_y*dyySE);
         }
      else
         {
         // SW corner:

         dxxSW=LONADDLL(coord_SE_x,-coord_SW_x);
         dxySW=coord_SE_y-coord_SW_y;

         if ((length=fsqrt(dxxSW*dxxSW+dxySW*dxySW))==0.0f) ERRORMSG();
         dxxSW/=length;
         dxySW/=length;

         dyxSW=LONSUBLL(coord_NW_x,coord_SW_x);
         dyySW=coord_NW_y-coord_SW_y;

         if ((length=fsqrt(dyxSW*dyxSW+dyySW*dyySW))==0.0f) ERRORMSG();
         dyxSW/=length;
         dyySW/=length;

         // NW corner:

         dxxNW=LONADDLL(coord_NE_x,-coord_NW_x);
         dxyNW=coord_NE_y-coord_NW_y;

         if ((length=fsqrt(dxxNW*dxxNW+dxyNW*dxyNW))==0.0f) ERRORMSG();
         dxxNW/=length;
         dxyNW/=length;

         dyxNW=LONSUBLL(coord_SW_x,coord_NW_x);
         dyyNW=coord_SW_y-coord_NW_y;

         if ((length=fsqrt(dyxNW*dyxNW+dyyNW*dyyNW))==0.0f) ERRORMSG();
         dyxNW/=length;
         dyyNW/=length;

         // NE corner:

         dxxNE=-LONADDLL(coord_NE_x,-coord_NW_x);
         dxyNE=coord_NW_y-coord_NE_y;

         if ((length=fsqrt(dxxNE*dxxNE+dxyNE*dxyNE))==0.0f) ERRORMSG();
         dxxNE/=length;
         dxyNE/=length;

         dyxNE=LONSUBLL(coord_SE_x,coord_NE_x);
         dyyNE=coord_SE_y-coord_NE_y;

         if ((length=fsqrt(dyxNE*dyxNE+dyyNE*dyyNE))==0.0f) ERRORMSG();
         dyxNE/=length;
         dyyNE/=length;

         // SE corner:

         dxxSE=-LONADDLL(coord_SE_x,-coord_SW_x);
         dxySE=coord_SW_y-coord_SE_y;

         if ((length=fsqrt(dxxSE*dxxSE+dxySE*dxySE))==0.0f) ERRORMSG();
         dxxSE/=length;
         dxySE/=length;

         dyxSE=LONSUBLL(coord_NE_x,coord_SE_x);
         dyySE=coord_NE_y-coord_SE_y;

         if ((length=fsqrt(dyxSE*dyxSE+dyySE*dyySE))==0.0f) ERRORMSG();
         dyxSE/=length;
         dyySE/=length;

         // shrink by 0.5 texel:

         coord_SW_x=LONSUBLL(coord_SW_x,-0.5f*(cell_size_x*dxxSW+cell_size_y*dyxSW));
         coord_SW_y+=0.5f*(cell_size_x*dxySW+cell_size_y*dyySW);

         coord_NW_x=LONSUBLL(coord_NW_x,-0.5f*(cell_size_x*dxxNW+cell_size_y*dyxNW));
         coord_NW_y+=0.5f*(cell_size_x*dxyNW+cell_size_y*dyyNW);

         coord_NE_x=LONSUBLL(coord_NE_x,-0.5f*(cell_size_x*dxxNE+cell_size_y*dyxNE));
         coord_NE_y+=0.5f*(cell_size_x*dxyNE+cell_size_y*dyyNE);

         coord_SE_x=LONSUBLL(coord_SE_x,-0.5f*(cell_size_x*dxxSE+cell_size_y*dyxSE));
         coord_SE_y+=0.5f*(cell_size_x*dxySE+cell_size_y*dyySE);
         }

   coord[0]=coord_SW_x;
   coord[1]=coord_SW_y;
   coord[2]=coord_NW_x;
   coord[3]=coord_NW_y;
   coord[4]=coord_NE_x;
   coord[5]=coord_NE_y;
   coord[6]=coord_SE_x;
   coord[7]=coord_SE_y;

   cell_size[0]=cell_size_x;
   cell_size[1]=cell_size_y;

   return(1);
   }

// transform Lat/Lon to UTM
// input in arc-seconds, output in meters
// latitude in [-90*60*60,90*60*60] arc-seconds
// longitude in [-180*60*60,180*60*60] arc-seconds
// 1 arc-second equals about 30 meters
void LL2UTM(float lat,float lon,
            int zone,int datum,
            float *x,float *y)
   {
   chooseUTMzone(zone,datum);
   calcLL2UTM(lat,lon,x,y);
   }

// transform UTM to Lat/Lon
// input in meters, output in arc-seconds
// latitude in [-90*60*60,90*60*60] arc-seconds
// longitude in [-180*60*60,180*60*60] arc-seconds
// 1 arc-second equals about 30 meters
void UTM2LL(float x,float y,
            int zone,int datum,
            float *lat,float *lon)
   {
   chooseUTMzone(zone,datum);
   calcUTM2LL(x,y,lat,lon);
   }

/*******************************************************************************
NAME:                           UNIVERSAL TRANSVERSE MERCATOR

PURPOSE:        Transforms input longitude and latitude to Easting and
                Northing for the Universal Transverse Mercator projection.
                The longitude and latitude must be in arc-seconds. The
                Easting and Northing values will be returned in meters.

                Transforms input Easting and Northing to latitude and
                longitude for the inverse Universal Transverse Mercator
                projection. The Easting and Northing must be in meters.
                The latitude and longitude values will be returned in
                arc-seconds.

PROGRAMMER              DATE
----------              ----
D. Steinwand, EROS      Nov, 1991
T. Mittan               Mar, 1993
S. Nelson               Feb, 1995

ALGORITHM REFERENCES:

1.  Snyder, John P., "Map Projections--A Working Manual", U.S. Geological
    Survey Professional Paper 1395 (Supersedes USGS Bulletin 1532), United
    State Government Printing Office, Washington D.C., 1987.

2.  Snyder, John P. and Voxland, Philip M., "An Album of Map Projections",
    U.S. Geological Survey Professional Paper 1453 , United State Government
    Printing Office, Washington D.C., 1989.
*******************************************************************************/

double r_major;        // major axis
double r_minor;        // minor axis
double scale_factor;   // scale factor
double lon_center;     // center longitude
double e0,e1,e2,e3;    // eccentricity constants
double e,es,esp;       // eccentricity constants
double false_northing; // y offset in meters
double false_easting;  // x offset in meters

// initialize the Universal Transverse Mercator (UTM) projection
void initUTM(float r_maj,float r_min, // major and minor axis
             float scale_fact,        // scale factor
             int zone)                // zone number
   {
   if (zone==0 || abs(zone)>60) ERRORMSG();

   r_major=r_maj;
   r_minor=r_min;

   scale_factor=scale_fact;

   lon_center=(6*abs(zone)-183)*PI/180.0;

   false_easting=500000.0;
   false_northing=(zone<0)?10000000.0:0.0;

   es=1.0-r_minor*r_minor/(r_major*r_major);
   e=sqrt(es);

   e0=1.0-0.25*es*(1.0+0.0625*es*(3.0+1.25*es));
   e1=0.375*es*(1.0+0.25*es*(1.0+0.46875*es));
   e2=0.05859375*es*es*(1.0+0.75*es);
   e3=35.0/3072.0*es*es*es;

   esp=es/(1.0-es);
   }

// calculate the UTM equations
void calcLL2UTM(float lat,float lon, // geographic input (Lat/Lon)
                float *x,float *y)   // output UTM coordinates (Easting and Northing)
   {
   double delta_lon;       // delta longitude
   double sin_phi,cos_phi; // sine and cosine values
   double a,as,t,ts,n,m,b; // temporary values

   lon*=2*PI/(360*60*60);
   lat*=2*PI/(360*60*60);

   delta_lon=lon-lon_center;
   while (delta_lon<-PI) delta_lon+=PI;
   while (delta_lon>PI) delta_lon-=PI;

   sin_phi=sin(lat);
   cos_phi=cos(lat);

   a=cos_phi*delta_lon;
   as=a*a;

   t=tan(lat);
   ts=t*t;

   n=r_major/sqrt(1.0-es*sin_phi*sin_phi);
   m=r_major*(e0*lat-e1*sin(2.0*lat)+e2*sin(4.0*lat)-e3*sin(6.0*lat));

   b=esp*cos_phi*cos_phi;

   // UTM Easting in meters
   *x=scale_factor*n*a*(1.0+as/6.0*(1.0-ts+b+as/20.0*
      (5.0-18.0*ts+ts*ts+72.0*b-58.0*esp)))+false_easting;

   // UTM Northing in meters
   *y=scale_factor*(m+n*t*(as*(0.5+as/24.0*
      (5.0-ts+9.0*b+4.0*b*b+as/30.0*(61.0-58.0*ts+
      ts*ts+600.0*b-330.0*esp)))))+false_northing;
   }

// calculate the inverse UTM equations
void calcUTM2LL(float x,float y,       // input UTM coordinates (Easting and Northing)
                float *lat,float *lon) // geographic output (Lat/Lon)
   {
   int i;

   double con,phi;                 // temporary angles
   double delta_phi;               // difference between longitudes
   double sin_phi,cos_phi,tan_phi; // sine, cosine and tangent values
   double a,as,t,ts,n,r,b,bs;      // temporary variables

   x-=false_easting;
   y-=false_northing;

   con=y/(r_major*scale_factor);
   phi=con;

   for (i=0; i<6; i++)
      {
      delta_phi=((con+e1*sin(2.0*phi)-e2*sin(4.0*phi)+e3*sin(6.0*phi))/e0)-phi;
      phi+=delta_phi;

      if (fabs(delta_phi)<1E-10f) break;
      }

   sin_phi=sin(phi);
   cos_phi=cos(phi);
   tan_phi=tan(phi);

   a=esp*cos_phi*cos_phi;
   as=a*a;

   t=tan_phi*tan_phi;
   ts=t*t;

   con=1.0-es*sin_phi*sin_phi;
   n=r_major/sqrt(con);
   r=n*(1.0-es)/con;

   b=x/(n*scale_factor);
   bs=b*b;

   // UTM latitude extends from -90 to 90 degrees
   *lat=(phi-(n*tan_phi*bs/r)*(0.5-bs/24.0*(5.0+3.0*t+
        10.0*a-4.0*as-9.0*esp-bs/30.0*(61.0+90.0*t+
        298.0*a+45.0*ts-252.0*esp-3.0*as))))*360*60*60/(2*PI);

   // UTM longitude extends from -180 to 180 degrees
   *lon=LONSUBLL((lon_center+(b*(1.0-bs/6.0*(1.0+2.0*t+
        a-bs/20.0*(5.0-2.0*a+28.0*t-3.0*as+8.0*esp+
        24.0*ts)))/cos_phi))*360*60*60/(2*PI));
   }

}
