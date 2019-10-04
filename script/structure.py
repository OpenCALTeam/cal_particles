
import drawSvg as draw
import sys
from PyQt4 import QtGui, QtSvg

width = 240
height = 240
delta_y = 15

delta_x = 17

ray = 8
d = draw.Drawing(width, height)

startX= 10
startY= 10
nParticle = 15


currentX = startX
currentY = startY

#d.append(draw.Rectangle(0,0,40,50, fill='gray', stroke_width=1, stroke='black', fill_opacity=0.5))
#d.append(draw.Text('Hyperlink',5,10,10,  fill='white'))

string = ["1", "2", "3", "4", "5","6","7", "8", "9","10","...", "n-1", "n"]
startIndex = 1

fontSize = 4

for i in range (1, nParticle):

    #d.append(draw.Text('Hyperlink',3,0,startY+delta_y*1.5,  fill='black'))
    for j in range(1, startIndex):
        d.append(draw.Rectangle(currentX,currentY,delta_x,delta_y, fill='gray', stroke_width=0.9, stroke='black', fill_opacity=0.3))
        if(i>5):
            fontSize = 4
        else:
            fontSize = 3.1

        if(i == 4):
            d.append(draw.Text("(..,..)", fontSize, currentX +  delta_x*0.15,currentY+ delta_y *0.4, fill='black'))

        else:
            d.append(draw.Text("(%s,%s)" % (string[nParticle - i - 1], string[nParticle+ j- startIndex-2]), fontSize, currentX + delta_x * 0.15,
                               currentY + delta_y * 0.4, fill='black'))
        currentX = currentX + delta_x

    currentY = currentY + delta_y
    currentX = delta_x
    startIndex += 1

for i in range(1, len(string)+1):
    d.append(draw.Text(string[i-1], 4, delta_x/2.8, currentY - delta_y*i + delta_y *0.4, fill='black'))

currentX = startX + delta_x/1.2
#for i in range(1, len(string)+1):
    #d.append(draw.Text(string[i-1], 4, currentX, currentY+ delta_y *0.2, fill='black'))
    #currentX = currentX + delta_x






d.setPixelScale(2)  # Set number of pixels per geometry unit
#d.setRenderSize(400,200)  # Alternative to setPixelScale
d.saveSvg('structure.svg')
#d.savePng('grid.png')

#cairosvg.svg2png(url='grid.svg', write_to='grid.png')

app = QtGui.QApplication(sys.argv)
svgWidget = QtSvg.QSvgWidget('structure.svg')
svgWidget.setGeometry(100,100,width*4,height*4)
svgWidget.show()

sys.exit(app.exec_())

