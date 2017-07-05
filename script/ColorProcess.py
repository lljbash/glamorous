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
            for i in range(0, 6):
                attrDict[key].append(-1)
        else:
            for i in range(1, 7):
                attrDict[key].append(line[i])

    return attrDict

DIV = 10  # Divided parts of [0,1]
for name in genres:
    temp = rootdir + '\\' + name
    temp = unicode(temp, 'utf-8')
    allData = []
    saturation = []
    saturationContrast = []
    brightness = []
    brightnessContrast = []
    warmOrCool = []
    clearOrDull = []
    deepSearch(temp)
    print name
    for item in allData:
        for key, value in item.items():
            saturation.append(float(value[0]))
            saturationContrast.append(float(value[1]))
            brightness.append(float(value[2]))
            brightnessContrast.append(float(value[3]))
            warmOrCool.append(float(value[4]))
            clearOrDull.append(float(value[5]))

    name = unicode(name, 'utf-8')
    filename = name + '.txt'
    fopen = open(filename, 'w')

    fopen.write('Zhong Shu:\n')
    counter = [0 for i1 in range(0, DIV)]
    for shuju in saturation:
        if shuju == 1.0:
            shuju = 0.99
        counter[int(shuju / (1.0 / DIV))] += 1
    maxValue = 0
    maxPos = 0
    for pos in range(len(counter)):
        if maxValue < counter[pos]:
            maxValue = counter[pos]
            maxPos = pos
    fopen.write('saturation: ' + str(1.0 / DIV * maxPos + 0.5 / DIV) + '\n')
    counter = [0 for i2 in range(0, DIV)]
    for shuju in saturationContrast:
        if shuju == 1.0:
            shuju = 0.99
        counter[int(shuju / (1.0 / DIV))] += 1
    maxValue = 0
    maxPos = 0
    for pos in range(len(counter)):
        if maxValue < counter[pos]:
            maxValue = counter[pos]
            maxPos = pos
    fopen.write('saturationContrast: ' + str(1.0 / DIV * maxPos + 0.5 / DIV) + '\n')
    counter = [0 for i3 in range(0, DIV)]
    for shuju in brightness:
        if shuju == 1.0:
            shuju = 0.99
        counter[int(shuju / (1.0 / DIV))] += 1
    maxValue = 0
    maxPos = 0
    for pos in range(len(counter)):
        if maxValue < counter[pos]:
            maxValue = counter[pos]
            maxPos = pos
    fopen.write('brightness: ' + str(1.0 / DIV * maxPos + 0.5 / DIV) + '\n')
    counter = [0 for i4 in range(0, DIV)]
    for shuju in brightnessContrast:
        if shuju == 1.0:
            shuju = 0.99
        counter[int(shuju / (1.0 / DIV))] += 1
    maxValue = 0
    maxPos = 0
    for pos in range(len(counter)):
        if maxValue < counter[pos]:
            maxValue = counter[pos]
            maxPos = pos
    fopen.write('brightnessContrast: ' + str(1.0 / DIV * maxPos + 0.5 / DIV) + '\n')
    counter = [0 for i5 in range(0, DIV)]
    for shuju in warmOrCool:
        if shuju == 1.0:
            shuju = 0.99
        counter[int(shuju / (1.0 / DIV))] += 1
    maxValue = 0
    maxPos = 0
    for pos in range(len(counter)):
        if maxValue < counter[pos]:
            maxValue = counter[pos]
            maxPos = pos
    fopen.write('warmOrCool: ' + str(1.0 / DIV * maxPos + 0.5 / DIV) + '\n')
    counter = [0 for i6 in range(0, DIV)]
    for shuju in clearOrDull:
        if shuju == 1.0:
            shuju = 0.99
        counter[int(shuju / (1.0 / DIV))] += 1
    maxValue = 0
    maxPos = 0
    for pos in range(len(counter)):
        if maxValue < counter[pos]:
            maxValue = counter[pos]
            maxPos = pos
    fopen.write('clearOrDull: ' + str(1.0 / DIV * maxPos + 0.5 / DIV) + '\n')

    fopen.write('\nZhong Wei Shu:\n')
    temp = sorted(saturation)
    fopen.write('saturation: ' + str(temp[len(saturation) / 2]) + '\n')
    temp = sorted(saturationContrast)
    fopen.write('saturationContrast: ' + str(temp[len(saturationContrast) / 2]) + '\n')
    temp = sorted(brightness)
    fopen.write('brightness: ' + str(temp[len(brightness) / 2]) + '\n')
    temp = sorted(brightnessContrast)
    fopen.write('brightnessContrast: ' + str(temp[len(brightnessContrast) / 2]) + '\n')
    temp = sorted(warmOrCool)
    fopen.write('warmOrCool: ' + str(temp[len(warmOrCool) / 2]) + '\n')
    temp = sorted(clearOrDull)
    fopen.write('clearOrDull: ' + str(temp[len(clearOrDull) / 2]) + '\n')

    fopen.close()
