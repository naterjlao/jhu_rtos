# Attitude Indicator
# Author: Nate Lao (nlao1@jh.edu)
# Designed for Windows OS
# GUI Driver for AI measurements
import PySimpleGUI as sg
import gps_monitor_lib as lib

sg.theme('Dark Blue')

FONT_SIZE = ('Any 30')
LAYOUT = [
    [sg.Text('FlyPi GPS Monitor'), sg.Push(), sg.Text('             Author: Nate Lao (lao.nathan@yahoo.com)')],
    [sg.Text('GPS Data',font=FONT_SIZE), sg.Push(), sg.Text('N/A',key='--data--',font=FONT_SIZE)]
    ]

if __name__ == "__main__":
    window = sg.Window('FlyPi GPS Monitor', LAYOUT, finalize=True)
    # Setup Receiver
    receiver = lib.ETHERNET_ADAPTER_GPS()

    persistance = 0
    while True:
        # Check to see if the cancel button was clicked and exit loop if clicked
        event, values = window.read(timeout=0)
        if event == 'Cancel' or event == sg.WIN_CLOSED:
            break

        # Poll Rx
        payload = receiver.recv()

        # Display GPS Data
        if payload != None:
            window['--data--'].update(payload)
            
    # Loop Terminated - Kill Window
    window.close()
else:
    print("This application cannot be imported, please execute directly.")