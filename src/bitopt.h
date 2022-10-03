#ifndef _BITOPT_H
#define _BITOPT_H

#define setbit(n, i) ((n) |= (1ul << (i)))
#define unsetbit(n, i) ((n) &= ~(1ul << (i)))
#define isbitset(n, i) (((n) & (1ul << (i))) != 0)
#define getbit(n, i) ((n) & (1ul << (i)))

#endif // _BITOPT_H
