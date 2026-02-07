# FT232H

_The FT232H is a single channel USB 2.0 Hi-Speed (480Mb/s) to UART/FIFO IC. It has a Multi-Protocol Synchronous Serial Engine (MPSSE). The MPSSE can be configured to a number of industry standard serial interface protocols such as JTAG, I2C or SPI (MASTER), or it can be used to implement a proprietary bus protocol._ ([DS_FT232H.pdf][1], [DS_UM232H.pdf][2]) 
```
        UM232H Pin Out
      ┌─────╥─────╥─────┐
      │     ║ USB ║     │
      │     ║     ║     │
  J2-1│     ╚═════╝     │J1-1
   ───┤ SLD         GND ├─P─
 *┌─P─┤ VIO         5V0 ├─◄─┐*
  └─◄►┤ 3V3         USB ├─►─┘
   ─C─┤ PU2        RST# ├─◄─
   ─C─┤ PU1         AC9 ├◄►─
   ─P─┤ GND         AC8 ├◄►─
   ─◄─┤ AD0         AC7 ├◄►─
   ─►─┤ AD1         AC6 ├◄►─
   ─◄─┤ AD2         AC5 ├◄►─
   ─►─┤ AD3         AC4 ├◄►─
   ─◄─┤ AD4         AC3 ├◄►─
   ─►─┤ AD5         AC2 ├◄►─
   ─►─┤ AD6         AC1 ├◄►─
   ─►─┤ AD7         AC0 ├◄►─
 J2-14└─────────────────┘J1-14

* USB bus powered configuration
```

## Build
1. Install `libftdi-devel`, `swig`
2. Run `git submodule update --init --recursive`
3. From libmpsse/src directory run `./configure`
4. Go to some example and run `make`

[1]: https://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT232H.pdf
[2]: https://www.ftdichip.com/Support/Documents/DataSheets/Modules/DS_UM232H.pdf
