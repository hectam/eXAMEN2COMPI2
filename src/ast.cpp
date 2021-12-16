#include "ast.h"
#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include "asm.h"

const char * floatTemps[] = {"$f0",
                            "$f1",
                            "$f2",
                            "$f3",
                            "$f4",
                            "$f5",
                            "$f6",
                            "$f7",
                            "$f8",
                            "$f9",
                            "$f10",
                            "$f11",
                            "$f12",
                            "$f13",
                            "$f14",
                            "$f15",
                            "$f16",
                            "$f17",
                            "$f18",
                            "$f19",
                            "$f20",
                            "$f21",
                            "$f22",
                            "$f23",
                            "$f24",
                            "$f25",
                            "$f26",
                            "$f27",
                            "$f28",
                            "$f29",
                            "$f30",
                            "$f31"
                        };

#define FLOAT_TEMP_COUNT 32
set<string> intTempMap;
set<string> floatTempMap;

extern Asm assemblyFile;

int globalStackPointer = 0;

map<string, int> codeGenerationVars;

string getFloatTemp(){
    for (int i = 0; i < FLOAT_TEMP_COUNT; i++)
    {
        if(floatTempMap.find(floatTemps[i]) == floatTempMap.end()){
            floatTempMap.insert(floatTemps[i]);
            return string(floatTemps[i]);
        }
    }
    cout<<"No more float registers!"<<endl;
    return "";
}

void releaseFloatTemp(string temp){
    floatTempMap.erase(temp);
}

void FloatExpr::genCode(Code &code){
    string floatTemp = getFloatTemp();
    code.place = floatTemp;
    stringstream ss;
    ss << "li.s " << floatTemp <<", "<< this->number <<endl;
    code.code = ss.str();
    
}

void SubExpr::genCode(Code &code){
Code leftCode;
Code rightCode;
this->expr1->genCode(leftCode);
this->expr2->genCode(rightCode);
code.code = leftCode.code + "\n" + rightCode.code + "\n";
string newTemp = getFloatTemp();
code.code = "sub.s " + newTemp + ", " + leftCode.place  +", "+ rightCode.place +"\n";
releaseFloatTemp(leftCode.place);
releaseFloatTemp(rightCode.place);
code.place = newTemp;

}

void DivExpr::genCode(Code &code){
Code leftCode;
Code rightCode;
this->expr1->genCode(leftCode);
this->expr2->genCode(rightCode);
code.code = leftCode.code + "\n" + rightCode.code + "\n";
string newTemp = getFloatTemp();
code.code = "div.s " + newTemp + ", " + leftCode.place  +", "+ rightCode.place +"\n";
releaseFloatTemp(leftCode.place);
releaseFloatTemp(rightCode.place);
code.place = newTemp;
}

void IdExpr::genCode(Code &code){

}

string ExprStatement::genCode(){
    Code code;
    this->expr->genCode(code);
    return code.place;
}

string IfStatement::genCode(){
    return "If statement code generation\n";
}

void MethodInvocationExpr::genCode(Code &code){
    
}

string AssignationStatement::genCode(){
    gl
    string var =getFloatTemp();
    Code rightCode;
    this->value->genCode(rightCode);
    rightCode.code;
    string code = "l.s " + var + ", " + rightCode.place+"\n";

    return code;
}

void GteExpr::genCode(Code &code){
}

void LteExpr::genCode(Code &code){
}

void EqExpr::genCode(Code &code){
}

void ReadFloatExpr::genCode(Code &code){
    
}

string PrintStatement::genCode(){
    return "Print statement code generation\n";
}

string ReturnStatement::genCode(){
    return "Return statement code generation\n";
}

string MethodDefinitionStatement::genCode(){
    return "Method definition code generation\n";
}