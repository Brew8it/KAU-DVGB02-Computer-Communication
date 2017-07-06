#include "prog3.h"
#include <string.h>

extern int TRACE;
extern int YES;
extern int NO;

#define ME 3

struct distance_table
{
  int costs[4][4];
} dt3;


void printdt3(struct distance_table *dtptr);


/* students to write the following two routines, and maybe some others */

static struct rtpkt new_rtpkt;
static int routing[4];


static void broadcast(){
  for (int i = 0; i < 4; ++i)
  {
    if (i != ME && dt3.costs[i][i] != 999)
    {
      new_rtpkt.sourceid = ME;
      new_rtpkt.destid = i;
      for (int j = 0; j < 4; ++j)
      {
        new_rtpkt.mincost[j] = dt3.costs[routing[j]][j];
      }
      tolayer2(new_rtpkt);
    }
  }
}



void rtinit3() 
{ /*init value */
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      dt3.costs[i][j] = 999;
    }
  }
  for (int i = 0; i < 4; ++i)
  {
    routing[i] = i;
  }

  dt3.costs[ME][ME] = 0;
  dt3.costs[0][0] = 7;
  dt3.costs[2][2] = 2;
  /*end init value*/ 
  broadcast();

  
}


void rtupdate3(struct rtpkt *rcvdpkt)
  
{
  //apply distance vector algorithm for updated mincost
  int src = rcvdpkt->sourceid, changed = 0;

  for (int i = 0; i < 4; ++i)
  {
    /*Compute new cost */
    int new_cost = dt3.costs[src][src] + rcvdpkt->mincost[i];
    /*Save old cost*/
    int old_cost = dt3.costs[routing[i]][i];
    /*update distance table*/
    dt3.costs[src][i] = new_cost;
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
  printdt3(&dt3);
}


void printdt3(struct distance_table *dtptr)


{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[2][0]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[0][1], dtptr->costs[2][1]);
  printf("     2|  %3d   %3d\n",dtptr->costs[0][2], dtptr->costs[2][2]);

}







