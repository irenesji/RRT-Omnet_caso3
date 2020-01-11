//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Sink.h"

namespace myexample {

Define_Module(Sink);

void Sink::initialize()
{
    lastArrival = simTime();
    iaTimeHistogram.setName("interarrival times");
    arrivalsVector.setName("arrivals");
    arrivalsVector.setInterpolationMode(cOutVector::NONE);
}

void Sink::handleMessage(cMessage *msg)
{
    simtime_t d = simTime() - lastArrival;
    cPacket *pack = check_and_cast<cPacket *>(msg);
    cPacket *response;


    EV << "Received " << msg->getName() << msg->par("seqNum").longValue() << endl;

    if(pack->hasBitError()){
        response = new cPacket("NACK",1);
    }
    else{
        response = new cPacket("ACK",1);
    }
    //Se asocia el número de secuencia del paquete al ACK o NACK
    response->addPar("seqNum");
    response->par("seqNum").setLongValue(msg->par("seqNum").longValue());
    //send(response, "in$o");
    sendDelayed(response, 0.0002, "in$o");
    //delete msg;

    iaTimeHistogram.collect(d);
    arrivalsVector.record(1);

    lastArrival = simTime();
}

void Sink::finish()
{
    recordStatistic(&iaTimeHistogram);
}

}; // namespace
