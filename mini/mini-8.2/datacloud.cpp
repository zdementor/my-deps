// (c) by Stefan Roettger

#include "minibase.h"

#include "minitime.h"

#include "datacloud.hpp"

// default constructor
datacloud::datacloud(miniload *terrain,int maxthreads)
   {
   int i;

   TERRAIN=terrain;

   if (maxthreads<1) maxthreads=1;

   MAXTHREADS=maxthreads;
   NUMTHREADS=0;

   TILECACHE=TILECACHETAIL=NULL;
   TILECOUNT=PENDINGTILES=0;

   JOBQUEUE=JOBQUEUETAIL=NULL;
   JOBCOUNT=0;

   REQUEST_CALLBACK=NULL;
   CHECK_CALLBACK=NULL;

   DELIVERED=FALSE;

   TIMESLOT=0.02; // 20ms
   EXPIRE=0.5; // 30sec

   MAXSIZE=128.0; // 128MB

   START_CALLBACK=NULL;
   JOIN_CALLBACK=NULL;
   LOCK_CALLBACK=NULL;
   UNLOCK_CALLBACK=NULL;
   LOCKIO_CALLBACK=NULL;
   UNLOCKIO_CALLBACK=NULL;

   ISRUNNING=new BOOLINT[MAXTHREADS+1];
   SHOULDSTOP=new BOOLINT[MAXTHREADS+1];
   ISREADY=new BOOLINT[MAXTHREADS+1];

   BACKARRAY=new backarrayelem[MAXTHREADS+1];

   for (i=0; i<=MAXTHREADS; i++)
      {
      ISRUNNING[i]=FALSE;
      SHOULDSTOP[i]=FALSE;
      ISREADY[i]=TRUE;

      BACKARRAY[i].background=i;
      BACKARRAY[i].cloud=this;
      }

   ISNOTREADY=FALSE;

   CONFIGURE_DONTFREE=0;
   CONFIGURE_AUTOLOCKIO=1;
   CONFIGURE_AUTOCOMPRESS=1;

   TERRAIN->configure_dontfree(1);
   }

// destructor
datacloud::~datacloud()
   {
   // stop background thread
   stopthread();

   delete[] ISRUNNING;
   delete[] SHOULDSTOP;
   delete[] ISREADY;

   delete[] BACKARRAY;

   tilecacheelem *tile=TILECACHE;

   // delete tile cache
   while (tile!=NULL)
      {
      tilecacheelem *next=tile->next;

      free(tile->tileid);

      if (CONFIGURE_DONTFREE==0) tile->tile->release();
      delete tile->tile;

      delete tile;
      tile=next;
      }

   jobqueueelem *job=JOBQUEUE;

   // delete job queue
   while (job!=NULL)
      {
      jobqueueelem *next=job->next;

      delete job;
      job=next;
      }
   }

// set callbacks for requesting tiles
void datacloud::setloader(void (*request)(unsigned char *mapfile,databuf *map,int istexture,int background,void *data),void *data,
                          int (*check)(unsigned char *mapfile,int istexture,void *data),
                          int paging,
                          float pfarp,
                          float prange,int pbasesize,
                          int plazyness,int pupdate,
                          int expire)
   {
   if (request==NULL || check==NULL) ERRORMSG();

   // propagate callbacks to the wrapped miniload object
   TERRAIN->setloader(mystaticrequest,this,mystaticpreload,mystaticdeliver,
                      paging,pfarp,prange,pbasesize,plazyness,pupdate,expire);

   REQUEST_CALLBACK=request;
   CHECK_CALLBACK=check;
   REQUEST_DATA=data;
   }

// set optional callback for inquiry of height map elevation range
void datacloud::setinquiry(int (*inquiry)(int col,int row,unsigned char *mapfile,int hlod,void *data,float *minvalue,float *maxvalue),void *data)
   {TERRAIN->setinquiry(inquiry,data);}

// set optional callback for query of texture map base size
void datacloud::setquery(void (*query)(int col,int row,unsigned char *texfile,int tlod,void *data,int *tsizex,int *tsizey),void *data)
   {TERRAIN->setquery(query,data);}

