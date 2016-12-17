from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import time
from tkinter import *
from tkinter import ttk
import tkinter
import variabler
import lesing_serial
import matplotlib.animation as animation
import threading
import metoder
from matplotlib.pylab import *

# Deklarere først alle de globale variablene jeg skal bruke i programet
metoder.instalering_av_variabler()


def start_knapp():

    sjekk_verdier() # Henter talene ut fra Entry og lagrer dem som globale variabler

    variabler.start = 1

    # Forste gang start blir betjent skal en traa starte for lesing på serieporten
    if(variabler.runde == 0):
        yo = threading.Thread(target=lesing_serial.start_serial)
        yo.start()
        variabler.runde = 1

    # restarter variabler
    time.sleep(1)               # venter 1 sekund for å være sikker på at det er blitt lagret verdier til plot
    variabler.start_plot = 1    # Starter kontinuerlig plotting
    variabler.stop = 0
    variabler.stop1 = 0


def sjekk_verdier():    # En metode for aa hente ut verdier fra Enty og lagre dem som globale variaber

    variabler.Kp_global = Kp_entry.get()
    variabler.Ti_global = Ti_entry.get()
    variabler.Td_global = Td_entry.get()

    variabler.avstand1_global = avstand1_entry.get()
    variabler.avstand2_global = avstand2_entry.get()

    return
#-----------------------------------------------------------------------------------------------------------------------
def stop_knapp():

    if variabler.stop1 == 0:

        variabler.antall_filer += 1
        #starter en ny traa for å lagre dataen i en pickle fil
        lagring_fil = threading.Thread(target=metoder.lagring_av_tideligere_data)
        lagring_fil.start()

        add_Button(variabler.knapp_nummer)
        variabler.knapp_nummer += 1

        time.sleep(1)               # Venter et sekund mes data blir lagret

        variabler.stop = 1
        variabler.stop1 = 1         # setter stop1 til 1 slik at du bare kjører igjennom if lokken en gang
        variabler.start = 0


    return
#---------------------------------------------------------------------
# Hvis programet er startet og du vil opptatere reguleringsparametrene
def opptater_PID():
    if(variabler.start == 1):
        sjekk_verdier()                      # leser av tallene i Entry
        variabler.opptater_PID_verdier = 1   # forteller lesing.serial metoden at nye reguleringsparametre skal sendes ned

    return
# Metoden som blir benyttet når du trykker på tidligere logg knappen
def tidligere_logg(tall):
    metoder.henting_av_tidligere_data(tall)

#---------------------------------------------------------------------------
# Metode for aa lage ny knap i GUI-et
def add_Button(nummer):
    l1 = ['Logg: ', str(nummer)]
    s = ''.join(l1)                 # Bruker ''.join() for og lage en string uten komma
    ttk.Button(etasje_2_rom_2, text=s,command=lambda: tidligere_logg(nummer)).grid(column=0, row=nummer)

#-----------------------------------------------------------------------------
# Figur for plotting av avstands-sensoren
figur_avstand = Figure(figsize=(20, 4), dpi=70,tight_layout=True)
figur_avstand.patch.set_visible(False)
figur_avstand.add_axes()
avstand_graf = figur_avstand.add_subplot(111)

# Figur for plotting av P-leddet
figur_P = Figure(figsize=(9, 2), dpi=70,tight_layout=True)
figur_P.patch.set_visible(False)
P_graf = figur_P.add_subplot(111)

# Figur for plotting av I-leddet
figur_I = Figure(figsize=(9, 2), dpi=70,tight_layout=True)
figur_I.patch.set_visible(False)
I_graf = figur_I.add_subplot(111)

# Figur for plotting av D-leddet
figur_D = Figure(figsize=(9, 2), dpi=70,tight_layout=True)
figur_D.patch.set_visible(False)
D_graf = figur_D.add_subplot(111)

# Figur for plotting av akselerasjon
figur_aks = Figure(figsize=(7, 5), dpi=70)
figur_aks.patch.set_visible(False)
a3 = figur_aks.add_subplot(111)

#----------------------------------------------------------------------------

#------------------------------------------------------------------------------
root = Tk()                           # Lager et vindu
root.title("Gruppe 2 Analyse av data")# setter navn på vinduet
#-----------------------------------------------------------------------

etasje_3 = ttk.Frame(root, padding="2 2 2 2")
etasje_3.grid(column=0, row=0)

# Henter ut bilde som vi bruker i overskriften av programet
bilde = tkinter.PhotoImage(file='Gruppe2_overskrift.gif')
ttk.Label(etasje_3, image=bilde).grid(column=0, row=0)


#-----------------------------------------------------------------------
etasje_2 = ttk.Frame(root, padding="2 2 2 2")
etasje_2.grid(column=0, row=1)

#-----------------------------------------------------------------------
# maa deklarere variablene i Entry boksen for aa hente ut tallene
Kp_entry = tkinter.StringVar()
Ti_entry = tkinter.StringVar()
Td_entry = tkinter.StringVar()
avstand1_entry = tkinter.StringVar()
avstand2_entry = tkinter.StringVar()

