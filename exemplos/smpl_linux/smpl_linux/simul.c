// compilar com -lm

/**********************************************************************/
/*                                                                    */
/*         File csqm.c:  Central Server Queueing Network Model        */
/*                                                                    */
/*  This is the queueing network simulation program of Figure 2.2     */
/*  (with a call to "reportf()" added).                               */
/*                                                                    */
/**********************************************************************/

//#include "smpl.h"
#include "smpl.c"
#include "stat.c"
#include "rand.c"


#define nt 10        /* total no. of tasks */
#define qd 1         /* queued req. return */

int
  disk, 	     /* disk facility descriptors */
  cpu,               /* cpu facility descriptor   */
  mf1,
  mf2,proba;

real
  tc=10.0,	     /* class 0,1 mean cpu times  */
  tm1=1.0,
  tm2=1.0,
  td=30.0, sd=7.5;   /* disk time mean, std. dev. */

int main(void)
  {
    int i,j,event,n;
    real t,s;

    n=0;
    s=0.0;

    smpl(0,"central server model");
    cpu=facility("CPU",1);
    disk=facility("disk",1);
    mf1=facility("mf1",1);
    mf2=facility("mf2",1);

    i=1;

    schedule(1,0.0,i);

    while (i<100)
      {
	cause(&event,&i);
	switch(event)
	  {
	    case 1:  /* begin tour */
	      schedule(2,0.0,i);
	      i++;
	      schedule(1,30.0,i);
	      break;
	    case 2:  /* request cpu */
	      if (request(cpu,i,0)!=qd) then
		schedule(3,expntl(tc),i);
	      break;
	    case 3:  /* release cpu, select disk */
	      release(cpu,i);
	      schedule(4,0.0,i);
	      break;
	    case 4:
	      if (request(mf1,i,0) != qd) then
		schedule(5,expntl(tm1),i);
	      break;
	    case 5:  /* request disk */
	      release(mf1,i);
	      schedule(6,0.0,i);
	      break;
	    case 6:
	      if (request(disk,i,0) != qd) then
		schedule(7,erlang(td,sd),i);
	      break;
	    case 7:
	      release(disk,i);
	      schedule(8,0.0,i);
	      break;
	    case 8:
	      if (request(mf2,i,0) != qd) then
		schedule(9,expntl(tm2),i);
	      break;
	    case 9:  /* release disk, end tour */
	      release(mf2,i);
	      proba = smpl_random(1,100);
	      if (proba < 20)
		      schedule(1,0.0,i);
	      break;
	  }
      }
    reportf();
    printf("\n\n");
  return 0;
  }

