# Silvia Espresso Machine Computer Control

## Web application built with Volt
Be sure to read the volt docs at http://docs.voltframework.com

## Firmware build with Particle

Clone the Particle firmware repository, switch to the develop branch.

Build with `make all PARTICLE_DEVELOP=1 PLATFORM=photon APPDIR=~/Programming/silvia/firmware`

To reflash all firmware, `cd modules`, `make all program-dfu PLATFORM=photon APPDIR=~/Programming/silvia/firmware`

To reflas main code, `cd main`, `make all program-dfu PLATFORM=photon APPDIR=~/Programming/silvia/firmware`

