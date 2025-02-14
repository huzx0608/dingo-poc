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

#ifndef DINGODB_CLIENT_STORE_CLIENT_FUNCTION_H_  // NOLINT
#define DINGODB_CLIENT_STORE_CLIENT_FUNCTION_H_

#include <cstdint>
#include <memory>
#include <string>

#include "client/client_interation.h"

namespace client {

struct Context {
  std::unique_ptr<Context> Clone() const {
    auto clone_ctx = std::make_unique<Context>();

    clone_ctx->table_name = table_name;
    clone_ctx->partition_num = partition_num;
    clone_ctx->req_num = req_num;
    clone_ctx->table_id = table_id;
    clone_ctx->index_id = index_id;
    clone_ctx->region_id = region_id;

    clone_ctx->thread_num = thread_num;
    clone_ctx->thread_no = thread_no;
    clone_ctx->prefix = prefix;

    clone_ctx->dimension = dimension;
    clone_ctx->start_id = start_id;
    clone_ctx->count = count;
    clone_ctx->step_count = step_count;
    clone_ctx->with_scalar = with_scalar;
    clone_ctx->with_table = with_table;

    clone_ctx->db_path = db_path;
    clone_ctx->offset = offset;
    clone_ctx->limit = limit;

    clone_ctx->key = key;

    clone_ctx->show_vector = show_vector;

    return clone_ctx;
  }

  std::string table_name;
  int partition_num;
  int req_num;

  uint64_t table_id;
  uint64_t index_id;
  uint64_t region_id;

  int32_t thread_num;
  int32_t thread_no;
  std::string prefix;

  uint32_t dimension;
  uint32_t start_id;
  uint32_t count;
  uint32_t step_count;

  bool with_scalar;
  bool with_table;

  std::string db_path;
  int32_t offset;
  int32_t limit;

  std::string key;

  bool show_vector;
};

// meta
dingodb::pb::meta::TableDefinition SendGetIndex(uint64_t index_id);
dingodb::pb::meta::TableDefinition SendGetTable(uint64_t table_id);
dingodb::pb::meta::TableRange SendGetTableRange(uint64_t table_id);

// coordinator
dingodb::pb::common::Region SendQueryRegion(uint64_t region_id);
dingodb::pb::store::Context GetRegionContext(uint64_t region_id);

// vector
void SendVectorSearch(uint64_t region_id, uint32_t dimension, uint32_t topn);
void SendVectorSearchDebug(uint64_t region_id, uint32_t dimension, uint64_t start_vector_id, uint32_t topn,
                           uint32_t batch_count, const std::string& key, const std::string& value);
void SendVectorBatchSearch(uint64_t region_id, uint32_t dimension, uint32_t topn, uint32_t batch_count);
void SendVectorBatchQuery(uint64_t region_id, std::vector<uint64_t> vector_ids);
void SendVectorAddRetry(std::shared_ptr<Context> ctx);
void SendVectorAdd(std::shared_ptr<Context> ctx);
void SendVectorDelete(uint64_t region_id, uint32_t start_id, uint32_t count);
void SendVectorGetMaxId(uint64_t region_id);
void SendVectorGetMinId(uint64_t region_id);
void SendVectorAddBatch(uint64_t region_id, uint32_t dimension, uint32_t count, uint32_t step_count, int64_t start_id,
                        const std::string& file);
void SendVectorScanQuery(uint64_t region_id, uint64_t start_id, uint64_t end_id, uint64_t limit, bool is_reverse);
void SendVectorAddBatchDebug(uint64_t region_id, uint32_t dimension, uint32_t count, uint32_t step_count,
                             int64_t start_id, const std::string& file);
void SendVectorGetRegionMetrics(uint64_t region_id);
void SendVectorCalcDistance(uint32_t dimension, const std::string& alg_type, const std::string& metric_type,
                            int32_t left_vector_size, int32_t right_vector_size, bool is_return_normlize);

void SendVectorCount(uint64_t region_id, uint64_t start_vector_id, uint64_t end_vector_id);

// key/value
void SendKvGet(uint64_t region_id, const std::string& key, std::string& value);
void SendKvBatchGet(uint64_t region_id, const std::string& prefix, int count);
int SendKvPut(uint64_t region_id, const std::string& key, std::string value = "");
void SendKvBatchPut(uint64_t region_id, const std::string& prefix, int count);
void SendKvPutIfAbsent(uint64_t region_id, const std::string& key);
void SendKvBatchPutIfAbsent(uint64_t region_id, const std::string& prefix, int count);
void SendKvBatchDelete(uint64_t region_id, const std::string& key);
void SendKvDeleteRange(uint64_t region_id, const std::string& prefix);
void SendKvScan(uint64_t region_id, const std::string& prefix);
void SendKvCompareAndSet(uint64_t region_id, const std::string& key);
void SendKvBatchCompareAndSet(uint64_t region_id, const std::string& prefix, int count);

// region
void SendAddRegion(uint64_t region_id, const std::string& raft_group, std::vector<std::string> raft_addrs);
void SendChangeRegion(uint64_t region_id, const std::string& raft_group, std::vector<std::string> raft_addrs);
void SendDestroyRegion(uint64_t region_id);
void SendSnapshot(uint64_t region_id);
void BatchSendAddRegion(int start_region_id, int region_count, int thread_num, const std::string& raft_group,
                        std::vector<std::string>& raft_addrs);
void SendSnapshotVectorIndex(uint64_t vector_index_id);

// test
void TestBatchPut(std::shared_ptr<Context> ctx);
void TestBatchPutGet(uint64_t region_id, int thread_num, int req_num, const std::string& prefix);
void TestRegionLifecycle(uint64_t region_id, const std::string& raft_group, std::vector<std::string>& raft_addrs,
                         int region_count, int thread_num, int req_num, const std::string& prefix);
void TestDeleteRangeWhenTransferLeader(std::shared_ptr<Context> ctx, uint64_t region_id, int req_num,
                                       const std::string& prefix);
void AutoTest(std::shared_ptr<Context> ctx);

// Table
void AutoDropTable(std::shared_ptr<Context> ctx);

void CheckTableDistribution(std::shared_ptr<Context> ctx);
void CheckIndexDistribution(std::shared_ptr<Context> ctx);

}  // namespace client

#endif  // DINGODB_CLIENT_STORE_CLIENT_FUNCTION_H_