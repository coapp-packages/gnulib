/* Test the pread function.
   Copyright (C) 2009 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Jim Meyering.  */

#include <config.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define ASSERT(expr) \
  do                                                                         \
    {                                                                        \
      if (!(expr))                                                           \
        {                                                                    \
          fprintf (stderr, "%s:%d: assertion failed\n", __FILE__, __LINE__); \
          fflush (stderr);                                                   \
          abort ();                                                          \
        }                                                                    \
    }                                                                        \
  while (0)

#define N (sizeof buf - 1)

int
main (void)
{
  char const *file = "in";
  int fd;
  char buf[] = "0123456789";
  off_t pos;

  ASSERT (file);

  fd = open (file, O_CREAT | O_WRONLY, 0600);
  ASSERT (0 <= fd);
  ASSERT (write (fd, buf, N) == N);
  ASSERT (close (fd) == 0);

  fd = open (file, O_RDONLY);
  ASSERT (0 <= fd);

  for (pos = 0; pos < 3; pos++)
    {
      size_t i;
      off_t init_pos = lseek (fd, pos, SEEK_SET);
      ASSERT (init_pos == pos);

      for (i = 0; i < N; i++)
        {
          char byte_buf;
          ASSERT (pread (fd, &byte_buf, 1, i) == 1);
          ASSERT (byte_buf == buf[i]);
          ASSERT (lseek (fd, 0, SEEK_CUR) == init_pos);
        }
    }

  {
    /* Invalid offset must evoke failure with EINVAL.  */
    char byte;
    ASSERT (pread (fd, &byte, 1, (off_t) -1) == -1);
    ASSERT (errno == EINVAL);
  }

  ASSERT (close (fd) == 0);

  {
    char byte;
    /* Trying to operate on a pipe must evoke failure with ESPIPE.
       This assumes that stdin is a pipe, and hence not seekable.  */
    ASSERT (pread (STDIN_FILENO, &byte, 1, 1) == -1);
    ASSERT (errno == ESPIPE);
  }

  return 0;
}