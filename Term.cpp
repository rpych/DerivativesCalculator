//
// Created by rpych on 05.09.18.
//


//#include <list>
#include <algorithm>
#include "Term.h"
#include "Parser.h"

Term::Term(){
    term = "";
    derivative = "";
    opr = ' ';
    exp = 1.0;
}


Term::Term( string &term,  string &derivative,  char &opr, double exp) : term(term), derivative(derivative), opr(opr), exp(exp) {}


const string &Term::getTerm() const {
    return term;
}

void Term::setTerm(const std::basic_string<char> &term) {
    Term::term = term;
}

const string &Term::getDerivative() const {
    return derivative;
}

void Term::setDerivative(const std::basic_string<char> &derivative) {
    Term::derivative = derivative;
}

const char &Term::getOpr() const {
    return opr;
}

void Term::setOpr(const char &opr) {
    Term::opr = opr;
}

double Term::getExp() const {
    return exp;
}

void Term::setExp(double exp) {
    Term::exp = exp;
}


void Term::fetchOperator(){
    int brackets = 0;
    for(int i=0;i<term.size();++i){
        if(term[i] == '(') brackets++;
        else if(term[i] == ')') brackets--;
        if(i+1 < term.size() && OPERATORS.find(term[i+1]) != string::npos && brackets == 0) {
            opr = term[i+1];
        }
    }
}

void Term::calculateComplexDerivative(list<Polynomial> polynomials, list<Term> terms){

    switch(Term::opr){
        case '/':
            makeMultiplicationOrDivision(polynomials, terms);
            break;
        case '*':
            makeMultiplicationOrDivision(polynomials, terms);
            break;
        case '^':
            makeExponentialComplexDerivative(polynomials, terms);
            break;
        default:
            cout<<"Unrecognized operator"<<endl;
            break;
    }
}


void Term::makeMultiplicationOrDivision(list<Polynomial> polynomials, list<Term> terms){
    list<string> factors = extractFactors();
    string tmp, firstDer = "0", secDer = "0";
    double exponent = 1.0;
    bool isSecPoly = false;
    for (const Polynomial &poly: polynomials) {
        if (poly.getExpression().compare(factors.front()) == 0)
            firstDer = poly.getDerivative();
        else if(poly.getExpression().compare(factors.back()) == 0){
            isSecPoly = true;
            secDer = poly.getDerivative();
        }
    }
    for(const Term &t: terms){
        if (t.getTerm().compare(factors.front()) == 0)
            firstDer = t.getDerivative();
        else if(t.getTerm().compare(factors.back()) == 0){
            exponent = t.getExp();
            secDer = t.getDerivative();
        }

    }
    if(opr == '*')
        tmp += "(" + firstDer + "*(" + factors.back() + ')' + "+" + secDer +   "*(" + factors.front() + "))";
    else if(opr == '/'){
        if(isSecPoly) tmp += "((" + firstDer + "*(" + factors.back() + ")+" + secDer +   "*(" + factors.front() + "))"
                    + "/" + '(' + factors.back() + ")^2)";
        else if(exponent > 1.0) {
            int pos = (int) factors.back().find(")^");
            string secFactor = factors.back().substr(0, pos+2);
            tmp += "((" + firstDer + "*(" + factors.back() + ")+" + secDer + "*(" + factors.front() + "))"
                   + "/" + '(' + secFactor + to_string(exponent * 2) + ")";
        }
    }
    Term::derivative = tmp;
}


void Term::makeExponentialComplexDerivative(list<Polynomial> polynomials, list<Term> terms){
    list<string> factors = extractFactors();
    string der;
    string tmp;
    for(const Term& t: terms){
        if(t.getTerm().compare(factors.back()) == 0) der = t.getDerivative();
    }
    if(der.empty()){
        for(const Polynomial &p: polynomials){
            if(p.getExpression().compare(factors.back()) == 0) der = p.getDerivative();
        }
    }
    tmp += "(" + to_string(exp) + "*(" + factors.back() + ")^(" + to_string(exp-1) + "))*" + der;
    Term::derivative = tmp;
}



list<string> Term::extractFactors(){
    list<string> factors;
    int brackets = 0;
    string tmp;
    for(int i=0;i<term.size();++i){
        if(term[i] == '(') brackets++;
        else if(term[i] == ')') brackets--;
        if(term[i] == opr && brackets == 0){
            deleteOuterBrackets(tmp);
            factors.push_back(tmp);
            tmp = "";
            if(opr == '^'){
                tmp = setExponent(term, i);
                break;
            }
            continue;
        }
        tmp += term[i];
    }
    if(opr != '^'){
        deleteOuterBrackets(tmp);
        factors.push_back(tmp);
    }
    return factors;
}



void Term::deleteOuterBrackets(string &t){
    string tmp;
    for(int i=0;i<t.size();++i){
        if((i==0 && t[0] == '(') || (i == t.size()-1 && t[t.size()-1] == ')')) continue;
        else tmp += t[i];
    }
     t = tmp;
}


string Term::setExponent(string& term, int i){
    string tmp;
    string oper;
    if(i + 1 < term.size() && term[i+1] == '('){
        i++;
        while(i + 1 < term.size() && term[i+1] != ')') {
            if(term[i+1] == '/') oper += term[i+1];
            tmp += term[i+1];
            i++;
        }
    }
    while (i + 1 < term.size() && NUMBERS.find(term[i+1]) != string::npos) {
        tmp += term[i+1];
        i++;
    }
    if(!oper.empty()){
        int pos = (int) tmp.find(oper);
        string first = tmp.substr(0, pos);
        string sec = tmp.substr(pos+1);
        double ans = stod(first) / stod(sec);
        exp = ans;
    }
    else  exp = stod(tmp);

    return tmp;
}