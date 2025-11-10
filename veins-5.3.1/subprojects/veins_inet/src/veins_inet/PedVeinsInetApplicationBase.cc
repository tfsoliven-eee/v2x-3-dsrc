/*
 * PedVeinsInetApplicationBase.cc
 *
 *  Created on: Oct 23, 2025
 *      Author: opp_env
 */




#include "veins_inet/PedVeinsInetApplicationBase.h"

#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"

namespace veins {

using namespace inet;

Define_Module(PedVeinsInetApplicationBase);

void PedVeinsInetApplicationBase::handleStartOperation(LifecycleOperation* operation)
{
    EV_INFO << "PedVeinsInetApplicationBase::handleStartOperation\n";
    mobilityPed = veins::VeinsInetPedMobilityAccess().get(getParentModule());
    traci = mobilityPed->getCommandInterface();
    
    traciPed = mobilityPed->getPersonCommandInterface();

    IInterfaceTable* ift = nullptr;
    ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);

    const char* interface = par("interface").stringValue();

    EV_INFO << "[PedVeinsInetApplicationBase::handleStartOperation] interface  = " << interface <<endl;

    
#if INET_VERSION >= 0x0403
    NetworkInterface* ie = ift->findInterfaceByName(interface);
#elif INET_VERSION >= 0x0402
    InterfaceEntry* ie = ift->findInterfaceByName(interface);
#else
    InterfaceEntry* ie = ift->getInterfaceByName(interface);
#endif    

    L3Address localAddress = L3AddressResolver().resolve(par("localAddress"));
    //L3AddressResolver().tryResolve(par("localAddress"), localAddress);
    //Ipv4Address localAddress = ie->getProtocolData<Ipv4InterfaceData>()->getIPAddress();
    socket.setOutputGate(gate("socketOut"));
    localPort = par("localPort");
    socket.bind(L3Address(), localPort);
    //socket.bind(L3Address(), localPort);
    socket.setMulticastOutputInterface(ie->getInterfaceId());
    
    //MulticastGroupList mgl = ift->collectMulticastGroups();
    //socket.joinLocalMulticastGroups(mgl);

    socket.setCallback(this);
    socket.setBroadcast(false);

    bool ok = startApplication();
    ASSERT(ok);
}

} // namespace veins

