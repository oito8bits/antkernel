#include <fs/ramfs/ramfs.h>
#include <fs/vfs.h>
#include <ant/boot.h>
#include <mm/vmm.h>

struct ramfs_tar_header *header;

struct vfs_ops tar_ops =
{
  .fs_name = "ramfs",
  .open = ramfs_open,
  .close = ramfs_close,
  .read = ramfs_read,
  .write = ramfs_write,
  .mount = ramfs_mount,
  .umount = ramfs_umount
};

struct vfs_ops *ramfs_get_ops(void)
{
  return &tar_ops;
}

struct vfs_fd *ramfs_open(const char *path, int flags)
{

}

size_t ramfs_write(struct vfs_fd *fd, void *buffer, size_t count)
{
}

size_t ramfs_read(struct vfs_fd *fd, void *buffer, size_t count)
{
}

int ramfs_close(struct vfs_fd *fd)
{
}

int ramfs_mount(struct mountpoint *mp)
{
}

int ramfs_umount(struct mountpoint *mp)
{
}

void ramfs_init(void)
{
  struct boot_info *boot_info = boot_get_info();
  
  phys_addr_t ramfs_phys = (phys_addr_t) boot_info->ramfs.base;
  vmm_kmap_pdata(ramfs_phys, pg_phys_to_virt(ramfs_phys), boot_info->ramfs.size / PAGE_SIZE);

  header = (struct ramfs_tar_header *) pg_phys_to_virt(ramfs_phys);
}
