#
# build.tcl: re-creating a Vivado project
#
#*****************************************************************************************
package require fileutil

################################################################
# Check if the script is running with a tested Vivado version.
################################################################
set current_vivado_version [version -short]

# complete here with all supported vivado version
List<String> supported_vivado_version = new ArrayList<String>();
list.add("2019.2");
list.add("2018.3");
list.add("2020.2");

if { [list.contains($current_vivado_version)} {
   puts ""
   puts "WARNING: this script was not tested with Vivado version $current_vivado_version"
}


# set scripts_vivado_version 2019.2

# if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
#    puts ""
#    catch {common::send_msg_id "BD_TCL-109" "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

#    return 1
# }

################################################################
# Check required environment variables.
################################################################
puts "========================"
puts "Checking inputs ..."
puts "========================"

if { ![info exists env(VIVADO_DESIGN_NAME)] } {
    puts "ERROR: Please set the environment variable VIVADO_DESIGN_NAME before running the script"
    return
}
set design_name $::env(VIVADO_DESIGN_NAME)
puts "Using design name: ${design_name}"

if { ![info exists env(VIVADO_TOP_NAME)] } {
    puts "WARNING: No top design defined. Using the default top name ${design_name}_wrapper"
    set top_name ${design_name}_wrapper
} else {
  set top_name $::env(VIVADO_TOP_NAME)
  puts "Using top name: ${top_name}"
}

# check the command 'launch_runs' learn more
# the valid steps are:
#  - opt_design, power_opt_design, place_design, route_design, phys_opt_design, and write_bitstream
if { ![info exists env(VIVADO_SYN_STEP)] } {
    puts "WARNING: the synthesis step is not defined. Using the default 'place_design'"
    set syntesis_step place_design
} else {
  set syntesis_step $::env(VIVADO_SYN_STEP)
  puts "Using synthesis step: ${syntesis_step}"
}

# the supported board as pynq and zynq
if { ![info exists env(DART_BOARD)] } {
    puts "WARNING: the FPGA board is not defined. Using the default 'pynq'"
    set dart_board place_design
} else {
  set dart_board $::env(DART_BOARD)
  # checking for the supported boards
  if {$dart_board != "pynq" && $dart_board != "zynq"} {
    puts "ERROR: Please set the environment variable DART_BOARD with one of the supported boards: pynq or zynq"
    return 1
  } 
  puts "Using FPGA board: ${dart_board}"
}


################################################################
# Creating a clean project.
################################################################
# Set the reference directory for source file relative paths (by default the value is script directory path)
puts "========================"
puts "Creating the design ..."
puts "========================"

set origin_dir "."

# Set the directory path for the original project from where this script was exported
set orig_proj_dir "[file normalize "$origin_dir/vivado/$design_name"]"

## Delete log and journal
set file_list [glob -nocomplain "vivado.*"]
if {[llength $file_list] != 0} {
  file delete -force {*}$file_list
}
set file_list [glob -nocomplain "vivado_*"]
if {[llength $file_list] != 0} {
  file delete -force {*}$file_list
}

if {[file exist ".Xil"]} {
  file delete -force .Xil
}

# Create a fresh project
file delete -force ${orig_proj_dir}
file mkdir ${orig_proj_dir}

################################################################
# Create the project and set the board. Default is PYNQ
################################################################
if {$dart_board == "pynq"} {
  set part_name xc7z020clg400-1
  create_project -force  $design_name $orig_proj_dir -part $part_name
  set_property BOARD_PART www.digilentinc.com:pynq-z1:part0:1.0 [current_project]
} elseif {$extension == "zynq"} {
  set part_name blabla
  puts "WARNING: the zynq support is under development!"
  return 1
} 

# Set the directory path for the new project
set proj_dir [get_property directory [current_project]]


################################################################
# Set path to FRED and custom IPs
################################################################
set_property  ip_repo_paths ./hw/ips [current_project]
update_ip_catalog


# Create 'sources_1' fileset (if not found)
if {[string equal [get_filesets -quiet sources_1] ""]} {
  create_fileset -srcset sources_1
}


# Set 'sources_1' fileset properties
set obj [get_filesets sources_1]
set_property "top" "${top_name}" $obj

# Insert all the vhdl, sv, and verilog source files from ./hw/hdl into the project
# find recursively all files with extensions vhd, v, sv 
set hdl_files [fileutil::findByPattern $origin_dir/hw/hdl/ -glob {*.vhd *.v *.sv}]
#set hdl_files [glob -nocomplain -directory $origin_dir/hw/hdl/ *{*.vhd,*.v,*.sv}*]
puts $hdl_files

foreach hdl_file $hdl_files {
  set file "[file normalize "$hdl_file"]"
  add_files -quiet -fileset sources_1 $file
  set file_obj [get_files -of_objects [get_filesets sources_1] $file]
  set extension [string tolower [file extension $file]]
  if {$extension == ".vhd"} {
    set hdl_type "VHDL"
    # this property is only valid for vhdl
    set_property -name "library" -value "work" -objects $file_obj
  } elseif {$extension == ".v"} {
    set hdl_type "Verilog"
  } elseif {$extension == ".sv"} {
    set hdl_type "SystemVerilog"
  } else {
    puts "ERROR: HDL extension $extension is not supported"
    return 1
  }
  set_property -name "file_type" -value $hdl_type -objects $file_obj
}

# Import IP-XACT config files if they exist
#  There are two situations where there will be subdirs inside hw/ips/:
#    - this project is an IP project: In this case, the component.xml file 
#      will be in hw/ips/<ip-name>/component.xml and this search will find 
#      the xml files
#    - this project is using a custom IP: In this case, the subdirs inside 
#      hw/ips/<ip-name> will be a git submodule of an IP repo. This structure 
#      is required when the command 
#          'generate_target all [get_files ${design_name}.bd]'
#      is executed to regenerate the IP's output products
set ip_files [glob -nocomplain -directory $origin_dir/hw/ips/ "**/*.xml"]
foreach ip_file $ip_files {
  set file "[file normalize "$ip_file"]"
  add_files -quiet -fileset sources_1 $file
  set file_obj [get_files -of_objects [get_filesets sources_1] $file]
  set_property -name "file_type" -value "IP-XACT" -objects $file_obj
}


# Create 'constrs_1' fileset (if not found)
if {[string equal [get_filesets -quiet constrs_1] ""]} {
  create_fileset -constrset constrs_1
}

# Set 'constrs_1' fileset object
set obj [get_filesets constrs_1]

# Add/Import constrs file and set constrs file properties
set constr_files [glob -nocomplain -directory $origin_dir/hw/xdc/ "*.xdc"]

foreach constr_file $constr_files {
  set file "[file normalize "$constr_file"]"
  add_files -quiet -fileset $obj [list $file]
  set file_obj [get_files -of_objects [get_filesets constrs_1] $file]
  set_property -name "file_type" -value "XDC" -objects $file_obj

  # Set 'constrs_1' fileset properties
  set obj [get_filesets constrs_1]
  set_property -name "target_constrs_file" -value "[get_files $file]" -objects $obj
  set_property -name "target_ucf" -value "[get_files $file]" -objects $obj
}


# Create 'sim_1' fileset (if not found)
if {[string equal [get_filesets -quiet sim_1] ""]} {
  create_fileset -simset sim_1
}

# Set 'sim_1' fileset object
# Import testbenches, waveform files, etc if they exist
set obj [get_filesets sim_1]
# find recursively all files with extensions vhd, v, sv 
set sim_files [fileutil::findByPattern $origin_dir/hw/sim/ -glob {*.vhd *.v *.sv}]

# if there is a testbench, then add few more properties 
if {[llength $sim_files] > 0} {
  set_property -name "sim_mode" -value "post-implementation" -objects $obj
  #TODO assuming the testbench name is tb. is it possible to find it out automatically ?
  set_property -name "top" -value "tb" -objects $obj
  set_property -name "top_lib" -value "xil_defaultlib" -objects $obj

  foreach sim_file $sim_files {
    set file "[file normalize "$sim_file"]"
    add_files -quiet -fileset sim_1 $file
    set file_obj [get_files -of_objects [get_filesets sim_1] $file]
    set extension [string tolower [file extension $file]]
    if {$extension == ".vhd"} {
      set hdl_type "VHDL"
    } elseif {$extension == ".v"} {
      set hdl_type "Verilog"
    } elseif {$extension == ".sv"} {
      set hdl_type "SystemVerilog"
    } else {
      puts "ERROR: HDL extension $extension is not supported"
      return 1
    }
    set_property -name "file_type" -value $hdl_type -objects $file_obj
    #TODO can i replace the lib by work ?
    set_property -name "library" -value "work" -objects $file_obj
  }
}

# waveform files are simply added to the project. no property is set
set wcfg_files [glob -nocomplain -directory $origin_dir/hw/sim "*.wcfg"]
add_files -quiet -fileset sim_1 $wcfg_files


# Create 'synth_1' run (if not found)
if {[string equal [get_runs -quiet synth_1] ""]} {
  create_run -name synth_1 -part $part_name -flow {Vivado Synthesis 2018} -strategy "Vivado Synthesis Defaults" -constrset constrs_1
} else {
  set_property strategy "Vivado Synthesis Defaults" [get_runs synth_1]
  set_property flow "Vivado Synthesis 2018" [get_runs synth_1]
}
set obj [get_runs synth_1]

# set the current synth run
current_run -synthesis [get_runs synth_1]

# Create 'impl_1' run (if not found)
if {[string equal [get_runs -quiet impl_1] ""]} {
  create_run -name impl_1 -part $part_name -flow {Vivado Implementation 2018} -strategy "Vivado Implementation Defaults" -constrset constrs_1 -parent_run synth_1
} else {
  set_property strategy "Vivado Implementation Defaults" [get_runs impl_1]
  set_property flow "Vivado Implementation 2018" [get_runs impl_1]
}
set obj [get_runs impl_1]
set_property -name "steps.write_bitstream.args.readback_file" -value "0" -objects $obj
set_property -name "steps.write_bitstream.args.verbose" -value "0" -objects $obj

# set the current impl run
current_run -implementation [get_runs impl_1]

puts "INFO: Project created:${design_name}"

# Prepare to create a block design
# CHECKING IF PROJECT EXISTS
if { [get_projects -quiet] eq "" } {
   puts "ERROR: Please open or create a project!"
   return 1
}


# Find the block tcl script and create the block design
set block_files [glob -nocomplain -directory $origin_dir/hw/bd/ "*.tcl"]

if {[llength $block_files] == 1} {
  # Create and empty block design
  create_bd_design $design_name
  current_bd_design $design_name
  source $block_files
  create_root_design ""

  # Generate the wrapper
  make_wrapper -files [get_files *${design_name}.bd] -top
  # It is asuming that if there is a block file, its wrapper will be the top 
  add_files -norecurse ./vivado/${design_name}/${design_name}.srcs/sources_1/bd/${design_name}/hdl/${design_name}_wrapper.v
} elseif {[llength $block_files] > 1} {
  puts "ERROR: multiple block files found. The script only supports one block per design"
  return 1
}

# Update the compile order
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

# If this design is block-basedm then ensure parameter propagation has been performed
if {[llength $block_files] == 1} {
  close_bd_design [current_bd_design]
  open_bd_design [get_files ${design_name}.bd]
  validate_bd_design -force
  # generate target for IPs, includig the custom IPs
  generate_target all [get_files ${design_name}.bd]
  save_bd_design
}

puts "========================"
puts "Running synthesis ..."
puts "========================"
# perform the synthesis and/or generate the bitstream
open_project ./vivado/${design_name}/${design_name}.xpr
update_compile_order -fileset sources_1
reset_run -quiet impl_1
launch_runs impl_1 -to_step $syntesis_step -jobs 8
wait_on_run impl_1

open_run synth_1 -name synth_1
puts "========================"
puts "Generating reports ..."
puts "========================"
report_timing_summary > timing.rpt
report_utilization > utilization.rpt
report_power > power.rpt

close_design -quiet
