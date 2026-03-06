#include "bitsetd.hpp"

#include <stdexcept>

BitsetD::BitsetD(const int32_t size, const bool val)
  : size_(size)
  , bits_((size + 31) / 32)
{
  if (val) {
    std::fill(bits_.begin(), bits_.end(), 0xFFFFFFFFU);
  }
}

BitsetD::BitsetD(const std::uint64_t mask, const std::int32_t size) 
  : size_(size)
  , bits_{ static_cast<uint32_t>(mask ),
           static_cast<uint32_t>(mask>>32) }  {
}

bool BitsetD::get(const std::int32_t idx) const {
  if (idx < 0 || size_ <= idx) {
    throw std::out_of_range("BitsetD::get");
  }
  return bits_[idx / 32] & (UINT32_C(1) << idx % 32);
}

void BitsetD::set(const std::int32_t idx, const bool val) {
  if (idx < 0 || size_ <= idx) {
    throw std::out_of_range("BitsetD::get");
  }
  if (val) {
    bits_[idx / 32] |= UINT32_C(1) << idx % 32;
  } else {
    bits_[idx / 32] &= ~(UINT32_C(1) << idx % 32);
  }
}

BitsetD& BitsetD::invert() noexcept {
  for (auto& bits : bits_) {
    bits = ~bits;
  }
  return *this;
}

void BitsetD::fill(const bool val) noexcept {
    int full_els = size_ / 32;
    int remaining_els = size_ % 32;

    for (int i = 0; i < full_els; i += 1) {
        bits_[i] = val ? 0xFFFFFFFFU : 0U;
    }

    if (remaining_els > 0) {
        if (val) {
            bits_[full_els] = (UINT32_C(1) << remaining_els) - 1;
        } else {
            bits_[full_els] = 0U;
        }
    }
}
void BitsetD::resize(const std::int32_t new_size, const bool val) {
  const auto new_chunks = (new_size + 31)/32;
  bits_.resize(new_chunks, val ? 0xFFFFFFFFU : 0U);
  size_ = new_size;
}