from subprocess import call
from threading import Thread
import Tkinter as tk
import time
import urllib2

class Gui():
   def __init__(self):
      self.BUTTON_WIDTH = 8
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

      self.forward = tk.Button(self.frame, text = "Forward", command= lambda: self.button("forward"), width = self.BUTTON_WIDTH)
      self.load = tk.Button(self.frame, text = " Load ", command= lambda: self.button("load"), width = self.BUTTON_WIDTH)
      self.reverse = tk.Button(self.frame, text = "Reverse", command= lambda: self.button("reverse"), width = self.BUTTON_WIDTH)
      self.left = tk.Button(self.frame, text = "Left", command= lambda: self.button("left"), width = self.BUTTON_WIDTH)
      self.right = tk.Button(self.frame, text = "Right", command= lambda: self.button("right"), width = self.BUTTON_WIDTH)


      self.toggle = tk.Button(self.frame, text = "Manual", command= lambda: self.toggleMode(), width = self.BUTTON_WIDTH)
      self.all = tk.Button(self.frame, text = "All", command= lambda: self.camSelector("all"), width = self.BUTTON_WIDTH)
      self.fw = tk.Button(self.frame, text = "^", command= lambda: self.camSelector("fw"), width = self.BUTTON_WIDTH)
      self.ri = tk.Button(self.frame, text = ">", command= lambda: self.camSelector("ri"), width = self.BUTTON_WIDTH)
      self.re = tk.Button(self.frame, text = "v", command= lambda: self.camSelector("re"), width = self.BUTTON_WIDTH)
      self.le = tk.Button(self.frame, text = "<", command= lambda: self.camSelector("le"), width = self.BUTTON_WIDTH)

      self.max = tk.Button(self.frame, text = "MAX", command= lambda: self.manualChange("max"), width = self.BUTTON_WIDTH)
      self.min = tk.Button(self.frame, text = "MIN", command= lambda: self.manualChange("min"), width = self.BUTTON_WIDTH)

      self.changeip = tk.Button(self.frame, text = "Change IPs", command= lambda: self.spawnIpWindow(), width = self.BUTTON_WIDTH)


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

      self.changeip.grid(row=6, column=0)

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

   # Not in use right now, could be used instead of the buttons in the GUI
   # to simulate the vehicle movements
   def onKeyPress(self, event):
       x = event.char
       if x == "d":
	  print "d"
       elif x == "a":
          print "a"
       elif x == "w":
          print "w"
       elif x == "s":
          print "s"

   # Changes cam parameters by http requests when "driving"
   def button(self, direction):
      try:

         if not self.manualMode:
	    url_list = []
	    if direction == "forward":
               url_list.append("http://%s/set?qp=32" % self.ipRe)
               url_list.append("http://%s/set?qp=16" % self.ipFw)

            elif direction == "reverse":
               url_list.append("http://%s/set?qp=16" % self.ipRe)
               url_list.append("http://%s/set?qp=32" % self.ipFw)

	    elif direction == "left":
	       url_list.append("http://%s/set?qp=16" % self.ipLe)
	       url_list.append("http://%s/set?qp=32" % self.ipRi)

	    elif direction == "right":
	       url_list.append("http://%s/set?qp=32" % self.ipLe)
	       url_list.append("http://%s/set?qp=16" % self.ipRi)

	    else:
	       return

 	    self.createUrlThreads(url_list)

      except Exception as e:
         print (type(e))

   # Returns the actual IP of the currently selected cam(s)
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

   #Spawns the change camera IP window
   def spawnIpWindow(self):
      self.t = tk.Toplevel(self.root)
      self.t.title("Change Ips")
      self.frame2 = tk.Frame(self.t)
      self.frame2.pack(fill=tk.X, side=tk.BOTTOM)

      self.change = tk.Button(self.frame2, text = "Change", command= lambda: self.changeIp())
      
      
      self.labelFront = tk.Label(self.t, text="Front camera")
      self.labelRight = tk.Label(self.t, text="Right camera")
      self.labelReverse = tk.Label(self.t, text="Reverse camera")
      self.labelLeft = tk.Label(self.t, text="Left camera")

      self.frontEntry = tk.Entry(self.t)
      self.rightEntry = tk.Entry(self.t)
      self.reverseEntry = tk.Entry(self.t)
      self.leftEntry = tk.Entry(self.t)

      self.frontEntry.insert(10, self.ipFw)
      self.rightEntry.insert(10, self.ipRi)
      self.reverseEntry.insert(10, self.ipRe)
      self.leftEntry.insert(10, self.ipLe)
      
      self.labelFront.pack()
      self.frontEntry.pack(padx=5)
      self.labelRight.pack()
      self.rightEntry.pack(padx=5)
      self.labelReverse.pack()
      self.reverseEntry.pack(padx=5)
      self.labelLeft.pack()
      self.leftEntry.pack(padx=5)

      self.change.pack()

   #Assigns new camera IPs to the object 
   def changeIp(self):
      self.ipFw = self.frontEntry.get()
      self.ipRi = self.rightEntry.get()
      self.ipRe = self.reverseEntry.get()
      self.ipLe = self.leftEntry.get()
      self.allCamIps = [self.ipFw, self.ipRi, self.ipRe, self.ipLe]
      
      
   # Creates a new thread for each url
   def createUrlThreads(self, url_list):
      for url in url_list:
         thread = Thread(target = self.executeUrl, args = [url])
         thread.setDaemon(True)
         thread.start()

   def executeUrl(self, url):
      urllib2.urlopen(url, timeout = 2)

   #root.bind_all('<Key>', onKeyPress)

# Calls extern gstreamer application
def startCams():
   call(["./rtpclient", "0", "0", "0"])

thread_1 = Thread(target = startCams)
thread_1.start()
gui = Gui()
