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
    receiver = lib.RECEIVER()

    # TESTING
    validity = 0xff
    yaw = 0.0
    pitch = 0.0
    roll = 0.0
    pitch_inc = True
    roll_inc = True

    ########## Event Loop ##########
    while True:
        # Check to see if the cancel button was clicked and exit loop if clicked
        event, values = window.read(timeout=0)
        if event == 'Cancel' or event == sg.WIN_CLOSED:
            break

        # TESTING
        window["--yaw--"].update(   "{:.2f}".format(yaw)    if (validity & 0x1 > 0) else "--")
        window["--pitch--"].update( "{:.2f}".format(pitch)  if (validity & 0x2 > 0) else "--")
        window["--roll--"].update(  "{:.2f}".format(roll)   if (validity & 0x4 > 0) else "--")

        img = gui.attitude_indicator_instrument(pitch, roll, 50)
        # this is faster, shorter and needs less includes
        imgbytes = cv2.imencode('.png', img)[1].tobytes()
        window['--viewport--'].update(data=imgbytes)

        if True:
            pitch += 0.1 if pitch_inc else -0.1
            if pitch < -45.0: pitch_inc = True
            elif pitch > 45.0: pitch_inc = False
            roll += 0.15 if roll_inc else -0.15
            if roll < -45.0: roll_inc = True
            elif roll > 45.0: roll_inc = False
        # END OF TESTING

        '''
        # Poll Rx
        payload = receiver.recv()

        if payload != None:
            # Unpack payload
            yaw, pitch, roll, validity = payload

            # Update GUI
            window["--yaw--"].update(   "{:.2f}".format(yaw)    if (validity & 0x1 > 0) else "--")
            window["--pitch--"].update( "{:.2f}".format(pitch)  if (validity & 0x2 > 0) else "--")
            window["--roll--"].update(  "{:.2f}".format(roll)   if (validity & 0x4 > 0) else "--")
        else:
            window["--yaw--"].update(   "--")
            window["--pitch--"].update( "--")
            window["--roll--"].update(  "--")
        '''
    # Loop Terminated - Kill Window
    window.close()
else:
    print("This application cannot be imported, please execute directly.")