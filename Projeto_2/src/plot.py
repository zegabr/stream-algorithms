from matplotlib import pyplot as plt
import numpy as np

epsVals = [0.05, 0.1, 0.15, 0.2]
univVals = [100, 1000, 10000]
columns = [4, 5, 6, 7]
queryTypes = ["rank", "quant"]


def plotXYGraph(x, y, plotName, xLabel, yLabel, filepath):
    print(x, "VS", y)
    plt.title(plotName)
    plt.xlabel(xLabel)
    plt.ylabel(yLabel)
    plt.plot(x, y, color="#ff6770")
    plt.savefig(filepath)
    plt.close()


def plotXYGraphWithBounds(x, y, minY, maxY, plotName, xLabel, yLabel, filepath):
    maxYval = 0
    minYval = 1000000000000

    for yVal in minY:
        if yVal < minYval and yVal >= 0:
            minYval = yVal

    for yVal in maxY:
        if yVal > maxYval:
            maxYval = yVal

    if minYval == maxYval:
        maxYval += 1

    plt.title(plotName)
    plt.xlabel(xLabel)
    plt.ylabel(yLabel)

    ax = plt.gca()
    ax.set_title(plotName, pad=20)

    plt.plot(x, y, color="#009ddf")

    print([x[0], x[-1], minYval, maxYval])
    plt.axis([x[0], x[-1], minYval, maxYval])

    plt.fill_between(x, minY, maxY, alpha=0.2, edgecolor='#ff0084', facecolor='#ff0084',
                     linewidth=1, linestyle='dashdot', antialiased=True)

    plt.savefig(filepath)
    plt.close()


def plotMemoryGraphs():
    for qType in queryTypes:
        for column in columns:
            filename = f"plotCsv/memory-time/{qType}-{column}.csv"

            for univ in univVals:
                x = []
                y = []
                plotName = f"{qType} variant with univ = {univ} for column {column}"
                xLabel = "Eps"
                yLabel = "Memory in bytes"
                filepath = f"qdigestPlots/memory/{qType}-{column}/univ-{univ}.png"

                lineCount = 0
                with open(filename) as reader:
                    for line in reader.readlines():
                        if lineCount > 0:
                            values = line.split(',')
                            eps = float(values[1])
                            univL = int(values[2])

                            if univL == univ:
                                memory = int(values[5]) / 8
                                x.append(eps)
                                y.append(memory)
                        lineCount += 1

                plotXYGraph(x, y, plotName, xLabel, yLabel, filepath)


def plotTimeGraphs():
    for qType in queryTypes:
        for column in columns:
            filename = f"plotCsv/memory-time/{qType}-{column}.csv"

            for univ in univVals:
                x = []
                y = []
                plotName = f"{qType} variant with univ = {univ} for column {column}"
                xLabel = "Eps"
                yLabel = "Time in seconds"
                filepath = f"qdigestPlots/time/{qType}-{column}/univ-{univ}.png"

                lineCount = 0
                with open(filename) as reader:
                    for line in reader.readlines():
                        if lineCount > 0:
                            values = line.split(',')
                            eps = float(values[1])
                            univL = int(values[2])

                            if univL == univ:
                                time = float(values[4])
                                x.append(eps)
                                y.append(time)
                        lineCount += 1

                plotXYGraph(x, y, plotName, xLabel, yLabel, filepath)


def plotQueryGraphs():
    for qType in queryTypes:
        for column in columns:
            filename = f"plotCsv/queries/{qType}-{column}.csv"

            for univ in univVals:
                for eps in epsVals:
                    x = []
                    y = []
                    yMin = []
                    yMax = []

                    plotName = f"{qType} variant with univ = {univ} and eps = {eps} for column {column}"
                    yLabel = ""

                    if qType == "rank":
                        yLabel = "Rank(x)"
                    else:
                        yLabel = "Quantile(x)"

                    xLabel = "x (query)"
                    filepath = f"qdigestPlots/queries/{qType}-{column}/univ-{univ}-eps-{eps}.png"

                    lineCount = 0
                    with open(filename) as reader:
                        for line in reader.readlines():
                            if lineCount > 0:
                                values = line.split(',')
                                epsL = float(values[1])
                                univL = int(values[2])

                                if univL == univ and epsL == eps:
                                    query = 0

                                    if qType == "rank":
                                        query = int(values[6])
                                    else:
                                        query = float(values[6])

                                    estimate = int(values[7])
                                    minAcceptable = int(values[10])
                                    maxAcceptable = int(values[11])
                                    x.append(query)
                                    y.append(estimate)
                                    yMin.append(minAcceptable)
                                    yMax.append(maxAcceptable)

                            lineCount += 1

                    plotXYGraphWithBounds(
                        x, y, yMin, yMax, plotName, xLabel, yLabel, filepath)


plotQueryGraphs()
