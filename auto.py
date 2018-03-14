"""Detect speed limits from webcam feed"""
import re
import os
import sys
import cv2
import time
import threading
import argparse
from imutils.video import VideoStream
from imutils.video import FPS


def read_paths(path):
    """Returns a list of files in given path"""
    images = [[] for _ in range(2)]
    for dirname, dirnames, _ in os.walk(path):
        for subdirname in dirnames:
            filepath = os.path.join(dirname, subdirname)
            for filename in os.listdir(filepath):
                try:
                    imgpath = str(os.path.join(filepath, filename))
                    images[0].append(imgpath)
                    limit = re.findall('[0-9]+', filename)
                    images[1].append(limit[0])
                except (errno, strerror):
                    print("I/O error({0}): {1}")
                except:
                    print("Unexpected error:", sys.exc_info()[0])
                    raise
    return images

def load_images(imgpath):
    """Loads images in given path and returns
     a list containing image and keypoints"""
    images = read_paths(imgpath)
    imglist = [[], [], [], []]
    cur_img = 0
    sift = cv2.xfeatures2d.SIFT_create()
    for i in images[0]:
        img = cv2.imread(i, 0)
        imglist[0].append(img)
        imglist[1].append(images[1][cur_img])
        cur_img += 1
        keypoints, des = sift.detectAndCompute(img, None)
        imglist[2].append(keypoints)
        imglist[3].append(des)
    return imglist

def run_flann(img):
    """Run FLANN-detector for given image with given image list"""
# Find the keypoint descriptors with SIFT
    _, des = SIFT.detectAndCompute(img, None)

    biggest_amnt = 0
    biggest_speed = 0
    cur_img = 0
    
    for _ in IMAGES[0]:
        des2 = IMAGES[3][cur_img]
        matches = FLANN.knnMatch(des2, des, k=2)
        matchamnt = 0
# Find matches with Lowe's ratio test
        for _, (moo, noo) in enumerate(matches):
            if moo.distance < 0.8*noo.distance:
                matchamnt += 1
        if matchamnt > biggest_amnt:
            biggest_amnt = matchamnt
            biggest_speed = IMAGES[1][cur_img]
        cur_img += 1
    if biggest_amnt > 5:
        return biggest_speed, biggest_amnt
    else:
        return "Unknown", 0


IMAGES = load_images("data")

def run_logic():
    """Run TSR and ISA"""
    lastlimit = "00"
    lastdetect = "00"
    downscale = ARGS.DOWNSCALE
    matches = 0
    counte = 0
    prevdetect = "20"
    possiblematch = "00"

    frame = CAP.read()
    print("Camera opened and frame read")

    while True:
        origframe = frame
        if ARGS.MORPH:
            frame = cv2.morphologyEx(
                frame,
                cv2.MORPH_OPEN, 
                cv2.getStructuringElement(cv2.MORPH_RECT,(2,2))
                )
            frame = cv2.morphologyEx(
                frame, 
                cv2.MORPH_CLOSE, 
                cv2.getStructuringElement(cv2.MORPH_RECT,(2,2))
                )
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        scaledsize = (int(frame.shape[1]/int(downscale)), int(frame.shape[0]/int(downscale)))
        scaledframe = cv2.resize(frame, scaledsize)

        # Detect signs in downscaled frame
        
        signs = CLASSIFIER.detectMultiScale(
            scaledframe,
            1.1,
            5,
            0,
            (10, 10),
            (200, 200))
        for sign in signs:
            xpos, ypos, width, height = [ i*downscale for i in sign ]

            crop_img = frame[ypos:ypos+height, xpos:xpos+width]
            sized = cv2.resize(crop_img, (128, 128))
            comp = "Unknown"
            comp, amnt  = run_flann(sized)
            if comp != "Unknown":
                if comp != lastlimit:
                    # Require two consecutive hits for new limit.
                    if comp == lastdetect:
                        possiblematch = comp
                        matches = matches + 1
                        if matches >= ARGS.matches:
                            #print "New speed limit: "+ possiblematch
                            lastlimit = possiblematch
                            matches = 0
                    else:
                        possiblematch = "00"
                        matches = 0
                cv2.rectangle(
                    origframe, 
                    (xpos, ypos), 
                    (xpos+width, ypos+height), 
                    (0, 0, 255))
                cv2.putText(
                    origframe,
                    "Speed limit: "+comp,
                    (xpos,ypos-5),
                    cv2.FONT_HERSHEY_SIMPLEX,
                     0.4,
                    (0,0,255),
                    1,)
            else:
                cv2.rectangle(
                    origframe,
                    (xpos,ypos),
                    (xpos+width,ypos+height),
                    (255,0,0))
                cv2.putText(
                    origframe,
                    "UNKNOWN SPEED LIMIT",
                    (xpos,ypos-5),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.4,
                    (255,0,0),
                    1,)
                comp = lastdetect
            lastdetect = comp
            
            if int(lastdetect) == int(prevdetect):
                counte = counte +1
            else:
                prevdetect = lastdetect
                counte = 0

            if counte >= 10:
                counte = 0
                print(prevdetect)


        if ARGS.PREVIEW:
            cv2.imshow("preview", origframe)

        _ = cv2.waitKey(20)
        if _ == ord("q"):
            break
        frame = CAP.read()

# Preload all classes used in detection
SIFT = cv2.xfeatures2d.SIFT_create()
INDEX_PARAMS = None
SEARCH_PARAMS = None
FLANN = None
CAP = None
ARGS = None

if __name__ == "__main__":
    PARSER = argparse.ArgumentParser(
      description="Traffic sign recognition and intelligent speed assist.",
      )

    PARSER.add_argument("-c", "--cascade", 
        dest="CASCADE", default="lbpCascade.xml")
    PARSER.add_argument("-D", "--downscale", 
        dest="DOWNSCALE", default=1)
    PARSER.add_argument("-m", "--matches", 
        dest="matches", default=10)
    PARSER.add_argument("-M", "--morphopenclose", 
        dest="MORPH", action="store_true", default=False)
    PARSER.add_argument("-s", "--showvid", 
        dest="PREVIEW", action="store_true", default=False)
    ARGS = PARSER.parse_args()


    CAP = VideoStream(usePiCamera=True).start()
    time.sleep(2.0)
    print("Opening Camera")
    CLASSIFIER = cv2.CascadeClassifier(ARGS.CASCADE)
    
    INDEX_PARAMS = dict(algorithm = 0, trees = 5)
    
    SEARCH_PARAMS = dict(checks=50) 
    

    FLANN = cv2.FlannBasedMatcher(INDEX_PARAMS, SEARCH_PARAMS)
    

    if ARGS.PREVIEW:
        cv2.namedWindow("preview")

    run_logic()
