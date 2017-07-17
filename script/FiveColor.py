# encoding=utf-8

import os
import os.path

genres = ['抽象派', '点彩派', '后印象派', '印象派', '至上主义', '水墨']
rootdir = 'dataset_simplify'


def deepSearch(filename):
    filename = filename.encode('utf-8')
    filename = unicode(filename, 'utf-8')
    files = os.listdir(filename)
    for f in files:
        f = filename + '\\' + f
        if os.path.isdir(f):
            deepSearch(f)
        if os.path.isfile(f):
            allData.append(txtAnalysis(filename))
            break


def txtAnalysis(filename):
    files = os.listdir(filename)
    attrDict = {}
    for f in files:
        temp = '_cp.txt'
        if temp in f:
            paintName = f[0:-7]
            attrDict[paintName] = []
    for key in attrDict:
        temp = filename + '\\' + key + '_cp.txt'
        fopen = open(temp, 'r')
        line = fopen.readline()
        fopen.close()
        line = line.replace('\n', '')
        line = line.split(' ')
        if len(line) != 23:
            for i in range(0, 15):
                attrDict[key].append(-1)
        else:
            for i in range(7, 22):
                attrDict[key].append(line[i])

    return attrDict


def preProcess(colors):
    ans = []
    for i in range(5):
        r = float(colors[i * 3]) / 255
        g = float(colors[i * 3 + 1]) / 255
        b = float(colors[i * 3 + 2]) / 255
        maxc = max(r, g, b)
        minc = min(r, g, b)
        h = 0
        if maxc != minc:
            if maxc == r:
                h = (g - b) / (maxc - minc)
            if maxc == g:
                h = 2 + (b - r) / (maxc - minc)
            if maxc == b:
                h = 4 + (r - g) / (maxc - minc)
        h *= 60
        if h < 0:
            h += 360
        v = maxc * 100
        if maxc == 0:
            s = 0
        else:
            s = (maxc - minc) / maxc * 100

        div = int(h / 30)
        if div == 12:
            div = 0
        if 10 <= v < 50 and 15 <= s < 60:
            ans.append(div * 6 + 1)
        if 50 <= v < 80 and 10 <= s < 60:
            ans.append(div * 6 + 2)
        if 80 <= v and 5 <= s < 60:
            ans.append(div * 6 + 3)
        if 10 <= v < 50 and 60 <= s:
            ans.append(div * 6 + 4)
        if 50 <= v < 80 and 60 <= s:
            ans.append(div * 6 + 5)
        if 80 <= v and 60 <= s:
            ans.append(div * 6 + 6)
        if v < 10:
            ans.append(73)
        if 10 <= v < 50 and s < 15:
            ans.append(74)
        if 50 <= v < 80 and s < 10:
            ans.append(75)
        if 80 <= v < 95 and s < 5:
            ans.append(76)
        if 95 <= v and s < 5:
            ans.append(77)

    return ans


def postProcess(cats):
    ans = []
    for i in range(len(cats)):
        h = s = v = 0.0
        if cats[i] <= 72:
            div = int((cats[i] - 1) / 6)
            h = div * 30 + 15
            temp = cats[i] - div * 6
            if temp == 1:
                s, v = [37.5, 30]
            if temp == 2:
                s, v = [35, 65]
            if temp == 3:
                s, v = [32.5, 90]
            if temp == 4:
                s, v = [80, 30]
            if temp == 5:
                s, v = [80, 62.5]
            if temp == 6:
                s, v = [80, 90]
        if cats[i] == 73:
            h, s, v = [180, 0, 5]
        if cats[i] == 74:
            h, s, v = [180, 7.5, 30]
        if cats[i] == 75:
            h, s, v = [180, 5, 65]
        if cats[i] == 76:
            h, s, v = [180, 2.5, 87.5]
        if cats[i] == 77:
            h, s, v = [180, 2.5, 97.5]

        h = float(h)
        s = float(s)
        v = float(v)
        if s == 0:
            R = G = B = v
        else:
            h = h / 60
            if h == 6.0:
                h = 0.0
            i = int(h)
        f = h - i
        a = v / 100 * (1 - s / 100)
        b = v / 100 * (1 - f * s / 100)
        c = v / 100 * (1 - (1 - f) * s / 100)
        R = G = B = 0.0
        if i == 0:
            R, G, B = [v/100, c, a]
        if i == 1:
            R, G, B = [b, v/100, a]
        if i == 2:
            R, G, B = [a, v/100, c]
        if i == 3:
            R, G, B = [a, b, v/100]
        if i == 4:
            R, G, B = [c, a, v/100]
        if i == 5:
            R, G, B = [v/100, a, b]
        ans.append(int(R * 256))
        ans.append(int(G * 256))
        ans.append(int(B * 256))

    return ans


