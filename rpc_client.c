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
int line = 1;
int bottom_line = 2;
int maxx, maxy; 

int my_revision = 0;
CLIENT *clnt ;
message msg;
int n_chars = 0;

void readMessage() 
{
	chat_block *chat;
	do
	{
		chat = getchat_1(&my_revision, clnt);
		if (chat == (chat_block *)NULL)
		{
			printf("can't read!\n");
			clnt_perror(clnt, "call failed");
		}

		if (chat->block[0] != 0)
		{
			int n = 0;
			for (int i = 0; i < strlen(chat->block); i++)
			{
				if (chat->block[i] == '\n')
				{
					if (line != (7 * maxy / 8))
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
			
			my_revision = chat->revision_number;
		}

	} while (my_revision < chat->total_revisions);

}

void  writeMessage()
{
	int *result_1;
	int ch = 0;
	ch = mvwgetch(bottom, 2, 2 + n_chars);
	if (ch != ERR ){
		if (n_chars < 269)
			msg.message[n_chars++] = ch;
			
		if (ch == 8 || ch == '^' || ch == 127){
			mvwprintw(bottom, bottom_line, 2 + (--n_chars), " ");
			msg.message[n_chars] = 0;

			if (n_chars != 0){
				mvwprintw(bottom, bottom_line, 2 + (--n_chars), " ");
				msg.message[n_chars] = 0;
			}
		}
		else 
		{
			mvwprintw(bottom, bottom_line, 1 + (n_chars), "%c", ch );

			if (n_chars >= maxx - 4){
				bottom_line++;
				if (bottom_line >= (maxy / 8) - 1){
					scroll(bottom);
				}
			}
		}
		if(ch == '\n')
		{
			bottom_line = 2;
			if (strcmp(msg.message, "\\exit") == 0)
			{
				done = 1;
				endwin();
			}

			msg.message[strlen(msg.message) - 1] = 0;
			
			result_1 = write_1(&msg, clnt);
			if (result_1 == (int *)NULL)
			{
				printf("can't write!\n");
				clnt_perror(clnt, "call failed");
			}

			bzero(msg.message, 269);
			n_chars=0;
			bzero(msg.message, 269);
			wclear(bottom);
			box(bottom, '|', '-');
			mvwprintw(bottom, 1, 2, "[%s]  -->", msg.name);
		}
	}

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

	//ncurses code here : 
	
	initscr();
	getmaxyx(stdscr, maxy, maxx);
	noecho();

	top = newwin((7 * maxy / 8), maxx, 0, 0);
	bottom = newwin((maxy / 8), maxx, (7 * maxy / 8), 0);

    scrollok(top, TRUE);
    scrollok(bottom, TRUE);

	box(bottom, '|', '-');
	mvwprintw(bottom, 1, 2, "[%s] -->", msg.name);

	while(1){
		wrefresh(top);
		wrefresh(bottom);
		readMessage();
		writeMessage();
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
