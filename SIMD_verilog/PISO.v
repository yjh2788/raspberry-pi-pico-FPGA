module PISO
#(parameter bw= 32)
(
	input rst,
	input clk,
	input send,
	input wire [bw-1:0]data_in,
	output data_out
);

reg [bw-1:0]tmp;

assign data_out = tmp[bw-1];

always@(posedge clk)begin
	if(rst) tmp <= 0;
	if(send) tmp <= {tmp[bw-2:0], 1'b0};
	else tmp <= data_in;

end

endmodule 