from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import pickle
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import threading
import Logging
import metoder
from tkinter import *
from tkinter import ttk
import tkinter
import os


class knapp(object):
    def __init__(self,x_data,y_data):
        self.x_data = x_data
        self.y_data = y_data

kor = 0
knapp_nummer = 1
graf2 = 0
stop1 = 0
start = 0
antall_filer = 0


# opptatering av graf 1-------------------------------
def animate(i):
    if kor == 1:
        (x,y) = metoder.henting_av_kontinuerlig_data(1)
        a.clear()
        a.plot(x, y)


# ------------------------------------------------------------------

# opptatering av graf 2-------------------------------
def animate2(i):
    global start
    global graf2
    if start == 1 and graf2 != 0:
        (x, y) = metoder.henting_av_tidligere_data(graf2)
        a2.clear()
        a2.plot(x, y)

# ------------------------------------------------------------------

def opptatering():
    while(1):
        time.sleep(1)
        (x, y) = metoder.henting_av_kontinuerlig_data(3)
        global avstand
        avstand.set(x)


# ------------------------------------------------------------------

def start_knapp():
    global kor
    global stop1
    global start
    start = 1
    if kor == 0:
        metoder.lagring_av_kontinuerlig_data(2,0,0)

        yo = threading.Thread(target=Logging.start_lesing)
        yo.start()

        time.sleep(0.5)
        # opptatering av lable (avstand)
        text_update = threading.Thread(target=opptatering)
        text_update.start()

        kor = 1
        stop1 = 0
    print('funk')

#-------------------------------------------------------------------------
def stop_knapp():
    global stop1
    if stop1 == 0:
        global kor
        kor = 0
        global antall_filer
        antall_filer += 1
        global knapp_nummer
        metoder.lagring_av_kontinuerlig_data(2, 1, 1) # for å stoppe loggingen

        lagring_fil = threading.Thread(target=metoder.lagring_av_tideligere_data)
        lagring_fil.start()

        add_Button(knapp_nummer)
        knapp_nummer += 1

        stop1 = 1

#---------------------------------------------------------------------
def tidligere_logg(tall):
    global graf2
    graf2 = tall
#---------------------------------------------------------------------------

def add_Button(nummer):
   ###command= lambda: action(someNumber)
    l1 = ['Logg: ', str(nummer)]
    s = ''.join(l1)
    ttk.Button(frame3, text=s,command=lambda: tidligere_logg(nummer)).grid(column=1, row=nummer)

#----------------------------------------------------------------------------
def avslutt():
    metoder.lagring_av_kontinuerlig_data(2, 1, 1)  # for å stoppe loggingen

# Fjerne filene som er blitt laget
    global antall_filer
    for i in range(1,antall_filer+1):
        l1 = ['Logg_', str(i), '.pkl']
        fil = ''.join(l1)
        os.path.isfile(fil)
        os.remove(fil)

    i = 1
    while i <= 3:
        l1 = ['Kontinuerlig_', str(i), '.pkl']
        fil = ''.join(l1)
        os.path.isfile(fil)
        os.remove(fil)
        i += 1

    quit()
#-----------------------------------------------------------------------------

f = Figure(figsize=(10, 3), dpi=70)
a = f.add_subplot(111)

f2 = Figure(figsize=(10, 3), dpi=70)
a2 = f2.add_subplot(111)

f3 = Figure(figsize=(10, 3), dpi=70)
a3 = f2.add_subplot(111)

#------------------------------------------------------------------------------
root = Tk()
root.title("Gruppe 2 Analyse av data")
#-----------------------------------------------------------------------

frame = ttk.Frame(root, padding="5 5 5 5")
frame.grid(column=0, row=0)
frame.columnconfigure(0, weight=1)
frame.rowconfigure(0, weight=1)

ttk.Button(frame, text='Start',command=start_knapp).grid(column=0, row=2)
ttk.Button(frame, text='Stop',command=stop_knapp).grid(column=0, row=2)
ttk.Button(frame, text='Avslutt',command=avslutt).grid(column=0, row=2)

#-----------------------------------------------------------------------

ttk.Label(frame,text='Kontunuerlig plott:',font=("Helvetica", 16)).grid(column=2, row=2)

Figure = FigureCanvasTkAgg(f,frame)
Figure.show()
Figure.get_tk_widget().grid(column=2, row=3)

bilde = tkinter.PhotoImage(file='b.gif')
ttk.Label(frame, image=bilde).grid(column=2, row=1)

#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
for child in frame.winfo_children(): child.grid_configure(padx=5, pady=10)


ani = animation.FuncAnimation(f, animate, interval=1000)




root.mainloop()




