from random import randint


def compare_states(state1, state2, n):
    for i in range(n):
        if state1[i] != state2[i]:
            return False
    return True


def fitness(board, state, n):
    # column 'i' row 'state[i]'
    attacking = 0
    for i in range(n):
        # To the left of same row
        row = state[i]
        col = i - 1
        while col >= 0 and board[row][col] != 1:
            col -= 1

        if col >= 0 and board[row][col] == 1:
            attacking += 1

        # To the right of same row
        row = state[i]
        col = i + 1
        while col < n and board[row][col] != 1:
            col += 1

        if col < n and board[row][col] == 1:
            attacking += 1

        # Diagonally to the left up
        row = state[i] - 1
        col = i - 1
        while col >= 0 and row >= 0 and board[row][col] != 1:
            col -= 1
            row -= 1

        if col >= 0 and row >= 0 and board[row][col] == 1:
            attacking += 1

        # Diagonally to the right down
        row = state[i] + 1
        col = i + 1
        while col < n and row < n and board[row][col] != 1:
            col += 1
            row += 1

        if col < n and row < n and board[row][col] == 1:
            attacking += 1

        # Diagonally to the left down
        row = state[i] + 1
        col = i - 1
        while col >= 0 and row < n and board[row][col] != 1:
            col -= 1
            row += 1
        if col >= 0 and row < n and board[row][col] == 1:
            attacking += 1

        # Diagonally to the right up
        row = state[i] - 1
        col = i + 1
        while col < n and row >= 0 and board[row][col] != 1:
            col += 1
            row -= 1

        if col < n and row >= 0 and board[row][col] == 1:
            attacking += 1

    return int(attacking / 2)


def get_neighbour(board, state, n):
    opState = state.copy()
    opBoard = board.copy()
    opObjective = fitness(opBoard, opState, n)
    NeighbourBoard = board.copy()
    NeighbourState = state.copy()
    for i in range(n):
        for j in range(n):
            if j != state[i]:
                NeighbourState[i] = j
                NeighbourBoard[NeighbourState[i]][i] = 1
                NeighbourBoard[state[i]][i] = 0
                temp = fitness(NeighbourBoard, NeighbourState, n)
                if temp <= opObjective:
                    opObjective = temp
                    opState = NeighbourState.copy()
                    opBoard = NeighbourBoard.copy()
                NeighbourBoard[NeighbourState[i]][i] = 0
                NeighbourState[i] = state[i]
                NeighbourBoard[state[i]][i] = 1
    return opState, opBoard


def hill_climbing(board, state, n):
    end = 0
    neighbourState = state.copy()
    neighbourBoard = board.copy()

    while True:
        state = neighbourState.copy()
        board = neighbourBoard.copy()

        neighbourState, neighbourBoard = get_neighbour(neighbourBoard, neighbourState, n)

        # if compare_states(state, neighbourState, n):
        #     for i in range(len(neighbourState)):
        #         print(board[i])
        #     break
        #
        # elif fitness(board, state, n) == fitness(neighbourBoard, neighbourState, n):
        #     #scape local extremum
        #     neighbourState[randint(0, 100000) % n] = randint(0, 100000) % n
        #     for i in range(n):
        #         neighbourBoard[neighbourState[i]][i] = 1

        # print(fitness(board, state, n))
        if compare_states(state, neighbourState, n):
            if fitness(board, state, n) == 0:
                print("Right Answer")
                for i in range(len(neighbourState)):
                    print(board[i])
                break
            else:
                if end == 100:
                    print("Wrong but Best Answer")
                    for i in range(len(neighbourState)):
                        print(board[i])
                    break
                else:
                    end += 1
        if fitness(board, state, n) == fitness(neighbourBoard, neighbourState, n):
            #scape local extremum
            neighbourState[randint(0, 100000) % n] = randint(0, 100000) % n
            for i in range(n):
                neighbourBoard[neighbourState[i]][i] = 1


# n = int(input("Enter n: "))
n = 8
state = [0] * n
board = [[0 for i in range(n)] for j in range(n)]
for i in range(n):
    state[i] = randint(0, 100000) % n
    board[state[i]][i] = 1
hill_climbing(board, state, n)
