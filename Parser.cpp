//
// Created by rpych on 30.08.18.
//
#include <stack>
#include <string>
#include <algorithm>
#include "Parser.h"




Parser::Parser(string &expression, list<Polynomial> polynomials, list<Term> terms) : expression(expression),
                                                                                     polynomials(polynomials), terms(terms) {}

Parser::Parser(const Parser &parser) {
    expression = parser.expression;
    polynomials = parser.polynomials;
    terms = parser.terms;
}

Parser::~Parser(){
    polynomials.clear();
    terms.clear();
}

const string &Parser::getExpression() const {
    return expression;
}

void Parser::setExpression(string &expression) {
    Parser::expression = expression;
}

const std::list <Polynomial> &Parser::getPolynomials() const {
    return polynomials;
}

void Parser::setPolynomials( std::list <Polynomial> &polynomials) {
    Parser::polynomials = polynomials;
}

const std::list <Term> &Parser::getTerms() const {
    return terms;
}

void Parser::setTerms( std::list <Term> &terms) {
    Parser::terms = terms;
}

void Parser::takeExpression() {

    string expression;
    cout<<"Remember to put expressions in brackets to distinguish operation's factors! \nEnter your algebraic expression!:"<<endl;
    getline(cin,expression);
    Parser::expression = expression;
    if(!prepareExpression()) cout<<"Wrong number of brackets or wrong variable letters!"<<endl;

}

bool Parser::prepareExpression() {
   if(!validateExpression(expression)) return false;
   else{

       string tmp = "";
       for(int i=0;i < expression.size();++i){
           if(expression[i] == ' ') continue;
           else tmp += expression[i];
       }
       expression = tmp;
       tmp = "";
       for(int i=0;i < expression.size();++i){
           if(i==0 && CHARS.find(expression[0]) != string::npos) tmp += "1*";
           tmp += expression[i];

           if(NUMBERS.find(expression[i]) == string::npos && CHARS.find(expression[i]) == string::npos && i+1 < expression.size()
            && CHARS.find(expression[i+1]) != string::npos) {
               if(i-1 >= 0 && NUMBERS.find(expression[i-1]) != string::npos && expression[i] == ')' ) tmp += "*"; //=='*'
               else tmp += "1*"; //when monomial with x^n exists
           }
           else if(expression[i] == ')' && i+1 < expression.size() && expression[i+1] == '(') tmp += "*";
           else if(i+1 < expression.size() && CHARS.find(expression[i+1]) != string::npos){
               tmp += '*';
           }
               // when monomial a*x exists
           else if(CHARS.find(expression[i]) != string::npos && i+1 < expression.size() && expression[i+1] != '^' ||
                   (CHARS.find(expression[i]) != string::npos && i+1 == expression.size())) tmp += "^1";
       }

       expression = tmp;
       cout<<"Expression before calculations: "<<expression<<endl;
       if(!isComplexExpression()){
           cout<<"Only polynomial:"<<endl;
           splitOnTermsAndPolynomials();
           Polynomial *poly = new Polynomial();
           poly->setExpression(expression);
           poly->makePolynomial();
           poly->calculateDerivative();
           cout<<poly->getExpression()<<endl;
           cout<<"Polynomial Derivative = "<<poly->getDerivative()<<endl<<endl;
       }
       else {
           splitOnTermsAndPolynomials();
           cout << "Polynomials:" << endl;
           for (Polynomial &poly: polynomials) {
               poly.makePolynomial();
               poly.calculateDerivative();
               cout << poly.getExpression() << endl;
           }
           cout << "Terms:" << endl;
           for (Term &t: terms) {
               t.fetchOperator();
               list<string> f = t.extractFactors();
               t.calculateComplexDerivative(polynomials, terms);
               cout << t.getTerm() <<endl;
           }
           string s = terms.back().getDerivative();
           cout <<"Complex Derivative = "<< s << endl<<endl;
       }

       return true;
   }
}


bool Parser::validateExpression(const string& expr){
    int brackets = 0;
    for(int i=0;i<expr.length();++i){
        if(expr[i] == '(') brackets++;
        else if(expr[i] == ')') brackets--;
        if(brackets < 0) return false;
    }
    if(brackets == 0){
        int pos = -1;
        for(int i=0;i<expr.size();++i){
            if(CHARS.find(expr[i]) != string::npos){
                if(pos ==-1) pos = (int) CHARS.find(expr[i]);
                else if (pos != (int) CHARS.find(expr[i])) return false;
            }
        }
        return true;
    }
}



