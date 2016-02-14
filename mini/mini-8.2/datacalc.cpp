// (c) by Stefan Roettger

#include "minibase.h"

#include "datacalc.hpp"

// default constructor
datacalc::datacalc()
   {
   MODE=1;
   doregister();
   }

// destructor
datacalc::~datacalc()
   {deregister();}

// register with databuf objects
void datacalc::doregister()
   {databuf::setinterpreter(mystaticparser,this,mystaticinterpreter);}

// deregister with databuf objects
void datacalc::deregister()
   {databuf::setinterpreter(NULL,NULL,NULL);}

// parser hook
void datacalc::myparser(unsigned int implformat,char *program,databuf *obj)
   {
   if (implformat>0)
      {
      MODE=implformat;
      if (implformat==1) parser.setprogram(program);
      }
   }

// interpreter hook
void datacalc::myinterpreter(float *value,int comps,float x,float y,float z,float t,databuf *obj)
   {
   if (comps==1)
      switch (MODE)
         {
         // general evaluation
         case 1:
            parser.init();
            parser.pushvalue(x);
            parser.pushvalue(y);
            parser.pushvalue(z);
            parser.pushvalue(t);
            parser.execute();
            *value=parser.popvalue();
            break;
         // sphere
         case 2: *value=2.0f*fsqrt(x*x+y*y+z*z); break;
         // upper half-sphere
         case 3: *value=(z<0.0f)?1.0f:2.0f*fsqrt(x*x+y*y+z*z); break;
         // downward cone
         case 4: *value=(z>0.25f)?1.0f:fsqrt(x*x+y*y)-z/2.0f+0.25f; break;
         // cube
         case 5: *value=2.0f*fmax(fmax(fabs(x),fabs(y)),fabs(z)); break;
         // undefined
         default: ERRORMSG();
         }
   else if (comps==3 || comps==4)
      {
      if (MODE!=1) ERRORMSG();

      parser.init();

      parser.pushvalue(x);
      parser.pushvalue(y);
      parser.pushvalue(z);
      parser.pushvalue(t);

      parser.execute();

      if (comps==4) value[3]=parser.popvalue();

      value[2]=parser.popvalue();
      value[1]=parser.popvalue();
      value[0]=parser.popvalue();
      }
   else ERRORMSG();
   }

// static parser hook
void datacalc::mystaticparser(unsigned int implformat,char *program,databuf *obj,void *data)
   {
   datacalc *calc=(datacalc *)data;
   calc->myparser(implformat,program,obj);
   }

// static interpreter hook
void datacalc::mystaticinterpreter(float *value,int comps,float x,float y,float z,float t,databuf *obj,void *data)
   {
   datacalc *calc=(datacalc *)data;
   calc->myinterpreter(value,comps,x,y,z,t,obj);
   }
