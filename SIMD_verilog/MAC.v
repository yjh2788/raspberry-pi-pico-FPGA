module MAC
#( parameter bw=8)
(
	//input rst,
	//input clk,
	//input en,
	input wire [1:0]mode,
	input wire [bw-1:0]din_a,
	input wire [bw-1:0]din_b,
	input wire [bw-1:0]din_c,
	output wire [bw-1:0]dout
);

wire [bw-1:0]add_a, add_b, res_add, res_mul, result;
///a+b mode=2'b00
///a*b mode=2'b1x
///a*b+c mode=2'b01
assign add_a = (mode[0]) ? din_c:din_a;
assign add_b = (mode[0]) ? res_mul:din_b;
assign dout = (mode[1]) ? res_mul:res_add;//result
assign res_add = add_a + add_b;
assign res_mul = din_a * din_b;

//always@(posedge clk or posedge rst)begin
//	if(rst)
//		dout<=0;
//	else if(en)  
//		dout<=result;
//end
endmodule 