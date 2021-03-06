/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef ART_RUNTIME_CODE_ITEM_ACCESSORS_NO_ART_INL_H_
#define ART_RUNTIME_CODE_ITEM_ACCESSORS_NO_ART_INL_H_

#include "code_item_accessors.h"

#include "cdex/compact_dex_file.h"
#include "dex_file-inl.h"
#include "standard_dex_file.h"

// The no ART version is used by binaries that don't include the whole runtime.
namespace art {

inline void CodeItemInstructionAccessor::Init(const CompactDexFile::CodeItem& code_item) {
  insns_size_in_code_units_ = code_item.insns_size_in_code_units_;
  insns_ = code_item.insns_;
}

inline void CodeItemInstructionAccessor::Init(const StandardDexFile::CodeItem& code_item) {
  insns_size_in_code_units_ = code_item.insns_size_in_code_units_;
  insns_ = code_item.insns_;
}

inline void CodeItemInstructionAccessor::Init(const DexFile* dex_file,
                                              const DexFile::CodeItem* code_item) {
  if (code_item != nullptr) {
    DCHECK(dex_file != nullptr);
    if (dex_file->IsCompactDexFile()) {
      Init(down_cast<const CompactDexFile::CodeItem&>(*code_item));
    } else {
      DCHECK(dex_file->IsStandardDexFile());
      Init(down_cast<const StandardDexFile::CodeItem&>(*code_item));
    }
  }
}

inline CodeItemInstructionAccessor::CodeItemInstructionAccessor(
    const DexFile* dex_file,
    const DexFile::CodeItem* code_item) {
  Init(dex_file, code_item);
}

inline DexInstructionIterator CodeItemInstructionAccessor::begin() const {
  return DexInstructionIterator(insns_, 0u);
}

inline DexInstructionIterator CodeItemInstructionAccessor::end() const {
  return DexInstructionIterator(insns_, insns_size_in_code_units_);
}

inline IterationRange<DexInstructionIterator> CodeItemInstructionAccessor::InstructionsFrom(
    uint32_t start_dex_pc) const {
  DCHECK_LT(start_dex_pc, InsnsSizeInCodeUnits());
  return {
      DexInstructionIterator(insns_, start_dex_pc),
      DexInstructionIterator(insns_, insns_size_in_code_units_) };
}

inline void CodeItemDataAccessor::Init(const CompactDexFile::CodeItem& code_item) {
  CodeItemInstructionAccessor::Init(code_item);
  registers_size_ = code_item.registers_size_;
  ins_size_ = code_item.ins_size_;
  outs_size_ = code_item.outs_size_;
  tries_size_ = code_item.tries_size_;
}

inline void CodeItemDataAccessor::Init(const StandardDexFile::CodeItem& code_item) {
  CodeItemInstructionAccessor::Init(code_item);
  registers_size_ = code_item.registers_size_;
  ins_size_ = code_item.ins_size_;
  outs_size_ = code_item.outs_size_;
  tries_size_ = code_item.tries_size_;
}

inline void CodeItemDataAccessor::Init(const DexFile* dex_file,
                                       const DexFile::CodeItem* code_item) {
  if (code_item != nullptr) {
    DCHECK(dex_file != nullptr);
    if (dex_file->IsCompactDexFile()) {
      CodeItemDataAccessor::Init(down_cast<const CompactDexFile::CodeItem&>(*code_item));
    } else {
      DCHECK(dex_file->IsStandardDexFile());
      CodeItemDataAccessor::Init(down_cast<const StandardDexFile::CodeItem&>(*code_item));
    }
  }
}

inline CodeItemDataAccessor::CodeItemDataAccessor(const DexFile* dex_file,
                                                  const DexFile::CodeItem* code_item) {
  Init(dex_file, code_item);
}

inline IterationRange<const DexFile::TryItem*> CodeItemDataAccessor::TryItems() const {
  const DexFile::TryItem* try_items = DexFile::GetTryItems(end(), 0u);
  return {
    try_items,
    try_items + TriesSize() };
}

inline const uint8_t* CodeItemDataAccessor::GetCatchHandlerData(size_t offset) const {
  return DexFile::GetCatchHandlerData(end(), TriesSize(), offset);
}

inline const DexFile::TryItem* CodeItemDataAccessor::FindTryItem(uint32_t try_dex_pc) const {
  IterationRange<const DexFile::TryItem*> try_items(TryItems());
  int32_t index = DexFile::FindTryItem(try_items.begin(),
                                       try_items.end() - try_items.begin(),
                                       try_dex_pc);
  return index != -1 ? &try_items.begin()[index] : nullptr;
}

inline void CodeItemDebugInfoAccessor::Init(const DexFile* dex_file,
                                            const DexFile::CodeItem* code_item,
                                            uint32_t debug_info_offset) {
  dex_file_ = dex_file;
  debug_info_offset_ = debug_info_offset;
  if (dex_file->IsCompactDexFile()) {
    Init(down_cast<const CompactDexFile::CodeItem&>(*code_item));
  } else {
    DCHECK(dex_file->IsStandardDexFile());
    Init(down_cast<const StandardDexFile::CodeItem&>(*code_item));
  }
}

inline void CodeItemDebugInfoAccessor::Init(const CompactDexFile::CodeItem& code_item) {
  CodeItemDataAccessor::Init(code_item);
}

inline void CodeItemDebugInfoAccessor::Init(const StandardDexFile::CodeItem& code_item) {
  CodeItemDataAccessor::Init(code_item);
}

template<typename NewLocalCallback>
inline bool CodeItemDebugInfoAccessor::DecodeDebugLocalInfo(bool is_static,
                                                            uint32_t method_idx,
                                                            NewLocalCallback new_local,
                                                            void* context) const {
  return dex_file_->DecodeDebugLocalInfo(RegistersSize(),
                                         InsSize(),
                                         InsnsSizeInCodeUnits(),
                                         DebugInfoOffset(),
                                         is_static,
                                         method_idx,
                                         new_local,
                                         context);
}

}  // namespace art

#endif  // ART_RUNTIME_CODE_ITEM_ACCESSORS_NO_ART_INL_H_
