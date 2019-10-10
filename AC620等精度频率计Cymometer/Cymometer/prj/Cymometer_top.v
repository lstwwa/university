module Cymometer_top(
	Clk,
	Rst_n,
	Xsig,
	SH_CP,
	ST_CP,
	cnt,
	DS
);
	input Clk;
	input Rst_n;
	input Xsig;
	
	output SH_CP;
	output ST_CP;
	output DS;
	output reg [27:0] cnt;
	
	wire Gate_1s;
	wire [31:0]disp_data;
	wire [31:0]Freq_Result;

	bin_to_BCD bin_to_BCD
	(
		.bin(Freq_Result[27:0]),
		.bcd(disp_data)
	);
	
	HEX_top HEX_top(
		.Clk(Clk),
		.Rst_n(Rst_n),
		.disp_data(disp_data),
		.SH_CP(SH_CP),
		.ST_CP(ST_CP),
		.DS(DS)
	);
	
	Cymometer Cymometer(
		.Clk(Clk),
		.Rst_n(Rst_n),
		.Gate_1s(Gate_1s),
		.Gate_Test_o(Gate_1s),
		.Xsig(Xsig),
		.Update_Flag(),
		.Freq_Result(Freq_Result),
		.f_result_fp(),
		.f_result_int(),
		.ndege_Gate_sync()
	);
	
	
	always@(posedge Clk)
		cnt <= cnt + 1;

endmodule
