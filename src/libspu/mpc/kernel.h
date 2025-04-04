// Copyright 2021 Ant Group Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "libspu/core/context.h"

namespace spu::mpc {

class RandKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx, const Shape& shape) const = 0;
};

class UnaryKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx,
                          const NdArrayRef& in) const = 0;
};

class RevealToKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          size_t rank) const = 0;
};

class ShiftKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          const Sizes& bits) const = 0;
};

class BinaryKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& lhs,
                          const NdArrayRef& rhs) const = 0;
};

class MatmulKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& a,
                          const NdArrayRef& b) const = 0;
};

class Conv2DKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  // tensor: NxHxWxC
  // filter: hxwxCxO
  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& tensor,
                          const NdArrayRef& filter, int64_t stride_h,
                          int64_t stride_w) const = 0;
};

class BitrevKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          size_t start, size_t end) const = 0;
};

enum class TruncLsbRounding {
  // For protocols like SecureML/ABY3, the LSB is random.
  Random,
  // For DEK19/EGK+20, the LSB is probabilistic, More precisely, for
  //    y = x/2` + u, where u ∈ [0, 1).
  // The result has probability of u to be x/2`+1, and probability 1-u to be
  // x/2`.
  Probabilistic,
  // For some deterministic truncation, the LSB is deterministic.
  Nearest,
};

class TruncAKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  // For protocol like SecureML, the most significant bit may have error with
  // low probability, which lead to huge calculation error.
  //
  // Return true if the protocol has this kind of error.
  virtual bool hasMsbError() const = 0;

  virtual TruncLsbRounding lsbRounding() const = 0;

  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          size_t bits, SignType sign) const = 0;
};

class BitSplitKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          size_t stride) const = 0;
};

class CastTypeKernel : public Kernel {
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          const Type& to_type) const = 0;
};

class PermKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          const NdArrayRef& perm) const = 0;
};

class OramOneHotKernel : public Kernel {
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          int64_t s) const = 0;
};

class OramReadKernel : public Kernel {
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& onehot,
                          const NdArrayRef& db, int64_t offset) const = 0;
};

class GenInvPermKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          bool is_ascending) const = 0;
};

class MergeKeysKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx,
                          absl::Span<NdArrayRef const> inputs,
                          bool is_ascending) const = 0;
};

class BroadcastKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          const Shape& to_shape, const Axes& in_dims) const = 0;
};

class DimsBasedKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          const Axes& perm) const = 0;
};

class ShapeBasedKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          const Shape& to_shape) const = 0;
};

class ExtractSliceKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;
  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          const Index& start, const Index& end,
                          const Strides& strides) const = 0;
};

class UpdateSliceKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          const NdArrayRef& update,
                          const Index& start) const = 0;
};

class PadKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx, const NdArrayRef& in,
                          const NdArrayRef& padding_value,
                          const Sizes& edge_padding_low,
                          const Sizes& edge_padding_high,
                          const Sizes& interior_padding) const = 0;
};

class ConcateKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx,
                          const std::vector<NdArrayRef>& values,
                          int64_t axis) const = 0;
};

class DisassembleKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  virtual std::vector<NdArrayRef> proc(KernelEvalContext* ctx,
                                       const NdArrayRef& in) const = 0;
};

class MultiKeyLowMcKernel : public Kernel {
 public:
  void evaluate(KernelEvalContext* ctx) const override;

  virtual NdArrayRef proc(KernelEvalContext* ctx,
                          const std::vector<NdArrayRef>& inputs) const = 0;
};

}  // namespace spu::mpc
