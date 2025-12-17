#ifndef _INCLUDE_ELF_
#define _INCLUDE_ELF_

#include <ant/types.h>
#include <mm/vmm.h>

#define NIDENT 16

struct elf_header
{
  unsigned char ident[NIDENT];
  u16 type;
  u16 machine;
  u32 version;
  u64 entry;
  u64 phoff;
  u64 shoff;
  u32 flags;
  u16 ehsize;
  u16 phentsize;
  u16 phnum;
  u16 shentsize;
  u16 shnum;
  u16 shstrndx;
};

struct elf_program_header
{
  u32 type;
  u32 flags;
  u64 offset;
  u64 vaddr;
  u64 paddr;
  u64 filesz;
  u64 memsz;
  u64 align;
};

struct elf_section_header
{
  u32 name;
  u32 type;
  u64 flags;
  u64 addr;
  u64 offset;
  u64 size;
  u32 link;
  u32 info;
  u64 addralign;
  u64 entsize;
};

struct elf_64
{
  struct elf_header header;
  struct elf_program_header *program_header;
  struct elf_section_header *section_header;
  char *string_table;
  int fd;
};

int elf_parse(struct elf_64 *);
int elf_load(struct elf_64 *, struct table_entry *);

#endif
