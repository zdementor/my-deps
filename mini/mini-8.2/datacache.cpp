// (c) by Stefan Roettger

#include "minibase.h"

#include "pnmbase.h"

#include "datacache.hpp"

// default constructor
datacache::datacache(miniload *terrain)
   {
   CLOUD=new datacloud(terrain);

   REQUEST_CALLBACK=NULL;
   REQUEST_DATA=NULL;

   RECEIVE_CALLBACK=NULL;
   CHECK_CALLBACK=NULL;
   RECEIVE_DATA=NULL;

   RURL=RID=NULL;
   LPATH=NULL;

   LOCAL=TRUE;

   HASHSIZE=100000;
   HASHTABLE=NULL;

   TILESETFILE=NULL;
   VTBINIFILE=NULL;
   STARTUPFILE=NULL;

   HAS_INFO=FALSE;

   INFO_TILESX=0;
   INFO_TILESY=0;
   INFO_CENTERX=0.0f;
   INFO_CENTERY=0.0f;
   INFO_SIZEX=0.0f;
   INFO_SIZEY=0.0f;

   HAS_INI=FALSE;

   INI_TILESX=0;
   INI_TILESY=0;
   INI_CENTERX=0.0f;
   INI_CENTERY=0.0f;
   INI_SIZEX=0.0f;
   INI_SIZEY=0.0f;

   CONFIGURE_DONTFREE=0;
   CONFIGURE_NETTHREADS=10;

   CLOUD->configure_dontfree(0);
   CLOUD->configure_autolockio(0);
   }

// destructor
datacache::~datacache()
   {
   int i;

   fileinfoelem *info,*next;

   delete CLOUD;

   save();

   if (RURL!=NULL) free(RURL);
   if (RID!=NULL) free(RID);
   if (LPATH!=NULL) free(LPATH);

   if (HASHTABLE!=NULL)
      for (i=0; i<HASHSIZE; i++)
         {
         info=HASHTABLE[i];

         while (info!=NULL)
            {
            next=info->next;

            free(info->filename);
            delete info;

            info=next;
            }
         }

   if (TILESETFILE!=NULL) free(TILESETFILE);
   if (VTBINIFILE!=NULL) free(VTBINIFILE);
   if (STARTUPFILE!=NULL) free(STARTUPFILE);
   }

// set callback for requesting tiles
void datacache::setloader(void (*request)(char *file,int istexture,databuf *buf,void *data),void *data,
                          int paging,
                          float pfarp,
                          float prange,int pbasesize,
                          int plazyness,int pupdate,
                          int expire)
   {
   if (request==NULL) ERRORMSG();

   // propagate request and check callbacks to the wrapped datacloud object
   CLOUD->setloader(mystaticrequest,this,mystaticcheck,
                    paging,pfarp,prange,pbasesize,plazyness,pupdate,expire);

   // propagate optional inquiry callback
   CLOUD->setinquiry(mystaticinquiry,this);

   // propagate optional query callback
   CLOUD->setquery(mystaticquery,this);

   REQUEST_CALLBACK=request;
   REQUEST_DATA=data;
   }

// set callback for requesting tiles from a server
void datacache::setreceiver(void (*receive)(char *src_url,char *src_id,char *src_file,char *dst_file,int background,void *data),void *data,
                            int (*check)(char *src_url,char *src_id,char *src_file,void *data))
   {
   RECEIVE_CALLBACK=receive;
   CHECK_CALLBACK=check;
   RECEIVE_DATA=data;
   }

// set remote url
void datacache::setremoteurl(char *url)
   {
   if (RURL!=NULL) free(RURL);

   if (url==NULL) RURL=NULL;
   else RURL=strdup(url);

   LOCAL=TRUE;

   if (RURL!=NULL)
      {
      if (strstr(RURL,"http://")!=NULL) LOCAL=FALSE;
      if (strstr(RURL,"ftp://")!=NULL) LOCAL=FALSE;
      }

   if (LOCAL) CLOUD->setmulti(1);
   else CLOUD->setmulti(CONFIGURE_NETTHREADS);
   }

// set remote world id
void datacache::setremoteid(char *id)
   {
   if (RID!=NULL) free(RID);

   if (id==NULL) RID=NULL;
   else RID=strdup(id);
   }

// set local path to file cache
void datacache::setlocalpath(char *path)
   {
   if (LPATH!=NULL) free(LPATH);

   if (path==NULL) LPATH=NULL;
   else LPATH=strdup(path);
   }

