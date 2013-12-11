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

INLINE void UNSIGNED_CLAMP(short* VD)
{
    short hi[N], lo[N];
    register int i;

    for (i = 0; i < N; i++)
        VD[i] = ACC_M(i);
    for (i = 0; i < N; i++)
        lo[i] = -(result[i] < 0);
    for (i = 0; i < N; i++)
        hi[i] = (+0x7FFF - result[i]) >> 31;
    for (i = 0; i < N; i++)
        VD[i] = VD[i] & ~lo[i];
    for (i = 0; i < N; i++)
        VD[i] = VD[i] | hi[i];
    return;
}

INLINE void do_macu(short* VD, short* VS, short* VT)
{
    long product[N];
    unsigned long addend[N];
    register int i;

    for (i = 0; i < N; i++)
        product[i] = VS[i] * VT[i];
    for (i = 0; i < N; i++)
        ACC_H(i) = ACC_H(i) - (product[i] < 0); /* `... + (product[i] >> 31)` */
    for (i = 0; i < N; i++)
        product[i] = product[i] << 1;
    for (i = 0; i < N; i++)
        addend[i] = (product[i] & 0x00000000FFFF) >>  0;
    for (i = 0; i < N; i++)
        addend[i] = (unsigned short)ACC_L(i) + addend[i];
    for (i = 0; i < N; i++)
        ACC_L(i) = (short)addend[i];
    for (i = 0; i < N; i++)
        addend[i] = (addend[i] >> 16) + (unsigned short)(product[i] >> 16);
    for (i = 0; i < N; i++)
        addend[i] = (unsigned short)ACC_M(i) + addend[i];
    for (i = 0; i < N; i++)
        ACC_M(i) = (short)addend[i];
    for (i = 0; i < N; i++)
        result[i] = (ACC_H(i) << 16) + addend[i];
    for (i = 0; i < N; i++)
        ACC_H(i) = (short)(result[i] >> 16);
    UNSIGNED_CLAMP(VD);
    return;
}

static void VMACU_v(void)
{
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    do_macu(VR[vd], VR[vs], VR[vt]);
    return;
}
static void VMACU0q(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x2);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU1q(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x3);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU0h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x4);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU1h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x5);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU2h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x6);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU3h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x7);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU0w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x8);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU1w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x9);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU2w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xA);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU3w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xB);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU4w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xC);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU5w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xD);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU6w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xE);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
static void VMACU7w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xF);
    do_macu(VR[vd], VR[vs], SV);
    return;
}
