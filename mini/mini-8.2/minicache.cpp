// (c) by Stefan Roettger

#include "minibase.h"

#include "miniOGL.h"

#include "minicache.hpp"

enum {BEGINFAN_OP=1,
      FANVERTEX_OP=2,
      NOTIFY_OP=3,
      TEXMAP_OP=4,
      TRIGGER_OP=5};

minicache *minicache::CACHE;

minitile *minicache::TERRAIN;

// default constructor
minicache::minicache()
   {
   int i;

   CACHE=NULL;

   TERRAIN=NULL;

   CACHE_NUM=1;

   CACHE_SIZE1=CACHE_SIZE2=0;
   CACHE_MAXSIZE=1;

   if ((CACHE1_OP=(unsigned char *)malloc(CACHE_MAXSIZE))==NULL) ERRORMSG();
   if ((CACHE2_OP=(unsigned char *)malloc(CACHE_MAXSIZE))==NULL) ERRORMSG();

   if ((CACHE1_ARG=(float *)malloc(3*CACHE_MAXSIZE*sizeof(float)))==NULL) ERRORMSG();
   if ((CACHE2_ARG=(float *)malloc(3*CACHE_MAXSIZE*sizeof(float)))==NULL) ERRORMSG();

   PRISM_SIZE1=PRISM_SIZE2=0;
   PRISM_MAXSIZE=1;

   if ((PRISM_CACHE1=(float *)malloc(4*PRISM_MAXSIZE*sizeof(float)))==NULL) ERRORMSG();
   if ((PRISM_CACHE2=(float *)malloc(4*PRISM_MAXSIZE*sizeof(float)))==NULL) ERRORMSG();

   FANCNT1=FANCNT2=0;
   VTXCNT1=VTXCNT2=0;

   OPACITY=1.0f;

   PRISM_MODE=1;
   PRISM_BASE=0.5f;
   PRISM_R=PRISM_G=PRISM_B=1.0f;
   PRISM_A=0.9f;

   VTXPROG=NULL;
   VTXDIRTY=0;

   VTXPROGID=0;
   USEVTXSHADER=0;

   for (i=0; i<8; i++)
      {
      VTXSHADERPAR1[i]=0.0f;
      VTXSHADERPAR2[i]=0.0f;
      VTXSHADERPAR3[i]=0.0f;
      VTXSHADERPAR4[i]=0.0f;
      }

   FRAGPROG=NULL;
   FRAGDIRTY=0;

   FRAGPROGID=0;
   USEPIXSHADER=0;

   for (i=0; i<8; i++)
      {
      PIXSHADERPAR1[i]=0.0f;
      PIXSHADERPAR2[i]=0.0f;
      PIXSHADERPAR3[i]=0.0f;
      PIXSHADERPAR4[i]=0.0f;
      }

   PIXSHADERTEXID=0;

   SHADERINITSTATE_CB=NULL;
   SHADEREXITSTATE_CB=NULL;

   GLSETUP=0;
   WGLSETUP=0;

   GLEXT_MT=0;
   GLEXT_VP=0;
   GLEXT_FP=0;

   CONFIGURE_OVERLAP=0.02f;
   CONFIGURE_MINSIZE=33;
   CONFIGURE_ZSCALE=0.95f;
   }

// destructor
minicache::~minicache()
   {
   free(CACHE1_OP);
   free(CACHE2_OP);

   free(CACHE1_ARG);
   free(CACHE2_ARG);

   free(PRISM_CACHE1);
   free(PRISM_CACHE2);

#ifndef NOOGL

   GLuint progid;

   if (VTXPROG!=NULL) free(VTXPROG);
   if (FRAGPROG!=NULL) free(FRAGPROG);

#if defined(GL_ARB_vertex_program) && defined(GL_ARB_fragment_program)

   if (GLEXT_VP!=0 && GLEXT_FP!=0)
      {
      if (VTXPROGID!=0)
         {
         progid=VTXPROGID;
         glDeleteProgramsARB(1,&progid);
         }

      if (FRAGPROGID!=0)
         {
         progid=FRAGPROGID;
         glDeleteProgramsARB(1,&progid);
         }
      }

#endif

   if (PIXSHADERTEXID!=0) deletetexmap(PIXSHADERTEXID);

#endif
   }

