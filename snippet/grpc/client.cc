
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <grpc++/grpc++.h>
#include "slankdev.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using slankdev::HelloRequest;
using slankdev::HelloReply;
using slankdev::Greeter;

class API_Client {
 public:
  API_Client(std::shared_ptr<Channel> channel)
    : stub_(Greeter::NewStub(channel)) {}
  std::string SayHello(const std::string& user)
  {
    HelloRequest req;
    req.set_name(user);

    HelloReply rep;
    ClientContext ctx;
    Status status = stub_->SayHello(&ctx, req, &rep);
    if (!status.ok()) {
      std::cout << status.error_code() << ": "
                << status.error_message()
                << std::endl;
      return "Failed";
    }

    return rep.message();
  }
 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv)
{
  auto channel = grpc::CreateChannel( "localhost:9999", grpc::InsecureChannelCredentials());
  API_Client greeter(channel);

  std::string user("world");
  std::string reply = greeter.SayHello(user);
  printf("Greeter received: %s\n", reply.c_str());
}

