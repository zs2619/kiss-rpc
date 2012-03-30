identifier    ([a-zA-Z_][a-zA-Z_0-9]*)
%{
#include <string>
#include "parser/Global.h"
#include "parser/DefType.h"
#include "rpc.tab.hpp"

void reserved_keyword(char* keyword) {
  yyerror("Cannot use reserved language keyword: \"%s\"\n", keyword);
}
%}
%option noyywrap
%option lex-compat

whitespace    ([ \t\r\n]*)
sillycomm     ("/*""*"*"*/")
multicomm     ("/*"[^*]"/"*([^*/]|[^*]"/"|"*"[^/])*"*"*"*/")
comment       ("//"[^\n]*)
symbol        ([:;\,\{\}\(\)\=<>\[\]])
%%

"bool"              { return tok_bool;                 }
"uint8"             { return tok_uint8;               }
"int8"				{ return tok_int8;                 }
"uint16"            { return tok_uint16;               }
"int16"				{ return tok_int16;                 }
"uint32"            { return tok_uint32;               }
"int32"				{ return tok_int32;                 }
"int64"				{ return tok_int64;                 }
"float"			    { return tok_float;               }
"string"            { return tok_string;               }

"include"           { return tok_include;              }

"enum"			    {return tok_enum;               }
"struct"			{ return tok_struct;			}
"array"			    { return tok_array;             }
"map"               { return tok_map;              }

"service"           { return tok_service;          }

"dynamic"            { reserved_keyword(yytext); }
"as"				 { reserved_keyword(yytext); }
"int"				 { reserved_keyword(yytext); }
"uint"				 { reserved_keyword(yytext); }
"char"				 { reserved_keyword(yytext); }
"uchar"				 { reserved_keyword(yytext); }
"short"				 { reserved_keyword(yytext); }
"ushort"			 { reserved_keyword(yytext); }


{whitespace}         { /* do nothing */                 }
{sillycomm}          { /* do nothing */                 }
{multicomm}          { /* do nothing */                 }
{comment}            { /* do nothing */                 }
{symbol}             { return yytext[0];                }

{identifier}		 { yylval.str_ = new std::string(yytext, yyleng); return tok_identifier; } 

%%