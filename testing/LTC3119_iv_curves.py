from lab_data_logging.data_logger import *
from lab_data_logging.libs.loading import *
from colorama import init, Fore
color = init(autoreset=True)
import traceback
import time

loader = loading()

###############################################################################
########################### SET INITIAL CONDITIONS ############################
###############################################################################


vinChannel  = 2
voutChannel = 1

psu_voltage = 9 #  [V]

psu_drop_correction = 0

measurement_time = 15 # [seconds]

a1 = numpy.arange(0.1,0.8,0.05)
a2 = numpy.arange(0.8,2.6,0.1)
A = numpy.concatenate([a1,a2])

firstTime = False

###############################################################################
############################ SET-UP DATA LOGGING ##############################
###############################################################################

try:

    log = data_logger()

    scope = log.connect('DS7034')
    load  = log.connect('DL3021')
    dmm =   log.connect('Keysight34460A')
    psu =   log.connect('DP832')

    log.add(scope,'VAVG_STAT',channel=vinChannel ,label='Vin'   )
    log.add(scope,'VPP_STAT' ,channel=vinChannel ,label='VinPP' )
    log.add(scope,'VAVG_STAT',channel=voutChannel,label='Vout'  )
    log.add(scope,'VPP_STAT' ,channel=voutChannel,label='VoutPP')
    log.add(dmm  ,'STAT'                         ,label='Iin'   )
    log.add(load ,'CURR_AVG'                     ,label='Iout'  )

    log.newFile('ltc3119AsShipped.txt')

    def ir_drop_correction(drop_correction , max_retries=10):

        vin_avg = scope.measure_stat('VAVG', vinChannel)[0]
        psu_drop_correction_counts = 0
        while vin_avg < psu_voltage:
            if psu_drop_correction_counts >= max_retries:
                print('\b \n\r\033[93m' +"Warning! Supply voltage incremented %d times without achieving target Vin."%max_retries+'\033[39m')
                input("\rPress Enter to continue (CTRL+C to Cancel)...\n")
                psu_drop_correction_counts = 0
            psu.set_voltage(1,psu_voltage + drop_correction)
            print("\rCompensating for IR drop:\t"+Back.WHITE+Fore.BLACK+\
                    " Vsupply: " + Back.BLUE + Fore.WHITE + " %.2f V "%(psu_voltage + drop_correction)\
                    + Back.RESET + Fore.RESET + '  ' + Back.WHITE+Fore.BLACK+\
                    " Vin_avg: " + Back.BLUE + Fore.WHITE + " %.2f V  "%(vin_avg), end = '')
            scope.measure_stat_reset()
            loader.delay_with_loading_indicator(1)
            drop_correction = drop_correction + 0.01
            vin_avg = scope.measure_stat('VAVG', vinChannel)[0]
            scope.set_vertical_offset(-vin_avg,vinChannel)
            psu_drop_correction_counts = psu_drop_correction_counts + 1
        print("\b ")
        return drop_correction

###############################################################################
############################# SET-UP INSTRUMENTS ##############################
###############################################################################

    for channel in [1,2,3]:
        while scope.configure_probe(channel,'OMEG',10,'20M') == False:
            print('\033[93m' +"Warning! Unable to configure channel %d. Reconnect the probe."%(channel)+'\033[39m')
            input("Press Enter to continue...\n")

    dmm.configure('CURRENT:DC', 1.0, 0.0001)
    psu.set_voltage(1,psu_voltage)
    psu.toggle_output(1,1)
    load.configure_output_sense()
    load.set_cc_current(0)
    load.enable()

    scope.measure_clear('ALL')

    scope.measure_item('VAVG', 'CHAN1')
    scope.measure_item('VAVG', 'CHAN2')
    scope.measure_item('VMID', 'CHAN2')
    scope.measure_item('VPP', 'CHAN1')
    scope.measure_item('VPP', 'CHAN2')
    scope.measure_item('PER' , 'CHAN2')
    scope.measure_item('PER', 'CHAN3')
    scope.measure_item('VMAX', 'CHAN3')

    scope.enable_statistics()

    loader.delay_with_loading_indicator(2)

    vout = load.measure_voltage()

    scope.configure_trigger_edge(scope.measure_stat('VMAX', 3)[0]*0.95,3)
    scope.set_vertical_scale(psu_voltage/3 , 3)
    scope.set_vertical_offset(-(psu_voltage/2), 3)
    
    scope.set_vertical_scale(0.5 ,  vinChannel)
    scope.set_vertical_scale(0.5 , voutChannel)
    scope.set_vertical_offset(-( psu.measure_voltage()),  vinChannel)
    scope.set_vertical_offset(-(load.measure_voltage()), voutChannel)

    scope.set_time_scale(0.0005)

    loader.delay_with_loading_indicator(2)

    max_initial_cc = 0.3

    if A[0] > max_initial_cc:
        while A[0] - max_initial_cc > 0.1:
            print("\rRamping up to initial value.")
            load.set_cc_current(max_initial_cc)
            load.input_status()
            psu_drop_correction = ir_drop_correction(psu_drop_correction,50)
            max_initial_cc = max_initial_cc + 0.1

    print("\rConfiguration complete.")