// set scheduling properties
void datacloud::setschedule(double timeslot,double expire)
   {
   if (timeslot<0.0 || expire<0.0) ERRORMSG();

   TIMESLOT=timeslot;
   EXPIRE=expire;
   }

// set maximum allowed tile cache size
void datacloud::setmaxsize(double maxsize)
   {
   if (maxsize<0.0) ERRORMSG();
   MAXSIZE=maxsize;
   }

// set optional background thread
void datacloud::setthread(void (*start)(void *(*thread)(void *background),backarrayelem *background,void *data),void *data,
                          void (*join)(backarrayelem *background,void *data),
                          void (*lock_cs)(void *data),
                          void (*unlock_cs)(void *data),
                          void (*lock_io)(void *data),
                          void (*unlock_io)(void *data))
   {
   if (start!=NULL && (join==NULL || lock_cs==NULL || unlock_cs==NULL)) ERRORMSG();
   if (lock_io!=NULL && unlock_io==NULL) ERRORMSG();

   // stop background thread
   stopthread();

   START_CALLBACK=start;
   JOIN_CALLBACK=join;
   LOCK_CALLBACK=lock_cs;
   UNLOCK_CALLBACK=unlock_cs;
   LOCKIO_CALLBACK=lock_io;
   UNLOCKIO_CALLBACK=unlock_io;
   START_DATA=data;
   }

// set number of multiple background threads
void datacloud::setmulti(int multi)
   {
   int i;

   if (multi==MAXTHREADS) return;

   // stop background thread
   stopthread();

   delete[] ISRUNNING;
   delete[] SHOULDSTOP;
   delete[] ISREADY;

   delete[] BACKARRAY;

   if (multi<1) multi=1;

   MAXTHREADS=multi;

   ISRUNNING=new BOOLINT[MAXTHREADS+1];
   SHOULDSTOP=new BOOLINT[MAXTHREADS+1];
   ISREADY=new BOOLINT[MAXTHREADS+1];

   BACKARRAY=new backarrayelem[MAXTHREADS+1];

   for (i=0; i<=MAXTHREADS; i++)
      {
      ISRUNNING[i]=FALSE;
      SHOULDSTOP[i]=FALSE;
      ISREADY[i]=TRUE;

      BACKARRAY[i].background=i;
      BACKARRAY[i].cloud=this;
      }
   }

// signal background thread to finish
void datacloud::signalthread(int background)
   {
   int i;

   if (!ISNOTREADY) return;

   if (background==0)
      // signal all threads to finish
      for (i=1; i<=MAXTHREADS; i++) signalthread(i);
   else
      // signal thread if it is still running
      if (!ISREADY[background]) SHOULDSTOP[background]=TRUE;
   }

// stop background thread
void datacloud::stopthread(int background)
   {
   int i;

   if (!ISNOTREADY) return;

   if (background==0)
      // stop all threads
      {
      signalthread();
      for (i=1; i<=MAXTHREADS; i++) stopthread(i);
      }
   else
      // join thread if it is still running
      if (!ISREADY[background])
         {
         SHOULDSTOP[background]=TRUE;
         JOIN_CALLBACK(&BACKARRAY[background],START_DATA);
         SHOULDSTOP[background]=FALSE;
         ISREADY[background]=TRUE;
         NUMTHREADS--;
         }

   if (NUMTHREADS==0) ISNOTREADY=FALSE;
   }

// lock background thread
void datacloud::lockthread()
   {if (ISNOTREADY) LOCK_CALLBACK(START_DATA);}

// unlock background thread
void datacloud::unlockthread()
   {if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);}

// lock io operation
void datacloud::lockio()
   {
   if (LOCKIO_CALLBACK!=NULL)
      if (ISNOTREADY) LOCKIO_CALLBACK(START_DATA);
   }

// unlock io operation
void datacloud::unlockio()
   {
   if (LOCKIO_CALLBACK!=NULL)
      if (ISNOTREADY) UNLOCKIO_CALLBACK(START_DATA);
   }

