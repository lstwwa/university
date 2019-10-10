`timescale 1ns/1ps

`define clk_period 20

`define gate_period 200000

module Cymometer_tb;

	reg Clk;	
	reg Rst_n;
	reg Gate_1s;		//1秒钟标准闸门信号，为了精准，可从外部输入
	wire Gate_Test_o;	//内部1秒钟闸门信号输出，方便临时测试，可在IO上直接将其和Gate_1s接到一起
	reg Xsig;			//外部待测试信号输入，推荐接到全局时钟管脚
	wire Update_Flag;	//测量结果更新标志，每当该信号产生一个高脉冲，表示测量结果更新了
	wire [31:0]Freq_Result;	//测量结果
	wire [31:0]f_result_fp;
	wire [31:0]f_result_int;

	Cymometer Cymometer(
		.Clk(Clk),
		.Rst_n(Rst_n),
		.Gate_1s(Gate_1s),
		.Gate_Test_o(Gate_Test_o),
		.Xsig(Xsig),
		.Update_Flag(Update_Flag),
		.Freq_Result(Freq_Result),
		.f_result_fp(f_result_fp),
		.f_result_int(f_result_int),
		.ndege_Gate_sync(ndege_Gate_sync)
	);

	initial Clk = 1;
	always#(`clk_period/2)Clk = ~Clk;

	initial begin
		Gate_1s = 1;
		#2;
		forever begin
			#(`gate_period/2)Gate_1s = ~Gate_1s;
		end	
	end
	
	initial begin
		Xsig = 1;
		Rst_n = 1'b0;
		#200;
		Rst_n = 1;
		repeat(500000)begin
			#550 Xsig = ~Xsig;
		end
		$stop;	
	end	

endmodule
