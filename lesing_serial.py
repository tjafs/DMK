import threading
import queue
import time
import numpy as np
import serial
import matplotlib.pyplot as graf
import pickle
import metoder
import math
import variabler


def hexascii2int(hex_teikn):
    if '0' <= hex_teikn <= '9':
        return (int(ord(hex_teikn) - 48))  # ASCII-koden for '0' er 0x30 = 48
    elif 'A' <= hex_teikn <= 'F':
        return (int(ord(hex_teikn) - 55))  # ASCII-koden for 'A' er 0x41 = 65


def start_serial():

    connected = True
    kommando = '0'
    brukarkommandoar = queue.Queue()
    port = 'COM5'
    baud = 115200  # 115200  # 9600
    a = 0

    serieport = serial.Serial(port, baud, timeout=1)

    if serieport.isOpen():
        print(serieport.name, 'er open')
    else:
        serieport.open()



    while(1):
        Les_data = 1
        tid = []
        tid_inn = 0
        klokke_runde = 0
        tid_gammel = 0

        x_data = []
        x_data_mid = 0

        variabler.tid_graf = []
        variabler.x_graf = []

        variabler.P_graf_tegn = []
        variabler.I_graf_tegn = []
        variabler.D_graf_tegn = []

        variabler.avstand_vis = []
        variabler.avstand_vis2 = []
        variabler.hold_avstand_graf1 = []
        variabler.hold_avstand_graf2 = []



        # ----------------------------------------------------------------------------------------------------------
        # start sende ned PID verdier til mikrokontroller
        if(variabler.start == 1):
            ## fortelle mikrokontroller om å gjøre seg klar til å mota data
            serieport.write('k'.encode('utf-8'))
            time.sleep(0.01)

            # Metode for å sende ned data til mikrokontroller
            # Det er bare mulig og sende talle mellom 0-500
            def send_data(data):
                if (data >= 250):
                    data_num_1 = 250
                    data_num_2 = data - 250
                else:
                    data_num_1 = data
                    data_num_2 = 0

                data2 = chr(data_num_1)
                data3 = ''.join(data2)
                serieport.write(data3.encode('utf-8'))
                time.sleep(0.01)

                data2 = chr(data_num_2)
                data3 = ''.join(data2)
                serieport.write(data3.encode('utf-8'))
                time.sleep(0.01)


            # sende data om Kp, Ti, Td og avstanden den skal holde
            send_data(int(float(variabler.Kp_global)))  ## Kp kan være mellom 0 til 500
            send_data(int(float(variabler.Ti_global) * 10))  ## Ti Må gange med 10 for å kunne sende, kan ikke sende kommatall
            send_data(int(float(variabler.Td_global) * 10))  ## Må gange med 10 for å kunne sende, kan ikke sende kommatall
            send_data(int(variabler.avstand1_global))        ## avvik fra kan være mellom 0 til 500
            send_data(int(float(variabler.avstand2_global)*10))        ## Må gange med 10 for å kunne sende, kan ikke sende kommatall
                                                                        # deler med 10 igjen når verdien er ankomet mikrokontrolleren
            time.sleep(0.01)
            serieport.write('M'.encode('utf-8'))    # Forteller mikrokontrolleren at alle tall er sendt ned
            teikn = 0



            while (teikn != '\x03'):
                teikn = str(serieport.read(1), encoding='utf-8')  # Les eitt teikn.  #KT La til convert til str

            variabler.opptater_PID_verdier = 0
            Les_data = 0

            # slutt sende ned PID verdier til mikrokontroller
            # ----------------------------------------------------------------------------------------------------------

        # Begynner å lese data fra mikrokontrolleren
        while(Les_data == 0):
            a = 0
            data = []
            while a == 0:
                brukarkommandoar.put(kommando)  # Gi melding til serietraaden om aa starta sjekking av port
                teikn = str(serieport.read(1), encoding='utf-8')  # Les eitt teikn.  #KT La til convert til str
                data.append(teikn)
                #print(teikn)
                if teikn == 'Z':
                    a = 1


            # hvis pcen har motat 31 data så skal pcen dekode signalet
            if (len(data) == 32):
                i = 0
                k = 0
                # Sortering av tid og x data----------------------------------------------------------------------------------------

                while i < len(data):
                    if data[i] == 'Y':
                        x_data_mid = (
                            4096 * hexascii2int(data[i + 1]) + 256 * hexascii2int(
                                data[i + 2]) + 16 * hexascii2int(
                                data[i + 3]) + hexascii2int(data[i + 4]))

                    if data[i] == 'T':
                        tid_inn = (float(16 * hexascii2int(data[i + 1]) + hexascii2int(data[i + 2])))*0.001
                        ##tid.append(variabler.antall_pakker)

                    if data[i] == 'S':
                        variabler.avstand_vis.append(
                            (65536 * hexascii2int(data[i + 1]) + 4096 * hexascii2int(data[i + 2]) + 256 * hexascii2int(
                                data[i + 3]) + 16 * hexascii2int(
                                data[i + 4]) + hexascii2int(data[i + 5]))*0.01)

                    if data[i] == 'P':
                        variabler.P_graf_tegn.append(
                            4096 * hexascii2int(data[i + 1]) + 256 * hexascii2int(
                                data[i + 2]) + 16 * hexascii2int(
                                data[i + 3]) + hexascii2int(data[i + 4]))

                    if data[i] == 'I':
                        variabler.I_graf_tegn.append(
                            4096 * hexascii2int(data[i + 1]) + 256 * hexascii2int(
                                data[i + 2]) + 16 * hexascii2int(
                                data[i + 3]) + hexascii2int(data[i + 4]))

                    if data[i] == 'Q':
                        variabler.D_graf_tegn.append(
                            (4096 * hexascii2int(data[i + 1]) + 256 * hexascii2int(
                                data[i + 2]) + 16 * hexascii2int(
                                data[i + 3]) + hexascii2int(data[i + 4])))

                    i += 1

                k = 0

                if x_data_mid >= 32768:
                    variabler.x_graf.append((float(x_data_mid) - 65536.0) / 1000.0)  # 1mg pr. LSb iflg. databladet.
                else:
                    variabler.x_graf.append(float(x_data_mid / 1000.0))
                k += 1

                #-------------------------------------------------------------------------------------------------------
                if(variabler.antall_pakker >= 1):
                    if(tid_inn < tid_gammel):
                        klokke_runde += 1

                    variabler.tid_graf.append(tid_inn + 0.256*klokke_runde)
                else:
                    variabler.tid_graf.append(tid_inn)

                tid_gammel = tid_inn

                #print('tid_inn: ',tid_inn)
                #print('tid: ',tid_inn + 0.255*klokke_runde)

                # Lagrer all data i filen variabler --------------------------------------------------------------------


                variabler.avstand_vis2 = variabler.avstand_vis[variabler.antall_pakker]
                # Lagger 2 arrayer for å plotte avstandsmarginene
                variabler.hold_avstand_graf1.append(float(variabler.avstand1_global) + float(variabler.avstand2_global))
                variabler.hold_avstand_graf2.append(float(variabler.avstand1_global) - float(variabler.avstand2_global))

                variabler.antall_pakker += 1

            # Slutt på lagrer av data-----------------------------------------------------------------------------------

            ## hvis opptaterings knappen er betjent, skal nye PID verdier sendes ned til mikrokontrolleren
            if (variabler.opptater_PID_verdier == 1):
                ## fortelle mikrokontroller om å gjøre seg klar til å mota data
                serieport.write('k'.encode('utf-8'))
                time.sleep(0.01)

                # Metode for å sende ned data til mikrokontroller
                def send_data(data):
                    if (data >= 250):
                        data_num_1 = 250
                        data_num_2 = data - 250
                    else:
                        data_num_1 = data
                        data_num_2 = 0

                    data2 = chr(data_num_1)
                    data3 = ''.join(data2)
                    # print(data3)
                    serieport.write(data3.encode('utf-8'))
                    time.sleep(0.01)

                    data2 = chr(data_num_2)
                    data3 = ''.join(data2)
                    # print(data3)
                    serieport.write(data3.encode('utf-8'))
                    time.sleep(0.01)

                # sende data om Kp, Ti, Td og avstanden den skal holde
                send_data(int(float(variabler.Kp_global)))  ## Kp
                send_data(int(float(variabler.Ti_global) * 10))  ## Ti
                send_data(int(float(variabler.Td_global) * 10))  ## Td
                send_data(int(variabler.avstand1_global))  ## avvik fra
                send_data(int(float(variabler.avstand2_global) * 10))  ## avvik til

                time.sleep(0.01)
                serieport.write('M'.encode('utf-8'))
                teikn = 0

                while (teikn != '\x03'):
                    teikn = str(serieport.read(1), encoding='utf-8')  # Les eitt teikn.  #KT La til convert til str

                variabler.opptater_PID_verdier = 0

                # slutt sende ned PID verdier til mikrokontroller
                # ----------------------------------------------------------------------------------------------------------


            # Hvis stop knapp er betjent
            if variabler.start == 0:
                time.sleep(0.5)
                serieport.write('s'.encode('utf-8'))  # Forteller mikrokontrolleren om å stoppe regulering
                Les_data = 1
                variabler.start_plot = 0 # stopper ploting av kontinuerlig data







