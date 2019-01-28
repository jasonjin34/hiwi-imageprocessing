import cv2
import numpy as np
from matplotlib import pyplot as plt

##### color pic histogram equalization 
def show_hsv_equalized(image):
    H, S, V = cv2.split(cv2.cvtColor(image, cv2.COLOR_BGR2HSV))
    eq_V = cv2.equalizeHist(V)
    eq_image = cv2.cvtColor(cv2.merge([H, S, eq_V]), cv2.COLOR_HSV2RGB)
    plt.imshow(eq_image)
    cv2.imwrite('C:/HIWI/images/transmission/equalized_B.jpg',eq_image)
    plt.show()

image_color = cv2.imread("C:/HIWI/images/transmission/B.png",-1)
show_hsv_equalized(image_color); 

equalized_img = cv2.imread('C:/HIWI/test-hiwi/equalized.jpg',0)
#plt.hist(equalized_img.ravel(),256,[0,256]); 

unequalized_img = cv2.imread('C:/HIWI/test-hiwi/unequalized.jpg',0)
#plt.hist(unequalized_img.ravel(),256,[0,256]);


img = cv2.imread('C:/HIWI/images/transmission/equalized_B.jpg')
color = ('b','g','r')
for i,col in enumerate(color):
    histr = cv2.calcHist([img],[i],None,[256],[0,256])
    plt.plot(histr,color = col)
    plt.xlim([0,256])
    plt.show()


plt.title('Histogram equalization')
plt.ylabel('Frequency')
plt.xlabel('pixel value')

plt.show()
