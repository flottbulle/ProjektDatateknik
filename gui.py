from subprocess import call
from threading import Thread
import Tkinter as tk
import time
import urllib2


GRID_W = "500"
GRID_H = "500"

class Gui():
   def __init__(self, GRID_W, GRID_H):
      
      self.orientation = 0
      self.root = tk.Tk()
      self.root.title("Camera settings")
      self.frame = tk.Frame(self.root)
      self.frame.pack(fill=tk.X, side=tk.BOTTOM)
      
      self.forward = tk.Button(self.frame, text = "Forward", command= lambda: self.button("forward"))
      self.reverse = tk.Button(self.frame, text = "Reverse", command= lambda: self.button("reverse"))
      self.left = tk.Button(self.frame, text = "Left", command= lambda: self.button("left"))
      self.right = tk.Button(self.frame, text = "Right", command= lambda: self.button("right"))
      
      self.frame.columnconfigure(0, weight=1)
      self.frame.columnconfigure(1, weight=1)

      self.forward.grid(row=0, column=1)
      self.reverse.grid(row=2, column=1)
      self.left.grid(row=1, column=0)
      self.right.grid(row=1, column=2)

      #self.root.geometry(GRID_W + "x" + GRID_H)
      #self.canvas = tk.Canvas(self.root, width=int(GRID_W), height=int(GRID_H), borderwidth=0,  highlightthickness=0, bg="black")
      #self.canvas.grid()
      #self.b = tk.Button(self.canvas, text="Button1", command=self.button_1)
      #self.b.pack()
      self.root.mainloop()


   def onKeyPress(self, event):
       x = event.char
       if x == "d":
	  print "d"
          #self.canvas.move(a, 2, 0)
       elif x == "a":
          print "a"
          #self.canvas.move(a, -2, 0)
       elif x == "w":
          print "w"
          #self.canvas.move(a, 0, -2)
       elif x == "s":
          print "s"
          #self.canvas.move(a, 0, 2)

   def button(self, direction):
      try:
         if self.orientation == 270:
	    self.orientation = 0
         else:
            self.orientation += 90
         
	 if direction == "forward":
            self.url = "http://192.168.10.4/set?rotate=%s" % self.orientation

         elif direction == "reverse":
	    self.url = "http://192.168.10.4/set?rotate=%s" % self.orientation
 
         elif direction == "left":
	    self.url = "http://192.168.10.2/set?rotate=%s" % self.orientation

	 elif direction == "right":
	    self.url = "http://192.168.10.2/set?rotate=%s" % self.orientation

	 else:
	    return

         self.response = urllib2.urlopen('%s' % self.url, timeout = 2) 
         
      except Exception as e:
         print (type(e))



   #root.bind_all('<Key>', onKeyPress)




gui_1 = Gui(GRID_W, GRID_H)
thread_1 = Thread(target = gui_1)
thread_1.start()
#thread_2 = Thread(call(["./mixerh264", "0", "0", "0"]))
#thread_2.start()