// static callback functions:

void minicache::cache_beginfan()
   {CACHE->cache(BEGINFAN_OP);}

void minicache::cache_fanvertex(float i,float y,float j)
   {CACHE->cache(FANVERTEX_OP,i,y,j);}

void minicache::cache_notify(int i,int j,int s)
   {CACHE->cache(NOTIFY_OP,i,j,s);}

void minicache::cache_texmap(int m,int n,int S)
   {CACHE->cache(TEXMAP_OP,m,n,S);}

void minicache::cache_prismedge(float x,float y,float yf,float z)
   {CACHE->cacheprismedge(x,y,yf,z);}

void minicache::cache_trigger(int phase,float scale,float ex,float ey,float ez)
   {CACHE->cachetrigger(phase,scale,ex,ey,ez);}

// caching:

void minicache::cache(int op,float a,float b,float c)
   {
   if (CACHE_SIZE1>=CACHE_MAXSIZE || CACHE_SIZE2>=CACHE_MAXSIZE)
      {
      CACHE_MAXSIZE*=2;

      if ((CACHE1_OP=(unsigned char *)realloc(CACHE1_OP,CACHE_MAXSIZE))==NULL) ERRORMSG();
      if ((CACHE2_OP=(unsigned char *)realloc(CACHE2_OP,CACHE_MAXSIZE))==NULL) ERRORMSG();

      if ((CACHE1_ARG=(float *)realloc(CACHE1_ARG,3*CACHE_MAXSIZE*sizeof(float)))==NULL) ERRORMSG();
      if ((CACHE2_ARG=(float *)realloc(CACHE2_ARG,3*CACHE_MAXSIZE*sizeof(float)))==NULL) ERRORMSG();
      }

   if (CACHE_NUM==1)
      {
      if (op==BEGINFAN_OP)
         {
         FANCNT1++;
         LAST_BEGINFAN=CACHE_SIZE1;
         }
      else if (op==FANVERTEX_OP)
         {
         VTXCNT1++;
         CACHE1_ARG[3*LAST_BEGINFAN]++;
         }

      CACHE1_OP[CACHE_SIZE1]=op;

      CACHE1_ARG[3*CACHE_SIZE1]=a;
      CACHE1_ARG[3*CACHE_SIZE1+1]=b;
      CACHE1_ARG[3*CACHE_SIZE1+2]=c;

      CACHE_SIZE1++;
      }
   else
      {
      if (op==BEGINFAN_OP)
         {
         FANCNT2++;
         LAST_BEGINFAN=CACHE_SIZE2;
         }
      else if (op==FANVERTEX_OP)
         {
         VTXCNT2++;
         CACHE2_ARG[3*LAST_BEGINFAN]++;
         }

      CACHE2_OP[CACHE_SIZE2]=op;

      CACHE2_ARG[3*CACHE_SIZE2]=a;
      CACHE2_ARG[3*CACHE_SIZE2+1]=b;
      CACHE2_ARG[3*CACHE_SIZE2+2]=c;

      CACHE_SIZE2++;
      }
   }

