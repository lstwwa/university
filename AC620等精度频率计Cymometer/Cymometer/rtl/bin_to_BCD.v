module bcd_single_modify(bcd_in, bcd_out);

	input [3:0] bcd_in;
	output [3:0] bcd_out;

	reg [3:0] bcd_out;
	
	always @ (bcd_in)
	begin
		if (bcd_in > 4)
			bcd_out = bcd_in + 2'd3;
		else
			bcd_out = bcd_in;
	end

endmodule


module bcd_modify(data_in, data_out);

	input [59:0] data_in;
	output [59:0] data_out;
	
	bcd_single_modify bcd9(.bcd_in(data_in[59:56]), .bcd_out(data_out[59:56]));
	bcd_single_modify bcd8(.bcd_in(data_in[55:52]), .bcd_out(data_out[55:52]));
	bcd_single_modify bcd7(.bcd_in(data_in[51:48]), .bcd_out(data_out[51:48]));
	
	bcd_single_modify bcd6(.bcd_in(data_in[47:44]), .bcd_out(data_out[47:44]));
	bcd_single_modify bcd5(.bcd_in(data_in[43:40]), .bcd_out(data_out[43:40]));
	bcd_single_modify bcd4(.bcd_in(data_in[39:36]), .bcd_out(data_out[39:36]));
	bcd_single_modify bcd3(.bcd_in(data_in[35:32]), .bcd_out(data_out[35:32]));
	bcd_single_modify bcd2(.bcd_in(data_in[31:28]), .bcd_out(data_out[31:28]));
	assign data_out[27:0] = data_in[27:0];

endmodule



module bin_to_BCD(bin, bcd);

	input [27:0] bin;
	output [31:0] bcd;
	
	wire [59:0] shift_reg [28:0];
	
	assign shift_reg[28] = {32'b0, bin};
	
	bcd_modify b27(.data_in(shift_reg[28]<<1), .data_out(shift_reg[27]));
	bcd_modify b26(.data_in(shift_reg[27]<<1), .data_out(shift_reg[26]));
	bcd_modify b25(.data_in(shift_reg[26]<<1), .data_out(shift_reg[25]));
	bcd_modify b24(.data_in(shift_reg[25]<<1), .data_out(shift_reg[24]));
	bcd_modify b23(.data_in(shift_reg[24]<<1), .data_out(shift_reg[23]));
	bcd_modify b22(.data_in(shift_reg[23]<<1), .data_out(shift_reg[22]));
	bcd_modify b21(.data_in(shift_reg[22]<<1), .data_out(shift_reg[21]));
	bcd_modify b20(.data_in(shift_reg[21]<<1), .data_out(shift_reg[20]));	
	
	bcd_modify b19(.data_in(shift_reg[20]<<1), .data_out(shift_reg[19]));
	bcd_modify b18(.data_in(shift_reg[19]<<1), .data_out(shift_reg[18]));
	bcd_modify b17(.data_in(shift_reg[18]<<1), .data_out(shift_reg[17]));
	bcd_modify b16(.data_in(shift_reg[17]<<1), .data_out(shift_reg[16]));
	bcd_modify b15(.data_in(shift_reg[16]<<1), .data_out(shift_reg[15]));
	
	bcd_modify b14(.data_in(shift_reg[15]<<1), .data_out(shift_reg[14]));
	bcd_modify b13(.data_in(shift_reg[14]<<1), .data_out(shift_reg[13]));
	bcd_modify b12(.data_in(shift_reg[13]<<1), .data_out(shift_reg[12]));
	bcd_modify b11(.data_in(shift_reg[12]<<1), .data_out(shift_reg[11]));
	bcd_modify b10(.data_in(shift_reg[11]<<1), .data_out(shift_reg[10]));
	bcd_modify b9(.data_in(shift_reg[10]<<1), .data_out(shift_reg[9]));
	bcd_modify b8(.data_in(shift_reg[9]<<1), .data_out(shift_reg[8]));
	
	bcd_modify b7(.data_in(shift_reg[8]<<1), .data_out(shift_reg[7]));
	bcd_modify b6(.data_in(shift_reg[7]<<1), .data_out(shift_reg[6]));
	bcd_modify b5(.data_in(shift_reg[6]<<1), .data_out(shift_reg[5]));
	bcd_modify b4(.data_in(shift_reg[5]<<1), .data_out(shift_reg[4]));
	bcd_modify b3(.data_in(shift_reg[4]<<1), .data_out(shift_reg[3]));
	bcd_modify b2(.data_in(shift_reg[3]<<1), .data_out(shift_reg[2]));
	bcd_modify b1(.data_in(shift_reg[2]<<1), .data_out(shift_reg[1]));
	assign shift_reg[0] = shift_reg[1]<<1;

	assign bcd = shift_reg[0][59:28];

endmodule
