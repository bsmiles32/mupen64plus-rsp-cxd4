/*
 * mupen64plus-rsp-cxd4 - RSP Interpreter
 * Copyright (C) 2012-2013  RJ 'Iconoclast' Swedlow
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "vu.h"

#ifndef SEMIFRAC
/*
 * acc = VS * VT;
 * acc = acc + 0x8000; // rounding value
 * acc = acc << 1; // partial value shifting
 *
 * Wrong:  ACC(HI) = -((INT32)(acc) < 0)
 * Right:  ACC(HI) = -(SEMIFRAC < 0)
 */
#define SEMIFRAC    (VS[i]*VT[i]*2/2 + 0x8000/2)
#endif

INLINE static void do_mulu(short* VD, short* VS, short* VT)
{
    register int i;

    for (i = 0; i < N; i++)
        VACC_L[i] = (SEMIFRAC << 1) >>  0;
    for (i = 0; i < N; i++)
        VACC_M[i] = (SEMIFRAC << 1) >> 16;
    for (i = 0; i < N; i++)
        VACC_H[i] = -((VACC_M[i] < 0) & (VS[i] != VT[i])); /* -32768 * -32768 */
#ifndef ARCH_MIN_SSE2
    vector_copy(VD, VACC_M);
    for (i = 0; i < N; i++)
        VD[i] |=  (VACC_M[i] >> 15); /* VD |= -(result == 0x000080008000) */
    for (i = 0; i < N; i++)
        VD[i] &= ~(VACC_H[i] ^ 0); /* VD &= -(result >= 0x000000000000) */
#else
    UNSIGNED_CLAMP(VD);
#endif
    return;
}

static void VMULU(void)
{
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;

    do_mulu(VR[vd], VR[vs], ST);
    return;
}

static void VMULU_v(void)
{
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    do_mulu(VR[vd], VR[vs], VR[vt]);
    return;
}
static void VMULU0q(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x2);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU1q(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x3);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU0h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x4);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU1h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x5);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU2h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x6);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU3h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x7);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU0w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x8);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU1w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x9);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU2w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xA);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU3w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xB);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU4w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xC);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU5w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xD);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU6w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xE);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
static void VMULU7w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xF);
    do_mulu(VR[vd], VR[vs], SV);
    return;
}
