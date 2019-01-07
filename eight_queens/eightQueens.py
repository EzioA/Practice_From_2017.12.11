import numpy

N = 8

positions = [[0, 0]]  # 按行存储皇后位置的坐标，每个元素为一个元组
answers = []  # 存储结果，每个元素为一个8x8的棋盘
chessBoard = numpy.zeros(shape=(N, N))  # 初始化棋盘
length = 0  #答案个数，在N大于8时用于记录


def rtGen(row=0, col=0):  # 向右上方向行进的对角线，生成器
    for i in range(1, N):
        if row - i >= 0 and col + i <= N - 1:
            yield (row - i, col + i)
        if row + i <= N - 1 and col - i >= 0:
            yield (row + i, col - i)


def rbGen(row=0, col=0):  # 向右下方向行进的对角线，生成器
    for i in range(1, N):
        if row + i <= N - 1 and col + i <= N - 1:
            yield (row + i, col + i)
        if row - i >= 0 and col - i >= 0:
            yield (row - i, col - i)


def check(index=0):  # index为上一个放置皇后的坐标索引
    global length

    chessBoard[positions[index][0], positions[index][1]] = 1.0
    newRow, newCol = positions[index]
    newRow = newRow + 1  # 从下一行开始检查

    if newRow >= N:  # 找到一副棋盘
        length += 1
        return -1   #伪负1，实际找到了一个答案，并且将其收录

    for newCol in range(0, N):
        flag = True
        for point in positions:
            if newRow == point[0] or newCol == point[1]:  # 新行新列在现有点的直线上
                flag = False
                break
            elif (newRow, newCol) in rtGen(point[0], point[1]) or (newRow, newCol) in rbGen(point[0], point[1]):
                flag = False  # 新行新列在现有点的对角线上
                break

        if flag == True:  # 放点，只有当前newCol不在任何对角线和行列上时才能为True
                positions.append([newRow, newCol])

                temp = check(len(positions) - 1)
                if temp == -1:  # 向下一层就走不通
                    positions.pop()  # 排除当前点
                    chessBoard[newRow, newCol] = 0  # 当前点置0
                    if newCol == N - 1: #删除的节点已经是该行最后一个节点
                        return -1
                    continue    #在当前层继续遍历
                else:   #找到0，整条路径通顺
                    return 0

        elif newCol == N - 1:  # 直到最后一列都没找到可放置的点
            return -1

def main():
    i = 0
    while i < N:
        chessBoard[:, :] = 0
        positions[0][1] = i
        check(0)
        i += 1



if __name__ == '__main__':
    main()
    pass
