module TOP(
	input clk,
	input CS,
	input WR,
	input RD,
	input CD,//addr=1,data=0
	input excute,
	input sel_poc,
	input sel_sod,
	input direction,
	inout wire[7:0] pico_data,
	input wire[7:0] cam_data,
	output wire[7:0] LCD_data
	);

wire [7:0] pico_data_in;
wire [7:0] pico_data_out;
wire [7:0] simd_out;
wire [7:0] pico_cam;
wire rst;

///assign simd_out = 8'hff;

assign pico_data_in = pico_data;
assign pico_data = direction ? pico_data_out:8'bz;

assign pico_cam = sel_poc ? cam_data:pico_data_in;
assign LCD_data = pico_cam;
assign pico_data_out = sel_sod ? simd_out:pico_cam;

power_on_reset POR(.clk(clk),.rst(rst));

//SIMD U1(.rst(rst),
//        .clk(clk),
//        .load(),
//        .mode(),
//        .dtype(),
//        .data_in(pico_data_in),
//        .send(),
//        .data());

SIMD_CNN U2(
		   .rst(rst),
			//.clk(),
		   .CS(CS),
			.excute(excute),
			.WR(WR),
			.RD(RD),
			.AD(CD),//1=Addr,0=data
			.data_in(pico_data_in),
			.data_out(simd_out));



endmodule 

module power_on_reset (
    input wire clk,       // system clk
    output reg rst     // reset(active low)
);
    reg [3:0] reset_counter = 4'h6; //
	 reg [3:0] reset_counter2 = 4'h6;
    always @(posedge clk) begin
        if (reset_counter != 0) begin
            reset_counter <= reset_counter - 1; 
				rst <= 0; end
			else if (reset_counter2 != 0) begin
					reset_counter2 <= reset_counter2 - 1; 
					rst <= 1; end
				else
					rst <= 0; 	
    end
endmodule

