import sys
import os
import cv2
import numpy as np

"""
Turns img into .zyb file.
ZYB files are newline seperated pixel values
with intensity ranges from 0-127.
"""
def encode_image(img_file):
    img = cv2.imread(img_file)
    img = cv2.resize(img, (250,250), interpolation=cv2.INTER_LINEAR)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img = (img // 2).astype('uint8')
    
    f = open(f"{img_file[0:len(img_file)-4]}.zyb", "w")
    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
            f.write(str(img[i,j])+"\n")
    f.close()


"""
Decodes .zyb into .png. 
"""
def decode_image(encoded_file, resize=False):
    with open(encoded_file, 'r') as file:
        lines = file.read().splitlines()
    
    pixel_values = list(map(int, lines))
    
    width = 250
    height = len(pixel_values) // width

    img_array = np.array(pixel_values[:height * width]).reshape((height, width))
    
    img_array = (img_array * 2).astype('uint8')
    
    if resize:
        img_array = cv2.resize(img_array, (500, 500), interpolation=cv2.INTER_LINEAR)
    
    img_array = cv2.cvtColor(img_array, cv2.COLOR_GRAY2BGR)
    cv2.imwrite((f"decoded_{encoded_file[0:len(encoded_file)-4]}.png"), img_array)

def main():
    if len(sys.argv) < 3:
        print(f"USAGE: {sys.argv[0]} <img_file> [encode|decode]")
        return
    img_file = sys.argv[1]
    if not os.path.exists(img_file):
        raise Exception("File not found")

    if sys.argv[2] == "decode":
        resize = False
        if len(sys.argv) > 3:
            if sys.argv[3].lower() == "resize":
                resize = True
        decode_image(img_file, resize)    
    elif sys.argv[2] == "encode":
        encode_image(img_file)
    else:
        print(f"USAGE: {sys.argv[0]} <img_file> [encode|decode]")

if __name__ == "__main__":
    main()
