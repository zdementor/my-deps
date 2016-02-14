// (c) by Stefan Roettger

#include "minibase.h"

#include "miniOGL.h"

#include "minitext.hpp"

float minitext::CONFIGURE_ZSCALE=0.95f; // must be 1.0f for orthographic projections

// configuring
void minitext::configure_zfight(float zscale) {CONFIGURE_ZSCALE=zscale;}

void minitext::drawline(float x1,float y1,float x2,float y2,
                        float hue,float sat,float val,float alpha)
   {
#ifndef NOOGL
   float rgb[3];

   hsv2rgb(hue,sat,val,rgb);

   glColor4f(rgb[0],rgb[1],rgb[2],alpha);
   glBegin(GL_LINES);
   glVertex2f(x1,y1);
   glVertex2f(x2,y2);
   glEnd();
#endif
   }

void minitext::drawlineRGBA(float x1,float y1,float x2,float y2,
                            float r,float g,float b,float alpha)
   {
#ifndef NOOGL
   glColor4f(r,g,b,alpha);
   glBegin(GL_LINES);
   glVertex2f(x1,y1);
   glVertex2f(x2,y2);
   glEnd();
#endif
   }

void minitext::drawquad(float x,float y,float width,float height,
                        float hue,float sat,float val,float alpha)
   {
#ifndef NOOGL
   float rgb[3];

   hsv2rgb(hue,sat,val,rgb);

   glColor4f(rgb[0],rgb[1],rgb[2],alpha);
   glBegin(GL_TRIANGLE_FAN);
   glVertex2f(x,y);
   glVertex2f(x+width,y);
   glVertex2f(x+width,y+height);
   glVertex2f(x,y+height);
   glEnd();
#endif
   }

void minitext::drawquadRGBA(float x,float y,float width,float height,
                            float r,float g,float b,float alpha)
   {
#ifndef NOOGL
   glColor4f(r,g,b,alpha);
   glBegin(GL_TRIANGLE_FAN);
   glVertex2f(x,y);
   glVertex2f(x+width,y);
   glVertex2f(x+width,y+height);
   glVertex2f(x,y+height);
   glEnd();
#endif
   }

void minitext::drawframe(float x,float y,float width,float height,
                         float hue,float sat,float val,float alpha)
   {
#ifndef NOOGL
   float rgb1[3],rgb2[3];

   hsv2rgb(hue,sat,fmax(val-0.25f,0.0f),rgb1);
   hsv2rgb(hue,sat,fmin(val+0.25f,1.0f),rgb2);

   glBegin(GL_LINES);
   glColor4f(rgb1[0],rgb1[1],rgb1[2],alpha);
   glVertex2f(x,y);
   glVertex2f(x+width,y);
   glVertex2f(x+width,y);
   glVertex2f(x+width,y+height);
   glColor4f(rgb2[0],rgb2[1],rgb2[2],alpha);
   glVertex2f(x+width,y+height);
   glVertex2f(x,y+height);
   glVertex2f(x,y+height);
   glVertex2f(x,y);
   glEnd();
#endif
   }

void minitext::drawframeRGBA(float x,float y,float width,float height,
                             float r,float g,float b,float alpha)
   {
#ifndef NOOGL
   glColor4f(r,g,b,alpha);
   glBegin(GL_LINES);
   glVertex2f(x,y);
   glVertex2f(x+width,y);
   glVertex2f(x+width,y);
   glVertex2f(x+width,y+height);
   glVertex2f(x+width,y+height);
   glVertex2f(x,y+height);
   glVertex2f(x,y+height);
   glVertex2f(x,y);
   glEnd();
#endif
   }

