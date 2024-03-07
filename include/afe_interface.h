#ifndef __AFE_INTERFACE__H__
#define __AFE_INTERFACE__H__

#define Interface(rttype, funcname, argtype...) rttype (*funcname)(argtype)

// int reset(char a, unsigned int c);
typedef struct _Ipll {

    Interface(int, reset, char);

} I_pll;

extern I_pll pllsa;
#endif //!__AFE_INTERFACE__H__