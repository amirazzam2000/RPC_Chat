/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpc.h"
#include <string.h>
#include <strings.h>

int *
write_1_svc(message *argp, struct svc_req *rqstp)
{
	static int  result;
	char buffer[300];

	//bzero(buffer, 300);

	printf("message recieved!");

	FILE *f;

	f = fopen("./TheChat.txt", "a");

	if (NULL != f)
	{
		printf("[%s]: %s\n", argp->name, argp->message);
		sprintf(buffer, "[%s]: %s\n", argp->name, argp->message);

		fputs(buffer, f);

		result = strlen(buffer);

		fclose(f);
	}

	return &result;
}

chat_block *
getchat_1_svc(int *client_revision, struct svc_req *rqstp)
{
	static chat_block  chat;

	FILE *f;

	//printf("opening file here\n");
	f = fopen("./TheChat.txt", "r");
	

	if (NULL != f)
	{
		fseek(f, 0L, SEEK_END);
		
		int total_revision = ftell(f);
		chat.total_revisions = total_revision;

		fseek(f, 0L, SEEK_SET);
		
		

		if(total_revision > *client_revision){
			int reading_size = total_revision - *client_revision > 900 ?  900 : (total_revision - *client_revision);

			char *feof;
			char aux[300];

			fseek(f, *client_revision, SEEK_SET);
			do
			{
				feof = fgets(aux, reading_size, f);
				if (NULL != feof)
					strcat(chat.block, aux);

			} while (NULL != feof && strlen(chat.block) < reading_size);

			chat.revision_number = *client_revision + reading_size;

			printf("\n\nreading : ------ \n");
			printf("%s", chat.block);
			printf("\n------ \n");
			printf("revisions: total = %d, Client = %d, Client_new = %d \n\n",total_revision, *client_revision, chat.revision_number );
		}
		else{
			chat.block[0] = 0;
			chat.revision_number = *client_revision;
		}
		
		fclose(f);
	}else{
		printf("omg no\n");
		chat.block[0] = 0;
		chat.revision_number = 0;
		chat.total_revisions = 0;
	}


	return &chat;
}
