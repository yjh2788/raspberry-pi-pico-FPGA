module PG(a,b,P,G);
input a,b;
output P,G;

assign P=a^b;
assign G=a&b;

endmodule 