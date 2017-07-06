/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#include "Stud.h"

#define DEBUGG_A 0
#define DEBUGG_A_MORE 0
#define DEBUGG_A_MORE_1 0
#define DEBUGG_B 0
#define DEBUGG_B_MORE 0

extern float time;
float starttime, estRTT = 10.0, Timeout = 15.0,DevRTT = 0.5,SampelRTT;
int seqnumber = 0;
int ReadyforTransmitt = 1;
int b_seqnumber = 0;
int A_input_seqnumber = 0;
struct pkt Output_deliver,Output_saved;

int checksum(struct pkt deliver){
	int sum = 0;
	for (int i = 0; i < 20; ++i)
	{
		sum +=(int)deliver.payload[i];
	}
	sum += deliver.seqnum + deliver.acknum;
	return sum;
}
struct pkt A_makepacket(int seq,struct msg message){
	if(DEBUGG_A_MORE){printf("A is makeing a packet\n");}
	struct pkt deliver;
	deliver.seqnum = seq;
	deliver.acknum = 0;
	memcpy(&deliver.payload,&message.data,sizeof(message.data));
	deliver.checksum = checksum(deliver);
	return deliver;
}
struct pkt B_makepacket(int ACKnum,struct pkt deliver){
	if(DEBUGG_B_MORE){printf("B is making a packet\n");}
	deliver.acknum = ACKnum;
	deliver.checksum = checksum(deliver);
	return deliver;
}
int A_isCorrupt(struct pkt packet){
	if(DEBUGG_A_MORE){printf("A side checking if packet is corrupt\n");}
	if (packet.checksum != checksum(packet)){
		return 1;
	}
	else{
		return 0;
	}
}
int B_isCorrupt(struct pkt packet){
	if(DEBUGG_B_MORE){printf("A side checking if packet is corrupt\n");}
	if (packet.checksum != checksum(packet)){
		return 1;
	}
	else{
		return 0;
	}
}

int A_isACK(struct pkt packet,int recvACK){
	if(DEBUGG_A_MORE){printf("A side is checking if incomming packet is ACK\n");}
	if (packet.acknum == 1 && packet.seqnum == recvACK)
	{
		return 1;
	}
	else{
		return 0;
	}
}
char *B_extractData(struct pkt packet){
	if(DEBUGG_B_MORE){printf("B extracting data to send to layer 5\n");}
	char* dataset = malloc(sizeof(packet.payload));
	memcpy(dataset,packet.payload,sizeof(packet.payload));
	return dataset;
}

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message){
	if (ReadyforTransmitt == 1)
	{
		ReadyforTransmitt = 0;
		if(DEBUGG_A){printf("Layer 5 A side wants to send a packet new packet to B\n");}
		if (seqnumber == 2){ seqnumber = 0;}
		Output_deliver = A_makepacket(seqnumber,message);
		starttime = time;
		if(DEBUGG_A)printf("Timeout value = %f\n", Timeout);
		starttimer(0,Timeout);
		printf("SENDING TO B\n");
		Output_saved = Output_deliver;
		tolayer3(0,Output_deliver);
		seqnumber++;
	}
	else{
		if (DEBUGG_A_MORE_1){printf("Layer 5 tried to send a packet but another packet is alreadyt in transit.\n");		}
	}
}

void B_output(struct msg message){  /* need be completed only for extra credit */

}
/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet){
	if(DEBUGG_A){printf("A side has recived a packet form B\n");}
	if (A_input_seqnumber == 2){A_input_seqnumber = 0;}
	if (!A_isCorrupt(packet) && A_isACK(packet,A_input_seqnumber))
	{
		if(DEBUGG_A_MORE_1){printf("A's incomming packet was OK\n");}
		SampelRTT = time - starttime;				//endtime - starttime;
		estRTT = (estRTT + SampelRTT) / 2;
		Timeout = estRTT + 4 * DevRTT;
		stoptimer(0);
		ReadyforTransmitt = 1;
		A_input_seqnumber++;
	}
	else{
		printf("NACK was reviced.\n");
	}
}


/* called when A's timer goes off */
void A_timerinterrupt(){
	if(DEBUGG_A){printf("A's timer has timeout. Resending packet.\n");}
	starttime = time;
	Timeout *= 2;
	if(DEBUGG_A){printf("A's new Timeout value = %f\n",Timeout);}
	printf("Timer was triggerd, resending: %s\n",Output_saved.payload );
	printf("SENDING TO B\n");
	starttimer(0,Timeout);
	tolayer3(0,Output_saved);
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init(){

}


/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet){
	if (b_seqnumber == 2) {b_seqnumber = 0;}
	if(DEBUGG_B){printf("B_seq# wait state = %d, packet seqnum = %d\n",b_seqnumber,packet.seqnum);}
	if (!B_isCorrupt(packet) && packet.seqnum == b_seqnumber){
		if(DEBUGG_B){printf("B_input_ACK tolayer 3 and 5\n");}
		tolayer3(1,B_makepacket(1,packet));	
		tolayer5(1,B_extractData(packet));
		b_seqnumber++;
	}
	else if (!B_isCorrupt(packet) && packet.seqnum != b_seqnumber){
		if(DEBUGG_B){printf("B_input_ACK tolayer 3\n");}
		tolayer3(1,B_makepacket(1,packet));
	}
	else if(B_isCorrupt(packet)){
		if(DEBUGG_B){printf("B_input_NACK tolayer 3\n");}
		tolayer3(1,B_makepacket(0,packet));
	}
}

/* called when B's timer goes off */
void B_timerinterrupt(){

}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init(){
	
}
