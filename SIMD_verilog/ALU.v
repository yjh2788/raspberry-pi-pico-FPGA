module ALU
 #(parameter bw= 32)
(
	input wire [bw-1:0]A,
	input wire [bw-1:0]B,
	input wire [3:0]mode,
	input dtype,
	output wire [bw-1:0]result
//	output wire [bw-1:0]high,
//	output wire [bw-1:0]low 
);

wire [bw-1:0] result_int,result_float;

arithmatic u1(.mode(mode),
				  .A(A), 
				  .B(B),
				  .out_L(result_int));
				  
floatingPoint u2(.dataa(A),
                 .datab(B),
                 .n(mode),    
					  .result(result_float));
					  
assign result=dtype? result_float:result_int;

					  
endmodule 