void minitext::drawsymbol(float hue,float sat,float val,float alpha,char *symbol)
   {
#ifndef NOOGL
   float rgb[3];

   float px,py,lx,ly;

   BOOLINT draw=TRUE;

   hsv2rgb(hue,sat,val,rgb);

   glColor4f(rgb[0],rgb[1],rgb[2],alpha);
   glBegin(GL_LINES);

   px=py=0.0f;

   while (*symbol!='\0')
      {
      lx=px;
      ly=py;

      switch (*symbol++)
         {
         case 'u': draw=FALSE; break;
         case 'd': draw=TRUE; break;
         case 'n': py+=1.0f; break;
         case 's': py-=1.0f; break;
         case 'e': px+=1.0f; break;
         case 'w': px-=1.0f; break;
         case 'N': py+=1.0f; px+=1.0f; break;
         case 'S': py-=1.0f; px-=1.0f; break;
         case 'E': px+=1.0f; py-=1.0f; break;
         case 'W': px-=1.0f; py+=1.0f; break;
         }

      if (draw)
         if (px!=lx || py!=ly)
            {
            glVertex2f(lx,ly);
            glVertex2f(px,py);
            }
      }

   glEnd();
#endif
   }

void minitext::drawletter(float hue,float sat,float val,float alpha,char letter)
   {
#ifndef NOOGL
   glPushMatrix();
   glScalef(1.0f/4,1.0f/6,0.0f);

   switch (toupper(letter))
      {
      // letters occupy a 7x4 grid
      case 'A': drawsymbol(hue,sat,val,alpha,"nnnnnNeEsssssunnndwww"); break;
      case 'B': drawsymbol(hue,sat,val,alpha,"nnnnnneeEsSwwueedEsSww"); break;
      case 'C': drawsymbol(hue,sat,val,alpha,"ueeNdSwWnnnnNeE"); break;
      case 'D': drawsymbol(hue,sat,val,alpha,"nnnnnneeEssssSww"); break;
      case 'E': drawsymbol(hue,sat,val,alpha,"ueeedwwwnnnnnneeeussswdww"); break;
      case 'F': drawsymbol(hue,sat,val,alpha,"nnnnnneeeussswdww"); break;
      case 'G': drawsymbol(hue,sat,val,alpha,"unnneedessSwWnnnnNeE"); break;
      case 'H': drawsymbol(hue,sat,val,alpha,"nnnnnnueeedssssssunnndwww"); break;
      case 'I': drawsymbol(hue,sat,val,alpha,"uedeeuwdnnnnnneuwdw"); break;
      case 'J': drawsymbol(hue,sat,val,alpha,"undEeNnnnnnwww"); break;
      case 'K': drawsymbol(hue,sat,val,alpha,"nnnnnnusssdNNNuSSSdEEE"); break;
      case 'L': drawsymbol(hue,sat,val,alpha,"ueeedwwwnnnnnn"); break;
      case 'M': drawsymbol(hue,sat,val,alpha,"nnnnnnEeNssssss"); break;
      case 'N': drawsymbol(hue,sat,val,alpha,"nnnnnnusdEEEssunndnnnn"); break;
      case 'O': drawsymbol(hue,sat,val,alpha,"uedWnnnnNeEssssSw"); break;
      case 'P': drawsymbol(hue,sat,val,alpha,"nnnnnneeEsSww"); break;
      case 'Q': drawsymbol(hue,sat,val,alpha,"uedWnnnnNeEssssSwuNdE"); break;
      case 'R': drawsymbol(hue,sat,val,alpha,"nnnnnneeEsSwwuedEEs"); break;
      case 'S': drawsymbol(hue,sat,val,alpha,"undEeNnWwWnNeE"); break;
      case 'T': drawsymbol(hue,sat,val,alpha,"ueednnnnnnwwueedee"); break;
      case 'U': drawsymbol(hue,sat,val,alpha,"unnnnnndsssssEeNnnnnn"); break;
      case 'V': drawsymbol(hue,sat,val,alpha,"unnnnnndsssssENNnnnn"); break;
      case 'W': drawsymbol(hue,sat,val,alpha,"unnnnnndssssssNeEnnnnnn"); break;
      case 'X': drawsymbol(hue,sat,val,alpha,"nnNNNnuwwwdsEEEss"); break;
      case 'Y': drawsymbol(hue,sat,val,alpha,"ueednnWWnnueeedsssS"); break;
      case 'Z': drawsymbol(hue,sat,val,alpha,"ueeendswwwnnNNNnwwws"); break;
      case '_': drawsymbol(hue,sat,val,alpha,"eee"); break;
      case '0': drawsymbol(hue,sat,val,alpha,"uedWnnnnNeEssssSwuNNdWWW"); break;
      case '1': drawsymbol(hue,sat,val,alpha,"ueednnnnnnSS"); break;
      case '2': drawsymbol(hue,sat,val,alpha,"ueeedwwwNNNnnWwSs"); break;
      case '3': drawsymbol(hue,sat,val,alpha,"undEeNnWwuedNnWwS"); break;
      case '4': drawsymbol(hue,sat,val,alpha,"ueednnnnnnuwwdssseee"); break;
      case '5': drawsymbol(hue,sat,val,alpha,"undEeNnWwwnnneee"); break;
      case '6': drawsymbol(hue,sat,val,alpha,"unndNeEsSwWnnnnNeE"); break;
      case '7': drawsymbol(hue,sat,val,alpha,"ueednnnnnnwwusssedee"); break;
      case '8': drawsymbol(hue,sat,val,alpha,"uedWnNWnNeEsSwuedEsSw"); break;
      case '9': drawsymbol(hue,sat,val,alpha,"undEeNnnnnWwSsEee"); break;
      case '.': drawsymbol(hue,sat,val,alpha,"uednesw"); break;
      case ',': drawsymbol(hue,sat,val,alpha,"uedN"); break;
      case ':': drawsymbol(hue,sat,val,alpha,"unedneswunndnesw"); break;
      case ';': drawsymbol(hue,sat,val,alpha,"uneedwnesSunnndnesw"); break;
      case '?': drawsymbol(hue,sat,val,alpha,"ueednnnNnWwS"); break;
      case '!': drawsymbol(hue,sat,val,alpha,"ueednundnnnn"); break;
      case '|': drawsymbol(hue,sat,val,alpha,"ueednnnnnn"); break;
      case '-': drawsymbol(hue,sat,val,alpha,"unnnedee"); break;
      case '+': drawsymbol(hue,sat,val,alpha,"unnnedeeunwdss"); break;
      case '*': drawsymbol(hue,sat,val,alpha,"unnedNNuwwdEE"); break;
      case '/': drawsymbol(hue,sat,val,alpha,"undNNN"); break;
      case '>': drawsymbol(hue,sat,val,alpha,"unedNNWW"); break;
      case '<': drawsymbol(hue,sat,val,alpha,"uneeedWWNN"); break;
      case '=': drawsymbol(hue,sat,val,alpha,"unnedeeunndww"); break;
      case '(': drawsymbol(hue,sat,val,alpha,"ueedWnnnnN"); break;
      case ')': drawsymbol(hue,sat,val,alpha,"uedNnnnnW"); break;
      case '[': drawsymbol(hue,sat,val,alpha,"ueedwnnnnnne"); break;
      case ']': drawsymbol(hue,sat,val,alpha,"uedennnnnnw"); break;
      case '{': drawsymbol(hue,sat,val,alpha,"ueedwnnWNnne"); break;
      case '}': drawsymbol(hue,sat,val,alpha,"udennNWnnw"); break;
      case '$': drawsymbol(hue,sat,val,alpha,"unndEeNWwWNeEuWndssssssuwdnnnnnn"); break;
      case '%': drawsymbol(hue,sat,val,alpha,"undNNNusssdswneuwwnnndnwse"); break;
      case '#': drawsymbol(hue,sat,val,alpha,"unndeeeunndwwwuNdssssuednnnn"); break;
      case '&': drawsymbol(hue,sat,val,alpha,"ueeedWWWnnNEssSSseNN"); break;
      case '@': drawsymbol(hue,sat,val,alpha,"ueeendSwWnnnnNeEssswwnnee"); break;
      case '^': drawsymbol(hue,sat,val,alpha,"uennnndNE"); break;
      case '~': drawsymbol(hue,sat,val,alpha,"unnnndNEN"); break;
      case '"': drawsymbol(hue,sat,val,alpha,"uennnndnueds"); break;
      case '\'': drawsymbol(hue,sat,val,alpha,"uennnndn"); break;
      }

   glPopMatrix();
#endif
   }

