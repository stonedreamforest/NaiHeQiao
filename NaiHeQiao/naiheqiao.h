#pragma once
#ifndef _NAIHEQIAO_H_
#define _NAIHEQIAO_H_

#ifdef _WIN64
typedef unsigned long long duint;
typedef signed long long dsint;
#else
typedef unsigned long duint;
typedef signed long dsint;
#endif //_WIN64


void nhq_recv_data(char *context);


void nhq_step_over(duint CurrentAdress);









bool nhq_inject_dll();



















#endif // !_NAIHEQIAO_H_
