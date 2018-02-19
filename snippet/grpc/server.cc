
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grpc++/grpc++.h>
#include "slankdev.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using slankdev::HelloRequest;
using slankdev::HelloReply;
using slankdev::Greeter;

class APIService final : public Greeter::Service {
 public:
  virtual grpc::Status
  SayHello(ServerContext* n, const HelloRequest* req, HelloReply* rep) override
  {
    printf("slankdev\n");
    return grpc::Status::OK;
  }
};

int main(int argc, char** argv)
{
  std::string servaddr("0.0.0.0:9999");
  APIService service;

  ServerBuilder builder;
  builder.AddListeningPort(servaddr, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());

  printf("Server listening on %s\n", servaddr.c_str());
  server->Wait();
}

