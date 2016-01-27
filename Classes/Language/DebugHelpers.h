#pragma once
#include <string>

#define DEBUG_EVAL
#ifdef DEBUG_EVAL
#include <iostream>
#define DEBUG_PRINT(s) std::cout << (s);
extern int _debugApps;
const  int _appIndent = 3;
#define DEBUG_INDENTATION for(int i=0;i<_debugApps;++i){for(int j=0;j<_appIndent;++j)std::cout<<" ";std::cout<<"| ";}
#define DEBUG_INDENT_INCR _debugApps++;
#define DEBUG_INDENT_DECR _debugApps--;
#define DEBUG_PRINT_OP(e) DEBUG_INDENTATION DEBUG_PRINT("OPERATION: " + e->show() + '\n');
#define DEBUG_PRINT_RS(e) DEBUG_INDENT_DECR DEBUG_INDENTATION DEBUG_PRINT("RES: "+e->show()+'\n');
#define DEBUG_PRINT_LT(e) DEBUG_INDENTATION                   DEBUG_PRINT("FST: "+e->show()+'\n');DEBUG_INDENT_INCR
#define DEBUG_PRINT_EV(e) DEBUG_INDENT_DECR DEBUG_INDENTATION DEBUG_PRINT("EVL: "+e->show()+'\n');DEBUG_INDENT_INCR
#define DEBUG_PRINT_RT(e) DEBUG_INDENT_DECR DEBUG_INDENTATION DEBUG_PRINT("SND: "+e->show()+'\n');DEBUG_INDENT_INCR
#define DEBUG_LINEBREAK   std::cout << std::endl;
#endif
#ifndef DEBUG_EVAL
#define DEBUG_PRINT(s)    true;
#define DEBUG_PRINT_RT(s) true;
#define DEBUG_PRINT_LT(s) true;
#define DEBUG_PRINT_RS(s) true;
#define DEBUG_PRINT_OP(s) true;
#define DEBUG_PRINT_EV(s) true;
#define DEBUG_LINEBREAK   true;
#endif
