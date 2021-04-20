// Testbench for configurable harwdare accelerator
// Francesco Restuccia, 2021 
// ReTiS Lab, Scuola Superiore Sant'Anna Pisa, Italy
// email: francesco.restuccia@santannapisa.it

`timescale 1 ns / 1 ps

`define TB_R_BASE_ADDR  'ha0000000
`define TB_W_BASE_ADDR  'ha8000000

`define TB_R_DISPL      'h05
`define TB_W_DISPL      'h05

`define TB_R_MAX_OUTS      'h08
`define TB_W_MAX_OUTS      'h08

`define TB_R_NUM_TRANS 4
`define TB_W_NUM_TRANS 4

`define TB_W_BURST_LEN 8
`define TB_R_BURST_LEN 8

`define TB_EXEC_TIME 'h00000080


module AXI_mem_control_tb#(
    
    parameter integer C_S00_AXI_DATA_WIDTH = 32,
    parameter integer C_S00_AXI_ADDR_WIDTH = 6,
    
    
    // ------------------- END CONFIGURATION SLAVE INTERFACE ---------------------------
    
    // ------------------- MASTER OUTPUT INTERFACE  -----------------------------------
    
    parameter integer C_M_TARGET_SLAVE_BASE_ADDR = 32'h40000000,
    parameter integer C_M_AXI_BURST_LEN    = 16,
    parameter integer C_M_AXI_ID_WIDTH    = 1,
    parameter integer C_M_AXI_ADDR_WIDTH    = 32,
    parameter integer C_M_AXI_DATA_WIDTH    = 32,
    parameter integer C_M_AXI_ARUSER_WIDTH    = 1,
    parameter integer C_M_AXI_AWUSER_WIDTH    = 1,
    parameter integer C_M_AXI_WUSER_WIDTH    = 1,
    parameter integer C_M_AXI_RUSER_WIDTH    = 1,
    parameter integer C_M_AXI_BUSER_WIDTH    = 1
);



// -------------------- END SLAVE CONTROL INTERFACE PORTS ----------------------

//--------------------- MASTER OUTPUT INTERFACE PORTS --------------------------


//---------------------- CONFIGURATION SIGNALS --------------------------------

    logic[31:0] tb_r_base_addr; 
    logic[31:0] tb_w_base_addr; 
    
    logic[7:0] tb_r_disp;
    logic[7:0] tb_w_disp;
    
    logic[7:0] tb_r_max_outs;
    logic[7:0] tb_w_max_outs;
    
    logic[15:0] tb_r_num_trans;
    logic[15:0] tb_w_num_trans;
    
    logic[7:0] tb_w_burst_len;
    logic[7:0] tb_r_burst_len;
    
    logic[31:0] tb_exec_time;
    
//--------------------- GLOBAL SIGNALS ---------------------------------------

  logic aclk = 0;
  //logic aresetn = 1;
  logic done;
  logic intr_line_r;
  logic intr_line_w;

