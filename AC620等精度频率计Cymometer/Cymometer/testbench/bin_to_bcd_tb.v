`timescale 1ns/1ns

module bin_to_bcd_tb;

	reg [27:0] bin;
	wire [31:0] bcd;
	integer i;

	bin_to_BCD u1(.bin(bin), .bcd(bcd));

	initial begin
		bin = 0;
		for(i=99999999; i>=0; i=i-1)
			#20 bin = i;
	end

endmodule
