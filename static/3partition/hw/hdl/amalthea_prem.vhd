-- generic AMALTHEA hw IP
-- Francesco Restuccia, 2021
-- ReTiS Lab, Scuola Superiore Sant'Anna Pisa, Italy
-- email: francesco.restuccia@santannapisa.it


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use ieee.math_real.all;
use ieee.std_logic_misc.all;

entity amalthea_prem is
	generic (
    -- Users to add parameters here

    -- User parameters ends
    -- Do not modify the parameters beyond this line


    -- Parameters of Axi Master Bus Interface M00_AXI
    C_M00_AXI_TARGET_SLAVE_BASE_ADDR    : std_logic_vector    := x"00000000";
    C_M00_AXI_BURST_LEN    : integer    := 16;
    C_M00_AXI_ID_WIDTH    : integer    := 1;
    C_M00_AXI_ADDR_WIDTH    : integer    := 32;
    C_M00_AXI_DATA_WIDTH    : integer    := 32;
    C_M00_AXI_AWUSER_WIDTH    : integer    := 1;
    C_M00_AXI_ARUSER_WIDTH    : integer    := 1;
    C_M00_AXI_WUSER_WIDTH    : integer    := 1;
    C_M00_AXI_RUSER_WIDTH    : integer    := 1;
    C_M00_AXI_BUSER_WIDTH    : integer    := 1;

    -- Parameters of Axi Slave Bus Interface S00_AXI
--    C_S00_AXI_ID_WIDTH    : integer    := 1;
--    C_S00_AXI_DATA_WIDTH    : integer    := 32;
--    C_S00_AXI_ADDR_WIDTH    : integer    := 32;
--    C_S00_AXI_AWUSER_WIDTH    : integer    := 1;
--    C_S00_AXI_ARUSER_WIDTH    : integer    := 1;
--    C_S00_AXI_WUSER_WIDTH    : integer    := 1;
--    C_S00_AXI_RUSER_WIDTH    : integer    := 1;
--    C_S00_AXI_BUSER_WIDTH    : integer    := 1
	C_S00_AXI_DATA_WIDTH	: integer	:= 32;
	C_S00_AXI_ADDR_WIDTH	: integer	:= 6

);
	port (
		-- Users to add ports here

            -- DEBUG PRUPOSE
            --r_misb_clk_cnt_out : out std_logic_vector(15 downto 0);
            --r_ready_to_sample_out   : out std_logic;

        --clock port
            ap_clk     : in std_logic;
--            ext_trigger : in std_logic;
        -- start port
            --r_start : in std_logic;
            --w_start : in std_logic;
        -- reset port
            --reset   : in std_logic;
            ap_rst_n  : in std_logic;
        -- min displacement between two requests
--            r_displ        : in std_logic_vector (7 downto 0);
--            w_displ        : in std_logic_vector (7 downto 0);
--        -- max outs transactions
--            r_max_outs      : in std_logic_vector (7 downto 0);
--            w_max_outs      : in std_logic_vector (7 downto 0);
--        -- phase
--            r_phase         : in std_logic_vector (15 downto 0);
--            w_phase         : in std_logic_vector (15 downto 0);
--        -- base address
--            r_base_addr     : in std_logic_vector (31 downto 0);
--            w_base_addr     : in std_logic_vector (31 downto 0);
--        -- num transaction
--            r_num_trans  : in std_logic_vector(15 downto 0);
--            w_num_trans  : in std_logic_vector(15 downto 0);

--       -- burst size
--            r_burst_len  : in std_logic_vector(7 downto 0);
--            w_burst_len  : in std_logic_vector(7 downto 0);

       -- input data valid
        --data_val     : in std_logic;

       -- misbehaving clock cycle
          --  r_misb_clk_cycle    : in std_logic_vector(15 downto 0);
          --  w_misb_clk_cycle    : in std_logic_vector(15 downto 0);

        -- output done
        w_done      : out std_logic;
        r_done      : out std_logic;

		-- Ports of Axi Master Bus Interface
		m_axi_mem_bus_awid	: out std_logic_vector(C_M00_AXI_ID_WIDTH-1 downto 0);
		m_axi_mem_bus_awaddr	: out std_logic_vector(C_M00_AXI_ADDR_WIDTH-1 downto 0);
		m_axi_mem_bus_awlen	: out std_logic_vector(7 downto 0);
		m_axi_mem_bus_awsize	: out std_logic_vector(2 downto 0);
		m_axi_mem_bus_awburst	: out std_logic_vector(1 downto 0);
		m_axi_mem_bus_awlock	: out std_logic;
		m_axi_mem_bus_awcache	: out std_logic_vector(3 downto 0);
		m_axi_mem_bus_awprot	: out std_logic_vector(2 downto 0);
		m_axi_mem_bus_awqos	: out std_logic_vector(3 downto 0);
		m_axi_mem_bus_awuser	: out std_logic_vector(C_M00_AXI_AWUSER_WIDTH-1 downto 0);
		m_axi_mem_bus_awvalid	: out std_logic;
		m_axi_mem_bus_awready	: in std_logic;
		m_axi_mem_bus_wdata	: out std_logic_vector(C_M00_AXI_DATA_WIDTH-1 downto 0);
		m_axi_mem_bus_wstrb	: out std_logic_vector(C_M00_AXI_DATA_WIDTH/8-1 downto 0);
		m_axi_mem_bus_wlast	: out std_logic;
		m_axi_mem_bus_wuser	: out std_logic_vector(C_M00_AXI_WUSER_WIDTH-1 downto 0);
		m_axi_mem_bus_wvalid	: out std_logic;
		m_axi_mem_bus_wready	: in std_logic;
		m_axi_mem_bus_bid	: in std_logic_vector(C_M00_AXI_ID_WIDTH-1 downto 0);
		m_axi_mem_bus_bresp	: in std_logic_vector(1 downto 0);
		m_axi_mem_bus_buser	: in std_logic_vector(C_M00_AXI_BUSER_WIDTH-1 downto 0);
		m_axi_mem_bus_bvalid	: in std_logic;
		m_axi_mem_bus_bready	: out std_logic;
		m_axi_mem_bus_arid	: out std_logic_vector(C_M00_AXI_ID_WIDTH-1 downto 0);
		m_axi_mem_bus_araddr	: out std_logic_vector(C_M00_AXI_ADDR_WIDTH-1 downto 0);
		m_axi_mem_bus_arlen	: out std_logic_vector(7 downto 0);
		m_axi_mem_bus_arsize	: out std_logic_vector(2 downto 0);
		m_axi_mem_bus_arburst	: out std_logic_vector(1 downto 0);
		m_axi_mem_bus_arlock	: out std_logic;
		m_axi_mem_bus_arcache	: out std_logic_vector(3 downto 0);
		m_axi_mem_bus_arprot	: out std_logic_vector(2 downto 0);
		m_axi_mem_bus_arqos	: out std_logic_vector(3 downto 0);
		m_axi_mem_bus_aruser	: out std_logic_vector(C_M00_AXI_ARUSER_WIDTH-1 downto 0);
		m_axi_mem_bus_arvalid	: out std_logic := '0'; -- initializing handshake signals on the address read channel
		m_axi_mem_bus_arready	: in std_logic;
		m_axi_mem_bus_rid	: in std_logic_vector(C_M00_AXI_ID_WIDTH-1 downto 0);
		m_axi_mem_bus_rdata	: in std_logic_vector(C_M00_AXI_DATA_WIDTH-1 downto 0);
		m_axi_mem_bus_rresp	: in std_logic_vector(1 downto 0);
		m_axi_mem_bus_rlast	: in std_logic;
		m_axi_mem_bus_ruser	: in std_logic_vector(C_M00_AXI_RUSER_WIDTH-1 downto 0);
		m_axi_mem_bus_rvalid	: in std_logic;
		m_axi_mem_bus_rready	: out std_logic;
		m_axi_mem_bus_AWREGION : OUT STD_LOGIC_VECTOR (3 downto 0);
		m_axi_mem_bus_ARREGION : OUT STD_LOGIC_VECTOR (3 downto 0);
		m_axi_mem_bus_WID : OUT STD_LOGIC_VECTOR (C_M00_AXI_ID_WIDTH-1 downto 0);



		--s_axi_ctrl_bus_ACLK	: in std_logic;
		--s_axi_ctrl_bus_ARESETN	: in std_logic;
		s_axi_ctrl_bus_AWADDR	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s_axi_ctrl_bus_AWPROT	: in std_logic_vector(2 downto 0);
		s_axi_ctrl_bus_AWVALID	: in std_logic;
		s_axi_ctrl_bus_AWREADY	: out std_logic;
		s_axi_ctrl_bus_WDATA	: in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s_axi_ctrl_bus_WSTRB	: in std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 downto 0);
		s_axi_ctrl_bus_WVALID	: in std_logic;
		s_axi_ctrl_bus_WREADY	: out std_logic;
		s_axi_ctrl_bus_BRESP	: out std_logic_vector(1 downto 0);
		s_axi_ctrl_bus_BVALID	: out std_logic;
		s_axi_ctrl_bus_BREADY	: in std_logic;
		s_axi_ctrl_bus_ARADDR	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s_axi_ctrl_bus_ARPROT	: in std_logic_vector(2 downto 0);
		s_axi_ctrl_bus_ARVALID	: in std_logic;
		s_axi_ctrl_bus_ARREADY	: out std_logic;
		s_axi_ctrl_bus_RDATA	: out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s_axi_ctrl_bus_RRESP	: out std_logic_vector(1 downto 0);
		s_axi_ctrl_bus_RVALID	: out std_logic;
		s_axi_ctrl_bus_RREADY	: in std_logic;
		interrupt : OUT STD_LOGIC
	);
