create_project RTL ./RTL -part xczu9eg-ffvb1156-2-e
set_property board_part xilinx.com:zcu102:part0:3.3 [current_project]

create_bd_design "design_1"

create_bd_cell -type ip -vlnv xilinx.com:ip:zynq_ultra_ps_e:3.3 zynq_ultra_ps_e_0
apply_bd_automation -rule xilinx.com:bd_rule:zynq_ultra_ps_e -config {apply_board_preset "1" }  [get_bd_cells zynq_ultra_ps_e_0]
set_property -dict [list CONFIG.PSU__USE__M_AXI_GP1 {0} CONFIG.PSU__USE__S_AXI_GP2 {1} CONFIG.PSU__USE__S_AXI_GP3 {1} CONFIG.PSU__USE__S_AXI_GP4 {1} CONFIG.PSU__USE__S_AXI_GP5 {1} CONFIG.PSU__USE__IRQ0 {0} CONFIG.PSU__CRL_APB__PL0_REF_CTRL__FREQMHZ {200}] [get_bd_cells zynq_ultra_ps_e_0]


set_property  ip_repo_paths  ./HLS [current_project]
update_ip_catalog


create_bd_cell -type ip -vlnv xilinx.com:hls:wino_systolic_top:1.0 wino_systolic_top_0

apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/zynq_ultra_ps_e_0/M_AXI_HPM0_FPD} Slave {/wino_systolic_top_0/s_axi_AXILiteS} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins wino_systolic_top_0/s_axi_AXILiteS]

apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/zynq_ultra_ps_e_0/pl_clk0 } Clk_slave {Auto} Clk_xbar {Auto} Master {/wino_systolic_top_0/m_axi_input_DDR0} Slave {/zynq_ultra_ps_e_0/S_AXI_HP0_FPD} intc_ip {Auto} master_apm {0}}  [get_bd_intf_pins zynq_ultra_ps_e_0/S_AXI_HP0_FPD]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/zynq_ultra_ps_e_0/pl_clk0 } Clk_slave {Auto} Clk_xbar {Auto} Master {/wino_systolic_top_0/m_axi_input_DDR1} Slave {/zynq_ultra_ps_e_0/S_AXI_HP1_FPD} intc_ip {Auto} master_apm {0}}  [get_bd_intf_pins zynq_ultra_ps_e_0/S_AXI_HP1_FPD]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/zynq_ultra_ps_e_0/pl_clk0 } Clk_slave {Auto} Clk_xbar {Auto} Master {/wino_systolic_top_0/m_axi_output_DDR0} Slave {/zynq_ultra_ps_e_0/S_AXI_HP2_FPD} intc_ip {Auto} master_apm {0}}  [get_bd_intf_pins zynq_ultra_ps_e_0/S_AXI_HP2_FPD]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/zynq_ultra_ps_e_0/pl_clk0 } Clk_slave {Auto} Clk_xbar {Auto} Master {/wino_systolic_top_0/m_axi_output_DDR1} Slave {/zynq_ultra_ps_e_0/S_AXI_HP3_FPD} intc_ip {Auto} master_apm {0}}  [get_bd_intf_pins zynq_ultra_ps_e_0/S_AXI_HP3_FPD]

apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/zynq_ultra_ps_e_0/pl_clk0 } Clk_slave {/zynq_ultra_ps_e_0/pl_clk0 } Clk_xbar {/zynq_ultra_ps_e_0/pl_clk0 } Master {/wino_systolic_top_0/m_axi_weight_DDR0} Slave {/zynq_ultra_ps_e_0/S_AXI_HP0_FPD} intc_ip {/axi_smc} master_apm {0}}  [get_bd_intf_pins wino_systolic_top_0/m_axi_weight_DDR0]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/zynq_ultra_ps_e_0/pl_clk0 } Clk_slave {/zynq_ultra_ps_e_0/pl_clk0 } Clk_xbar {/zynq_ultra_ps_e_0/pl_clk0 } Master {/wino_systolic_top_0/m_axi_weight_DDR1} Slave {/zynq_ultra_ps_e_0/S_AXI_HP1_FPD} intc_ip {/axi_smc} master_apm {0}}  [get_bd_intf_pins wino_systolic_top_0/m_axi_weight_DDR1]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/zynq_ultra_ps_e_0/pl_clk0 } Clk_slave {/zynq_ultra_ps_e_0/pl_clk0 } Clk_xbar {/zynq_ultra_ps_e_0/pl_clk0 } Master {/wino_systolic_top_0/m_axi_weight_DDR2} Slave {/zynq_ultra_ps_e_0/S_AXI_HP2_FPD} intc_ip {/axi_smc} master_apm {0}}  [get_bd_intf_pins wino_systolic_top_0/m_axi_weight_DDR2]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/zynq_ultra_ps_e_0/pl_clk0 } Clk_slave {/zynq_ultra_ps_e_0/pl_clk0 } Clk_xbar {/zynq_ultra_ps_e_0/pl_clk0 } Master {/wino_systolic_top_0/m_axi_weight_DDR3} Slave {/zynq_ultra_ps_e_0/S_AXI_HP3_FPD} intc_ip {/axi_smc} master_apm {0}}  [get_bd_intf_pins wino_systolic_top_0/m_axi_weight_DDR3]

apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/zynq_ultra_ps_e_0/pl_clk0 } Clk_slave {/zynq_ultra_ps_e_0/pl_clk0 } Clk_xbar {/zynq_ultra_ps_e_0/pl_clk0 } Master {/wino_systolic_top_0/m_axi_mem_params} Slave {/zynq_ultra_ps_e_0/S_AXI_HP2_FPD} intc_ip {/axi_smc_2} master_apm {0}}  [get_bd_intf_pins wino_systolic_top_0/m_axi_mem_params]


make_wrapper -files [get_files ./RTL/RTL.srcs/sources_1/bd/design_1/design_1.bd] -top
add_files -norecurse ./RTL/RTL.srcs/sources_1/bd/design_1/hdl/design_1_wrapper.v


launch_runs impl_1 -to_step write_bitstream -jobs 16
wait_on_run impl_1