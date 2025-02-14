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

#ifndef DINGODB_VECTOR_INDEX_UTILS_H_  // NOLINT
#define DINGODB_VECTOR_INDEX_UTILS_H_

#include <atomic>
#include <cassert>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "butil/status.h"
#include "common/logging.h"
#include "proto/index.pb.h"

namespace dingodb {

class VectorIndexUtils {
 public:
  VectorIndexUtils() = delete;
  ~VectorIndexUtils() = delete;

  VectorIndexUtils(const VectorIndexUtils& rhs) = delete;
  VectorIndexUtils& operator=(const VectorIndexUtils& rhs) = delete;
  VectorIndexUtils(VectorIndexUtils&& rhs) = delete;
  VectorIndexUtils& operator=(VectorIndexUtils&& rhs) = delete;

  static butil::Status CalcDistanceEntry(
      const ::dingodb::pb::index::VectorCalcDistanceRequest& request,
      std::vector<std::vector<float>>& distances,                            // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_left_vectors,    // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_right_vectors);  // NOLINT

  using DoCalcDistanceFunc =
      std::function<butil::Status(const ::dingodb::pb::common::Vector&, const ::dingodb::pb::common::Vector&, bool,
                                  float&, dingodb::pb::common::Vector&, dingodb::pb::common::Vector&)>;
  static butil::Status CalcDistanceCore(
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_left_vectors,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_right_vectors,
      bool is_return_normlize,
      std::vector<std::vector<float>>& distances,                           // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_left_vectors,   // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_right_vectors,  // NOLINT
      DoCalcDistanceFunc do_calc_distance_func);                            // NOLINT

  static butil::Status CalcDistanceByFaiss(
      pb::common::MetricType metric_type,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_left_vectors,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_right_vectors,
      bool is_return_normlize,
      std::vector<std::vector<float>>& distances,                            // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_left_vectors,    // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_right_vectors);  // NOLINT

  static butil::Status CalcDistanceByHnswlib(
      pb::common::MetricType metric_type,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_left_vectors,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_right_vectors,
      bool is_return_normlize,
      std::vector<std::vector<float>>& distances,                            // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_left_vectors,    // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_right_vectors);  // NOLINT

  static butil::Status CalcL2DistanceByFaiss(
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_left_vectors,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_right_vectors,
      bool is_return_normlize,
      std::vector<std::vector<float>>& distances,                            // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_left_vectors,    // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_right_vectors);  // NOLINT

  static butil::Status CalcIpDistanceByFaiss(
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_left_vectors,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_right_vectors,
      bool is_return_normlize,
      std::vector<std::vector<float>>& distances,                            // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_left_vectors,    // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_right_vectors);  // NOLINT

  static butil::Status CalcCosineDistanceByFaiss(
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_left_vectors,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_right_vectors,
      bool is_return_normlize,
      std::vector<std::vector<float>>& distances,                            // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_left_vectors,    // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_right_vectors);  // NOLINT

  static butil::Status CalcL2DistanceByHnswlib(
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_left_vectors,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_right_vectors,
      bool is_return_normlize,
      std::vector<std::vector<float>>& distances,                            // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_left_vectors,    // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_right_vectors);  // NOLINT

  static butil::Status CalcIpDistanceByHnswlib(
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_left_vectors,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_right_vectors,
      bool is_return_normlize,
      std::vector<std::vector<float>>& distances,                            // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_left_vectors,    // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_right_vectors);  // NOLINT

  static butil::Status CalcCosineDistanceByHnswlib(
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_left_vectors,
      const google::protobuf::RepeatedPtrField<::dingodb::pb::common::Vector>& op_right_vectors,
      bool is_return_normlize,
      std::vector<std::vector<float>>& distances,                            // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_left_vectors,    // NOLINT
      std::vector<::dingodb::pb::common::Vector>& result_op_right_vectors);  // NOLINT

  // internal api

  static butil::Status DoCalcL2DistanceByFaiss(const ::dingodb::pb::common::Vector& op_left_vectors,
                                               const ::dingodb::pb::common::Vector& op_right_vectors,
                                               bool is_return_normlize,
                                               float& distance,                                        // NOLINT
                                               dingodb::pb::common::Vector& result_op_left_vectors,    // NOLINT
                                               dingodb::pb::common::Vector& result_op_right_vectors);  // NOLINT

  static butil::Status DoCalcIpDistanceByFaiss(const ::dingodb::pb::common::Vector& op_left_vectors,
                                               const ::dingodb::pb::common::Vector& op_right_vectors,
                                               bool is_return_normlize,
                                               float& distance,                                        // NOLINT
                                               dingodb::pb::common::Vector& result_op_left_vectors,    // NOLINT
                                               dingodb::pb::common::Vector& result_op_right_vectors);  // NOLINT
  static butil::Status DoCalcCosineDistanceByFaiss(const ::dingodb::pb::common::Vector& op_left_vectors,
                                                   const ::dingodb::pb::common::Vector& op_right_vectors,
                                                   bool is_return_normlize,
                                                   float& distance,                                        // NOLINT
                                                   dingodb::pb::common::Vector& result_op_left_vectors,    // NOLINT
                                                   dingodb::pb::common::Vector& result_op_right_vectors);  // NOLINT

  static butil::Status DoCalcL2DistanceByHnswlib(const ::dingodb::pb::common::Vector& op_left_vectors,
                                                 const ::dingodb::pb::common::Vector& op_right_vectors,
                                                 bool is_return_normlize,
                                                 float& distance,                                        // NOLINT
                                                 dingodb::pb::common::Vector& result_op_left_vectors,    // NOLINT
                                                 dingodb::pb::common::Vector& result_op_right_vectors);  // NOLINT

  static butil::Status DoCalcIpDistanceByHnswlib(const ::dingodb::pb::common::Vector& op_left_vectors,
                                                 const ::dingodb::pb::common::Vector& op_right_vectors,
                                                 bool is_return_normlize,
                                                 float& distance,                                        // NOLINT
                                                 dingodb::pb::common::Vector& result_op_left_vectors,    // NOLINT
                                                 dingodb::pb::common::Vector& result_op_right_vectors);  // NOLINT

  static butil::Status DoCalcCosineDistanceByHnswlib(const ::dingodb::pb::common::Vector& op_left_vectors,
                                                     const ::dingodb::pb::common::Vector& op_right_vectors,
                                                     bool is_return_normlize,
                                                     float& distance,                                        // NOLINT
                                                     dingodb::pb::common::Vector& result_op_left_vectors,    // NOLINT
                                                     dingodb::pb::common::Vector& result_op_right_vectors);  // NOLINT

  static void ResultOpVectorAssignment(dingodb::pb::common::Vector& result_op_vectors,  // NOLINT
                                       const ::dingodb::pb::common::Vector& op_vectors);

  static void ResultOpVectorAssignmentWrapper(const ::dingodb::pb::common::Vector& op_left_vectors,
                                              const ::dingodb::pb::common::Vector& op_right_vectors,
                                              bool is_return_normlize,
                                              dingodb::pb::common::Vector& result_op_left_vectors,    // NOLINT
                                              dingodb::pb::common::Vector& result_op_right_vectors);  // NOLINT

  static void NormalizeVectorForFaiss(float* x, int32_t d);
  static void NormalizeVectorForHnsw(const float* data, uint32_t dimension, float* norm_array);
};

}  // namespace dingodb

#endif  // DINGODB_VECTOR_INDEX_UTILS_H_
