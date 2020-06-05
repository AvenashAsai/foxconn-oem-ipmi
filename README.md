# Commands
Note the summary about the following commands:

System Information Command Set:
Hardware Configuration - Not Implemented
Firmware Version - In-Progress

Network Function Command Set:
MAC Address - In-Progress
Network Function - Implemented

Fan Control Command Set:
FAN Debug Message - Not Implemented
Fan Get Fan Speed - Not Implemented
Fan Set Duty Cycle - Not Implemented
Fan FSC Control - Not Implemented

CPLD Control Command Set - Not Implemented

Bus Control Command Set - Not Implemented

File/Firmware Command Set - Not Implemented

Miscellaneous Command Set:
KVM Screenshot - Not Implemented

BIOS Information Command Set - Not Implemented

## System Information Command Set

### Hardware Configuration (NetFn 0x34 Cmd 0x00)
The system can be reconfigured by using this command, it would depend on the hardware configuration. If the hardware didn't install in the target system, it would return the 0x10 and means the hardware didn't supported.

Request Data \
Byte 1 – Select the device 
>  01 : NCSI device \
>  02 : VGA device \
Byte 2 – select the mode for this device 
>  NCSI – 
>>    00 : Get the current configuration \
>>    01 : onboard interface \
>>    02 : OCP3.0 \
>  VGA – 
>>    00 : Get the current configuration \
>>    01 : Rear end VGA port \
>>    02 : Front end VGA port
    
Response Data \
Byte 1 – Completion Code 
>  00 : No error \
>  10 : Hardware didn’t support. \
>  C0 : There is some error happened. \
Byte 2 – configuration device 
>  01 : NCSI device \
>  02 : VGA device \
Byte 3 – device mode 
>  NCSI 
>>    01 : onboard interface \
>>    02 : OCP3.0 \
>  VGA 
>>    01 : Rear end VGA port \
>>    02 : Front end VGA port
    
### Get Firmware Version (NetFn 0x34 Cmd 0x01)
This command provides the version from the target system to the user. Hardware device aare listed, if you couldn't find the device, then it didn't support in current firmware.

Request Data \
Byte 1 – Firmware type 
>  01 : BMC firmware version \
>  02 : BIOS firmware version \
>  03 : CPLD firmware version \
>  04 : Motherboard version \
>  05 : PSU firmware version \
>  06 : FRU VPD version \
Byte 2 – Index of Firmware type 
>  CPLD :
>>   0x01 à MB \
>>   0x02 à MP \
>  PSU : index of PSU \
>  FRU : index of FRU as display on ipmitool fru

Response Data \
Byte 1 – Completion Code 
>  00 : No error \
>  10 : Device didn’t support \
>  C0 : There is some error happened. \
Byte 2 – Firmware Type 
>  01 : BMC firmware version \
>  02 : BIOS firmware version \
>  03 : CPLD firmware version \
>  04 : Motherboard version \
>  05 : PSU firmware version \
>  06 : FRU VPD version \
Byte 3 : Byte number of firmware version \
Byte 4:N – Firmware version (1~3 bytes) \
Byte N:M – base on firmware type

## Network Function Command Set

### MAC Address Command (NetFn 0x34 Cmd 0x10)
BMC reads the base MAC address from the FRU EEPROM device, and the FRU has its own format that follows the IPMI FRU specification. BMC calculates the MAC address offset dynamic and calculates the checksum save it to the IPMI FRU data format.

Request Data \
Byte 1 – [0:1] -- Get or Set MAC address 
>  00 : get the MAC address \
>  01 : set the MAC address \
Byte 2:7 - MAC address

Response Data \
Byte 1 – Completion Code 
>  00 : No error \
>  21 : Fru data error \
>  C0 : There is some error happened. \
Byte 2:7 – MAC address

### Network Function Command (NetFn 0x34 Cmd 0x11)
There are several network functions that supports via BMC, and this command provides the capacity to disable or enable these functions. These functions include bonding, https, SSH, telnet, SOL & Redfish.

Bonding Function
There are two ethernet interfaces within the BMC chip, there is also a function that can be bonding two ethernet interfaces into one ethernet interface. This command allows the user to enable or disable the function.

Request Data \
Byte 1 – configuration \
0:3 
>  0 : set the network function \
>  1 : get the network function \
4:7 
>  0 : disable the network function \
>  1 : enable the network function \
Byte 2 – Network function 
>  0x01 : bonding function \
>  0x02 : http/https function \
>  0x04 : SSH function \
>  0x08 : Telnet function \
>  0x10 : SOL function \
>  0x20 : Redfish function \
>  0x40 : Ping function
  
Response Data \
Byte 1 – Completion Code 
>  0x00 : No error \
>  0x21 : Fru data error \
>  0xC0 : There is some error happened. \
Byte 2 
>  1 – the network function is enable \
>  0 – the network function is disable.

