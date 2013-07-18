// Copyright (C) 2010-2013 Joshua Boyce.
// See the file COPYING for copying permission.

#pragma once

#include <windows.h>

#include <hadesmem/error.hpp>
#include <hadesmem/process.hpp>
#include <hadesmem/detail/smart_handle.hpp>

namespace hadesmem
{

namespace detail
{

inline detail::SmartHandle CreateRemoteThreadAndWait(Process const& process, 
  LPTHREAD_START_ROUTINE func, DWORD timeout = INFINITE)
{
  detail::SmartHandle remote_thread(::CreateRemoteThread(process.GetHandle(), 
    nullptr, 0, func, nullptr, 0, nullptr));
  if (!remote_thread.GetHandle())
  {
    DWORD const last_error = ::GetLastError();
    HADESMEM_THROW_EXCEPTION(Error() << 
      ErrorString("CreateRemoteThread failed.") << 
      ErrorCodeWinLast(last_error));
  }

  DWORD const wait_res = ::WaitForSingleObject(remote_thread.GetHandle(), 
    timeout);
  if (wait_res != WAIT_OBJECT_0)
  {
    if (wait_res == WAIT_TIMEOUT)
    {
      HADESMEM_THROW_EXCEPTION(Error() << 
        ErrorString("WaitForSingleObject timeout."));
    }

    DWORD const last_error = ::GetLastError();
    HADESMEM_THROW_EXCEPTION(Error() << 
      ErrorString("WaitForSingleObject failed.") << 
      ErrorCodeWinLast(last_error));
  }

  return remote_thread;
}

}

}
