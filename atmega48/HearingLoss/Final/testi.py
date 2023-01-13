import matplotlib.pyplot as plt

resistances = [100, 220, 480, 1000, 2200, 4800, 10000, 22000, 48000, 100000]
def matchRes( target ):
    result = 0
    for r in resistances[::-1]:
        if target >= r:
            target -= r
            result += r
    return result

plt.plot([matchRes(i) for i in range(0, 200000, 50)])

plt.show()
    