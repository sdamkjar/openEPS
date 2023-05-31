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

load_current = 0.3 # [A]

measurement_time = 20 # [seconds]
settling_time    = 5  # [seconds]

initial_dac_voltage = 0.5 # [V]
final_dac_voltage   = 1.5 # [V]
dac_voltage_step    = 0.01 # [V]

dac_voltage_list = numpy.arange(initial_dac_voltage,final_dac_voltage+dac_voltage_step,dac_voltage_step)

###############################################################################
############################ SET-UP DATA LOGGING ##############################
###############################################################################

try:

    log = data_logger()


    oscilloscope = log.connect('RigolDS7034')
    multimeter1  = log.connect('Keysight34460A')
    multimeter2  = log.connect('U1233A')
    load         = log.connect('DL3021')
    function_gen = log.connect('KS33500B')

    log.add(time,'elapsed')

    log.add(multimeter2,'MEAS_AVG',label='Iin')

    log.add(load,'VOLT_AVG',label='Vout')
    log.add(load,'CURR_AVG',label='Iout')

    log.add(oscilloscope,'VPP_STAT' ,channel=vin_channel ,label='VinPP')
    log.add(oscilloscope,'VPP_STAT' ,channel=vout_channel,label='VoutPP')
    log.add(oscilloscope,'VAVG_STAT',channel=dac_channel ,label='Vdac')

    log.add(multimeter1,'statistics',label='Vin')


    log.add(oscilloscope,'screenshot')

    log.new_file(f'testing/mppt_test_data/ltc3119mppt({load_current}A).txt')

    oscilloscope.set_screenshot_path(f'testing/mppt_test_data/screenshots({load_current}A)/*')

###############################################################################
############################# SET-UP INSTRUMENTS ##############################
###############################################################################

    
    while not oscilloscope.configure_probe(vout_channel,'OMEG',10,'AC','20M'): pass
    while not oscilloscope.configure_probe(sw1_channel, 'OMEG',10,'DC','20M'): pass
    while not oscilloscope.configure_probe(vin_channel, 'OMEG',10,'AC','20M'): pass
    while not oscilloscope.configure_probe(dac_channel, 'OMEG',1,'DC','20M'): pass

    loader.delay_with_loading_indicator(2)
            
    oscilloscope.configure_time_scale(0.0001) # 100us/div

    oscilloscope.set_vertical_scale(vin_channel,  0.1) # 100mV/div
    oscilloscope.set_vertical_scale(vout_channel, 0.1) # 100mV/div
    oscilloscope.set_vertical_scale(sw1_channel,  1.0)  # 1V/div
    oscilloscope.set_vertical_offset(sw1_channel,-2.5)  # -2.5V
    oscilloscope.set_vertical_scale(dac_channel,  1.0)  # 1V/div
    oscilloscope.set_vertical_offset(dac_channel,-2.5)  # -2.5V

    multimeter1.configure('VOLTAGE:DC', 10, 0.0001) # 10V range, 100uV resolution

    oscilloscope.clear_measure_item('ALL')

    oscilloscope.enable_statistic_item('VPP', vin_channel)
    oscilloscope.enable_statistic_item('VPP', vout_channel)
    oscilloscope.enable_statistic_item('VAVG', dac_channel)

    load.configure_output_sense()
    load.set_cc_current(load_current)
    load.enable()

    vout = load.measure_voltage()

    print(_SUCCESS_STYLE + "Instrument setup complete!")


###############################################################################
############################# SET UP MEASUREMENT ##############################
###############################################################################

    for dac_voltage in dac_voltage_list:

        retry = True

        while retry:

            print("\rPreparing for measurement...")

            function_gen.set_squ_offset(dac_voltage)
            print(f"\rDAC voltage set to {dac_voltage}V")
            loader.delay_with_loading_bar(settling_time,'Waiting for settling')
            print("\n",end='')

############################## TAKE MEASUREMENTS ##############################

            oscilloscope.reset_statistics()
            multimeter1.start_measurement(50)

            print("\rRecording data:")
            log.get_data()

            #if input("\rPress Enter to continue to next measurement (0 to retry)...\n") == '0':
            #    retry = True
            #else:
            retry = False

except Exception as e:

    print('\r'+Fore.RED + str(e))
    print('\r'+traceback.format_exc())

finally:

    load.disable()

    log.close_file()



