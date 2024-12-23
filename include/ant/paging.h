#ifndef _INCLUDE_PAGING_
#define _INCLUDE_PAGING_

#include <ant/types.h>

typedef struct pde p4d_t;      
typedef struct pde pud_t;      
typedef struct pde pmd_t;      
typedef struct pte pte_t;      
typedef u64 pgt_t;             
    
u64 get_pmd_idx(virt_addr_t virt_addr);

#endif
