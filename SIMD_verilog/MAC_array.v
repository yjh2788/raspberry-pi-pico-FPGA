module MAC_array
#(parameter bw = 8, num_MAC = 32)
(	
	input rst,
	input clk,
	input wire [1:0]mode,
	input wire [bw*num_MAC-1:0]din_a,
	input wire [bw*num_MAC-1:0]din_b,
	output wire [bw*num_MAC-1:0]dout
);

MAC A(rst,clk,mode,din_a[bw-1:0],din_b[bw-1:0],8'b0,dout[bw-1:0]);

generate 
genvar i;
	for(i=1;i<num_MAC;i=i+1)begin: mac
		MAC u(rst,clk,mode,din_a[i*bw+bw-1:i*bw],din_b[i*bw+bw-1:i*bw],dout[i*bw-1:i*bw-bw],dout[i*bw+bw-1:i*bw]);
	end

endgenerate

endmodule 