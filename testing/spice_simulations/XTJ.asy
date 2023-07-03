Version 4
SymbolType BLOCK

* Define the size and position of the block
RECTANGLE Normal -32 -40 32 40

* Define the window display (usually used for device name and value)
WINDOW 0 0 -40 Bottom 2
WINDOW 3 0 40 Top 2

* Define the prefix and value for the device
SYMATTR Prefix X
SYMATTR Value XTJ

* Path to the model file
SYMATTR ModelFile solar_cell_model.lib

* Define the pins and their order in the SPICE model
* Positive terminal of the device (V+)
PIN 32 -16 RIGHT 8
PINATTR PinName V+
PINATTR SpiceOrder 1

* Negative terminal of the device (V-)
PIN 32 16 RIGHT 8
PINATTR PinName V-
PINATTR SpiceOrder 2

* Parameters for percent illumination, number of series and parallel PV cells
SYMATTR SpiceLine percentIllumination=100
SYMATTR SpiceLine2 numSeries=1 numParallel=1