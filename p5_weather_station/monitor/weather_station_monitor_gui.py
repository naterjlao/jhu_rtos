import PySimpleGUI as sg

sg.theme('Dark Blue')
BAR_MAX = 1000
BAR_DIM = (30,50)

COMM_LED_KEY = "-COMM_LED-"
TEMP_VAL_KEY = "-TEMP_VAL-"
TEMP_BAR_KEY = "-TEMP_BAR-"
HUMD_VAL_KEY = "-HUMD_VAL-"
HUMD_BAR_KEY = "-HUMD_BAR-"

# https://github.com/PySimpleGUI/PySimpleGUI/blob/master/DemoPrograms/Demo_LED_Indicators.py
def LEDIndicator(key=None, radius=30):
    return sg.Graph(canvas_size=(radius, radius),
             graph_bottom_left=(-radius, -radius),
             graph_top_right=(radius, radius),
             pad=(0, 0), key=key)

def SetLED(window, key, color):
    graph = window[key]
    graph.erase()
    graph.draw_circle((0, 0), 12, fill_color=color, line_color=color)

TEMP_COLUMN = [
    [sg.Text('Temperature')],
    [sg.ProgressBar(BAR_MAX, orientation='v', size=BAR_DIM, key=TEMP_BAR_KEY)],
    [sg.Text('N/A',key=TEMP_VAL_KEY)]]

HUM_COLUMN = [
    [sg.Text('Humidity')],
    [sg.ProgressBar(BAR_MAX, orientation='v', size=BAR_DIM, key=HUMD_BAR_KEY)],
    [sg.Text('N/A',key=HUMD_VAL_KEY)]]

LAYOUT = [
    [sg.Text('ESP Weather Station Monitor')],
    [sg.Text('COMM STATUS'), LEDIndicator(COMM_LED_KEY)],
    [sg.Column(TEMP_COLUMN,justification='center',vertical_alignment='center'), sg.VSeperator(), sg.Column(HUM_COLUMN,justification='center')],
    [sg.Cancel()]]