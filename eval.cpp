//eval.cpp
#include <iostream>
#include "eval.h"
#include <cmath>
#include <cstdio>
#include <map>

using namespace std;

float eval(string s, map<string, float> val);
map<string, float(*)(float)> functie; 

float cube(float x) {
    return cbrt(x);
}

float ctg(float x) {
    return cos(x) / sin(x);
}

float arcctg(float x) {
    return PI / 2 -  atan(x);
}

float lb2(float x) {
    return log2(x);
}


void init() {
    //Functii trigonometrice
    functie["sin"] = sin;
    functie["cos"] = cos;
    functie["tg"] = tan;
    functie["tan"] = tan;
    functie["ctg"] = ctg;
    functie["ctan"] = ctg;
    //Functii trigonometrice inverse
    functie["arcsin"] = asin;
    functie["arccos"] = acos;
    functie["arctg"] = atan;
    functie["arctan"] = atan;
    functie["arcctg"] = arcctg;
    functie["arccot"] = arcctg;
    //Alte functii
    functie["sqrt"] = sqrt;
    functie["cbrt"] = cube;
    functie["abs"] = abs;
    functie["ln"] = log;
    functie["lg"] = log10;
    functie["log"] = lb2;
}

int power(char x) {
    if (x == '-' || x == '+')
        return 3;
    if (x == '*' || x == '/')
        return 2;
    if (x == '^');
        return 1;
}

string remove_spaces_paranthesis(string s) {
    string ciu;
    for(int i = 0; i < s.size(); i++) {
        if(s[i] != ' ')
            ciu += s[i];
    }
    if(ciu[0] != '(' || ciu[ciu.size() - 1] != ')')
        return ciu;
    int count = 0;
    for(int i = 0; i < ciu.size() - 1; i++) {
        if(ciu[i] == '(') count++;
        if(ciu[i] == ')') count--; 
    }
    if(count == 0)
        return ciu;
    else
        return ciu.substr(1, ciu.size() - 2);
}

float eval_simple_expr(string s, map<string, float> val) {
    float value = 0;
    bool cifre = true;
    int poz = s.find('(');
    if(poz == -1) { 
        for(int i = 0; i < s.size(); i++) {
            if(s[i] < '0' || s[i] > '9' ) {
                cifre = false;
                break;
            }             
        }
        if(cifre)
            sscanf(s.c_str(), "%f", &value);
        else
            value = val[s];
        
    } else {
        value = functie[s.substr(0, poz)](eval(s.substr(poz, s.size() - poz), val));
    }
    return value;
}

float eval(string s, map<string, float> val) {
    // TODO(robert): doesn't fix double paranthesis ex: ((4+3)) 
    s = remove_spaces_paranthesis(s); //SWAG
    int poz = -1;
    int count = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(' ) count++;
        if (s[i] == ')' ) count--;
        if (count) continue;
        if (strchr("+-*/^", s[i])) {
            if(poz == -1 || power(s[poz]) <= power(s[i]))
                poz = i;    
        }
    }
    if (poz == -1) 
        return eval_simple_expr(s, val);
    float left = eval(s.substr(0, poz), val);
    float right = eval(s.substr(poz + 1, s.size() - poz - 1), val);
    if (s[poz] == '+') 
        return left + right;
    if (s[poz] == '-')
        return left - right;
    if (s[poz] == '*')
        return left * right;
    if (s[poz] == '/')
        return left / right;
    if (s[poz] == '^')
        return pow(left,right);           
}
