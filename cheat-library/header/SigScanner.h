#ifndef SIGSCANNER_H
#define SIGSCANNER_H
#include "../header/Signature.h"
#include "../header/Include.h"

std::uint8_t* Scan(const char* signature) noexcept;

const auto get = [](const char* signature) noexcept -> std::uintptr_t
{
	return reinterpret_cast<std::uintptr_t>(Scan(signature));
};

template<typename T = std::uintptr_t>
constexpr T GetOffset(std::uintptr_t address, int offset);

std::vector<uintptr_t> GetOffsets(std::vector<sig> signatures);

#endif // SIGSCANNER_H
