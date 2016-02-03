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
      self.selectedCam = "fw"
      self.manualMode = False

      self.root = tk.Tk()
      self.root.title("Camera settings")
      self.frame = tk.Frame(self.root)
      self.frame.pack(fill=tk.X, side=tk.BOTTOM)
      
      self.forward = tk.Button(self.frame, text = "Forward", command= lambda: self.button("forward"))
      self.load = tk.Button(self.frame, text = " Load ", command= lambda: self.button("load"))
      self.reverse = tk.Button(self.frame, text = "Reverse", command= lambda: self.button("reverse"))
      self.left = tk.Button(self.frame, text = "Left", command= lambda: self.button("left"))
      self.right = tk.Button(self.frame, text = "Right", command= lambda: self.button("right"))
      

      self.toggle = tk.Button(self.frame, text = "Auto/Man", command= lambda: self.toggleMode())
      self.all = tk.Button(self.frame, text = "All", command= lambda: self.camSelector("all"))
      self.fw = tk.Button(self.frame, text = "^", command= lambda: self.camSelector("fw"))
      self.ri = tk.Button(self.frame, text = ">", command= lambda: self.camSelector("ri"))
      self.re = tk.Button(self.frame, text = "v", command= lambda: self.camSelector("re"))    
      self.le = tk.Button(self.frame, text = "<", command= lambda: self.camSelector("le"))
	

      self.frame.columnconfigure(0, weight=1)
      self.frame.columnconfigure(1, weight=1)
      self.frame.columnconfigure(2, weight=1)
      self.frame.columnconfigure(3, weight=1)
      self.frame.columnconfigure(4, weight=1)
      self.frame.columnconfigure(5, weight=1)

      self.forward.grid(row=0, column=1)
      self.load.grid(row=1, column=1)
      self.reverse.grid(row=2, column=1)
      self.left.grid(row=1, column=0)
      self.right.grid(row=1, column=2)
      
      self.toggle.grid(row=3, column=0)
      self.all.grid(row=3, column=1)
      self.fw.grid(row=3, column=2)
      self.ri.grid(row=3, column=3)
      self.re.grid(row=3, column=4)
      self.le.grid(row=3, column=5)

      #self.root.geometry(GRID_W + "x" + GRID_H)
      #self.canvas = tk.Canvas(self.root, width=int(GRID_W), height=int(GRID_H), borderwidth=0,  highlightthickness=0, bg="black")
      #self.canvas.grid()
      #self.b = tk.Button(self.canvas, text="Button1", command=self.button_1)
      #self.b.pack()
      self.root.mainloop()


   def turnDirBlack(self):
      self.fw.configure(fg = "BLACK")
      self.ri.configure(fg = "BLACK")
      self.re.configure(fg = "BLACK")
      self.le.configure(fg = "BLACK")
      self.all.configure(fg = "BLACK")

   def toggleMode(self):
      if self.manualMode:
         self.manualMode = False
	 self.toggle.configure(fg = "BLACK")
	 self.turnDirBlack()

      else:
         self.manualMode = True
         self.toggle.configure(fg = "GREEN")

   def camSelector(self, cam):
      if self.manualMode:
         self.selectedCam = cam
         self.turnDirBlack()
         
         if cam == "all":
            self.all.configure(fg = "GREEN")

         elif cam == "fw":
            self.fw.configure(fg = "GREEN")

	 elif cam == "ri":
            self.ri.configure(fg = "GREEN")

	 elif cam == "re":
            self.re.configure(fg = "GREEN")

	 elif cam == "le":
            self.le.configure(fg = "GREEN")


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
         
         if not self.manualMode:
	    if direction == "forward":
               self.url1 = "http://192.168.10.4/set?qp=16"
               self.url2 = "http://192.168.10.2/set?qp=16"
               self.url3 = "http://192.168.10.4/set?res=full"
               self.url4 = "http://192.168.10.2/set?resolution=full"
	    

            elif direction == "reverse":
               self.url1 = "http://192.168.10.4/set?qp=32"
               self.url2 = "http://192.168.10.2/set?qp=32"
               self.url3 = "http://192.168.10.4/set?res=half"
               self.url4 = "http://192.168.10.2/set?res=half"


            elif direction == "left":
	       self.url = "http://192.168.10.2/set?rotate=%s" % self.orientation

	    elif direction == "right":
	       self.url = "http://192.168.10.2/set?rotate=%s" % self.orientation

	    else:
	       return

            self.response1 = urllib2.urlopen('%s' % self.url1, timeout = 2)
	    self.response2 = urllib2.urlopen('%s' % self.url2, timeout = 2)
            self.response3 = urllib2.urlopen('%s' % self.url3, timeout = 2)
	    self.response4 = urllib2.urlopen('%s' % self.url4, timeout = 2) 
         
      except Exception as e:
         print (type(e))



   #root.bind_all('<Key>', onKeyPress)




gui_1 = Gui(GRID_W, GRID_H)
thread_1 = Thread(target = gui_1)
thread_1.start()
#thread_2 = Thread(call(["./mixerh264", "0", "0", "0"]))
#thread_2.start()