void minicache::cacheprismedge(float x,float y,float yf,float z)
   {
   if (PRISM_SIZE1>=PRISM_MAXSIZE || PRISM_SIZE2>=PRISM_MAXSIZE)
      {
      PRISM_MAXSIZE*=2;

      if ((PRISM_CACHE1=(float *)realloc(PRISM_CACHE1,4*PRISM_MAXSIZE*sizeof(float)))==NULL) ERRORMSG();
      if ((PRISM_CACHE2=(float *)realloc(PRISM_CACHE2,4*PRISM_MAXSIZE*sizeof(float)))==NULL) ERRORMSG();
      }

   if (CACHE_NUM==1)
      {
      PRISM_CACHE1[4*PRISM_SIZE1]=x;
      PRISM_CACHE1[4*PRISM_SIZE1+1]=y;
      PRISM_CACHE1[4*PRISM_SIZE1+2]=yf;
      PRISM_CACHE1[4*PRISM_SIZE1+3]=z;

      PRISM_SIZE1++;
      }
   else
      {
      PRISM_CACHE2[4*PRISM_SIZE2]=x;
      PRISM_CACHE2[4*PRISM_SIZE2+1]=y;
      PRISM_CACHE2[4*PRISM_SIZE2+2]=yf;
      PRISM_CACHE2[4*PRISM_SIZE2+3]=z;

      PRISM_SIZE2++;
      }
   }

void minicache::cachetrigger(int phase,float scale,float ex,float ey,float ez)
   {
   cache(TRIGGER_OP,phase,scale);

   if (phase==0)
      if (CACHE_NUM==1)
         {
         CACHE_SIZE2=PRISM_SIZE2=0;
         FANCNT2=VTXCNT2=0;
         CACHE_NUM=2;
         }
      else
         {
         CACHE_SIZE1=PRISM_SIZE1=0;
         FANCNT1=VTXCNT1=0;
         CACHE_NUM=1;
         }

   if (PRISMCACHE_CALLBACK!=NULL)
      PRISMCACHE_CALLBACK(phase,scale,ex,ey,ez);
   }

// render back buffer of the cache
int minicache::rendercache()
   {
   int vtx=0;

#ifndef NOOGL

   int i,p;

   vtx+=getvtxcnt();

   if (CACHE_NUM==1)
      for (i=0; i<CACHE_SIZE2; i++)
         switch (CACHE2_OP[i])
            {
            case BEGINFAN_OP:
               p=(int)CACHE2_ARG[3*i];
               glDrawArrays(GL_TRIANGLE_FAN,i+1,p);
               i+=p;
               break;
            case FANVERTEX_OP:
               break;
            case NOTIFY_OP:
               if (NOTIFY_CALLBACK!=NULL)
                  NOTIFY_CALLBACK((int)CACHE2_ARG[3*i],(int)CACHE2_ARG[3*i+1],(int)CACHE2_ARG[3*i+2]);
               break;
            case TEXMAP_OP:
               rendertexmap((int)CACHE2_ARG[3*i],(int)CACHE2_ARG[3*i+1],(int)CACHE2_ARG[3*i+2]);
               break;
            case TRIGGER_OP:
               vtx+=rendertrigger((int)CACHE2_ARG[3*i],CACHE2_ARG[3*i+1]);
               break;
            }
   else
      for (i=0; i<CACHE_SIZE1; i++)
         switch (CACHE1_OP[i])
            {
            case BEGINFAN_OP:
               p=(int)CACHE1_ARG[3*i];
               glDrawArrays(GL_TRIANGLE_FAN,i+1,p);
               i+=p;
               break;
            case FANVERTEX_OP:
               break;
            case NOTIFY_OP:
               if (NOTIFY_CALLBACK!=NULL)
                  NOTIFY_CALLBACK((int)CACHE1_ARG[3*i],(int)CACHE1_ARG[3*i+1],(int)CACHE1_ARG[3*i+2]);
               break;
            case TEXMAP_OP:
               rendertexmap((int)CACHE1_ARG[3*i],(int)CACHE1_ARG[3*i+1],(int)CACHE1_ARG[3*i+2]);
               break;
            case TRIGGER_OP:
               vtx+=rendertrigger((int)CACHE1_ARG[3*i],CACHE1_ARG[3*i+1]);
               break;
            }

#endif

   return(vtx);
   }

// rendering:

