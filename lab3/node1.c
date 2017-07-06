#include "prog3.h"
#include <string.h> 

#define ME 1


extern int TRACE;
extern int YES;
extern int NO;



struct distance_table 
{
  int costs[4][4];
} dt1;

void printdt1(struct distance_table *dtptr);
static struct rtpkt new_rtpkt;
static int routing[4];

/* students to write the following two routines, and maybe some others */

static void broadcast(){
	for (int i = 0; i < 4; ++i)
	{
		if (i != ME && dt1.costs[i][i] != 999)
		{
			new_rtpkt.sourceid = ME;
			new_rtpkt.destid = i;
			for (int j = 0; j < 4; ++j)
			{
				new_rtpkt.mincost[j] = dt1.costs[routing[j]][j];
			}
			tolayer2(new_rtpkt);
		}
	}
}



void rtinit1() 
{	/*init value */
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			dt1.costs[i][j] = 999;
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		routing[i] = i;
	}

	dt1.costs[ME][ME] = 0;
	dt1.costs[0][0] = 1;
	dt1.costs[2][2] = 1;
	/*end init value*/ 
	broadcast();

	
}


void rtupdate1(struct rtpkt *rcvdpkt)
  
{
  //apply distance vector algorithm for updated mincost
	int src = rcvdpkt->sourceid, changed = 0;

	for (int i = 0; i < 4; ++i)
	{
		/*Compute new cost */
		int new_cost = dt1.costs[src][src] + rcvdpkt->mincost[i];
		/*Save old cost*/
		int old_cost = dt1.costs[routing[i]][i];
		/*update distance table*/
		dt1.costs[src][i] = new_cost;
		/* Update routing tabel*/
		if (new_cost < old_cost)
		{
			changed = 1;
			routing[i] = src;
		}
	}
	if (changed)
	{
		broadcast();
	}
	printdt1(&dt1);
}


void printdt1(struct distance_table *dtptr)

  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[2][0]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[0][2], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[0][3], dtptr->costs[2][3]);

}



void linkhandler1(int linkid, int newcost)

/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}


