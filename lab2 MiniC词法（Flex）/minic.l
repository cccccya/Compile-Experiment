%{
#include "string.h"
#include "def.h"

extern int yycolumn;

#define YYSTYPE yylval
extern "C"
{	
	int yywrap(void);
	int yylex(void);
}

void flexerr(int column,std::string info,std::string value){
  std::cout<<"Error type A at Line "<<column<<": "<<info<<" '"<<value<<"'"<<std::endl;
}

void flexout(std::string name,std::string value){
  std::cout<<name<<"\t:"<<value<<std::endl;
}
//flex为bison提供行号信息
%}
%option yylineno 
INTIGER ((\-)?[1-9][0-9]*)|0
ID [a-zA-Z]+[a-zA-Z0-9]*
%%

"int"        {flexout("TYPE","int");}
"float"      {flexout("TYPE","float");}
"char"      {flexout("TYPE","char");}
";"         {flexout("SEMI",";");}
","         {flexout("COMMA",",");}
"=="        {flexout("RELOP","==");}
"="         {flexout("ASSIGNOP","=");}
"++"        {flexout("PLUSPLUS","++");}
"+="        {flexout("PLUSASS","+=");}
"+"         {flexout("PLUS","+");}
"--"        {flexout("MINUSMINUS","--");}
"-="        {flexout("MINUSASS","-=");}
"-"         {flexout("MINUS","-");}
"*"         {flexout("STAR","*");}
"/"         {flexout("DIV","/");}
"&&"        {flexout("AND","&&");}
"||"        {flexout("OR","||");}
"."         {flexout("DOT",".");}
"!"         {flexout("NOT","!");}
"("         {flexout("LP","(");}
")"         {flexout("RP",")");}
"["         {flexout("LB","[");}
"]"         {flexout("RB","]");}
"{"         {flexout("LC","{");}
"}"         {flexout("RC","}");}
"struct"    {flexout("STRUCT","struct");}
"return"    {flexout("RETURN","return");}
"if"        {flexout("IF","if");}
"else"      {flexout("ELSE","else");}
"while"     {flexout("WHILE","while");}
{ID}          {flexout("ID",yytext);}
[0][x][1-9a-fA-F][0-9a-fA-F]* {flexout("INT",yytext);}
[0][x][1-9a-fA-F]*[g-zG-Z]+ {flexerr(yycolumn,"Illegal hexadecimal number",yytext);}
[0][1-7][0-7]*  {flexout("INT",yytext);}
[0][0-7]*[9A-Za-z]+    {flexerr(yycolumn,"Illegal octal number",yytext);}
[0]         {flexout("INT",yytext);}
{INTIGER}+"."{INTIGER}*[e]?{INTIGER}+     {flexout("FLOAT",yytext);}
{INTIGER}     {flexout("INT",yytext);}


[\n]		{yycolumn++;}
[ \r\t]		{/*printf("过滤空格等字符\n");*/}
.       {flexerr(yycolumn,"Mysterious characters",yytext);}
%%

int yywrap()
{
    return 1;
}