// query number of missing tiles in the cache
int datacloud::getmissing()
   {
   int missing=0;

   tilecacheelem *tile;

   // scan for hi prio tiles that are not yet available
   for (tile=TILECACHE; tile!=NULL; tile=tile->next)
      if (!tile->isavailable && !tile->loprio) missing++;

   return(missing);
   }

// query total amount of memory allocated in the cache
double datacloud::getmem()
   {
   double mem=0.0;

   tilecacheelem *tile;

   // scan for tiles that are already available
   for (tile=TILECACHE; tile!=NULL; tile=tile->next)
      if (tile->isavailable) mem+=tile->tile->bytes;

   return(mem/(1024*1024));
   }

// own callbacks:

int datacloud::myrequest(int col,int row,unsigned char *mapfile,int hlod,unsigned char *texfile,int tlod,unsigned char *fogfile,databuf *hfield,databuf *texture,databuf *fogmap)
   {
   BOOLINT immediate=FALSE;

   // catch file existance check
   if (hfield==NULL && texture==NULL && fogmap==NULL)
      {
      if (!checkfile(mapfile,FALSE)) return(0);
      if (!checkfile(texfile,TRUE)) return(0);
      if (!checkfile(fogfile,FALSE)) return(0);

      return(1);
      }

   // check for height fields that should be delivered immediately
   if (mapfile!=NULL)
      if (TERRAIN->getminitile()==NULL) immediate=TRUE;
      else if (!TERRAIN->getminitile()->isloaded(col,row)) immediate=TRUE;

   // check for texture maps that should be delivered immediately
   if (texfile!=NULL)
      if (TERRAIN->getminitile()==NULL) immediate=TRUE;
      else if (TERRAIN->getminitile()->gettexid(col,row)==0) immediate=TRUE;

   // check for request of texture map only
   if (mapfile==NULL && texfile!=NULL && fogfile==NULL) immediate=TRUE;

   // schedule job as immediate or deferred with hi prio
   insertjob(col,row,mapfile,hlod,texfile,tlod,fogfile,immediate,FALSE);

   // immediate data delivery
   if (immediate) deliverdata(hfield,texture,fogmap,TRUE);

   return(0);
   }

void datacloud::mypreload(int col,int row,unsigned char *mapfile,int hlod,unsigned char *texfile,int tlod,unsigned char *fogfile)
   {
   // schedule job as deferred with lo prio
   insertjob(col,row,mapfile,hlod,texfile,tlod,fogfile,FALSE,TRUE);
   }

void datacloud::mydeliver(int *col,int *row,databuf *hfield,int *hlod,databuf *texture,int *tlod,databuf *fogmap)
   {
   // deferred data delivery
   deliverdata(hfield,texture,fogmap,FALSE,col,row,hlod,tlod);
   }

// file existance check
BOOLINT datacloud::checkfile(unsigned char *mapfile,BOOLINT istexture)
   {
   BOOLINT check;

   if (mapfile==NULL) return(TRUE);

   check=(CHECK_CALLBACK(mapfile,istexture?1:0,REQUEST_DATA)!=0);

   return(check);
   }

// insert one job into the queue
void datacloud::insertjob(int col,int row,unsigned char *mapfile,int hlod,unsigned char *texfile,int tlod,unsigned char *fogfile,BOOLINT immediate,BOOLINT loprio)
   {
   // check for already existing job
   if (checkjob(mapfile,texfile,fogfile,immediate,loprio)) return;

   jobqueueelem *newjob=new jobqueueelem;

   newjob->col=col;
   newjob->row=row;

   // schedule height field
   if (mapfile!=NULL) newjob->hfield=inserttile(mapfile,col,row,FALSE,immediate,loprio);
   else newjob->hfield=NULL;
   newjob->hlod=hlod;

   // schedule texture map
   if (texfile!=NULL) newjob->texture=inserttile(texfile,col,row,TRUE,immediate,loprio);
   else newjob->texture=NULL;
   newjob->tlod=tlod;

   // schedule fog field
   if (fogfile!=NULL) newjob->fogmap=inserttile(fogfile,col,row,FALSE,immediate,loprio);
   else newjob->fogmap=NULL;

   newjob->loprio=loprio;

   // insert immediate job at begin
   if (immediate) insertjob(NULL,newjob);
   // insert hi prio job before lo prio jobs
   else if (!loprio)
      {
      jobqueueelem *job=JOBQUEUETAIL;

      // scan past lo prio jobs in reverse order
      while (job!=NULL)
         {
         if (!job->loprio) break;
         job=job->prev;
         }

      // insert after last hi prio job
      insertjob(job,newjob);
      }
   // insert lo prio job at end
   else insertjob(JOBQUEUETAIL,newjob);
   }

