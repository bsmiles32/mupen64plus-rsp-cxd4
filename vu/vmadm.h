/******************************************************************************\
* Authors:  Iconoclast                                                         *
* Release:  2013.11.26                                                         *
* License:  CC0 Public Domain Dedication                                       *
*                                                                              *
* To the extent possible under law, the author(s) have dedicated all copyright *
* and related and neighboring rights to this software to the public domain     *
* worldwide. This software is distributed without any warranty.                *
*                                                                              *
* You should have received a copy of the CC0 Public Domain Dedication along    *
* with this software.                                                          *
* If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.             *
\******************************************************************************/
#include "vu.h"

INLINE static void do_madm(short* VD, short* VS, short* VT)
{
    uint32_t addend[N];
    register int i;

    for (i = 0; i < N; i++)
        addend[i] = (unsigned short)(VACC_L[i]) + (unsigned short)(VS[i]*VT[i]);
    for (i = 0; i < N; i++)
        VACC_L[i] += (short)(VS[i] * VT[i]);
    for (i = 0; i < N; i++)
        addend[i] = (addend[i] >> 16) + (VS[i]*(unsigned short)(VT[i]) >> 16);
    for (i = 0; i < N; i++)
        addend[i] = (unsigned short)(VACC_M[i]) + addend[i];
    for (i = 0; i < N; i++)
        VACC_M[i] = (short)addend[i];
    for (i = 0; i < N; i++)
        VACC_H[i] += addend[i] >> 16;
    SIGNED_CLAMP_AM(VD);
    return;
}

VECTOR_OPERATION VMADM(v16 vd, v16 vs, v16 vt)
{
#ifdef ARCH_MIN_SSE2
    ALIGNED short VD[N], VS[N], VT[N];

    *(__m128i *)VD = vd;
    *(__m128i *)VS = vs;
    *(__m128i *)VT = vt;
#else
    v16 VD, VS, VT;

    VD = vd;
    VS = vs;
    VT = vt;
#endif
    do_madm(VD, VS, VT);
#ifdef ARCH_MIN_SSE2
    vd = *(__m128i *)VD;
#endif
    return (vd);
}
