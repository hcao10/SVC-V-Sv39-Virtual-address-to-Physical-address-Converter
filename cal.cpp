#include<iostream>
#include<cmath>
#include<cstdlib>
#include<stdio.h>
#include<string>

using namespace std;
enum MmuEntryBits {
	MEB_VALID = 1 << 0,
	MEB_READ = 1 << 1,
	MEB_WRITE = 1 << 2,
	MEB_EXECUTE = 1 << 3,
	MEB_USER = 1 << 4,
	MEB_GLOBAL = 1 << 5,

	MEB_RWX = MEB_READ | MEB_WRITE | MEB_EXECUTE,
};

//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define BrightGreen    "\033[92m"      /* Bright Green */
#define BrightBlack    "\033[90m"      /* Bright Black */
#define BrightRed    "\033[91m"      /* Bright Red */
#define BrightYellow    "\033[93m"      /* Bright Yellow */
#define BrightBlue    "\033[94m"      /* Bright Blue */
#define BrightMagenta    "\033[95m"      /* Bright Magenta */
#define BrightCyan    "\033[96m"      /* Bright Cyan */
#define BrightWhite    "\033[97m"      /* Bright White */
int main(){
    cout << BOLDCYAN <<" _    _ _____ _     _____ ________  ___ _____ " << endl;
    cout << BOLDCYAN <<"| |  | |  ___| |   /  __ \\  _  |  \\/  ||  ___|" << endl;
    cout << BOLDCYAN <<"| |  | | |__ | |   | /  \\/ | | | .  . || |__  " << endl;
    cout << BOLDCYAN <<"| |/\\| |  __|| |   | |   | | | | |\\/| ||  __| " << endl;
    cout << BOLDCYAN <<"\\  /\\  / |___| |___| \\__/\\ \\_/ / |  | || |___ " << endl;
    cout << BOLDCYAN <<" \\/  \\/\\____/\\_____/\\____/\\___/\\_|  |_/\\____/ " << endl;
                                              
    cout << MAGENTA <<"Powered by "<<WHITE<<"C H W "<<MAGENTA<<":)" << endl;
    cout << BOLDCYAN << "Bug feedback: " << BrightWhite << "c.hw@me.com" <<endl;

    unsigned long va;
    unsigned long level2pta;
    unsigned long level2;
    unsigned long level1pta;
    unsigned long level1;
    unsigned long level0pta;
    unsigned long level0;
    unsigned long pte; //page table entry.
    unsigned long ppn;
    unsigned long pa;
    bool continueNext = 1;
    
    while(continueNext){
    cout << WHITE<<"Input the virtual address: 0x ";
    cin >> hex>>va;
    cout << GREEN<<"Confirm the virtual address is: 0x " <<WHITE <<hex << va <<endl;
unsigned int vpn[] = {
    (unsigned int)(va >> 12) & 0x1ff, // vpn[0]
    (unsigned int)(va >> 21) & 0x1ff, // vpn[1]
    (unsigned int)(va >> 30) & 0x1ff  // vpn[2]
};
int po = va & 0xfff;
for(int i = 2; i >=0; i--){
    cout << WHITE<< "VPN[" << i << "] is: " << dec<< vpn[i] <<endl;
}
cout << "PO is: 0x " <<hex<< po <<endl;
cout<<YELLOW<<"Use Marz orz https://web.eecs.utk.edu/~smarz1/courses/cosc361/mmu"<<endl;
cout<<YELLOW<<"Get SATP(in hex) by typing \""<<WHITE<<"satp" << YELLOW<<" \" "<<endl;
cout << WHITE<< "The SATP is: 0x";
unsigned long theSatp;
cin >> hex >> theSatp;
cout << GREEN<<"Confirm SATP: 0x " <<WHITE<< hex << theSatp <<endl;

//Check mode:
if((theSatp >>60) == 0){
    cout << BrightRed << "MODE is 0! PA=VA!"<<endl;
    cout << WHITE << "The PA is: 0x " <<va <<endl;
}
else if( (theSatp >>60) == 8){
    //Start
    //PPN
    ppn = (theSatp & ~(0xfffffLU<<44));
    level2 = ppn << 12;
    level2pta = vpn[2] * 8 + level2;

    cout << BrightYellow << "Please dereference levle 2 pte: 0x " << WHITE << level2pta<<endl;
    cout << "Input the level 2 page table entry: 0x ";
    cin >> hex >> pte;
    cout << "Checking the level 2 page table entry ("<< BrightYellow << "0x" << pte<< WHITE<<")..."<<endl;
    if( (pte & MEB_VALID) == 1){
        cout <<BrightGreen <<"The entry is vaild"<<endl;
        //Check the RWX:
        if((pte & MEB_RWX) != 0){
            cout << "This is a Leaf." <<endl<<endl;
            //If level 2 is a leaf PPN[2] is the only thing translated. 
            //PPN[1], PPN[0], and PO are copied from the virtual address directly.
            cout << "level 2 is a leaf PPN[2] is the only thing translated. "<<endl;

            //Get the ppn:
            //PPN starts at bit 10, but in the physical address, it starts at bit 12:
            ppn = (pte & (0x3ffffffLU << 28))<<2;
            //Get physical address:
            pa = ppn + (va & ~(0x1ffUL<<30));
            //return (u64)paddress;
            cout << BrightYellow << "Got result!"<<endl;
            cout << WHITE << "The Physical Address is: " << BrightGreen <<"0x"<< hex << pa <<endl;
        }
        else{
            //We see that R=0,W=0,and X=0. Therefore, this is a branch.
            cout << "This is a branch." <<endl<<endl;
            level1 = (pte & (~0x3ffLU)) <<2;
            level1pta = vpn[1] * 8 + level1;
            cout << BrightYellow << "Please dereference levle 1 pte: 0x " << WHITE << level1pta<<endl;
            cout << "Input the level 1 page table entry: 0x ";
            cin >> hex >> pte;
            cout << "Checking the level 1 page table entry ("<< BrightYellow << "0x" << pte<< WHITE<<")..."<<endl;
            if( (pte & MEB_VALID) == 1){
                cout <<BrightGreen<< "The entry is vaild"<<endl;
                //Check the RWX:
                if((pte & MEB_RWX) != 0){
                    cout << "This is a Leaf." <<endl<<endl;
                    //If level 1 is a leaf, PPN[2] and PPN[1] are the only things translated. 
                    //PPN[0] and PO are copied from the virtual address directly.
                    //Get the ppn:
                    //PPN starts at bit 10, but in the physical address, it starts at bit 12:
                    ppn = (pte & (0x7ffffffffLU <<19))<<2;
                    //Get physical address:
                    pa = ppn + (va & 0x1fffffUL);
                    //return (u64)paddress;
                    cout << BrightYellow << "Got result!"<<endl;
                    cout << WHITE << "The Physical Address is: " <<BrightGreen <<"0x"<< hex << pa <<endl;
                }
                else{
                    //We see that R=0,W=0,and X=0. Therefore, this is a branch.
                    cout << "This is a branch." <<endl<<endl;
                    level0 = (pte & (~0x3ffLU)) <<2;
                    level1pta = vpn[0] * 8 + level0;
                    cout << BrightYellow << "Please dereference levle 0 pte: 0x " << WHITE << level1pta<<endl;
                    cout << "Input the level 0 page table entry: 0x ";
                    cin >> hex >> pte;
                    cout << "Checking the level 0 page table entry ("<< BrightYellow << "0x" << pte<< WHITE<<")..."<<endl;
                    if( (pte & MEB_VALID) == 1){
                        cout <<BrightGreen<< "The entry is vaild"<<endl;
                         if((pte & MEB_RWX) != 0){
                            cout << "This is a Leaf." <<endl <<endl;
                            //If level 0 is a leaf, PPN[2], PPN[1], and PPN[0] are translated. 
                            //The PO is copied from the virtual address directly.
                            pa = (pte & (~0x3ffLU)) <<2;
                            pa += po;
                            cout << BrightYellow << "Got result!"<<endl;
                            cout << WHITE << "The Physical Address is: " <<BrightGreen <<"0x"<<  hex << pa <<endl;
                         }
                         else{
                             //If level 0 is NOT a leaf or if ANY entry is invalid (V=0), then it gives a page fault (segmentation fault).
                             cout<<BrightRed << "!Got a branch, page fault!"<< WHITE <<endl<<endl;
                         }
                
                    }
                    else{
                        cout << BrightRed <<"!Level 0 Page fault!" << WHITE <<endl;
                    }

                }
            }
            else{
                cout << BrightRed <<"!Level 1 Page fault!" << WHITE <<endl;
            }
        }
    }
    else
    {
        cout << BrightRed <<"!Level 2 Page fault!" << WHITE <<endl;
        //exit
    }
    




}
else{
    cout << BrightRed << "THis is NOT a !"<< WHITE << "RISC-V MMU" <<endl;
}
    cout <<BrightGreen<<"----------------------------------------------------------------------" <<endl;
    cout << endl;
    cout <<BrightWhite<<"----------------------------------------------------------------------" <<endl;
    }
    return 0;
}
