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

#ifndef DINGODB_NODE_SERVICE_H_
#define DINGODB_NODE_SERVICE_H_

#include <memory>

#include "brpc/controller.h"
#include "brpc/server.h"
#include "coordinator/coordinator_control.h"
#include "engine/engine.h"
#include "proto/node.pb.h"
#include "server/server.h"

namespace dingodb {

class NodeServiceImpl : public pb::node::NodeService {
 public:
  NodeServiceImpl() = default;

  void GetNodeInfo(google::protobuf::RpcController* controller, const pb::node::GetNodeInfoRequest* request,
                   pb::node::GetNodeInfoResponse* response, google::protobuf::Closure* done) override;
  void GetLogLevel(google::protobuf::RpcController* controller, const pb::node::GetLogLevelRequest* request,
                   pb::node::GetLogLevelResponse* response, google::protobuf::Closure* done) override;
  void ChangeLogLevel(google::protobuf::RpcController* controller, const pb::node::ChangeLogLevelRequest* request,
                      pb::node::ChangeLogLevelResponse* response, google::protobuf::Closure* done) override;
  void DingoMetrics(google::protobuf::RpcController* controller, const pb::node::MetricsRequest* request,
                    pb::node::MetricsResponse* response, google::protobuf::Closure* done) override;
  void SetFailPoint(google::protobuf::RpcController* controller, const pb::node::SetFailPointRequest* request,
                    pb::node::SetFailPointResponse* response, google::protobuf::Closure* done) override;
  void GetFailPoints(google::protobuf::RpcController* controller, const pb::node::GetFailPointRequest* request,
                     pb::node::GetFailPointResponse* response, google::protobuf::Closure* done) override;
  void DeleteFailPoints(google::protobuf::RpcController* controller, const pb::node::DeleteFailPointRequest* request,
                        pb::node::DeleteFailPointResponse* response, google::protobuf::Closure* done) override;

  void InstallVectorIndexSnapshot(google::protobuf::RpcController* controller,
                                  const pb::node::InstallVectorIndexSnapshotRequest* request,
                                  pb::node::InstallVectorIndexSnapshotResponse* response,
                                  google::protobuf::Closure* done) override;

  void GetVectorIndexSnapshot(google::protobuf::RpcController* controller,
                              const pb::node::GetVectorIndexSnapshotRequest* request,
                              pb::node::GetVectorIndexSnapshotResponse* response,
                              google::protobuf::Closure* done) override;

  void CheckVectorIndex(google::protobuf::RpcController* controller, const pb::node::CheckVectorIndexRequest* request,
                        pb::node::CheckVectorIndexResponse* response, google::protobuf::Closure* done) override;

  void SetServer(dingodb::Server* server);

 private:
  dingodb::Server* server_;
};

}  // namespace dingodb

#endif  // DINGODB_NODE_SERVICE_H_
