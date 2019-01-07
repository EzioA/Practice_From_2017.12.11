def conflict(state, nextx):
    nexty = len(state)
    for i in range(nexty):
        if abs(state[i] - nextx) in (0, nexty - i):
            return True
    return False

def queens(num = 8, state=()):
    for pos in range(num):
        if not conflict(state, pos):
            if len(state) == num - 1:
                yield (pos, )
            else:
                for result in queens(num, state + (pos, )):
                    yield (pos, ) + result

if __name__ == '__main__':
    path = list(queens())
    print(len(path))
