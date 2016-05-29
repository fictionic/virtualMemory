#include "swapfile.h"
#include "statistics.h"
#include "pagetable.h"

/*******************************************************************************
 * Looks up an address in the current page table. If the entry for the given
 * page is not valid, traps to the OS.
 *
 * @param vpn The virtual page number to lookup.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical frame number of the page we are accessing.
 */
pfn_t pagetable_lookup(vpn_t vpn, int write) {

	pte_t pte = current_pagetable[vpn];

	/* check if it's valid */
	if(!pte.valid) {
		count_pagefaults++;
		pte.pfn = pagefault_handler(vpn, write);
		pte.valid = (char)1;
	} 
	return pte.pfn;
}
