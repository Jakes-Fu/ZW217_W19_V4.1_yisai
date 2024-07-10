// eval.cpp: implementation of the eval class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eval.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// constructor
Ceval::Ceval() {

}

// constructor
Ceval::Ceval(const char *val) {
  this->infix = string(val); // fill infix by constructor
  this->toSuffix();          // convert infix to suffix
}

// convert char to string
string Ceval::char2str(const char &c) {
  stringstream ss;
  ss << c;
 
   return ss.str();
}

// convert double to string
string Ceval::dbl2str(const double &d) {
  stringstream ss;
  ss << d;
  
  return ss.str();
}

// convert string to double
double Ceval::str2dbl(const string &s) {
  stringstream ss(s);
  double d;
  ss >> d;

  return d;
}

// identify whether it is an operator
bool Ceval::isoperator(const char &c) {
  switch(c) {
    case '(' :
    case ')' :
    case '+' :
    case '-' :
    case '*' :
    case '/' : return true;
    default  : return false;
  }
}

// parse operand to operandStack
void Ceval::parseOperand(const double &dOperand) {
  suffix.push_back(make_pair(OPERAND, dbl2str(dOperand)));
}

// parse operator to operatorStack
void Ceval::parseOperator(const char &cOperator) {
  if (operatorStack.empty() || cOperator == '(') {
    operatorStack.push(cOperator);
  }
  else {
    if (cOperator == ')') {
      while(operatorStack.top() != '(') {
        suffix.push_back(make_pair(OPERATOR, char2str(operatorStack.top())));
        operatorStack.pop();

        if (operandStack.empty()) break;
      }
      // Remove '('
      operatorStack.pop();
    }
    else { // not ')'
      while(operatorPriority(cOperator) <= operatorPriority(operatorStack.top()) && !operatorStack.empty()) {
        suffix.push_back(make_pair(OPERATOR, char2str(operatorStack.top())));
        operatorStack.pop();

        if (operatorStack.empty()) 
          break;
      }
      operatorStack.push(cOperator);
    }
  }
}

// define operator priority
int Ceval::operatorPriority(const char &cOperator) {
  switch(cOperator) {
    case '*' :
    case '/' : return 3;
    case '+' :
    case '-' : return 2;
    case '(' : return 1;
    default  : return 0;
  }
}

// Convert infix to suffix
// Algorithm : Parse infix string one char by one char. If char 
//             is operator, check if _operand is "", if not, let 
//             _operand to operandStack, and make _operand string 
//             clear, then let operator to operatorStack. If char 
//             is digit, concatenate to _operand string.
void Ceval::toSuffix(void) {
  string _operand;
  for(string::iterator p = infix.begin(); p != infix.end(); ++p) {
    if (isoperator(*p)) {
      if (_operand != "") {
        parseOperand(str2dbl(_operand));
        //_operand.clear();
		_operand.erase(_operand.begin(),_operand.end());

      }
      parseOperator(*p);
    } else if (isdigit(*p)) 
	{
      //_operand.push_back(*p);
	  char szC[5] = {0};
	  int nIndex = p - infix.begin();
	  sprintf(szC, "%c", infix[nIndex]);
	  _operand.append(szC);
	}
  }

  // If _operand is not "", let _operand to operandStack.
  if (_operand != "")
    parseOperand(str2dbl(_operand));

  // If operatorStack is not empty, push it to suffix vector until
  // operatorStack is empty.
  while(!operatorStack.empty()) {
    suffix.push_back(make_pair(OPERATOR,char2str(operatorStack.top())));
    operatorStack.pop();
  }
}

// calculate result by operator and operand
double Ceval::calculate(const string &op, const double &operand1, const double &operand2) {
  if (op == "+") 
    return operand2 + operand1;
  else if (op == "-") 
    return operand2 - operand1;
  else if (op == "*")
    return operand2 * operand1;
  else if (op == "/")
    return operand2 / operand1;
  else
    return 0;
}

// get eval result
double Ceval::eval(void) {
  // Clear OperandStack
  while(!operandStack.empty())
    operandStack.pop();

  for(vector<pair<int, string> >::iterator iter = suffix.begin(); iter != suffix.end(); ++iter) {
    if (iter->first == OPERATOR) {
      double operand1 = operandStack.top();
      operandStack.pop();
      double operand2 = operandStack.top();
      operandStack.pop();
      operandStack.push(calculate(iter->second, operand1, operand2));
    }
    else if (iter->first == OPERAND) {
      operandStack.push(str2dbl(iter->second));
    }
  }

  return operandStack.top();
}