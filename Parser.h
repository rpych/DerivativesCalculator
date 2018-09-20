//
// Created by rpych on 30.08.18.
//

#ifndef DERIVATIVESCALC_PARSER_H
#define DERIVATIVESCALC_PARSER_H
#include <iomanip>
#include <iostream>
#include <string>
#include "Polynomial.h"
#include "Term.h"

using namespace std;

 const string NUMBERS = "0123456789";
 const string CHARS = "abcdefghijklmnopqrstuwxyzABCDEFGHIJKLMNOPQRSTUWXYZ";
 const string OPERATORS = "+-*/^";


class Parser{
    string expression;
    list<Polynomial> polynomials;
    list<Term> terms;
public:
    Parser() = default;

    explicit Parser(string &expression, list<Polynomial> polynomials, list<Term> terms);

    Parser(const Parser& parser);

    ~Parser();

    const string &getExpression() const;

    void setExpression(string &expression);

    const list<Polynomial> &getPolynomials() const;

    void setPolynomials( list<Polynomial> &polynomials);

    const list<Term> &getTerms() const;

    void setTerms( list<Term> &terms);

    void takeExpression();

    bool prepareExpression();

    bool validateExpression(const string& expr);

    string &rpn(string& expression);

    int getPriority(const char sign) const;

    bool isRightAssociative(const char sign) const;

    void splitOnTermsAndPolynomials();

    bool isComplexExpression();

    bool isCoefficient(string &tmp, int j);

};



#endif //DERIVATIVESCALC_PARSER_H
