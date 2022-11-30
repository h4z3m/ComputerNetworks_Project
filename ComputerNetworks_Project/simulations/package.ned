package computernetworks_project.simulations;

import computernetworks_project.Coordinator;
import computernetworks_project.Node;
@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=561,269");
    submodules:
        coordinator: Coordinator {
            @display("p=295,168;i=block/cogwheel");
        }
        Node1: Node {
            @display("p=410,93;i=device/switch,,0");
        }
        Node0: Node {
            @display("p=166,93;i=device/switch");
        }
    connections:
        coordinator.out1 --> Node1.in_gate;
        coordinator.out0 --> Node0.in_gate;
        Node0.out_gate --> Node1.coordinator_gate;
        Node1.out_gate --> Node0.coordinator_gate;
}