void minitext::drawstring(float width,
                          float hue,float sat,float val,float alpha,char *str,
                          float backval,float backalpha)
   {
#ifndef NOOGL
   const float linefeed=0.2f;

   int c,cmax,l;
   float scale;
   char *ptr;

   if (str==NULL) return;

   for (c=0,cmax=l=1,ptr=str; *ptr!='\0'; ptr++)
      {
      if (*ptr!='\n') c++;
      else {c=0; l++;}
      if (c>cmax) cmax=c;
      }

   scale=width/cmax;

   if (backalpha>0.0f)
      {
      drawquad(0.0f,0.0f,scale*cmax,scale*(l+(l-1)*linefeed),0.0f,0.0f,backval,backalpha);

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glScalef(CONFIGURE_ZSCALE,CONFIGURE_ZSCALE,CONFIGURE_ZSCALE); // prevent Z-fighting
      glMatrixMode(GL_MODELVIEW);
      }

   glPushMatrix();
   glScalef(scale,scale,0.0f);
   glTranslatef(0.0f,(l-1)*(1.0f+linefeed),0.0f);
   glPushMatrix();

   while (*str!='\0')
      {
      if (*str=='\n')
         {
         glPopMatrix();
         glTranslatef(0.0f,-(1.0f+linefeed),0.0f);
         glPushMatrix();
         }
      else
         {
         if (islower(*str))
            {
            glPushMatrix();
            glTranslatef(0.2f,0.0f,0.0f);
            glScalef(0.6f,0.75f,1.0f);
            }

         drawletter(hue,sat,val,alpha,*str);

         if (islower(*str)) glPopMatrix();

         glTranslatef(1.0f,0.0f,0.0f);
         }

      str++;
      }

   glPopMatrix();
   glPopMatrix();

   if (backalpha>0.0f)
      {
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      }
#endif
   }

