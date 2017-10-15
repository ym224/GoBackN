#include "gbn.h"

uint16_t checksum(uint16_t *buf, int nwords)
{
	uint32_t sum;

	for (sum = 0; nwords > 0; nwords--)
		sum += *buf++;
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return ~sum;
}

ssize_t gbn_send(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr * dest, socklen_t socklen){
	// check current state

    // if state
	/* TODO: Your code here. */

	/* Hint: Check the data length field 'len'.
	 *       If it is > DATALEN, you will have to split the data
	 *       up into multiple packets - you don't have to worry
	 *       about getting more than N * DATALEN.
	 */

	socklen_t socketlen = sizeof(struct sockaddr);
	if (len > DATALEN) {
		// split data into multiple packets
		int numPackets = (int)len / DATALEN;
		if (len % DATALEN != 0) {
			numPackets ++;
		}
		char slicedBuf[DATALEN];
		for (int i=0; i<numPackets; i++) {
			memcpy(slicedBuf, &buf[i*DATALEN], DATALEN);
			slicedBuf[DATALEN] = '\0';
			sendto(sockfd, slicedBuf, DATALEN, flags, dest, socketlen);
		}
	}

	return sendto(sockfd, buf, DATALEN, flags, dest, socklen);
}

ssize_t gbn_recv(int sockfd, void *buf, size_t len, int flags){

	/* TODO: Your code here. */

	return(-1);
}

int gbn_close(int sockfd){

	/* TODO: Your code here. */

	return(-1);
}

int gbn_connect(int sockfd, const struct sockaddr *server, socklen_t socklen){
    //set signal handler

	/* TODO: Your code here. */

	return connect(sockfd, server, socklen);
}

int gbn_listen(int sockfd, int backlog){

	/* TODO: Your code here. */

	return listen(sockfd, backlog);
}

int gbn_bind(int sockfd, const struct sockaddr *server, socklen_t socklen){

	/* TODO: Your code here. */

	return bind(sockfd, server, socklen);
}	

int gbn_socket(int domain, int type, int protocol){
		
	/*----- Randomizing the seed. This is used by the rand() function -----*/
	srand((unsigned)time(0));

	/* TODO: Your code here. */

	return socket(domain, type, protocol);
}

int gbn_accept(int sockfd, struct sockaddr *client, socklen_t *socklen){

	/* TODO: Your code here. */

	return accept(sockfd, client, socklen);
}

ssize_t maybe_sendto(int  s, const void *buf, size_t len, int flags, \
                     const struct sockaddr *to, socklen_t tolen){

	char *buffer = malloc(len);
	memcpy(buffer, buf, len);
	
	
	/*----- Packet not lost -----*/
	if (rand() > LOSS_PROB*RAND_MAX){
		/*----- Packet corrupted -----*/
		if (rand() < CORR_PROB*RAND_MAX){
			
			/*----- Selecting a random byte inside the packet -----*/
			int index = (int)((len-1)*rand()/(RAND_MAX + 1.0));

			/*----- Inverting a bit -----*/
			char c = buffer[index];
			if (c & 0x01)
				c &= 0xFE;
			else
				c |= 0x01;
			buffer[index] = c;
		}

		/*----- Sending the packet -----*/
		int retval = sendto(s, buffer, len, flags, to, tolen);
		free(buffer);
		return retval;
	}
	/*----- Packet lost -----*/
	else
		return(len);  /* Simulate a success */
}