// check job for existance
BOOLINT datacloud::checkjob(unsigned char *mapfile,unsigned char *texfile,unsigned char *fogfile,BOOLINT immediate,BOOLINT loprio)
   {
   jobqueueelem *job;

   // scan queue for equal job
   for (job=JOBQUEUE; job!=NULL; job=job->next)
      {
      // check mapfile:

      if (mapfile!=NULL && job->hfield==NULL) continue;
      if (mapfile==NULL && job->hfield!=NULL) continue;

      if (mapfile!=NULL)
         if (strcmp((char *)mapfile,(char *)(job->hfield->tileid))!=0) continue;

      // check texfile:

      if (texfile!=NULL && job->texture==NULL) continue;
      if (texfile==NULL && job->texture!=NULL) continue;

      if (texfile!=NULL)
         if (strcmp((char *)texfile,(char *)(job->texture->tileid))!=0) continue;

      // check fogfile:

      if (fogfile!=NULL && job->fogmap==NULL) continue;
      if (fogfile==NULL && job->fogmap!=NULL) continue;

      if (fogfile!=NULL)
         if (strcmp((char *)fogfile,(char *)(job->fogmap->tileid))!=0) continue;

      // job already exists
      if (immediate || (!loprio && job->loprio))
         {
         deletejob(job);
         return(FALSE);
         }
      else return(TRUE);
      }

   return(FALSE);
   }

// insert a job into the queue after a given element
void datacloud::insertjob(jobqueueelem *job,jobqueueelem *newjob)
   {
   // insert in empty queue
   if (JOBQUEUE==NULL)
      {
      JOBQUEUE=JOBQUEUETAIL=newjob;
      newjob->prev=newjob->next=NULL;
      }
   // insert at queue head
   else if (job==NULL)
      {
      newjob->prev=NULL;
      newjob->next=JOBQUEUE;
      JOBQUEUE->prev=newjob;
      JOBQUEUE=newjob;
      }
   // insert at queue tail
   else if (job==JOBQUEUETAIL)
      {
      newjob->prev=JOBQUEUETAIL;
      newjob->next=NULL;
      JOBQUEUETAIL->next=newjob;
      JOBQUEUETAIL=newjob;
      }
   // insert after given element
   else
      {
      newjob->prev=job;
      newjob->next=job->next;
      job->next=newjob;
      newjob->next->prev=newjob;
      }

   // one more job
   JOBCOUNT++;
   }

// delete a job from the queue
void datacloud::deletejob(jobqueueelem *job)
   {
   // decrease refcounts
   if (job->hfield!=NULL) job->hfield->refcount--;
   if (job->texture!=NULL) job->texture->refcount--;
   if (job->fogmap!=NULL) job->fogmap->refcount--;

   // delete from head
   if (job==JOBQUEUE)
      {
      JOBQUEUE=job->next;
      if (JOBQUEUE!=NULL) JOBQUEUE->prev=NULL;
      else JOBQUEUETAIL=NULL;
      delete job;
      }
   // delete from tail
   else if (job==JOBQUEUETAIL)
      {
      JOBQUEUETAIL=job->prev;
      if (JOBQUEUETAIL!=NULL) JOBQUEUETAIL->next=NULL;
      else JOBQUEUE=NULL;
      delete job;
      }
   // delete from queue
   else
      {
      jobqueueelem *prev=job->prev;
      jobqueueelem *next=job->next;
      prev->next=next;
      next->prev=prev;
      delete job;
      }

   // one job less
   JOBCOUNT--;
   }