void minicache::rendertexmap(int m,int n,int S)
   {
#ifndef NOOGL

   int texid,texw,texh,texmm;

   float ox,oz;

   glPopMatrix();

   texid=TERRAIN->gettexid(m,n);
   texw=TERRAIN->gettexw(m,n);
   texh=TERRAIN->gettexh(m,n);
   texmm=TERRAIN->gettexmm(m,n);

   ox=XDIM*(m-(COLS-1)/2.0f)+CENTERX;
   oz=ZDIM*(n-(ROWS-1)/2.0f)+CENTERZ;

   glPushMatrix();
   glTranslatef(ox,CENTERY,oz);

   // avoid gaps between tiles
   if (CONFIGURE_OVERLAP!=0.0f)
      if (S>=CONFIGURE_MINSIZE) glScalef((S-1+CONFIGURE_OVERLAP)/(S-1),1.0f,(S-1+CONFIGURE_OVERLAP)/(S-1));

   glScalef(XDIM/(S-1),SCALE,-ZDIM/(S-1));
   glTranslatef(-S/2,0.0f,-S/2);

   bindtexmap(texid,texw,texh,S,texmm);

   if (USEVTXSHADER!=0)
      enablevtxshader(1.0f/(S-1)*(texw-1)/texw,
                      -1.0f/(S-1)*(texh-1)/texh,
                      0.5f/texh,
                      1.0f-0.5f/texh,
                      SCALE);

   if (USEPIXSHADER!=0) enablepixshader();

   if (TEXMAP_CALLBACK!=NULL)
      TEXMAP_CALLBACK(m,n,S,texid,texw,texh,texmm);

#endif
   }

int minicache::rendertrigger(int phase,float scale)
   {
   int vtx=0;

#ifndef NOOGL

   if (phase==1)
      {
      SCALE=scale;

      initstate();
      glPushMatrix();

      if (OPACITY<1.0f)
         if (OPACITY<=0.0f) disableRGBAwriting();
         else
            {
            color(1.0f,1.0f,1.0f,OPACITY);

            disableZwriting();
            enableblending();
            }

      if (CACHE_NUM==1) glVertexPointer(3,GL_FLOAT,0,CACHE2_ARG);
      else glVertexPointer(3,GL_FLOAT,0,CACHE1_ARG);

      glEnableClientState(GL_VERTEX_ARRAY);

      if (USEVTXSHADER!=0 || USEPIXSHADER!=0)
         if (SHADERINITSTATE_CB!=NULL) SHADERINITSTATE_CB();
      }
   else if (phase==3)
      {
      LAMBDA=scale;

      if (USEVTXSHADER!=0 || USEPIXSHADER!=0)
         if (SHADEREXITSTATE_CB!=NULL) SHADEREXITSTATE_CB();

      glDisableClientState(GL_VERTEX_ARRAY);

      if (OPACITY<1.0f)
         if (OPACITY<=0.0f) enableRGBAwriting();
         else
            {
            enableZwriting();
            disableblending();
            }

      bindtexmap(0,0,0,0);

      disablevtxshader();
      disablepixshader();

      glPopMatrix();
      exitstate();
      }

   if (TRIGGER_CALLBACK!=NULL)
      vtx+=TRIGGER_CALLBACK(phase);

   if (phase==3)
      if (PRISMEDGE_CALLBACK==NULL)
         if (PRISMRENDER_CALLBACK!=NULL)
            if (CACHE_NUM==1)
               vtx+=PRISMRENDER_CALLBACK(PRISM_CACHE2,PRISM_SIZE2/3,LAMBDA);
            else
               vtx+=PRISMRENDER_CALLBACK(PRISM_CACHE1,PRISM_SIZE1/3,LAMBDA);
         else
            if (CACHE_NUM==1)
               vtx+=renderprisms(PRISM_CACHE2,PRISM_SIZE2/3,LAMBDA,
                                 PRISM_MODE,PRISM_BASE,PRISM_R,PRISM_G,PRISM_B,PRISM_A);
            else
               vtx+=renderprisms(PRISM_CACHE1,PRISM_SIZE1/3,LAMBDA,
                                 PRISM_MODE,PRISM_BASE,PRISM_R,PRISM_G,PRISM_B,PRISM_A);

#endif

   return(vtx);
   }

