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

#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "butil/status.h"
#include "faiss/MetricType.h"
#include "proto/common.pb.h"
#include "proto/error.pb.h"
#include "proto/index.pb.h"
#include "vector/vector_index_factory.h"
#include "vector/vector_index_ivf_flat.h"

namespace dingodb {

class VectorIndexIvfFlatTest : public testing::Test {
 protected:
  static void SetUpTestSuite() {}

  static void TearDownTestSuite() {
    vector_index_ivf_flat_l2.reset();
    vector_index_ivf_flat_ip.reset();
    vector_index_ivf_flat_consine.reset();
  }

  static void ReCreate() {
    static const pb::common::Range kRange;
    // valid param IP
    {
      uint64_t id = 1;
      pb::common::VectorIndexParameter index_parameter;
      index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
      index_parameter.mutable_ivf_flat_parameter()->set_dimension(dimension);
      index_parameter.mutable_ivf_flat_parameter()->set_metric_type(
          ::dingodb::pb::common::MetricType::METRIC_TYPE_INNER_PRODUCT);
      index_parameter.mutable_ivf_flat_parameter()->set_ncentroids(ncentroids);
      vector_index_ivf_flat_ip = VectorIndexFactory::New(id, index_parameter, kRange);
    }

    // valid param L2
    {
      uint64_t id = 1;
      pb::common::VectorIndexParameter index_parameter;
      index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
      index_parameter.mutable_ivf_flat_parameter()->set_dimension(dimension);
      index_parameter.mutable_ivf_flat_parameter()->set_metric_type(::dingodb::pb::common::MetricType::METRIC_TYPE_L2);
      index_parameter.mutable_ivf_flat_parameter()->set_ncentroids(ncentroids);
      vector_index_ivf_flat_l2 = VectorIndexFactory::New(id, index_parameter, kRange);
    }

    // valid param cosine
    {
      uint64_t id = 1;
      pb::common::VectorIndexParameter index_parameter;
      index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
      index_parameter.mutable_ivf_flat_parameter()->set_dimension(dimension);
      index_parameter.mutable_ivf_flat_parameter()->set_metric_type(
          ::dingodb::pb::common::MetricType::METRIC_TYPE_COSINE);
      index_parameter.mutable_ivf_flat_parameter()->set_ncentroids(ncentroids);
      vector_index_ivf_flat_consine = VectorIndexFactory::New(id, index_parameter, kRange);
    }
  }

  void SetUp() override {}

  void TearDown() override {}

  inline static std::shared_ptr<VectorIndex> vector_index_ivf_flat_l2;
  inline static std::shared_ptr<VectorIndex> vector_index_ivf_flat_ip;
  inline static std::shared_ptr<VectorIndex> vector_index_ivf_flat_consine;
  inline static faiss::idx_t dimension = 8;
  inline static int data_base_size = 100;
  inline static int32_t ncentroids = 10;
  inline static std::vector<float> data_base;
  inline static int32_t start_id = 1000;
  inline static std::string path_l2 = "./l2_ivf_flat";
  inline static std::string path_ip = "./ip_ivf_flat";
  inline static std::string path_consine = "./consine_ivf_flat";
};

TEST_F(VectorIndexIvfFlatTest, Create) {
  static const pb::common::Range kRange;
  // invalid param
  {
    uint64_t id = 1;
    pb::common::VectorIndexParameter index_parameter;
    vector_index_ivf_flat_l2 = VectorIndexFactory::New(id, index_parameter, kRange);
    EXPECT_EQ(vector_index_ivf_flat_l2.get(), nullptr);
  }

  // invalid param
  {
    uint64_t id = 1;
    pb::common::VectorIndexParameter index_parameter;
    index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_NONE);
    vector_index_ivf_flat_l2 = VectorIndexFactory::New(id, index_parameter, kRange);
    EXPECT_EQ(vector_index_ivf_flat_l2.get(), nullptr);
  }

  // invalid param
  {
    uint64_t id = 1;
    pb::common::VectorIndexParameter index_parameter;
    index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
    vector_index_ivf_flat_l2 = VectorIndexFactory::New(id, index_parameter, kRange);
    EXPECT_EQ(vector_index_ivf_flat_l2.get(), nullptr);
  }

  // invalid param
  {
    uint64_t id = 1;
    pb::common::VectorIndexParameter index_parameter;
    index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
    index_parameter.mutable_ivf_flat_parameter()->set_dimension(64);
    vector_index_ivf_flat_l2 = VectorIndexFactory::New(id, index_parameter, kRange);
    EXPECT_EQ(vector_index_ivf_flat_l2.get(), nullptr);
  }

