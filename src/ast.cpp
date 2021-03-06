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

int labelCounter = 0;
extern Asm assemblyFile;
string getNewLabel(string prefix){
    stringstream ss;
    ss<<prefix << labelCounter;
    labelCounter++;
    return ss.str();
}
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
code.code += "sub.s " + newTemp + ", " + leftCode.place  +", "+ rightCode.place +"\n";
code.code += "mov.s " + leftCode.place  + ", " + newTemp +"\n";
releaseFloatTemp(leftCode.place);
releaseFloatTemp(rightCode.place);
cout<< code.code.c_str();
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
code.code += "mov.s " + leftCode.place  + ", " + newTemp +"\n";
releaseFloatTemp(leftCode.place);
releaseFloatTemp(rightCode.place);
cout<< code.code.c_str();
code.place = newTemp;
}

void IdExpr::genCode(Code &code){
    if(floatTempMap.find(this->id) == floatTempMap.end()){
        string temp = getFloatTemp();
        code.place = temp;
        code.code = "l.s "+ temp + ", "+ this->id + "\n";
        
    }
}

string ExprStatement::genCode(){
    Code code;
    this->expr->genCode(code);
    releaseFloatTemp(code.place);
    return code.code;
}

string IfStatement::genCode(){
    string endIfLabel = getNewLabel("endif");
    Code exprCode;
    this->conditionalExpr->genCode(exprCode);
    stringstream code;
    code << exprCode.code << endl;
    
    code << "bc1f "<< endIfLabel <<endl;

    list<Statement*>::iterator expr = this->trueStatement.begin();
    while (expr != trueStatement.end())
    {
       code<< (*expr)->genCode()<<endl;
        expr++;
    }

    code<<   endl
    << endIfLabel <<" :"<< endl;

    releaseFloatTemp(exprCode.place);
    
    return code.str();
}

void MethodInvocationExpr::genCode(Code &code){
    
}

string AssignationStatement::genCode(){
    
    Code Code;
      
    this->value->genCode(Code);

    IdExpr* fullExpre = (IdExpr *)this->value;
    string id = fullExpre->id;
    if(floatTempMap.find(id) == floatTempMap.end())
    releaseFloatTemp(Code.place);
    
    return Code.code;
}

void GteExpr::genCode(Code &code){
    Code leftCode;
    Code rightCode;
    this->expr1->genCode(leftCode);
    this->expr2->genCode(rightCode);
    code.code = leftCode.code + "\n" + rightCode.code + "\n";
  
    code.code = "c.le.s " + rightCode.place  +", "+ leftCode.place+"\n";
    releaseFloatTemp(leftCode.place);
    releaseFloatTemp(rightCode.place);
    cout<< code.code.c_str();
    code.place = leftCode.place;
    
}

void LteExpr::genCode(Code &code){
    Code leftCode;
    Code rightCode;
    this->expr1->genCode(leftCode);
    this->expr2->genCode(rightCode);
    code.code = leftCode.code + "\n" + rightCode.code + "\n";
   
    code.code = "c.lt.s " + leftCode.place  +", "+ rightCode.place +"\n";
    releaseFloatTemp(leftCode.place);
    releaseFloatTemp(rightCode.place);
    cout<< code.code.c_str();
    code.place = leftCode.place;
   
}

void EqExpr::genCode(Code &code){
    Code leftCode;
    Code rightCode;
    this->expr1->genCode(leftCode);
    this->expr2->genCode(rightCode);
    code.code = leftCode.code + "\n" + rightCode.code + "\n";
    
    code.code = "c.eq.s "  + leftCode.place  +", "+ rightCode.place +"\n";
    releaseFloatTemp(leftCode.place);
    releaseFloatTemp(rightCode.place);
    cout<< code.code.c_str();
    code.place = leftCode.place;
}

void ReadFloatExpr::genCode(Code &code){
    
}

string PrintStatement::genCode(){
   Code exprCode;
    list<Expr *>::iterator printExpr = this->expressions.begin();
    while (printExpr != expressions.end())
    {
        (*printExpr)->genCode(exprCode);
        printExpr++;
    }
    stringstream code;
    code<< exprCode.code<<endl;
    code << "mov.s $f12, "<< exprCode.place<<endl
    << "li $v0, 2"<<endl
    << "syscall"<<endl;
    return code.str();
}

string ReturnStatement::genCode(){
    Code exprCode;
    this->expr->genCode(exprCode);
    
        releaseFloatTemp(exprCode.place);
    

    stringstream ss;
    ss << exprCode.code << endl
    << "mov.s $v0, "<< exprCode.place <<endl;
    return ss.str();
}

string MethodDefinitionStatement::genCode(){
    return "Method definition code generation\n";
}