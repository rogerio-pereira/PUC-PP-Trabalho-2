/*********************************************************************/
/*                                                                   */
/*                       >>> stat.c <<<                              */
/*       Normal and T Distribution Quantile Computation Functions    */
/*                                                                   */
/*********************************************************************/
#include  <math.h>

/*---------- COMPUTE pth QUANTILE OF THE NORMAL DISTRIBUTION --------*/
double Z(p)
  double    p ;
{
  double  q,z1,n,d ; 
  double  sqrt(),log() ;
  
  q = (p>0.5)? (1-p):p ;
  z1 = sqrt( -2.0*log(q) ) ;
  n = ( 0.010328*z1+0.802853 ) * z1 + 2.515517 ;
  d = ( (0.001308*z1+0.189269) * z1 + 1.43278 ) * z1 + 1 ;
  z1 -= n/d ; 
  if( p>0.5 ) {
    z1 = -z1 ;
  }
  return( z1 ) ;
}

/*---------- COMPUTE pth QUANTILE OF THE t DISTRIBUTION -------------*/
double T( p,ndf )
  double  p ;
  int    ndf ;
{
  int  i ;
  double  z1,z2,h[4],x=0.0 ;
  double  fabs() ;

  z1 = fabs( Z(p) ) ;
  z2 = z1*z1 ;

  h[0] = 0.25*z1*(z2+1.0) ;
  h[1] = 0.010416667*z1*( (5.0*z2+16.0) *z2+3.0 ) ;
  h[2] = 0.002604167*z1*( ( (3.0*z2+19.0) *z2+17.0) *z2-15.0 ) ;
  h[3] = z1*( ( ( ( 79.0*z2+776.0) *z2+1482.0) *z2-1920.0) *z2-945.0 );
  h[3] *= 0.000010851 ;

  for( i=3;i>=0;i-- ) {
    x = (x+h[i])/(double)ndf ;
  }
  z1 +=x ;
  if( p>0.5 ) {
    z1 = -z1 ;
  }
  return( z1 ) ;
}

