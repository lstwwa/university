/*
设被测信号频率为fx
系统时钟频率为fclk
被测信号计数值为Nx
系统时钟计数值为Ns
则 fx = (Nx / Ns) * fclk
*/

module Cymometer(
	Clk,
	Rst_n,
	Gate_1s,
	Gate_Test_o,
	Xsig,
	Update_Flag,
	Freq_Result,
	f_result_fp,
	f_result_int,
	ndege_Gate_sync
);

	input Clk;	
	input Rst_n;
	input Gate_1s;		//1秒钟标准闸门信号，为了精准，可从外部输入
	output Gate_Test_o;	//内部1秒钟闸门信号输出，方便临时测试，可在IO上直接将其和Gate_1s接到一起
	input Xsig;			//外部待测试信号输入，推荐接到全局时钟管脚
	output Update_Flag;	//测量结果更新标志，每当该信号产生一个高脉冲，表示测量结果更新了
	output [31:0]Freq_Result;	//测量结果
	output [31:0]f_result_fp;	//测量结果浮点值
	output [31:0]f_result_int;	//测量结果整型值（放大了1000倍）
	output ndege_Gate_sync;	//同步后的闸门信号下降沿标志信号
	
	reg Gate_sync;	//与被测信号同步了的闸门信号
	reg clr;	//计数器清零信号
	reg [31:0]q_base_r,q_Xsig_r;	//存储每次测量的基本时钟信号和闸门信号计数值
	
	wire [31:0]q_base, q_Xsig;	//基本时钟信号和闸门信号计数值
	
	wire [31:0]Base_Clk_Freq;	//基准时钟信号频率值
	assign Base_Clk_Freq = 50_000_000;
	
	//使用被测信号同步1秒的闸门信号
	always@(posedge Xsig)
		Gate_sync <= Gate_1s;

	//内部1秒钟闸门信号，用于临时测试，可在IO上直接将其和Gate_1s接到一起
	gate_gen gate_gen(
		.Clk(Clk),
		.Rst_n(Rst_n),
		.gate_test(Gate_Test_o)
	);
		
	//使用计数器计数基准时钟脉冲
	counter counter_base(
		.aclr(clr),
		.clock(Clk),
		.cnt_en(Gate_sync),
		.q(q_base)
	);
	
	//使用计数器计数外部信号脉冲
	counter counter_gate(
		.aclr(clr),
		.clock(Xsig),
		.cnt_en(Gate_sync),
		.q(q_Xsig)
	);
	
	reg [1:0]Gate_sync_r;
	
	//寄存Gate_sync信号相邻两个clk周期的状态，用于检测边沿
	always@(posedge Clk)
		Gate_sync_r <= {Gate_sync_r[0],Gate_sync};
		
	assign ndege_Gate_sync = (Gate_sync_r == 2'b10);
		
	//检测到闸门信号下降沿后，清零计数器，保存该次测量结果
	always@(posedge Clk)
	if(ndege_Gate_sync)begin
		clr <= 1;
		q_base_r <= q_base;
		q_Xsig_r <= q_Xsig;
	end
	else begin
		clr <= 0;
		q_base_r <= q_base_r;
		q_Xsig_r <= q_Xsig_r;	
	end
	
	wire[31:0]q_Xsig_r_fp;	//被测信号计数结果浮点值
	wire[31:0]q_base_r_fp;	//基准时钟信号计数结果浮点值
	wire[31:0]q_50m_fp;		//50_000_000结果浮点值
	
	wire [31:0] div_r;	//被测信号计数值与基准时钟计数值的商
	
	wire [31:0]f_result_fp;	//测量频率的浮点值
	
	//整型转浮点，将被测信号计数结果转为浮点数，数据延迟6个时钟周期
	fp_conv fp_conv_xsig(
		.aclr(0),
		.clock(Clk),
		.dataa(q_Xsig_r),
		.result(q_Xsig_r_fp)
	);
	
	//整型转浮点，将基准时钟计数结果转为浮点数，数据延迟6个时钟周期	
	fp_conv fp_conv_base(
		.aclr(0),
		.clock(Clk),
		.dataa(q_base_r),
		.result(q_base_r_fp)
	);
	
	//整型转浮点，将50MHz时钟频率值转换为浮点数
	//其实该部分可以优化，直计算得到50_000_000的浮点值后写成一个常量
	fp_conv fp_conv_50M(
		.aclr(0),
		.clock(Clk),
		.dataa(Base_Clk_Freq),
		.result(q_50m_fp)
	);
	
	//浮点除法，计算被测信号计数值与基准时钟计数值的商
	div div(
		.aclr(0),
		.clock(Clk),
		.dataa(q_Xsig_r_fp),
		.datab(q_base_r_fp),
		.result(div_r)
	);
	
	//将被测信号计数值与基准时钟计数值的商乘以基准时钟信号频率
	mult mult(
		.aclr(0),
		.clock(Clk),
		.dataa(div_r),
		.datab(q_50m_fp),
		.result(f_result_fp)
	);
	
	//将浮点运算结果转为整型，注意，小数部分会因为四舍五入丢失
	fp2int fp2int(
		.clock(Clk),
		.dataa(f_result_fp),
		.result(f_result_int)
	);
	
	/*闸门信号下降沿标志延迟24个时钟周期后输出，作为数据结果输出标志，因为从
	闸门信号下降沿标志信号有效开始，将两个信号的测量结果寄存并送往计算单元进行
	计算，整型转浮点单元数据延迟时间为6个时钟、浮点除法计算单元延迟6个时钟，浮
	点乘法运算单元延迟11个时钟，合计23个时钟，加上数据寄存一个时钟，累计需延迟
	24个时钟，该值可以通过仿真查看验证*/
	
	reg [23:0]shift_reg;	//延时移位寄存器	
	//将闸门信号下降沿送入移位寄存器中
	always@(posedge Clk)
		shift_reg <= {shift_reg[22:0],ndege_Gate_sync};
	
	//使用经过24个时钟周期延迟的闸门信号下降沿标志信号作为结果有效的标志信号
	assign Update_Flag = shift_reg[23];	
	
	
	//以下为直接整型计算方法，会因为整型计算精度问题导致结果误差
	wire [63:0]Freq_Result_r;	//64位
	wire [63:0]mult_r;	//64位乘法计算结果值
	assign mult_r = 50000000* q_Xsig_r;
	assign Freq_Result_r =(mult_r / q_base_r);
	assign Freq_Result = Freq_Result_r[31:0];
	
endmodule
