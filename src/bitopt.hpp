#ifndef _BITOPT_H
#define _BITOPT_H

#define _ONE (1ull)

#define setbit(n, i) ((n) |= (_ONE << (i)))
#define unsetbit(n, i) ((n) &= ~(_ONE << (i)))
#define isbitset(n, i) (((n) & (_ONE << (i))) != 0)
#define getbit(n, i) ((n) & (_ONE << (i)))

#endif // _BITOPT_H
