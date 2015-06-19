#pragma once
#include <string>

#define DEBUG_EVAL_
#ifdef DEBUG_EVAL
#include <iostream>
#define DEBUG_PRINT(s) std::cout << (s);
extern int _debugApps;
const  int _appIndent = 8;
#define DEBUG_PRINT_INDENT for(int i=0;i<_debugApps;++i){for(int j=0;j<_appIndent;++j)std::cout<<" ";std::cout<<"| ";}
#define DEBUG_TAB_INCREASE _debugApps++;
#define DEBUG_TAB_DECREASE _debugApps--;
#define DEBUG_PRINT_FUN(e) DEBUG_PRINT_INDENT                    DEBUG_PRINT("FUNCTION: "+e->toString()+'\n');DEBUG_TAB_INCREASE
#define DEBUG_PRINT_EVT(e) DEBUG_TAB_DECREASE DEBUG_PRINT_INDENT DEBUG_PRINT(" [eval] = "+e->toString()+'\n');
#define DEBUG_PRINT_ARG(e) DEBUG_PRINT_INDENT                    DEBUG_PRINT("ARGUMENT: "+e->toString()+'\n');DEBUG_TAB_INCREASE
#define DEBUG_PRINT_RES(e) DEBUG_TAB_DECREASE DEBUG_PRINT_INDENT DEBUG_PRINT("RETURNED: "+e->toString()+'\n');
#define DEBUG_LINEBREAK    std::cout << std::endl;
#endif
#ifndef DEBUG_EVAL
#define DEBUG_PRINT(s)     true;
#define DEBUG_PRINT_FUN(s) true;
#define DEBUG_PRINT_EVT(s) true;
#define DEBUG_PRINT_ARG(s) true;
#define DEBUG_PRINT_RES(s) true;
#define DEBUG_LINEBREAK    true;
#endif
