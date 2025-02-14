// Copyright (c) 2023 dingodb.com, Inc. All Rights Reserved
//
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

#include "vector/vector_index_snapshot.h"

#include <sys/wait.h>  // Add this include

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "braft/file_system_adaptor.h"
#include "butil/endpoint.h"
#include "butil/iobuf.h"
#include "butil/status.h"
#include "common/failpoint.h"
#include "common/file_reader.h"
#include "common/helper.h"
#include "common/logging.h"
#include "common/service_access.h"
#include "fmt/core.h"
#include "proto/error.pb.h"
#include "proto/file_service.pb.h"
#include "proto/node.pb.h"
#include "server/file_service.h"

namespace dingodb {

namespace vector_index {

SnapshotMeta::SnapshotMeta(uint64_t vector_index_id, const std::string& path)
    : vector_index_id_(vector_index_id), path_(path) {}

SnapshotMeta::~SnapshotMeta() {
  // Delete directory
  DINGO_LOG(INFO) << "Delete vector index snapshot directory" << path_;
  Helper::RemoveAllFileOrDirectory(path_);
}

bool SnapshotMeta::Init() {
  std::filesystem::path path(path_);
  path.filename();

  uint64_t snapshot_index_id = 0;
  // int match = sscanf(path.filename().c_str(), "snapshot_%020" PRId64, &snapshot_index_id);  // NOLINT
  try {
    char* endptr;
    snapshot_index_id = strtoull(path.filename().c_str() + 9, &endptr, 10);
    if (*endptr != '\0') {
      DINGO_LOG(ERROR) << fmt::format("Parse snapshot index id failed from snapshot name, {}", path_);
      return false;
    }
  } catch (const std::exception& e) {
    DINGO_LOG(ERROR) << fmt::format("Parse snapshot index id failed from snapshot name, {}", path_);
    return false;
  }

  DINGO_LOG(INFO) << fmt::format("Load vector index snapshot meta, vector index id: {}, snapshot index id: {}",
                                 vector_index_id_, snapshot_index_id);

  snapshot_log_id_ = snapshot_index_id;

  return true;
}

std::string SnapshotMeta::MetaPath() { return fmt::format("{}/meta", path_); }

std::string SnapshotMeta::IndexDataPath() {
  return fmt::format("{}/index_{}_{}.idx", path_, vector_index_id_, snapshot_log_id_);
}

std::vector<std::string> SnapshotMeta::ListFileNames() { return Helper::TraverseDirectory(path_); }

bool SnapshotMetaSet::AddSnapshot(SnapshotMetaPtr snapshot) {
  BAIDU_SCOPED_LOCK(mutex_);

  if (snapshots_.find(snapshot->SnapshotLogId()) == snapshots_.end()) {
    // Delete stale snapshot
    snapshots_.clear();
    snapshots_[snapshot->SnapshotLogId()] = snapshot;
  } else {
    DINGO_LOG(WARNING) << fmt::format("Already exist vector index snapshot {} {}", snapshot->VectorIndexId(),
                                      snapshot->SnapshotLogId());
    return false;
  }

  return true;
}

void SnapshotMetaSet::ClearSnapshot() {
  BAIDU_SCOPED_LOCK(mutex_);

  snapshots_.clear();
}

SnapshotMetaPtr SnapshotMetaSet::GetLastSnapshot() {
  BAIDU_SCOPED_LOCK(mutex_);

  return !snapshots_.empty() ? snapshots_.rbegin()->second : nullptr;
}

std::vector<SnapshotMetaPtr> SnapshotMetaSet::GetSnapshots() {
  BAIDU_SCOPED_LOCK(mutex_);

  std::vector<SnapshotMetaPtr> result;
  result.reserve(snapshots_.size());
  for (auto& [_, snapshot] : snapshots_) {
    result.push_back(snapshot);
  }

  return result;
}

bool SnapshotMetaSet::IsExistSnapshot(uint64_t snapshot_log_id) {
  auto snapshot = GetLastSnapshot();
  if (snapshot == nullptr) {
    return false;
  }

  return snapshot_log_id <= snapshot->SnapshotLogId();
}

bool SnapshotMetaSet::IsExistLastSnapshot() { return GetLastSnapshot() != nullptr; }

}  // namespace vector_index

}  // namespace dingodb
