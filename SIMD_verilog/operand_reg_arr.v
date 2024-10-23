module operand_reg_arr
#(parameter arr_size=32, bw=8)
(
	input rst,
	input clk,
	input wire[arr_size-1:0]en,
	input load,
	input wire[bw-1:0]din,
	output wire[bw*arr_size-1:0]dout
);



generate
genvar i;
	for(i=0;i<arr_size;i=i+1)begin:regs
		operand_reg u(rst,clk,en[i],load,din,dout[i*bw+bw-1:i*bw]);
	end

endgenerate 

endmodule