module code(a,b,c,x_sel,x2_sel,neg);
input a,b,c;
output x_sel,x2_sel,neg;

wire L,H;
assign L = a&b;
assign H = !(a|b);
assign x_sel = a^b;
assign x2_sel = c? H:L;
assign neg = c;

endmodule
