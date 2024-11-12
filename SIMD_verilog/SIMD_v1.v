module SIMD_v1
#(parameter reg_size=32, bw=8, num_reg = 65)
(
	input rst,
	input CS,
	input excute,
	input WR,
	input RD,
	input AD,//1=Addr,0=data
	input wire[bw-1:0]data_in,
	output wire[bw-1:0]data_out

);

wire [reg_size*bw-1:0]reg_A,reg_B,result;
wire [bw-1:0]addr_reg_out,reg_mode;
wire [64:0]regs_en;
wire excute_n;

			 reg_8 	ADDR		(.rst(rst),.clk(WR),.en(AD&CS),.din(data_in),.dout(addr_reg_out));
	addr_decoder 	decoder	(.din(addr_reg_out),.enable(regs_en));
operand_reg_arr 	op_A		(.rst(rst),.clk(WR),.en(regs_en[reg_size-1:0] & {reg_size{CS}}),.load(AD),.din(data_in),.dout(reg_A));
operand_reg_arr 	op_B		(.rst(rst),.clk(WR),.en(regs_en[reg_size*2-1:reg_size] & {reg_size{CS}}),.load(AD),.din(data_in),.dout(reg_B));
			 reg_8 	MODE		(.rst(rst),.clk(WR),.en(regs_en[reg_size*2]&(!AD)&CS),.din(data_in),.dout(reg_mode));
		//MAC_array 	MAC		(.rst(rst),.clk(excute),.en(CS),.mode(reg_mode[1:0]),.din_a(reg_A),.din_b(reg_B),.dout(result));
		MAC_array 	MAC		(.mode(reg_mode[1:0]),.din_a(reg_A),.din_b(reg_B),.dout(result));
		shift_reg	results 	(.rst(rst),.  clk(RD),.en(CS),.load(excute),.din(result),.dout(data_out));


endmodule 

module reg_result 
#(parameter reg_size=32, bw=8)
(
	input rst, 
	input clk,
	input en,
	input wire [reg_size*bw-1:0]din,
	output reg [reg_size*bw-1:0]dout 
);

	always@(posedge clk or posedge rst) begin
		if(rst) dout<=0;
		if(en)begin
			dout<=din;
		end
	end	
endmodule

module reg_8(
	input rst,
	input clk,
	input en,
	input wire[7:0] din,
	output reg[7:0] dout
);

	always@(posedge clk or posedge rst) begin
		if(rst) dout<=0;
		else if(en)begin
			dout<=din;
		end
	end
endmodule

module addr_decoder
#(parameter num_reg=65, bw=8)
(
	input wire [bw-1:0]din,
	output wire[num_reg-1:0]enable
);

	assign enable=1<<din;

endmodule


module shift_reg
#(parameter reg_size=32, bw=8)
(
	input rst,
	input clk,
	input en,
	input load,
	input wire[bw*reg_size-1:0]din,
	output wire[bw-1:0]dout
);
reg [bw*reg_size-1:0] shift_data;
assign dout=shift_data[bw*reg_size-1:bw*reg_size-8];
 
always@(posedge clk or posedge load or posedge rst)begin
		if(rst) begin
			shift_data<=0;
		end
		else if(load) shift_data<=din;
		else if(en) begin  
			shift_data<={shift_data[bw*reg_size-bw-1:0],{bw{1'b0}}};
		end	

		
end

endmodule