// rgb to hsv conversion
void minitext::rgb2hsv(float r,float g,float b,float *hsv)
   {
   float h,s,v;
   float maxv,minv,diff,rdist,gdist,bdist;

   if (r<0.0f || r>1.0f || g<0.0f || g>1.0f || b<0.0f || b>1.0f) ERRORMSG();

   maxv=fmax(r,fmax(g,b));
   minv=fmin(r,fmin(g,b));
   diff=maxv-minv;

   v=maxv;

   if (maxv!=0.0f) s=diff/maxv;
   else s=0.0f;

   if (s==0.0f) h=0.0f;
   else
      {
      rdist=(maxv-r)/diff;
      gdist=(maxv-g)/diff;
      bdist=(maxv-b)/diff;

      if (r==maxv) h=bdist-gdist;
      else if (g==maxv) h=2.0f+rdist-bdist;
      else h=4.0f+gdist-rdist;

      h*=60.0f;
      if (h<0.0f) h+=360.0f;
      }

   hsv[0]=h;
   hsv[1]=s;
   hsv[2]=v;
   }

// hsv to rgb conversion
void minitext::hsv2rgb(float hue,float sat,float val,float *rgb)
   {
   float hue6,r,s,t;

   if (hue<0.0f || sat<0.0f || sat>1.0f || val<0.0f || val>1.0f) ERRORMSG();

   hue/=60.0f;
   hue=hue-6.0f*ftrc(hue/6.0f);
   hue6=hue-ftrc(hue);

   r=val*(1.0f-sat);
   s=val*(1.0f-sat*hue6);
   t=val*(1.0f-sat*(1.0f-hue6));

   switch (ftrc(hue))
        {
        case 0: // red -> yellow
           rgb[0] = val;
           rgb[1] = t;
           rgb[2] = r;
           break;
        case 1: // yellow -> green
           rgb[0] = s;
           rgb[1] = val;
           rgb[2] = r;
           break;
        case 2: // green -> cyan
           rgb[0] = r;
           rgb[1] = val;
           rgb[2] = t;
           break;
        case 3: // cyan -> blue
           rgb[0] = r;
           rgb[1] = s;
           rgb[2] = val;
           break;
        case 4: // blue -> magenta
           rgb[0] = t;
           rgb[1] = r;
           rgb[2] = val;
           break;
        case 5: // magenta -> red
           rgb[0] = val;
           rgb[1] = r;
           rgb[2] = s;
           break;
        }
   }
