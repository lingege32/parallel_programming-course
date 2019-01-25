#!/uhome/home/lingege32/local/python-3.6.3/bin/python3
import os,sys
if len(sys.argv)!=2 :
    commandMake  = 'make DATASIZE=LARGE'
else:
    commandMake  = 'make DATASIZE='+sys.argv[1]

    #quit()
commandClean = 'make clean'

#commandExe = 'taskset -c 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 ./bin/cg > out.txt'
commandExe = 'taskset -c 0,1,2,3 ./bin/cg'
#commandExe = './bin/cg > out.txt'

print(commandClean)
print(commandMake)
print(commandExe)

#os.system(commandClean)
#os.system(commandMake)
#os.system(commandExe)
os.system("rm -rf out10.txt")
#for i in range(10):
    #os.system(commandExe)
os.system("cp cg_good2.c cg.c")
os.system(commandClean)
os.system(commandMake)
#os.system("rm -rf out11.txt")
for i in range(50):
    print("------->Iteration "+str(i+1)+"  <--------")
    print("------->Iteration "+str(i+1)+"  <--------")
    print("------->Iteration "+str(i+1)+"  <--------")
    print("------->Iteration "+str(i+1)+"  <--------")
    print("------->Iteration "+str(i+1)+"  <--------")
    os.system(commandExe)
