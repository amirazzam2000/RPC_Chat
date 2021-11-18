/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void
program_write_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
	message  msg;
	chat_block  *chat;
	int  getchat_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, PROGRAM_WRITE, VERSION_WRITE, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	printf("Enter your username: ");
	scanf("%s", msg.name);
	msg.name[ strlen(msg.name)] = 0;

	while(1)
	{
		printf("\n[%s]: ", msg.name);
		fgets(msg.message, 269, stdin);
		
		fflush(stdin);

		msg.message[strlen(msg.message) - 1] = 0;
		printf("|%s|", msg.message);

		if (strcmp(msg.message, "quit") == 0) break;
		
		if (strcmp(msg.message, "read") == 0) 
		{
			chat = getchat_1(&getchat_1_arg, clnt);
			printf("%s \n", chat->block);

			if (chat == (chat_block *)NULL)
			{
				clnt_perror(clnt, "call failed");
			}
			continue;
		}


			result_1 = write_1(&msg, clnt);
		if (result_1 == (int *)NULL)
		{
			clnt_perror(clnt, "call failed");
		}
	}
	
	chat = getchat_1(&getchat_1_arg, clnt);
	if (chat == (chat_block *) NULL) {
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
