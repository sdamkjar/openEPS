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

load_current = 0.4 # [A]

settling_time    = 5  # [seconds]

dac_voltage = 0.63 # [V] (0.63V is the maximum power point of the solar panel)



initial_load_current = 0.1 # [A]
final_load_current   = 0.6 # [A]
load_current_step    = 0.01 # [A]

a1 = numpy.arange(initial_load_current,final_load_current+load_current_step,load_current_step)
#and back down
a2 = numpy.arange(final_load_current,initial_load_current-load_current_step,-load_current_step)

load_current_list = numpy.concatenate((a2,a1))

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

    for dac_voltage in numpy.arange(0.7,1.5,0.1):

        log.new_file(f'testing/mppt_test_data/ltc3119mpptiv(DACHiZ).txt')

        oscilloscope.set_screenshot_path(f'testing/mppt_test_data/screenshots(DAC{dac_voltage}V)/*')

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
        function_gen.set_squ_offset(dac_voltage)
        load.set_cc_current(initial_load_current)
        load.enable()

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
                multimeter1.start_measurement(50)

                print("\rRecording data:")
                log.get_data()

                #if input("\rPress Enter to continue to next measurement (0 to retry)...\n") == '0':
                #    retry = True
                #else:
                retry = False
    log.close_file()

except Exception as e:

    print('\r'+Fore.RED + str(e))
    print('\r'+traceback.format_exc())

finally:

    load.disable()

    log.close_file()



