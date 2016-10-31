import pickle



class Company(object):
    def __init__(self,x_data,y_data):
        self.x_data = x_data
        self.y_data = y_data

class data(object):
    def __init__(self,x_data,y_data):
        self.x_data = x_data
        self.y_data = y_data

global num
num = 0

def lagring_data():
    global num
    num += 1
    x = []
    y = []
    (x,y) = input(1)

    l1 = ['Logg_', str(num), '.pkl']
    s = ''.join(l1)

    with open(str(s), 'wb') as output:
        company2 = Company(x, y)
        pickle.dump(company2, output, pickle.HIGHEST_PROTOCOL)
    del company2
    return

def henting_av_tidligeredata(num):

    l1 = ['logg_', str(num), '.pkl']
    s = ''.join(l1)

    try:
        with open(str(s), 'rb') as input:
            data = pickle.load(input)
            x = data.x_data
            y = data.y_data
    except EOFError:
        return 'jalla'
    return (x,y)


def input(nummer):

    l1 = ['company_data', str(nummer), '.pkl']
    s = ''.join(l1)

    try:
        with open(str(s), 'rb') as input:
            company1 = pickle.load(input)
            x = company1.x_data
            y = company1.y_data
    except EOFError:
        return 'jalla'
    return (x,y)

def output(nummer,data1,data2):

    l1 = ['company_data', str(nummer), '.pkl']
    s = ''.join(l1)

    with open(str(s), 'wb') as output:
        company2 = Company(data1, data2)
        pickle.dump(company2, output, pickle.HIGHEST_PROTOCOL)
    del company2

    return

