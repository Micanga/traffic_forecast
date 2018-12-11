from lsmrn import LSMRN
from numpy import array, multiply, transpose
import sklearn.decomposition as sk_dec


def main():
    # 1. Loading the LSM-RN model
    model = LSMRN()
    model = model.load()
    model.global_learning(0,22,'Jan',0.001)

if __name__ == "__main__":
    main()