def subs(u, num):
    ans = []
    dict_num = {}
    for i in u:
        if i not in dict_num.keys():
            dict_num[i] = u.count(i)
    sort_dict = sorted(dict_num.iteritems(), key=lambda x:x[1], reverse=True)
    for i in range(num):
        if len(sort_dict) > i:
            ans.append(sort_dict[i][0])

    return ans


def lastt(allFive, a, b):
    if a == 0 or b == 0:
        return []

    someFive = []
    for item in allFive:
        if item[0] == a and item[1] == b:
            someFive.append(item)

    B = []
    for item in someFive:
        B.append(item[2])
        B.append(item[3])
        B.append(item[4])
    temp = subs(B, 1)
    if len(temp) == 0:
        return []
    c = temp[0]
    C = []
    for item in someFive:
        if c in item:
            if item[2] != c:
                C.append(item[2])
            if item[3] != c:
                C.append(item[3])
            if item[4] != c:
                C.append(item[4])
    temp = subs(C, 1)
    if len(temp) == 0:
        return []
    d = temp[0]
    D = []
    for item in someFive:
        if c in item and d in item:
            if item[2] != c and item[2] != d:
                D.append(item[2])
            if item[3] != c and item[3] != d:
                D.append(item[3])
            if item[4] != c and item[4] != d:
                D.append(item[4])
    temp = subs(D, 1)
    if len(temp) == 0:
        return []
    e = temp[0]

    return [a, b, c, d, e]


def select(allFive):
    ans = []

    A0 = []
    for item in allFive:
        A0.append(item[0])
    a = [0, 0, 0]
    temp = subs(A0, 3)
    for i in range(len(temp)):
        a[i] = temp[i]
    A = [[], [], []]
    for item in allFive:
        for i in range(3):
            if item[0] == a[i]:
                A[i].append((item[1]))
    b = [0, 0, 0, 0, 0, 0]
    for i in range(3):
        temp = subs(A[i], 2)
        for j in range(len(temp)):
            b[i*2+j] = temp[j]

    for i in range(3):
        for j in range(2):
            ans.append(lastt(allFive, a[i], b[i*2+j]))

    return ans


def superSerach(five, allFullFive):
    if len(five) == 0:
        return []
    for item in allFullFive:
        flag = True
        temp = preProcess(item[1])
        for i in range(5):
            if five[i] not in temp:
                flag = False
                break
        if flag:
            return item

    return []

for name in genres:
    temp = rootdir + '\\' + name
    temp = unicode(temp, 'utf-8')
    allData = []
    deepSearch(temp)
    print name

    allFive = []
    allFullFive = []
    for item in allData:
        for key, value in item.items():
            allFullFive.append([key, value])
            five = preProcess(value)
            allFive.append(five)

    allSelectedFive = select(allFive)
    print allSelectedFive

    allSelectedRGB = []
    for item in allSelectedFive:
        allSelectedRGB.append(superSerach(item, allFullFive))
        # allSelectedRGB.append(postProcess(item))
    print allSelectedRGB

    filename = 'selectedFiveColors.txt'
    fopen = open(filename, 'a')
    fopen.write(name)
    fopen.write('\n')
    for item in allSelectedRGB:
        fopen.write(str(item))
        fopen.write('\n')
    fopen.close()