end entity;

architecture arch_imp of amalthea_prem is

    ---------------- CONTROL INTERFACE SIGNALS ------------------

	signal axi_awaddr	: std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
	signal axi_awready	: std_logic;
	signal axi_wready	: std_logic;
	signal axi_bresp	: std_logic_vector(1 downto 0);
	signal axi_bvalid	: std_logic;
	signal axi_araddr	: std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
	signal axi_arready	: std_logic;
	signal axi_rdata	: std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal axi_rresp	: std_logic_vector(1 downto 0);
	signal axi_rvalid	: std_logic;

	constant ADDR_LSB  : integer := (C_S00_AXI_DATA_WIDTH/32)+ 1;
	constant OPT_MEM_ADDR_BITS : integer := 3;
	------------------------------------------------

	signal slv_reg0	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg1	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg2	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg3	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg4	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg5	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg6	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg7	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg8	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg9	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg10	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg11	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg12	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg13	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg14	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg15	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal slv_reg_rden	: std_logic;
	signal slv_reg_wren	: std_logic;
	signal reg_data_out	:std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
	signal byte_index	: integer;
	signal aw_en	: std_logic;


--------------------------------------------------------------------------------------------
----------------------- ADDRESS REGISTERS MAP ----------------------------------------------
----------------32|--------|---------|---------|---------|0---------------------------------

