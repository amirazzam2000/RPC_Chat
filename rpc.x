
struct message { 
    char message[269];
    char name[10];
};

struct chat_block{
    char block[900];
    int revision_number;
    int total_revisions;
};

program PROGRAM_WRITE { 
version VERSION_WRITE { 
   int write (message) = 0; 
   chat_block getChat(int) = 1;
} = 1; 
} = 0x20000001;