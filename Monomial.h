//
// Created by rpych on 30.08.18.
//

#ifndef DERIVATIVESCALC_MONOMIAL_H
#define DERIVATIVESCALC_MONOMIAL_H
#include <iomanip>
#include <iostream>
using namespace std;


class Monomial{

public:
    Monomial() = default;

    Monomial(string& _sign, double _exp, double _coefficient): sign(_sign), exp(_exp),
                                                                     coefficient(_coefficient){}
    Monomial(const Monomial& monomial);

    const string &getSign() const;

    void setSign( string sign);

    double getExp() const;

    void setExp(double exp);

    double getCoefficient() const;

    void setCoefficient(double coefficient);

    bool operator < (const Monomial& monomial) const;

private:
    string sign;
    double exp;
    double coefficient;
};


#endif //DERIVATIVESCALC_MONOMIAL_H
