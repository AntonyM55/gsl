#include <config.h>
#include <stdio.h>
#include <gsl/gsl_histogram2d.h>
#include <gsl/gsl_test.h>

#define M 107
#define N 239
#define M1 17
#define N1 23
#define MR 10
#define NR 5

int
main (void)
{
  double xr[MR + 1] =
  {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

  double yr[NR + 1] =
  {90.0, 91.0, 92.0, 93.0, 94.0, 95.0};

  gsl_histogram2d *h, *h1, *g, *hr;
  size_t i, j, k;

  h = gsl_histogram2d_calloc (M, N);
  h1 = gsl_histogram2d_calloc (M, N);
  g = gsl_histogram2d_calloc (M, N);

  gsl_test (h->xrange == 0,
	    "gsl_histogram2d_calloc returns valid xrange pointer");
  gsl_test (h->yrange == 0,
	    "gsl_histogram2d_calloc returns valid yrange pointer");
  gsl_test (h->bin == 0,
	    "gsl_histogram2d_calloc returns valid bin pointer");

  gsl_test (h->nx != M, "gsl_histogram2d_calloc returns valid nx");
  gsl_test (h->ny != N, "gsl_histogram2d_calloc returns valid ny");

  hr = gsl_histogram2d_calloc_range (MR, NR, xr, yr);

  gsl_test (hr->xrange == 0,
	    "gsl_histogram2d_calloc_range returns valid xrange pointer");
  gsl_test (hr->yrange == 0,
	    "gsl_histogram2d_calloc_range returns valid yrange pointer");
  gsl_test (hr->bin == 0,
	    "gsl_histogram2d_calloc_range returns valid bin pointer");

  gsl_test (hr->nx != MR, "gsl_histogram2d_calloc_range returns valid nx");
  gsl_test (hr->ny != NR, "gsl_histogram2d_calloc_range returns valid ny");

  {
    int status = 0;
    for (i = 0; i <= MR; i++)
      {
	if (hr->xrange[i] != xr[i])
	  {
	    status = 1;
	  }
      };

    gsl_test (status, "gsl_histogram2d_calloc_range creates xrange correctly");
  }

  {
    int status = 0;
    for (i = 0; i <= NR; i++)
      {
	if (hr->yrange[i] != yr[i])
	  {
	    status = 1;
	  }
      };

    gsl_test (status, "gsl_histogram2d_calloc_range creates yrange correctly");
  }

  k = 0;
  for (i = 0; i < M; i++)
    {
      for (j = 0; j < N; j++)
	{
	  k++;
	  gsl_histogram2d_accumulate (h, (double) i, (double) j, (double) k);
	};
    }

  {
    int status = 0;
    k = 0;
    for (i = 0; i < M; i++)
      {
	for (j = 0; j < N; j++)
	  {
	    k++;
	    if (h->bin[i * N + j] != (double) k)
	      {
		status = 1;
	      }
	  }
      }
    gsl_test (status,
	      "gsl_histogram2d_accumulate writes into array correctly");
  }

  {
    int status = 0;
    k = 0;
    for (i = 0; i < M; i++)
      {
	for (j = 0; j < N; j++)
	  {
	    k++;
	    if (gsl_histogram2d_get (h, i, j) != (double) k)
	      status = 1;
	  };
      }
    gsl_test (status, "gsl_histogram2d_get reads from array correctly");
  }

  for (i = 0; i <= M; i++)
    {
      h1->xrange[i] = 100.0 + i;
    }

  for (i = 0; i <= N; i++)
    {
      h1->yrange[i] = 900.0 + i*i;
    }

  gsl_histogram2d_memcpy (h1, h);

  {
    int status = 0;
    for (i = 0; i <= M; i++)
      {
	if (h1->xrange[i] != h->xrange[i])
	  status = 1;
      };
    gsl_test (status, "gsl_histogram2d_memcpy copies bin xranges correctly");
  }

  {
    int status = 0;
    for (i = 0; i <= N; i++)
      {
	if (h1->yrange[i] != h->yrange[i])
	  status = 1;
      };
    gsl_test (status, "gsl_histogram2d_memcpy copies bin yranges correctly");
  }

  {
    int status = 0;
    for (i = 0; i < M; i++)
      {
        for (j = 0; j < N; j++)
          {
            if (gsl_histogram2d_get (h1, i, j) != gsl_histogram2d_get (h, i, j))
              status = 1;
          }
      }
    gsl_test (status, "gsl_histogram2d_memcpy copies bin values correctly");
  }

  gsl_histogram2d_free (h1);

  h1 = gsl_histogram2d_clone (h);

  {
    int status = 0;
    for (i = 0; i <= M; i++)
      {
	if (h1->xrange[i] != h->xrange[i])
	  status = 1;
      };
    gsl_test (status, "gsl_histogram2d_clone copies bin xranges correctly");
  }

  {
    int status = 0;
    for (i = 0; i <= N; i++)
      {
	if (h1->yrange[i] != h->yrange[i])
	  status = 1;
      };
    gsl_test (status, "gsl_histogram2d_clone copies bin yranges correctly");
  }

  {
    int status = 0;
    for (i = 0; i < M; i++)
      {
        for (j = 0; j < N; j++)
          {
            if (gsl_histogram2d_get (h1, i, j) != gsl_histogram2d_get (h, i, j))
              status = 1;
          }
      }
    gsl_test (status, "gsl_histogram2d_clone copies bin values correctly");
  }


  gsl_histogram2d_reset (h);

  {
    int status = 0;

    for (i = 0; i < M * N; i++)
      {
	if (h->bin[i] != 0)
	  status = 1;
      }
    gsl_test (status, "gsl_histogram2d_reset zeros array correctly");
  }

  gsl_histogram2d_free (h);
  h = gsl_histogram2d_calloc (M1, N1);

  {

    int status = 0;

    for (i = 0; i < M1; i++)
      {
	for (j = 0; j < N1; j++)
	  {
	    gsl_histogram2d_increment (h, (double) i, (double) j);

	    for (k = 0; k <= i * N1 + j; k++)
	      {
		if (h->bin[k] != 1)
		  {
		    status = 1;
		  }
	      }

	    for (k = i * N1 + j + 1; k < M1 * N1; k++)
	      {
		if (h->bin[k] != 0)
		  {
		    status = 1;
		  }
	      }
	  }
      }
    gsl_test (status, "gsl_histogram2d_increment works correctly");
  }

  gsl_histogram2d_free (h);
  h = gsl_histogram2d_calloc (M, N);

  {
    int status = 0;
    for (i = 0; i < M; i++)
      {
	double x0 = 0, x1 = 0;
	gsl_histogram2d_get_xrange (h, i, &x0, &x1);

	if (x0 != i || x1 != i + 1)
	  {
	    status = 1;
	  }
      }
    gsl_test (status,
      "gsl_histogram2d_get_xlowerlimit and xupperlimit works correctly");
  }


  {
    int status = 0;
    for (i = 0; i < N; i++)
      {
	double y0 = 0, y1 = 0;
	gsl_histogram2d_get_yrange (h, i, &y0, &y1);

	if (y0 != i || y1 != i + 1)
	  {
	    status = 1;
	  }
      }
    gsl_test (status, "gsl_histogram2d_get_ylowerlimit and yupperlimit works correctly");
  }


  {
    int status = 0;
    if (gsl_histogram2d_xmax (h) != M)
      status = 1;
    gsl_test (status, "gsl_histogram2d_xmax works correctly");
  }

  {
    int status = 0;
    if (gsl_histogram2d_xmin (h) != 0)
      status = 1;
    gsl_test (status, "gsl_histogram2d_xmin works correctly");
  }

  {
    int status = 0;
    if (gsl_histogram2d_nx (h) != M)
      status = 1;
    gsl_test (status, "gsl_histogram2d_nx works correctly");
  }

  {
    int status = 0;
    if (gsl_histogram2d_ymax (h) != N)
      status = 1;
    gsl_test (status, "gsl_histogram2d_ymax works correctly");
  }

  {
    int status = 0;
    if (gsl_histogram2d_ymin (h) != 0)
      status = 1;
    gsl_test (status, "gsl_histogram2d_ymin works correctly");
  }

  {
    int status = 0;
    if (gsl_histogram2d_ny (h) != N)
      status = 1;
    gsl_test (status, "gsl_histogram2d_ny works correctly");
  }

  h->bin[3*N+2] = 123456.0;
  h->bin[4*N+3] = -654321;

  {
    double max = gsl_histogram2d_max_val (h);
    gsl_test (max != 123456.0, "gsl_histogram2d_max_val finds maximum value");
  }

  {
    double min = gsl_histogram2d_min_val (h);
    gsl_test (min != -654321.0, "gsl_histogram2d_min_val finds minimum value");
  }

  {
    size_t imax, jmax;
    gsl_histogram2d_max_bin (h, &imax, &jmax);
    gsl_test (imax != 3 || jmax != 2, "gsl_histogram2d_max_bin finds maximum value bin");
  }

  {
    size_t imin, jmin;
    gsl_histogram2d_min_bin (h, &imin, &jmin);
    gsl_test (imin != 4 || jmin != 3, "gsl_histogram2d_min_bin find minimum value bin");
  }

  for (i = 0; i < M*N; i++)
    {
      h->bin[i] = i + 27;
      g->bin[i] = (i + 27) * (i + 1);
    }

  gsl_histogram2d_memcpy (h1, g);
  gsl_histogram2d_add (h1, h);

  {
    int status = 0;
    for (i = 0; i < M*N; i++)
      {
	if (h1->bin[i] != g->bin[i] + h->bin[i])
	  status = 1;
      }
    gsl_test (status, "gsl_histogram2d_add works correctly");
  }

  gsl_histogram2d_memcpy (h1, g);
  gsl_histogram2d_sub (h1, h);

  {
    int status = 0;
    for (i = 0; i < M*N; i++)
      {
	if (h1->bin[i] != g->bin[i] - h->bin[i])
	  status = 1;
      }
    gsl_test (status, "gsl_histogram2d_sub works correctly");
  }


  gsl_histogram2d_memcpy (h1, g);
  gsl_histogram2d_mul (h1, h);

  {
    int status = 0;
    for (i = 0; i < M*N; i++)
      {
	if (h1->bin[i] != g->bin[i] * h->bin[i])
	  status = 1;
      }
    gsl_test (status, "gsl_histogram2d_mul works correctly");
  }

  gsl_histogram2d_memcpy (h1, g);
  gsl_histogram2d_div (h1, h);

  {
    int status = 0;
    for (i = 0; i < M*N; i++)
      {
	if (h1->bin[i] != g->bin[i] / h->bin[i])
	  status = 1;
      }
    gsl_test (status, "gsl_histogram2d_div works correctly");
  }

  gsl_histogram2d_memcpy (h1, g);
  gsl_histogram2d_scale (h1, 0.5);

  {
    int status = 0;
    for (i = 0; i < M*N; i++)
      {
	if (h1->bin[i] != 0.5 * g->bin[i])
	  status = 1;
      }
    gsl_test (status, "gsl_histogram2d_scale works correctly");
  }

  gsl_histogram2d_memcpy (h1, g);
  gsl_histogram2d_shift (h1, 0.25);

  {
    int status = 0;
    for (i = 0; i < M*N; i++)
      {
	if (h1->bin[i] != 0.25 + g->bin[i])
	  status = 1;
      }
    gsl_test (status, "gsl_histogram2d_shift works correctly");
  }

  gsl_histogram2d_free (h);	/* free whatever is in h */

  h = gsl_histogram2d_calloc_uniform (M1, N1, 0.0, 5.0, 0.0, 5.0);

  gsl_test (h->xrange == 0,
	    "gsl_histogram2d_calloc_uniform returns valid range pointer");
  gsl_test (h->yrange == 0,
	    "gsl_histogram2d_calloc_uniform returns valid range pointer");
  gsl_test (h->bin == 0,
	    "gsl_histogram2d_calloc_uniform returns valid bin pointer");
  gsl_test (h->nx != M1,
	    "gsl_histogram2d_calloc_uniform returns valid nx");
  gsl_test (h->ny != N1,
	    "gsl_histogram2d_calloc_uniform returns valid ny");

  gsl_histogram2d_accumulate (h, 0.0, 3.01, 1.0);
  gsl_histogram2d_accumulate (h, 0.1, 2.01, 2.0);
  gsl_histogram2d_accumulate (h, 0.2, 1.01, 3.0);
  gsl_histogram2d_accumulate (h, 0.3, 0.01, 4.0);

  {
    size_t i1, i2, i3, i4;
    size_t j1, j2, j3, j4;
    double expected;
    int status;
    status = gsl_histogram2d_find (h, 0.0, 3.01, &i1, &j1);
    status = gsl_histogram2d_find (h, 0.1, 2.01, &i2, &j2);
    status = gsl_histogram2d_find (h, 0.2, 1.01, &i3, &j3);
    status = gsl_histogram2d_find (h, 0.3, 0.01, &i4, &j4);

    for (i = 0; i < M1; i++)
      {
	for (j = 0; j < N1; j++)
	  {
	    if (i == i1 && j == j1)
	      {
		expected = 1.0;
	      }
	    else if (i == i2 && j == j2)
	      {
		expected = 2.0;
	      }
	    else if (i == i3 && j == j3)
	      {
		expected = 3.0;
	      }
	    else if (i == i4 && j == j4)
	      {
		expected = 4.0;
	      }
	    else
	      {
		expected = 0.0;
	      }

	    if (h->bin[i * N1 + j] != expected)
	      {
		status = 1;
	      }
	  }
      }
    gsl_test (status, "gsl_histogram2d_find works correctly");
  }

  {
    FILE *f = fopen ("test.txt", "w");
    gsl_histogram2d_fprintf (f, h, "%.19g", "%.19g");
    fclose (f);
  }

  {
    FILE *f = fopen ("test.txt", "r");
    gsl_histogram2d *hh = gsl_histogram2d_calloc (M1, N1);
    int status = 0;

    gsl_histogram2d_fscanf (f, hh);

    for (i = 0; i <= M1; i++)
      {
	if (h->xrange[i] != hh->xrange[i])
	  {
	    printf ("xrange[%d] : %g orig vs %g\n",
		    (int)i, h->xrange[i], hh->xrange[i]);
	    status = 1;
	  }
      }

    for (j = 0; j <= N1; j++)
      {
	if (h->yrange[j] != hh->yrange[j])
	  {
	    printf ("yrange[%d] : %g orig vs %g\n",
		    (int)j, h->yrange[j], hh->yrange[j]);
	    status = 1;
	  }
      }

    for (i = 0; i < M1 * N1; i++)
      {
	if (h->bin[i] != hh->bin[i])
	  {
	    printf ("bin[%d] : %g orig vs %g\n", 
		    (int)i, h->bin[i], hh->bin[i]);
	    status = 1;
	  }
      }

    gsl_test (status, "gsl_histogram2d_fprintf and fscanf work correctly");

    gsl_histogram2d_free (hh);
    fclose (f);
  }

  {
    FILE *f = fopen ("test.dat", "w");
    gsl_histogram2d_fwrite (f, h);
    fclose (f);
  }

  {
    FILE *f = fopen ("test.dat", "r");
    gsl_histogram2d *hh = gsl_histogram2d_calloc (M1, N1);
    int status = 0;

    gsl_histogram2d_fread (f, hh);

    for (i = 0; i <= M1; i++)
      {
	if (h->xrange[i] != hh->xrange[i])
	  {
	    printf ("xrange[%d] : %g orig vs %g\n",
		    (int)i, h->xrange[i], hh->xrange[i]);
	    status = 1;
	  }
      }

    for (j = 0; j <= N1; j++)
      {
	if (h->yrange[j] != hh->yrange[j])
	  {
	    printf ("yrange[%d] : %g orig vs %g\n",
		    (int)j, h->yrange[j], hh->yrange[j]);
	    status = 1;
	  }
      }

    for (i = 0; i < M1 * N1; i++)
      {
	if (h->bin[i] != hh->bin[i])
	  {
	    printf ("bin[%d] : %g orig vs %g\n", 
		    (int)i, h->bin[i], hh->bin[i]);
	    status = 1;
	  }
      }

    gsl_test (status, "gsl_histogram2d_fwrite and fread work correctly");

    gsl_histogram2d_free (hh);
    fclose (f);
  }
  
  gsl_histogram2d_free (h);
  gsl_histogram2d_free (h1);
  gsl_histogram2d_free (g);
  gsl_histogram2d_free (hr);

  return gsl_test_summary ();
}