// set size of hash table
void datacache::sethashsize(int size)
   {
   if (HASHSIZE<1) ERRORMSG();

   if (HASHTABLE==NULL) HASHSIZE=size;
   }

// get file
char *datacache::getfile(char *src_file)
   {
   char *filename;
   char *localname;

   fileinfoelem *info;

   BOOLINT isavailable=FALSE;
   BOOLINT islocal=FALSE;

   filename=sourcefilename(RID,src_file);

   if (!LOCAL)
      {
      if (CHECK_CALLBACK==NULL || RECEIVE_CALLBACK==NULL) ERRORMSG();

      info=checkfilename(filename);

      if (info!=NULL)
         {
         isavailable=info->isavailable;
         islocal=info->islocal;
         }
      else
         {
         if (localfilecheck(filename)) isavailable=islocal=TRUE;
         else if (CHECK_CALLBACK(RURL,RID,src_file,REQUEST_DATA)!=0) isavailable=TRUE;
         }

      if (isavailable && !islocal)
         {
         localname=localfilename(filename);
         RECEIVE_CALLBACK(RURL,RID,src_file,localname,0,REQUEST_DATA);
         free(localname);
         islocal=TRUE;
         }
      }
   else isavailable=islocal=localfilecheck(filename);

   insertfilename(filename,isavailable,islocal,FALSE,FALSE);

   localname=localfilename(filename);
   free(filename);

   if (islocal) return(localname);

   free(localname);

   return(NULL);
   }

// set name of tileset file
void datacache::settilesetfile(char *filename)
   {
   if (TILESETFILE!=NULL) free(TILESETFILE);

   if (filename==NULL) TILESETFILE=NULL;
   else TILESETFILE=strdup(filename);
   }

// set name of vtb ini file
void datacache::setvtbinifile(char *filename)
   {
   if (VTBINIFILE!=NULL) free(VTBINIFILE);

   if (filename==NULL) VTBINIFILE=NULL;
   else VTBINIFILE=strdup(filename);
   }

// set name of startup file
void datacache::setstartupfile(char *filename)
   {
   if (STARTUPFILE!=NULL) free(STARTUPFILE);

   if (filename==NULL) STARTUPFILE=NULL;
   else STARTUPFILE=strdup(filename);
   }

// load tileset info file
void datacache::loadtilesetinfo()
   {
   FILE *file;

   HAS_INFO=FALSE;

   if (TILESETFILE==NULL) return;

   char *tilesetname=getfile(TILESETFILE);

   if (tilesetname!=NULL)
      {
      HAS_INFO=TRUE;

      if ((file=fopen(tilesetname,"rb"))==NULL) ERRORMSG();

      if (fscanf(file,"tilesx=%d\n",&INFO_TILESX)!=1) HAS_INFO=FALSE;
      if (fscanf(file,"tilesy=%d\n",&INFO_TILESY)!=1) HAS_INFO=FALSE;
      if (fscanf(file,"centerx=%g arc-seconds\n",&INFO_CENTERX)!=1) HAS_INFO=FALSE;
      if (fscanf(file,"centery=%g arc-seconds\n",&INFO_CENTERY)!=1) HAS_INFO=FALSE;
      if (fscanf(file,"sizex=%g arc-seconds\n",&INFO_SIZEX)!=1) HAS_INFO=FALSE;
      if (fscanf(file,"sizey=%g arc-seconds\n",&INFO_SIZEY)!=1) HAS_INFO=FALSE;
      if (fscanf(file,"maxtexsize=%d\n",&INFO_MAXTEXSIZE)!=1) HAS_INFO=FALSE;

      fclose(file);

      free(tilesetname);
      }
   }

