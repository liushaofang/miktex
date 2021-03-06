/* winCommandLineBuilder.cpp: command-line builder

   Copyright (C) 1996-2018 Christian Schenk

   This file is part of the MiKTeX Core Library.

   The MiKTeX Core Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The MiKTeX Core Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "config.h"

#include <miktex/Core/CommandLineBuilder>

#include "internal.h"

#include "../ArgvImpl.h"

using namespace std;

using namespace MiKTeX::Core;

// mimic the behaviour of CommandLineToArgvW().
void Argv::Append(const string& arguments)
{
  MIKTEX_ASSERT(!pimpl->argv.empty());
  MIKTEX_ASSERT(pimpl->argv.back() == nullptr);
  pimpl->argv.pop_back();
  for (const char* lpsz = arguments.c_str(); *lpsz != 0; )
  {
    for (; *lpsz == ' ' || *lpsz == '\t'; ++lpsz)
    {
    }
    if (*lpsz == 0)
    {
      break;
    }
    string arg;
    bool inQuotation = false;
    for (; ; ++lpsz)
    {
      size_t nBackslashes = 0;
      for (; *lpsz == '\\'; ++lpsz)
      {
        ++nBackslashes;
      }
      bool quoteOrUnquote = false;
      if (lpsz[0] == '"')
      {
        if (nBackslashes % 2 == 0)
        {
          if (lpsz[1] == '"' && inQuotation)
          {
            ++lpsz;
          }
          else
          {
            inQuotation = !inQuotation;
            quoteOrUnquote = true;
          }
        }
        nBackslashes /= 2;
      }
      for (; nBackslashes > 0; --nBackslashes)
      {
        arg += '\\';
      }
      if (*lpsz == 0 || ((*lpsz == ' ' || *lpsz == '\t') && !inQuotation))
      {
        pimpl->argv.push_back(MIKTEX_STRDUP(arg.c_str()));
        break;
      }
      else if (!quoteOrUnquote)
      {
        arg += *lpsz;
      }
    }
  }
  pimpl->argv.push_back(nullptr);
}
