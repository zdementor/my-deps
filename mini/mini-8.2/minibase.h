// (c) by Stefan Roettger

#ifndef MINIBASE_H
#define MINIBASE_H

#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include <math.h>

extern void (*minierrorhandler)(char *file,int line,int fatal);

#define WARNMSG() minierrormsg(__FILE__,__LINE__)
#define ERRORMSG() minierrormsg(__FILE__,__LINE__,1)

inline void minierrormsg(char *file,int line,int fatal=0)
   {
   if (fatal!=0) fprintf(stderr,"fatal ");
   fprintf(stderr,"error in <%s> at line %d!\n",file,line);
   if (minierrorhandler!=0) minierrorhandler(file,line,fatal);
   if (fatal!=0) exit(EXIT_FAILURE);
   }

#ifndef NULL
#define NULL (0)
#endif

#define BOOLINT char

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef PI
#define PI (3.14159265358979323846)
#endif

#ifndef RAD
#define RAD (PI/180.0)
#endif

#ifndef MAXFLOAT
#define MAXFLOAT (FLT_MAX)
#endif

#undef ffloor
#define ffloor(x) floor((double)(x))
#undef fceil
#define fceil(x) ceil((double)(x))
#define ftrc(x) (int)ffloor(x)

inline double FABS(const double x) {return((x<0.0)?-x:x);}
#define fabs(x) FABS(x)

inline int min(const int a,const int b) {return((a<b)?a:b);}
inline double FMIN(const double a,const double b) {return((a<b)?a:b);}
#define fmin(a,b) FMIN(a,b)

inline int max(const int a,const int b) {return((a>b)?a:b);}
inline double FMAX(const double a,const double b) {return((a>b)?a:b);}
#define fmax(a,b) FMAX(a,b)

inline int sqr(const int x) {return(x*x);}
inline double fsqr(const double x) {return(x*x);}

#undef fsqrt
#define fsqrt(x) sqrt((double)(x))

#undef fsin
#define fsin(x) sin((double)(x))
#undef fcos
#define fcos(x) cos((double)(x))
#undef ftan
#define ftan(x) tan((double)(x))

#undef fasin
#define fasin(x) asin((double)(x))
#undef facos
#define facos(x) acos((double)(x))
#undef fatan
#define fatan(x) atan((double)(x))

#undef fexp
#define fexp(x) exp((double)(x))
#undef flog
#define flog(x) log((double)(x))
#undef fpow
#define fpow(x,y) pow((double)(x),(double)(y))

#ifdef _MSC_VER
#define strdup _strdup
#define snprintf _snprintf
#endif

#endif
