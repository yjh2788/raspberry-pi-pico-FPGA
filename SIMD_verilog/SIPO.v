module SIPO
#(parameter bw= 32)
(
	input rst,
	input clk,
	input load,
	input data_in,
	output reg [bw-1:0]data_out
	
); 

always@(posedge clk)begin
	if(rst) data_out <= 0;
	if(load) begin
		data_out <= {data_out[bw-2:0], data_in};
	end

end

endmodule 