// ---------------------------------------------------------------------------
  
    
  
  logic[C_S00_AXI_ADDR_WIDTH-1 : 0] tb_S_AXI_AWADDR;
  logic[2 : 0] tb_S_AXI_AWPROT;
  logic tb_S_AXI_AWVALID;
  logic tb_S_AXI_AWREADY;

  logic[C_S00_AXI_DATA_WIDTH-1 : 0] tb_S_AXI_WDATA;
  logic[(C_S00_AXI_DATA_WIDTH/8)-1 : 0] tb_S_AXI_WSTRB;
  logic tb_S_AXI_WVALID;
  logic tb_S_AXI_WREADY;

  logic[1 : 0] tb_S_AXI_BRESP;
  logic tb_S_AXI_BVALID;
  logic tb_S_AXI_BREADY;

  logic[C_S00_AXI_ADDR_WIDTH-1 : 0] tb_S_AXI_ARADDR;
  logic[2 : 0] tb_S_AXI_ARPROT;
  logic tb_S_AXI_ARVALID;
  logic tb_S_AXI_ARREADY;

  logic[C_S00_AXI_DATA_WIDTH-1 : 0] tb_S_AXI_RDATA;
  logic[1 : 0] tb_S_AXI_RRESP;
  logic tb_S_AXI_RVALID;
  logic tb_S_AXI_RREADY;
  
  

  logic[C_M_AXI_ID_WIDTH-1 : 0]  tb_m_axi_awid;
  logic[C_M_AXI_ADDR_WIDTH-1 : 0]  tb_m_axi_awaddr;
  logic[7 : 0] tb_m_axi_awlen;
  logic[2 : 0] tb_m_axi_awsize;
  logic[1 : 0] tb_m_axi_awburst;
  logic tb_m_axi_awlock;
  logic[3 : 0] tb_m_axi_awcache;
  logic[2 : 0] tb_m_axi_awprot;
  logic[3 : 0] tb_m_axi_awqos;
  logic[C_M_AXI_AWUSER_WIDTH-1 : 0] tb_m_axi_awuser;
  logic tb_m_axi_awvalid;
  logic tb_m_axi_awready;

  logic[C_M_AXI_DATA_WIDTH-1 : 0] tb_m_axi_wdata;
  logic[C_M_AXI_DATA_WIDTH/8-1 : 0] tb_m_axi_wstrb;
  logic tb_m_axi_wlast;
  logic[C_M_AXI_WUSER_WIDTH-1 : 0] tb_m_axi_wuser;
  logic tb_m_axi_wvalid;
  logic tb_m_axi_wready;

  logic[C_M_AXI_ID_WIDTH-1 : 0] tb_m_axi_bid;
  logic[1 : 0] tb_m_axi_bresp;
  logic[C_M_AXI_BUSER_WIDTH-1 : 0] tb_m_axi_buser;
  logic tb_m_axi_bvalid;
  logic tb_m_axi_bready;

  logic[C_M_AXI_ID_WIDTH-1 : 0] tb_m_axi_arid;
  logic[C_M_AXI_ADDR_WIDTH-1 : 0] tb_m_axi_araddr;
  logic[7 : 0] tb_m_axi_arlen;
  logic[2 : 0] tb_m_axi_arsize;
  logic[1 : 0] tb_m_axi_arburst;
  logic tb_m_axi_arlock;
  logic[3 : 0] tb_m_axi_arcache;
  logic[2 : 0] tb_m_axi_arprot;
  logic[3 : 0] tb_m_axi_arqos;
  logic[C_M_AXI_ARUSER_WIDTH-1 : 0] tb_m_axi_aruser;
  logic tb_m_axi_arvalid;
  logic tb_m_axi_arready;

  logic[C_M_AXI_ID_WIDTH-1 : 0] tb_m_axi_rid;
  logic[C_M_AXI_DATA_WIDTH-1 : 0] tb_m_axi_rdata;
  logic[1 : 0] tb_m_axi_rresp;
  logic tb_m_axi_rlast;
  logic[C_M_AXI_RUSER_WIDTH-1 : 0] tb_m_axi_ruser;
  logic tb_m_axi_rvalid;
  logic tb_m_axi_rready;
  
//  logic HW_r_start;
// logic HW_w_start; //: in std_logic;
//  logic HW_reset; 
//  logic[31:0] tb_r_base_addr; // = 32'h08000000;   //  : in std_logic_vector (31 downto 0);
//  logic[31:0] tb_w_base_addr; // = 32'h0f000000;   //  : in std_logic_vector (31 downto 0);
//  logic [15:0] tb_r_num_trans; // = 1; //  : in std_logic_vector(15 downto 0);
//  logic [15:0] tb_w_num_trans; // = 1; // : in std_logic_vector(15 downto 0);
  
//  logic[7:0] tb_r_burst_len; // = 8;  //: in std_logic_vector(7 downto 0);
//  logic[7:0] tb_w_burst_len; // = 8;  //: in std_logic_vector(7 downto 0);
//  logic HW_data_val;   //   : in std_logic;
  //-- output done
  logic tb_w_done;  //    : out std_logic;
  logic tb_r_done;   //   : out std_logic;
  
  logic w_reply;
  
  logic tb_aresetn;
  
  logic[7:0] tb_r_trans_issued;
  logic[7:0] tb_r_served_trans;

  logic tb_r_trans_to_serve;


