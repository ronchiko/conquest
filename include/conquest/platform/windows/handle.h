#pragma once

#include <utility>

#include "conquest/platform/windows.h"

namespace conquest::win32 {

	class Handle
	{
	public:
		constexpr Handle()
			: Handle(INVALID_HANDLE_VALUE)
		{}

		constexpr Handle(HANDLE handle)
			: mHandle(handle)
		{}

		Handle(const Handle&) = delete;
		Handle& operator=(const Handle&) = delete;

		constexpr Handle(Handle&& other) noexcept
			: mHandle(other.mHandle)
		{
			other.mHandle = INVALID_HANDLE_VALUE;
		}

		inline Handle& operator=(Handle&& other) noexcept
		{
			std::exchange(mHandle, other.mHandle);
			return *this;
		}

		inline ~Handle() noexcept
		{
			try {
				if(INVALID_HANDLE_VALUE == mHandle || nullptr == mHandle) {
					return;
				}

				CloseHandle(mHandle);
			} catch(...) {}
		}

		constexpr operator HANDLE() const
		{
			return mHandle;
		}

	private:
		HANDLE mHandle;
	};
}