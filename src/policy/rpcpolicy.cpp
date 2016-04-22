#include "rpcpolicy.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using boxbot::ControlData;
using boxbot::ObservationData;

RPCPolicy::RPCPolicy(uint xdim, uint udim, std::shared_ptr<Channel> channel)
    : m_stub(boxbot::RPCPolicy::NewStub(channel)), Policy(xdim, udim)
{
}

const ControlData RPCPolicy::computeControls(const ObservationData &data)
{
    ControlData cdata;
    ClientContext context;
    Status status = m_stub->GetControls(&context, data, &cdata);
    if (!status.ok()) {
        std::cout << "RPC request failed" << std::endl;
    }
    return cdata;
}
