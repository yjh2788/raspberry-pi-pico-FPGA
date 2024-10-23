module operand_reg
#(parameter bw=8)
(
	input rst,
	input clk,
	input en,
	input load,
	input wire[bw-1:0]din,
	output reg[bw-1:0]dout
);



always@(posedge clk or posedge rst)begin
	if(rst)
		dout<=0;
	else if(en) begin
		if(!load) dout<=din;
	end
end

endmodule