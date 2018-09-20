//
// Created by rpych on 30.08.18.
//

#ifndef DERIVATIVESCALC_POLYNOMIAL_H
#define DERIVATIVESCALC_POLYNOMIAL_H
#include <iomanip>
#include <iostream>
#include "Monomial.h"
#include <list>

using namespace std;

class Polynomial{
public:
    Polynomial();

    Polynomial(const string &expression, list<Monomial> monomials,string &derivative);

    Polynomial(const Polynomial& polynomial);

    ~Polynomial();

    const string &getExpression() const;

    void setExpression(const string &expression);

     list<Monomial> &getMonomials() ;

    void setMonomials(const list<Monomial> &monomials);

    const string &getDerivative() const;

    void setDerivative(const string derivative);

    bool operator < (const Polynomial &polynomial) const;

    void calculateDerivative();

    void makePolynomial();

    double calculateFraction(string &s, string oper);

    void reduceSigns(string &tmp);

private:
    string expression;
    list<Monomial> monomials;
    string derivative;
};


#endif //DERIVATIVESCALC_POLYNOMIAL_H
