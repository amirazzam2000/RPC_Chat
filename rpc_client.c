/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpc.h"
#include <string.h>


void
program_write_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
	message  msg;
	chat_block  *result_2;
	int  getchat_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, PROGRAM_WRITE, VERSION_WRITE, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	//printf("[+%s*]: ", msg.name);
	strcpy(msg.message , "hello! :P\n");
	
	result_1 = write_1(&msg, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = getchat_1(&getchat_1_arg, clnt);
	if (result_2 == (chat_block *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	program_write_1 (host);
exit (0);
}
