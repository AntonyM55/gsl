/* zsolve.c - finds the complex roots of  = 0 */

#include <config.h>
#include <math.h>
#include <stdlib.h>
#include <gsl_math.h>
#include <gsl_errno.h>
#include <gsl_complex.h>
#include <gsl_poly.h>

/* C-style matrix elements */
#define MAT(m,i,j,n) ((m)[(i)*(n) + (j)])

/* Fortran-style matrix elements */
#define FMAT(m,i,j,n) ((m)[((i)-1)*(n) + ((j)-1)])

#include "companion.c"
#include "balance.c"
/* #include "norm.c" */
#include "qr.c"

int
gsl_poly_complex_solve (const double *a, size_t n,
			gsl_poly_complex_workspace * w,
			gsl_complex_packed_ptr z)
{
  int status;
  double *m;

  if (n == 0)
    {
      GSL_ERROR ("number of terms must be a positive integer", GSL_EINVAL);
    }

  if (n == 1)
    {
      GSL_ERROR ("cannot solve for only one term", GSL_EINVAL);
    }

  if (a[n - 1] == 0)
    {
      GSL_ERROR ("leading term of polynomial must be non-zero", GSL_EINVAL) ;
    }

  if (w->nc != n - 1)
    {
      GSL_ERROR ("size of workspace does not match polynomial", GSL_EINVAL);
    }
  
  m = w->matrix;

  set_companion_matrix (a, n - 1, m);

  balance_companion_matrix (m, n - 1);

  status = qr_companion (m, n - 1, z);

  if (status)
    {
      GSL_ERROR("root solving qr method failed to converge", GSL_EFAILED);
    }

  return GSL_SUCCESS;
}



