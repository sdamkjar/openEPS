from lab_data_logging.data_logger import *
from lab_data_logging.libs.loading import *
from colorama import init, Fore
color = init(autoreset=True)
import traceback
import time

# Constants and global variables
_ERROR_STYLE = Fore.RED + Style.BRIGHT + "\rError! "
_SUCCESS_STYLE = Fore.GREEN + Style.BRIGHT  + "\r"
_DELAY = 0.1

loader = loading()
retry = True

###############################################################################
########################### SET INITIAL CONDITIONS ############################
###############################################################################

# oscilloscope channels
vin_channel  = 3
vout_channel = 1
sw1_channel  = 2
dac_channel  = 4

# input voltage given by console argument
input_voltage = float(sys.argv[1])

settling_time    = 5  # [seconds]

initial_load_current = 0.1 # [A]
final_load_current   = 3 # [A]
load_current_step    = 0.02 # [A]

load_current_list = numpy.arange(initial_load_current,final_load_current+load_current_step,load_current_step)

###############################################################################
############################ SET-UP DATA LOGGING ##############################
###############################################################################

try:

    log = data_logger()

    oscilloscope = log.connect('RigolDS7034')
    multimeter  = log.connect('U1233A')
    load         = log.connect('DL3021')
    power_supply = log.connect('RigolDP832')

    log.add(time,'elapsed')

    log.add(power_supply,'average_voltage',channel=1,label='Vin')
    log.add(power_supply,'average_current',channel=1,label='Iin')

    log.add(multimeter,'MEAS_AVG',label='Temperature')

    log.add(load,'VOLT_AVG',label='Vout')
    log.add(load,'CURR_AVG',label='Iout')

    log.add(oscilloscope,'VPP_STAT' ,channel=vin_channel ,label='VinPP')
    log.add(oscilloscope,'VPP_STAT' ,channel=vout_channel,label='VoutPP')

    log.add(oscilloscope,'screenshot')

    log.new_file(f'testing/iv_test_data/ltc3119iv(vin={input_voltage}V).txt')

    oscilloscope.set_screenshot_path(f'testing/iv_test_data/screenshots(vin={input_voltage}V)/*')

###############################################################################
############################# SET-UP INSTRUMENTS ##############################
###############################################################################

    
    while not oscilloscope.configure_probe(vout_channel,'OMEG',10,'AC','20M'): pass
    while not oscilloscope.configure_probe(sw1_channel, 'OMEG',10,'DC','20M'): pass
    while not oscilloscope.configure_probe(vin_channel, 'OMEG',10,'AC','20M'): pass

    loader.delay_with_loading_indicator(2)
            
    oscilloscope.configure_time_scale(0.0001) # 100us/div

    oscilloscope.set_vertical_scale(vin_channel,  0.15) # 100mV/div
    oscilloscope.set_vertical_scale(vout_channel, 0.1) # 100mV/div
    oscilloscope.set_vertical_scale(sw1_channel,  1.0)  # 1V/div
    oscilloscope.set_vertical_offset(sw1_channel,-2.5)  # -2.5V

    oscilloscope.clear_measure_item('ALL')

    oscilloscope.enable_statistic_item('VPP', vin_channel)
    oscilloscope.enable_statistic_item('VPP', vout_channel)

    load.configure_output_sense()
    load.set_cc_current(0)
    load.enable()

    power_supply.set_output_voltage(channel = 1, voltage = input_voltage)
    power_supply.set_output_current(channel = 1, current = 3)
    power_supply.set_output_state(channel = 1, state = 'ON')

    vout = load.measure_voltage()

    print(_SUCCESS_STYLE + "Instrument setup complete!")


###############################################################################
############################# SET UP MEASUREMENT ##############################
###############################################################################

    for load_current in load_current_list:

        retry = True

        while retry:

            print("\rPreparing for measurement...")

            load.set_cc_current(load_current)
            load.enable()
            loader.delay_with_loading_bar(settling_time,'Waiting for settling')
            print("\n",end='')

############################## TAKE MEASUREMENTS ##############################

            oscilloscope.reset_statistics()

            print("\rRecording data:")
            log.get_data()

            # check if the power supply current is above 3A
            # if so, raise an error and stop the test
            if power_supply.measure_current(channel = 1) > 3:
                error_message = "Power supply current is above 3A!"
                raise Exception(_ERROR_STYLE + error_message)
               

            #if input("\rPress Enter to continue to next measurement (0 to retry)...\n") == '0':
            #    retry = True
            #else:
            retry = False

except Exception as e:

    print('\r'+Fore.RED + str(e))
    print('\r'+traceback.format_exc())

finally:

    load.disable()
    power_supply.set_output_state(channel=1,state='OFF')

    log.close_file()



