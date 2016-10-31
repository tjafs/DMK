import pickle

class data(object):
    def __init__(self,x_data,y_data):
        self.x_data = x_data
        self.y_data = y_data

global num
num = 0

def lagring_av_tideligere_data():
    global num
    num += 1
    (x,y) = henting_av_kontinuerlig_data(1)

    l1 = ['Logg_', str(num), '.pkl']
    s = ''.join(l1)

    with open(str(s), 'wb') as output:
        data1 = data(x, y)
        pickle.dump(data1, output, pickle.HIGHEST_PROTOCOL)
    del data1
    return

def henting_av_tidligere_data(num):

    l1 = ['logg_', str(num), '.pkl']
    s = ''.join(l1)

    try:
        with open(str(s), 'rb') as input:
            data1 = pickle.load(input)
            x = data1.x_data
            y = data1.y_data
    except EOFError:
        return 'jalla'
    return (x,y)

#---------------------------------------------------------------------------------

def henting_av_kontinuerlig_data(num):

    l1 = ['Kontinuerlig_', str(num), '.pkl']
    s = ''.join(l1)

    try:
        with open(str(s), 'rb') as input:
            data = pickle.load(input)
            x = data.x_data
            y = data.y_data
    except EOFError:
        return 'jalla'
    return (x,y)

def lagring_av_kontinuerlig_data(num,data1,data2):

    l1 = ['Kontinuerlig_', str(num), '.pkl']
    s = ''.join(l1)

    with open(str(s), 'wb') as output:
        data2 = data(data1, data2)
        pickle.dump(data2, output, pickle.HIGHEST_PROTOCOL)
    del data2

    return

