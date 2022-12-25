package computernetworks_project;

import computernetworks_project.Coordinator;
import computernetworks_project.Node;
@license(LGPL);
//
// TODO documentation
//
network Network
{


        @display("bgb=281,212");



    submodules:
        coordinator: Coordinator {
            @display("p=133,131;i=block/cogwheel");
        }
        Node1: Node {
            @display("p=215,40;i=device/switch,,0");
        }
        Node0: Node {
            @display("p=54,40;i=device/switch");
        }
    connections:
        coordinator.out1 --> Node1.in_gate;
        coordinator.out0 --> Node0.in_gate;
        Node0.out_gate --> Node1.coordinator_gate;
        Node1.out_gate --> Node0.coordinator_gate;


}