# setter disse verdiene i starten slik at vi ikke må skrive inn samme verdi hver gang vi tester programet
Kp_entry.set('20')
Ti_entry.set('2')
Td_entry.set('0')
avstand1_entry.set('35')
avstand2_entry.set('0.1')

etasje_2_rom_1 = ttk.Frame(etasje_2, padding="2 2 2 2")
etasje_2_rom_1.grid(column=0, row=0)

ttk.Label(etasje_2_rom_1,text='Regulering:',font=("Helvetica", 16)).grid(column=0, row=0)

etasje_2_rom_1_i_rom_1 = ttk.Frame(etasje_2_rom_1, padding="2 2 2 2")
etasje_2_rom_1_i_rom_1.grid(column=0, row=1)

# Kp parameter
ttk.Label(etasje_2_rom_1_i_rom_1,text='Kp',font=("Helvetica", 10)).grid(column=0, row=0)
ttk.Entry(etasje_2_rom_1_i_rom_1,width=10,textvariable=Kp_entry).grid(column=1, row=0)
# Ti parameter
ttk.Label(etasje_2_rom_1_i_rom_1,text='Ti',font=("Helvetica", 10)).grid(column=0, row=1)
ttk.Entry(etasje_2_rom_1_i_rom_1,width=10,textvariable=Ti_entry).grid(column=1, row=1)
# Td parameter
ttk.Label(etasje_2_rom_1_i_rom_1,text='Td',font=("Helvetica", 10)).grid(column=0, row=2)
ttk.Entry(etasje_2_rom_1_i_rom_1,width=10,textvariable=Td_entry).grid(column=1, row=2)

# Avstand
ttk.Label(etasje_2_rom_1,text='Avstand:',font=("Helvetica", 15)).grid(column=0, row=2)


etasje_2_rom_2_i_rom_1 = ttk.Frame(etasje_2_rom_1, padding="2 2 2 2")
etasje_2_rom_2_i_rom_1.grid(column=0, row=3)

ttk.Entry(etasje_2_rom_2_i_rom_1,width=5,textvariable=avstand1_entry).grid(column=0, row=0)
ttk.Label(etasje_2_rom_2_i_rom_1,text='cm',font=("Helvetica", 13)).grid(column=1, row=0)
#------------------------------
ttk.Label(etasje_2_rom_1,text='Margin:',font=("Helvetica", 15)).grid(column=0, row=4)
#------------------------------
etasje_2_rom_2_i_rom_2 = ttk.Frame(etasje_2_rom_1, padding="2 2 2 2")
etasje_2_rom_2_i_rom_2.grid(column=0, row=5)

ttk.Label(etasje_2_rom_2_i_rom_2,text='±',font=("Helvetica", 13)).grid(column=0, row=0)     # Avstand som linmotenn skal holde seg mellom
ttk.Entry(etasje_2_rom_2_i_rom_2,width=5,textvariable=avstand2_entry).grid(column=1, row=0)
ttk.Label(etasje_2_rom_2_i_rom_2,text='cm',font=("Helvetica", 13)).grid(column=2, row=0)

ttk.Button(etasje_2_rom_1, text='Oppdater',command=opptater_PID).grid(column=0, row=6)      # Start stop og avslutt knapp
ttk.Button(etasje_2_rom_1, text='Start',command=start_knapp).grid(column=0, row=7)
ttk.Button(etasje_2_rom_1, text='Stopp',command=stop_knapp).grid(column=0, row=8)

# maa sette avstand til tkinter.StringVar() for aa kunne sette ny verdi i Label
avstand = tkinter.StringVar()

ttk.Label(etasje_2_rom_1,text='Avstanden: ',font=("Helvetica", 16)).grid(column=0, row=9)
ttk.Label(etasje_2_rom_1,textvariable=avstand,font=("Helvetica", 25)).grid(column=0, row=10)

##
etasje_2_rom_2 = ttk.Frame(etasje_2, padding="2 2 2 2")
etasje_2_rom_2.grid(column=1, row=0)

ttk.Label(etasje_2_rom_2,text='Tidligere data:',font=("Helvetica", 14)).grid(column=0, row=0)

#-----------------------------------------------------------------------
etasje_2_rom_3 = ttk.Frame(etasje_2, padding="2 2 2 2")
etasje_2_rom_3.grid(column=2, row=0)

ttk.Label(etasje_2_rom_3,text='Akselerasjon:',font=("Helvetica", 15)).grid(column=0, row=0)
# akselerasjon Plot/figur
Figure = FigureCanvasTkAgg(figur_aks,etasje_2_rom_3)
Figure.show()
Figure.get_tk_widget().grid(column=0, row=1)

etasje_2_rom_4 = ttk.Frame(etasje_2, padding="2 2 2 2")
etasje_2_rom_4.grid(column=3, row=0)

# P ledd Plot/figur
ttk.Label(etasje_2_rom_4,text='P',font=("Helvetica", 10)).grid(column=0, row=0)
Figure = FigureCanvasTkAgg(figur_P,etasje_2_rom_4)
Figure.show()
Figure.get_tk_widget().grid(column=0, row=1)

