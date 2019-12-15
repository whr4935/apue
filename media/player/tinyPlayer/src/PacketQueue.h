#ifndef	PACKET_QUEUE_H
#define PACKET_QUEUE_H

typedef struct PacketQueue {
	AVPacketList *firtst_pkt, *last_pkt;
	int nb_packets;
	int size;
	int abort;
	SDL_mutex *mutex;
	SDL_cond *cond;
}PacketQueue;

int InitPacketQueue(PacketQueue *q);
int DestroyPacketQueue(PacketQueue *q);
int AbortPakcetQueue(PacketQueue *q);
int FlushPacketQueue(PacketQueue *q);
int PutPacketQueue(PacketQueue *q, AVPacket *pkt);
int GetPacketQueue(PacketQueue *q, AVPacket *pkt, int block);


#endif