import PySimpleGUI as sg
import attitude_indicator_lib as lib

sg.theme('Dark Blue')

FONT_SIZE = ('Any 30')
LAYOUT = [
    [sg.Text('Attitude Indicator Raw Monitor',font=FONT_SIZE)],
    [sg.Text('Yaw = ',font=FONT_SIZE), sg.Text("N/A",key="--yaw--",font=FONT_SIZE)],
    [sg.Text('Pitch = ',font=FONT_SIZE), sg.Text("N/A",key="--pitch--",font=FONT_SIZE)],
    [sg.Text('Roll = ',font=FONT_SIZE), sg.Text("N/A",key="--roll--",font=FONT_SIZE)],
    ]

if __name__ == "__main__":
    window = sg.Window('Attitude Indicator Raw Monitor', LAYOUT, finalize=True)
    # Setup Receiver
    receiver = lib.RECEIVER()

    ########## Event Loop ##########
    while True:
        # Check to see if the cancel button was clicked and exit loop if clicked
        event, values = window.read(timeout=10)
        if event == 'Cancel' or event == sg.WIN_CLOSED:
            break
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
    # Loop Terminated - Kill Window
    window.close()
else:
    print("This application cannot be imported, please execute directly.")