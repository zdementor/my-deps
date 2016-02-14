// (c) by Stefan Roettger

#ifndef DATACACHE_H
#define DATACACHE_H

/* This class caches data that is arriving from a server and stores the
   downloaded data persistently on disk. The data transfer is delegated
   to an external library like libcurl. */

#include "minibase.h"

#include "database.hpp"
#include "miniload.hpp"
#include "datacloud.hpp"

// one element of file information
struct fileinfoelem
   {
   char *filename;
   unsigned int hash;

   BOOLINT isavailable,islocal;

   BOOLINT istile,istexture;
   float minvalue,maxvalue;
   int width,height;

   fileinfoelem *next;
   };

// a cache for terrain data tile sets and related information
class datacache
   {
   public:

   //! default constructor
   datacache(miniload *terrain);

   //! destructor
   ~datacache();

   //! cloud getter
   datacloud *getcloud() {return(CLOUD);}

   //! set callback for requesting tiles
   void setloader(void (*request)(char *file,int istexture,databuf *buf,void *data),void *data,
                  int paging=0,
                  float pfarp=0.0f,
                  float prange=0.0f,int pbasesize=0,
                  int plazyness=0,int pupdate=0,
                  int expire=0);

   //! set optional callback for requesting tiles from a server
   void setreceiver(void (*receive)(char *src_url,char *src_id,char *src_file,char *dst_file,int background,void *data),void *data,
                    int (*check)(char *src_url,char *src_id,char *src_file,void *data));

   //! set remote url
   void setremoteurl(char *url);

   //! set remote world id
   void setremoteid(char *id);

   //! set local path to file cache
   void setlocalpath(char *path);

   //! set size of hash table
   void sethashsize(int size);

   //! get file
   char *getfile(char *src_file);

   //! set name of tileset file
   void settilesetfile(char *filename);

   //! set name of vtb ini file
   void setvtbinifile(char *filename);

   //! set name of startup file
   void setstartupfile(char *filename);

   //! load persistent startup file
   void load();

   //! save persistent startup file
   void save();

   //! getters for tileset info
   BOOLINT hasinfo() {return(HAS_INFO);}
   int getinfo_tilesx() {return(INFO_TILESX);}
   int getinfo_tilesy() {return(INFO_TILESY);}
   float getinfo_centerx() {return(INFO_CENTERX);}
   float getinfo_centery() {return(INFO_CENTERY);}
   float getinfo_sizex() {return(INFO_SIZEX);}
   float getinfo_sizey() {return(INFO_SIZEY);}
   int getinfo_maxtexsize() {return(INFO_MAXTEXSIZE);}

   //! getters for vtb ini file
   BOOLINT hasini() {return(HAS_INI);}
   int getini_tilesx() {return(INI_TILESX);}
   int getini_tilesy() {return(INI_TILESY);}
   float getini_centerx() {return(INI_CENTERX);}
   float getini_centery() {return(INI_CENTERY);}
   float getini_sizex() {return(INI_SIZEX);}
   float getini_sizey() {return(INI_SIZEY);}
   int getini_maxtexsize() {return(INI_MAXTEXSIZE);}

   //! configuring
   void configure_dontfree(int dontfree=0); // free memory chunks returned by callbacks
   void configure_netthreads(int netthreads=10); // number of background threads for networking

   protected:

   void loadtilesetinfo();
   void loadvtbini();

   void myrequest(unsigned char *mapfile,databuf *map,int istexture,int background);
   int mycheck(unsigned char *mapfile,int istexture);
   int myinquiry(int col,int row,unsigned char *mapfile,int hlod,float *minvalue,float *maxvalue);
   void myquery(int col,int row,unsigned char *texfile,int tlod,int *tsizex,int *tsizey);

   char *sourcefilename(char *id,char *filename);
   char *localfilename(char *filename);
   BOOLINT localfilecheck(char *filename);

   void insertfilename(char *filename,BOOLINT isavailable,BOOLINT islocal,BOOLINT istile,BOOLINT istexture,float minvalue=1.0f,float maxvalue=0.0f,int width=0,int height=0,BOOLINT tailinsert=FALSE);
   fileinfoelem *checkfilename(char *filename);

   char *concat(char *str1,char *str2);
   unsigned int hashsum(char *str);

   private:

   datacloud *CLOUD;

   void (*REQUEST_CALLBACK)(char *file,int istexture,databuf *buf,void *data);
   void *REQUEST_DATA;

   void (*RECEIVE_CALLBACK)(char *src_url,char *src_id,char *src_file,char *dst_file,int background,void *data);
   int (*CHECK_CALLBACK)(char *src_url,char *src_id,char *src_file,void *data);
   void *RECEIVE_DATA;

   char *RURL,*RID;
   char *LPATH;

   BOOLINT LOCAL;

   fileinfoelem **HASHTABLE;
   int HASHSIZE;

   char *TILESETFILE;
   char *VTBINIFILE;
   char *STARTUPFILE;

   BOOLINT HAS_INFO;
   int INFO_TILESX;
   int INFO_TILESY;
   float INFO_CENTERX;
   float INFO_CENTERY;
   float INFO_SIZEX;
   float INFO_SIZEY;
   int INFO_MAXTEXSIZE;

   BOOLINT HAS_INI;
   int INI_TILESX;
   int INI_TILESY;
   float INI_CENTERX;
   float INI_CENTERY;
   float INI_SIZEX;
   float INI_SIZEY;
   int INI_MAXTEXSIZE;

   int CONFIGURE_DONTFREE;
   int CONFIGURE_NETTHREADS;

   static void mystaticrequest(unsigned char *mapfile,databuf *map,int istexture,int background,void *data);
   static int mystaticcheck(unsigned char *mapfile,int istexture,void *data);
   static int mystaticinquiry(int col,int row,unsigned char *mapfile,int hlod,void *data,float *minvalue,float *maxvalue);
   static void mystaticquery(int col,int row,unsigned char *texfile,int tlod,void *data,int *tsizex,int *tsizey);
   };

#endif
