#pragma once

#include <memory>
#include <optional>

#include <SDL3/SDL.h>

#include "conquest/data/Buffer.h"


namespace conquest::engine::sdl {

class IoStream
{
public:
    /**
        Creates a new read-only stream from a buffer.

        @param buffer - The buffer to initialize the stream from.
     */
    static IoStream fromBuffer(const Buffer& buffer);
	static IoStream fromBuffer(Buffer&& buffer);

    /**
		Returns the raw SDL_IOStream object of this stream.
     */
    [[nodiscard]] SDL_IOStream& raw() const;

    /**
		Reads a given number of bytes from the stream.

		@param size - The number of bytes to read.
     */
    [[nodiscard]] Buffer read(uint32_t size) const;

private:
	explicit IoStream(SDL_IOStream *ioStream, std::optional<Buffer> content = std::nullopt);

    std::unique_ptr<SDL_IOStream, decltype(&SDL_CloseIO)> m_IoStream;
	std::optional<Buffer> m_Content;
};

}
