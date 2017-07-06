#include "prog3.h"
#include <string.h> 

#define ME 0


extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt0;

void printdt0(struct distance_table *dtptr);


static struct rtpkt new_rtpkt;
static int routing[4];


/* students to write the following two routines, and maybe some others */

static void broadcast(){
	for (int i = 0; i < 4; ++i)
	{
		if (i != ME && dt0.costs[i][i] != 999)
		{
			new_rtpkt.sourceid = ME;
			new_rtpkt.destid = i;
			for (int j = 0; j < 4; ++j)
			{
				new_rtpkt.mincost[j] = dt0.costs[routing[j]][j];
			}
			tolayer2(new_rtpkt);
		}
	}
}



void rtinit0() 
{	/*init value */
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			dt0.costs[i][j] = 999;
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		routing[i] = i;
	}

	dt0.costs[ME][ME] = 0;
	dt0.costs[1][1] = 1;
	dt0.costs[2][2] = 3;
	dt0.costs[3][3] = 7;
	/*end init value*/ 
	broadcast();

	
}


void rtupdate0(struct rtpkt *rcvdpkt)
  
{
  //apply distance vector algorithm for updated mincost
	int src = rcvdpkt->sourceid, changed = 0;

	for (int i = 0; i < 4; ++i)
	{
		/*Compute new cost */
		int new_cost = dt0.costs[src][src] + rcvdpkt->mincost[i];
		/*Save old cost*/
		int old_cost = dt0.costs[routing[i]][i];
		/*update distance table*/
		dt0.costs[src][i] = new_cost;
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
	printdt0(&dt0);
}

void printdt0(struct distance_table *dtptr){
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[2][1],dtptr->costs[3][1]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[1][2],
	 dtptr->costs[2][2],dtptr->costs[3][2]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[1][3],
	 dtptr->costs[2][3],dtptr->costs[3][3]);
}



void linkhandler0(int linkid, int newcost)


/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}

