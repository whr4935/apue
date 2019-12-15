#include "player.h"

int InitPacketQueue(PacketQueue *q)
{
	memset(q, 0, sizeof(PacketQueue));

	q->mutex = SDL_CreateMutex();
	q->cond = SDL_CreateCond();
	
	return 0;
}

int DestroyPacketQueue(PacketQueue *q)
{
	SDL_DestroyMutex(q->mutex);
	SDL_DestroyCond(q->cond);

	return 0;
}

int FlushPacketQueue(PacketQueue *q)
{
	return 0;
}

int AbortPakcetQueue(PacketQueue *q)
{
	SDL_LockMutex(q->mutex);
	q->abort = 1;
	SDL_CondSignal(q->cond);
	SDL_UnlockMutex(q->mutex);

	return 0;
}

int PutPacketQueue(PacketQueue *q, AVPacket *pkt)
{
	AVPacketList *pkt1;

	//av_packet_unref(pkt) ;
		//return -1;

	pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));
	pkt1->pkt = *pkt;
	pkt1->next = NULL;

	SDL_LockMutex(q->mutex);

	if (!q->firtst_pkt)
		q->firtst_pkt = pkt1;
	else
		q->last_pkt->next = pkt1;
	q->last_pkt = pkt1;

	q->size += pkt->size;
	q->nb_packets++;

	SDL_CondSignal(q->cond);

	SDL_UnlockMutex(q->mutex);

	return 0;
}

int GetPacketQueue(PacketQueue *q, AVPacket *pkt, int block)
{
	AVPacketList *pkt1;
	int ret = 0;

	SDL_LockMutex(q->mutex);
	for (;;) {
		if (q->abort == 1)
			break;

		pkt1 = q->firtst_pkt;
		if (pkt1) {
			q->firtst_pkt = pkt1->next;
			if (!q->firtst_pkt)
				q->last_pkt = NULL;
			q->nb_packets--;
			q->size -= pkt1->pkt.size;	
			*pkt = pkt1->pkt;
			av_free(pkt1);
			ret = 1;
			break;
		} else if (!block) {
			ret = 0;
			break;
		} else {
			SDL_CondWait(q->cond, q->mutex);
		}
	}
	SDL_UnlockMutex(q->mutex);

	return 0;
}
