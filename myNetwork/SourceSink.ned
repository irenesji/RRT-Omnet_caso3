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

package myexample;

import ned.DatarateChannel;


//
// Sample network, consisting of a source and a sink.
//
network SourceSink
{
          
    @display("bgb=326.12003,265.63");
    types:
        channel C extends ned.DatarateChannel
        {
            datarate = 3000000bps;
            ber = 0.001;
        }
    
    
    
    submodules:
        source: Source {
            @display("p=27,35");
        }
        source2: Source {
            @display("p=82.40667,34.190002");
        }
        source5: Source {
            @display("p=77.14667,205.14001");
        }
        node: Node {
            @display("p=82.40667,97.310005");
            //Se puede definir que distintas colas tengan distintos protocolos
            queue[*].protocol = 0;
            routing.probability = 0.25;
            gates:
                filler;
                port[2];
        }
        node2: Node {
            @display("p=130.62334,34.190002");
            queue[*].protocol = 0;
            routing.probability=1/3;
            gates:
                filler;
                port[4];
        }
        node3: Node2 {
            @display("p=200.75667,34.190002");
            queue[*].protocol = 0;
            routing.probability=1;
            gates:
                port[2];
        }
        node4: Node2 {
            @display("p=213.90668,170.95001");
            queue[*].protocol = 0;
            routing.probability=1;
            gates:
                
                port[3];
        }
        node5: Node {
            @display("p=130.62334,163.06001");
            queue[*].protocol = 0;
            routing.probability=0.5;
            gates:
                filler;
                port[3];
        }
        sink: Sink {
            @display("p=278.78,34.190002");

        }
        sink2: Sink {
            @display("p=291.05334,170.95001");

        }
    connections:
        source.out --> node.filler;
        source2.out --> node2.filler;
        source5.out --> node5.filler;
        node.port[0] <--> C <--> node2.port[0];
        node.port[1] <--> C <--> node5.port[0];
        node2.port[1] <--> C <--> node5.port[1];
        node2.port[2] <--> C <--> node3.port[0];
        node2.port[3] <--> C <--> node4.port[0];
        node5.port[2] <--> C <--> node4.port[1];
        node3.port[1] <--> C <--> sink.in;
        node4.port[2] <--> C <--> sink2.in;



}
