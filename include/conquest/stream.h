#pragma once

#include <memory>
#include <vector>

#include "conquest/error.h"

namespace conquest {
	template<typename StoredT>
	class Stream
	{
	public:
		explicit Stream(std::vector<StoredT> buffer)
			: mCurrent(0)
			, mBuffer(std::move(buffer))
		{}

		constexpr bool eof() const
		{
			return mCurrent >= mBuffer.size();
		}

		constexpr const StoredT& peek() const
		{
			if(eof()) {
				throw ConquestError("Cannot peek, stream is over");
			}

			return mBuffer[mCurrent];
		}

		constexpr const StoredT& advance()
		{
			if(eof()) {
				throw ConquestError("Cannot advance, stream is over");
			}

			return mBuffer[mCurrent++];
		}

	private:
		size_t mCurrent;
		std::vector<StoredT> mBuffer;
	};
}
