/**
    \file particle.c
    \author Sachith Dunatunga
    \date 04.06.12

    mpm_2d -- An implementation of the Material Point Method in 2D.
*/
#include "particle.h"
#include <math.h>
#define TOL 1e-10
#define SGN(x) (((x) > 0) - ((x) < 0))

/*---global_to_local_coords---------------------------------------------------*/
void global_to_local_coords(double *x_local, double *y_local,
    double x, double y, 
    double x_ref, double y_ref, double h)
{
    double xl_sgn, yl_sgn;

    *x_local = (x - x_ref) / h;
    xl_sgn = copysign(1.0, *x_local);
    if ((xl_sgn * (*x_local)) > 1.0f && ((xl_sgn * (*x_local)) - 1.0f) < TOL) {
/*        printf("x_local: %32.32g\n", *x_local);*/
        *x_local = xl_sgn;
    }

    *y_local = (y - y_ref) / h;
    yl_sgn = copysign(1.0, *y_local);
    if ((yl_sgn * (*y_local)) > 1.0f && ((yl_sgn * (*y_local)) - 1.0f) < TOL) {
/*        printf("y_local: %32.32g\n", *y_local);*/
        *y_local = yl_sgn;
    }
	return;
}
/*----------------------------------------------------------------------------*/

/*---which_element------------------------------------------------------------*/
int which_element(double x_particle, double y_particle, 
    int N, double h)
{
    int i;
    int j;

    i = floor(x_particle / h);
    j = floor(y_particle / h);

	return (i + j*(N-1));
}
/*----------------------------------------------------------------------------*/
