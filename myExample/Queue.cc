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

#include "Queue.h"

namespace myexample {

Define_Module(Queue);

Queue::Queue()
{

    timerMessage = NULL;

}
Queue::~Queue(){

}

void Queue::initialize()
{
    // TODO - Generated method body
    queue = new cPacketQueue();
    msgNum=0;
    protocol = STOPANDWAIT;
    window = NULL;
    sentInWindow = NULL;
    //timerMessage = new cMessage("timer");
    //scheduleAt( simTime(), timerMessage);
}

void Queue::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    EV << "Message arrived on in gate = " << msg->arrivedOn("in") << "]\n";
    EV << "Queue is empty = " << queue->isEmpty() << "]\n";
    simtime_t timer;
    cMessage *timerMessage = new cMessage("txFinished");
    cPacket *msgOut;

    if(msg->getName()=="txFinished")
    {
        cancelEvent(timerMessage);
        switch(protocol)
        {
            case 1:
                if( sentInWindow < window && !queue->isEmpty() ){
                    lastPack=queue->pop();
                    msgOut = lastPack->dup();
                    send(msgOut, "line$o");
                    timer=gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
                    scheduleAt(simTime()+timer, timerMessage);
                    msgNum--;
                }

            default :
                break;
        }

    }
    else if(msg->arrivedOn("in"))
    {

        if(msgNum==0 )
        {
            lastPack = check_and_cast<cPacket *>(msg);
            send(lastPack->dup(), "line$o");
            EV << "msgNum = " << msgNum << "]\n";
            timer=gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
            scheduleAt(timer, timerMessage);
        }
        else{
            queue->insert(check_and_cast<cPacket *>(msg));

        }
        msgNum++;

    }
    else if(msg->arrivedOn("line$i")){
        const char *name;
        name = msg->getName();
        EV << "In line: " << name <<"\n";
        if(opp_strcmp("ACK", name)==0){
            EV<< "ARRIVED ACK" <<"]\n";
            delete(lastPack);
            if( !queue->isEmpty() ){
                lastPack=queue->pop();
                msgOut = lastPack->dup();

                send(msgOut, "line$o");
                timer=gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
                scheduleAt(simTime()+timer, timerMessage);

            }
            msgNum--;

        }
        else if(opp_strcmp("NACK", name)==0){
            EV<< "ARRIVED NACK" <<"]\n";
            send(lastPack->dup(), "line$o");
            scheduleAt(simTime()+timer, timerMessage);
        }

    }



}

} //namespace