int minicache::renderprisms(float *cache,int cnt,float lambda,
                            int mode,float base,float pr,float pg,float pb,float pa)
   {
   int vtx=0;

#ifndef NOOGL

   int i;

   initstate();

   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);

   glPushMatrix();
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glScalef(CONFIGURE_ZSCALE,CONFIGURE_ZSCALE,CONFIGURE_ZSCALE); // prevent Z-fighting
   glMatrixMode(GL_MODELVIEW);

   glColor4f(pr,pg,pb,pa);

   switch (mode)
      {
      case 0:

         glBegin(GL_LINES);
         for (i=0; i<cnt; i++)
            {
            float x=(cache[12*i]+cache[12*i+4]+cache[12*i+8])/3.0f;
            float y=(cache[12*i+1]+cache[12*i+5]+cache[12*i+9])/3.0f;
            float yf=(cache[12*i+2]+cache[12*i+6]+cache[12*i+10])/3.0f;
            float z=(cache[12*i+3]+cache[12*i+7]+cache[12*i+11])/3.0f;

            glVertex3f(x,y,z);
            glVertex3f(x,yf,z);
            }
         glEnd();

         vtx+=2*cnt;

         break;

      case 1:

         glBegin(GL_TRIANGLES);
         for (i=0; i<3*cnt; i++)
            {
            float x=cache[4*i],y=cache[4*i+1],yf=cache[4*i+2],z=cache[4*i+3];
            glColor4f(pr,pg,pb,fmin((yf-y)/(255.0f*lambda*base),pa));
            glVertex3f(x,yf,z);
            }
         glEnd();

         vtx+=3*cnt;

         break;
      }

   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);

   exitstate();

#endif

   return(vtx);
   }

// set callbacks for scene double buffering
void minicache::setcallbacks(minitile *terrain,
                             int cols,int rows,
                             float xdim,float zdim,
                             float centerx,float centery,float centerz,
                             void (*notify)(int i,int j,int s),
                             void (*texmap)(int m,int n,int S,int texid,int texw,int texh,int texmm),
                             void (*prismedge)(float x,float y,float yf,float z),
                             void (*prismcache)(int phase,float scale,float ex,float ey,float ez),
                             int (*prismrender)(float *cache,int cnt,float lambda),
                             int (*trigger)(int phase))
   {
   CACHE=this;

   TERRAIN=terrain;

   terrain->setcallbacks(cache_beginfan,
                         cache_fanvertex,
                         (notify==NULL)?NULL:cache_notify,
                         cache_texmap,
                         (prismedge!=NULL)?prismedge:cache_prismedge,
                         cache_trigger);

   if (cols>0 && rows>0)
      {
      COLS=cols;
      ROWS=rows;

      XDIM=xdim;
      ZDIM=zdim;

      CENTERX=centerx;
      CENTERY=centery;
      CENTERZ=centerz;
      }

   NOTIFY_CALLBACK=notify;
   TEXMAP_CALLBACK=texmap;
   PRISMEDGE_CALLBACK=prismedge;
   PRISMCACHE_CALLBACK=prismcache;
   PRISMRENDER_CALLBACK=prismrender;
   TRIGGER_CALLBACK=trigger;
   }

// define triangle mesh opacity
void minicache::setopacity(float alpha)
   {OPACITY=alpha;}

// set prism display parameters
void minicache::setprismmode(int prismmode,float prismbase,
                             float prismR,float prismG,float prismB,float prismA)
   {
   PRISM_MODE=prismmode;
   PRISM_BASE=prismbase;

   PRISM_R=prismR;
   PRISM_G=prismG;
   PRISM_B=prismB;
   PRISM_A=prismA;
   }

