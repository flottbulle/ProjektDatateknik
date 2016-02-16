from subprocess import call
from threading import Thread
import Tkinter as tk
import time
import urllib2


GRID_W = "500"
GRID_H = "500"

class Gui():
   def __init__(self, GRID_W, GRID_H):

      self.selectedCam = "fw"

      self.ipFw = "192.168.10.2"
      self.ipRi = "192.168.10.2"
      self.ipRe = "192.168.10.4"
      self.ipLe = "192.168.10.4"

      self.allCamIps = [self.ipFw, self.ipRi, self.ipRe, self.ipLe]

      self.manualMode = False

      #Create GUI components 
      self.root = tk.Tk()
      self.root.title("Camera settings")
      self.frame = tk.Frame(self.root)
      self.frame.pack(fill=tk.X, side=tk.BOTTOM)

      self.forward = tk.Button(self.frame, text = "Forward", command= lambda: self.button("forward"))
      self.load = tk.Button(self.frame, text = " Load ", command= lambda: self.button("load"))
      self.reverse = tk.Button(self.frame, text = "Reverse", command= lambda: self.button("reverse"))
      self.left = tk.Button(self.frame, text = "Left", command= lambda: self.button("left"))
      self.right = tk.Button(self.frame, text = "Right", command= lambda: self.button("right"))


      self.toggle = tk.Button(self.frame, text = "Manual", command= lambda: self.toggleMode())
      self.all = tk.Button(self.frame, text = "All", command= lambda: self.camSelector("all"))
      self.fw = tk.Button(self.frame, text = "^", command= lambda: self.camSelector("fw"))
      self.ri = tk.Button(self.frame, text = ">", command= lambda: self.camSelector("ri"))
      self.re = tk.Button(self.frame, text = "v", command= lambda: self.camSelector("re"))
      self.le = tk.Button(self.frame, text = "<", command= lambda: self.camSelector("le"))

      self.max = tk.Button(self.frame, text = "MAX", command= lambda: self.manualChange("max"))
      self.min = tk.Button(self.frame, text = "MIN", command= lambda: self.manualChange("min"))


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
      self.max.grid(row=4, column=0)
      self.min.grid(row=4, column=1)

      self.root.mainloop()


   # Set foreground of buttons black
   def turnDirBlack(self):
      self.fw.configure(fg = "BLACK")
      self.ri.configure(fg = "BLACK")
      self.re.configure(fg = "BLACK")
      self.le.configure(fg = "BLACK")
      self.all.configure(fg = "BLACK")

   # Toggle between manual/auto mode
   def toggleMode(self):
      if self.manualMode:
         self.manualMode = False
	 self.toggle.configure(fg = "BLACK")
	 self.turnDirBlack()

      else:
         self.manualMode = True
         self.toggle.configure(fg = "GREEN")

   # Select which cam to control manually
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

   # Not in use right now
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

   # Changes cam parameters by http requests when "driving"
   def button(self, direction):
      try:

         if not self.manualMode:
	    url_list = []
	    if direction == "forward":
               url_list.append("http://%s/set?qp=16" % self.ipRe)
               url_list.append("http://%s/set?qp=16" % self.ipFw)

            elif direction == "reverse":
               url_list.append("http://%s/set?qp=32" % self.ipRe)
               url_list.append("http://%s/set?qp=32" % self.ipFw)

	    elif direction == "left":
	       print ""
	       #url_list.append("http://192.168.10.2/set?rotate=%s" % self.orientation)

	    elif direction == "right":
	       print ""
	       #url_list.append("http://192.168.10.2/set?rotate=%s" % self.orientation)

	    else:
	       return

 	    self.createUrlThreads(url_list)

      except Exception as e:
         print (type(e))


   def returnCamIp(self):
      if self.selectedCam == "fw":
         return self.ipFw
      elif self.selectedCam == "ri":
         return self.ipRi
      elif self.selectedCam == "re":
         return self.ipRe
      elif self.selectedCam == "le":
         return self.ipLe
      elif self.selctedCam == "all":
         return self.allCamIps


   #Changes camera settings of selected cam(s) when in manualMode
   def manualChange(self, action):
      if self.manualMode:

         url_list = []
         if action == "max":

            if self.selectedCam == "all":
               for ip in self.allCamIps:
                  url_list.append("http://%s/set?qp=16" % ip)
	    else:
               url_list.append("http://%s/set?qp=16" % self.returnCamIp())
         
         elif action == "min":

            if self.selectedCam == "all":
               for ip in self.allCamIps:
                  url_list.append("http://%s/set?qp=32" % ip)
            else:
               url_list.append("http://%s/set?qp=32" % self.returnCamIp())

         self.createUrlThreads(url_list)

   # Creates a new thread for each url
   def createUrlThreads(self, url_list):
      for url in url_list:
         #threads = []
         thread = Thread(target = self.executeUrl, args = [url])
         thread.setDaemon(True)
         thread.start()
         #threads.append(thread)

   def executeUrl(self, url):
      print url
      urllib2.urlopen(url, timeout = 2)

   #root.bind_all('<Key>', onKeyPress)

# Calls extern gstreamer application
def startCams():
   call(["./mixerh264", "0", "0", "0"])

thread_1 = Thread(target = startCams)
thread_1.start()
gui = Gui(GRID_W, GRID_H)
