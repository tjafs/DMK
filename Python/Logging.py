import threading
import queue
import time
import numpy as np
import serial
import matplotlib.pyplot as graf
import pickle
import metoder

class P:

    def __init__(self,x,skyv,tidsomloepnr,x_data,tid_data,runde,avstand):
        self.__x = x
        self.__skyv = skyv
        self.__tidsomloepnr = tidsomloepnr
        self.__x_data = x_data
        self.__tid_data = tid_data
        self.__runde = runde
        self.__avstand = avstand

    def getX(self):
        return self.__x

    def setX(self, x):
        self.__x = x

    def getSkyv(self):
        return self.__skyv

    def setSkyv(self,skyv):
        self.__skyv = skyv

    def gettidsomloepnr(self):
        return self.__tidsomloepnr

    def settidsomloepnr(self, tidsomloepnr):
        self.__tidsomloepnr = tidsomloepnr

    def getx_data(self):
        return self.__x_data

    def setx_data(self, x_data):

        self.__x_data = x_data

    def gettid_data(self):
        return self.__tid_data

    def settid_data(self, tid_data):
        self.__tid_data = tid_data

    def getrunde(self):
        return self.__runde

    def setrunde(self, runde):
        self.__runde = runde

    def get_avstand(self):
        return self.__avstand

    def set_avstand(self, avstand):
        self.__avstand = avstand

def hexascii2int(hex_teikn):
    if '0' <= hex_teikn <= '9':
        return (int(ord(hex_teikn) - 48))  # ASCII-koden for '0' er 0x30 = 48
    elif 'A' <= hex_teikn <= 'F':
        return (int(ord(hex_teikn) - 55))  # ASCII-koden for 'A' er 0x41 = 65

# Lese metode ------------------------------------------------------------------------------------

def start_lesing():
    print('1')
    def lesing(port, hvilken_komando):
        tid = []
        x_data = []
        p1 = P(0, 0, 0, 0, 0, 0, 0)
        l = 1
        nyavstand = 0

        metoder.lagring_av_kontinuerlig_data(1,0,0)

        kl = 99
        while (l <= 10):
            a = 0
            data = []
            while a < 1:

                brukarkommandoar.put(kommando)  # Gi melding til serietraaden om aa starta sjekking av port
                serieport.write('k'.encode('utf-8'))  # Gi melding til uC-en om aa koeyra i gong # KT la til encoding
                teikn = str(serieport.read(1), encoding='utf-8')  # Les eitt teikn.  #KT La til convert til str
                data.append(teikn)

                if teikn == 'Y':
                    a = 1
                #if teikn == 'S':
                #    kl = 1

                #if kl <= 3:
                #    print(teikn)
                #    kl += 1

                #print(teikn)
                #print('her ',p1.getrunde())

            # dekoding
            p1 = (sortering(data, p1))

            tid.append(p1.gettid_data())
            x_data.append(p1.getx_data())

            metoder.lagring_av_kontinuerlig_data(1,tid,x_data)

            nyavstand = (p1.get_avstand())
            metoder.lagring_av_kontinuerlig_data(3,nyavstand,p1.getrunde())

            (stop,stop2) = metoder.henting_av_kontinuerlig_data(2)


            if stop == 1:

                time.sleep(0.5)
                serieport.write('s'.encode('utf-8'))  # Gi melding til uC-en om aa stoppa sending av nye data #KT La til encoding
                return

    # dekoding av data metode ----------------------------------------------------------------------------------------

    def sortering(inn_data, p1):
        i = 0
        k = 0
        ut_data_x = []
        ut_data_tid = []
        ut_data_avstand = []
        x_data = []
        tid = []
        avstand = []

        # Sortering av tid og x data
        while i < len(inn_data):
            if inn_data[i] == 'X':
                ut_data_x.append(
                    4096 * hexascii2int(inn_data[i + 1]) + 256 * hexascii2int(inn_data[i + 2]) + 16 * hexascii2int(
                        inn_data[i + 3]) + hexascii2int(inn_data[i + 4]))
            if inn_data[i] == 'T':
                ut_data_tid.append(16 * hexascii2int(inn_data[i + 1]) + hexascii2int(inn_data[i + 2]))
            if inn_data[i] == 'S':
                ut_data_avstand.append(16 * hexascii2int(inn_data[i + 1]) + hexascii2int(inn_data[i + 2]))

            i += 1

            # Behandling av x data
        if len(ut_data_x) == 0:
            print('ingen data')
        else:
            k = 0
            for k in range(0, len(ut_data_x)):
                if ut_data_x[k] >= 32768:
                    x_data.append((float(ut_data_x[k]) - 65536.0) / 1000.0)  # 1mg pr. LSb iflg. databladet.
                else:
                    x_data.append(float(ut_data_x[k] / 1000.0))
                k += 1


                # Behandling av tid data
        if len(ut_data_tid) == 0:
            print('ingen data')
        else:
            k = 0
            Ts = 0.1
            for k in range(0, len(ut_data_tid)):
                tid.append(ut_data_tid[k] + p1.gettidsomloepnr() * 256)
                if ut_data_tid[k] == 255:
                    p1.settidsomloepnr(p1.gettidsomloepnr() + 1)
                k += 1

            if p1.getX() == 0:
                p1.setSkyv(tid[0])  # Vil at tidslista skal starta paa null.
                p1.setX(1)

            for k in range(0, len(tid)):
                tid[k] = Ts * (tid[k] - p1.getSkyv())

            # Behandling av avstand
        if len(ut_data_avstand) == 0:
            print('ingen data')
        else:
            k = 0
            for k in range(0, len(ut_data_avstand)):
                avstand.append(ut_data_avstand[k] + p1.gettidsomloepnr() * 256)
                if ut_data_avstand[k] == 255:
                    p1.settidsomloepnr(p1.gettidsomloepnr() + 1)
                k += 1
        print(avstand)



        # lagrer tid og x data i p1
        p1.set_avstand(avstand)
        p1.settid_data(tid)
        p1.setx_data(x_data)
        p1.setrunde(p1.getrunde() + 1)

        #print('x: ', x_data, 'tid: ', tid, 'runde: ', p1.getrunde())


        return p1

#----------------------------------------------------------------------------------
    # Installering av variabler
    kommando = '0'
    brukarkommandoar = queue.Queue()
    connected = True
    port = 'COM4'
    baud = 115200  # 9600

    serieport = serial.Serial(port, baud, timeout=1)

    if serieport.isOpen():
        print(serieport.name, 'er open')
    else:
        serieport.open()



    starting = threading.Thread(target=lesing, args=(serieport, kommando))
    starting.start()

    return







