#include <stdio.h>
#include <gsl_errno.h>
#include <gsl_vector.h>
#include <gsl_histogram.h>

int
gsl_histogram_fread (FILE * stream, gsl_histogram * h)
{
  int status = gsl_block_fread (stream, h->range, h->nbins + 1) ;

  if (status) 
    return status ;
  
  status = gsl_block_fread (stream, h->bin, h->nbins) ;
  return status ;
}

int
gsl_histogram_fwrite (FILE * stream, const gsl_histogram * h)
{
  int status = gsl_block_fwrite (stream, h->range, h->nbins + 1) ;

  if (status) 
    return status ;
  
  status = gsl_block_fwrite (stream, h->bin, h->nbins) ;
  return status ;
}

int
gsl_histogram_fprintf (FILE * stream, const gsl_histogram * h, 
		       const char * format)
{
  size_t i ;
  const size_t n = h->nbins ;

  for (i = 0 ; i < n ; i++) 
    {
      int status = fprintf(stream, format, h->range[i]) ;
      
      if (status < 0) 
	{
	  GSL_ERROR ("fprintf failed", GSL_EFAILED) ;
	}

      status = putc(' ', stream) ;

      if (status == EOF) 
	{
	  GSL_ERROR ("putc failed", GSL_EFAILED) ;
	}

      status = fprintf(stream, format, h->range[i + 1]) ;
      
      if (status < 0) 
	{
	  GSL_ERROR ("fprintf failed", GSL_EFAILED) ;
	}

      status = putc(' ', stream) ;

      if (status == EOF) 
	{
	  GSL_ERROR ("putc failed", GSL_EFAILED) ;
	}
      
      status = fprintf(stream, format, h->bin[i]) ;

      if (status < 0) 
	{
	  GSL_ERROR ("fprintf failed", GSL_EFAILED) ;
	}

      status = putc('\n', stream) ;

      if (status == EOF) 
	{
	  GSL_ERROR ("putc failed", GSL_EFAILED) ;
	}
    }

  return 0 ;
}

int
gsl_histogram_fscanf (FILE * stream, gsl_histogram * h)
{
  size_t i ;
  const size_t n = h->nbins ;
  double upper ;

  for (i = 0 ; i < n ; i++) 
    {
      int status = fscanf(stream, 
			  "%lg %lg %lg", h->range + i, &upper, 
			  h->bin + i);

      if (status != 3) 
	{
	  GSL_ERROR ("fscanf failed", GSL_EFAILED) ;
	}
    }

  h->range[n] = upper ;

  return 0;
} 
