module SIMD
#(parameter bw = 32)
(
	 input rst,
	 input clk,
	 input load,
	 input wire [3:0]mode,
	 input dtype,
	 input wire [7:0]data_in,
	 input send,
	 output wire [3:0]data
	 
);

wire [bw-1:0]A0, A1, A2, A3;
wire [bw-1:0]B0, B1, B2, B3;
wire [bw-1:0]result0, result1, result2, result3;

SPI_SIMD t1(
.stsinkvalid(),  
.stsinkdata(),   
.stsinkready(),  
.stsourceready(),
.stsourcevalid(),
.stsourcedata(), 
.sysclk(),       
.nreset(),       
.mosi(),         
.nss(),          
.miso(),         
.sclk());          


SIPO u1(rst,clk,load,data_in[0],A0);
SIPO u2(rst,clk,load,data_in[1],B0);
ALU A_1(.A(A0),.B(B0),.mode(mode),.dtype(dtype),.result(result0));
PISO P1(.rst(rst), .clk(clk), .send(send), .data_in(result0), .data_out(data[0]));

SIPO u3(rst,clk,load,data_in[2],A1);
SIPO u4(rst,clk,load,data_in[3],B1);
ALU A_2(.A(A1),.B(B1),.mode(mode),.dtype(dtype),.result(result1));
PISO P2(.rst(rst), .clk(clk), .send(send), .data_in(result1), .data_out(data[1]));

SIPO u5(rst,clk,load,data_in[4],A2);
SIPO u6(rst,clk,load,data_in[5],B2);
ALU A_3(.A(A2),.B(B2),.mode(mode),.dtype(dtype),.result(result2));
PISO P3(.rst(rst), .clk(clk), .send(send), .data_in(result2), .data_out(data[2]));

SIPO u7(rst,clk,load,data_in[6],A3);
SIPO u8(rst,clk,load,data_in[7],B3);   
ALU A_4(.A(A3),.B(B3),.mode(mode),.dtype(dtype),.result(result3));
PISO P4(.rst(rst), .clk(clk), .send(send), .data_in(result3), .data_out(data[3]));

endmodule
