import pickle
import os.path
import numpy as np
from numpy import transpose, multiply
import scipy.sparse as scy_sparce
import scipy.spatial as spy_spatial
import sklearn.decomposition as sk_dec

from copy import deepcopy

class LSMRN:

    PickleFile = 'HighwayGraphPickle'
    LinkCoordinates = "all_data/link-coordinates"

    def __init__(self,n_vertex = 0,g_matrix = None,ls_matrix = None, att_matrix = None, t_matrix = None,\
        l_dimension = 0,n_snapshots = 0,gap = 0,horizon = 0):
        self.N = n_vertex
        self.G = g_matrix
        self.U = ls_matrix
        self.A = t_matrix
        self.B = att_matrix
        self.k = l_dimension
        self.T = n_snapshots
        self.span = gap
        self.h = horizon
        self.delta = 0
        self.gamma = 0

        self.links_name = []

    def save(self):
        file = open(self.PickleFile,'w+')
        pickle.dump(self,file,pickle.HIGHEST_PROTOCOL)
        file.close()
        print self,'saved'

    def load(self):
        self.extract()

        file = open(self.PickleFile,'r')
        G = pickle.load(file)
        file.close()
        return G

    def extract(self):
        STARTX, STARTY, ENDX, ENDY = 4,5,6,7
        if not os.path.exists(self.PickleFile):
            links = []
            for i in range(10):
                links.append(np.zeros(10))
            links = np.array(links)

            with open(self.LinkCoordinates) as linkfile:
                parsed = [line.split(',') for line in linkfile]
                for row in parsed:
                    row[ENDY] = row[ENDY][:-1]

                index, cur_con = 0, 0
                while(cur_con < 10):
                    print parsed[index][0]
                    self.links_name.append(parsed[index][0])
                    if cur_con != 9:
                        for j in range(len(parsed)):
                            if parsed[index][STARTX] == parsed[j][ENDX] \
                            and parsed[index][STARTY] == parsed[j][ENDY]:
                                index = j
                                links[cur_con][cur_con+1] = float(1)
                                cur_con += 1
                                break
                    else:
                        cur_con += 1

            self.N = 10
            self.model = np.array(links)
            self.G = []

            self.k = 5
            self.T = 10
            self.span = 15
            self.horizon = 1

            self.U = [np.random.rand(self.N,self.k)]
            self.A = np.random.rand(self.N,self.k)
            self.B = np.random.rand(self.k,self.k)

            self.delta = 2**3
            self.gamma = 2**(-5)

            self.save()
            return True
        else:
            return False

    def a_update(self,t,T):
        a_upper = 0
        a_lower = 0

        Utm1 = self.U[t-1]
        G = self.G[t]

        for t in range(1,T):
            a_upper = a_upper + np.matmul(transpose(Utm1),self.U[t])
            a_lower = a_lower + np.matmul(np.matmul(transpose(Utm1),Utm1),self.A)
        self.A = multiply(self.A,a_upper/a_lower)    # Eq. 9


    def b_update(self,t,T):
        b_upper = 0
        b_lower = 0

        G = self.G[t]
        U = self.U[t]
        Yt = [np.zeros(len(G[i])) for i in range(len(G))]
        for i in range(len(G)):
            for j in range(len(G[i])):
                if G[i][j] > 0:
                    Yt[i][j] = 1
        Yt = np.array(Yt)

        for t in range(1,T):
            predict = np.matmul(np.matmul(U,self.B),transpose(U))
            b_upper = b_upper + np.matmul(np.matmul(transpose(U),multiply(Yt,G)),U)
            b_lower = b_lower + np.matmul(np.matmul(transpose(U),multiply(Yt,predict)),U)
        self.B = multiply(self.B,b_upper/b_lower)    # Eq. 8

    def ut_update(self,t):
        # 1. Initializing
        G, U, A, B = self.G[t], self.U, self.A, self.B

        # 2. Defining the latent adjacent attributes
        Ut = U[t]
        Utm1 = U[t-1]
        Utp1 = U[t+1]

        # 3. Defining the Laplacian Parameters
        W = spy_spatial.distance_matrix(G,np.matmul(np.matmul(Ut,B),transpose(Ut)))
        
        D = []
        dim = W.shape[0]
        for i in range(dim):
            D.append(np.zeros(dim))
        D = np.array(D)

        for i in range(dim):
            sum_ = 0
            for j in range(dim):
                sum_ += W[(i,j)]
            D[i][i] = sum_

        # 4. Updating
        u1 =(np.matmul(Ut,transpose(B)) + np.matmul(Ut,B))
        u2 =(np.matmul(Utm1,A)+np.matmul(Utp1,transpose(A)))

        Yt = [np.zeros(len(G[i])) for i in range(len(G))]
        for i in range(len(G)):
            for j in range(len(G[i])):
                if G[i][j] > 0:
                    Yt[i][j] = 1
        Yt = np.array(Yt)

        u_upper = (np.matmul(multiply(Yt,G),u1)) +\
                    (self.delta*np.matmul(W,Ut)) +\
                    (self.gamma*u2)
        predict = np.matmul(np.matmul(Ut,B),transpose(Ut))
        u_lower = np.matmul(\
                    (multiply(Yt,predict)),\
                    (np.matmul(Ut,transpose(B))+ np.matmul(Ut,B))\
                  ) +\
                  (self.delta * np.matmul(D,Ut)) +\
                    self.gamma*(Ut + np.matmul(np.matmul(Ut,A),transpose(A)))
                 
        Ut= (multiply(Ut,(u_upper/u_lower)**(1/4)))   # Eq. 7
        self.U[t] = Ut
        

    def global_learning(self,time,day,month,eps):
        # 1. Initializing
        self.U = []
        self.A = np.random.rand(self.k,self.k)
        self.B = np.random.rand(self.k,self.k)

        STARTX, STARTY = 4,5
        ENDX, ENDY = 6,7
        T = ((day-1)*96)
        for t in range(0,T+1):
            print t
            self.U.append(np.random.rand(self.N,self.k))
            graph = [np.zeros(len(self.model[i])) for i in range(len(self.model))]
            
            for i in range(len(self.links_name)):
                filename = 'all_data/'+self.links_name[i]+month
                
                if os.path.exists(filename):
                    with open(filename) as linkfile:
                        parsed = [line.split(',') for line in linkfile]
                        for row in parsed:
                            row[ENDY] = row[ENDY][:-1]

                        for j in range(len(self.model[i])):
                            if self.model[i][j] > 0 and len(parsed) >= t\
                            and len(parsed[t]) >= 8 and parsed[t][8] != '\n':
                                graph[i][j] = float(parsed[t][8])
            
            self.G.append(np.array(graph))

        # 2. Running
        it = 0
        print '>>',len(self.G)
        while it < 1000:
            print it
            it += 1
            for t in range(1,T):
                self.ut_update(t)
            self.b_update(t,T)
            self.a_update(t,T)

        #print np.matmul(np.matmul(np.matmul(self.U[T-1],self.A),self.B),transpose(np.matmul(self.U[T-1],self.A)))
        print np.matmul(np.matmul(self.U[T],self.B),transpose(self.U[T]))
        print self.G[T]

    def show(self):
        for row in range(2510):
            for col in range(2510):
                print self.G[row][col],' ',
            print ''