  // invalid param
  {
    uint64_t id = 1;
    pb::common::VectorIndexParameter index_parameter;
    index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
    index_parameter.mutable_ivf_flat_parameter()->set_dimension(64);
    index_parameter.mutable_ivf_flat_parameter()->set_metric_type(::dingodb::pb::common::MetricType::METRIC_TYPE_NONE);
    vector_index_ivf_flat_l2 = VectorIndexFactory::New(id, index_parameter, kRange);
    EXPECT_EQ(vector_index_ivf_flat_l2.get(), nullptr);
  }

  // invalid param IP
  {
    uint64_t id = 1;
    pb::common::VectorIndexParameter index_parameter;
    index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
    index_parameter.mutable_ivf_flat_parameter()->set_dimension(dimension);
    index_parameter.mutable_ivf_flat_parameter()->set_metric_type(
        ::dingodb::pb::common::MetricType::METRIC_TYPE_INNER_PRODUCT);
    vector_index_ivf_flat_l2 = VectorIndexFactory::New(id, index_parameter, kRange);
    EXPECT_NE(vector_index_ivf_flat_l2.get(), nullptr);
  }

  // valid param L2  ncentroids = 0
  {
    uint64_t id = 1;
    pb::common::VectorIndexParameter index_parameter;
    index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
    index_parameter.mutable_ivf_flat_parameter()->set_dimension(dimension);
    index_parameter.mutable_ivf_flat_parameter()->set_metric_type(::dingodb::pb::common::MetricType::METRIC_TYPE_L2);
    vector_index_ivf_flat_l2 = VectorIndexFactory::New(id, index_parameter, kRange);
    EXPECT_NE(vector_index_ivf_flat_l2.get(), nullptr);
  }

  // valid param IP
  {
    uint64_t id = 1;
    pb::common::VectorIndexParameter index_parameter;
    index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
    index_parameter.mutable_ivf_flat_parameter()->set_dimension(dimension);
    index_parameter.mutable_ivf_flat_parameter()->set_metric_type(
        ::dingodb::pb::common::MetricType::METRIC_TYPE_INNER_PRODUCT);
    index_parameter.mutable_ivf_flat_parameter()->set_ncentroids(ncentroids);
    vector_index_ivf_flat_ip = VectorIndexFactory::New(id, index_parameter, kRange);
    EXPECT_NE(vector_index_ivf_flat_ip.get(), nullptr);
  }

  // valid param L2
  {
    uint64_t id = 1;
    pb::common::VectorIndexParameter index_parameter;
    index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
    index_parameter.mutable_ivf_flat_parameter()->set_dimension(dimension);
    index_parameter.mutable_ivf_flat_parameter()->set_metric_type(::dingodb::pb::common::MetricType::METRIC_TYPE_L2);
    index_parameter.mutable_ivf_flat_parameter()->set_ncentroids(ncentroids);
    vector_index_ivf_flat_l2 = VectorIndexFactory::New(id, index_parameter, kRange);
    EXPECT_NE(vector_index_ivf_flat_l2.get(), nullptr);
  }

  // valid param cosine
  {
    uint64_t id = 1;
    pb::common::VectorIndexParameter index_parameter;
    index_parameter.set_vector_index_type(::dingodb::pb::common::VectorIndexType::VECTOR_INDEX_TYPE_IVF_FLAT);
    index_parameter.mutable_ivf_flat_parameter()->set_dimension(dimension);
    index_parameter.mutable_ivf_flat_parameter()->set_metric_type(
        ::dingodb::pb::common::MetricType::METRIC_TYPE_COSINE);
    index_parameter.mutable_ivf_flat_parameter()->set_ncentroids(ncentroids);
    vector_index_ivf_flat_consine = VectorIndexFactory::New(id, index_parameter, kRange);
    EXPECT_NE(vector_index_ivf_flat_consine.get(), nullptr);
  }
}

TEST_F(VectorIndexIvfFlatTest, DeleteNoData) {
  butil::Status ok;

  // id not found
  {
    uint64_t id = 10000000;
    std::vector<uint64_t> ids;
    ids.push_back(id);
    vector_index_ivf_flat_l2->Delete(ids);
    vector_index_ivf_flat_ip->Delete(ids);
    vector_index_ivf_flat_consine->Delete(ids);
  }

  // id exist
  {
    uint64_t id = 0;
    std::vector<uint64_t> ids;
    ids.push_back(id);
    vector_index_ivf_flat_l2->Delete(ids);
    vector_index_ivf_flat_ip->Delete(ids);
    vector_index_ivf_flat_consine->Delete(ids);
  }

  // id exist batch
  {
    std::vector<uint64_t> ids;
    for (size_t i = 0; i < data_base_size; i++) {
      ids.push_back(i);
    }
    vector_index_ivf_flat_l2->Delete(ids);
    vector_index_ivf_flat_ip->Delete(ids);
    vector_index_ivf_flat_consine->Delete(ids);
  }

  // id exist batch again
  {
    std::vector<uint64_t> ids;
    for (size_t i = 0; i < data_base_size; i++) {
      ids.push_back(i);
    }
    vector_index_ivf_flat_l2->Delete(ids);
    vector_index_ivf_flat_ip->Delete(ids);
    vector_index_ivf_flat_consine->Delete(ids);
  }
}

