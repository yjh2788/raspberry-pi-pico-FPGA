`timescale 1ns/1ns
module tb_TOP;
reg clk;
reg CS;
reg WR;
reg RD;
reg CD;//addr=1,data=0
reg excute;
reg sel_poc;
reg sel_sod;
reg direction;//input=1 output =0
reg [7:0] in_pico_data;
wire [7:0] monitor_pico_data;
reg [7:0] cam_data;
wire [7:0] LCD_data;


assign monitor_pico_data = (direction) ? 8'bz:in_pico_data;

TOP U1(clk,CS,WR,RD,CD,excute,sel_poc,sel_sod,direction,monitor_pico_data,cam_data,LCD_data);

always forever #1 clk=~clk;
initial begin
clk=0;CS=0;WR=0;RD=0;CD=0;excute=0;sel_poc=0;sel_sod=1;direction=0;in_pico_data=8'hff;cam_data=8'h00;
#50 CD=1; #10 WR=1;#10 WR=0;
#50 CS=1;
#50 CD=1; #50 in_pico_data=8'd64; #10 WR=1;#10 WR=0;//mode
#50 CD=0; #50 in_pico_data=8'd1; #10 WR=1;#10 WR=0;//a+b=0, a*b=1, a*b+c'=2

#50 CD=1; #50 in_pico_data=8'd00; #10 WR=1;#10 WR=0;
#50 CD=0; #50 in_pico_data=8'd2; #10 WR=1;#10 WR=0;

#50 CD=1; #50 in_pico_data=8'd01; #10 WR=1;#10 WR=0;
#50 CD=0; #50 in_pico_data=8'd3; #10 WR=1;#10 WR=0;

#50 CD=1; #50 in_pico_data=8'd02; #10 WR=1;#10 WR=0;
#50 CD=0; #50 in_pico_data=8'd4; #10 WR=1;#10 WR=0;

#50 CD=1; #50 in_pico_data=8'd32; #10 WR=1;#10 WR=0;
#50 CD=0; #50 in_pico_data=8'd5; #10 WR=1;#10 WR=0;

#50 CD=1; #50 in_pico_data=8'd33; #10 WR=1;#10 WR=0;
#50 CD=0; #50 in_pico_data=8'd6; #10 WR=1;#10 WR=0;

#50 CD=1; #50 in_pico_data=8'd34; #10 WR=1;#10 WR=0;
#50 CD=0; #50 in_pico_data=8'd7; #10 WR=1;#10 WR=0;

#50 excute=1; #10 excute=0;direction=1;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;

#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;

#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;

#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;

#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
#10 RD=1; #10 RD=0;
end
endmodule