// insert a tile into the cache
tilecacheelem *datacloud::inserttile(unsigned char *tileid,int col,int row,BOOLINT istexture,BOOLINT immediate,BOOLINT loprio)
   {
   int i;

   tilecacheelem *oldtile,*newtile;

   // check for already existing tile
   oldtile=checktile(tileid,col,row,istexture,immediate,loprio);

   // already existing tile
   if (oldtile!=NULL)
      {
      newtile=oldtile;
      newtile->refcount++;
      }
   // allocate and initialize new tile
   else
      {
      newtile=new tilecacheelem;

      newtile->tileid=(unsigned char *)strdup((char *)tileid);
      newtile->tile=new databuf;

      newtile->col=col;
      newtile->row=row;

      newtile->istexture=istexture;

      newtile->loprio=loprio;

      newtile->refcount=1;

      newtile->isavailable=FALSE;
      newtile->isloading=FALSE;

      newtile->background=0;
      }

   // lock critical section
   if (ISNOTREADY) LOCK_CALLBACK(START_DATA);

   // load immediate data
   if (immediate && !newtile->isavailable)
      {
      // signal background thread to finish
      signalthread();

      // check if actual tile is already being loaded in the background
      if (newtile->isloading)
         {
         // unlock critical section
         if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);

         // wait for background thread to finish loading
         stopthread(newtile->background);

         // relock critical section
         if (ISNOTREADY) LOCK_CALLBACK(START_DATA);
         }

      // load actual tile directly
      if (!newtile->isavailable)
         {
         // signal direct loading
         newtile->isloading=TRUE;

         // unlock critical section
         if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);

         // lock io
         if (ISNOTREADY)
            if (CONFIGURE_AUTOLOCKIO!=0)
               if (LOCKIO_CALLBACK!=NULL) LOCKIO_CALLBACK(START_DATA);

         // load data
         REQUEST_CALLBACK(tileid,newtile->tile,istexture,0,REQUEST_DATA);

         // autocompress textures
         if (CONFIGURE_AUTOCOMPRESS!=0) newtile->tile->autocompress();

         // unlock io
         if (ISNOTREADY)
            if (CONFIGURE_AUTOLOCKIO!=0)
               if (LOCKIO_CALLBACK!=NULL) UNLOCKIO_CALLBACK(START_DATA);

         // relock critical section
         if (ISNOTREADY) LOCK_CALLBACK(START_DATA);

         // signal availability
         newtile->isavailable=TRUE;
         newtile->isloading=FALSE;

         // decrease pending tile count
         if (oldtile!=NULL) PENDINGTILES--;
         }
      }

   // unlock critical section
   if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);

   // set access time
   newtile->access=minitime();

   // insert tile at cache tail
   if (oldtile==NULL) inserttile(TILECACHETAIL,newtile);

   return(newtile);
   }

// check tile for existance
tilecacheelem *datacloud::checktile(unsigned char *tileid,int col,int row,BOOLINT istexture,BOOLINT immediate,BOOLINT loprio)
   {
   tilecacheelem *tile;

   // scan cache for equal tile
   for (tile=TILECACHE; tile!=NULL; tile=tile->next)
      {
      // mismatch of tile identifier
      if (strcmp((char *)tileid,(char *)(tile->tileid))!=0) continue;

      // consistency check
      if (istexture!=tile->istexture) ERRORMSG();
      if (col!=tile->col || row!=tile->row) ERRORMSG();

      // upgrade lo prio to hi prio tile if necessary
      if (immediate || (!loprio && tile->loprio)) tile->loprio=FALSE;

      return(tile);
      }

   return(NULL);
   }

