/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_COMPILER_OPTIMIZING_PC_RELATIVE_FIXUPS_X86_H_
#define ART_COMPILER_OPTIMIZING_PC_RELATIVE_FIXUPS_X86_H_

#include "nodes.h"
#include "optimization.h"

namespace art {

class CodeGenerator;

namespace x86 {

class PcRelativeFixups : public HOptimization {
 public:
  PcRelativeFixups(HGraph* graph, CodeGenerator* codegen, OptimizingCompilerStats* stats)
      : HOptimization(graph, kPcRelativeFixupsX86PassName, stats),
        codegen_(codegen) {}

  static constexpr const char* kPcRelativeFixupsX86PassName  = "pc_relative_fixups_x86";

  void Run() OVERRIDE;

 private:
  CodeGenerator* codegen_;
};

}  // namespace x86
}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_PC_RELATIVE_FIXUPS_X86_H_
