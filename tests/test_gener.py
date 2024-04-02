import random   


MIN = -1
MAX =  1
CNT =  500_000

name = "11"

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
