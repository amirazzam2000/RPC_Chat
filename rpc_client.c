/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpc.h"
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <pthread.h>
#include <curses.h>
#include <termios.h>

int done = 0;

WINDOW * top;
WINDOW * bottom;
int line = 1;	// Line position of top
int input = 1;	// Line position of top
int maxx, maxy; // Screen dimensions
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;

int my_revision = 0;

void readMessage(CLIENT *clnt)
{
	chat_block *chat;
	
	//Get chat
	do
	{
		if (NULL != chat->block)
			bzero(chat->block, 269);

		chat = getchat_1(&my_revision, clnt);
		if (chat->block[0] != 0)
			printf("%s", chat->block);

		//printf("Reading\n");
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
}

void writeMessage(message msg, CLIENT *clnt)
{
	int *result_1;
	//Read input
		while (read(0, msg.message, sizeof(msg.message)) > 0)
		{
			msg.message[strlen(msg.message) - 1] = 0;
			result_1 = write_1(&msg, clnt);

			bzero(msg.message, 269);
			//printf("\nmessage sent!\n");
			my_revision += *result_1;

			if (result_1 == (int *)NULL)
			{
				clnt_perror(clnt, "call failed");
			}
		}
		fflush(stdin);	
}



void
program_write_1(char *host)
{
	CLIENT *clnt;
	message msg;

#ifndef	DEBUG
	clnt = clnt_create(host, PROGRAM_WRITE, VERSION_WRITE, "udp");
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

	printf("Welcome %s!\n", msg.name);

	// insert ncurses code here : 
	/*
	initscr();
	getmaxyx(stdscr, maxy, maxx);

	top = newwin(maxy / 4, maxx, 0, 0);
    bottom = newwin(maxy / 4, maxx, maxy / 4, 0);

    scrollok(top, TRUE);
    scrollok(bottom, TRUE);
    box(bottom, '|', '-');
	*/
	
	while(1)
	{
		readMessage(clnt);
		writeMessage(msg, clnt);
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