--- r: reset
--- gs: global start

---slv_reg0-------|--------|---------|---------|-|-|-|-|-|-|gs|r|
---slv_reg1-------|----------r-base-addr-----------------|
---slv_reg2-------|----------w-base-addr-----------------|
---slv_reg3-------|r_displ-|w_displ--|r-max-outs|w-max-outs|
---slv-reg4-------|r-phase-----------|w-phase------------|
---slv_reg5-------|r-num-trans-------|w-num-trans--------|
---slv_reg6-------|r_b_len-|w_b_len--|-------------------|
---slv_reg7-------|r-misbehave-clk---|w-misbehave-clk----|
---slv-reg8-------|---------exec-time--------------------|

   signal r_misb_clk_cycle      : std_logic_vector(15 downto 0);
   signal w_misb_clk_cycle      : std_logic_vector(15 downto 0);


    signal exec_cnt_int         : std_logic_vector(31 downto 0);
    signal exec_time_clk        : std_logic_vector(31 downto 0);

    signal r_displ_int          : std_logic_vector(7 downto 0);
    signal w_displ_int          : std_logic_vector(7 downto 0);

    signal r_max_outs_int       : std_logic_vector(7 downto 0);
    signal w_max_outs_int       : std_logic_vector(7 downto 0);

    signal r_base_addr_int      : std_logic_vector(31 downto 0);
    signal w_base_addr_int      : std_logic_vector(31 downto 0);

    signal r_num_trans_int      : std_logic_vector(15 downto 0);
    signal w_num_trans_int      : std_logic_vector(15 downto 0);

    signal r_trans_counter      : std_logic_vector(15 downto 0) := (others => '0');
    signal w_trans_counter      : std_logic_vector(15 downto 0) := (others => '0');

    signal r_done_counter       : std_logic_vector(15 downto 0) := (others => '0');
    signal w_done_counter       : std_logic_vector(15 downto 0) := (others => '0');

    signal r_phase_int          : std_logic_vector(15 downto 0) := (others => '0');
    signal w_phase_int          : std_logic_vector(15 downto 0) := (others => '0');

    signal r_phase_counter          : std_logic_vector(15 downto 0) := (others => '0');
    signal w_phase_counter          : std_logic_vector(15 downto 0) := (others => '0');

    signal r_burst_len_int      : std_logic_vector(7 downto 0);
    signal w_burst_len_int      : std_logic_vector(7 downto 0);

    signal r_start_int          : std_logic := '0';
    signal w_start_int          : std_logic := '0';

