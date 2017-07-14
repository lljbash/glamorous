# encoding=utf-8

import os
import os.path

genres = ['abstract', 'pointillism', 'post_impressionism', 'impressionism', 'suprematism', 'shuimo']
attrs = ['cp', 'fg', 'shape']
rootdir = 'data'


def deepSearch(filename):
    filename = filename.encode('utf-8')
    filename = unicode(filename, 'utf-8')
    files = os.listdir(filename)
    for f in files:
        f = filename + '/' + f
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
            paintName = filename + '/' + f
            attrDict[paintName] = []
    for key in attrDict:
        for attr in attrs:
            temp = key[0:-7] + '_' + attr + '.txt'
            if os.path.exists(temp):
                fopen = open(temp, 'r')
                line = fopen.readline()
                fopen.close()
                line = line.replace('\n', '')
                line = line.split(' ')
            else:
                line = []
            attrDict[key].append(attr)
            if attr == 'cp':
                if len(line) != 23:
                    for i in range(0, 6):
                        attrDict[key].append('-1')
                else:
                    for i in range(1, 22):
                        attrDict[key].append(line[i])
            if attr == 'fg':
                if len(line) != 4:
                    for i in range(0, 4):
                        attrDict[key].append('-1')
                else:
                    for i in range(0, 4):
                        attrDict[key].append(line[i])
            if attr == 'shape':
                if len(line) != 17:
                    for i in range(0, 2):
                        attrDict[key].append('-1')
                else:
                    for i in range(1, 17):
                        attrDict[key].append(line[i])

    return attrDict


def outputCSV(name, data):
    name = unicode(name, 'utf-8')
    filename = name + '.tsv'
    fopen = open(rootdir + '/' + filename, 'a')
    for key, value in data.items():
        temp = key.encode('utf-8')
        temp = temp[5:-7]
        if os.path.exists(rootdir + '/' + temp + '.jpg'):
            temp += '.jpg'
        else:
            if os.path.exists(rootdir + '/' + temp + '.jpeg'):
                temp += '.jpeg'
            else:
                if os.path.exists(rootdir + '/' + temp + '.JPG'):
                    temp += '.JPG'
                else:
                    if os.path.exists(rootdir + '/' + temp + '.png'):
                        temp += '.png'
                    else:
                        llj_bash = 250
        fopen.write(temp)
        for i in range(0, len(value)):
            fopen.write('\t')
            fopen.write(value[i])
        fopen.write('\n')
    fopen.close()

for i in range(0, 6):
    name = genres[i]
    temp = rootdir + '/' + name
    temp = unicode(temp, 'utf-8')
    allData = []
    deepSearch(temp)
    print name
    for item in allData:
        outputCSV(name, item)
