#include "conquest/engine/sdl/SdlError.h"

#include "conquest/engine/sdl/stream/IoStream.h"


namespace conquest::engine::sdl {

IoStream IoStream::fromBuffer(const Buffer &buffer)
{
	auto* stream = SDL_IOFromConstMem(buffer.address(), buffer.length());
	if(nullptr == stream) {
		raiseSdlError("Failed to create SDL_IOStream from buffer via SDL_IOFromConstMem");
	}

	return IoStream(stream, std::nullopt);
}

IoStream IoStream::fromBuffer(Buffer&& buffer)
{
	auto *stream = SDL_IOFromConstMem(buffer.address(), buffer.length());
	if(nullptr == stream) {
		raiseSdlError("Failed to create SDL_IOStream from buffer via SDL_IOFromConstMem");
	}

	return IoStream(stream, std::move(buffer));
}

SDL_IOStream & IoStream::raw() const
{
	return *m_IoStream;
}

Buffer IoStream::read(const uint32_t size) const
{
	auto response = Buffer{size};
	if(0 == SDL_ReadIO(&raw(), response.address(), response.length())) {
		raiseSdlError("Failed to read from SDL_IOStream");
	}

	return response;
}

IoStream::IoStream(SDL_IOStream *ioStream, std::optional<Buffer> content)
	: m_IoStream(ioStream, &SDL_CloseIO)
	, m_Content(std::move(content))
{}

}
