#include <kernel/elf.h>
#include <fs/vfs.h>
#include <ant/types.h>
#include <ant/align.h>
#include <mm/heap.h>
#include <libk/string.h>

static int is_elf(struct elf_64 *elf)
{
  return *((u32 *) &elf->header.ident) == 0x464c457f;
}

int elf_parse(struct elf_64 *elf)
{
  int fd = elf->fd;

  vfs_read(fd, &elf->header, sizeof(struct elf_header));
  if(!is_elf(elf))
    return -1;

  vfs_lseek(fd, elf->header.phoff, VFS_SEEK_SET);
  size_t program_header_size = elf->header.phentsize * elf->header.phnum;
  elf->program_header = heap_malloc(program_header_size);
  if(elf->program_header == NULL)
    return -1;
  vfs_read(fd, elf->program_header, program_header_size);
  
  vfs_lseek(fd, elf->header.shoff, VFS_SEEK_SET);
  size_t section_header_size = elf->header.shentsize * elf->header.shnum;
  elf->section_header = heap_malloc(section_header_size);
  if(elf->section_header == NULL)
    return -1;
  vfs_read(fd, elf->section_header, section_header_size);

  return 0;
}
extern void tlb_flush(void);
int elf_load(struct elf_64 *elf, struct table_entry *table)
{
  size_t table_size = sizeof(struct table_entry) * 512;
#include <libk/kprintf.h> 
  kprintf("phnum: %li\n", elf->header.phnum);
  size_t i;
  for(i = 0; i < elf->header.phnum; i++)
  {
    u64 vaddr = elf->program_header[i].vaddr;
    if(IS_ALIGN(vaddr, PAGE_SIZE))
      vaddr &= ~0xfffUL;
   
    if(elf->program_header[i].type != 1)
      continue;
 
    size_t size = elf->program_header[i].memsz; 
    if(!size)
      continue;
    
    size_t pages = size; 
    if(IS_ALIGN(pages, PAGE_SIZE))
      pages = ALIGNUP(pages, PAGE_SIZE);
    pages /= PAGE_SIZE;

    kprintf("vaddr: %#lx, pages: %i, size: %#lx, type: %li\n", vaddr, pages, size, elf->program_header[i].type);
    vmm_map(table, 0, (void *) vaddr, pages , USER_DATA);
    vmm_kappend_process_space(table);
    //tlb_flush();
    vfs_lseek(elf->fd, elf->program_header[i].offset, VFS_SEEK_SET);
    vfs_read(elf->fd, (void *) vaddr, size);
  }
}
