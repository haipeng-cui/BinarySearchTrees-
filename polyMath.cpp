// Program to use (test) univariate polynomials with a user interface. 
// Commands supported 
// -> input two polynomials
// -> calculate their sum
// -> get the coefficients of some terms
// -> get the degree of the polynomial
// -> evaluate the polynomial given a specific value for x.
// -> get the number of terms in the polynomial
// All the input from a user comes from standard input (std::cin) and the output
// with results and questions for the user are done to standard output (std::cout).

// usage: polyMath -batch < input.txt > results.txt
// to run program non-interactively obtaining data from input.txt
// putting the output into results.txt
//
// usage: polyMath
// renders an interactive session with commands

// Assignment #6, CPSC 2150
// Version: October 24, 2019
// Author: Gladys Monagan (with material from Jian hua Pan)
// DO NOT POST THIS ANYWHERE.
// This code is copyrighted.
// Use it exclusively for CPSC 2150 at Langara College in Vancouver
 
#include "Polynomial.h"
#include <functional> // std::less
#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::string;

// the commands allowed
enum Commands 
{
   INPUT,
   SUM,
   PRINT,
   NUMBER_OF_TERMS,
   EVALUATE,
   GET_COEFF,
   RESET,
   DOCUMENT,
   QUIT,
   INVALID
};

// output the commands available
void printCommandMenu() 
{
   cout << "\n ------------------ command list ----------------------\n";
   cout << "(i) - input the polynomials a and b\n";
   cout << "(s) - sum up the polynomials a and b the polynomial a + b\n";
   cout << "(p) - print the polynomials a and b and a + b\n";
   cout << "(n) - number of terms in the polynomials a and b and a+b\n";
   cout << "(e) - evaluate a, b and a + b at a specific x value\n";
   cout << "(c) - coefficient of a term when the term's degree is supplied\n";
   cout << "(r) - reset or restart the process with two new polynomials\n";
   cout << "(d) - documentation, a comment, the subsequent line is ignored\n";
   cout << "(q) - quit the program altogether\n";
   cout << " --------------------------------------------------------\n"; 
}

// read from standard input a single word and check if the first
// letter corresponds to 'y' or 'Y' (implying a yes)
// return true if the string read from cin starts with 'y' or 'Y', false otherwise
// if notInteractive, the answer read is echoed onto standard output
bool yesAnswer(bool notInteractive = false) 
{
   string answerStr;
   cin >> answerStr;
   if (notInteractive) cout << answerStr << "\n";
   return (answerStr[0] == 'Y' || answerStr[0] == 'y');
}

// output to standard output a polynomial with its label
// precondition:
//    the operator << has been overloaded for Polynomial
void printPoly(const string& label, const Polynomial& poly)
{
   cout << label << " = " << poly << "\n";
}

// output to standard output the polynomial's degree with its label
void printDegree(const string& label, const Polynomial& poly) 
{
   cout << "degree of " << label << " = " << poly.degree() << "\n";
}

// output to standard output the polynomials a and b
// precondition:
//    aPoly, bPoly are bona fide polynomials 
//    (including the zero polynomial)
void printPolys(const Polynomial& aPoly, const Polynomial& bPoly)
{
   printPoly("a", aPoly);
   printPoly("b", bPoly);
   printDegree("a", aPoly);
   printDegree("b", bPoly);
}

// output to standard output the coefficient and degree with a blurb
void printCoeffDegree(string label, int coefficient, int degree)
{
   cout << "in " << label << ", " << coefficient;
   cout << " is the coefficient of the term of degree " << degree << "\n";
}

// output to standard output the polynomials a, b and their sum 
// precondition:
//    aPoly, bPoly and sumPoly are bona fide polynomials 
//    (including the zero polynomial)
void printPolys(const Polynomial& aPoly, const Polynomial& bPoly, const Polynomial& sumPoly)
{
   printPoly("a", aPoly);
   printPoly("b", bPoly);
   printPoly("a+b", sumPoly);
   printDegree("a", aPoly);
   printDegree("b", bPoly);
   printDegree("a+b", sumPoly);
}

