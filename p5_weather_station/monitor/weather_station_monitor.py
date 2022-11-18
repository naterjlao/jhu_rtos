# Weather Station Monitor Driver
# Author: Nate Lao (nlao1@jh.edu)
# Designed for Windows OS
import PySimpleGUI as sg
import weather_station_monitor_gui as gui
import weather_station_monitor_lib as lib
if __name__ == "__main__":
    ########## Initialization ##########
    # GUI Window
    window = sg.Window('ESP Weather Station Monitor', gui.LAYOUT, finalize=True)
    # Set Communication Status
    gui.SetLED(window, gui.COMM_LED_KEY, 'red')
    # Setup Receiver
    receiver = lib.RECEIVER()
    # Output logging
    #print("sample, temp, humidity")
    ########## Event Loop ##########
    while True:
        # Check to see if the cancel button was clicked and exit loop if clicked
        event, values = window.read(timeout=10)
        if event == 'Cancel' or event == sg.WIN_CLOSED:
            break
        # Poll Rx
        payload = receiver.recv()

        if payload != None:
            # Connection OK, Payload Verified
            gui.SetLED(window, gui.COMM_LED_KEY, 'green')
            # Unpack payload
            traffic_id, sample_n, temp, humidity = payload
            # Update GUI
            window[gui.SAMPLE_N_KEY].update(sample_n)
            window[gui.TEMP_BAR_KEY].update(temp)
            window[gui.TEMP_VAL_KEY].update(f"{temp:.2f}")
            window[gui.HUMD_BAR_KEY].update(humidity)
            window[gui.HUMD_VAL_KEY].update(f"{humidity:.2f}")
            #print(f"{sample_n},{temp},{humidity}")
        else:
            gui.SetLED(window, gui.COMM_LED_KEY, 'red')
            window[gui.SAMPLE_N_KEY].update("N/A")
            window[gui.TEMP_BAR_KEY].update(0)
            window[gui.TEMP_VAL_KEY].update("N/A")
            window[gui.HUMD_BAR_KEY].update(0)
            window[gui.HUMD_VAL_KEY].update("N/A")
            # Loop Terminated - Kill Window
            window.close()
else:
    print("This application cannot be imported, please execute directly.")