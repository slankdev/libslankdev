
LIBSLANKDEV := $(HOME)/git/libslankdev
CXXFLAGS += -I$(LIBSLANKDEV) -std=c++11

API = slankdev
GRPCOBJ = $(API).grpc.pb.o $(API).pb.o
GRPC_CXXFLAGS = -std=c++11 `pkg-config --cflags protobuf grpc++ grpc`
GRPC_LDFLAGS  = `pkg-config --libs protobuf grpc++ grpc` -lgrpc++_reflection -ldl

LDFLAGS += $(GRPC_LDFLAGS)

all: grpcbuild
	$(CXX) $(CXXFLAGS) -o server.out server.cc $(GRPCOBJ) $(LDFLAGS)
	$(CXX) $(CXXFLAGS) -o client.out client.cc $(GRPCOBJ) $(LDFLAGS)

grpcbuild:
	protoc -I. --cpp_out=. $(API).proto
	protoc -I. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $(API).proto
	$(CXX) $(GRPC_CXXFLAGS) -c -o $(API).grpc.pb.o $(API).grpc.pb.cc
	$(CXX) $(GRPC_CXXFLAGS) -c -o $(API).pb.o $(API).pb.cc

grpcclean:
	rm -f \
		$(API).grpc.pb.cc \
		$(API).grpc.pb.h  \
		$(API).grpc.pb.o  \
		$(API).pb.cc  \
		$(API).pb.h   \
		$(API).pb.o

dum:
	echo this is dummy
	protoc -I. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` api.proto

clean: grpcclean
	rm -f *.o *.out

