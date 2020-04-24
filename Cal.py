
val = input('input satp >>>>> ')
vir = input('input vir addr >>>>> ')
vpn2 = (vir>>30)&0x1ff
vpn1 = (vir>>21)&0x1ff
vpn0 = (vir>>12)&0x1ff
po = vir & 0xfff
turn = input('input turn >>>>> ')
if(turn==1):
    buf = (val<<12)
    buf = buf + vpn2 * 8
    print(format(buf, '#018x'))
if(turn==2):
    buf = ((val&~0x3ff) << 2)
    buf = buf + vpn1 * 8
    print(format(buf,'#018x'))
if(turn==3):
    buf = (val&~0x3ff) << 2
    buf = buf+vpn0 * 8
    ##buf = buf + po
    print(format(buf,'#018x'))
if(turn==4):
    buf = (val&~0x3ff) << 2
    buf  = buf + po
    print(format(buf,'#018x'))
