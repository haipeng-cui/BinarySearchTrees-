// Author: Gladys Monagan
// Represents a univariate polynomial term
// Course: CPSC 2150
// Version: October 24, 2019

#include "Term.h"
#include <iostream>

Term::Term(int coefficient, int degree): coeff(coefficient), deg(degree) 
{
}

int Term::getCoeff() 
{
   return coeff;
}

int Term::getDegree()
{
   return deg;
} 

Term::Term(const Term& a){
   coeff = a.coeff;
   deg = a.deg;
   }

void Term::setCoeff(int coefficient) {
   coeff = coefficient;
}

void Term::setDeg(int degree) {
   deg = degree;
}

std::ostream& operator << (std::ostream& out, const Term& t)
{
   int coeff = t.coeff;
   if (coeff > 0)
   {
      out << " + ";
   }
   else if (coeff < 0)
   {
      out << " - ";
      coeff *= -1;
   }

   if (coeff != 1 || t.deg == 0)
   {
      out << coeff;
   }

   if (t.deg == 1)
   {
      out << "x";
   }
   else if (t.deg != 0)
   {
      out << "x^" << t.deg;
   }
   return out;
}

