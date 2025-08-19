`timescale 1 ns /  100 ps
module counter;
reg  clk; 
reg  reset; 
reg  enable; 
reg  [3:0] count; 


initial
   begin
  $display("\t\ttime,  \tclk  \treset  \tenable  \tcount");
  $monitor("%d,\t%b,  \t%b,  \t%b,  \t%b,\t%d",$time,  \tclk,  \treset,  \tenable,  \tcount");
 $dumpfile ("counter.vcd");
 $dumpvars;
end


initial
   begin
   clk =0;
   reset =0;
   enable =0;
#10   reset =1;
#10   enable =1;
#10   count =4'bXXXX;
#10   reset =0;
#10   enable =1;
#10   count =4'b0000;
#10   reset =0;
#10   enable =1;
#10   count =4'b0001;
#10   reset =0;
#10   enable =0;
#10   count =4'b0001;
#10   reset =1;
#10   enable =1;
#10   count =4'b0000;
#10  $finish;
end


always
#50 clk = ~clk
always
#10 clk2 = ~clk2


counter  U0 (
 .clk  (clk),
 .reset  (reset),
 .enable  (enable),
 .count  (count)
);


endmodule
