/* Test acquiring ownership of the slave side of a pseudo-terminal.
   Copyright (C) 2011 Free Software Foundation, Inc.

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

#include <config.h>

#include <stdlib.h>

#include "signature.h"
SIGNATURE_CHECK (grantpt, int, (int));

#include <errno.h>

#include "macros.h"

int
main (void)
{
  /* Test behaviour for invalid file descriptors.  */
  {
    errno = 0;
    ASSERT (grantpt (-1) == -1);
    ASSERT (errno == EBADF
            || errno == EINVAL /* seen on FreeBSD 6.4 */
            || errno == 0 /* seen on Solaris 8 */
           );
  }
  {
    errno = 0;
    ASSERT (grantpt (99) == -1);
    ASSERT (errno == EBADF
            || errno == EINVAL /* seen on FreeBSD 6.4 */
            || errno == 0 /* seen on Solaris 8 */
           );
  }

  return 0;
}
