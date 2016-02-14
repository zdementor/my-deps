// (c) by Stefan Roettger

#ifndef DATACALC_H
#define DATACALC_H

#include "database.hpp"
#include "dataparse.hpp"

class datacalc
   {
   public:

   //! default constructor
   datacalc();

   //! destructor
   ~datacalc();

   //! register with databuf objects
   void doregister();

   //! deregister with databuf objects
   void deregister();

   protected:

   int MODE;

   dataparse parser;

   void myparser(unsigned int implformat,char *program,databuf *obj);
   void myinterpreter(float *value,int comps,float x,float y,float z,float t,databuf *obj);

   private:

   static void mystaticparser(unsigned int implformat,char *program,databuf *obj,void *data);
   static void mystaticinterpreter(float *value,int comps,float x,float y,float z,float t,databuf *obj,void *data);
   };

#endif
