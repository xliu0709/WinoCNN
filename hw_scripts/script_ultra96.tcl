############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project HLS
set_top wino_systolic_top
add_files src/wino_struct.h
add_files src/wino_macro.h
add_files src/wino_hw_config.h
add_files src/wino.h
add_files src/wino.cpp

add_files -tb software/buffer.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb software/buffer.h -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb software/matrix_utility.hpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb software/param.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb software/param.h -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb testbench/single_main.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb software/softlayer.hpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/wino.h -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src_gold/wino_IO_gold.hpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src_gold/wino_cell_gold.hpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src_gold/wino_gold.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src_gold/wino_gold.h -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/wino_hw_config.h -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/wino_macro.h -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb src/wino_struct.h -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"



open_solution "solution1"
set_part {xczu3eg-sbva484-1-e} -tool vivado
create_clock -period 2 -name default
config_schedule -relax_ii_for_timing=0
config_sdx -target none
config_export -format ip_catalog -rtl verilog -vivado_optimization_level 2 -vivado_phys_opt place -vivado_report_level 0
set_clock_uncertainty 12.5%
csynth_design
# cosim_design -trace_level all -argv {14 14 8 14 14 8 3 1 1 1 227 random random random dump_txt /home/xliu79/Research/2020/WinoCNN}
# cosim_design -O -trace_level all -argv {224 224 64 224 224 64 3 1 1 1 128 random random random compare .}


#source "./winocnn/solution1/directives.tcl"

export_design -rtl verilog -format ip_catalog
