import Image
import ImageColor
import time

centerX = 0.28693186889504513
centerY = 0.014286693904085048

zoomIterations = 200
finalZoom = 10000000
width = 192
height = 108
startRangeY = 0.1
startRangeX = startRangeY * float(width) / float(height)
max_iteration = 2000
colour = [20, 125, 19]

img = Image.new('RGB', (width, height))
pixels = img.load()

#Create 2D array to save iteration values
iSpace = [[0 for y in range(height)] for x in range(width)]

#0-based counting
width = width - 1
height = height - 1

def cardioidCheck(xin,yin):
    #Cardioid check
    q = (xin - 0.25)*(xin-0.25) + yin*yin
    if (q * (q + (xin-0.25)) < 0.25*yin*yin):
        return False
        #print "In Cardioid"
    #Period-2 bulb check
    elif (xin+1)*(xin+1) + yin*yin < (1/16):
        return False
        #print "In bulb"
    else:
        return True
        #print "Not in cardioid or bulb"

for zoomLevel in range(0,zoomIterations):
    zoomLevel = float(zoomLevel)
    currentZoom = finalZoom**(zoomLevel/(zoomIterations-1))
    print "Computing image " + str(int(zoomLevel+1)) + " at x" + str(currentZoom) + " zoom"
    rangeX = startRangeX / currentZoom
    rangeY = startRangeY / currentZoom
    startX = centerX  - (rangeX / 2)
    startY = centerY - (rangeY / 2)
    for pixel_x in range(0,width):
        for pixel_y in range(0,height):
            x0 = 0.0
            y0 = 0.0
            x0 = (pixel_x * rangeX) / width + startX
            y0 = (pixel_y * rangeY) / height + startY
            if (pixel_y==0 and pixel_x%50==0):
                print "Pixels: ("+str(pixel_x)+","+str(pixel_y)+") Coords: ("+str(x0)+","+str(y0)+")"
            x = 0.0
            y = 0.0
            xSqr = 0.0
            ySqr = 0.0
            i = -1
            if cardioidCheck(x0,y0):
                while (xSqr + ySqr < 4 and i < max_iteration):
                    xtemp = xSqr - ySqr + x0
                    ytemp = 2*x*y + y0
                    x = xtemp
                    y = ytemp
                    xSqr = x * x
                    ySqr = y * y
                    i = i + 1
            else:
                i = max_iteration
            iSpace[pixel_x][pixel_y]=i
    #Create histogram and total area underneath
    total = 0
    histogram = [0 for i in range(max_iteration+1)]
    for x in range(0,width):
        for y in range(0,height):
            #print "Dealing with pixel ("+str(x)+","+str(y)+") with i-value "+str(iSpace[x][y])
            histogram[iSpace[x][y]] = histogram[iSpace[x][y]] + 1
            total = total + 1
    #Decide colour for each iteration value
    hue = [0.0 for i in range(max_iteration+1)]
    for i in range(0,max_iteration):
        hue[i+1] = hue[i] + (float(histogram[i+1]) / float(total))
    #print str(hue[max_iteration])
    #Colour in image
    for x in range(0,width):
        for y in range(0,height):
            pixels[x,y] = ImageColor.getrgb("hsl(235,100%,"+str(int(hue[iSpace[x][y]] * 100))+"%)")
    #Save image
    img.save("ZoomOutput\output"+str(int(zoomLevel+1)).zfill(3)+".bmp")
    print "Image "+str(int(zoomLevel+1))+" complete"
    #time.sleep(3)

