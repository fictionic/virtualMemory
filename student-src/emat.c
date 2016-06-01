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

	double tlb_hit_time = (count_tlbhits) * MEMORY_ACCESS_TIME;
	double pagetable_hit_time = 2 * (count_reads + count_writes - count_tlbhits - count_diskaccesses) * MEMORY_ACCESS_TIME;
	double pagefault_time = (count_pagefaults) * (DISK_ACCESS_TIME + MEMORY_ACCESS_TIME);
	return (tlb_hit_time + pagetable_hit_time + pagefault_time) / (count_writes + count_reads);

}

double compute_emat_unforced() {

	double tlb_hit_time = (count_tlbhits) * MEMORY_ACCESS_TIME;
	double pagetable_hit_time = 2 * (count_reads + count_writes - count_tlbhits - count_diskaccesses) * MEMORY_ACCESS_TIME;
	double pagefault_time = (count_diskaccesses) * (DISK_ACCESS_TIME + MEMORY_ACCESS_TIME);
	return (tlb_hit_time + pagetable_hit_time + pagefault_time) / (count_writes + count_reads);

}
