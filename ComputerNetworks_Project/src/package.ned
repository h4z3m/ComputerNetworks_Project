package computernetworks_project;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=544,329");
    submodules:
        coordinator: Coordinator {
            @display("p=270,75");
        }
        node0: Node {
            @display("p=90,184");
        }
        node1: Node {
            @display("p=441,184");
        }
    connections:
        coordinator.out1 --> node0.coordinator_gate;
        coordinator.out0 --> node1.coordinator_gate;
        node0.out_gate --> node1.in_gate;
        node1.out_gate --> node0.in_gate;
}