// insert a tile into the cache after a given element
void datacloud::inserttile(tilecacheelem *tile,tilecacheelem *newtile)
   {
   // lock critical section
   if (ISNOTREADY) LOCK_CALLBACK(START_DATA);

   // increase pending tile count
   if (!newtile->isavailable) PENDINGTILES++;

   // insert in empty cache
   if (TILECACHE==NULL)
      {
      TILECACHE=TILECACHETAIL=newtile;
      newtile->prev=newtile->next=NULL;
      }
   // insert at cache head
   else if (tile==NULL)
      {
      newtile->prev=NULL;
      newtile->next=TILECACHE;
      TILECACHE->prev=newtile;
      TILECACHE=newtile;
      }
   // insert at cache tail
   else if (tile==TILECACHETAIL)
      {
      newtile->prev=TILECACHETAIL;
      newtile->next=NULL;
      TILECACHETAIL->next=newtile;
      TILECACHETAIL=newtile;
      }
   // insert after given element
   else
      {
      newtile->prev=tile;
      newtile->next=tile->next;
      tile->next=newtile;
      newtile->next->prev=newtile;
      }

   // one more tile
   TILECOUNT++;

   // unlock critical section
   if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);
   }

// delete a tile from the cache
void datacloud::deletetile(tilecacheelem *tile)
   {
   // lock critical section
   if (ISNOTREADY) LOCK_CALLBACK(START_DATA);

   // decrease pending tile count
   if (!tile->isavailable) PENDINGTILES--;

   // release tile identifier
   free(tile->tileid);

   // release memory chunk
   if (CONFIGURE_DONTFREE==0) tile->tile->release();

   // release databuf object
   delete tile->tile;

   // delete from head
   if (tile==TILECACHE)
      {
      TILECACHE=tile->next;
      if (TILECACHE!=NULL) TILECACHE->prev=NULL;
      else TILECACHETAIL=NULL;
      delete tile;
      }
   // delete from tail
   else if (tile==TILECACHETAIL)
      {
      TILECACHETAIL=tile->prev;
      if (TILECACHETAIL!=NULL) TILECACHETAIL->next=NULL;
      else TILECACHE=NULL;
      delete tile;
      }
   // delete from queue
   else
      {
      tilecacheelem *prev=tile->prev;
      tilecacheelem *next=tile->next;
      prev->next=next;
      next->prev=prev;
      delete tile;
      }

   // one tile less
   TILECOUNT--;

   // unlock critical section
   if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);
   }

// load a pending tile in the background
BOOLINT datacloud::loadpendingtile(int background)
   {
   float prio,p;

   tilecacheelem *tile,*scan;

   // lock critical section
   if (ISNOTREADY) LOCK_CALLBACK(START_DATA);

   prio=0.0f;

   tile=NULL;
   scan=TILECACHE;

   // scan for tile with highest priority
   while (scan!=NULL)
      {
      if (!scan->loprio && !scan->isavailable && !scan->isloading)
         {
         p=getpriority(scan);

         if (p>prio)
            {
            prio=p;
            tile=scan;
            }
         }

      scan=scan->next;
      }

   // check hi prio tile with highest priority first
   if (tile!=NULL)
      {
      // signal loading
      tile->isloading=TRUE;
      tile->background=background;

      // unlock critical section
      if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);

      // lock io
      if (ISNOTREADY)
         if (CONFIGURE_AUTOLOCKIO!=0)
            if (LOCKIO_CALLBACK!=NULL) LOCKIO_CALLBACK(START_DATA);

      // load data
      REQUEST_CALLBACK(tile->tileid,tile->tile,tile->istexture,background,REQUEST_DATA);

      // autocompress textures in the background
      if (CONFIGURE_AUTOCOMPRESS!=0) tile->tile->autocompress();

      // unlock io
      if (ISNOTREADY)
         if (CONFIGURE_AUTOLOCKIO!=0)
            if (LOCKIO_CALLBACK!=NULL) UNLOCKIO_CALLBACK(START_DATA);

      // relock critical section
      if (ISNOTREADY) LOCK_CALLBACK(START_DATA);

      // signal availability
      tile->isavailable=TRUE;
      tile->isloading=FALSE;

      // decrease pending tile count
      PENDINGTILES--;

      // set access time
      tile->access=minitime();

      // unlock critical section
      if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);

      return(TRUE);
      }

   tile=TILECACHE;

   // check for lo prio tiles next
   while (tile!=NULL)
      {
      if (tile->loprio && !tile->isavailable && !tile->isloading)
         {
         // signal loading
         tile->isloading=TRUE;
         tile->background=background;

         // unlock critical section
         if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);

         // lock io
         if (ISNOTREADY)
            if (CONFIGURE_AUTOLOCKIO!=0)
               if (LOCKIO_CALLBACK!=NULL) LOCKIO_CALLBACK(START_DATA);

         // load data
         REQUEST_CALLBACK(tile->tileid,tile->tile,tile->istexture,background,REQUEST_DATA);

         // autocompress textures in the background
         if (CONFIGURE_AUTOCOMPRESS!=0) tile->tile->autocompress();

         // unlock io
         if (ISNOTREADY)
            if (CONFIGURE_AUTOLOCKIO!=0)
               if (LOCKIO_CALLBACK!=NULL) UNLOCKIO_CALLBACK(START_DATA);

         // relock critical section
         if (ISNOTREADY) LOCK_CALLBACK(START_DATA);

         // signal availability
         tile->isavailable=TRUE;
         tile->isloading=FALSE;

         // decrease pending tile count
         PENDINGTILES--;

         // set access time
         tile->access=minitime();

         // unlock critical section
         if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);

         return(TRUE);
         }

      tile=tile->next;
      }

   // unlock critical section
   if (ISNOTREADY) UNLOCK_CALLBACK(START_DATA);

   return(FALSE);
   }