// output to standard output the number of terms of the aPoly, bPoly and sumPoly
// precondition:
//    aPoly, bPoly, sumPoly are bona fide polynomials 
//    (including the zero polynomial)
void numberOfTerms(const Polynomial& aPoly, const Polynomial& bPoly, 
   const Polynomial& sumPoly)
{
   cout << aPoly.numberOfTerms() << " term(s) in a = " << aPoly << "\n";
   cout << bPoly.numberOfTerms() << " term(s) in b = " << bPoly << "\n";
   cout << sumPoly.numberOfTerms() << " term(s) in sum = " << sumPoly << "\n";
}

// read from standard input a command and obtain the first letter of that 
// command to match it to a corresponding value from the enum type Commands
// if notInteractive, the command read is echoed to standard output
// postcondition:
//    returns a command from Commands (including INVALID)
Commands getCommand(bool notInteractive)
{
   Commands command;
   cout << "\ncommand: ";
   string commandStr;
   cin >> commandStr;
   if (notInteractive) cout << commandStr << "\n";  // echo what was read

   char cmd = commandStr[0];
   if ('A' <= cmd && cmd <= 'Z') { // if upper case,
      cmd = (cmd - 'A') + 'a'; // translate to lower case
   }
   switch (cmd) {
      case 'i':
         command = INPUT;
         break;
      case 's':
         command = SUM;
         break;
      case 'p':
         command = PRINT;
         break;
      case 'e':
         command = EVALUATE;
         break;
      case 'c':
         command = GET_COEFF;
         break;
      case 'r':
         command = RESET;
         break;
      case 'n':
         command = NUMBER_OF_TERMS;
         break;
      case 'd':
         command = DOCUMENT;
         break;
      case 'q':
         command = QUIT;
         break;
      default:
         command = INVALID;
         break;
   } 
   return command;
}

// reset the input stream cin (out of its fail state) 
// and clear the buffer
// postcondition:
//    cin is ready to read again
void cinReset()
{
   static const int MAX_CHARACTERS_IN_BUFFER = 250;
   cin.clear();
   cin.ignore(MAX_CHARACTERS_IN_BUFFER, '\n');
}

// read from standard input the info on the first polynomial (polynomial a)
// if successful, read the info on the second polynomial (polynomial b)
// expected for each polynomial is
// -> the number of terms
// -> pairs of numbers corresponding to the coefficient of the term and the degree
// precondition:
//    aPoly, bPoly, sumPoly are the zero polynomials
//    the operator << has been defined for Polynomials
//    reading into a polynomial consists of
//        an integer n for the number of terms
//        coefficient degree pairs n times    
// postcondition: 
//    aPoly, bPoly are bona fide polynomials 
//    the input stream cin is cleared in case of having read non-numeric input
bool inputPolynomials(Polynomial& aPoly, Polynomial& bPoly)
{
   bool successfulRead = true;
   cout << "for polynomial a, enter # of terms,";
   cout << " then \"coefficient degree\" pairs:\n";
   if (!(cin >> aPoly)) 
   {
      cout << "ERROR in reading polynomial a\n";
      successfulRead = false;
   }
   
   if (successfulRead) 
   {
      cout << "for polynomial b, enter # of terms,";
      cout << " then \"coefficient degree\" pairs:\n";
      if (!(cin >> bPoly)) 
      {
         cout << "ERROR in reading polynomial b\n";
         successfulRead = false;
      }
   }
   cinReset();
   return successfulRead;
}

// read from standard input the degree of the term whose coefficients will
// be printed out for each of the three polynomials
// if notInteractive is true, the read degree is echoed to standard output
// postcondition:
//    the input stream cin is cleared in case of having read non-numeric input
void getCoeffs(const Polynomial& aPoly, const Polynomial& bPoly, 
   const Polynomial& sumPoly, bool notInteractive)
{
   do
   {
      int degree;
      cout << "enter degree of polynomial term whose coefficient is needed ";
      cin >> degree;
      if (notInteractive) cout << degree << "\n";
      printCoeffDegree("a", aPoly.coefficient(degree), degree);
      printCoeffDegree("b", bPoly.coefficient(degree), degree);
      printCoeffDegree("a+b", sumPoly.coefficient(degree), degree);
      cout << "continue getting coefficients? (y/n): ";
   } while (yesAnswer(notInteractive));
   cinReset();
}

