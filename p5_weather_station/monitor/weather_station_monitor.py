import PySimpleGUI as sg
import weather_station_monitor_gui as gui

if __name__ == "__main__":
    ########## Initialization ##########

    # GUI Window
    window = sg.Window('ESP Weather Station Monitor', gui.LAYOUT, finalize=True)

    # Set Communication Status
    comm_state = 'red'
    gui.SetLED(window, gui.COMM_LED_KEY, 'red')

    ########## Event Loop ##########
    for i in range(1000):
        # Check to see if the cancel button was clicked and exit loop if clicked
        event, values = window.read(timeout=10)
        if event == 'Cancel' or event == sg.WIN_CLOSED:
            break

        # TODO BEBUGGING

        # Update bar with loop value +1 so that bar eventually reaches the maximum
        window[gui.TEMP_BAR_KEY].update(i+1)
        window[gui.TEMP_VAL_KEY].update(i+1)
        window[gui.HUMD_BAR_KEY].update(i+1)
        window[gui.HUMD_VAL_KEY].update(i+1)

        if i % 100 == 0:
            comm_state = 'green' if comm_state == 'red' else 'red'
            print(comm_state,i)
            gui.SetLED(window, gui.COMM_LED_KEY, comm_state)

    # Loop Terminated - Kill Window
    window.close()

else:
    print("This application cannot be imported, please execute directly.")