/**
    \file interpolate.c
    \author Sachith Dunatunga
    \date 04.06.12

    mpm_2d -- An implementation of the Material Point Method in 2D.
*/
#include <stdio.h>
#include "interpolate.h"

#define SF_WARNING4(tok,xl,yl) \
    if (*tok > 1 || *tok < 0) { \
        fprintf(stderr, "warning, " #tok " = %g", *tok); \
        fprintf(stderr, ", (%g, %g)\n", xl, yl); \
    }

#define SF_WARNING(tok) \
    if (*tok > 1 || *tok < -1) { \
        fprintf(stderr, "warning, " #tok " = %g\n", *tok); \
    }

#if 0
/*---tent---------------------------------------------------------------------*/
void tent(double * restrict h1, double * restrict h2, double * restrict h3, double * restrict h4,
    double x_local, double y_local)
{
    *h1 = (1 - x_local) * (1 - y_local);
    *h2 = (x_local) * (1 - y_local);
    *h3 = (x_local) * (y_local);
    *h4 = (1 - x_local) * (y_local);

    SF_WARNING4(h1, x_local, y_local);
    SF_WARNING4(h2, x_local, y_local);
    SF_WARNING4(h3, x_local, y_local);
    SF_WARNING4(h4, x_local, y_local);

    return;
}
/*----------------------------------------------------------------------------*/

/*---grad_tent----------------------------------------------------------------*/
void grad_tent(double * restrict b11, double * restrict b12, double * restrict b13, double * restrict b14,
    double * restrict b21, double * restrict b22, double * restrict b23, double * restrict b24,
    double x_local, double y_local, double hx, double hy, double Lx, double Ly)
{
    double dxl_dx = (1.0 / hx);
    double dyl_dy = (1.0 / hy);

    /*
        d/dx1
    */
    *b11 = -(1 - y_local) * dxl_dx;
    *b12 = (1 - y_local) * dxl_dx;
    *b13 = (y_local) * dxl_dx;
    *b14 = -(y_local) * dxl_dx;

    /*
        d/dx2
    */
    *b21 = -(1 - x_local) * dyl_dy;
    *b22 = -(x_local) * dyl_dy;
    *b23 = (x_local) * dyl_dy;
    *b24 = (1 - x_local) * dyl_dy;

    return;
}
/*----------------------------------------------------------------------------*/
#endif

/*
    Node order is

    4----3
    |    |
    |    |
    1----2

    (1 in lower left, increases in positive direction ccw).
*/

void tent(
    double * restrict h1, double * restrict h2, double * restrict h3, double * restrict h4,
    double x1, double y1,
    double x2, double y2,
    double x3, double y3,
    double x4, double y4,
    double hx, double hy,
    double xp, double yp
)
{
    const double da = (hx * hy);

    // *h1 = ((hx + x1 - xp) / hx) * ((hy + y1 - yp) / hy);
    *h1 = (hx + x1 - xp) * (hy + y1 - yp) / da;
    // *h2 = ((hx + xp - x2) / hx) * ((hy + y2 - yp) / hy);
    *h2 = (hx + xp - x2) * (hy + y2 - yp) / da;
    // *h3 = ((hx + xp - x3) / hx) * ((hy + yp - y3) / hy);
    *h3 = (hx + xp - x3) * (hy + yp - y3) / da;
    // *h4 = ((hx + x4 - xp) / hx) * ((hy + yp - y4) / hy);
    *h4 = (hx + x4 - xp) * (hy + yp - y4) / da;

    SF_WARNING4(h1, xp, yp);
    SF_WARNING4(h2, xp, yp);
    SF_WARNING4(h3, xp, yp);
    SF_WARNING4(h4, xp, yp);

    return;
}

void grad_tent(
    double * restrict b11, double * restrict b12, double * restrict b13, double * restrict b14,
    double * restrict b21, double * restrict b22, double * restrict b23, double * restrict b24,
    double x1, double y1,
    double x2, double y2,
    double x3, double y3,
    double x4, double y4,
    double hx, double hy,
    double xp, double yp
)
{
    const double da = (hx * hy);

    // *h1 = ((hx + x1 - xp) / hx) * ((hy + y1 - yp) / hy);
    // *h2 = ((hx + xp - x2) / hx) * ((hy + y2 - yp) / hy);
    // *h3 = ((hx + xp - x3) / hx) * ((hy + yp - y3) / hy);
    // *h4 = ((hx + x4 - xp) / hx) * ((hy + yp - y4) / hy);

    /*
        d/dx1
    */
    *b11 = - (hy + y1 - yp) / da;
    *b12 = (hy + y2 - yp) / da;
    *b13 = (hy + yp - y3) / da;
    *b14 = - (hy + yp - y4) / da;

    /*
        d/dx2
    */
    *b21 = - (hx + x1 - xp) / da;
    *b22 = - (hx + xp - x2) / da;
    *b23 = (hx + xp - x3) / da;
    *b24 = (hx + x4 - xp) / da;

    return;
}
