# Attitude Indicator
# Author: Nate Lao (nlao1@jh.edu)
# Designed for Windows OS
# GUI Driver for AI measurements
import PySimpleGUI as sg
import numpy as np
import cv2
import viewport_lib as lib

sg.theme('Dark Blue')

HYSTERESIS = 5
FONT_SIZE = ('Any 30')
LAYOUT = [
    [sg.Text('FlyPi ViewPort'), sg.Push(), sg.Text('             Author: Nate Lao (lao.nathan@yahoo.com)')],
    [sg.Image(filename='',key='--viewport--')]
    ]

# RED X IF NO TRANSMISSION RECEIVED
INOP_SCREEN = np.zeros((480,640,3), np.uint8)
cv2.line(INOP_SCREEN, (0,0),  (640,480),(0,0,255),2)
cv2.line(INOP_SCREEN, (0,480),  (640,0),(0,0,255),2)

if __name__ == "__main__":
    window = sg.Window('FlyPi ViewPort', LAYOUT, finalize=True)
    # Setup Receiver
    receiver = lib.ETHERNET_ADAPTER_CAM()

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

        # Show Live Footage
        if payload != None:
            # NOTE Unfortunately, PySimpleGUI does not support JPG,
            # the footage has to be converted to PNG.
            conversion = np.frombuffer(payload, np.uint8)
            img = cv2.imdecode(conversion, cv2.IMREAD_UNCHANGED)
            imgbytes = cv2.imencode('.png', img)[1].tobytes() # this is faster, shorter and needs less includes
            window['--viewport--'].update(data=imgbytes)
        else:
            imgbytes = cv2.imencode('.png', INOP_SCREEN)[1].tobytes() # this is faster, shorter and needs less includes
            window['--viewport--'].update(data=imgbytes)
            
    # Loop Terminated - Kill Window
    window.close()
else:
    print("This application cannot be imported, please execute directly.")