/* Author:  G. Jungman
 * RCS:     $Id$
 */

#include <gsl_math.h>
#include <gsl_errno.h>
#include "gsl_sf_bessel.h"

#include "bessel_Y0_impl.h"
#include "bessel_Y1_impl.h"

#include "bessel_Yn_impl.h"


/*-*-*-*-*-*-*-*-*-*-*-* (semi)Private Implementations *-*-*-*-*-*-*-*-*-*-*-*/

int gsl_sf_bessel_Yn_impl(int n, const double x, double * result)
{
  int sign = 1;

  if(n < 0) {
    /* reduce to case n >= 0 */
    n = -n;
    if(GSL_IS_ODD(n)) sign = -1;
  }
  
  if(n == 0) {
    double b0 = 0.;
    int status = gsl_sf_bessel_Y0_impl(x, &b0);
    *result = sign * b0;
    return status;
  }
  else if(n == 1) {
    double b0 = 0.;
    int status = gsl_sf_bessel_Y1_impl(x, &b0);
    *result = sign * b0;
    return status;
  }
  else {
    if(x <= 0.) {
      return GSL_EDOM;
    }
    if(x < GSL_ROOT4_MACH_EPS) {
      double ln_x2  = log(0.5*x);
      double ln_fc  = gsl_sf_lnfact(n-1);
      double ln_pre = ln_fc - n * ln_x2;
      if(ln_pre > GSL_LOG_DBL_MAX - 2.) {
        *result = 0.; /* FIXME: should be Inf */
	return GSL_EOVRFLW;
      }
      else {
        double pre = exp(ln_pre);
        double Jn  = exp(n * ln_x2);
        double b0  = -pre * (1. + 0.25*x*x/(n-1.)) + 2./M_PI*ln_x2*Jn;
	*result = sign * b0;
	return GSL_SUCCESS;
      }
    }
    else if(x > 500.*n) {
      double ampl  = gsl_sf_bessel_asymp_Mnu(n, x);
      double theta = gsl_sf_bessel_asymp_thetanu(n, x);
      *result = sign * ampl * sin(theta);
      return GSL_SUCCESS;
    }
    else if(n < 30) {
      double b0 = 0.;
      int status = gsl_sf_bessel_Ynu_asymp_Olver_impl(n, x, &b0);
      *result = sign * b0;
      return status;
    }
    else {
      int j;
      double by, bym, byp;
      double two_over_x = 2.0/x;
      gsl_sf_bessel_Y1_impl(x, &by);
      gsl_sf_bessel_Y0_impl(x, &bym);
      for(j=1; j<n; j++) { 
	byp = j*two_over_x*by - bym;
	bym = by;
	by  = byp;
      }
      *result = sign * by;
      return GSL_SUCCESS;
    }
  }
}


/*-*-*-*-*-*-*-*-*-*-*-* Functions w/ Error Handling *-*-*-*-*-*-*-*-*-*-*-*/

int gsl_sf_bessel_Yn_e(const int n, const double x, double * result)
{
  int status = gsl_sf_bessel_Yn_impl(n, x, result);
  if(status != GSL_SUCCESS) {
    GSL_ERROR("gsl_sf_bessel_Yn_e", status);
  }
  return status;
}


/*-*-*-*-*-*-*-*-*-*-*-* Functions w/ Natural Prototypes *-*-*-*-*-*-*-*-*-*-*-*/

double gsl_sf_bessel_Yn(const int n, const double x)
{
  double y;
  int status = gsl_sf_bessel_Yn_impl(n, x, &y);
  if(status != GSL_SUCCESS) {
    GSL_WARNING("gsl_sf_bessel_Yn", status);
  }
  return y;
}
