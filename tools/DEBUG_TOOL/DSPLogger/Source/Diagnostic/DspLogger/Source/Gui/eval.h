// eval.h: interface for the eval class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CEVAL_H__5ED829D4_3C15_42F9_AF99_2EFC285ACEAF__INCLUDED_)
#define AFX_CEVAL_H__5ED829D4_3C15_42F9_AF99_2EFC285ACEAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(push,3)
#include <iostream> // cout

#include <sstream>  // stringstream
#include <stack>    // stack

#include <string>   // string
#include <vector>   // vector
#pragma  warning(pop)
#pragma warning( disable : 4018 4245)
#include <cctype>   // isdigit()

using namespace std;

// define const variable for readability
const int OPERATOR = 0; 
const int OPERAND  = 1;

class Ceval {
// constructor
public: 
  Ceval();
  Ceval(const char*);

// public member function
public:
 double eval(); // get eval result

// private data member
private: 
  stack<double> operandStack; // stack to store operand
  stack<char> operatorStack;  // stack to store operator
  string infix;               // string to hold infix expression
  vector<pair<int, string> > suffix; // vector to hold suffix expression

// private member function
private:
  string char2str(const char &);      // convert char to string
  string dbl2str(const double &);     // convert double to string
  double str2dbl(const string &);     // convert string to double
  bool isoperator(const char &);      // identify whether it is an operator
  void parseOperand(const double &);  // parse operand to operandStack
  void parseOperator(const char &);   // parse operator to operatorStack
  int operatorPriority(const char&); // define operator priority
  void toSuffix(void);        // convert infix to suffix
  double calculate(const string &, const double &, const double &); // calculate result by operator and operand
};

#endif // !defined(AFX_CEVAL_H__5ED829D4_3C15_42F9_AF99_2EFC285ACEAF__INCLUDED_)
