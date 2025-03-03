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

#include <string>

#include "mlir/IR/OwningOpRef.h"

#include "libspu/spu.h"

namespace mlir {

class PassManager;
class ModuleOp;

} // namespace mlir

namespace spu::compiler {

class CompilationContext;

class FE final {
public:
  explicit FE(CompilationContext *ctx);

  mlir::OwningOpRef<mlir::ModuleOp> doit(const CompilationSource &source);

private:
  void buildFrontEndPipeline(mlir::PassManager *pm, const std::string &args);

  CompilationContext *ctx_;
};

} // namespace spu::compiler