// delete expired tiles
void datacloud::deleteexpiredtiles()
   {
   double time;

   tilecacheelem *tile,*next;

   if (EXPIRE==0.0) return;

   time=minitime();

   tile=TILECACHE;

   // check all tiles
   while (tile!=NULL)
      {
      next=tile->next;

      if (tile->isavailable && tile->refcount==0)
         {
         double idle=time-tile->access;
         if (idle>60*EXPIRE) deletetile(tile);
         }

      tile=next;
      }
   }

// shrink cache to maximum allowed size
void datacloud::shrinkcache(double maxsize)
   {
   double totalsize;

   tilecacheelem *tile,*next;

   if (maxsize==0.0) return;

   totalsize=0.0;

   tile=TILECACHE;

   // sum up all tiles
   while (tile!=NULL)
      {
      if (tile->isavailable) totalsize+=tile->tile->bytes;

      tile=tile->next;
      }

   tile=TILECACHE;

   // delete old tiles
   while (tile!=NULL && totalsize>maxsize)
      {
      next=tile->next;

      if (tile->isavailable && tile->refcount==0)
         {
         totalsize-=tile->tile->bytes;
         deletetile(tile);
         }

      tile=next;
      }
   }

// calculate the tile priority
float datacloud::getpriority(tilecacheelem *tile)
   {
   float prio;

   float col0=TERRAIN->getminitile()->getcenteri();
   float row0=TERRAIN->getminitile()->getcenterj();

   prio=fsqr(tile->col+0.5f-col0)+fsqr(tile->row+0.5f-row0);

   if (prio==0.0f) prio=MAXFLOAT;
   else prio=1.0f/prio;

   return(prio);
   }

