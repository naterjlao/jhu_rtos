#!/usr/bin/env python
import cv2
import numpy as np

INST_DIMENSION = 512

INST_SKY_COLOR = (238,176,8)
INST_GROUND_COLOR = (26,45,54)
INST_MARKER_COLOR = (255,255,255)
INST_INDICATOR_COLOR = (30,100,255)

def attitude_indicator_inop(scale):
    output = np.zeros((INST_DIMENSION,INST_DIMENSION,3), np.uint8)
    cv2.line(output, (0,0),  (INST_DIMENSION,INST_DIMENSION),(0,0,255),2)
    cv2.line(output, (0,INST_DIMENSION),  (INST_DIMENSION,0),(0,0,255),2)
    if scale < 100 and scale > 0:
        scale_dim = int(INST_DIMENSION * scale / 100.0)
        scale_dim = (scale_dim, scale_dim)
        output = cv2.resize(output, scale_dim, interpolation=cv2.INTER_AREA)
    return output

def attitude_indicator_instrument(pitch: float, roll: float, scale:int = 100):
    output = np.zeros((INST_DIMENSION,INST_DIMENSION,3), np.uint8)
    output = __render_pitch(output, pitch)
    output = __render_roll(output, roll)
    if scale < 100 and scale > 0:
        scale_dim = int(INST_DIMENSION * scale / 100.0)
        scale_dim = (scale_dim, scale_dim)
        output = cv2.resize(output, scale_dim, interpolation=cv2.INTER_AREA)
    return output