string &Parser::rpn(string& expression){
    stack<char> stack;
    string tmp = "";
    for(int i=0;i<expression.size();++i){

        if(NUMBERS.find(expression[i]) != string::npos){
            while(NUMBERS.find(expression[i]) != string::npos) tmp += expression[i++];
            tmp += ","; //to distinguish multidigital numbers during calculation
        }
        if(CHARS.find(expression[i]) != string::npos ) tmp += expression[i];
        else if(expression[i] == '(' || (OPERATORS.find(expression[i]) != string::npos && stack.empty())) stack.push(expression[i]);
        else if(expression[i] == ')'){
            while(stack.top() != '('){
                tmp += stack.top();
                stack.pop();
            }
            stack.pop();
        }
        else if(OPERATORS.find(expression[i]) != string::npos) {
            while( !stack.empty() && (getPriority(expression[i]) <= getPriority(stack.top()) ||
                    (getPriority(expression[i]) < getPriority(stack.top()) && isRightAssociative(expression[i])))) {
                tmp += stack.top();
                stack.pop();
            }
            stack.push(expression[i]);
        }
    }
    while(!stack.empty()){
        tmp += stack.top();
        stack.pop();
    }
    expression = tmp;
    return expression;
}



bool Parser::isRightAssociative(const char sign) const{
    return sign == '^';
}



int Parser::getPriority(const char sign) const{
    if(sign == '^') return 3;
    else if(sign == '*' || sign == '/') return 2;
    else if(sign == '+' || sign == '-') return 1;
    else return 0;
}




void Parser::splitOnTermsAndPolynomials() {


    string tmp;
    for(int i=0;i<expression.size();++i){
        bool isTerm = false, variableExists =false;
        int brackets = 0;
        tmp = "";
        if(expression[i] == ')'){
            int j = i - 1;
            brackets++;
            while(j >= 0 && (expression[j] != '(' || (brackets - 1) != 0)){
                if(expression[j] == '(') brackets--;
                else if(expression[j] == ')') brackets++;
                else if(CHARS.find(expression[j]) != string::npos) variableExists = true;
                if(brackets > 1 && !isCoefficient(expression, j)) isTerm = true; //then there is Term or simple Term-Polynomial
                tmp += expression[j--];
            }
            if(!variableExists) continue;   //protect from creating non-variable Polynomial or Term
            reverse(tmp.begin(), tmp.end());
            if(isTerm){
                Term *t = new Term();
                t->setTerm(tmp);
                Parser::terms.push_back(*t);
                delete t;
            }
            else{
                Polynomial *p = new Polynomial();
                p->setExpression(tmp);
                Parser::polynomials.push_back(*p);
                delete p;
            }

        }

    }
    Term *t = new Term();
    t->setTerm(expression);
    Parser::terms.push_back(*t);
    delete t;
}



bool Parser::isComplexExpression(){
     int brackets = 0, vars = 0; //vars - amount of different factors in expression
     bool varExists = false;
    for(int i=0;i<expression.size();++i){
        if(expression[i] == '(') {
            while (i < expression.size() && (expression[i] != ')' || brackets-1 != 0)){
                if(expression[i] == '(')  brackets++;
                else if(expression[i] == ')') brackets--;
                i++;
            }
            if(i+1 < expression.size() && expression[i+1] == '^') return true;
            vars++;
        }
    }

    if(vars > 1) return true;
    brackets = 0, vars = 0;
    for(int i=0;i<expression.size();++i){
        if(expression[i] == '(') {
            while(i+1 < expression.size() && expression[i+1] == '(') i++;
            while (i < expression.size() && (expression[i] != ')' || brackets-1 != 0)){
                if(expression[i] == '(')  brackets++;
                else if(expression[i] == ')') brackets--;
                else if(CHARS.find(expression[i]) != string::npos) varExists = true;
                i++;
            }
            if(varExists)
                vars++;
        }
    }
    return vars > 1;
}


bool Parser::isCoefficient(string &tmp, int j){
    int brackets = 0;
    bool isVariable = true;
    while(tmp[j] != '(' || brackets != 0){
        if(tmp[j] == ')') brackets++;
        else if(tmp[j] == '(') brackets--;
        else if(CHARS.find(tmp[j]) != string::npos) {
            isVariable = false;
            return isVariable;
        }
        j--;
    }
    return isVariable;
}