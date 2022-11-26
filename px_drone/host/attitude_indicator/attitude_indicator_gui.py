#!/usr/bin/env python
import cv2
import numpy as np

INST_DIMENSION = 512


INST_SKY_COLOR = (238,176,8)
INST_GROUND_COLOR = (26,45,54)
INST_MARKER_COLOR = (255,255,255)
INST_INDICATOR_COLOR = (30,100,255)


'''
cv2.rectangle(output,(128,128),(384,384),(0,255,0),3)
M = cv2.getRotationMatrix2D((INST_DIMENSION/2, INST_DIMENSION/2),-30,1)
output = cv2.warpAffine(output,M,(INST_DIMENSION,INST_DIMENSION))
'''

def attitude_indicator_instrument(pitch: float, roll: float):
    output = np.zeros((INST_DIMENSION,INST_DIMENSION,3), np.uint8)
    output = __render_pitch(output, pitch)
    output = __render_roll(output, roll)
    output = __render_indicators(output)
    output = __render_final(output)
    return output

# Outer Circle Instrument Mask
__outer_circle_mask = np.zeros((INST_DIMENSION,INST_DIMENSION), np.uint8)
cv2.circle(__outer_circle_mask,\
        (INST_DIMENSION // 2, INST_DIMENSION // 2), \
        INST_DIMENSION // 2, 255, -1)
cv2.circle(__outer_circle_mask,\
        (INST_DIMENSION // 2, INST_DIMENSION // 2), \
        7 * (INST_DIMENSION // 18), 0, -1) # Diameter is 7/9 -> convert to radians
def __render_pitch(src: np.ndarray, pitch: float) -> np.ndarray:
    output = src
    outer = np.zeros((INST_DIMENSION,INST_DIMENSION,3), np.uint8)
    inner = np.zeros((INST_DIMENSION,INST_DIMENSION,3), np.uint8)

    # Outer circle
    cv2.rectangle(outer,(0,0),(INST_DIMENSION,INST_DIMENSION),INST_SKY_COLOR,-1)
    cv2.rectangle(outer,(0,INST_DIMENSION // 2),(INST_DIMENSION,INST_DIMENSION),INST_GROUND_COLOR,-1)
    outer = cv2.bitwise_and(outer, outer, mask=__outer_circle_mask)

    # Inner circle
    #cv2.circle(output, \
    #    (INST_DIMENSION // 2, INST_DIMENSION // 2), \
    #    INST_INNER_DIAMETER // 2, \
    #    INST_SKY_COLOR, -1)

    return output

def __render_roll(src: np.ndarray, roll: float) -> np.ndarray:
    output = src

    return output

__indicator_ptr = np.array([
    [INST_DIMENSION // 2 - 3,   (INST_DIMENSION // 9) + 4 ],    # Top Point
    [INST_DIMENSION // 2 - 16,  (INST_DIMENSION // 9) + 37],    # Left Corner
    [INST_DIMENSION // 2 + 10,  (INST_DIMENSION // 9) + 37]     # Right Corner
    ],np.int32)
__indicator_ptr = __indicator_ptr.reshape((-1,1,2))
def __render_indicators(src: np.ndarray) -> np.ndarray:
    output = src

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

def __render_final(src: np.ndarray) -> np.ndarray:
    output = src
    return output

if __name__ == "__main__":
    cv2.imshow("AI", attitude_indicator_instrument(10.0, -30.0))
    cv2.waitKey(0)
    cv2.destroyAllWindows()


