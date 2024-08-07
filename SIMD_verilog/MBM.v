module MBM(x,y,p);
parameter w=16;
input wire[w-1:0]x,y;
output wire[2*w-1:0]p;

wire [w:0]ha_1s,ha_1c,fa_1s,fa_1c,fa_2s,fa_2c,fa_3s,fa_3c,fa_4s,fa_4c,fa_5s,fa_5c,fa_6s,fa_6c;
wire [w:0]tpp1,tpp2,tpp3,tpp4,tpp5,tpp6,tpp7,tpp8,tpp9,tpp10,tpp11,tpp12,tpp13;
wire [w:0]y_;
wire [w/2-1:0]neg;
wire [w:0]pp[w/2-1:0];
wire x_sel[0:w/2-1];
wire x2_sel[0:w/2-1];

genvar i;
generate
for(i=0;i<w/2;i=i+1) begin:c
code u(y_[2*i],y_[2*i+1],y_[2*i+2],x_sel[i],x2_sel[i],neg[i]);
partial #(w)a(x,x_sel[i],x2_sel[i],neg[i],pp[i]);
end
endgenerate

assign y_={y,1'b0};
assign tpp1={{2{pp[0][w]}},pp[0][w:2]};//pp[0];
assign tpp2={ha_1c[w],ha_1c[w:1]};
assign tpp3={ha_1s[w],ha_1s[w],ha_1s[w:2]};
assign tpp4={fa_1c[w],fa_1c[w:1]};
assign tpp5={fa_1s[w],fa_1s[w],fa_1s[w:2]};
assign tpp6={fa_2c[w],fa_2c[w:1]};
assign tpp7={fa_2s[w],fa_2s[w],fa_2s[w:2]};
assign tpp8={fa_3c[w],fa_3c[w:1]};
assign tpp9={fa_3s[w],fa_3s[w],fa_3s[w:2]};
assign tpp10={fa_4c[w],fa_4c[w:1]};
assign tpp11={fa_4s[w],fa_4s[w],fa_4s[w:2]};
assign tpp12={fa_5c[w],fa_5c[w:1]};
assign tpp13={fa_5s[w],fa_5s[w],fa_5s[w:2]};

HA_a #(w)c1(pp[1],tpp1,ha_1s,ha_1c);//{{2{tpp1[w]}},tpp1[w:2]}
FA_a #(w)c2(pp[2],tpp2,tpp3,fa_1s,fa_1c);
FA_a #(w)c3(pp[3],tpp4,tpp5,fa_2s,fa_2c);
FA_a #(w)c4(pp[4],tpp6,tpp7,fa_3s,fa_3c);
FA_a #(w)c5(pp[5],tpp8,tpp9,fa_4s,fa_4c);
FA_a #(w)c6(pp[6],tpp10,tpp11,fa_5s,fa_5c);
FA_a #(w)c7(pp[7],tpp12,tpp13,fa_6s,fa_6c);

wire [2*w-1:0]a,b;
assign a={fa_6s[w],fa_6s,fa_5s[1:0],fa_4s[1:0],fa_3s[1:0],fa_2s[1:0],fa_1s[1:0],ha_1s[1:0],pp[0][1:0]};
assign b={fa_6c,neg[7],fa_5c[0],neg[6],fa_4c[0],neg[5],fa_3c[0],neg[4],fa_2c[0],neg[3],fa_1c[0],neg[2],ha_1c[0],neg[1],1'b0,neg[0]};

assign p=a+b;

endmodule 

