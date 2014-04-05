/*
 * memps.c
 *
 *  Created on: Mar 29, 2014
 *      Author: Jon Green
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memps.h"


int main(int argc, char *argv[])
{
    FILE* fd = NULL;

    /* Fields we want to find */
    long state=0;
    unsigned int addr=0;
    pid_t pid=0;
    pid_t ppid=0;
    unsigned int next_task=0;
    char name[TASK_COMM_LEN];
    uid_t uid=0;
    uid_t gid=0;
    unsigned int mm_struct=0;
    unsigned long vsz=0;
    unsigned long rss=0;

    char state_code[3];

    if (argc != 2)
    {
            printf("usage: memps <filename>\n");
            printf("\n");
            exit(-1);
    }

    fd = fopen(argv[1],"r");

    if(NULL == fd)
      perror("Couldn't open file");

    addr = SWAPPER_PTR;		/* Set address of initial process */

   	printf("%-5s %-5s %-16s %-8s %-5s %-5s %-2s %-6s %-6s\n", \
   			"PID", "PPID", "Name", "Address", "UID", "GID", "S", "VSZ", "RSS");

    do {
    	/* State */
    	fetch(fd, (void *)&state, sizeof(state), addr);		/* State is at offset 0 */
    	get_state_code(state, state_code);

    	/* PID */
    	if (PID_OFFSET)
    		fetch(fd, (void *)&pid, sizeof(pid), addr + PID_OFFSET);

    	/* PPID */
    	if (PPID_OFFSET)
    		fetch(fd, (void *)&ppid, sizeof(ppid), addr + PPID_OFFSET); /* Pointer to PPID */
    		fetch(fd, (void *)&ppid, sizeof(ppid), ppid - MEM_OFFSET + PID_OFFSET); /* PID of parent */

    	/* UID */
    	if (UID_OFFSET)
    		fetch(fd, (void *)&uid, sizeof(uid), addr + UID_OFFSET);

    	/* GID */
    	if (GID_OFFSET)
    		fetch(fd, (void *)&gid, sizeof(gid), addr + GID_OFFSET);

    	/* Next task */
    	if (NEXT_TASK_OFFSET)
    		fetch(fd, (void *)&next_task, sizeof(next_task), addr + NEXT_TASK_OFFSET);

    	/* Process name */
    	if (NAME_OFFSET)
    		fetch(fd, (void *)&name, sizeof(name), addr + NAME_OFFSET);

    	/* Memory size */
    	if (MM_OFFSET) {
    		fetch(fd, (void *)&mm_struct, sizeof(mm_struct), addr + MM_OFFSET); /* Pointer to mm_struct */
    		if (mm_struct) {
    			// Only non-kernel processes will have an mm_struct
    			if (MM_VSZ_OFFSET)
    				fetch(fd, (void *)&vsz, sizeof(vsz), mm_struct - MEM_OFFSET + MM_VSZ_OFFSET);
    			if (MM_RSS_OFFSET)
    				fetch(fd, (void *)&rss, sizeof(rss), mm_struct - MEM_OFFSET + MM_RSS_OFFSET);
    		} else {
    			vsz = 0;
    			rss = 0;
    		}
    	}

    	/* All done fetching - now print */
     	printf("%-5u %-5u %-16s %-8.08X %-5u %-5u %-2s %-6lu %-6lu\n", \
     			pid, ppid, name, addr + MEM_OFFSET, uid, gid, state_code, vsz, rss);
      
    	/* Calculate next addr */
    	addr = next_task - MEM_OFFSET - NEXT_TASK_OFFSET;
    } while (addr != SWAPPER_PTR);

    fclose(fd);
    return 0;
}

void fetch(FILE *fd, void *field, int bytes, int offset) {
	if(0 != fseek(fd,offset,SEEK_SET)) {
    	perror("Seek error");
    	exit(-1);
    }

    if (1 != fread(field, bytes,1,fd)) {
    	perror("fread() error");
    	exit(-1);
    }
}

void get_state_code(long state, char *state_code) {
   	switch(state) {
    	case 0:
    		strncpy(state_code, "RU", 3);
    		break;
    	case 1:
      		strncpy(state_code, "IN", 3);
    		break;
    	case 2:
      		strncpy(state_code, "UN", 3);
    		break;
    	case 4:
      		strncpy(state_code, "ST", 3);
    		break;
    	case 8:
      		strncpy(state_code, "TR", 3);
    		break;
    	}
}
