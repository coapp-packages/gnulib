/* Tests of utimens.
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

/* Written by Eric Blake <ebb9@byu.net>, 2009.  */

#include <config.h>

#include "utimens.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(expr) \
  do                                                                         \
    {                                                                        \
      if (!(expr))                                                           \
        {                                                                    \
          fprintf (stderr, "%s:%d: assertion failed\n", __FILE__, __LINE__);  \
          fflush (stderr);                                                   \
          abort ();                                                          \
        }                                                                    \
    }                                                                        \
  while (0)

#define BASE "test-utimens.t"

#include "test-futimens.h"
#include "test-lutimens.h"
#include "test-utimens.h"

/* Wrap gl_futimens to behave like futimens.  */
static int
do_futimens (int fd, struct timespec const times[2])
{
  return gl_futimens (fd, NULL, times);
}

/* Test the use of file descriptors alongside a name.  */
static int
do_fdutimens (char const *name, struct timespec const times[2])
{
  int result;
  int fd = open (name, O_WRONLY);
  if (fd < 0)
    fd = open (name, O_RDONLY);
  errno = 0;
  result = fdutimens (name, fd, times);
  if (0 <= fd)
    {
      int saved_errno = errno;
      close (fd);
      errno = saved_errno;
    }
  return result;
}

int
main ()
{
  int result1;
  int result2;

  /* Clean up any trash from prior testsuite runs.  */
  ASSERT (system ("rm -rf " BASE "*") == 0);

  ASSERT (test_utimens (utimens) == 0);
  ASSERT (test_utimens (do_fdutimens) == 0);
  result1 = test_futimens (do_futimens, true);
  if (result1)
    ASSERT (result1 == 77);
  /* Print only one skip message.  */
  result2 = test_lutimens (lutimens, result1 == 0);
  if (result2)
    ASSERT (result2 == 77);
  return result1 | result2;
}