// deliver data that is already available
void datacloud::deliverdata(databuf *hfield,databuf *texture,databuf *fogmap,BOOLINT immediate,int *col,int *row,int *hlod,int *tlod)
   {
   // start periodical tasks
   if (!immediate)
      {
      if (!DELIVERED)
         {
         // delete expired tiles
         deleteexpiredtiles();

         // shrink cache to maximum allowed size
         shrinkcache(MAXSIZE*1024*1024);
         }

      // load pending tiles in the background
      startthread();
      }

   // check deferred data delivery
   if (!immediate)
      if (DELIVERED)
         {
         double upload=minitime()-DELIVERYSTART;

         // stop deferred data delivery because time is up
         if (upload>TIMESLOT)
            {
            DELIVERED=FALSE;
            return;
            }
         }

   jobqueueelem *job=JOBQUEUE;

   // scan jobs
   if (!immediate)
      while (job!=NULL)
         {
         BOOLINT isavailable=TRUE;

         // check availability of maps:

         if (job->hfield!=NULL)
            if (!job->hfield->isavailable) isavailable=FALSE;

         if (job->texture!=NULL)
            if (!job->texture->isavailable) isavailable=FALSE;

         if (job->fogmap!=NULL)
            if (!job->fogmap->isavailable) isavailable=FALSE;

         // start deferred data delivery
         if (isavailable) break;

         job=job->next;
         }

   // data delivery
   if (job!=NULL)
      {
      // return maps
      if (job->hfield!=NULL) *hfield=*(job->hfield->tile);
      if (job->texture!=NULL) *texture=*(job->texture->tile);
      if (job->fogmap!=NULL) *fogmap=*(job->fogmap->tile);

      // return tile position
      if (col!=NULL) *col=job->col;
      if (row!=NULL) *row=job->row;

      // return level of detail
      if (hlod!=NULL) *hlod=job->hlod;
      if (tlod!=NULL) *tlod=job->tlod;

      // check for immediate delivery
      if (immediate) DELIVERED=FALSE;
      // remember starting time of deferred delivery
      else if (!DELIVERED)
         {
         DELIVERED=TRUE;
         DELIVERYSTART=minitime();
         }

      // remove finished job from queue
      deletejob(job);
      }
   else DELIVERED=FALSE;
   }

// start background thread
void datacloud::startthread()
   {
   int i;

   // check pending tile count
   if (PENDINGTILES==0) return;

   // non-threaded paging
   if (START_CALLBACK==NULL) pager();
   // threaded paging
   else
      for (i=1; i<=MAXTHREADS; i++)
         if (!ISRUNNING[i])
            {
            // wait for background thread to finish
            stopthread(i);

            // start new background thread
            ISRUNNING[i]=TRUE;
            ISREADY[i]=FALSE;
            ISNOTREADY=TRUE;
            START_CALLBACK(triggerpager,&BACKARRAY[i],START_DATA);
            NUMTHREADS++;
            }
   }

// jump to background thread
void *datacloud::triggerpager(void *background)
   {
   backarrayelem *backarray=(backarrayelem *)background;
   backarray->cloud->pager(backarray->background);

   return(NULL);
   }

// background thread
void datacloud::pager(int background)
   {
   // cycle until stop is requested
   while (!SHOULDSTOP[background])
      if (!loadpendingtile(background)) break; // stop if no further tile is pending

   ISRUNNING[background]=FALSE;
   }

// static callback wrappers:

int datacloud::mystaticrequest(int col,int row,unsigned char *mapfile,int hlod,unsigned char *texfile,int tlod,unsigned char *fogfile,void *data,databuf *hfield,databuf *texture,databuf *fogmap)
   {
   datacloud *mycloud=(datacloud *)data;
   return(mycloud->myrequest(col,row,mapfile,hlod,texfile,tlod,fogfile,hfield,texture,fogmap));
   }

void datacloud::mystaticpreload(int col,int row,unsigned char *mapfile,int hlod,unsigned char *texfile,int tlod,unsigned char *fogfile,void *data)
   {
   datacloud *mycloud=(datacloud *)data;
   mycloud->mypreload(col,row,mapfile,hlod,texfile,tlod,fogfile);
   }

void datacloud::mystaticdeliver(int *col,int *row,databuf *hfield,int *hlod,databuf *texture,int *tlod,databuf *fogmap,void *data)
   {
   datacloud *mycloud=(datacloud *)data;
   mycloud->mydeliver(col,row,hfield,hlod,texture,tlod,fogmap);
   }

// configuring:

void datacloud::configure_dontfree(int dontfree)
   {CONFIGURE_DONTFREE=dontfree;}

void datacloud::configure_autolockio(int autolockio)
   {CONFIGURE_AUTOLOCKIO=autolockio;}

void datacloud::configure_autocompress(int autocompress)
   {CONFIGURE_AUTOCOMPRESS=autocompress;}
