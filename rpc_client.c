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
int input = 1;	// Line position of bottom
int maxx, maxy; // Screen dimensions
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;

int my_revision = 0;
CLIENT *clnt ;
message msg;
int n_chars = 0;

void readMessage()
{
	chat_block *chat;
	
	//Get chat
	
	do
	{
		chat = getchat_1(&my_revision, clnt);
		if (chat == (chat_block *)NULL)
		{
			printf("can't read!\n");
			clnt_perror(clnt, "call failed");
		}

		//int mvwprintw(WINDOW *win, int y, int x, const char *fmt, ...);

		if (chat->block[0] != 0)
		{
			int n = 0;
			for (int i = 0; i < strlen(chat->block); i++)
			{
				if (chat->block[i] == '\n')
				{
					if (line != maxy / 2 - 2)
						line++;
					else
						scroll(top);

					n = 0;
				}
				else
				{
					mvwprintw(top, line, 2 + n, "%c", chat->block[i]);
					n++;
				}
			}

			//printf("%s", chat->block);

			//printf("Reading\n");
			my_revision = chat->revision_number;
		}

		/*
	if (my_revision >= chat->total_revisions)
	{
		printf("No new messages!\n");
	}*/

	} while (my_revision < chat->total_revisions);

	//pthread_exit(NULL);
}

void  writeMessage()
{
	int *result_1;
	//Read input

	
	
	int ch = 0;
	//err = mvwgetstr(bottom, input, 2, msg.message);
	ch = mvwgetch(bottom, input, 2 + n_chars);
	if (ch != ERR){
		msg.message[n_chars++] = ch;
		if(ch == '\n'){
			//fflush(stdin);
			if (strcmp(msg.message, "\\exit") == 0)
			{
				done = 1;
				endwin();
				//pthread_exit(NULL);
			}

			msg.message[strlen(msg.message) - 1] = 0;
			
			result_1 = write_1(&msg, clnt);
			if (result_1 == (int *)NULL)
			{
				printf("can't write!\n");
				clnt_perror(clnt, "call failed");
			}

			bzero(msg.message, 269);
			//printf("\nmessage sent!\n");
			//my_revision += *result_1;
			n_chars=0;
			bzero(msg.message, 269);
			wclear(bottom);
		}
	}

	//pthread_exit( NULL );
}



void
program_write_1(char *host)
{
	

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
	
	initscr();
	getmaxyx(stdscr, maxy, maxx);
	echo();

	//WINDOW *newwin(int nlines, int ncols, int begin_y, int begin_x);
	top = newwin((7 * maxy / 8), maxx, 0, 0);
	bottom = newwin((maxy / 8), maxx, (7 * maxy / 8), 0);


    scrollok(top, TRUE);
    scrollok(bottom, TRUE);


	box(top, '*', '=');
	box(bottom, '|', '-');

	//wsetscrreg(top, 1, (7 * maxy / 8) - 2);
	//wsetscrreg(bottom, 1, (maxy / 8) - 2);

	//pthread_t threads[2];
	// Spawn the listen/receive deamons
	//pthread_create(&threads[0], NULL, readMessage, NULL);
	//pthread_create(&threads[1], NULL, writeMessage, NULL);

	done = 1;
	while(1){
		wrefresh(top);
		wrefresh(bottom);
		readMessage();
		writeMessage();
	}
	/*while(1)
	{
		readMessage();
		writeMessage(msg);
		sleep(1);		
	}*/
	
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
