// Name: Haipeng Cui
// Represents a univariate polynomial with a binary search tree
// Course: CPSC 2150
// Version: October 24, 2019

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <functional>   // std::function

#include "Term.h"
#include "Polynomial.h"

using std::string;

// create the zero polynomial, represented by the nullptr
Polynomial::Polynomial(std::function<bool(int, int)> fct) : poly(nullptr)
{
   cmpFct = fct;
}


// determine if the polynomial is the zero polynomial
bool Polynomial::isZeroPolynomial() const
{ 
   return poly == nullptr;
}

// precondition:
//    c is the coefficient of the only term in the polynomial
//    d is the degree of the only term in the polynomial
// postcondition: 
//    a Polynomial with one term c x^d is created
Polynomial::Polynomial(int c, int d, std::function<bool(int, int)> fct)
{
   cmpFct = fct;
   poly = new Node;
   Term temp(c,d);
   poly->term = &temp;
   poly->left = nullptr;
   poly->right = nullptr;
}

// precondition:
//    p is the Polynomial we are copying from
// postcondition: 
//    a Polynomial which is a copy of p is created
Polynomial::Polynomial(const Polynomial& p)
{
   poly = copyTree(p.poly);
   cmpFct = p.cmpFct;
}

Polynomial::Node* Polynomial::copyTree(const Node* t){
   if(t==nullptr){
      return nullptr;
      }
   Node *p = new Node;
   p->term = new Term(t->term->getCoeff(),t->term->getDegree());
      
   p->left=copyTree(t->left);
   p->right=copyTree(t->right);

   return p;
   }

// destructor frees up the memory
Polynomial::~Polynomial()
{
   Polynomial::deleteTree(poly);
}
void Polynomial::deleteTree(Node *&t){
   if(t==nullptr)
      return;
   deleteTree(t->left);
   deleteTree(t->right);
   delete t->term;
   delete t;
   t=nullptr;
   }

// postcondition: 
//    returns the coefficient of degree n of the polynomial p
int Polynomial::coefficient(int n) const
{  
   if(poly == nullptr)
      return 0;
   else
      return search(n,poly);
 
}
int Polynomial::search(int n,const Polynomial::Node* t){
   if(t==nullptr)
      return 0;
   if(t->term->getCoeff() == n)
      return t->term->getDegree();
   if(n < t->term->getCoeff())
      return search(n,t->left);
   else
      return search(n,t->right);
   }


// postcondition: 
//    returns the number of terms in the tree
//    in the special case of the zero polynomial return 1
int Polynomial::numberOfTerms() const 
{
  return count(poly);
}

int Polynomial::count(Polynomial::Node* t){
      if (t == nullptr) {  
            return 0;  
        } else {  
            return count(t->left) + count(t->right) + 1;  
        }
   
   }

// postcondition: 
//    returns the highest degree of the polynomial
//    in the special case of the zero polynomial, return -1 
int Polynomial::degree() const
{
  return  findDegree(poly);
}

int Polynomial::findDegree(Polynomial::Node *t){
      if(t==nullptr)
         return 0;
      else {
		   if(t->right != nullptr)
			   return findDegree(t->right);
		   else
			   return t->term->getCoeff();
         }
   }

// postcondition: 
//    returns the value of evaluating the polynomial at x
double Polynomial::evaluate(double x) const
{
  return x; //not finish yet
}

// postcondition: 
//    if non numeric input is given, the istream enters a fail state
//    only non-zero terms will be stored in the polynomial
std::istream& operator >> (std::istream& in, Polynomial& newpoly) 
{
   int count;
   in >> count;
   while(count != 0){
      
      int c;
      int d;
      in >> c;
      in >> d;

      newpoly.insert(c,d);
      count--;
      }
      std::cout<<std::endl;
   return in;
}

void Polynomial::insert(int c, int d){
   
   poly = insert_helper(c,d,poly);
   }
Polynomial::Node* Polynomial::insert_helper(int c, int d, Node * &t){
   if(t == nullptr){
         Node *p = new Node;
         Term *temp = new Term(c,d);
         p->term = temp;
         p->left=nullptr;
         p->right= nullptr;
      return p;
      }
   if(d == t->term->getDegree() ){
      t->term->setCoeff(t->term->getCoeff()+c);
      return t;
      }
   if(d * (-1) == t->term->getDegree() ){
      t->term->setCoeff(0);//not right, not finish
      return t;
      }
   if( d < t->term->getDegree() ){
      t->left=insert_helper(c,d,t->left);
      }
   else {
      t->right=insert_helper(c,d,t->right);
      }
   return t;
   }

// precondition:
//    a and b are the two polynomials to sum up,
// postcondition: 
//    create a new polynomial which is the sum of two given polynomials
//    only the non-zero terms will be stored in the sum polynomial 
const Polynomial Polynomial::operator + (const Polynomial& b)
{  
  if(poly == nullptr){
     return b;
     }
   if(b.poly == nullptr){
      return *this;
      }
   Polynomial temp = *this;// not finish yet!!!
   sumTwo(temp.poly,b.poly);
   return temp;

}

Polynomial::Node* Polynomial::sumTwo(Node * &working, Node * rSide){
   
      if(rSide == nullptr){
         return working;
         }
      insert_helper(rSide->term->getCoeff(),rSide->term->getDegree(),working);
      sumTwo(working,rSide->left);
      sumTwo(working,rSide->right);
      return working;
   
   }
// postcondition:
//    deep copy of the rtSide was made      
Polynomial& Polynomial::operator = (const Polynomial& rtSide)
{  if(this ==&rtSide){
      return *this;
   }
   deleteTree(poly);
   poly = copyTree(rtSide.poly);
   cmpFct = rtSide.cmpFct;
   return *this;

}

// postcondition:     
//    outputs the polynomial, starting with the highest degree
std::ostream& operator << (std::ostream& out, const Polynomial& p)
{  if(p.poly==nullptr){
      out<<"0";
      return out;}
   p.toString(p.poly);
   return out;
}
void Polynomial::toString(Node*p){
   if(p==nullptr)
      return;
   toString(p->right);
   
   if(p->term->getCoeff() >0) {
				std::cout<<"+";
			}
   std::cout<<p->term->getCoeff();
   std::cout <<"x^"<<p->term->getDegree();

   toString(p->left);
   }