from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import pickle
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import threading
import Logging
import Metoder
from tkinter import *
from tkinter import ttk
import tkinter

class knapp(object):
    def __init__(self,x_data,y_data):
        self.x_data = x_data
        self.y_data = y_data

kor = 0
knapp_nummer = 1

# opptatering av graf 1-------------------------------
def animate(i):
    if kor == 1:
        (x,y) = Metoder.input(1)
        a.clear()
        a.plot(x, y)

# ------------------------------------------------------------------
# opptatering av graf 1-------------------------------
def animate2(i):

    (x,y) = Metoder.input(5)
    a2.clear()
    a2.plot(x, y)
# ------------------------------------------------------------------



def start_knapp():
    global kor
    if kor == 0:
        Metoder.output(2,0,0)

        #yo = threading.Thread(target=Logging.start_lesing)
        #yo.start()
        time.sleep(0.5)
        kor = 1
    print('funk')

#-------------------------------------------------------------------------
def stop_knapp():
    global kor
    kor = 0
    global knapp_nummer
    global yo
    Metoder.output(2,1,1)

    lagring_fil = threading.Thread(target=Metoder.lagring_data)
    lagring_fil.start()

    add_Button(knapp_nummer)
    knapp_nummer += 1
#---------------------------------------------------------------------

f = Figure(figsize=(10, 3), dpi=70)
a = f.add_subplot(111)

f2 = Figure(figsize=(10, 3), dpi=70)
a2 = f2.add_subplot(111)

root = Tk()
root.title("Feet to Meters")
#-----------------------------------------------------------------------

mainframe = ttk.Frame(root, padding="3 3 12 12")
mainframe.grid(column=0, row=0)
mainframe.columnconfigure(0, weight=1)
mainframe.rowconfigure(0, weight=1)

ttk.Button(mainframe, text='Start',command=start_knapp).grid(column=1, row=1)
ttk.Button(mainframe, text='Stop',command=stop_knapp).grid(column=1, row=2)
ttk.Button(mainframe, text='Avslutt').grid(column=1, row=3)

#-----------------------------------------------------------------------

mainframe2 = ttk.Frame(root, padding="3 3 12 12")
mainframe2.grid(column=1, row=0)
mainframe2.columnconfigure(0, weight=1)
mainframe2.rowconfigure(0, weight=1)

Figure = FigureCanvasTkAgg(f,mainframe2)
Figure.show()
Figure.get_tk_widget().grid(column=2, row=2)

bilde = tkinter.PhotoImage(file='b.gif')
ttk.Label(mainframe2, image=bilde).grid(column=2, row=1)

#-----------------------------------------------------------------------

mainframe3 = ttk.Frame(root, padding="3 3 12 12")
mainframe3.grid(column=0, row=1)
mainframe3.columnconfigure(0, weight=1)
mainframe3.rowconfigure(0, weight=1)

def tidligere_logg(tall):
    (x,y) = Metoder.input(tall)
    Metoder.output(5,x,y)
    print('yo')

ttk.Button(mainframe3, text='Start').grid(column=1, row=1)
def add_Button(nummer):
   ###command= lambda: action(someNumber)
    ttk.Button(mainframe3, text=('Logg ',nummer),command=lambda: tidligere_logg(nummer)).grid(column=1, row=nummer+1)


#-----------------------------------------------------------------------
mainframe4 = ttk.Frame(root, padding="3 3 12 12")
mainframe4.grid(column=1, row=1)
mainframe4.columnconfigure(0, weight=1)
mainframe4.rowconfigure(0, weight=1)

ttk.Label(mainframe4,text='Tidligere plott').grid(column=2, row=1)

Figure = FigureCanvasTkAgg(f2,mainframe4)
Figure.show()
Figure.get_tk_widget().grid(column=2, row=2)

#-----------------------------------------------------------------------

for child in mainframe.winfo_children(): child.grid_configure(padx=5, pady=5)
for child in mainframe2.winfo_children(): child.grid_configure(padx=2, pady=2)

ani = animation.FuncAnimation(f, animate, interval=1000)
ani2 = animation.FuncAnimation(f2, animate2, interval=1000)
root.mainloop()