// load vtb ini file
void datacache::loadvtbini()
   {
   FILE *file;

   float left,right,bottom,top;

   HAS_INI=FALSE;

   if (VTBINIFILE==NULL) return;

   char *vtbininame=getfile(VTBINIFILE);

   if (vtbininame!=NULL)
      {
      HAS_INI=TRUE;

      if ((file=fopen(vtbininame,"rb"))==NULL) ERRORMSG();

      if (fscanf(file,"[TilesetDescription]\nColumns=%d\n",&INI_TILESX)!=1) HAS_INI=FALSE;
      if (fscanf(file,"Rows=%d\n",&INI_TILESY)!=1) HAS_INI=FALSE;
      if (fscanf(file,"LOD0_Size=%d\n",&INI_MAXTEXSIZE)!=1) HAS_INI=FALSE;

      if (fscanf(file,"Extent_Left=%g\n",&left)!=1) HAS_INI=FALSE;
      if (fscanf(file,"Extent_Right=%g\n",&right)!=1) HAS_INI=FALSE;
      if (fscanf(file,"Extent_Bottom=%g\n",&bottom)!=1) HAS_INI=FALSE;
      if (fscanf(file,"Extent_Top=%g\n",&top)!=1) HAS_INI=FALSE;

      if (HAS_INI)
         {
         INI_CENTERX=(left+right)/2.0f;
         INI_CENTERY=(bottom+top)/2.0f;
         INI_SIZEX=right-left;
         INI_SIZEY=top-bottom;
         }

      fclose(file);

      free(vtbininame);
      }
   }

// load persistent startup file
void datacache::load()
   {
   int c;

   FILE *file;
   char ch;

   char *name;
   int size;

   int v1,v2,v3,v4;
   float minvalue,maxvalue;
   int width,height;

   char *filename;
   char *localname;

   BOOLINT remote=FALSE;

   loadtilesetinfo();
   loadvtbini();

   if (STARTUPFILE==NULL) return;

   filename=sourcefilename(RID,STARTUPFILE);
   localname=localfilename(filename);
   free(filename);

   if ((file=fopen(localname,"rb"))==NULL)
      if (LOCAL)
         {
         free(localname);
         return;
         }
      else
         {
         if (CHECK_CALLBACK==NULL || RECEIVE_CALLBACK==NULL) ERRORMSG();

         if (CHECK_CALLBACK(RURL,RID,STARTUPFILE,REQUEST_DATA)==0)
            {
            free(localname);
            return;
            }
         else
            {
            RECEIVE_CALLBACK(RURL,RID,STARTUPFILE,localname,0,REQUEST_DATA);

            if ((file=fopen(localname,"rb"))==NULL)
               {
               free(localname);
               return;
               }

            remote=TRUE;
            }
         }

   free(localname);

   size=1000;

   if ((name=(char *)malloc(size))==NULL) ERRORMSG();

   ch=getc(file);

   while (ch!=EOF)
      {
      c=0;

      while (ch!=' ' && ch!=EOF)
         {
         if (c>size-2)
            {
            size*=2;
            if ((name=(char *)realloc(name,size))==NULL) ERRORMSG();
            }

         name[c++]=ch;

         ch=getc(file);
         }

      name[c]='\0';

      fscanf(file,"%d %d %d %d %g %g %d %d\n",&v1,&v2,&v3,&v4,&minvalue,&maxvalue,&width,&height);

      ch=getc(file);

      while (ch=='\n' && ch!=EOF) ch=getc(file);

      if (remote) v2=0;

      insertfilename(name,v1!=0,v2!=0,v3!=0,v4!=0,minvalue,maxvalue,width,height,TRUE);
      }

   free(name);

   fclose(file);

   if (remote) save();
   }

// save persistent startup file
void datacache::save()
   {
   int i;

   FILE *file;

   fileinfoelem *info;

   char *filename;
   char *localname;

   if (STARTUPFILE==NULL) return;

   if (HASHTABLE==NULL) return;

   filename=sourcefilename(RID,STARTUPFILE);
   localname=localfilename(filename);

   if ((file=fopen(localname,"wb"))==NULL) ERRORMSG();

   free(filename);
   free(localname);

   for (i=0; i<HASHSIZE; i++)
      {
      info=HASHTABLE[i];

      while (info!=NULL)
         {
         fprintf(file,"%s %d %d %d %d %g %g %d %d\n",
                 info->filename,
                 info->isavailable,
                 info->islocal,
                 info->istile,
                 info->istexture,
                 info->minvalue,
                 info->maxvalue,
                 info->width,
                 info->height);

         info=info->next;
         }
      }

   fclose(file);
   }

// own callbacks:

