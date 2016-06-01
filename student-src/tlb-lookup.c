#include <stdlib.h>
#include <stdio.h>
#include "tlb.h"
#include "pagetable.h"
#include "global.h" /* for tlb_size */
#include "statistics.h"

int recent_tlbe_index;

/*******************************************************************************
 * Looks up an address in the TLB. If no entry is found, calls pagetable_lookup()
 * to get the entry from the page table instead
 *
 * @param vpn The virtual page number to lookup.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical frame number of the page we are accessing.
 */
pfn_t tlb_lookup(vpn_t vpn, int write) {
	pfn_t pfn;

	/* 
	 * FIX ME : Step 5
	 * Note that tlb is an array with memory already allocated and initialized to 0/null
	 * meaning that you don't need special cases for a not-full tlb, the valid field
	 * will be 0 for both invalid and empty tlb entries, so you can just check that!
	 */

	/* 
	 * Search the TLB - hit if find valid entry with given VPN 
	 * Increment count_tlbhits on hit. 
	 */
	int i;
	int hit = 0;
	tlbe_t *tlbe;
	for(i=0; i<tlb_size; i++) {
		tlbe = &tlb[i];
		if(tlbe->vpn == vpn && tlbe->valid == 1) {
			recent_tlbe_index = i;
			pfn = tlbe->pfn;
			count_tlbhits++;
			hit = 1;
			break;
		}
	}

	/* 
	 * If it was a miss, call the page table lookup to get the pfn
	 * Add current page as TLB entry. Replace any invalid entry first, 
	 * then do a clock-sweep to find a victim (entry to be replaced).
	 */
	if(!hit) {
		pfn = pagetable_lookup(vpn, write);
		/* find a pte to evict */
		/* first check for invalid entries */
		int found_invalid = 0;
		for(i=0; i<tlb_size; i++) {
			tlbe = &tlb[i];
			if(tlbe->valid == 0) {
				found_invalid = 1;
				break;
			}
		}
		/* do clock algorithm */
		if(!found_invalid) {
			int flag = 0;
			for(i=recent_tlbe_index; i != recent_tlbe_index || flag == 0; i++) {
				if(i == tlb_size)
					i = 0;
				tlbe = &tlb[i];
				if(tlbe->used == 1) {
					tlbe->used = 0;
				} else {
					break;
				}
				flag = 1;
			}
		}
		/* evict the victim entry */
		tlbe->valid = 1;
		tlbe->vpn = vpn;
		tlbe->pfn = pfn;
		/* update the recent index */
		recent_tlbe_index = i;
	}

	/*
	 * In all cases perform TLB house keeping. This means marking the found TLB entry as
	 * used and if we had a write, dirty. We also need to update the page
	 * table entry in memory with the same data.
	 */
	tlbe->used = 1;
	if(write) {
		tlbe->dirty = 1;
		current_pagetable[vpn].dirty = 1;
	}

	printf("=====================\n");
	printf("vpn | pfn | v | d | u\n");
	for(i=0; i<tlb_size; i++) {
		printf(" %x  |  %x  | %d | %d | %d\n", tlb[i].vpn, tlb[i].pfn, tlb[i].valid, tlb[i].dirty, tlb[i].used);
	}
	printf("====================\n");

	return pfn;
}