--    signal r_phase_start        : std_logic := '0';
--    signal w_phase_start        : std_logic := '0';

    signal m_axi_mem_bus_ARADDR_INT     : std_logic_vector(C_M00_AXI_ADDR_WIDTH-1 downto 0) := (others => '0');
    signal m_axi_mem_bus_ARVALID_INT    : std_logic := '0';

    -- address write channel internal signals

    signal m_axi_mem_bus_AWADDR_INT     : std_logic_vector(C_M00_AXI_ADDR_WIDTH-1 downto 0) := (others => '0');
    signal m_axi_mem_bus_AWVALID_INT    : std_logic := '0';
    --data read channel internal signals

    --w data channel

    signal m_axi_mem_bus_WVALID_INT     : std_logic := '0';
    signal m_axi_mem_bus_WLAST_INT      : std_logic := '0';
    signal m_axi_mem_bus_WDATA_INT      : std_logic_vector(31 downto 0);

    signal w_state              : std_logic_vector (2 downto 0) := (others => '0');
    signal r_state              : std_logic_vector (2 downto 0) := (others => '0');

    signal r_addr_init          : std_logic := '0';
    signal w_addr_init          : std_logic := '0';

    signal r_displ_cnt          : std_logic_vector(15 downto 0) := (others => '0');
    signal w_displ_cnt          : std_logic_vector(15 downto 0) := (others => '0');

    signal w_data_counter       : std_logic_vector(7 downto 0)  := x"01";
    signal w_data_done_cnt      : std_logic_vector(15 downto 0) := (others => '0');

    --signal data_val_flag        : std_logic := '0';

    signal r_pend_outs_trans    : std_logic_vector(15 downto 0) := (others => '0');
    signal w_pend_outs_trans    : std_logic_vector(15 downto 0) := (others => '0');

    signal r_end_trans_flag     : std_logic_vector(0 downto 0) := "0";
    signal w_end_trans_flag     : std_logic_vector(0 downto 0) := "0";

    signal r_grant_trans_flag     : std_logic_vector(0 downto 0) := "0";
    signal w_grant_trans_flag     : std_logic_vector(0 downto 0) := "0";

    signal r_misb_clk_cnt       : std_logic_vector(15 downto 0);
    signal w_misb_clk_cnt       : std_logic_vector(15 downto 0);

    signal r_ready_to_sample    : std_logic;
    signal w_ready_to_write     : std_logic;

    signal reset                : std_logic;
    signal global_start         : std_logic;

    signal r_done_int           : std_logic;
    signal w_done_int           : std_logic;

    begin

        -- FOR DEBUG PURPOSE
        --r_misb_clk_cnt_out <= r_misb_clk_cnt;
        --r_ready_to_sample_out <= r_ready_to_sample;

        -- address write channel
        m_axi_mem_bus_awid  <= "0";
        m_axi_mem_bus_awaddr    <= m_axi_mem_bus_AWADDR_INT;
        m_axi_mem_bus_awlen <= w_burst_len_int;
        m_axi_mem_bus_awsize    <= "010";
        m_axi_mem_bus_awburst    <= "01";
        m_axi_mem_bus_awlock    <= '0';
        m_axi_mem_bus_awcache    <= "0011";
        m_axi_mem_bus_awprot    <= "000";
        m_axi_mem_bus_awqos    <= x"0";
        m_axi_mem_bus_awuser    <= "0";
        m_axi_mem_bus_awvalid    <= m_axi_mem_bus_AWVALID_INT;

		-- data write channel
		m_axi_mem_bus_wdata	<= m_axi_mem_bus_WDATA_INT; --x"00000000";
        m_axi_mem_bus_wstrb    <= x"F";
        m_axi_mem_bus_wlast   <= m_axi_mem_bus_WLAST_INT;
        m_axi_mem_bus_wuser   <= "0";
        m_axi_mem_bus_wvalid    <= m_axi_mem_bus_WVALID_INT;

		-- b channel

        m_axi_mem_bus_bready <= '1';

		-- address read channel
		m_axi_mem_bus_arid  <= "0";
        m_axi_mem_bus_araddr    <= m_axi_mem_bus_ARADDR_INT;
        m_axi_mem_bus_arlen <= r_burst_len_int;
        m_axi_mem_bus_arsize    <= "010";
        m_axi_mem_bus_arburst    <= "01";
        m_axi_mem_bus_arlock    <= '0';
        m_axi_mem_bus_arcache    <= "0011";
        m_axi_mem_bus_arprot    <= "000";
        m_axi_mem_bus_arqos    <= x"0";
        m_axi_mem_bus_aruser    <= "0";
        m_axi_mem_bus_arvalid    <= m_axi_mem_bus_ARVALID_INT;


        r_displ_int <= slv_reg3(31 downto 24);
        w_displ_int <= slv_reg3(23 downto 16);
        r_max_outs_int <= slv_reg3(15 downto 8);
        w_max_outs_int <= slv_reg3(7 downto 0);
        r_phase_int <= slv_reg4(31 downto 16);
        w_phase_int <= slv_reg4(15 downto 0);
        r_base_addr_int <= slv_reg1;
        w_base_addr_int <= slv_reg2;
        r_num_trans_int <= slv_reg5(31 downto 16);
        w_num_trans_int <= slv_reg5(15 downto 0);
        r_burst_len_int <= slv_reg6(31 downto 24);
        w_burst_len_int <= slv_reg6(23 downto 16);
        exec_time_clk <= slv_reg8;

        reset <= slv_reg0(0);
--        global_start <= slv_reg0(1) or ext_trigger;
        global_start <= slv_reg0(1);
        --data_val_flag <= global_start;
        r_start_int <= global_start;
        --w_start_int <= global_start and slv_reg0(3);
        r_misb_clk_cycle <= slv_reg7(31 downto 16);
        w_misb_clk_cycle <= slv_reg7(15 downto 0);

        r_done <= r_done_int;
        w_done <= w_done_int;

        r_done_int <= global_start and and_reduce((r_done_counter xnor r_num_trans_int));
        w_done_int <= global_start and and_reduce((w_done_counter xnor w_num_trans_int));

        w_start_int <= global_start and and_reduce(exec_cnt_int xnor exec_time_clk);


----------------------- SLAVE CONTROL LOGIC  ---------------------------




