import os
import pandas as pd
import time
import matplotlib.pyplot as plt
from lab_data_logging.libs.RigolDP832 import *
from lab_data_logging.libs.loading import *
from colorama import init, Fore, Back, Style

# Constants and global variables
_ERROR_STYLE = Fore.RED + Style.BRIGHT + "\rError! "
_SUCCESS_STYLE = Fore.GREEN + Style.BRIGHT  + "\r"
_POWER_SUPPLY_CHANNEL = 1
_DELAY = 0.05

loading = loading()

def emulate_iv_curve(power_supply, current_curve, voltage_curve):
    try:

        if not power_supply.status == "Connected":
            raise ConnectionError(_ERROR_STYLE + "Not connected to Rigol DP832 Power Supply.")
        
        if voltage_curve is None or current_curve is None:
            raise ValueError(_ERROR_STYLE + "Voltage and current curve lists must not be empty.")

        if len(voltage_curve) != len(current_curve):
            raise ValueError(_ERROR_STYLE + "Voltage and current curve lists must have the same length.")

        current = power_supply.measure_current(channel = _POWER_SUPPLY_CHANNEL)
        voltage = power_supply.measure_voltage(channel = _POWER_SUPPLY_CHANNEL)
        voltage_curve_value = voltage_curve[int(current*1000)]

        voltage_difference = voltage - voltage_curve_value

        # Adjust the step size based on the difference between measured voltage and target voltage
        if abs(voltage_difference) > 1:
            step_size = 0.5
        elif abs(voltage_difference) > 0.5:
            step_size = 0.2
        elif abs(voltage_difference) > 0.2:
            step_size = 0.05
        else:
            step_size = 0.01


        if voltage < voltage_curve_value:

            power_supply.increment_output_voltage(channel=_POWER_SUPPLY_CHANNEL, step_size=step_size)

        elif voltage > voltage_curve_value:
                
            power_supply.increment_output_voltage(channel=_POWER_SUPPLY_CHANNEL, step_size=-step_size)

        # if the operating point is within +/- 0.05 V of the curve, then we are on the curve, so display bright green
        if abs(voltage_difference) < 0.05:
            text_colour = Fore.GREEN + Style.BRIGHT
        
        # if the operating point is within +/- 0.1 V of the curve, then we are close to the curve, so display dim green
        elif abs(voltage_difference) < 0.1:
            text_colour = Fore.GREEN 
        
        # if the operating point is within +/- 0.2 V of the curve, then we are far from the curve, so display dim red
        elif abs(voltage_difference) < 0.2:
            text_colour = Fore.WHITE

        # if the operating point is more than +/- 0.2 V of the curve, then we are very far from the curve, so display bright red
        else:
            text_colour = Fore.RED + Style.BRIGHT

        loading.delay_with_loading_indicator(_DELAY)

        # Display values with three decimal places
        print(f"\rTracing IV curve: {text_colour} {current:.3f} A, {voltage:.3f} V {Style.RESET_ALL} Target voltage: {voltage_curve_value:.3f} V", end="")  


        # Plot the IV curve
        plt.clf()
        plt.plot(voltage_curve, current_curve, label='IV Curve')
        plt.scatter(voltage, current*1000, color='red', label='Operating Point')
        plt.xlabel('Voltage (V)')
        plt.ylabel('Current (mA)')
        plt.title('IV Curve')
        plt.legend()
        plt.grid(True)

        # Display values with three decimal places
        plt.text(0.02, 0.95, f"Current: {current:.3f} A\nVoltage: {voltage:.3f} V\nTarget Voltage: {voltage_curve_value:.3f} V",
                transform=plt.gca().transAxes, verticalalignment='top', bbox=dict(facecolor='white', alpha=0.5))

        plt.pause(0.001)
    except:
        pass


if __name__ == "__main__":
    
    # Get the current directory
    current_dir = os.getcwd()

    # Construct the relative file path
    file_path = os.path.join(current_dir, 'testing', 'solar_panel_iv_curve_data', 'interpolatedData.csv')

    # Load the CSV file
    csv_data = pd.read_csv(file_path)

    # Access the data from the CSV file
    current_mA = csv_data['Current_mA']
    voltage_V = csv_data['Voltage_V']

    # Connect to the power supply
    power_supply = RigolDP832()

    # Make sure the power supply is off
    power_supply.set_output_state(channel=_POWER_SUPPLY_CHANNEL, state='OFF')

    # Set the current limit and current value
    power_supply.set_current_limit(channel=_POWER_SUPPLY_CHANNEL, current=3)
    power_supply.set_overcurrent_protection_state(channel=_POWER_SUPPLY_CHANNEL, state='ON')
    power_supply.set_output_current(channel=_POWER_SUPPLY_CHANNEL, current=3)

    # Set the voltage limit
    power_supply.set_voltage_limit(channel=_POWER_SUPPLY_CHANNEL, voltage=16)
    power_supply.set_overvoltage_protection_state(channel=_POWER_SUPPLY_CHANNEL, state='ON')
    power_supply.set_output_voltage(channel=_POWER_SUPPLY_CHANNEL, voltage=0)

    # Set the output state to on
    power_supply.set_output_state(channel=_POWER_SUPPLY_CHANNEL, state='ON')

#    try:

    # Emulate the IV curve in a loop
    while True:
        # Emulate the IV curve
        try:
            emulate_iv_curve(power_supply, current_mA, voltage_V)
        except KeyboardInterrupt:
            # Make sure the power supply is off
            print("\n", end="")
            power_supply.set_output_state(channel=_POWER_SUPPLY_CHANNEL, state='OFF')
            print(Fore.RED + Style.BRIGHT + "\rKeyboard interrupt.")
        except:
            pass


#    except KeyboardInterrupt:
#        # Make sure the power supply is off
#        print("\n", end="")
#        power_supply.set_output_state(channel=_POWER_SUPPLY_CHANNEL, state='OFF')
#        print(Fore.RED + Style.BRIGHT + "\rKeyboard interrupt.")
#    except Exception as e:
#        # Make sure the power supply is off
#        power_supply.set_output_state(channel=_POWER_SUPPLY_CHANNEL, state='OFF')
#        # Raise the exception
#        error_message = f"{e}"
#        raise ValueError(_ERROR_STYLE + error_message)
