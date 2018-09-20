//
// Created by rpych on 30.08.18.
//
#include "Monomial.h"

Monomial::Monomial(const Monomial &monomial) {
    coefficient = monomial.coefficient;
    exp = monomial.exp;
    sign = monomial.sign;
}


const string &Monomial::getSign() const {
    return sign;
}

void Monomial::setSign( string sign) {
    Monomial::sign = sign;
}

double Monomial::getExp() const {
    return exp;
}

void Monomial::setExp(double exp) {
    Monomial::exp = exp;
}

double Monomial::getCoefficient() const {
    return coefficient;
}

void Monomial::setCoefficient(double coefficient) {
    Monomial::coefficient = coefficient;
}

bool Monomial::operator < (const Monomial& monomial) const{
    return coefficient < monomial.coefficient && exp < monomial.exp;
}