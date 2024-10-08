                                                                            
   
           
                                                             
   
                                                                         
   
                  
                      
   
                                                                           
                                                      
   
                                                                            
   

#include "c.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

ssize_t
pg_pread(int fd, void *buf, size_t size, off_t offset)
{
#ifdef WIN32
  OVERLAPPED overlapped = {0};
  HANDLE handle;
  DWORD result;

  handle = (HANDLE)_get_osfhandle(fd);
  if (handle == INVALID_HANDLE_VALUE)
  {
    errno = EBADF;
    return -1;
  }

  overlapped.Offset = offset;
  if (!ReadFile(handle, buf, size, &result, &overlapped))
  {
    if (GetLastError() == ERROR_HANDLE_EOF)
    {
      return 0;
    }

    _dosmaperr(GetLastError());
    return -1;
  }

  return result;
#else
  if (lseek(fd, offset, SEEK_SET) < 0)
  {
    return -1;
  }

  return read(fd, buf, size);
#endif
}
