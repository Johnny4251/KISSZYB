import sys
import os
import cv2

def main(img_file):
    img = cv2.imread(img_file)
    img = cv2.resize(img, (250,250), interpolation=cv2.INTER_LINEAR)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img = (img // 2).astype('uint8')

    f = open(f"{img_file[0:len(img_file)-4]}.zyb", "w")
    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
            f.write(str(img[i,j])+"\n")
    f.close()

if __name__ == "__main__":
    img_file = sys.argv[1]
    if not os.path.exists(img_file):
        raise Exception("File not found")

    main(img_file)