# I ledd Plot/figur
ttk.Label(etasje_2_rom_4,text='I',font=("Helvetica", 10)).grid(column=0, row=2)
Figure = FigureCanvasTkAgg(figur_I,etasje_2_rom_4)
Figure.show()
Figure.get_tk_widget().grid(column=0, row=3)

# D ledd Plot/figur
ttk.Label(etasje_2_rom_4,text='D',font=("Helvetica", 10)).grid(column=0, row=4)
Figure = FigureCanvasTkAgg(figur_D,etasje_2_rom_4)
Figure.show()
Figure.get_tk_widget().grid(column=0, row=5)
#-----------------------------------------------------------------------
etasje_1 = ttk.Frame(root)
etasje_1.grid(column=0, row=2)

ttk.Label(etasje_1,text='Avstandssensor:',font=("Helvetica", 15)).grid(column=0, row=0)
# Avstand Plot/figur
Figure = FigureCanvasTkAgg(figur_avstand,etasje_1)
Figure.show()
Figure.get_tk_widget().grid(column=0, row=1)

#-----------------------------------------------------------------------
for child in etasje_3.winfo_children(): child.grid_configure(padx=1, pady=1)

for child in etasje_2.winfo_children(): child.grid_configure(padx=5, pady=5)
for child in etasje_2_rom_1.winfo_children(): child.grid_configure(padx=1, pady=1)
for child in etasje_2_rom_2.winfo_children(): child.grid_configure(padx=1, pady=1)
for child in etasje_2_rom_3.winfo_children(): child.grid_configure(padx=1, pady=1)
for child in etasje_2_rom_4.winfo_children(): child.grid_configure(padx=1, pady=1)
for child in etasje_2_rom_1_i_rom_1.winfo_children(): child.grid_configure(padx=1, pady=3)

for child in etasje_1.winfo_children(): child.grid_configure(padx=5, pady=5)


# opptatering av kontunerlig akselerasjon -------------------------------
def animate_aks(i):
    if variabler.start_plot == 1:
        a3.clear()
        a3.plot(variabler.tid_graf,variabler.x_graf)

        avstand.set(round(variabler.avstand_vis2))


    if variabler.start_plot == 0 and variabler.aks_ok == 1:
        a3.clear()
        a3.plot(variabler.data_tid,variabler.data_aks)
        variabler.aks_ok = 0

# ------------------------------------------------------------------

# opptatering av avstand data -------------------------------
def animate_avstand(i):
    if variabler.start_plot == 1:
        avstand_graf.clear()
        avstand_graf.plot(variabler.tid_graf, variabler.avstand_vis,'r')
        avstand_graf.plot(variabler.tid_graf,variabler.hold_avstand_graf1,'b')
        avstand_graf.plot(variabler.tid_graf, variabler.hold_avstand_graf2,'b')


    if variabler.start_plot == 0 and variabler.avstand_ok == 1:
        avstand_graf.clear()
        avstand_graf.plot(variabler.data_tid, variabler.data_avstand,'r')
        avstand_graf.plot(variabler.data_tid,variabler.data_avstand_graf1,'b')
        avstand_graf.plot(variabler.data_tid, variabler.data_avstand_graf2,'b')
        variabler.avstand_ok = 0



# opptatering av P ledd data -------------------------------
def animate_P(i):
    if variabler.start_plot == 1:
        P_graf.clear()
        P_graf.plot(variabler.tid_graf, variabler.P_graf_tegn)

    if variabler.start_plot == 0 and variabler.P_ok == 1:
        P_graf.clear()
        P_graf.plot(variabler.data_tid, variabler.data_P)
        variabler.P_ok = 0



# opptatering av I ledd data -------------------------------
def animate_I(i):
    if variabler.start_plot == 1:
        I_graf.clear()
        I_graf.plot(variabler.tid_graf, variabler.I_graf_tegn)

    if variabler.start_plot == 0 and variabler.I_ok == 1:
        I_graf.clear()
        I_graf.plot(variabler.data_tid, variabler.data_I)
        variabler.I_ok = 0

# opptatering av D ledd data -------------------------------
def animate_D(i):
    if variabler.start_plot == 1:
        D_graf.clear()
        D_graf.plot(variabler.tid_graf, variabler.D_graf_tegn)

    if variabler.start_plot == 0 and variabler.D_ok == 1:
        D_graf.clear()
        D_graf.plot(variabler.data_tid, variabler.data_D)
        variabler.D_ok = 0


ani1 = animation.FuncAnimation(figur_aks, animate_aks, interval=500,repeat=False)
ani2 = animation.FuncAnimation(figur_avstand, animate_avstand, interval=500,repeat=False)
ani3 = animation.FuncAnimation(figur_P, animate_P, interval=500,repeat=False)
ani4 = animation.FuncAnimation(figur_I, animate_I, interval=500,repeat=False)
ani5 = animation.FuncAnimation(figur_D, animate_D, interval=500,repeat=False)





root.mainloop()




