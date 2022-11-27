# Attitude Indicator
# Author: Nate Lao (nlao1@jh.edu)
# Designed for Windows OS
# GUI Driver for AI measurements
import PySimpleGUI as sg
import numpy as np
import cv2
import attitude_indicator_lib as lib
import attitude_indicator_gui as gui

sg.theme('Dark Blue')

HYSTERESIS = 5
FONT_SIZE = ('Any 30')
RAW_DATA_COLUMN = sg.Column([
    [sg.Push()],
    [sg.Frame('Yaw (degrees)',[[sg.Text("N/A",key="--yaw--",font=FONT_SIZE),sg.Push(),sg.Text("°",font=FONT_SIZE)]])],
    [sg.Frame('Pitch (degrees)',[[sg.Text("N/A",key="--pitch--",font=FONT_SIZE),sg.Push(),sg.Text("°",font=FONT_SIZE)]])],
    [sg.Frame('Roll (degrees)',[[sg.Text("N/A",key="--roll--",font=FONT_SIZE),sg.Push(),sg.Text("°",font=FONT_SIZE)]])]])
LAYOUT = [
    [sg.Text('FlyPi Attitude Indicator'), sg.Push(), sg.Text('             Author: Nate Lao (lao.nathan@yahoo.com)')],
    [sg.Image(filename='',key='--viewport--'),RAW_DATA_COLUMN]
    ]

if __name__ == "__main__":
    window = sg.Window('FlyPi Attitude Indicator', LAYOUT, finalize=True)
    # Setup Receiver
    receiver = lib.ETHERNET_ADAPTER()

    ########## Event Loop ##########
    persistance = 0
    while True:
        # Check to see if the cancel button was clicked and exit loop if clicked
        event, values = window.read(timeout=0)
        if event == 'Cancel' or event == sg.WIN_CLOSED:
            break

        # Poll Rx
        payload = receiver.recv()

        # Apply Hysteresis on Input
        if payload != None:
            cache = payload
            persistance = HYSTERESIS
        elif persistance > 0:
            payload = cache
            persistance -= 1

        # Render/Update Values
        if payload != None:
            # Unpack payload
            yaw, pitch, roll, validity = payload

            # Update GUI
            window["--yaw--"].update(   "{:.2f}".format(yaw)    if (validity & 0x1 > 0) else "--")
            window["--pitch--"].update( "{:.2f}".format(pitch)  if (validity & 0x2 > 0) else "--")
            window["--roll--"].update(  "{:.2f}".format(roll)   if (validity & 0x4 > 0) else "--")

            # Update Instrumentation
            img = gui.attitude_indicator_instrument(pitch, roll, 50)
            imgbytes = cv2.imencode('.png', img)[1].tobytes() # this is faster, shorter and needs less includes
            window['--viewport--'].update(data=imgbytes)
        else:
            # Invalidate GUI
            window["--yaw--"].update(   "--")
            window["--pitch--"].update( "--")
            window["--roll--"].update(  "--")

            # INOP Instrumentation
            img = gui.attitude_indicator_inop(50)
            imgbytes = cv2.imencode('.png', img)[1].tobytes() # this is faster, shorter and needs less includes
            window['--viewport--'].update(data=imgbytes)
            
    # Loop Terminated - Kill Window
    window.close()
else:
    print("This application cannot be imported, please execute directly.")