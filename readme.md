
# Table of Contents

1.  [CMSIS STM32 Programming](#org7ca71c5)
    1.  [Hardware Requirements](#org0db8be5)
        1.  [Hardware](#org7c5b7a2)
        2.  [Software](#org053e9f7)
    2.  [Software setup on Arch Linux](#orgba859bd)
    3.  [Build & Flash](#org7af6963)


<a id="org7ca71c5"></a>

# CMSIS STM32 Programming


<a id="org0db8be5"></a>

## Hardware Requirements


<a id="org7c5b7a2"></a>

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


<a id="org053e9f7"></a>

### Software

1.  [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)

2.  [st-link](https://github.com/stlink-org/stlink)

3.  [STM32CubeF4 Library](https://github.com/STMicroelectronics/STM32CubeF4)


<a id="orgba859bd"></a>

## Software setup on Arch Linux

Install needed packages

    sudo pacman -S make unzip gcc-arm-none-eabi-bin stlink

Download the standard peripheral library from [st.com](https://www.st.com/en/embedded-software/stsw-stm32065.html)

    git clone https://github.com/STMicroelectronics/STM32CubeF4

The cloned folder is the `STM_COMMON` directory referenced in the `Makefile`

Done.


<a id="org7af6963"></a>

## Build & Flash

Edit the `Makefile` variables as needed.

`STM_COMMON` should point to the cloned repository.

    make build
    make burn