// instanciate the DUT
amalthea_prem #(

  .C_M00_AXI_TARGET_SLAVE_BASE_ADDR(0),
  .C_M00_AXI_BURST_LEN(16),
  .C_M00_AXI_ID_WIDTH(1),
  .C_M00_AXI_ADDR_WIDTH(32),
  .C_M00_AXI_DATA_WIDTH(32),
  .C_M00_AXI_AWUSER_WIDTH(1),
  .C_M00_AXI_ARUSER_WIDTH(1),
  .C_M00_AXI_WUSER_WIDTH(1),
  .C_M00_AXI_RUSER_WIDTH(1),
  .C_M00_AXI_BUSER_WIDTH(1),
  .C_S00_AXI_DATA_WIDTH(32),
  .C_S00_AXI_ADDR_WIDTH(6)
  
  )
  amalthea_prem (

    .clk (aclk),
    .axi_resetn (tb_aresetn),
    .w_done (tb_w_done),
    .r_done (tb_r_done),
    
    .m_axi_awid(tb_m_axi_awid),	
    .m_axi_awaddr(tb_m_axi_awaddr),	
    .m_axi_awlen(tb_m_axi_awlen),	
    .m_axi_awsize(tb_m_axi_awsize),
    .m_axi_awburst(tb_m_axi_awburst),
    .m_axi_awlock(tb_m_axi_awlock),
    .m_axi_awcache(tb_m_axi_awcache),
    .m_axi_awprot(tb_m_axi_awprot),
    .m_axi_awqos(tb_m_axi_awqos),
    .m_axi_awuser(tb_m_axi_awuser),
    .m_axi_awvalid(tb_m_axi_awvalid),
    .m_axi_awready(tb_m_axi_awready),
    .m_axi_wdata(tb_m_axi_wdata),
    .m_axi_wstrb(tb_m_axi_wstrb),
    .m_axi_wlast(tb_m_axi_wlast),
    .m_axi_wuser(tb_m_axi_wuser),
    .m_axi_wvalid(tb_m_axi_wvalid),
    .m_axi_wready(tb_m_axi_wready),
    .m_axi_bid(tb_m_axi_bid),
    .m_axi_bresp(tb_m_axi_bresp),
    .m_axi_buser(tb_m_axi_buser),
    .m_axi_bvalid(tb_m_axi_bvalid),
    .m_axi_bready(tb_m_axi_bready),
    .m_axi_arid(tb_m_axi_arid),
    .m_axi_araddr(tb_m_axi_araddr),
    .m_axi_arlen(tb_m_axi_arlen),
    .m_axi_arsize(tb_m_axi_arsize),
    .m_axi_arburst(tb_m_axi_arburst),
    .m_axi_arlock(tb_m_axi_arlock),
    .m_axi_arcache(tb_m_axi_arcache),
    .m_axi_arprot(tb_m_axi_arprot),
    .m_axi_arqos(tb_m_axi_arqos),
    .m_axi_aruser(tb_m_axi_aruser),
    .m_axi_arvalid(tb_m_axi_arvalid),
    .m_axi_arready(tb_m_axi_arready),
    .m_axi_rid(tb_m_axi_rid),
    .m_axi_rdata(tb_m_axi_rdata),
    .m_axi_rresp(tb_m_axi_rresp),
    .m_axi_rlast(tb_m_axi_rlast),
    .m_axi_ruser(tb_m_axi_ruser),
    .m_axi_rvalid(tb_m_axi_rvalid),
    .m_axi_rready(tb_m_axi_rready),
    
   
    .S_AXI_AWADDR(tb_S_AXI_AWADDR),
    .S_AXI_AWPROT(tb_S_AXI_AWPROT),
    .S_AXI_AWVALID(tb_S_AXI_AWVALID),
    .S_AXI_AWREADY(tb_S_AXI_AWREADY),
    .S_AXI_WDATA(tb_S_AXI_WDATA),
    .S_AXI_WSTRB(tb_S_AXI_WSTRB),
    .S_AXI_WVALID(tb_S_AXI_WVALID),
    .S_AXI_WREADY(tb_S_AXI_WREADY),
    .S_AXI_BRESP(tb_S_AXI_BRESP),
    .S_AXI_BVALID(tb_S_AXI_BVALID),
    .S_AXI_BREADY(tb_S_AXI_BREADY),
    .S_AXI_ARADDR(tb_S_AXI_ARADDR),
    .S_AXI_ARPROT(tb_S_AXI_ARPROT),
    .S_AXI_ARVALID(tb_S_AXI_ARVALID),
    .S_AXI_ARREADY(tb_S_AXI_ARREADY),
    .S_AXI_RDATA(tb_S_AXI_RDATA),
    .S_AXI_RRESP(tb_S_AXI_RRESP),
    .S_AXI_RVALID(tb_S_AXI_RVALID),
    .S_AXI_RREADY(tb_S_AXI_RREADY)
   
  );

 
