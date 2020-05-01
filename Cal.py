#!/usr/bin/python3


# Constants
PPN_SIZE_NO_OFFSET = 44
PPN_OFFSET_SIZE = 12
VPN_OFFSET_SIZE = 12
PPN_SIZE = PPN_OFFSET_SIZE + PPN_SIZE_NO_OFFSET
ENTRY_OFFSET_SIZE = 10
# PPN0 PPN1 PPN2 bit length
PPN_BIT_LEN = [9, 9, 26]
# VPN0 VPN1 VPN2 bit length
VPN_BIT_LEN = [9, 9, 9]

MEB_VALID = 1 << 0
MEB_READ = 1 << 1
MEB_WRITE = 1 << 2
MEB_EXECUTE = 1 << 3
MEB_USER = 1 << 4
MEB_GLOBAL = 1 << 5
MEB_RWX = MEB_READ | MEB_WRITE | MEB_EXECUTE

# The following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
RESET   = "\033[0m"
BLACK   = "\033[30m"      # Black 
RED     = "\033[31m"      # Red 
GREEN   = "\033[32m"      # Green 
YELLOW  = "\033[33m"      # Yellow 
BLUE    = "\033[34m"      # Blue 
MAGENTA = "\033[35m"      # Magenta 
CYAN    = "\033[36m"      # Cyan 
WHITE   = "\033[37m"      # White 
BOLDBLACK   = "\033[1m\033[30m"      # Bold Black 
BOLDRED     = "\033[1m\033[31m"      # Bold Red 
BOLDGREEN   = "\033[1m\033[32m"      # Bold Green 
BOLDYELLOW  = "\033[1m\033[33m"      # Bold Yellow 
BOLDBLUE    = "\033[1m\033[34m"      # Bold Blue 
BOLDMAGENTA = "\033[1m\033[35m"      # Bold Magenta 
BOLDCYAN    = "\033[1m\033[36m"      # Bold Cyan 
BOLDWHITE   = "\033[1m\033[37m"      # Bold White 
BrightGreen    = "\033[92m"      # Bright Green 
BrightBlack    = "\033[90m"      # Bright Black 
BrightRed    = "\033[91m"      # Bright Red 
BrightYellow    = "\033[93m"      # Bright Yellow 
BrightBlue    = "\033[94m"      # Bright Blue 
BrightMagenta    = "\033[95m"      # Bright Magenta 
BrightCyan    = "\033[96m"      # Bright Cyan 
BrightWhite    = "\033[97m"      # Bright White 
NC = '\033[0m'                  # No color

def welcome():
    print(BOLDCYAN +" _    _ _____ _     _____ ________  ___ _____ " )
    print(BOLDCYAN +"| |  | |  ___| |   /  __ \\  _  |  \\/  ||  ___|" )
    print(BOLDCYAN +"| |  | | |__ | |   | /  \\/ | | | .  . || |__  " )
    print(BOLDCYAN +"| |/\\| |  __|| |   | |   | | | | |\\/| ||  __| " )
    print(BOLDCYAN +"\\  /\\  / |___| |___| \\__/\\ \\_/ / |  | || |___ " )
    print(BOLDCYAN +" \\/  \\/\\____/\\_____/\\____/\\___/\\_|  |_/\\____/ " )
                                              
    print(MAGENTA +"Powered by " + WHITE + "C H W " + MAGENTA + ":)" )
    print(BOLDCYAN + "Bug feedback: " + BrightWhite + "c.hw@me.com")

def color_print(color, text):
    print("{}{}{}".format(color, text, NC))

def ones(num):
    return ((1 << num) - 1)

def not_ones(num_ones, num_bits):
    return ((1 << num_bits) - 1 - ones(num_ones))

def is_branch(entry):
    return ((entry & MEB_RWX) == 0)

def is_leaf(entry):
    return ((entry & MEB_RWX) != 0)

def is_valid(entry):
    return (entry & MEB_VALID == 1)

def main():
    welcome()
    while True:
        # get virtual address and find vpn and po
        va = int(input("Input the virtual address: "), 16)
        color_print(YELLOW, "Confirm the virtual address is: {}".format(hex(va)))
        vpn = [
            (va >> 12) & 0x1ff, # vpn[0]
            (va >> 21) & 0x1ff, # vpn[1]
            (va >> 30) & 0x1ff  # vpn[2]
        ]
        po = va & 0xfff

        # print VPN and PO
        for i in range(len(vpn)):
            print("VPN[{}] is: {}".format(i, vpn[i]))
        print("PO is: {}".format(hex(po)))

        # Get satp register value
        color_print(YELLOW, "Use Marz orz https://web.eecs.utk.edu/~smarz1/courses/cosc361/mmu")
        print("{}Get SATP(in hex) by typing \"{}satp{}\"{}".format(YELLOW, WHITE, YELLOW, NC))
        satp = int(input("Input the satp register value: "), 16)
        color_print(YELLOW, "Confirm SATP: {}".format(hex(satp)))

        # Check satp
        if((satp >> 60 == 0)):
            color_print(BrightRed, "MODE is 0! PA=VA!")
            print("The PA is: {}".format(hex(va)))
        elif (satp >> 60) != 8:
            color_print(BrightRed, "THis is NOT a RISC-V MMU!")
            return

        level = len(vpn) - 1
        ppn = (satp & ones(44)) << PPN_OFFSET_SIZE
        ppn += vpn[-1] * 8
        entry = 0
        while(level >= 0):
            color_print(YELLOW, "Please dereference level {} ppn: {}".format(level, hex(ppn)))
            entry = int(input("Input the level {} page table entry: ".format(level)), 16)
            color_print(YELLOW, "Confirm the level {} page table entry: {}".format(level, hex(entry)))
            if not is_valid:
                color_print(BrightRed, "Level {} page fault: invalid entry!".format(level))
                break 
            if is_branch(entry):
                if(level == 0):
                    color_print(BrightRed, "Level {} page fault: is branch!".format(level))
                    break
                ppn = (entry & ~ones(ENTRY_OFFSET_SIZE)) << 2
                ppn += vpn[level - 1] * 8
            else:
                print("This is a leaf!")
                bit_len = sum([VPN_BIT_LEN[i] for i in range(len(VPN_BIT_LEN)) if i < level])
                bit_len += VPN_OFFSET_SIZE
                pa = (entry << 2) & ~ones(bit_len)
                pa |= (va & ones(bit_len))
                color_print(YELLOW, "Got result!")
                print("The Physical Address is: {}".format(hex(pa)))
                break
            level -= 1
        color_print(BrightGreen, "----------------------------------------------------------------------")
        print("")
        color_print(BrightGreen, "----------------------------------------------------------------------")

if __name__ == "__main__":
    main()