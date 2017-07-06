#include "prog3.h"
#include <string.h> 

#define ME 2


extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt2;

void printdt2(struct distance_table *dtptr);


/* students to write the following two routines, and maybe some others */
static struct rtpkt new_rtpkt;
static int routing[4];


static void broadcast(){
  for (int i = 0; i < 4; ++i)
  {
    if (i != ME && dt2.costs[i][i] != 999)
    {
      new_rtpkt.sourceid = ME;
      new_rtpkt.destid = i;
      for (int j = 0; j < 4; ++j)
      {
        new_rtpkt.mincost[j] = dt2.costs[routing[j]][j];
      }
      tolayer2(new_rtpkt);
    }
  }
}



void rtinit2() 
{ /*init value */
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      dt2.costs[i][j] = 999;
    }
  }
  for (int i = 0; i < 4; ++i)
  {
    routing[i] = i;
  }

  dt2.costs[ME][ME] = 0;
  dt2.costs[0][0] = 3;
  dt2.costs[1][1] = 1;
  dt2.costs[3][3] = 2;
  /*end init value*/ 
  broadcast();

  
}


void rtupdate2(struct rtpkt *rcvdpkt)
  
{
  //apply distance vector algorithm for updated mincost
  int src = rcvdpkt->sourceid, changed = 0;

  for (int i = 0; i < 4; ++i)
  {
    /*Compute new cost */
    int new_cost = dt2.costs[src][src] + rcvdpkt->mincost[i];
    /*Save old cost*/
    int old_cost = dt2.costs[routing[i]][i];
    /*update distance table*/
    dt2.costs[src][i] = new_cost;
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
  printdt2(&dt2);  
}


void printdt2(struct distance_table *dtptr)

  
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[1][0],dtptr->costs[3][0]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[0][1],
	 dtptr->costs[1][1],dtptr->costs[3][1]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[0][3],
	 dtptr->costs[1][3],dtptr->costs[3][3]);
}
