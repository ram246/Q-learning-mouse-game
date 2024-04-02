#import csv

# read flash.dat to a list of lists
#datContent = [i.strip().split() for i in open("./QTable_0008.dat").readlines()]
print (open("./QTable_0008.dat", errors='ignore').readlines())

# write it as a new CSV file
'''
with open("./QTable_0008.csv", "wb") as f:
    writer = csv.writer(f)
    writer.writerows(datContent)
    '''