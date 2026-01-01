
# ===================== Smart Hotel AIoT - OpenMV H7 =====================
# Person/Face counter - sends count over UART in format: COUNT:<n>

import sensor, time, image
from pyb import UART

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

uart = UART(3, 115200)  # UART3 on OpenMV (check board pins)

face_cascade = image.HaarCascade("frontalface", stages=25)

while True:
    img = sensor.snapshot()
    faces = img.find_features(face_cascade, threshold=0.7, scale=1.35)
    # Debug: draw rectangles
    for r in faces:
        img.draw_rectangle(r, color=(255, 0, 0), thickness=2)
    count = len(faces)
    uart.write("COUNT:{}
".format(count))
    time.sleep_ms(200)
