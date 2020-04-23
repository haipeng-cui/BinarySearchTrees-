// Author: Gladys Monagan
// Represents a univariate polynomial term
// Course: CPSC 2150
// Version: October 24, 2019

#ifndef TERM_H
#define TERM_H

#include <iostream>
class Term
{
   public:
      // creates a term with the given coefficient and degree
      Term(int coefficient, int degree);

      // returns the coefficient of the term
      int getCoeff();

      // returns the degree of the term
      int getDegree(); 

      // sets the coefficient of the term to be the given value
      void setCoeff(int coefficient);

      // sets the degree of the term to be the given value
      void setDeg(int degree);
      Term(const Term& a);

      // outputs the term nicely
      friend std::ostream& operator << (std::ostream&, const Term&);

   private:
      int coeff;
      int deg;
};

#endif
