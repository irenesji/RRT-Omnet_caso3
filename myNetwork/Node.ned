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

module Node
{
    @display("bgb=254,228");
    gates:
        inout port[];
        input filler;
    submodules:
        queue[sizeof(port)]: Queue {
            @display("p=111,161");
            
        }
        routing: Routing {
            @display("p=216,47");
            gates:
                in[sizeof(port)];
                out[sizeof(port)];
                filler;
        }
    connections:
        for i=0..sizeof(port)-1 {
            port[i] <--> queue[i].line;
            queue[i].in <-- routing.out[i];
            routing.in[i] <-- queue[i].out;

        }
        routing.filler <-- filler;
        

}
