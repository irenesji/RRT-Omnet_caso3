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

#include "Source.h"

namespace myexample {

Define_Module(Source);

Source::Source()
{
    timerMessage = NULL;
}

Source::~Source()
{
    cancelAndDelete(timerMessage);
}

void Source::initialize()
{
    time=0.0005;
    timerMessage = new cMessage("timer");
    seqNum = 1;
    scheduleAt(simTime(), timerMessage);
}

void Source::handleMessage(cMessage *msg)
{
    ASSERT(msg==timerMessage);

    cPacket *job = new cPacket("job");
    //Se añade un número de secuencia para identicicar el paquete
    job->addPar("seqNum");
    job->par("seqNum").setLongValue(seqNum++);

    job->setBitLength(1024);


    send(job, "out");

    scheduleAt(simTime()+exponential(time), timerMessage);
}

}; // namespace
