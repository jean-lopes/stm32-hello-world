# Table of Contents

1.  [CMSIS STM32 Programming](#org2a55ff3)
    1.  [Hardware Requirements](#org57b1cf2)
        1.  [Hardware](#org044ba03)
        2.  [Software](#orgdb1076d)
    2.  [Software setup on Arch Linux](#org6eb58d7)
    3.  [Build & Flash](#orgfa857db)


<a id="org2a55ff3"></a>

# CMSIS STM32 Programming


<a id="org57b1cf2"></a>

## Hardware Requirements


<a id="org044ba03"></a>

### Hardware

1.  [Blackpill](https://stm32world.com/wiki/Black_Pill) clone (STM32F401CCU6)

    ![img](res/blackpill.webp)

2.  RS232 FTDI module and usb-cable if needed

    ![img](res/rs232-ftdi.webp)

3.  st-link v2 clone

    ![img](res/st-link-v2.webp)

4.  jumper wires

    ![img](res/jumpers.webp)

5.  breadboard

    ![img](res/breadboard.webp)


<a id="orgdb1076d"></a>

### Software

1.  [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)

2.  [st-link](https://github.com/stlink-org/stlink)

3.  [STM32F4 DSP and standard peripherals library](https://www.st.com/en/embedded-software/stsw-stm32065.html)


<a id="org6eb58d7"></a>

## Software setup on Arch Linux

Install needed packages

    sudo pacman -S make unzip gcc-arm-none-eabi-bin stlink

Download the standard peripheral library from [st.com](https://www.st.com/en/embedded-software/stsw-stm32065.html)

Unzip the library

    unzip en.stsw-stm32065_v1-9-0.zip

The folder created during extraction is the `STM_COMMON` directory referenced in the `Makefile`

Done.


<a id="orgfa857db"></a>

## Build & Flash

Edit the `Makefile` variables as needed.

`STM_COMMON` should point to the extracted directory from the downloaded library.

make build
make burn
