/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void
program_write_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
	message  msg;
	chat_block  *chat;
	int  my_revision;

#ifndef	DEBUG
	clnt = clnt_create (host, PROGRAM_WRITE, VERSION_WRITE, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	printf("Enter your username: ");
	fgets(msg.name, 10, stdin);
	msg.name[ strlen(msg.name) - 1 ] = 0;

	int flags = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, flags | O_NONBLOCK);

	while(1)
	{
		//Get chat
		do
		{
			chat = getchat_1(&my_revision, clnt);
			printf("%s", chat->block);
			my_revision = chat->revision_number;
			
			if (my_revision >= chat->total_revisions)
			{
				//printf("No new messages!\n");
			}

			if (chat == (chat_block *)NULL)
			{
				clnt_perror(clnt, "call failed");
			}

			
		} while (my_revision < chat->total_revisions);
		

		//Read input
		while (read(0, msg.message, sizeof(msg.message)) > 0)
		{
			msg.message[strlen(msg.message) - 1] = 0;
			result_1 = write_1(&msg, clnt);
			
			printf("\nmessage sent! (%d)\n", *result_1);
			//my_revision += *result_1;

			if (result_1 == (int *)NULL)
			{
				clnt_perror(clnt, "call failed");
			}
		}
		fflush(stdin);	
		if (strcmp(msg.message, "quit") == 0) break;

		//Sleep
		sleep(1);		
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