------------------ END SLAVE CONTROL LOGIC ----------------------------

	-- I/O Connections assignments

	s_axi_ctrl_bus_AWREADY	<= axi_awready;
	s_axi_ctrl_bus_WREADY	<= axi_wready;
	s_axi_ctrl_bus_BRESP	<= axi_bresp;
	s_axi_ctrl_bus_BVALID	<= axi_bvalid;
	s_axi_ctrl_bus_ARREADY	<= axi_arready;
	s_axi_ctrl_bus_RDATA	<= axi_rdata;
	s_axi_ctrl_bus_RRESP	<= axi_rresp;
	s_axi_ctrl_bus_RVALID	<= axi_rvalid;
	process (ap_clk)
	begin
	  if rising_edge(ap_clk) then
	    if ap_rst_n = '0' then
	      axi_awready <= '0';
	      aw_en <= '1';
	    else
	      if (axi_awready = '0' and s_axi_ctrl_bus_AWVALID = '1' and s_axi_ctrl_bus_WVALID = '1' and aw_en = '1') then
	           axi_awready <= '1';
	           aw_en <= '0';
	        elsif (s_axi_ctrl_bus_BREADY = '1' and axi_bvalid = '1') then
	           aw_en <= '1';
	           axi_awready <= '0';
	      else
	        axi_awready <= '0';
	      end if;
	    end if;
	  end if;
	end process;

	process (ap_clk)
	begin
	  if rising_edge(ap_clk) then
	    if ap_rst_n = '0' then
	      axi_awaddr <= (others => '0');
	    else
	      if (axi_awready = '0' and s_axi_ctrl_bus_AWVALID = '1' and s_axi_ctrl_bus_WVALID = '1' and aw_en = '1') then
	        axi_awaddr <= s_axi_ctrl_bus_AWADDR;
	      end if;
	    end if;
	  end if;
	end process;

	process (ap_clk)
	begin
	  if rising_edge(ap_clk) then
	    if ap_rst_n = '0' then
	      axi_wready <= '0';
	    else
	      if (axi_wready = '0' and s_axi_ctrl_bus_WVALID = '1' and s_axi_ctrl_bus_AWVALID = '1' and aw_en = '1') then
	          axi_wready <= '1';
	      else
	        axi_wready <= '0';
	      end if;
	    end if;
	  end if;
	end process;


	slv_reg_wren <= axi_wready and s_axi_ctrl_bus_WVALID and axi_awready and s_axi_ctrl_bus_AWVALID ;

	process (ap_clk)
	variable loc_addr :std_logic_vector(OPT_MEM_ADDR_BITS downto 0);
	begin
	  if rising_edge(ap_clk) then
	    if ap_rst_n = '0' or slv_reg0(0) = '1' then
	      slv_reg0 <= (others => '0');
	      slv_reg1 <= (others => '0');
	      slv_reg2 <= (others => '0');
	      slv_reg3 <= (others => '0');
	      slv_reg4 <= (others => '0');
	      slv_reg5 <= (others => '0');
	      slv_reg6 <= (others => '0');
	      slv_reg7 <= (others => '0');
	      slv_reg8 <= (others => '0');
	      slv_reg9 <= (others => '0');
	      slv_reg10 <= (others => '0');
	      slv_reg11 <= (others => '0');
	      slv_reg12 <= (others => '0');
	      slv_reg13 <= (others => '0');
	      slv_reg14 <= (others => '0');
	      slv_reg15 <= (others => '0');
	    else
	      loc_addr := axi_awaddr(ADDR_LSB + OPT_MEM_ADDR_BITS downto ADDR_LSB);
	      if (slv_reg_wren = '1') then
	        case loc_addr is
	          when b"0000" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg0(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0001" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg1(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0010" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg2(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0011" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg3(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0100" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg4(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0101" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg5(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0110" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg6(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"0111" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg7(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1000" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg8(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1001" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg9(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1010" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg10(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1011" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg11(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1100" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg12(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1101" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg13(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1110" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg14(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when b"1111" =>
	            for byte_index in 0 to (C_S00_AXI_DATA_WIDTH/8-1) loop
	              if ( s_axi_ctrl_bus_WSTRB(byte_index) = '1' ) then
	                slv_reg15(byte_index*8+7 downto byte_index*8) <= s_axi_ctrl_bus_WDATA(byte_index*8+7 downto byte_index*8);
	              end if;
	            end loop;
	          when others =>
	            slv_reg0 <= slv_reg0;
	            slv_reg1 <= slv_reg1;
	            slv_reg2 <= slv_reg2;
	            slv_reg3 <= slv_reg3;
	            slv_reg4 <= slv_reg4;
	            slv_reg5 <= slv_reg5;
	            slv_reg6 <= slv_reg6;
	            slv_reg7 <= slv_reg7;
	            slv_reg8 <= slv_reg8;
	            slv_reg9 <= slv_reg9;
	            slv_reg10 <= slv_reg10;
	            slv_reg11 <= slv_reg11;
	            slv_reg12 <= slv_reg12;
	            slv_reg13 <= slv_reg13;
	            slv_reg14 <= slv_reg14;
	            slv_reg15 <= slv_reg15;
	        end case;
	      end if;
	    end if;
	  end if;
	end process;

	process (ap_clk)
	begin
	  if rising_edge(ap_clk) then
	    if ap_rst_n = '0' then
	      axi_bvalid  <= '0';
	      axi_bresp   <= "00"; --need to work more on the responses
	    else
	      if (axi_awready = '1' and s_axi_ctrl_bus_AWVALID = '1' and axi_wready = '1' and s_axi_ctrl_bus_WVALID = '1' and axi_bvalid = '0'  ) then
	        axi_bvalid <= '1';
	        axi_bresp  <= "00";
	      elsif (s_axi_ctrl_bus_BREADY = '1' and axi_bvalid = '1') then
	        axi_bvalid <= '0';
	      end if;
	    end if;
	  end if;
	end process;


	process (ap_clk)
	begin
	  if rising_edge(ap_clk) then
	    if ap_rst_n = '0' then
	      axi_arready <= '0';
	      axi_araddr  <= (others => '1');
	    else
	      if (axi_arready = '0' and s_axi_ctrl_bus_ARVALID = '1') then
	        axi_arready <= '1';
	        axi_araddr  <= s_axi_ctrl_bus_ARADDR;
	      else
	        axi_arready <= '0';
	      end if;
	    end if;
	  end if;
	end process;


	process (ap_clk)
	begin
	  if rising_edge(ap_clk) then
	    if ap_rst_n = '0' then
	      axi_rvalid <= '0';
	      axi_rresp  <= "00";
	    else
	      if (axi_arready = '1' and s_axi_ctrl_bus_ARVALID = '1' and axi_rvalid = '0') then
	        axi_rvalid <= '1';
	        axi_rresp  <= "00";
	      elsif (axi_rvalid = '1' and s_axi_ctrl_bus_RREADY = '1') then
	        axi_rvalid <= '0';
	      end if;
	    end if;
	  end if;
	end process;


	slv_reg_rden <= axi_arready and s_axi_ctrl_bus_ARVALID and (not axi_rvalid) ;

	process (slv_reg0, slv_reg1, slv_reg2, slv_reg3, slv_reg4, slv_reg5, slv_reg6, slv_reg7, slv_reg8, slv_reg9, slv_reg10, slv_reg11, slv_reg12, slv_reg13, slv_reg14, slv_reg15, axi_araddr, ap_rst_n, slv_reg_rden)
	variable loc_addr :std_logic_vector(OPT_MEM_ADDR_BITS downto 0);
	begin

	    loc_addr := axi_araddr(ADDR_LSB + OPT_MEM_ADDR_BITS downto ADDR_LSB);
	    case loc_addr is
	      when b"0000" =>
	        reg_data_out <= slv_reg0;
	      when b"0001" =>
	        reg_data_out <= slv_reg1;
	      when b"0010" =>
	        reg_data_out <= slv_reg2;
	      when b"0011" =>
	        reg_data_out <= slv_reg3;
	      when b"0100" =>
	        reg_data_out <= slv_reg4;
	      when b"0101" =>
	        reg_data_out <= slv_reg5;
	      when b"0110" =>
	        reg_data_out <= slv_reg6;
	      when b"0111" =>
	        reg_data_out <= slv_reg7;
	      when b"1000" =>
	        reg_data_out <= slv_reg8;
	      when b"1001" =>
	        reg_data_out <= slv_reg9;
	      when b"1010" =>
	        reg_data_out <= slv_reg10;
	      when b"1011" =>
	        reg_data_out <= slv_reg11;
	      when b"1100" =>
	        reg_data_out <= slv_reg12;
	      when b"1101" =>
	        reg_data_out <= slv_reg13;
	      when b"1110" =>
	        reg_data_out <= slv_reg14;
	      when b"1111" =>
	        reg_data_out <= slv_reg15;
	      when others =>
	        reg_data_out  <= (others => '0');
	    end case;
	end process;


	process(ap_clk) is
	begin
	  if (rising_edge(ap_clk)) then
	    if ( ap_rst_n = '0' ) then
	      axi_rdata  <= (others => '0');
	    else
	      if (slv_reg_rden = '1') then
	          axi_rdata <= reg_data_out;
	      end if;
	    end if;
	  end if;
	end process;

----------------- MASTER MACHINE LOGIC ----------------------

--data_set_process: process(ap_clk)
--begin
--    if(reset = '1' or ap_rst_n = '0') then
--        r_displ_int <= (others => '0');
--        w_displ_int <= (others => '0');
--        r_max_outs_int <= (others => '0');
--        w_max_outs_int <= (others => '0');
--        r_phase_int <= (others => '0');
--        w_phase_int <= (others => '0');
--        r_base_addr_int <= (others => '0');
--        w_base_addr_int <= (others => '0');
--        r_num_trans_int <= (others => '0');
--        w_num_trans_int <= (others => '0');
--        r_burst_len_int <= (others => '0');
--        w_burst_len_int <= (others => '0');
--        data_val_flag <= '0';
--    elsif(rising_edge(ap_clk)) then
--        if(data_val = '1') then
--            r_displ_int <= r_displ;
--            w_displ_int <= w_displ;
--            r_max_outs_int <= r_max_outs;
--            w_max_outs_int <= w_max_outs;
--            r_phase_int <= r_phase;
--            w_phase_int <= w_phase;
--            r_base_addr_int <= r_base_addr;
--            w_base_addr_int <= w_base_addr;
--            r_num_trans_int <= r_num_trans;
--            w_num_trans_int <= w_num_trans;
--            data_val_flag <= '1';
--            r_burst_len_int <= r_burst_len;
--            w_burst_len_int <= w_burst_len;
--        end if;
--    end if;
--end process data_set_process;

--start_proc: process(ap_clk)
--begin
--    if(reset = '1' or ap_rst_n = '0') then
--        r_phase_start <= '0';
--        w_phase_start <= '0';
--    elsif(rising_edge(ap_clk)) then
--        if(r_start = '1') then
--            r_phase_start <= '1';
--        end if;

--        if(w_start = '1') then
--            w_phase_start <= '1';
--        end if;
--    end if;

--end process;

--phase_proc: process(ap_clk)
--begin
--    if(reset = '1' or ap_rst_n = '0') then
--        r_phase_counter <= (others => '0');
--        w_phase_counter <= (others => '0');
--        --w_start_int <= '0';
--        r_start_int <= '0';
--    elsif(rising_edge(ap_clk)) then
--        if(r_phase_start = '1') then
--            if(r_phase_counter /= r_phase_int) then
--                r_phase_counter <= r_phase_counter + 1;
--            else
--                r_start_int <= '1';
--            end if;
--        end if;

----        if(w_phase_start = '1') then
----            if(w_phase_counter /= w_phase_int) then
----                w_phase_counter <= w_phase_counter + 1;
----            else
----                w_start_int <= '1';
----            end if;
----        end if;
--    end if;
--end process;

exec_proc : process(ap_clk)
begin
    if(reset = '1' or ap_rst_n = '0') then
        exec_cnt_int <= (others => '0');
    elsif(rising_edge(ap_clk)) then
        if(w_start_int = '0' and r_done_int = '1') then
            exec_cnt_int <= exec_cnt_int + 1;
        end if;
    end if;
end process;



--done_proc : process(ap_clk)
--begin

--    if(rising_edge(ap_clk)) then

--        if(data_val_flag = '1') then
--            if(r_done_counter = r_num_trans_int) then
--                r_done <= '1';
--            end if;

--            if(w_done_counter = w_num_trans_int) then
--                w_done <= '1';
--            end if;
--        else
--            r_done <= '0';
--            w_done <= '0';
--        end if;
--    end if;
--end process done_proc;

address_read_channel_process: process(ap_clk)
begin
    if(reset = '1' or ap_rst_n = '0') then
        r_trans_counter <= (others => '0');
        r_displ_cnt <= (others => '0');
        r_pend_outs_trans <= (others => '0');
        r_grant_trans_flag <= "0";
        r_addr_init <= '0';
    elsif(rising_edge(ap_clk)) then

        r_grant_trans_flag <= "0";
        r_pend_outs_trans <= r_pend_outs_trans + r_grant_trans_flag - r_end_trans_flag;

        if(r_start_int = '1') then
            if(r_state = "000") then -- putting a new address on the bus
                if( (r_trans_counter < r_num_trans_int) and (r_pend_outs_trans < r_max_outs_int) ) then
                    if(r_addr_init = '0') then
                        m_axi_mem_bus_ARADDR_INT <= r_base_addr_int;
                        r_addr_init <= '1';
                    else
                        m_axi_mem_bus_ARADDR_INT <= m_axi_mem_bus_ARADDR_INT + ((r_burst_len_int(5 downto 0)+1) & "00");
                    end if;
                    m_axi_mem_bus_ARVALID_INT <= '1';
                    r_state <= "001";
                    r_trans_counter <= r_trans_counter + 1;
                    r_grant_trans_flag <= "1";
                else
                    m_axi_mem_bus_ARVALID_INT <= '0';
                end if;
            elsif(r_state = "001") then -- waiting for the address to be granted
                if(m_axi_mem_bus_ARVALID_INT = '1' and m_axi_mem_bus_arready = '1') then
                    if(r_displ_cnt < r_displ_int) then
                        r_displ_cnt <= r_displ_cnt + 1;
                        r_state <= "010"; -- to displacement
                    else
                        r_state <= "000"; --ready for new address
                    end if;
                    m_axi_mem_bus_ARVALID_INT <= '0';
                end if;
            elsif(r_state = "010") then
                if(r_displ_cnt = r_displ_int) then
                    r_displ_cnt <= (others => '0');
                    r_state <= "000";
                else
                    r_displ_cnt <= r_displ_cnt + 1;
                end if;
           else
                m_axi_mem_bus_ARVALID_INT <= '0';
           end if;
        end if;
    end if;
end process address_read_channel_process;

address_write_channel_process: process(ap_clk)
begin
    if(reset = '1' or ap_rst_n = '0') then
        w_trans_counter <= (others => '0');
        w_displ_cnt <= (others => '0');
        w_grant_trans_flag <= "0";
        w_addr_init <= '0';
    elsif(rising_edge(ap_clk)) then

        w_grant_trans_flag <= "0";
        w_pend_outs_trans <= w_pend_outs_trans + w_grant_trans_flag - w_end_trans_flag;

        if(w_start_int = '1') then
            if(w_state = "000") then -- putting a new address on the bus
                if( (w_trans_counter < w_num_trans_int) and (w_pend_outs_trans < w_max_outs_int) ) then
                    if(w_addr_init = '0') then
                        m_axi_mem_bus_AWADDR_INT <= w_base_addr_int;
                        w_addr_init <= '1';
                    else
                        m_axi_mem_bus_AWADDR_INT <= m_axi_mem_bus_AWADDR_INT + ((w_burst_len_int(5 downto 0)+1) & "00");
                    end if;
                    m_axi_mem_bus_AWVALID_INT <= '1';
                    w_state <= "001";
                    -- COMMENTED FOLLOWING LINE 30/05/2020 MEM ACCESS PROJECT
                    --w_trans_counter <= w_trans_counter + 1;
                    w_grant_trans_flag <= "1";
                else
                    m_axi_mem_bus_AWVALID_INT <= '0';
                end if;
            elsif(w_state = "001") then -- waiting for the address to be granted
                if(m_axi_mem_bus_AWVALID_INT = '1' and m_axi_mem_bus_awready = '1') then
                    -- ADDED THE FOLLWING LINE 30/05/2020 MEM ACCESS PROJECT
                    w_trans_counter <= w_trans_counter + 1;
                    if(w_displ_cnt < w_displ_int) then
                        w_displ_cnt <= w_displ_cnt + 1;
                        w_state <= "010"; -- to displacement
                    else
                        w_state <= "000"; --ready for new address
                    end if;
                    m_axi_mem_bus_AWVALID_INT <= '0';
                end if;
            elsif(w_state = "010") then
                if(w_displ_cnt = w_displ_int) then
                    w_displ_cnt <= (others => '0');
                    w_state <= "000";
                else
                    w_displ_cnt <= w_displ_cnt + 1;
                end if;
            else
                m_axi_mem_bus_AWVALID_INT <= '0';
            end if;
        end if;
    end if;

end process address_write_channel_process;

--TODO Extension to single transactions

r_ready_to_sample <= and_reduce(r_misb_clk_cnt xnor r_misb_clk_cycle);

m_axi_mem_bus_rready <= r_ready_to_sample;

data_read_channel: process(ap_clk)
begin
    if(reset = '1' or ap_rst_n = '0') then -- reset the data read channel
        r_done_counter <= (others => '0');
        r_misb_clk_cnt <= (others => '0');
    elsif(rising_edge(ap_clk)) then
       if(m_axi_mem_bus_rvalid = '1' and r_ready_to_sample = '1' and m_axi_mem_bus_rlast = '1') then
            r_done_counter <= r_done_counter + 1;
            r_end_trans_flag <= "1";
            r_misb_clk_cnt <= (others => '0');
        elsif(m_axi_mem_bus_rvalid = '1' and r_ready_to_sample = '0') then
            r_misb_clk_cnt <= r_misb_clk_cnt + 1;
        else
            r_end_trans_flag <= "0";
        end if;
    end if;

end process data_read_channel;

--ready_to_write_flag <= and_reduce(w_misb_clk_cnt xnor w_misb_clk_cycle);

--data_delay_channel: process(ap_clk)
--begin
--    if(reset = '1') then
--        w_misb_clk_cnt <= (others => '0');
--    elsif(rising_edge(ap_clk)) then
--        if((w_data_done_cnt < w_trans_counter) and ready_to_write_flag = '0') then
--            w_misb_clk_cnt <= w_misb_clk_cnt + 1;
--            if()
--        else
--            w_misb_clk_cnt <= (others => '0');
--        end if;

--    end if;
--end process;

data_write_channel: process(ap_clk)
begin

    if(reset = '1' or ap_rst_n = '0') then
        w_data_done_cnt <= (others => '0');
        w_data_counter <= x"01";
        m_axi_mem_bus_WDATA_INT <= x"00000000";
    elsif(rising_edge(ap_clk)) then
        if(w_data_done_cnt < w_trans_counter) then
            m_axi_mem_bus_WVALID_INT <= '1';
        else
            m_axi_mem_bus_WVALID_INT <= '0';
        end if;

       -- if(w_burst_len_int = x"00") then
       --     m_axi_mem_bus_WLAST_INT <= '1';
       -- end if;

        if(m_axi_mem_bus_WVALID_INT = '1' and m_axi_mem_bus_wready = '1') then
            if(w_data_counter = w_burst_len_int) then
                m_axi_mem_bus_WLAST_INT <= '1';
                w_data_counter <= x"00";
                w_data_done_cnt <= w_data_done_cnt + 1;
            else
                w_data_counter <= w_data_counter + 1;
                m_axi_mem_bus_WLAST_INT <= '0';
            end if;
            m_axi_mem_bus_WDATA_INT <= x"000000" & w_data_counter;
        end if;
   end if;

end process data_write_channel;

b_resp_channel: process(ap_clk)
begin
    if(reset = '1' or ap_rst_n = '0') then -- reset the data read channel
        w_done_counter <= (others => '0');
    elsif(rising_edge(ap_clk)) then
        if(m_axi_mem_bus_bvalid = '1') then
            w_done_counter <= w_done_counter + 1;
            w_end_trans_flag <= "1";
        else
            w_end_trans_flag <= "0";
        end if;

    end if;

end process b_resp_channel;

------------------------ END MASTER MACHINE LOGIC -------------------------------------

m_axi_mem_bus_AWREGION <= (others => '0');
m_axi_mem_bus_ARREGION <= (others => '0');
m_axi_mem_bus_WID <= (others => '0');
interrupt <= w_done_int;


end arch_imp;
