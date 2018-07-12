from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""

    table = []

    for buffer in range(len(a) + 1):
        table.append([0] * (len(b) + 1))

    table[0][0] = 0, None

    for col in range(1, len(b) + 1):
        table[0][col] = col, Operation.INSERTED

    for row in range(1, len(a) + 1):
        table[row][0] = row, Operation.DELETED

    for row in range(1, (len(a) + 1)):
        for col in range(1, (len(b) + 1)):
            delBuf = table[row - 1][col][0] + 1
            insBuf = table[row][col - 1][0] + 1
            subBuf = table[row - 1][col - 1][0]

            if (a[row - 1] != b[col - 1]):
                subBuf += 1

            if (delBuf <= insBuf and delBuf <= subBuf):
                buf = delBuf, Operation.DELETED
            elif (insBuf <= subBuf):
                buf = insBuf, Operation.INSERTED
            else:
                buf = subBuf, Operation.SUBSTITUTED

            table[row][col] = buf

    return table