// read from standard input the value of x to use in evaluating the polynomials
// evaluate a, b and a + b with the given value of x and output the results
// if notInteractive is true, the value read is echoed to standard output
// postcondition:
//    the input stream cin is cleared in case of having read non-numeric input
void evalPolys(const Polynomial& aPoly, const Polynomial& bPoly, 
               const Polynomial& sumPoly, bool notInteractive)
{
   do
   {
      double x;
      cout << "enter the value of x: ";
      cin >> x;
      if (notInteractive) cout << x << "\n";
      cout << "when x = " << x << "\n";
      cout << "a = " << aPoly << " = " << aPoly.evaluate(x) << "\n";
      cout << "b = " << bPoly << " = " << bPoly.evaluate(x) << "\n";
      cout << "a+b = " << sumPoly << " = " << sumPoly.evaluate(x) << "\n";
      cout << "evaluate for another value of x? (y/n) : ";
   } while (yesAnswer(notInteractive));
   cinReset();
}

// read from standard input a line and output it to standard output if notInteractive
void getNextLine(bool notInteractive)
{
   string line;
   std::getline(cin, line); // consume the end-of-line
   std::getline(cin, line);
   if (notInteractive) cout << line << "\n";
}

// process the command 
//   read into aPoly and bPoly 
//   compute the polynomial which is the sum of aPoly and bPoly
//   print aPoly, bPoly and sumPoly
//   evaluate aPoly, bPoly and sumPoly asking the user for the value of x
//   get the coefficients of terms in aPoly, bPoly and sumPoly 
//   get the number of terms in aPoly, bPoly and sumPoly
//   print to standard output if an error was made
//   read a comment (or document) and echo it if necessary
// postcondition:
//   return true when the command is RESET or QUIT, returns false otherwise
// 
bool processCommand(Commands command, bool notInteractive,
   Polynomial& aPoly, Polynomial& bPoly, Polynomial& sumPoly)
{   
   bool doneProcessing = false;
   switch (command)
   {
      case INPUT:
         // use the overloaded operator << of Polynomial to enter info
         if (inputPolynomials(aPoly, bPoly)) printPolys(aPoly, bPoly);
         break;
      case SUM:
         sumPoly = aPoly + bPoly;
         printPolys(aPoly, bPoly, sumPoly);
         break;
      case PRINT:
         // print the three polynomials
         printPolys(aPoly, bPoly, sumPoly);
         break;
      case EVALUATE:
         // ask the user for the values of x and evaluate the polynomials
         evalPolys(aPoly, bPoly, sumPoly, notInteractive);
         break;
      case GET_COEFF:
         // ask the user for the degree of the term whose coefficient will be returned
         getCoeffs(aPoly, bPoly, sumPoly, notInteractive);
         break;
      case NUMBER_OF_TERMS:
         // output the number of terms of each polynomial
         numberOfTerms(aPoly, bPoly, sumPoly);
         break;
      case RESET:
         // will be used to restart the process eliminating the polynomials
         doneProcessing = true;
         break;
      case DOCUMENT:
         getNextLine(notInteractive);
         break;
      case QUIT:
         // the task is done
         doneProcessing = true;
         break;
      case INVALID:
         cout << "invalid command, please input again!\n";
         break;
      default:
         std::cerr << "**** ERROR in the switch of processCommand ***\n";
         break;
   }
   return doneProcessing;
}

// determine if the "-batch" option was entered meaning that the
// session is NOT interactive
// return true if the string matches -batch, false otherwise
bool notInteractiveOption(string option) {
   return option == "-batch";
} 

// if the session is interactive, echo the input
// for each pair of polynomials read, do the operations given in the command
// finish once the command is quit
int main(int argc, char* argv[]) {
   bool notInteractive = false;
   if (argc > 1) 
   {
      notInteractive = notInteractiveOption(argv[1]);
   }

   struct MyFunctor 
   {
      bool operator() (int a, int b) 
      {
         return a > b;
      }
   };
   Commands cmd;
   do 
   {
      if (!notInteractive) printCommandMenu();

      // process two polynomials and their sum at a time 
      bool doneWithThesePolys = false;
      Polynomial aPoly([](int a, int b) -> bool {return a > b;});
      Polynomial bPoly((std::greater<int>()));
      MyFunctor f;
      Polynomial sumPoly(f); 
      do 
      {
         cmd = getCommand(notInteractive);
         doneWithThesePolys = processCommand(cmd, notInteractive, aPoly, bPoly, sumPoly);
      } while (!doneWithThesePolys); 
      // calls the destructors
      
   } while (cmd != QUIT); 
   return 0;
}
