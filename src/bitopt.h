#ifndef _BITOPT_H
#define _BITOPT_H

#define _ONE ((unsigned long long) 1)

#define setbit(n, i) ((n) |= (_ONE << (i)))
#define unsetbit(n, i) ((n) &= ~(_ONE << (i)))
#define isbitset(n, i) (((n) & (_ONE << (i))) != 0)
#define getbit(n, i) ((n) & (_ONE << (i)))

#endif // _BITOPT_H
