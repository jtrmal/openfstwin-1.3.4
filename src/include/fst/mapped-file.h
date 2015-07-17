
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Copyright 2005-2010 Google, Inc.
// Author: sorenj@google.com (Jeffrey Sorensen)

#ifndef FST_LIB_MAPPED_FILE_H_
#define FST_LIB_MAPPED_FILE_H_



#include <fst/fst.h>
#include <iostream>
#include <fstream>
#include <sstream>

DECLARE_CORE_int32(fst_arch_alignment);  // defined in mapped-file.h

namespace fst {

// A memory region is a simple abstraction for allocated memory or data from
// mmap'ed files.  If mmap equals NULL, then data represents an owned region of
// size bytes.  Otherwise, mmap and size refer to the mapping and data is a
// casted pointer to a region contained within [mmap, mmap + size).
// If size is 0, then mmap refers and data refer to a block of memory managed
// externally by some other allocator.
struct MemoryRegion {
  void *data;
  void *mmap;
  size_t size;
#ifdef _MSC_VER
  void *map_handle;
  void *file_handle;
#endif
};

class MappedFile {
 public:
  virtual ~MappedFile();

  void* mutable_data() const {
    return reinterpret_cast<void*>(region_.data);
  }

  const void* data() const {
    return reinterpret_cast<void*>(region_.data);
  }

  // Returns a MappedFile object that contains the contents of the input
  // stream s starting from the current file position with size bytes.
  // The file name must also be provided in the FstReadOptions as opts.source
  // or else mapping will fail.  If mapping is not possible, then a MappedFile
  // object with a new[]'ed  block of memory will be created.
  static OPENFSTDLL MappedFile* Map(istream* s, const FstReadOptions& opts, size_t size);

  // Creates a MappedFile object with a new[]'ed block of memory of size.
  // RECOMMENDED FOR INTERNAL USE ONLY, may change in future releases.
  static MappedFile* Allocate(size_t size);

  // Creates a MappedFile object pointing to a borrowed reference to data.
  // This block of memory is not owned by the MappedFile object and will not
  // be freed.
  // RECOMMENDED FOR INTERNAL USE ONLY, may change in future releases.
  static MappedFile* Borrow(void *data);

  static const int kArchAlignment;

 private:
  explicit MappedFile(const MemoryRegion &region);

  MemoryRegion region_;
  DISALLOW_COPY_AND_ASSIGN(MappedFile);
};
}  // namespace fst

#endif  // FST_LIB_MAPPED_FILE_H_
