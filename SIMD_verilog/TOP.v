module TOP(
//	input cs,
//	input WR,
//	input RD,
	input sel_poc,
	input sel_sod,
	input direction,
	inout wire[7:0] pico_data,
	input wire[7:0] cam_data,
	output wire[7:0] LCD_data
	);

wire[7:0] pico_data_in;
wire [7:0] pico_data_out;
wire [7:0] simd_out;
wire [7:0] pico_cam;

assign simd_out = 8'hff;

assign pico_data_in = pico_data;
assign pico_data = direction ? pico_data_out:8'bz;

assign pico_cam = sel_poc ? cam_data:pico_data_in;
assign LCD_data = pico_cam;
assign pico_data_out = sel_sod ? simd_out:pico_cam;


//SIMD U1(.rst(),
//        .clk(),
//        .load(),
//        .mode(),
//        .dtype(),
//        .data_in(pico_data_in),
//        .send(),
//        .data());

//always@(posedge WR or posedge RD) begin
//	if(!cs) begin
//		if(WR) begin
//			data_in <= pico_data;
//		end
//		
//		if(RD) begin
//			pico_data< = data_out;
//		end
//	end
//
//end


endmodule 