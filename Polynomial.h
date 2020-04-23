// Name: Haipeng Cui
// Represents a univariate polynomial with a binary search tree
// Course: CPSC 2150
// Authors of initial code: Jian hua Pan and Gladys Monagan
// Version: October 24, 2019

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <functional>   // std::function
#include "Term.h"

class Polynomial 
{
   public :
      // create the zero polynomial 
      // and set the comparison function cmp between 2 term degrees
      Polynomial(std::function<bool(int, int)> cmp);
      // create a monomial with one term c x^d 
      // and set the comparison function between 2 term degrees
      Polynomial(int c, int d, std::function<bool(int, int)> cmp);
      // return the highest degree of the polynomial
      // in the special case of the zero polynomial return -1
      int degree() const;
      // return the coefficient of term that has degree n
      int coefficient(int n) const;
      // evaluate the polynomial with the given value of x
      double evaluate(double x) const; 
      // determine if the polynomial is the zero polynomial
      bool isZeroPolynomial() const;
      

// add functions as needed 
      void insert(int c,int d);
       
      // return the number of terms in the polynomial:
      // for the zero polynomial return 1
      int numberOfTerms() const;
      // add (sum) two polynomials
      const Polynomial operator + (const Polynomial& b); 

      // copy constructor
      Polynomial(const Polynomial&);
      // destructor
      ~Polynomial();
      // overloaded assignment operator
      Polynomial& operator = (const Polynomial&); 

      // output the polynomial nicely
      friend std::ostream& operator << (std::ostream&, const Polynomial&);
      // first read the number of terms n (as an int) 
      // followed by the pairs of coefficient and degree 
      // for each of the n terms
      friend std::istream& operator >> (std::istream&, Polynomial&);

    private:
      // a Node that is used to represent a polynomial stored as a 
      // BST. Each Node stores the pointer of a term and the links to its 
      // children.
      // Terms are stored in the tree in the order depending on cmpFct order, 
      // ideally, the terms in the right branch always have higher degrees 
      // than the node, while the terms in the left branch always 
      // have lower degrees than the node.
      // Note:
      //     No duplicate terms with the same degree will be in the tree.
      struct Node
      {
          Node *left;
          Term *term;
          Node *right;
      };

      // the BST is always sorted and never has duplicates
      Node * poly;

      std::function<bool(int,int)> cmpFct;
      
      static Node* insert_helper(int c,int d,Node* &t);
      static void toString(Node*p);
      static void deleteTree(Node *&t);
      static Node* copyTree(const Node* t);
      static int search(int x,const Node* t);
      static int count(Node* t);
      static int findDegree(Node *t);
      static Node* sumTwo(Node * &working, Node * rSide);

};
#endif

