//
// Created by rpych on 30.08.18.
//

#include <algorithm>
#include "Polynomial.h"
#include "Parser.h"


Polynomial::Polynomial() {
expression = "";
derivative = "";
}

Polynomial::Polynomial(const string &expression, list<Monomial> monomials, string &derivative) : expression(
        expression), monomials(monomials), derivative(derivative) {}

Polynomial::Polynomial(const Polynomial& polynomial) {
    expression = polynomial.expression;
    monomials = polynomial.monomials;
    derivative = polynomial.derivative;
}


Polynomial::~Polynomial(){
    monomials.clear();
}


const string &Polynomial::getExpression() const {
    return expression;
}

void Polynomial::setExpression(const string &expression) {
    Polynomial::expression = expression;
}

 list<Monomial> &Polynomial::getMonomials(){
    return monomials;
}

void Polynomial::setMonomials(const list<Monomial> &monomials) {
    Polynomial::monomials = monomials;
}

const string &Polynomial::getDerivative() const {
    return derivative;
}

void Polynomial::setDerivative(const string derivative) {
    Polynomial::derivative = derivative;
}

bool Polynomial::operator < (const Polynomial &polynomial) const{
    return expression < polynomial.expression;
}

void Polynomial::calculateDerivative() {
    string tmp;
    for(auto &mono: monomials){
        if(mono.getExp() != 1)
            tmp += mono.getSign() + to_string(mono.getExp()*mono.getCoefficient()) + "*x^(" + to_string(mono.getExp()-1) + ")";
        else tmp += mono.getSign() + to_string(mono.getCoefficient());
    }
    reduceSigns(tmp);
    tmp = "(" + tmp + ")";
    Polynomial::setDerivative(tmp);
}


void Polynomial::makePolynomial(){
    bool variableExists = false;
    for(int i=0;i<expression.size();++i){
        if(CHARS.find(expression[i]) != string::npos) variableExists = true;
    }
    if(!variableExists){
        cout<<"Derivative = 0"<<endl;
        exit(0);
    }
    for(int i=0;i<expression.size();++i){
        Monomial *mono = new Monomial();
        if(i+1 < expression.size() && expression[i+1] == '*') {
            int j = i;
            string tmp;
            if(expression[j] == ')'){
                j--;
                while(j >= 0 && expression[j] != '('){
                    if(expression[j] != '-')
                        tmp += expression[j--];
                    else j--;
                }
            reverse(tmp.begin(), tmp.end());
            double ans = calculateFraction(tmp, "");
            mono->setCoefficient(ans);
            tmp = "";
            if(OPERATORS.find(expression[j-1]) != string::npos) tmp += expression[j-1];
            if(OPERATORS.find(expression[j+1]) != string::npos) {
                tmp += expression[j+1];
                if(tmp.compare("+-") == 0 || tmp.compare("-+") == 0) tmp = "-";
                else if(tmp.compare("--") == 0) tmp = "+";
                mono->setSign(tmp);
            }
            else
                mono->setSign(tmp);
            }else {
                while (j >= 0 && NUMBERS.find(expression[j]) != string::npos) {
                    tmp += expression[j--];
                    reverse(tmp.begin(), tmp.end());
                    mono->setCoefficient(stoi(tmp));
                }
                tmp = "";
                if(OPERATORS.find(expression[j]) != string::npos) {
                    tmp += expression[j];
                    mono->setSign(tmp);
                }
                else
                    mono->setSign("+");
            }
            while (expression[i] != '^') i++;
            tmp = "";
            string oper;
            if(i + 1 < expression.size() && expression[i+1] == '('){
                i++;
                while(i + 1 < expression.size() && expression[i+1] != ')') {
                    if(expression[i+1] == '/') oper += expression[i+1];
                    tmp += expression[i+1];
                    i++;
                }
            }
            while (i + 1 < expression.size() && NUMBERS.find(expression[i+1]) != string::npos) {
                tmp += expression[i+1];
                i++;
            }
            if(!oper.empty()){
                double ans = calculateFraction(tmp, oper);
                mono->setExp(ans);
            }
            else  mono->setExp(stoi(tmp));

            Polynomial::monomials.push_back(*mono);
            delete mono;
        }
    }
}



double Polynomial::calculateFraction(string &tmp, string oper){
    if(oper.empty()){
        for(int i=0;i<tmp.size();++i){
            if(tmp[i] == '/') oper += tmp[i];
        }
    }
    if(oper.empty()) return 1.0;
    int pos = (int) tmp.find(oper);
    string first = tmp.substr(0, pos);
    string sec = tmp.substr(pos+1);
    double ans = stod(first) / stod(sec);
    return ans;
}


void Polynomial::reduceSigns(string &s){
    string tmp;
    bool variableExists = false;
    for(int i=0;i<s.size();++i){
        if(NUMBERS.find(s[i]) != string::npos) variableExists = true;
        else if(s[i] == '+' && !variableExists) continue;
        tmp += s[i];
    }
    s =tmp;
}