void datacache::myrequest(unsigned char *mapfile,databuf *map,int istexture,int background)
   {
   char *filename;
   char *localname;

   if (REQUEST_CALLBACK==NULL) ERRORMSG();

   filename=sourcefilename(RID,(char *)mapfile);

   if (!LOCAL)
      {
      if (RECEIVE_CALLBACK==NULL) ERRORMSG();

      if (!localfilecheck(filename))
         {
         localname=localfilename(filename);
         RECEIVE_CALLBACK(RURL,RID,(char *)mapfile,localname,background,REQUEST_DATA);
         free(localname);
         }
      }

   localname=localfilename(filename);
   CLOUD->lockio();
   REQUEST_CALLBACK(localname,istexture,map,REQUEST_DATA);
   CLOUD->unlockio();
   free(localname);

   insertfilename(filename,TRUE,TRUE,TRUE,istexture!=0);

   free(filename);
   }

int datacache::mycheck(unsigned char *mapfile,int istexture)
   {
   char *filename;

   fileinfoelem *info;

   BOOLINT isavailable=FALSE;
   BOOLINT islocal=FALSE;

   filename=sourcefilename(RID,(char *)mapfile);

   if (!LOCAL)
      {
      if (CHECK_CALLBACK==NULL) ERRORMSG();

      info=checkfilename(filename);

      if (info!=NULL)
         {
         isavailable=info->isavailable;
         islocal=info->islocal;
         }
      else
         {
         if (localfilecheck(filename)) isavailable=islocal=TRUE;
         else if (CHECK_CALLBACK(RURL,RID,(char *)mapfile,REQUEST_DATA)!=0) isavailable=TRUE;
         }
      }
   else isavailable=islocal=localfilecheck(filename);

   insertfilename(filename,isavailable,islocal,TRUE,istexture!=0);

   free(filename);

   return(isavailable?1:0);
   }

int datacache::myinquiry(int col,int row,unsigned char *mapfile,int hlod,float *minvalue,float *maxvalue)
   {
   char *filename;

   fileinfoelem *info;

   databuf map;

   mycheck(mapfile,TRUE);

   filename=sourcefilename(RID,(char *)mapfile);

   info=checkfilename(filename);

   free(filename);

   if (info==NULL) ERRORMSG();

   if (!info->isavailable) return(0);

   if (info->minvalue>info->maxvalue)
      {
      myrequest(mapfile,&map,FALSE,0);

      map.getminmax(&info->minvalue,&info->maxvalue);

      if (CONFIGURE_DONTFREE==0) map.release();
      }

   *minvalue=info->minvalue;
   *maxvalue=info->maxvalue;

   return(1);
   }

void datacache::myquery(int col,int row,unsigned char *texfile,int tlod,int *tsizex,int *tsizey)
   {
   char *filename;

   fileinfoelem *info;

   databuf map;

   mycheck(texfile,TRUE);

   filename=sourcefilename(RID,(char *)texfile);

   info=checkfilename(filename);

   free(filename);

   if (info==NULL) ERRORMSG();

   if (!info->isavailable) ERRORMSG();

   if (info->width==0 || info->height==0)
      {
      myrequest(texfile,&map,TRUE,0);

      info->width=map.xsize;
      info->height=map.ysize;

      if (CONFIGURE_DONTFREE==0) map.release();
      }

   *tsizex=info->width;
   *tsizey=info->height;
   }

// concatenate source file name
char *datacache::sourcefilename(char *id,char *filename)
   {
   char *name;

   if (filename==NULL) ERRORMSG();
   if (strlen(filename)==0) ERRORMSG();

   // concat id and filename
   if (id==NULL) name=strdup(filename);
   else name=concat(id,filename);

   return(name);
   }

// construct a local filename for a remote file
char *datacache::localfilename(char *filename)
   {
   int i;

   char *name,*path;

   name=strdup(filename);

   if (!LOCAL)
      {
      // replace bad characters
      for (i=0; i<strlen(name); i++)
         if (name[i]=='/' || name[i]=='\\' || name[i]==':' || name[i]=='~') name[i]='_';

      // concat local destination path
      if (LPATH!=NULL)
         {
         path=concat(LPATH,name);
         free(name);
         name=path;
         }
      }
   else
      // concat local source path
      if (RURL!=NULL)
         {
         path=concat(RURL,name);
         free(name);
         name=path;
         }

   return(name);
   }

// check whether or not a remote file is already stored in the local file cache
BOOLINT datacache::localfilecheck(char *filename)
   {
   char *localname;

   fileinfoelem *info;

   BOOLINT islocal;

   info=checkfilename(filename);

   if (info!=NULL) return(info->islocal);
   else
      {
      localname=localfilename(filename);
      islocal=(checkfile(localname)!=0);
      free(localname);

      return(islocal);
      }
   }