//-------------------- START THE TEST ---------------------------
initial begin

reset_dut();

repeat (4) @(posedge aclk);

// ------------------- INITIAL CONFIGURATION ------------------------
tb_m_axi_rid <= 0;
tb_m_axi_rdata <= 0;
tb_m_axi_rresp <= 0;
tb_m_axi_rlast <= 0;
tb_m_axi_ruser <= 0;
tb_m_axi_rvalid <= 0;

tb_m_axi_bid <= 0;
tb_m_axi_bresp <= 0;
tb_m_axi_buser <= 0;


//------------SETUP THE BASE READ ADDRESS --------------

tb_S_AXI_AWADDR <= 6'h04;
tb_S_AXI_AWVALID <= 1;
    
repeat (2) @(posedge aclk);

tb_S_AXI_WDATA <= tb_r_base_addr;
tb_S_AXI_WSTRB <= 4'hf;
tb_S_AXI_WVALID <= 1;
    
@(tb_S_AXI_BVALID == 1 && tb_S_AXI_BREADY);
tb_S_AXI_AWVALID <= 0;
tb_S_AXI_WVALID <= 0;

//------------SETUP THE BASE WRITE ADDRESS --------------

tb_S_AXI_AWADDR <= 6'h08;
tb_S_AXI_AWVALID <= 1;
    
repeat (2) @(posedge aclk);

tb_S_AXI_WDATA <= tb_w_base_addr;
tb_S_AXI_WSTRB <= 4'hf;
tb_S_AXI_WVALID <= 1;
    
@(tb_S_AXI_WVALID == 1 && tb_S_AXI_AWVALID == 1 && tb_S_AXI_BVALID == 1);
tb_S_AXI_AWVALID <= 0;
tb_S_AXI_WVALID <= 0;


//------------SETUP DISPLACEMENTS AND MAX OUTS TRANSACTIONS  --------------


tb_S_AXI_AWADDR <= 6'h0c;
tb_S_AXI_AWVALID <= 1;
    
repeat (2) @(posedge aclk);

tb_S_AXI_WDATA <= {tb_r_disp,tb_w_disp,tb_r_max_outs,tb_w_max_outs};
tb_S_AXI_WSTRB <= 4'hf;
tb_S_AXI_WVALID <= 1;
    
@(tb_S_AXI_WVALID == 1 && tb_S_AXI_AWVALID == 1 && tb_S_AXI_BVALID == 1);
tb_S_AXI_AWVALID <= 0;
tb_S_AXI_WVALID <= 0;


//------------SETUP START PHASES -- NOT IN USE AT THIS TIME  --------------

tb_S_AXI_AWADDR <= 6'h10;
tb_S_AXI_AWVALID <= 1;
    
repeat (2) @(posedge aclk);

tb_S_AXI_WDATA <= 0;
tb_S_AXI_WSTRB <= 4'hf;
tb_S_AXI_WVALID <= 1;
    
@(tb_S_AXI_WVALID == 1 && tb_S_AXI_AWVALID == 1 && tb_S_AXI_BVALID == 1);
tb_S_AXI_AWVALID <= 0;
tb_S_AXI_WVALID <= 0;


//------------SETUP TRANSACTIONS NUMBER  --------------

tb_S_AXI_AWADDR <= 6'h14;
tb_S_AXI_AWVALID <= 1;
    
repeat (2) @(posedge aclk);

tb_S_AXI_WDATA <= {tb_r_num_trans,tb_w_num_trans};
tb_S_AXI_WSTRB <= 4'hf;
tb_S_AXI_WVALID <= 1;
    
@(tb_S_AXI_WVALID == 1 && tb_S_AXI_AWVALID == 1 && tb_S_AXI_BVALID == 1);
tb_S_AXI_AWVALID <= 0;
tb_S_AXI_WVALID <= 0;


//------------SETUP BURST LENGHTS  --------------


tb_S_AXI_AWADDR <= 6'h18;
tb_S_AXI_AWVALID <= 1;
    
repeat (2) @(posedge aclk);