# Inner/Outer Circle Instrument Mask
__inner_circle_mask = np.zeros((INST_DIMENSION,INST_DIMENSION), np.uint8)
cv2.circle(__inner_circle_mask,\
        (INST_DIMENSION // 2, INST_DIMENSION // 2), \
        7 * (INST_DIMENSION // 18), 255, -1) # Diameter is 7/9 -> convert to radians
__outer_circle_mask = np.zeros((INST_DIMENSION,INST_DIMENSION), np.uint8)
cv2.circle(__outer_circle_mask,\
        (INST_DIMENSION // 2, INST_DIMENSION // 2), \
        INST_DIMENSION // 2, 255, -1)
cv2.circle(__outer_circle_mask,\
        (INST_DIMENSION // 2, INST_DIMENSION // 2), \
        7 * (INST_DIMENSION // 18), 0, -1) # Diameter is 7/9 -> convert to radians

# Displayed pitch is directly proportional to the raw pitch, whereas 90 degrees is maximum
__pitch_displacement = lambda pitch: int(INST_DIMENSION * (pitch / 90.0))

# Prerendered Roll Indicators
__roll_x_coor = lambda radius, roll: (INST_DIMENSION // 2) + int((radius) * np.sin(roll * np.pi / 180.0))
__roll_y_coor = lambda radius, roll: (INST_DIMENSION // 2) - int((radius) * np.cos(roll * np.pi / 180.0))
__center_roll_ptr = np.array([
    [INST_DIMENSION // 2 - 3,   (INST_DIMENSION // 9) + 4 ],    # Top Point
    [INST_DIMENSION // 2 - 18,  (INST_DIMENSION // 9) - 45],    # Left Corner
    [INST_DIMENSION // 2 + 12,  (INST_DIMENSION // 9) - 45]     # Right Corner
    ],np.int32)
__r10_roll_displacement = (__roll_x_coor((INST_DIMENSION / 2) - 30, 10.0),__roll_y_coor((INST_DIMENSION / 2) - 30, 10.0))
__r20_roll_displacement = (__roll_x_coor((INST_DIMENSION / 2) - 30, 20.0),__roll_y_coor((INST_DIMENSION / 2) - 30, 20.0))
__r30_roll_displacement = (__roll_x_coor((INST_DIMENSION / 2) - 10, 30.0),__roll_y_coor((INST_DIMENSION / 2) - 10, 30.0))
__r60_roll_displacement = (__roll_x_coor((INST_DIMENSION / 2) - 10, 60.0),__roll_y_coor((INST_DIMENSION / 2) - 10, 60.0))
__l10_roll_displacement = (__roll_x_coor((INST_DIMENSION / 2) - 30, -10.0),__roll_y_coor((INST_DIMENSION / 2) - 30, -10.0))
__l20_roll_displacement = (__roll_x_coor((INST_DIMENSION / 2) - 30, -20.0),__roll_y_coor((INST_DIMENSION / 2) - 30, -20.0))
__l30_roll_displacement = (__roll_x_coor((INST_DIMENSION / 2) - 10, -30.0),__roll_y_coor((INST_DIMENSION / 2) - 10, -30.0))
__l60_roll_displacement = (__roll_x_coor((INST_DIMENSION / 2) - 10, -60.0),__roll_y_coor((INST_DIMENSION / 2) - 10, -60.0))

def __render_pitch(src: np.ndarray, pitch: float) -> np.ndarray:
    output = src

    pitch_pos = INST_DIMENSION // 2 + __pitch_displacement(pitch)    

    # TODO this is kinda inefficient since redrawing all these shapes occur at every frame.
    # For now, this is good enough

    # Outer circle
    outer = np.zeros((INST_DIMENSION,INST_DIMENSION,3), np.uint8)
    cv2.rectangle(outer,(0,0),(INST_DIMENSION,INST_DIMENSION),INST_SKY_COLOR,-1)
    cv2.rectangle(outer,(0,INST_DIMENSION // 2),(INST_DIMENSION,INST_DIMENSION),INST_GROUND_COLOR,-1)
    cv2.circle(outer,(INST_DIMENSION // 2, INST_DIMENSION // 2), 7 * (INST_DIMENSION // 18) + 1, (128,128,128), -1)
    cv2.line(outer, (0,INST_DIMENSION//2), (INST_DIMENSION,INST_DIMENSION //2),INST_MARKER_COLOR,6)
    cv2.line(outer, (INST_DIMENSION//2, INST_DIMENSION//2), __r10_roll_displacement,INST_MARKER_COLOR,3)
    cv2.line(outer, (INST_DIMENSION//2, INST_DIMENSION//2), __r20_roll_displacement,INST_MARKER_COLOR,3)
    cv2.line(outer, (INST_DIMENSION//2, INST_DIMENSION//2), __r30_roll_displacement,INST_MARKER_COLOR,5)
    cv2.line(outer, (INST_DIMENSION//2, INST_DIMENSION//2), __r60_roll_displacement,INST_MARKER_COLOR,5)
    cv2.line(outer, (INST_DIMENSION//2, INST_DIMENSION//2), __l10_roll_displacement,INST_MARKER_COLOR,3)
    cv2.line(outer, (INST_DIMENSION//2, INST_DIMENSION//2), __l20_roll_displacement,INST_MARKER_COLOR,3)
    cv2.line(outer, (INST_DIMENSION//2, INST_DIMENSION//2), __l30_roll_displacement,INST_MARKER_COLOR,5)
    cv2.line(outer, (INST_DIMENSION//2, INST_DIMENSION//2), __l60_roll_displacement,INST_MARKER_COLOR,5)
    # Roll Pointer
    output = cv2.fillPoly(output, [__center_roll_ptr], INST_INDICATOR_COLOR)
    outer = cv2.bitwise_and(outer, outer, mask=__outer_circle_mask)

    # Inner circle
    inner = np.zeros((INST_DIMENSION,INST_DIMENSION,3), np.uint8)
    cv2.rectangle(inner,(0,0),(INST_DIMENSION,INST_DIMENSION),INST_SKY_COLOR,-1)
    cv2.rectangle(inner,(0,pitch_pos),(INST_DIMENSION,INST_DIMENSION),INST_GROUND_COLOR,-1)
    cv2.line(inner, (0,pitch_pos), (INST_DIMENSION,pitch_pos),INST_MARKER_COLOR,2)
    cv2.line(inner, (INST_DIMENSION // 2 - 60,pitch_pos + __pitch_displacement(-20.0)), (INST_DIMENSION // 2 + 60,pitch_pos + __pitch_displacement(-20.0)),INST_MARKER_COLOR,2)
    cv2.putText(inner, '20', (INST_DIMENSION // 2 - 90,pitch_pos + __pitch_displacement(-20.0) + 5),cv2.FONT_HERSHEY_SIMPLEX,0.5,INST_MARKER_COLOR,1,cv2.LINE_AA)
    cv2.putText(inner, '20', (INST_DIMENSION // 2 + 70,pitch_pos + __pitch_displacement(-20.0) + 5),cv2.FONT_HERSHEY_SIMPLEX,0.5,INST_MARKER_COLOR,1,cv2.LINE_AA)
    cv2.line(inner, (INST_DIMENSION // 2 - 15,pitch_pos + __pitch_displacement(-15.0)), (INST_DIMENSION // 2 + 15,pitch_pos + __pitch_displacement(-15.0)),INST_MARKER_COLOR,2)
    cv2.line(inner, (INST_DIMENSION // 2 - 30,pitch_pos + __pitch_displacement(-10.0)), (INST_DIMENSION // 2 + 30,pitch_pos + __pitch_displacement(-10.0)),INST_MARKER_COLOR,2)
    cv2.putText(inner, '10', (INST_DIMENSION // 2 - 60,pitch_pos + __pitch_displacement(-10.0) + 5),cv2.FONT_HERSHEY_SIMPLEX,0.5,INST_MARKER_COLOR,1,cv2.LINE_AA)
    cv2.putText(inner, '10', (INST_DIMENSION // 2 + 40,pitch_pos + __pitch_displacement(-10.0) + 5),cv2.FONT_HERSHEY_SIMPLEX,0.5,INST_MARKER_COLOR,1,cv2.LINE_AA)
    cv2.line(inner, (INST_DIMENSION // 2 - 15,pitch_pos + __pitch_displacement(-5.0)), (INST_DIMENSION // 2 + 15,pitch_pos + __pitch_displacement(-5.0)),INST_MARKER_COLOR,2)
    cv2.line(inner, (INST_DIMENSION // 2 - 15,pitch_pos + __pitch_displacement(5.0)),  (INST_DIMENSION // 2 + 15,pitch_pos + __pitch_displacement(5.0)),INST_MARKER_COLOR,2)
    cv2.line(inner, (INST_DIMENSION // 2 - 30,pitch_pos + __pitch_displacement(10.0)),  (INST_DIMENSION // 2 + 30,pitch_pos + __pitch_displacement(10.0)),INST_MARKER_COLOR,2)
    cv2.putText(inner, '10', (INST_DIMENSION // 2 - 60,pitch_pos + __pitch_displacement(10.0) + 5),cv2.FONT_HERSHEY_SIMPLEX,0.5,INST_MARKER_COLOR,1,cv2.LINE_AA)
    cv2.putText(inner, '10', (INST_DIMENSION // 2 + 40,pitch_pos + __pitch_displacement(10.0) + 5),cv2.FONT_HERSHEY_SIMPLEX,0.5,INST_MARKER_COLOR,1,cv2.LINE_AA)
    cv2.line(inner, (INST_DIMENSION // 2 - 15,pitch_pos + __pitch_displacement(15.0)),  (INST_DIMENSION // 2 + 15,pitch_pos + __pitch_displacement(15.0)),INST_MARKER_COLOR,2)
    cv2.line(inner, (INST_DIMENSION // 2 - 60,pitch_pos + __pitch_displacement(20.0)),  (INST_DIMENSION // 2 + 60,pitch_pos + __pitch_displacement(20.0)),INST_MARKER_COLOR,2)
    cv2.putText(inner, '20', (INST_DIMENSION // 2 - 90,pitch_pos + __pitch_displacement(20.0) + 5),cv2.FONT_HERSHEY_SIMPLEX,0.5,INST_MARKER_COLOR,1,cv2.LINE_AA)
    cv2.putText(inner, '20', (INST_DIMENSION // 2 + 70,pitch_pos + __pitch_displacement(20.0) + 5),cv2.FONT_HERSHEY_SIMPLEX,0.5,INST_MARKER_COLOR,1,cv2.LINE_AA)
    inner = cv2.bitwise_and(inner, inner, mask=__inner_circle_mask)

    output = cv2.add(output, outer)
    output = cv2.add(output, inner)

    return output

__indicator_ptr = np.array([
    [INST_DIMENSION // 2 - 3,   (INST_DIMENSION // 9) + 4 ],    # Top Point
    [INST_DIMENSION // 2 - 16,  (INST_DIMENSION // 9) + 37],    # Left Corner
    [INST_DIMENSION // 2 + 10,  (INST_DIMENSION // 9) + 37]     # Right Corner
    ],np.int32)
__indicator_ptr = __indicator_ptr.reshape((-1,1,2))
def __render_roll(src: np.ndarray, roll: float) -> np.ndarray:
    output = src

    # Displayed Roll is inverse of Raw Roll
    M = cv2.getRotationMatrix2D((INST_DIMENSION/2, INST_DIMENSION/2),(-1.0) * roll,1)
    output = cv2.warpAffine(output,M,(INST_DIMENSION,INST_DIMENSION))

    # Pitch Indicator
    cv2.line(output, \
        ((INST_DIMENSION // 9) * 3, INST_DIMENSION // 2), \
        ((INST_DIMENSION // 9) * 4, INST_DIMENSION // 2), \
        INST_INDICATOR_COLOR, 3)
    cv2.line(output, \
        ((INST_DIMENSION // 9) * 5, INST_DIMENSION // 2), \
        ((INST_DIMENSION // 9) * 6, INST_DIMENSION // 2), \
        INST_INDICATOR_COLOR, 3)
    cv2.circle(output, \
        (INST_DIMENSION // 2 - 2, INST_DIMENSION // 2),
        4, INST_INDICATOR_COLOR, -1)

    # Roll Pointer
    output = cv2.polylines(output, [__indicator_ptr], True, INST_INDICATOR_COLOR, 2)
    return output

if __name__ == "__main__":
    pitch = -45.0
    pitch_inc = True
    roll = 0.0
    roll_inc = True
    while True:
        cv2.imshow("AI", attitude_indicator_instrument(pitch, roll))
        cv2.waitKey(1)
        pitch += 0.5 if pitch_inc else -0.5
        if pitch < -45.0: pitch_inc = True
        elif pitch > 45.0: pitch_inc = False
        roll += 0.2 if roll_inc else -0.2
        if roll < -45.0: roll_inc = True
        elif roll > 45.0: roll_inc = False
    cv2.destroyAllWindows()


