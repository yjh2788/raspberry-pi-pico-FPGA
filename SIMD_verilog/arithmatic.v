module arithmatic
#( parameter bw=32)
(
	input wire [3:0]mode,
	input wire signed[bw-1:0]A,
	input wire signed[bw-1:0]B,
	//output wire [bw-1:0]out_H,
	output reg [bw-1:0]out_L
	
);

wire signed [bw-1:0] sum;
wire signed [bw-1:0] sub;
wire signed [2*bw-1:0] mul;
wire signed [bw-1:0] div;

assign sum = A + B;
assign sub = A - B;
assign mul = A * B;
assign div = B != 0 ? A / B : 0;

always@* begin
	case(mode) 
		4'b0000 : out_L = sum;
		4'b0001 : out_L = sub;
		4'b0010 : out_L = mul[bw-1:0];
		4'b0011 : out_L = div;
		default : out_L = 0;
	endcase
end
endmodule 