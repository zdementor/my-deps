// (c) by Stefan Roettger

#ifndef PNMBASE_H
#define PNMBASE_H

namespace pnm {

//! helper class to construct a PNM comment string
class PNMcomment
   {
   public:

   static const int MAX_STR;
   static const int MAX_COM;

   PNMcomment();
   ~PNMcomment();

   void reset();

   char addchar(char ch);
   void addstring(char *str);
   void addfloat(float v,int leading=0);
   void addunits(int units);

   char *str() {return(com);}

   private:

   char *com;
   int c,maxc;
   };

//! check a file
int checkfile(const char *filename);

//! write a RAW file
void writefile(const char *filename,unsigned char *data,int bytes);

//! read a RAW file
unsigned char *readfile(const char *filename,int *bytes);

//! write a PNM image with optional comment to a file
void writePNMimage(const char *pnmfilename,
                   unsigned char *image,
                   int width,int height,int components,
                   PNMcomment *comment=0);

//! read a PNM image with optional comment from a file or memory
unsigned char *readPNMimage(unsigned char *pnmimage,
                            int *width,int *height,int *components,
                            int loaded=0,unsigned char **ptr=0,
                            PNMcomment *comment=0);

//! read a PNM image with optional comment from a file
unsigned char *readPNMfile(const char *pnmfilename,
                           int *width,int *height,int *components,
                           PNMcomment *comment=0);

//! write a PVM volume
void writePVMvolume(const char *filename,unsigned char *volume,
                    int width,int height,int depth,int components);

//! read a PVM volume
unsigned char *readPVMvolume(const char *filename,
                             int *width,int *height,int *depth,int *components);

//! put a geographic grid descriptor into a PNM comment
void putPNMparams(PNMcomment *comment, // output PNM comment
                  char *pnm_description=0, // data description
                  int coord_sys=0, // reference coordinate system: 0=LL 1=UTM
                  int coord_zone=0, // UTM zone: +- 1-60 0=LL (negative zones on southern hemisphere)
                  int coord_datum=0, // UTM datum: 1=NAD27 2=WGS72 3=WGS84 4=GRS80 5=Sphere 0=LL
                  int coord_units=4, // coordinate units: 0=radians 1=feet 2=meters 3=decimeters 4=arc-seconds
                  float coord_SW_x=-1800.0f,float coord_SW_y=-1800.0f, // SW corner in coordinate units
                  float coord_NW_x=-1800.0f,float coord_NW_y=1800.0f, // NW corner in coordinate units
                  float coord_NE_x=1800.0f,float coord_NE_y=1800.0f, // NE corner in coordinate units
                  float coord_SE_x=1800.0f,float coord_SE_y=-1800.0f, // SE corner in coordinate units
                  float cell_size_x=3.0f,float cell_size_y=3.0f, // cell size in coordinate units
                  int scaling_units=2, // elevation units: 1=feet 2=meters 3=decimeters
                  float vertical_scaling=1.0f, // elevation scaling factor
                  int missing_value=-9999); // missing data value

//! calculate the grid parameters in geographic coordinates
int getPNMparamsLL(PNMcomment *comment, // input PNM comment
                   float *coord, // output corners in arc-seconds
                   float *cell_size, // output cell size in arc-seconds (mean approximate value)
                   float *vertical_scaling, // output elevation scaling factor (to yield meters)
                   int *missing_value, // output missing data value
                   int *utm_zone=0, // UTM zone: +- 1-60 0=LL
                   int *utm_datum=0); // UTM datum

//! transform Lat/Lon to UTM
void LL2UTM(float lat,float lon, // geographic input (Lat/Lon)
            int zone,int datum, // UTM zone and datum of output coordinates
            float *x,float *y); // output UTM coordinates (Easting and Northing)

//! transform UTM to Lat/Lon
void UTM2LL(float x,float y, // input UTM coordinates (Easting and Northing)
            int zone,int datum, // UTM zone and datum of input coordinates
            float *lat,float *lon); // geographic output (Lat/Lon)

// longitude arithmetic (all calculations in arc-seconds):

//! add longitudes
inline float LONADDLL(float a,float b=0)
   {
   float lon=a+b;
   while (lon<0) lon+=360*60*60;
   while (lon>360*60*60) lon-=360*60*60;
   return(lon);
   }

//! subtract longitudes
inline float LONSUBLL(float a,float b=0)
   {
   float diff=a-b;
   while (diff<-180*60*60) diff+=360*60*60;
   while (diff>180*60*60) diff-=360*60*60;
   return(diff);
   }

//! return leftmost longitude
inline float LONLEFTLL(float a,float b)
   {
   if (LONSUBLL(a,b)<0) return(a);
   else return(b);
   }

//! return rightmost longitude
inline float LONRIGHTLL(float a,float b)
   {
   if (LONSUBLL(a,b)>0) return(a);
   else return(b);
   }

//! linear interpolation of longitudes
inline float LONLERPLL(float a,float b,float lerp=0.5f)
   {
   float diff=LONADDLL(b,-a);
   return(LONSUBLL(a,-lerp*diff));
   }

//! average longitudes
inline float LONMEANLL(float a,float b,float weight=0.5f)
   {
   if (LONSUBLL(a,b)<0) return(LONLERPLL(a,b,weight));
   else return(LONLERPLL(b,a,1.0f-weight));
   }

}

using namespace pnm;

#endif
