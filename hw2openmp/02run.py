#!/uhome/home/lingege32/local/python-3.6.3/bin/python3
file = open("out10.txt","r")
ans=0;
init=0;
filetotal=file.readlines()
for i in range(50):
    ans += float(filetotal[i].split(" ")[-2])
    init += float(filetotal[i].split(" ")[-7])

print(ans/50)
print(init/50)
