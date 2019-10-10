module gate_gen(
	Clk,
	Rst_n,
	gate_test
);

	input Clk;
	input Rst_n;
	output reg gate_test;

	//产生闸门信号
	reg [31:0]cnt2;
	
	always@(posedge Clk or negedge Rst_n)
	if(!Rst_n)
		cnt2 <= 1'b0;
	else if(cnt2 == 49_999_999)
		cnt2 <= 1'b0;
	else
		cnt2 <= cnt2 + 1'b1;
		
	always@(posedge Clk or negedge Rst_n)
	if(!Rst_n)
		gate_test <= 1'b0;
	else if(cnt2 == 49_999_999)
		gate_test <= ~gate_test;

endmodule
