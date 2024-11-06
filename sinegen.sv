module sinegen #(
    parameter A_WIDTH = 8,
              D_WIDTH = 8
)(
    input logic clk,
    input logic rst,
    input logic [D_WIDTH-1:0] incr,        // increment value for counter
    input logic [A_WIDTH-1:0] phase_offset, // phase offset
    output logic [D_WIDTH-1:0] dout1,      // output for first sinusoid
    output logic [D_WIDTH-1:0] dout2       // output for phase-shifted sinusoid
);

    logic [A_WIDTH-1:0] address1, address2;

    // Counter for first address
    counter addrCounter1(
        .clk (clk),
        .rst (rst),
        .incr (incr),
        .count (address1)
    );

    // Compute second address with phase offset
    assign address2 = address1 + phase_offset;

    // Dual-port ROM instantiation
    rom2ports #(
        .ADDRESS_WIDTH(A_WIDTH),
        .DATA_WIDTH(D_WIDTH)
    ) sineRom (
        .clk (clk),
        .addr1 (address1),
        .addr2 (address2),
        .dout1 (dout1),
        .dout2 (dout2)
    );

endmodule
