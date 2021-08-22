# MEMORIA PRA CADA VARIACAO DOS GRAFICOS DE RANK
epsVals = [0.05, 0.1, 0.15, 0.2]
univVals = [100, 1000, 10000]
columns = [4, 5, 6, 7]
queryTypes = ["rank", "quant"]


def genMemoryCsvs():
    for qType in queryTypes:
        for column in columns:
            filename = f"{qType}-{column}.csv"

            neededlines = []

            with open(filename) as reader:
                for line in reader.readlines():
                    values = line.split(',')
                    memory = values[5]
                    if(memory):
                        neededlines.append(line)

            with open(f"plotCsv/memory-time/{filename}", "w") as writer:
                for line in neededlines:
                    writer.write(line)


def genQueryCsvs():
    for qType in queryTypes:
        for column in columns:
            filename = f"{qType}-{column}.csv"

            neededlines = []

            with open(filename) as reader:
                for line in reader.readlines():
                    values = line.split(',')
                    estimate = values[7]
                    if(estimate):
                        neededlines.append(line)

            with open(f"plotCsv/queries/{filename}", "w") as writer:
                for line in neededlines:
                    writer.write(line)


genMemoryCsvs()
genQueryCsvs()
