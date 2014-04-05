/*
 * memps.h
 *
 *  Created on: Mar 29, 2014
 *      Author: Jon Green
 */

#ifndef MEMPS_H_
#define MEMPS_H_

#define SWAPPER_PTR 0x003ed3a0		/* Physical address where init_task is found */
#define MEM_OFFSET 0xc0000000		/* Offset from kernel addr to physical addr */

#define NEXT_TASK_OFFSET 0x8c		/* Position of next task */
#define NAME_OFFSET 0x1cd      		/* Position of name */
#define PID_OFFSET 0xc8				/* PID */
#define PPID_OFFSET 0xd0			/* Pointer to parent PID */
#define UID_OFFSET 0x18c			/* User ID */
#define GID_OFFSET 0x19c			/* Group ID */
#define MM_OFFSET 0xa4				/* Memory manager pointer */
#define MM_VSZ_OFFSET 0x60			/* Virtual memory size offset within mm_struct */
#define MM_RSS_OFFSET 0x58			/* RSS offset within mm_struct */

#define TASK_COMM_LEN 16       		/* Length of process name, from sched.h on target system */

void fetch(FILE *fd, void *field, int bytes, int offset);
void get_state_code(long, char *);

#endif /* MEMPS_H_ */