## Fan Control Command Set
### Fan Debug Message (NetFn 0x34 Cmd 0x20)
This IPMI OEM command could be enabled or disabled the FSC debug message for testing only. When enable the debug message, the FSC control would stop and all FAN functions could control manually. When individual item’s debug message is enabled, the full debug message would be disabled and only display the debug message for the enabled item.

Request Data  \
Byte 1 – display the type of debug message 
>  00 : display all FSC debug message \
>  01 : display PWM debug message \
>  02 : display PID debug message \
>  03 : display MIX debug message \
>  04 : display sensor debug message \
>  05 : display FAN run table debug message \
>  06 : display OEM CMD \
>  FF : enable FAN manual control \
Byte 2 – switch to enable or disable 
>  00 : disable debug message \
>  01 : enable debug message

Response Data  \
Byte 1 Completion Code 
>  00 : Normal \
>  C0 : Node busy, resources are temporarily unavailable

### Fan Get Fan Speed (NetFn 0x34 Cmd 0x21)
BMC provides the command to control the FAN speed by the FAN index, the user sends the PWM to the target FAN and then BMC return to the PWM of the target FAN speed. This command also provides the FAN control mode within the FCS table now.

Request Data  \
Byte 1 FAN index \
Bit 7:4 Enable FAN control mode (only enable when bit7 set to 1) 
>  001: Mix mode \
>  002: PID mode \
>  003: Open loop mode \
>  004: Close loop mode \
Bit 3:0 FAN index 
>  00 : All FAN \
>  x : FAN x → the index of the FAN

Response Data  \
Byte 1 Completion Code 
>  0x00 Normal \
>  0xC0 Node busy, resources are temporarily unavailable  \
Byte 2:2N RPM for the FAN (for get FAN speed), the number of bytes depend on the FAN index

### Fan Set Duty Cycle (NetFn 0x34 Cmd 0x22)
If user tries to modify the FAN speed, this command provide the capacity to control the FAN speed. User could set the duty cycle to control the FAN speed.

Request Data 
Byte 1 FAN index 
>  00 : All FAN \
>  x : FAN x → the index of the FAN \
Byte 2 Set the FAN duty cycle, if it’s a get command and then set to 0x00

Response Data \
Byte 1 Completion Code 
>  0x00 Normal \
>  0xC0 Node busy, resources are temporarily unavailable  \
Byte 2 Duty cycle (for set FAN speed), the number of bytes depend on the FAN index

### Fan FSC Control (NetFn 0x34 Cmd 0x23)
This IPMI OEM command could allow user to configure the parameter in the FSC table dynamically.

Request Data  \
Byte 1 Type to query 
>  01: get the FSC table \
>  02: set the FSC table \
>  03: get number of FSC table \
Byte 2 Index of the profile 
>  00 : return the all number of FSC table ( for type 03 only) \
Byte 3:18 16 bytes of the data

Response Data \
Byte 1 Completion Code 
>  0x00 Normal \
>  0xC0 Node busy, resources are temporarily unavailable \
Byte 2:17 Type 0/1 16 bytes of FCS data \
Byte 2 Type 3 index of FCS tables \
Byte 3:N Type 3 - Type of FCS 
>  01: Open loop \
>  02: Close \
>  03: PID

## CPLD Control Command Set 

## Bus Control Command Set

## File/Firmware Command Set

## Miscellaneous Function Command Set

## BIOS Information Command Set


# Add to a Project
Follow these steps, to build this repository into a project:
1. For meta-foxconn projects, the foxconn-oem-ipmi.bb recipe should be in the meta-foxconn/recipes-phosphor/ipmi/ directory.
2. For project customizations of the repository add foxconn-oem-ipmi_git.bbappend and foxconn-oem-ipmi directory to the project meta layer recipes-phosphor/ipmi/.

Note: Step 3 can be skipped if the Gitlab server is already a known host of the build machine user.

Note: Step 4 can be skipped if the Gitlab server already has a copy of the public SSH key of the build machine user.

# Development
Development of the repository should be done with a development workspace, and note the different branches and their purpose(s).

## Branches
common-dev: Branch with development code for common IPMI commands

master: Branch with stable, common code for releases

tomahawk-dev: Branch that can be created for Tomahawk platform-specific commands.

To push local changes to the remote repository, navigate to the local workspace repository at /build/workspace/sources/foxconn-oem-ipmi, and then do the following:
1. git status (to check status of changes to commit)
2. git commit -m "Commit message"
3. git push (to the branch to push changes)

## Setup Development Workspace
Follow these steps to create a development workspace:

1. Navigate to the openbmc project repository.
2. Run the command, ". openbmc-env" to enter the openbmc environment.
3. Build the openbmc project, so that a "build" directory exists for the project.
4. Run the command, "devtool modify foxconn-oem-ipmi" to build the workspace directory under /build/workspace/sources/foxconn-oem-ipmi.

A .bbappend will be added to the build/appends directory to build changes from the local workspace directory instead of the remote repository for development.

## Remove Development Workspace
Follow these steps to remove the workspace and build from the remote repository:
1. Run "devtool reset foxconn-oem-ipmi"
2. Manually remove the workspace folder with "rm"

