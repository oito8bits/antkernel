#include <fs/ramfs/ramdisk.h>
#include <fs/ramfs/ramfs.h>
#include <fs/devfs/devfs.h>
#include <ant/boot.h>
#include <mm/vmm.h>
#include <libk/string.h>

char *ramdisk_base;

struct vfs_ops ramdisk_ops =
{
  .fs_name = "ramdisk",
  .read = ramdisk_read,
  .write = ramdisk_write
};

struct dev ramdisk_dev;

size_t ramdisk_write(struct vfs_fd *fd, void *buffer, size_t count)
{
  memcpy(ramdisk_base + fd->offset, buffer, count);

  return count;
}

size_t ramdisk_read(struct vfs_fd *fd, void *buffer, size_t count)
{
  memcpy(buffer, ramdisk_base + fd->offset, count);

  return count;
}

void ramdisk_init(void)
{
  struct boot_info *boot_info = boot_get_info();

  phys_addr_t ramfs_phys = (phys_addr_t) boot_info->ramfs.base;
  vmm_map(0, ramfs_phys, pg_phys_to_virt(ramfs_phys), boot_info->ramfs.size / PAGE_SIZE, KERNEL_DATA);
  
  ramdisk_base =  pg_phys_to_virt(ramfs_phys);

  ramdisk_dev.ops = &ramdisk_ops;
  ramdisk_dev.size = boot_info->ramfs.size;
  devfs_register(&ramdisk_dev);
}