// insert a filename into the hash table
void datacache::insertfilename(char *filename,BOOLINT isavailable,BOOLINT islocal,
                               BOOLINT istile,BOOLINT istexture,
                               float minvalue,float maxvalue,
                               int width,int height,
                               BOOLINT tailinsert)
   {
   int i;

   unsigned int hash,index;

   fileinfoelem *info,*last;

   CLOUD->lockthread();

   if (HASHTABLE==NULL)
      {
      HASHTABLE=new fileinfoelem *[HASHSIZE];
      for (i=0; i<HASHSIZE; i++) HASHTABLE[i]=NULL;
      }

   hash=hashsum(filename);
   index=hash%HASHSIZE;

   info=HASHTABLE[index];

   while (info!=NULL)
      {
      if (hash==info->hash)
         if (strcmp(filename,info->filename)==0)
            {
            info->isavailable=isavailable;
            info->islocal=islocal;

            CLOUD->unlockthread();

            return;
            }

      info=info->next;
      }

   info=new fileinfoelem;

   info->filename=strdup(filename);
   info->hash=hash;

   info->isavailable=isavailable;
   info->islocal=islocal;

   info->istile=istile;
   info->istexture=istexture;

   info->minvalue=minvalue;
   info->maxvalue=maxvalue;

   info->width=width;
   info->height=height;

   if (!tailinsert || HASHTABLE[index]==NULL)
      {
      info->next=HASHTABLE[index];
      HASHTABLE[index]=info;
      }
   else
      {
      last=HASHTABLE[index];
      while (last->next!=NULL) last=last->next;

      last->next=info;
      info->next=NULL;
      }

   CLOUD->unlockthread();
   }

// check for a filename in the hash table
fileinfoelem *datacache::checkfilename(char *filename)
   {
   unsigned int hash,index;

   fileinfoelem *info;

   CLOUD->lockthread();

   if (HASHTABLE==NULL) return(NULL);

   hash=hashsum(filename);
   index=hash%HASHSIZE;

   info=HASHTABLE[index];

   while (info!=NULL)
      {
      if (hash==info->hash)
         if (strcmp(filename,info->filename)==0)
            {
            CLOUD->unlockthread();

            return(info);
            }

      info=info->next;
      }

   CLOUD->unlockthread();

   return(NULL);
   }

// concatenate two strings
char *datacache::concat(char *str1,char *str2)
   {
   char *str;

   if ((str=(char *)malloc(strlen(str1)+strlen(str2)+1))==NULL) ERRORMSG();

   memcpy(str,str1,strlen(str1));
   memcpy(str+strlen(str1),str2,strlen(str2)+1);

   return(str);
   }

// compute hash sum of a string
unsigned int datacache::hashsum(char *str)
   {
   static const unsigned int hashconst=271;

   int i,j;

   unsigned int hash;

   hash=0;
   j=strlen(str);

   for (i=0; i<j; i++) hash=hashconst*(hash+str[i])+hash/HASHSIZE;

   return(hash);
   }

// static callback wrappers:

void datacache::mystaticrequest(unsigned char *mapfile,databuf *map,int istexture,int background,void *data)
   {
   datacache *mycache=(datacache *)data;
   mycache->myrequest(mapfile,map,istexture,background);
   }

int datacache::mystaticcheck(unsigned char *mapfile,int istexture,void *data)
   {
   datacache *mycache=(datacache *)data;
   return(mycache->mycheck(mapfile,istexture));
   }

int datacache::mystaticinquiry(int col,int row,unsigned char *mapfile,int hlod,void *data,float *minvalue,float *maxvalue)
   {
   datacache *mycache=(datacache *)data;
   return(mycache->myinquiry(col,row,mapfile,hlod,minvalue,maxvalue));
   }

void datacache::mystaticquery(int col,int row,unsigned char *texfile,int tlod,void *data,int *tsizex,int *tsizey)
   {
   datacache *mycache=(datacache *)data;
   mycache->myquery(col,row,texfile,tlod,tsizex,tsizey);
   }

// configuring:

void datacache::configure_dontfree(int dontfree)
   {
   CONFIGURE_DONTFREE=dontfree;
   CLOUD->configure_dontfree(dontfree);
   }

void datacache::configure_netthreads(int netthreads)
   {CONFIGURE_NETTHREADS=netthreads;}
