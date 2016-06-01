#include "statistics.h"

#define MEMORY_ACCESS_TIME      100 /* 100 ns */
#define DISK_ACCESS_TIME   10000000 /* 10 million ns = 10 ms */

/*	Available variables from statistics that you may find useful
 *    count_pagefaults   - the number of page faults that occurred (includes required)                      
 *    count_tlbhits      - the number of tlbhits that occurred
 *    count_writes       - the number of stores/writes that occurred
 *    count_reads        - the number of reads that occurred
 * 	  disk_accesses		 - the number of disk accesses NOT including required faults
 * Any other values you might need are composites of the above values.  
 */

double compute_emat_all() {
	/* FIX ME - Compute the average memory access time, including required page faults
	 * that occur when loading a new process.
	 */

	double tlb_time = (count_tlbhits) * MEMORY_ACCESS_TIME;
	double ram_time = 2 * (count_reads + count_writes) * MEMORY_ACCESS_TIME;
	double disk_time = (count_pagefaults) * (DISK_ACCESS_TIME + MEMORY_ACCESS_TIME);
	double total = (ram_time + disk_time) / (count_writes + count_reads);
	return total;

}

double compute_emat_unforced() {
	/* FIX ME - Compute the average memory access time NOT including required faults
	*/

	double tlb_time = (count_tlbhits) * MEMORY_ACCESS_TIME;
	double ram_time = 2 * (count_reads + count_writes) * MEMORY_ACCESS_TIME;
	double disk_time = (count_diskaccesses) * (DISK_ACCESS_TIME + MEMORY_ACCESS_TIME);
	double total = (ram_time + disk_time) / (count_writes + count_reads);
	return total;

}
