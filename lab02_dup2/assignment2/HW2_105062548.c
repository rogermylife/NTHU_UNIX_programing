#include "hw2.h"
#include <unistd.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/posix_types.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <errno.h>
#include <stdio.h>
/*
bool get_close_on_exec(unsigned int fd)
{
        struct files_struct *files = current->files;
        struct fdtable *fdt;
        bool res;
        rcu_read_lock();
        fdt = files_fdtable(files);
        res = close_on_exec(fd, fdt);
        rcu_read_unlock();
        return res;
}
*/
static int dupfd (int fd, int desired_fd)
{
  int duplicated_fd = dup (fd);
  printf("duplicate fd %d\n",duplicated_fd);
  if (duplicated_fd < 0 || duplicated_fd == desired_fd)
    return duplicated_fd;
  else
    {
      int r = dupfd (fd, desired_fd);
      int e = errno;
      close (duplicated_fd);
      errno = e;
      return r;
    }
}

int isValid(int fd)
{
    //int err = get_close_on_exec(fd);
    return 1;
}
int mydup2(int oldfd, int newfd){
    //Your implementation here
    int result = isValid(oldfd);
    if(!result || oldfd==newfd)
        return -1;
    close (newfd);
    result = dupfd (oldfd, newfd);
    if (result == -1 && (errno == EMFILE || errno == EINVAL))
        errno = EBADF;
    return result;
    
}
