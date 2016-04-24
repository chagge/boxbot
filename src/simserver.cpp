#include "simserver.h"
#include <string>
#include <grpc++/grpc++.h>
#include "sim.h"
#include "agents/agent.h"
#include "agents/agentfactory.h"
#include "debugdraw.h"

#include "GL/freeglut.h"

// GRPC STUFF

using grpc::Status;
using grpc::Server;
using grpc::ServerContext;
using boxbot::ObservationData;
using boxbot::ControlData;
using boxbot::ExperimentDef;
using boxbot::SimParams;


RPCSimImpl::RPCSimImpl(Sim *sim, setup_t f1, redraw_t f2, observe_t f3)
    : m_sim(sim), m_fn_setupSim(f1), m_fn_redraw(f2), m_fn_observe(f3)
{
}


Status RPCSimImpl::init(ServerContext *context, const ExperimentDef *edef, SimParams *sp)
{
    std::cout << "received init request" << std::endl;
    m_fn_setupSim(edef, sp);
    return Status::OK;
}


Status RPCSimImpl::step(ServerContext *context, const ControlData *cdata, ObservationData *odata)
{
    // just support the first agent for now
    Agent *agent = m_sim->getAgent(0);

    // set controls and simulate
    agent->applyControls(*cdata);
    m_sim->step();

    // we can't call fn_redraw
    //m_fn_redraw();

    // fill in observation
    m_fn_observe(odata);

    return Status::OK;
}
