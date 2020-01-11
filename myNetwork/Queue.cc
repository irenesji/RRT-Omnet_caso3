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
    //protocol = GOBACKN;
    protocol = STOPANDWAIT;
    window = 5;
    sentInWindow = 0;
    firstElemNum = 0;
    retransmision = false;
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
        if(retransmision)
        {

        }
        else{
            switch(protocol)
            {
                case GOBACKN:
                    if( sentInWindow < window && !queue->isEmpty() ){
                        lastPack=queue->pop();
                        packets.add(lastPack);
                        msgOut = lastPack->dup();
                        send(msgOut, "line$o");
                        timer=gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
                        scheduleAt(simTime()+timer, timerMessage);
                        sentInWindow++;
                        msgNum--;
                    }

                default :
                    if( !queue->isEmpty() ){
                        lastPack=queue->pop();

                        msgOut = lastPack->dup();
                        send(msgOut, "line$o");
                        timer=gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
                        scheduleAt(simTime()+timer, timerMessage);

                        msgNum--;
                    }
                    break;
            }
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
            if(protocol == GOBACKN)
            {
                sentInWindow++;
            }

        }
        else{
            queue->insert(check_and_cast<cPacket *>(msg));

        }
        msgNum++;

    }
    else if(msg->arrivedOn("line$i")){
        cPacket *arrivedPacket = check_and_cast<cPacket *>(msg);;
        const char *name;
        name = msg->getName();
        EV << "In line: " << name <<"\n";
        if(opp_strcmp("ACK", name)==0){

            switch (protocol)
            {
                case 1:
                    EV<< "ARRIVED ACK "<<"]\n";
                    packets.remove(msg->par("seqNum").longValue() - firstElemNum);
                    if( !queue->isEmpty() ){
                        lastPack=queue->pop();
                        packets.add(lastPack);
                        msgOut = lastPack->dup();
                        send(msgOut, "line$o");
                        timer=gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
                        scheduleAt(simTime()+timer, timerMessage);

                    }


                    break;
                default:
                    EV<< "ARRIVED ACK\n";
                    delete(lastPack);
                    if( !queue->isEmpty() ){
                        lastPack=queue->pop();
                        msgOut = lastPack->dup();
                        send(msgOut, "line$o");
                        timer=gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
                        scheduleAt(simTime()+timer, timerMessage);

                    }
                    break;

            }

            msgNum--;

        }
        else if(opp_strcmp("NACK", name)==0){
            if(protocol == STOPANDWAIT){
                EV<< "ARRIVED NACK" <<"]\n";
                send(lastPack->dup(), "line$o");
                scheduleAt(simTime()+timer, timerMessage);
            }
            else if(protocol == GOBACKN){

            }
        }
        else
        {
            cPacket *response;


            EV << "Received " << msg->getName() << msg->par("seqNum").longValue() << endl;

            if(arrivedPacket->hasBitError()){
                response = new cPacket("NACK",1);

            }
            else{
                response = new cPacket("ACK",1);
                send(arrivedPacket,"out");

            }
            response->addPar("seqNum");
            response->par("seqNum").setLongValue(arrivedPacket->par("seqNum").longValue());
            sendDelayed(response, 0.0002, "line$o");
        }

    }



}

} //namespace
