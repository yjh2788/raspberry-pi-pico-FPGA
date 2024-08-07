module partial(x,x_sel,x2_sel,neg,pp);
parameter w=16;
input wire [w-1:0]x;
input x_sel,x2_sel,neg;
output wire [w:0]pp;

wire [w:0]xj2;
wire [w:0]xj1;
assign xj2={x,1'b0};
assign xj1={x[w-1],x};

assign pp= {(w+1){neg}}^((xj1&{(w+1){x_sel}})|(xj2&{(w+1){x2_sel}}));
endmodule 


/*
module partial(xj,xj_1,x_sel,x2_sel,neg,pp);
//partial u5( xj, xj1, w11, w12, neg1, pp1);
input wire [16:0]xj;
input wire [16:0]xj_1;
input x_sel,x2_sel,neg;
output wire [16:0]pp;

assign pp= neg^((xj&{17{x_sel}})|(xj_1&{17{x2_sel}}));
endmodule 
*/