import random   


MIN = -1000
MAX =  1000
CNT =  50_000

name = "3"

lis = list()

with open("./unit/" + name + "/" + name + ".in", "w") as tape_file:
    for i in range(CNT):
        num = random.randrange(MIN, MAX)
        print(num, file=tape_file, end="  ")
        lis.append(num)
        if ((i + 1) % 20 == 0):
            print(file=tape_file)

lis.sort()
with open("./unit/" + name + "/" + name + ".ans", "w") as tape_file:
    for i in range(CNT):
        print(lis[i], file=tape_file)