TEST_F(VectorIndexIvfFlatTest, TrainVectorWithId) {
  butil::Status ok;

  // create random data
  {
    std::mt19937 rng;
    std::uniform_real_distribution<> distrib;

    data_base.resize(dimension * data_base_size, 0.0f);
    // float* xb = new float[dimension_ * data_base_size_];

    for (int i = 0; i < data_base_size; i++) {
      for (int j = 0; j < dimension; j++) data_base[dimension * i + j] = distrib(rng);
      data_base[dimension * i] += i / 1000.;
    }

    for (size_t i = 0; i < data_base_size; i++) {
      std::cout << "[" << i << "]"
                << " [";
      for (faiss::idx_t j = 0; j < dimension; j++) {
        if (0 != j) {
          std::cout << ",";
        }
        std::cout << std::setw(10) << data_base[i * dimension + j];
      }

      std::cout << "]" << '\n';
    }
  }

  // invalid.  no data
  {
    ok = vector_index_ivf_flat_l2->Train(std::vector<pb::common::VectorWithId>{});
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
    ok = vector_index_ivf_flat_ip->Train(std::vector<pb::common::VectorWithId>{});
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
    ok = vector_index_ivf_flat_consine->Train(std::vector<pb::common::VectorWithId>{});
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
  }

  // invalid . not align.
  {
    std::vector<float> data_base_not_align = data_base;
    data_base_not_align.resize(data_base.size() - 1);
    data_base_not_align.insert(data_base_not_align.end(), data_base.begin(), data_base.end() - 1);

    std::vector<pb::common::VectorWithId> vector_with_ids;
    for (int i = 0; i < data_base_size - 1; i++) {
      pb::common::VectorWithId vector_with_id;
      vector_with_id.mutable_vector()->mutable_float_values()->Add(
          data_base_not_align.begin() + (i * static_cast<int>(dimension)),
          data_base_not_align.begin() + ((i + 1) * static_cast<int>(dimension)));

      vector_with_ids.push_back(vector_with_id);
    }

    pb::common::VectorWithId vector_with_id;
    vector_with_id.mutable_vector()->mutable_float_values()->Add(
        data_base_not_align.begin() + ((data_base_size - 1) * static_cast<int>(dimension)),
        data_base_not_align.begin() + (data_base_size * static_cast<int>(dimension) - 1));
    vector_with_ids.push_back(vector_with_id);

    ok = vector_index_ivf_flat_l2->Train(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
    ok = vector_index_ivf_flat_ip->Train(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
    ok = vector_index_ivf_flat_consine->Train(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
  }

  // valid. data_base size < ncentroids . nlist = 1
  {
    std::vector<float> data_base_too_small = data_base;
    data_base_too_small.insert(data_base_too_small.end(), data_base.begin(), data_base.end() - dimension);

    std::vector<pb::common::VectorWithId> vector_with_ids;
    for (int i = 0; i < data_base_size - 1; i++) {
      pb::common::VectorWithId vector_with_id;
      vector_with_id.mutable_vector()->mutable_float_values()->Add(
          data_base_too_small.begin() + (i * static_cast<int>(dimension)),
          data_base_too_small.begin() + ((i + 1) * static_cast<int>(dimension)));

      vector_with_ids.push_back(vector_with_id);
    }

    ok = vector_index_ivf_flat_l2->Train(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_ip->Train(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_consine->Train(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);

    ReCreate();
  }
}

TEST_F(VectorIndexIvfFlatTest, Train) {
  butil::Status ok;

  // create random data
  {
    std::mt19937 rng;
    std::uniform_real_distribution<> distrib;

    data_base.resize(dimension * data_base_size, 0.0f);
    // float* xb = new float[dimension_ * data_base_size_];

    for (int i = 0; i < data_base_size; i++) {
      for (int j = 0; j < dimension; j++) data_base[dimension * i + j] = distrib(rng);
      data_base[dimension * i] += i / 1000.;
    }

    for (size_t i = 0; i < data_base_size; i++) {
      std::cout << "[" << i << "]"
                << " [";
      for (faiss::idx_t j = 0; j < dimension; j++) {
        if (0 != j) {
          std::cout << ",";
        }
        std::cout << std::setw(10) << data_base[i * dimension + j];
      }

      std::cout << "]" << '\n';
    }
  }

  // invalid.  no data
  {
    ok = vector_index_ivf_flat_l2->Train(std::vector<float>{});
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
    ok = vector_index_ivf_flat_ip->Train(std::vector<float>{});
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
    ok = vector_index_ivf_flat_consine->Train(std::vector<float>{});
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
  }

  // invalid . not align.
  {
    std::vector<float> data_base_not_align = data_base;
    data_base_not_align.resize(data_base.size() - 1);
    ok = vector_index_ivf_flat_l2->Train(data_base_not_align);
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
    ok = vector_index_ivf_flat_ip->Train(data_base_not_align);
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
    ok = vector_index_ivf_flat_consine->Train(data_base_not_align);
    EXPECT_EQ(ok.error_code(), pb::error::EINTERNAL);
  }

  // valid. data_base size < ncentroids . nlist = 1
  {
    std::vector<float> data_base_too_small = data_base;
    data_base_too_small.resize((ncentroids - 1) * dimension);
    ok = vector_index_ivf_flat_l2->Train(data_base_too_small);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_ip->Train(data_base_too_small);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_consine->Train(data_base_too_small);
    EXPECT_EQ(ok.error_code(), pb::error::OK);

    ReCreate();
  }

  // valid. but warning. ok
  {
    ok = vector_index_ivf_flat_l2->Train(data_base);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_ip->Train(data_base);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_consine->Train(data_base);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ReCreate();
  }

  // valid. but warning.
  {
    std::vector<float> data_base_warning = data_base;
    data_base_warning.resize(38 * (ncentroids)*dimension);
    ok = vector_index_ivf_flat_l2->Train(data_base_warning);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_ip->Train(data_base_warning);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_consine->Train(data_base_warning);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ReCreate();
  }

  // valid. but warning.
  {
    std::vector<float> data_base_warning = data_base;
    data_base_warning.resize(255 * (ncentroids)*dimension);
    ok = vector_index_ivf_flat_l2->Train(data_base_warning);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_ip->Train(data_base_warning);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_consine->Train(data_base_warning);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    // ReCreate();
  }
}

TEST_F(VectorIndexIvfFlatTest, Add) {
  butil::Status ok;
  auto internal_start_id = start_id;

  // empty OK
  {
    std::vector<pb::common::VectorWithId> vector_with_ids;
    ok = vector_index_ivf_flat_l2->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_ip->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_consine->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
  }

  // one but empty failed
  {
    std::vector<pb::common::VectorWithId> vector_with_ids;
    pb::common::VectorWithId vector_with_id;

    vector_with_ids.push_back(vector_with_id);

    ok = vector_index_ivf_flat_l2->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::EVECTOR_INVALID);
    ok = vector_index_ivf_flat_ip->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::EVECTOR_INVALID);
    ok = vector_index_ivf_flat_consine->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::EVECTOR_INVALID);
  }

  // one ok
  {
    std::vector<pb::common::VectorWithId> vector_with_ids;
    pb::common::VectorWithId vector_with_id;

    vector_with_id.set_id(internal_start_id);
    for (size_t i = 0; i < dimension; i++) {
      vector_with_id.mutable_vector()->add_float_values(data_base[i]);
    }

    vector_with_ids.push_back(vector_with_id);

    ok = vector_index_ivf_flat_l2->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_consine->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
  }

  // others
  {
    std::vector<pb::common::VectorWithId> vector_with_ids;

    for (size_t id = 1; id < data_base_size; id++) {
      pb::common::VectorWithId vector_with_id;

      vector_with_id.set_id(internal_start_id + id);
      for (size_t i = 0; i < dimension; i++) {
        vector_with_id.mutable_vector()->add_float_values(data_base[id * dimension + i]);
      }

      vector_with_ids.push_back(vector_with_id);
    }

    ok = vector_index_ivf_flat_l2->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_consine->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
  }
}

TEST_F(VectorIndexIvfFlatTest, Delete) {
  butil::Status ok;

  // id not found
  {
    uint64_t id = 10000000;
    std::vector<uint64_t> ids;
    ids.push_back(id);
    vector_index_ivf_flat_l2->Delete(ids);
    vector_index_ivf_flat_ip->Delete(ids);
    vector_index_ivf_flat_consine->Delete(ids);
  }

  // id exist
  {
    uint64_t id = start_id;
    std::vector<uint64_t> ids;
    ids.push_back(id);
    vector_index_ivf_flat_l2->Delete(ids);
    vector_index_ivf_flat_ip->Delete(ids);
    vector_index_ivf_flat_consine->Delete(ids);
  }

  // id exist batch
  {
    std::vector<uint64_t> ids;
    for (size_t i = 0; i < data_base_size; i++) {
      ids.push_back(start_id + i);
    }
    vector_index_ivf_flat_l2->Delete(ids);
    vector_index_ivf_flat_ip->Delete(ids);
    vector_index_ivf_flat_consine->Delete(ids);
  }

  // id exist batch again
  {
    std::vector<uint64_t> ids;
    for (size_t i = 0; i < data_base_size; i++) {
      ids.push_back(start_id + i);
    }
    vector_index_ivf_flat_l2->Delete(ids);
    vector_index_ivf_flat_ip->Delete(ids);
    vector_index_ivf_flat_consine->Delete(ids);
  }
}

TEST_F(VectorIndexIvfFlatTest, Upsert) {
  butil::Status ok;

  // add all data
  {
    std::vector<pb::common::VectorWithId> vector_with_ids;

    for (size_t id = 0; id < data_base_size; id++) {
      pb::common::VectorWithId vector_with_id;

      vector_with_id.set_id(start_id + id);
      for (size_t i = 0; i < dimension; i++) {
        vector_with_id.mutable_vector()->add_float_values(data_base[id * dimension + i]);
      }

      vector_with_ids.push_back(vector_with_id);
    }

    ok = vector_index_ivf_flat_l2->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_consine->Add(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
  }

  // empty OK
  {
    std::vector<pb::common::VectorWithId> vector_with_ids;
    ok = vector_index_ivf_flat_l2->Upsert(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_ip->Upsert(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_consine->Upsert(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
  }

  // update all data
  {
    std::vector<pb::common::VectorWithId> vector_with_ids;

    for (size_t id = 0; id < data_base_size; id++) {
      pb::common::VectorWithId vector_with_id;

      vector_with_id.set_id(start_id + id);
      for (size_t i = 0; i < dimension; i++) {
        vector_with_id.mutable_vector()->add_float_values(data_base[id * dimension + i]);
      }

      vector_with_ids.push_back(vector_with_id);
    }

    ok = vector_index_ivf_flat_l2->Upsert(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_ip->Upsert(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
    ok = vector_index_ivf_flat_consine->Upsert(vector_with_ids);
    EXPECT_EQ(ok.error_code(), pb::error::OK);
  }
}

TEST_F(VectorIndexIvfFlatTest, Search) {
  butil::Status ok;

  // invalid param failed, topk == 0, return OK
  {
    pb::common::VectorWithId vector_with_id;
    std::vector<pb::common::VectorWithId> vector_with_ids;
    vector_with_ids.push_back(vector_with_id);
    uint32_t topk = 0;
    std::vector<pb::index::VectorWithDistanceResult> results;
    ok = vector_index_ivf_flat_l2->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
  }

  // invalid param failed, topk == 0, return OK
  {
    pb::common::VectorWithId vector_with_id;
    vector_with_id.set_id(0);
    vector_with_id.mutable_vector()->set_dimension(dimension);
    vector_with_id.mutable_vector()->set_value_type(::dingodb::pb::common::ValueType::FLOAT);
    for (size_t i = 0; i < dimension; i++) {
      float value = data_base[i];
      vector_with_id.mutable_vector()->add_float_values(value);
    }
    uint32_t topk = 0;
    std::vector<pb::index::VectorWithDistanceResult> results;
    std::vector<pb::common::VectorWithId> vector_with_ids;
    vector_with_ids.push_back(vector_with_id);

    ok = vector_index_ivf_flat_l2->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_consine->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
  }

  // ok
  {
    pb::common::VectorWithId vector_with_id;
    vector_with_id.set_id(0);
    vector_with_id.mutable_vector()->set_dimension(dimension);
    vector_with_id.mutable_vector()->set_value_type(::dingodb::pb::common::ValueType::FLOAT);
    for (size_t i = 0; i < dimension; i++) {
      float value = data_base[i];
      vector_with_id.mutable_vector()->add_float_values(value);
    }
    uint32_t topk = 3;
    std::vector<pb::index::VectorWithDistanceResult> results_l2;
    std::vector<pb::index::VectorWithDistanceResult> results_ip;
    std::vector<pb::index::VectorWithDistanceResult> results_constine;
    std::vector<pb::common::VectorWithId> vector_with_ids;
    vector_with_ids.push_back(vector_with_id);
    ok = vector_index_ivf_flat_l2->Search(vector_with_ids, topk, {}, results_l2);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Search(vector_with_ids, topk, {}, results_ip);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_consine->Search(vector_with_ids, topk, {}, results_constine);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

    for (const auto& result : results_l2) {
      DINGO_LOG(INFO) << "L2:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }

    for (const auto& result : results_ip) {
      DINGO_LOG(INFO) << "IP:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }

    for (const auto& result : results_constine) {
      DINGO_LOG(INFO) << "COSINE:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }
  }

  // ok with param
  {
    pb::common::VectorWithId vector_with_id;
    vector_with_id.set_id(0);
    vector_with_id.mutable_vector()->set_dimension(dimension);
    vector_with_id.mutable_vector()->set_value_type(::dingodb::pb::common::ValueType::FLOAT);
    for (size_t i = 0; i < dimension; i++) {
      float value = data_base[i];
      vector_with_id.mutable_vector()->add_float_values(value);
    }
    uint32_t topk = 3;
    std::vector<pb::index::VectorWithDistanceResult> results_l2;
    std::vector<pb::index::VectorWithDistanceResult> results_ip;
    std::vector<pb::index::VectorWithDistanceResult> results_constine;
    std::vector<pb::common::VectorWithId> vector_with_ids;
    vector_with_ids.push_back(vector_with_id);

    std::vector<uint64_t> vector_ids;
    for (uint64_t i = 0; i < data_base_size; i++) {
      vector_ids.emplace_back(i + start_id);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vector_ids.begin(), vector_ids.end(), g);

    std::vector<uint64_t> vector_select_ids(vector_ids.begin(), vector_ids.begin() + (data_base_size / 2));
    std::vector<uint64_t> vector_select_ids_clone = vector_select_ids;

    std::shared_ptr<VectorIndex::IvfFlatListFilterFunctor> filter =
        std::make_shared<VectorIndex::IvfFlatListFilterFunctor>(std::move(vector_select_ids));
    const bool reconstruct = false;
    pb::common::VectorSearchParameter parameter;
    parameter.mutable_ivf_flat()->set_nprobe(10);
    ok = vector_index_ivf_flat_l2->Search(vector_with_ids, topk, {filter}, results_l2, false, parameter);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

    ok = vector_index_ivf_flat_ip->Search(vector_with_ids, topk, {filter}, results_ip, false, parameter);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

    ok = vector_index_ivf_flat_consine->Search(vector_with_ids, topk, {filter}, results_constine, false, parameter);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

    for (const auto& result : results_l2) {
      DINGO_LOG(INFO) << "L2:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }

    bool is_all_in_vector = true;
    for (const auto& result : results_l2) {
      for (const auto& distance : result.vector_with_distances()) {
        auto id = distance.vector_with_id().id();
        auto iter = std::find(vector_select_ids_clone.begin(), vector_select_ids_clone.end(), id);
        if (iter == vector_select_ids_clone.end()) {
          DINGO_LOG(INFO) << "L2 : Not Find id : " << id;
          is_all_in_vector = false;
        }
      }
    }
    if (is_all_in_vector) {
      DINGO_LOG(INFO) << "L2 : All Id in  vectors ";
    }

    for (const auto& result : results_ip) {
      DINGO_LOG(INFO) << "IP:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }

    is_all_in_vector = true;
    for (const auto& result : results_ip) {
      for (const auto& distance : result.vector_with_distances()) {
        auto id = distance.vector_with_id().id();
        auto iter = std::find(vector_select_ids_clone.begin(), vector_select_ids_clone.end(), id);
        if (iter == vector_select_ids_clone.end()) {
          DINGO_LOG(INFO) << "IP : Not Find id : " << id;
          is_all_in_vector = false;
        }
      }
    }
    if (is_all_in_vector) {
      DINGO_LOG(INFO) << "IP : All Id in vectors ";
    }

    for (const auto& result : results_constine) {
      DINGO_LOG(INFO) << "COSINE:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }

    is_all_in_vector = true;
    for (const auto& result : results_constine) {
      for (const auto& distance : result.vector_with_distances()) {
        auto id = distance.vector_with_id().id();
        auto iter = std::find(vector_select_ids_clone.begin(), vector_select_ids_clone.end(), id);
        if (iter == vector_select_ids_clone.end()) {
          DINGO_LOG(INFO) << "COSINE : Not Find id : " << id;
          is_all_in_vector = false;
        }
      }
    }
    if (is_all_in_vector) {
      DINGO_LOG(INFO) << "COSINE : All Id in vectors ";
    }
  }

  // // id exist batch
  // {
  //   std::vector<uint64_t> ids;
  //   for (size_t i = 0; i < data_base_size; i++) {
  //     ids.push_back(i + start_id);
  //   }
  //   vector_index_ivf_flat_l2->Delete(ids);
  //   vector_index_ivf_flat_ip->Delete(ids);
  //   vector_index_ivf_flat_consine->Delete(ids);
  // }
}

TEST_F(VectorIndexIvfFlatTest, NeedToRebuild) {
  bool b1 = vector_index_ivf_flat_l2->NeedToRebuild();
  bool b2 = vector_index_ivf_flat_ip->NeedToRebuild();
  bool b3 = vector_index_ivf_flat_consine->NeedToRebuild();
}

TEST_F(VectorIndexIvfFlatTest, NeedTrain) {
  EXPECT_TRUE(vector_index_ivf_flat_l2->NeedTrain());
  EXPECT_TRUE(vector_index_ivf_flat_ip->NeedTrain());
  EXPECT_TRUE(vector_index_ivf_flat_consine->NeedTrain());
}

TEST_F(VectorIndexIvfFlatTest, IsTrained) {
  EXPECT_TRUE(vector_index_ivf_flat_l2->IsTrained());
  EXPECT_TRUE(vector_index_ivf_flat_ip->IsTrained());
  EXPECT_TRUE(vector_index_ivf_flat_consine->IsTrained());
}

TEST_F(VectorIndexIvfFlatTest, Save) {
  butil::Status ok;

  ok = vector_index_ivf_flat_l2->Save("");
  EXPECT_EQ(ok.error_code(), pb::error::Errno::EILLEGAL_PARAMTETERS);

  ok = vector_index_ivf_flat_l2->Save("/var/ivf_flat");
  EXPECT_EQ(ok.error_code(), pb::error::Errno::EINTERNAL);

  ok = vector_index_ivf_flat_l2->Save(path_l2);
  EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

  ok = vector_index_ivf_flat_ip->Save(path_ip);
  EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

  ok = vector_index_ivf_flat_consine->Save(path_consine);
  EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
}

TEST_F(VectorIndexIvfFlatTest, Load) {
  butil::Status ok;

  ok = vector_index_ivf_flat_l2->Load("");
  EXPECT_EQ(ok.error_code(), pb::error::Errno::EILLEGAL_PARAMTETERS);

  ok = vector_index_ivf_flat_l2->Load("/var/ivf_flat");
  EXPECT_EQ(ok.error_code(), pb::error::Errno::EINTERNAL);

  ok = vector_index_ivf_flat_l2->Load(path_l2);
  EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

  ok = vector_index_ivf_flat_ip->Load(path_ip);
  EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

  ok = vector_index_ivf_flat_consine->Load(path_consine);
  EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
}

TEST_F(VectorIndexIvfFlatTest, SearchAfterLoad) {
  butil::Status ok;

  // invalid param failed, topk == 0, return OK
  {
    pb::common::VectorWithId vector_with_id;
    std::vector<pb::common::VectorWithId> vector_with_ids;
    vector_with_ids.push_back(vector_with_id);
    uint32_t topk = 0;
    std::vector<pb::index::VectorWithDistanceResult> results;
    ok = vector_index_ivf_flat_l2->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
  }

  // invalid param failed, topk == 0, return OK
  {
    pb::common::VectorWithId vector_with_id;
    vector_with_id.set_id(0);
    vector_with_id.mutable_vector()->set_dimension(dimension);
    vector_with_id.mutable_vector()->set_value_type(::dingodb::pb::common::ValueType::FLOAT);
    for (size_t i = 0; i < dimension; i++) {
      float value = data_base[i];
      vector_with_id.mutable_vector()->add_float_values(value);
    }
    uint32_t topk = 0;
    std::vector<pb::index::VectorWithDistanceResult> results;
    std::vector<pb::common::VectorWithId> vector_with_ids;
    vector_with_ids.push_back(vector_with_id);

    ok = vector_index_ivf_flat_l2->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_consine->Search(vector_with_ids, topk, {}, results);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
  }

  // ok
  {
    pb::common::VectorWithId vector_with_id;
    vector_with_id.set_id(0);
    vector_with_id.mutable_vector()->set_dimension(dimension);
    vector_with_id.mutable_vector()->set_value_type(::dingodb::pb::common::ValueType::FLOAT);
    for (size_t i = 0; i < dimension; i++) {
      float value = data_base[i];
      vector_with_id.mutable_vector()->add_float_values(value);
    }
    uint32_t topk = 3;
    std::vector<pb::index::VectorWithDistanceResult> results_l2;
    std::vector<pb::index::VectorWithDistanceResult> results_ip;
    std::vector<pb::index::VectorWithDistanceResult> results_constine;
    std::vector<pb::common::VectorWithId> vector_with_ids;
    vector_with_ids.push_back(vector_with_id);
    ok = vector_index_ivf_flat_l2->Search(vector_with_ids, topk, {}, results_l2);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_ip->Search(vector_with_ids, topk, {}, results_ip);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);
    ok = vector_index_ivf_flat_consine->Search(vector_with_ids, topk, {}, results_constine);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

    for (const auto& result : results_l2) {
      DINGO_LOG(INFO) << "L2:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }

    for (const auto& result : results_ip) {
      DINGO_LOG(INFO) << "IP:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }

    for (const auto& result : results_constine) {
      DINGO_LOG(INFO) << "COSINE:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }
  }

  // ok with param
  {
    pb::common::VectorWithId vector_with_id;
    vector_with_id.set_id(0);
    vector_with_id.mutable_vector()->set_dimension(dimension);
    vector_with_id.mutable_vector()->set_value_type(::dingodb::pb::common::ValueType::FLOAT);
    for (size_t i = 0; i < dimension; i++) {
      float value = data_base[i];
      vector_with_id.mutable_vector()->add_float_values(value);
    }
    uint32_t topk = 3;
    std::vector<pb::index::VectorWithDistanceResult> results_l2;
    std::vector<pb::index::VectorWithDistanceResult> results_ip;
    std::vector<pb::index::VectorWithDistanceResult> results_constine;
    std::vector<pb::common::VectorWithId> vector_with_ids;
    vector_with_ids.push_back(vector_with_id);

    std::vector<uint64_t> vector_ids;
    for (uint64_t i = 0; i < data_base_size; i++) {
      vector_ids.emplace_back(i + start_id);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vector_ids.begin(), vector_ids.end(), g);

    std::vector<uint64_t> vector_select_ids(vector_ids.begin(), vector_ids.begin() + (data_base_size / 2));
    std::vector<uint64_t> vector_select_ids_clone = vector_select_ids;

    std::shared_ptr<VectorIndex::IvfFlatListFilterFunctor> filter =
        std::make_shared<VectorIndex::IvfFlatListFilterFunctor>(std::move(vector_select_ids));
    const bool reconstruct = false;
    pb::common::VectorSearchParameter parameter;
    parameter.mutable_ivf_flat()->set_nprobe(10);
    ok = vector_index_ivf_flat_l2->Search(vector_with_ids, topk, {filter}, results_l2, false, parameter);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

    ok = vector_index_ivf_flat_ip->Search(vector_with_ids, topk, {filter}, results_ip, false, parameter);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

    ok = vector_index_ivf_flat_consine->Search(vector_with_ids, topk, {filter}, results_constine, false, parameter);
    EXPECT_EQ(ok.error_code(), pb::error::Errno::OK);

    for (const auto& result : results_l2) {
      DINGO_LOG(INFO) << "L2:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }

    bool is_all_in_vector = true;
    for (const auto& result : results_l2) {
      for (const auto& distance : result.vector_with_distances()) {
        auto id = distance.vector_with_id().id();
        auto iter = std::find(vector_select_ids_clone.begin(), vector_select_ids_clone.end(), id);
        if (iter == vector_select_ids_clone.end()) {
          DINGO_LOG(INFO) << "L2 : Not Find id : " << id;
          is_all_in_vector = false;
        }
      }
    }
    if (is_all_in_vector) {
      DINGO_LOG(INFO) << "L2 : All Id in  vectors ";
    }

    for (const auto& result : results_ip) {
      DINGO_LOG(INFO) << "IP:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }

    is_all_in_vector = true;
    for (const auto& result : results_ip) {
      for (const auto& distance : result.vector_with_distances()) {
        auto id = distance.vector_with_id().id();
        auto iter = std::find(vector_select_ids_clone.begin(), vector_select_ids_clone.end(), id);
        if (iter == vector_select_ids_clone.end()) {
          DINGO_LOG(INFO) << "IP : Not Find id : " << id;
          is_all_in_vector = false;
        }
      }
    }
    if (is_all_in_vector) {
      DINGO_LOG(INFO) << "IP : All Id in vectors ";
    }

    for (const auto& result : results_constine) {
      DINGO_LOG(INFO) << "COSINE:" << result.DebugString();
      DINGO_LOG(INFO) << "";
    }

    is_all_in_vector = true;
    for (const auto& result : results_constine) {
      for (const auto& distance : result.vector_with_distances()) {
        auto id = distance.vector_with_id().id();
        auto iter = std::find(vector_select_ids_clone.begin(), vector_select_ids_clone.end(), id);
        if (iter == vector_select_ids_clone.end()) {
          DINGO_LOG(INFO) << "COSINE : Not Find id : " << id;
          is_all_in_vector = false;
        }
      }
    }
    if (is_all_in_vector) {
      DINGO_LOG(INFO) << "COSINE : All Id in vectors ";
    }
  }
}

}  // namespace dingodb