// set vertex shader plugin
void minicache::setvtxshader(char *vp)
   {
#ifndef NOOGL

   // default vertex shader
   static char *vtxprog="!!ARBvp1.0\
      PARAM t=program.env[0];\
      PARAM e=program.env[1];\
      PARAM c0=program.env[2];\
      PARAM c1=program.env[3];\
      PARAM c2=program.env[4];\
      PARAM c3=program.env[5];\
      PARAM c4=program.env[6];\
      PARAM c5=program.env[7];\
      PARAM c6=program.env[8];\
      PARAM c7=program.env[9];\
      PARAM mat[4]={state.matrix.mvp};\
      TEMP vtx,col,pos;\
      MOV vtx,vertex.position;\
      MOV col,vertex.color;\
      DP4 pos.x,mat[0],vtx;\
      DP4 pos.y,mat[1],vtx;\
      DP4 pos.z,mat[2],vtx;\
      DP4 pos.w,mat[3],vtx;\
      MOV result.position,pos;\
      MOV result.color,col;\
      MAD result.texcoord[0].x,vtx.x,t.x,t.z;\
      MAD result.texcoord[0].y,vtx.z,t.y,t.w;\
      MUL result.texcoord[0].z,vtx.y,e.y;\
      END";

   if (vp==NULL) vp=vtxprog;

   if (VTXPROG!=NULL)
      {
      if (strcmp(vp,VTXPROG)==0) return;
      free(VTXPROG);
      }

   VTXPROG=strdup(vp);
   VTXDIRTY=1;

#endif
   }

// set vertex shader parameter vector
void minicache::setvtxshaderparams(float p1,float p2,float p3,float p4,int n)
   {
   if (n<0 || n>=8) ERRORMSG();

   VTXSHADERPAR1[n]=p1;
   VTXSHADERPAR2[n]=p2;
   VTXSHADERPAR3[n]=p3;
   VTXSHADERPAR4[n]=p4;
   }

// enable vertex shader plugin
void minicache::enablevtxshader(float s1,float s2,float o1,float o2,float scale)
   {
#ifndef NOOGL

#if defined(GL_ARB_vertex_program) && defined(GL_ARB_fragment_program)

   int i;

   GLuint vtxprogid;

   initglexts();
   initwglprocs();

   if (GLEXT_VP!=0 && GLEXT_FP!=0)
      {
      if (VTXDIRTY!=0)
         {
         if (VTXPROGID!=0)
            {
            vtxprogid=VTXPROGID;
            glDeleteProgramsARB(1,&vtxprogid);
            }

         glGenProgramsARB(1,&vtxprogid);
         glBindProgramARB(GL_VERTEX_PROGRAM_ARB,vtxprogid);
         glProgramStringARB(GL_VERTEX_PROGRAM_ARB,GL_PROGRAM_FORMAT_ASCII_ARB,strlen(VTXPROG),VTXPROG);
         VTXPROGID=vtxprogid;

         VTXDIRTY=0;
         }

      if (VTXPROGID!=0)
         {
         glBindProgramARB(GL_VERTEX_PROGRAM_ARB,VTXPROGID);
         glEnable(GL_VERTEX_PROGRAM_ARB);

         glProgramEnvParameter4fARB(GL_VERTEX_PROGRAM_ARB,0,s1,s2,o1,o2);
         glProgramEnvParameter4fARB(GL_VERTEX_PROGRAM_ARB,1,0.0f,scale,0.0f,0.0f);

         for (i=0; i<8; i++)
            glProgramEnvParameter4fARB(GL_VERTEX_PROGRAM_ARB,2+i,VTXSHADERPAR1[i],VTXSHADERPAR2[i],VTXSHADERPAR3[i],VTXSHADERPAR4[i]);
         }
      }

#endif

#endif
   }