tb_S_AXI_WDATA <= {tb_r_burst_len,tb_w_burst_len,16'h0};
tb_S_AXI_WSTRB <= 4'hf;
tb_S_AXI_WVALID <= 1;
    
@(tb_S_AXI_WVALID == 1 && tb_S_AXI_AWVALID == 1 && tb_S_AXI_BVALID == 1);
tb_S_AXI_AWVALID <= 0;
tb_S_AXI_WVALID <= 0;


//------------EXEC TIME  --------------

tb_S_AXI_AWADDR <= 6'h20;
tb_S_AXI_AWVALID <= 1;
    
repeat (2) @(posedge aclk);

tb_S_AXI_WDATA <= tb_exec_time;
tb_S_AXI_WSTRB <= 4'hf;
tb_S_AXI_WVALID <= 1;
    
@(tb_S_AXI_WVALID == 1 && tb_S_AXI_AWVALID == 1 && tb_S_AXI_BVALID == 1);
tb_S_AXI_AWVALID <= 0;
tb_S_AXI_WVALID <= 0;


//------------  FIRE!  --------------


tb_S_AXI_AWADDR <= 6'h00;
tb_S_AXI_AWVALID <= 1;
    
repeat (2) @(posedge aclk);

tb_S_AXI_WDATA <= 32'h00000002;
tb_S_AXI_WSTRB <= 4'hf;
tb_S_AXI_WVALID <= 1;
    
@(tb_S_AXI_WVALID == 1 && tb_S_AXI_AWVALID == 1 && tb_S_AXI_BVALID == 1);
tb_S_AXI_AWVALID <= 0;
tb_S_AXI_WVALID <= 0;


    
    
repeat (20) @(posedge aclk);

// ----------------- read ----------------



for(int i = 0; i < `TB_R_NUM_TRANS; i++)
begin
 
    // reads
    //@(posedge aclk && tb_m_axi_arvalid && tb_m_axi_arready);
    repeat (10) @(posedge aclk);
    tb_m_axi_rdata <= i;
    tb_m_axi_rresp <= 0;
    tb_m_axi_rvalid <= 1;
    repeat (`TB_W_BURST_LEN-1) @(posedge aclk && tb_m_axi_rvalid == 1 && tb_m_axi_rready == 1);
    tb_m_axi_rlast <= 1;
    repeat (2) @(posedge aclk && tb_m_axi_rvalid == 1 && tb_m_axi_rready == 1 && tb_m_axi_rlast == 1);
    tb_m_axi_rvalid <= 0;
    tb_m_axi_rlast <= 0;

end

@(posedge aclk && (tb_m_axi_awvalid))

//repeat (50) @(posedge aclk);

for(int j = 0; j < `TB_W_NUM_TRANS; j++)
begin
    repeat (15) @(posedge aclk);    
    tb_m_axi_bresp <= 0;
    tb_m_axi_bvalid <= 1;
    repeat (2) @(posedge aclk && tb_m_axi_bready)
    tb_m_axi_bvalid <= 0;

end



end

// ------------- CONCURRENT-----------------------------

  // ----------------- RESET THE DUT -----------------------
  task reset_dut();
      repeat (4) #10;
      tb_aresetn <= 0;
      repeat (4) #10;
      tb_aresetn <= 1;
      #10;
  endtask : reset_dut
  // --------------------------------------------------------

  //------------------ CLK PROCESS --------------------------
  initial begin
      while (!done) begin
          aclk <= 1;
          #(10/2);
          aclk <= 0;
          #(10/2);
      end
      $stop;
  end
// ----------------------------------------------------------

//--------------------- STATIC CONF TESTBENCH (MOCK MEMORY AND MOCK PROCESSOR) -------------

assign done = 0;

assign tb_s_axi_arprot = 0;
assign tb_s_axi_awprot = 0;
assign tb_s_axi_bready = 1;
assign tb_s_axi_rready = 1;

assign tb_m_axi_awready = 1;
assign tb_m_axi_arready = 1;
assign tb_m_axi_wready = 1;


assign tb_r_base_addr = `TB_R_BASE_ADDR;
assign tb_w_base_addr = `TB_W_BASE_ADDR;

assign tb_r_disp = `TB_R_DISPL;
assign tb_w_disp = `TB_W_DISPL;

assign tb_r_max_outs = `TB_R_MAX_OUTS;
assign tb_w_max_outs = `TB_W_MAX_OUTS;

assign tb_r_num_trans = `TB_R_NUM_TRANS;
assign tb_w_num_trans = `TB_W_NUM_TRANS;

assign tb_w_burst_len = `TB_W_BURST_LEN;
assign tb_r_burst_len = `TB_R_BURST_LEN;

assign tb_exec_time = `TB_EXEC_TIME;

//assign tb_S_AXI_BVALID = w_reply;


assign tb_S_AXI_BREADY = 1;
assign tb_S_AXI_AWREADY = 1;
assign tb_S_AXI_ARREADY = 1;


 



//always@(posedge aclk)
//begin
//    w_reply <= 0;
//    if(tb_S_AXI_WREADY && tb_S_AXI_WVALID) begin
//        w_reply <= 1;
//    end
//end


//if(`READ_TEST)
//begin
//    HW_r_start <= 1;

////    repeat (`HW_NUM_R_TRANS) @(posedge aclk && m_axi_arvalid == 1 && m_axi_arready == 1);

//    repeat (10) @(posedge aclk);

//    for(int i = 0; i < `HW_NUM_R_TRANS; i++)
//    begin
//        if(`ILL_TRANS) begin 
//            illegal <= 1;
//            @(posedge aclk && (intr_line_r || intr_line_w) );  
//            repeat (20) @(posedge aclk);
//            // read the anomaly register
            
            
//            // reset the anomaly
//            s_axi_awaddr <= 4'b0000;
//            s_axi_awvalid <= 1;
            
//            repeat (2) @(posedge aclk);
            
//            s_axi_wdata <= 1'b1;
//            s_axi_wstrb <= 4'hf;
//            s_axi_wvalid <= 1;
        
//            @(s_axi_wvalid == 1 && s_axi_awvalid == 1 && s_axi_bvalid == 1);
//            s_axi_awvalid <= 0;
//            s_axi_wvalid <= 0; 
            
//        end      
//        else begin 
//            illegal <= 0;
//            @(posedge aclk && (~init_flag || (m_axi_arvalid && m_axi_arready)));
//            init_flag <= 0;
//            repeat (10) @(posedge aclk);
//            m_axi_rdata <= i;
//            m_axi_rresp <= 0;
//            m_axi_rvalid <= 1;
//            repeat (`HW_R_BURST_SIZE-1) @(posedge aclk && m_axi_rvalid == 1 && m_axi_rready == 1);
//            m_axi_rlast <= 1;
//            repeat (2) @(posedge aclk && m_axi_rvalid == 1 && m_axi_rready == 1 && m_axi_rlast == 1);
//            m_axi_rvalid <= 0;
//            m_axi_rlast <= 0;
//        end
//    end
//end
////---------------------- CHECK THE RESULTS ------------------------------------------

//// ----------------------- FIRE ON WRITES ------------------------------------------

//if(`WRITE_TEST)
//begin
//    HW_w_start <= 1;

////    repeat (`HW_NUM_W_TRANS) @(posedge aclk && m_axi_awvalid == 1 && m_axi_awready == 1);

//    for(int i = 0; i < `HW_NUM_W_TRANS; i++)
//    begin 
//        if(`ILL_TRANS) begin // illegal transaction
        
//            $display("into illegal trans");
//            illegal <= 1;
            
//            @(posedge aclk && (intr_line_r || intr_line_w) );
//            repeat (5) @(posedge aclk);
//            // read the anomaly register
            
            
//            // reset the anomaly
//            s_axi_awaddr <= 4'b0000;
//            s_axi_awvalid <= 1;
            
//            repeat (2) @(posedge aclk);
            
//            s_axi_wdata <= 1'b1;
//            s_axi_wstrb <= 4'hf;
//            s_axi_wvalid <= 1;
        
//            @(s_axi_wvalid == 1 && s_axi_awvalid == 1 && s_axi_bvalid == 1);
//            s_axi_awvalid <= 0;
//            s_axi_wvalid <= 0;
           
//        end
//        else begin
        
//            $display("into legal trans");
//            illegal <= 0;
            
//            //@(posedge aclk && m_axi_awvalid && m_axi_awready);
        
//            @(posedge aclk && m_axi_wvalid && m_axi_wready && m_axi_wlast);
//            repeat (5) @(posedge aclk); 
//            m_axi_bvalid <= 1;
//            repeat (2) @(posedge aclk && m_axi_bvalid);
//            m_axi_bvalid <= 0;

//        end
        
//    end
//end


// ------------------------- RESET THE ANOMALIES ---------------------------------------


endmodule
