// (c) by Stefan Roettger

/*

This module parses an UPN-style language according to the following EBNF grammar:

program     ::= { statement | ( "include" <incl-id> ) }
statement   ::= expression |
                ( "{" { statement | declaration } "}" ) |
                ( "if" expression statement [ "else" statement ] ) |
                ( "while" expression statement ) |
                ( "repeat" statement "until" expression ) |
                ( "for" expression expression expression statement ) |
                ( "return" expression ) |
                ( "warn" expression ) |
                "break"
declaration ::= var_decl | array_decl | func_decl
var_decl    ::= "var" | "byte" | "short" | "int" <var-id> [ "=" expression ] { "," <var-id> [ "=" expression ] }
array_decl  ::= "array" | "byte" | "short" | "int" <array-id> [ "[" { expression } "]" ] { "," <array-id> [ "[" { expression } "]" ] }
func_decl   ::= "func" <func-id> [ "(" { expression } [ "," { expression } ] ")" ] statement
expression  ::= (["-"] <float-val>) | "args" |
                ( "(" { expression } ")" ) |
                ( <var-id> [ ( "=" expression ) | "++" | "--" ] ) |
                ( <array-id> [ "[" { expression } "]" ] [ "=" expression ] ) |
                ( <func-id> | operators expression )
operators   ::= "+" | "-" | "*" | "/" | "-*" | "%" | "#" |
                "<" | ">" | "<=" | ">=" | "==" | "!=" | "!" | "false" | "true" |
                min | max | sqr | sqrt | exp | log | pow |
                sin | cos | tan | atan

*/

#ifndef DATAPARSE_H
#define DATAPARSE_H

class dataparse
   {
   public:

   //! default constructor
   dataparse();

   //! destructor
   ~dataparse();

   //! set the program to be parsed and executed
   void setprogram(char *program);

   //! completely reset the parser
   void reset();

   //! push one value onto the computation stack
   void pushvalue(float v);

   //! pop one value from the computation stack
   float popvalue();

   //! completely reset the interpreter
   void init();

   //! execute the previously parsed program
   void execute();

   protected:

   private:
   };

#endif
