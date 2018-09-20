//
// Created by rpych on 05.09.18.
//

#ifndef DERIVATIVESCALC_TERM_H
#define DERIVATIVESCALC_TERM_H
#include <iomanip>
#include <iostream>
#include <string>
#include "Polynomial.h"
#include "Monomial.h"
using namespace std;


class Term{
public:
    Term();

    Term( string &term,  string &derivative,  char &opr, double exp);

    const string &getTerm() const;

    void setTerm(const string &term);

    const string &getDerivative() const;

    void setDerivative(const string &derivative);

    const char &getOpr() const;

    void setOpr(const char &opr);

    double getExp() const;

    void setExp(double exp);

    void fetchOperator();

    void deleteOuterBrackets(string &t);

    void calculateComplexDerivative(list<Polynomial> polynomials, list<Term> terms);

    list<string> extractFactors();

    void makeMultiplicationOrDivision(list<Polynomial> polynomials, list<Term> terms);

    void makeExponentialComplexDerivative(list<Polynomial> polynomials, list<Term> terms);

    string setExponent(string& term, int i);


private:
    string term;
    string derivative;
    char opr; //operator - main (outer) arithmetic operator in expression
    double exp = 1.0; //optional field, used while '^' operator exists

};



#endif //DERIVATIVESCALC_TERM_H
