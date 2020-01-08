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
    @display("bgb=259,253");
    types:
        channel C extends ned.DatarateChannel{
        	datarate=3000000bps;
        	ber=0.001;
        }
    
    submodules:
        source: Source {
            @display("p=27,35");
        }
        node: Node {
            @display("p=76,112");
            gates:
                filler;
                port[1];
        }
        sink: Sink {
            @display("p=214,112");

        }
    connections:
        source.out --> node.filler;
        node.port[0] <--> C <--> sink.in;

}
