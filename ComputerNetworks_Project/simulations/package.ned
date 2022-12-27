package computernetworks_project.simulations;

import computernetworks_project.Coordinator;
import computernetworks_project.Node;
@license(LGPL);
//
// TODO documentation
//
network Network
{
    parameters:
        double ProcessingDelay = default(0.5);
        double DuplicationDelay = default(0.1);
        double TransmissionDelay = default(0.3);
        double ErrorDelay = default(4.0);
        int WindowSize = default(5);
        double TimeoutInterval = default(10);
        double ACKLossProbability = default(10);

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
        coordinator.out0 --> Node0.coordinator_gate;
        coordinator.out1 --> Node1.coordinator_gate;
        Node1.out_gate --> Node0.in_gate;
        Node0.out_gate --> Node1.in_gate;
}

