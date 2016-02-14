// (c) by Stefan Roettger

#ifndef MINITIME_H
#define MINITIME_H

#include "minibase.h"

#include <time.h>
#ifndef _WIN32
#include <sys/time.h>
#else
#ifndef APIENTRY
#include <windows.h>
#endif
#endif

inline double minitime()
   {
#ifndef _WIN32
   struct timeval t;
   gettimeofday(&t,NULL);
   return(t.tv_sec+t.tv_usec/1.0E6);
#else
   LARGE_INTEGER freq,count;
   if (QueryPerformanceFrequency(&freq)==0) exit(EXIT_FAILURE);
   QueryPerformanceCounter(&count);
   return((double)count.QuadPart/freq.QuadPart);
#endif
   }

inline void waitfor(double secs)
   {
#ifndef _WIN32
   struct timespec dt,rt;
   dt.tv_sec=(int)floor(secs);
   dt.tv_nsec=(int)floor(1.0E9*(secs-floor(secs)));
   while (nanosleep(&dt,&rt)!=0) dt=rt;
#else
   double time=minitime()+secs;
   while (minitime()<time);
#endif
   }

#endif
