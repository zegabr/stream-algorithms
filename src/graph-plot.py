# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
import csv

################### DELTA X TIME ##################
def readCsvTimeDeltaGraph(target):
    with open('../kmvResult.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        content = []

        content.append(["delta", "time"])

        for row in csv_reader:
            if line_count > 0:
                curColumn = row[0]
                if curColumn == str(target):
                    content.append([str(row[2]), str(row[10])])
            line_count += 1

    print(len(content))
    return content

def writeCsvDeltaTimeGraph(target, content):
    with open('delta-time-{}.csv'.format(target), mode='w') as resultCsv:
        writer = csv.writer(resultCsv, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

        for row in content:
            writer.writerow(row)


################## EPS X K ######################
def readCsvEpsKGraph(target):
    with open('./kmvResult.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        content = []

        content.append(["eps", "k"])

        for row in csv_reader:
            if line_count > 0:
                curColumn = row[0]
                if curColumn == str(target):
                    content.append([str(row[1]), str(row[9])])
            line_count += 1

    print(len(content))
    return content

def writeCsvEpsKGraph(target, content):
    with open('eps-k-{}.csv'.format(target), mode='w') as resultCsv:
        writer = csv.writer(resultCsv, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

        for row in content:
            writer.writerow(row)


################## DELTA, ESTIMATE, MIN ACCEPTABLE, MAX ACCEPTABLE ######################
def readCsvDeltaEstimatesGraph(target):
    with open('./kmvResult.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        content = []

        content.append(["delta", "estimate", "min acceptable value", "max acceptable value"])

        for row in csv_reader:
            if line_count > 0:
                curColumn = row[0]
                if curColumn == str(target):
                    content.append([str(row[2]), str(row[3]), str(row[5]), str(row[6])])
            line_count += 1

    print(len(content))
    return content

def writeCsvDeltaEstimatesGraph(target, content):
    with open('delta-estimates-{}.csv'.format(target), mode='w') as resultCsv:
        writer = csv.writer(resultCsv, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

        for row in content:
            writer.writerow(row)

################## EPS, DELTA, ESTIMATE, MIN ACCEPTABLE, MAX ACCEPTABLE ######################
def readCsvEpsDeltaEstimatesGraph(target):
    with open('./kmvResult.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        content = []

        content.append(["eps", "delta", "estimate", "min acceptable value", "max acceptable value"])

        for row in csv_reader:
            if line_count > 0:
                curColumn = row[0]
                if curColumn == str(target):
                    content.append([str(row[1]), str(row[2]), str(row[3]), str(row[5]), str(row[6])])
            line_count += 1

    print(len(content))
    return content

def writeCsvEpsDeltaEstimatesGraph(target, content):
    with open('eps-delta-estimates-{}.csv'.format(target), mode='w') as resultCsv:
        writer = csv.writer(resultCsv, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

        for row in content:
            writer.writerow(row)

################## DELTA X NUM SKETCHES ######################
def readCsvDeltaSketchesGraph(target):
    with open('./kmvResult.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        content = []

        content.append(["delta", "number of sketches"])

        for row in csv_reader:
            if line_count > 0:
                curColumn = row[0]
                if curColumn == str(target):
                    content.append([str(row[2]), str(row[8])])
            line_count += 1

    print(len(content))
    return content

def writeCsvDeltaSketchesGraph(target, content):
    with open('delta-sketches-{}.csv'.format(target), mode='w') as resultCsv:
        writer = csv.writer(resultCsv, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

        for row in content:
            writer.writerow(row)

target = 0
content = readCsvDeltaSketchesGraph(target)
writeCsvDeltaSketchesGraph(target, content)