// disable vertex shader plugin
void minicache::disablevtxshader()
   {
#ifndef NOOGL

#if defined(GL_ARB_vertex_program) && defined(GL_ARB_fragment_program)

   if (GLEXT_VP!=0 && GLEXT_FP!=0)
      if (VTXPROGID!=0)
         {
         glBindProgramARB(GL_VERTEX_PROGRAM_ARB,0);
         glDisable(GL_VERTEX_PROGRAM_ARB);
         }

#endif

#endif
   }

// set pixel shader plugin
void minicache::setpixshader(char *fp)
   {
#ifndef NOOGL

   // default pixel shader
   static char *fragprog="!!ARBfp1.0\
      PARAM c0=program.env[0];\
      PARAM c1=program.env[1];\
      PARAM c2=program.env[2];\
      PARAM c3=program.env[3];\
      PARAM c4=program.env[4];\
      PARAM c5=program.env[5];\
      PARAM c6=program.env[6];\
      PARAM c7=program.env[7];\
      PARAM t=program.env[8];\
      TEMP col;\
      TEX col,fragment.texcoord[0],texture[0],2D;\
      MUL result.color,col,fragment.color;\
      END";

   if (fp==NULL) fp=fragprog;

   if (FRAGPROG!=NULL)
      {
      if (strcmp(fp,FRAGPROG)==0) return;
      free(FRAGPROG);
      }

   FRAGPROG=strdup(fp);
   FRAGDIRTY=1;

#endif
   }

// set pixel shader parameter vector
void minicache::setpixshaderparams(float p1,float p2,float p3,float p4,int n)
   {
   if (n<0 || n>=8) ERRORMSG();

   PIXSHADERPAR1[n]=p1;
   PIXSHADERPAR2[n]=p2;
   PIXSHADERPAR3[n]=p3;
   PIXSHADERPAR4[n]=p4;
   }

// set pixel shader RGB texture map
void minicache::setpixshadertexRGB(unsigned char *image,int width,int height)
   {
   if (width<2 || height<2) ERRORMSG();

   if (PIXSHADERTEXID!=0)
      {
      deletetexmap(PIXSHADERTEXID);
      PIXSHADERTEXID=0;
      }

   if (image!=NULL)
      {
      PIXSHADERTEXID=buildRGBtexmap(image,&width,&height,0);

      PIXSHADERTEXWIDTH=width;
      PIXSHADERTEXHEIGHT=height;
      }
   }

// set shader state callbacks
void minicache::setshadercb(void (*initstatecb)(),void (*exitstatecb)())
   {
   SHADERINITSTATE_CB=initstatecb;
   SHADEREXITSTATE_CB=exitstatecb;
   }

// enable pixel shader plugin
void minicache::enablepixshader()
   {
#ifndef NOOGL

#if defined(GL_ARB_vertex_program) && defined(GL_ARB_fragment_program)

   int i;

   GLuint fragprogid;

   initglexts();
   initwglprocs();

   if (GLEXT_VP!=0 && GLEXT_FP!=0)
      {
      if (FRAGDIRTY!=0)
         {
         if (FRAGPROGID!=0)
            {
            fragprogid=FRAGPROGID;
            glDeleteProgramsARB(1,&fragprogid);
            }

         glGenProgramsARB(1,&fragprogid);
         glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB,fragprogid);
         glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB,GL_PROGRAM_FORMAT_ASCII_ARB,strlen(FRAGPROG),FRAGPROG);
         FRAGPROGID=fragprogid;

         FRAGDIRTY=0;
         }

      if (FRAGPROGID!=0)
         {
         glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB,FRAGPROGID);
         glEnable(GL_FRAGMENT_PROGRAM_ARB);

         for (i=0; i<8; i++)
            glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB,i,PIXSHADERPAR1[i],PIXSHADERPAR2[i],PIXSHADERPAR3[i],PIXSHADERPAR4[i]);

         if (GLEXT_MT!=0)
            if (PIXSHADERTEXID!=0)
               {
#ifdef GL_ARB_multitexture
               glActiveTextureARB(GL_TEXTURE1_ARB);
               bindtexmap(PIXSHADERTEXID,PIXSHADERTEXWIDTH,PIXSHADERTEXHEIGHT,0,0);
               glActiveTextureARB(GL_TEXTURE0_ARB);

               glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB,8,
                                          (float)(PIXSHADERTEXWIDTH-1)/PIXSHADERTEXWIDTH,0.5f/PIXSHADERTEXWIDTH,
                                          (float)(PIXSHADERTEXHEIGHT-1)/PIXSHADERTEXHEIGHT,0.5f/PIXSHADERTEXHEIGHT);
