`timescale 1 ns / 1 ns

module tb_simd;

reg rst;
reg clk=0;
reg load;
reg [3:0]mode;
reg dtype=1;
reg [7:0]data_;
reg send;
wire [3:0]data;

always forever #100 clk= ~clk;

SIMD u1(rst,clk,load,mode,dtype,data_,send,data);


initial begin
rst=1;send=0;mode=4'b0000;load=0;data_ = 8'ha1;
#300 rst=0; load=1;
#200 data_ = 8'h13;
#100 data_ = 8'h3d;
#100 data_ = 8'h73;
#100 data_ = 8'h07;
#100 data_ = 8'h90;
#100 data_ = 8'h49;
#100 data_ = 8'h83;
#100 data_ = 8'h76;
#100 data_ = 8'h13;
#100 data_ = 8'h2e;
#100 data_ = 8'h64;
#100 data_ = 8'h58;
#100 data_ = 8'h07;
#100 data_ = 8'h11;
#100 data_ = 8'h57;

#100 data_ = 8'ha1;
#100 data_ = 8'h13;
#100 data_ = 8'h3d;
#100 data_ = 8'h73;
#100 data_ = 8'h0f;
#100 data_ = 8'h9c;
#100 data_ = 8'h49;
#100 data_ = 8'h83;
#100 data_ = 8'h7a;
#100 data_ = 8'h13;
#100 data_ = 8'hce;
#100 data_ = 8'h64;
#100 data_ = 8'hd8;
#100 data_ = 8'h07;
#100 data_ = 8'he1;
#100 data_ = 8'h57;
#10 load=0;
#300 send=1;
#3200 send=0;
 

#100 mode=4'b0001;
#300 send=1;
#3200 send=0;

#100 mode=4'b0010;
#300 send=1;
#3200 send=0;

#100 mode=4'b0011;
#300 send=1;
#3200 send=0;
 
end
endmodule 