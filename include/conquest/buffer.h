#pragma once

#include <utility>
#include <memory>

#include "conquest/error.h"

namespace conquest {

	template<typename StoreT>
	class Buffer
	{
	public:
		using ValueT = StoreT;

		explicit Buffer(size_t size, const ValueT *data = nullptr)
			: mSize(size)
			, mData(nullptr)
		{
			mData = new ValueT[size]{};
			if(nullptr != data) {
				std::uninitialized_copy_n(data, mSize, mData);
			}
		}

		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		Buffer(Buffer&& other) noexcept
			: mSize(other.mSize)
			, mData(other.mData)
		{}

		Buffer& operator=(Buffer&& other) noexcept
		{
			std::exchange(mData, other.mData);
			std::exchange(mSize, other.mSize);

			return *this;
		}

		~Buffer() noexcept
		{
			try {
				if(nullptr == mData) {
					return;
				}

				delete[] mData;
			} catch(...) {}
		}

		constexpr size_t size() const
		{
			return mSize;
		}

		constexpr ValueT *data()
		{
			return mData;
		}

		constexpr const ValueT *data() const
		{
			return mData;
		}

		ValueT& operator[](size_t index)
		{
			if(index >= mSize) {
				throw std::out_of_range("Attempted to access buffer out of range");
			}

			return mData[index];
		}

		const ValueT& operator[](size_t index) const
		{
			if(index >= mSize) {
				throw std::out_of_range("Attempted to access buffer out of range");
			}

			return mData[index];
		}

	private:
		size_t mSize;
		ValueT *mData;
	};
}