#endif
               }
         }
      }

#endif

#endif
   }

// disable pixel shader plugin
void minicache::disablepixshader()
   {
#ifndef NOOGL

#if defined(GL_ARB_vertex_program) && defined(GL_ARB_fragment_program)

   if (GLEXT_VP!=0 && GLEXT_FP!=0)
      if (FRAGPROGID!=0)
         {
         glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB,0);
         glDisable(GL_FRAGMENT_PROGRAM_ARB);

         if (GLEXT_MT!=0)
            if (PIXSHADERTEXID!=0)
               {
#ifdef GL_ARB_multitexture
               glActiveTextureARB(GL_TEXTURE1_ARB);
               bindtexmap(0);
               glActiveTextureARB(GL_TEXTURE0_ARB);
#endif
               }
         }

#endif

#endif
   }

// check for OpenGL extensions
void minicache::initglexts()
   {
#ifndef NOOGL

   char *gl_exts;

   if (GLSETUP==0)
      {
      GLEXT_MT=0;
      GLEXT_VP=0;
      GLEXT_FP=0;

      if ((gl_exts=(char *)glGetString(GL_EXTENSIONS))==NULL) ERRORMSG();

      if (strstr(gl_exts,"GL_ARB_multitexture")!=NULL) GLEXT_MT=1;
      if (strstr(gl_exts,"GL_ARB_vertex_program")!=NULL) GLEXT_VP=1;
      if (strstr(gl_exts,"GL_ARB_fragment_program")!=NULL) GLEXT_FP=1;

      GLSETUP=1;
      }

#endif
   }

// Windows OpenGL extension setup
void minicache::initwglprocs()
   {
#ifndef NOOGL

#ifdef _WIN32

   if (WGLSETUP==0)
      {
#ifdef GL_ARB_multitexture
      glActiveTextureARB=(PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
#endif

#if defined(GL_ARB_vertex_program) && defined(GL_ARB_fragment_program)
      glGenProgramsARB=(PFNGLGENPROGRAMSARBPROC)wglGetProcAddress("glGenProgramsARB");
      glBindProgramARB=(PFNGLBINDPROGRAMARBPROC)wglGetProcAddress("glBindProgramARB");
      glProgramStringARB=(PFNGLPROGRAMSTRINGARBPROC)wglGetProcAddress("glProgramStringARB");
      glProgramEnvParameter4fARB=(PFNGLPROGRAMENVPARAMETER4FARBPROC)wglGetProcAddress("glProgramEnvParameter4fARB");
      glDeleteProgramsARB=(PFNGLDELETEPROGRAMSARBPROC)wglGetProcAddress("glDeleteProgramsARB");
#endif

      WGLSETUP=1;
      }

#endif

#endif
   }

// get triangle fan count of active cache
int minicache::getfancnt()
   {
   if (CACHE_NUM==1) return(FANCNT2);
   else return(FANCNT1);
   }

// get vertex count of active cache
int minicache::getvtxcnt()
   {
   if (CACHE_NUM==1) return(VTXCNT2);
   else return(VTXCNT1);
   }

// configuring
void minicache::configure_overlap(float overlap) {CONFIGURE_OVERLAP=overlap;}
void minicache::configure_minsize(int minsize) {CONFIGURE_MINSIZE=minsize;}
void minicache::configure_zfight(float zscale) {CONFIGURE_ZSCALE=zscale;}
