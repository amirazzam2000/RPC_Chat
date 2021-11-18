/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpc.h"
#include <string.h>

int *
write_1_svc(message *argp, struct svc_req *rqstp)
{
	static int  result;
	char buffer[300];

	FILE *f;

	f = fopen("./TheChat.txt", "a");

	if (NULL != f)
	{
		printf("[%s]: %s\n", argp->name, argp->message);
		sprintf(buffer, "[%s]: %s\n", argp->name, argp->message);

		fputs(buffer , f);

		fclose(f);
	}

	return &result;
}

chat_block *
getchat_1_svc(int *argp, struct svc_req *rqstp)
{
	static chat_block  chat;

	FILE *f;

	f = fopen("./TheChat.txt", "r");

	if (NULL != f)
	{
		int p = 0;
		char aux[300];

		do {
			p = fgets(aux, 900, f);
			strcat(chat.block, aux);

		} while ( p != EOF && strlen(chat.block) < 900);
		
		printf("reading : ------ \n");
		printf("%s", chat.block);
		printf("\n------ \n");

		fclose(f);
	}

	return &chat;
}