###############################################################################
############################# SET UP MEASUREMENT ##############################
###############################################################################

    for x in A:
        retry = True
        while retry:
            if not(firstTime):
                load.set_cc_current(x)
                load.input_status()
            else:
                firstTime = False

            print("\rPreparing for next measurement...")

######################### SET TIME SCALE IF NECESSARY #########################

            if x >= 0.7:
                scope.set_time_scale(0.000002)
                
            elif x == 0.02:
                scope.set_time_scale(0.0005)

            loader.delay_with_loading_indicator(2)


########################### SET INITIAL CONDITIONS ############################

            scope.set_vertical_scale(0.5 ,  vinChannel)
            scope.set_vertical_scale(0.5 , voutChannel)
            scope.set_vertical_offset(-( psu.measure_voltage()),  vinChannel)
            scope.set_vertical_offset(-(load.measure_voltage()), voutChannel)

            loader.delay_with_loading_indicator(1)

            scope.configure_trigger_edge(scope.measure_stat('VMAX', 3)[0]*0.95,3)

            scope.measure_stat_reset()

            loader.delay_with_loading_indicator(2)


############################# ZOOM IN AT DC BIAS ##############################

            offset = -scope.measure_stat('VAVG', voutChannel)[0]
            scope.set_vertical_scale(0.2,voutChannel)
            scope.set_vertical_offset(offset,voutChannel)

            offset = -scope.measure_stat('VAVG', vinChannel)[0]
            scope.set_vertical_scale(0.2,vinChannel)
            scope.set_vertical_offset(offset,vinChannel)

            scope.measure_stat_reset()

            loader.delay_with_loading_indicator(2)

############################# CORRECT FOR IR DROP #############################

            psu_drop_correction=ir_drop_correction(psu_drop_correction)
            
######################### ZOOM IN FOR HIGH RESULTION ##########################

            period = scope.measure_stat('PER', vinChannel)[0]
            if period > 1:
                period = scope.measure_stat('PER', 3)[0]
            scope.set_time_scale(period)

            loader.delay_with_loading_indicator(2)

            offset = -scope.measure_stat('VAVG', voutChannel)[0]
            scope.set_vertical_scale(scope.measure_stat('VPP', voutChannel)[0]/5,voutChannel)
            scope.set_vertical_offset(offset,voutChannel)

            offset = -scope.measure_stat('VMID', vinChannel)[0]
            scale = scope.measure_stat('VPP', vinChannel)[0]
            if scale < 0.1:
                scope.set_vertical_scale(scale/3,vinChannel)
            else:
                scope.set_vertical_scale(scale/5,vinChannel)
            if numpy.abs(offset + psu_voltage) < 0.5:
                scope.set_vertical_offset(offset,vinChannel)
            else:
                scope.set_vertical_offset(-psu_voltage,vinChannel)
                loader.delay_with_loading_indicator(2)
                offset = -scope.measure_stat('VAVG', vinChannel)[0]
                scope.set_vertical_offset(-psu_voltage,vinChannel)

            input("\rReady to measure. Press Enter to continue...\n")

############################## TAKE MEASUREMENTS ##############################

            scope.measure_stat_reset()
            dmm.start_measurement(50)
            loader.delay_with_loading_bar(measurement_time,'Waiting for measurements')
            print("\rRecording data:")
            log.get()

            if input("\rPress Enter to continue to next measurement (0 to retry)...\n") == '0':
                retry = True
            else:
                retry = False

except Exception as e:

    print('\r'+Fore.RED + str(e))
    print('\r'+traceback.format_exc())

finally:

    psu.toggle_output(1,0)
    load.disable()

    log.save()



